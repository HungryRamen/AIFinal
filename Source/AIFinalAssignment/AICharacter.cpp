// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "AIAIController.h"
// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	SK_REDWARRIOR = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_solid.SK_CharM_solid'")).Object;
	SK_BLUEWARRIOR = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant'")).Object;
	SK_REDWEAPON = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_AnthraciteAxe/SK_Blade_AnthraciteAxe.SK_Blade_AnthraciteAxe'")).Object;
	SK_BLUEWEAPON = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_IceElemental/SK_Blade_IceElemental.SK_Blade_IceElemental'")).Object;
	SetTeam(CurrentTeam);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/AIFinal/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(EControlMode::Player);

	IsAttacking = false;
	MaxCombo = 3;
	AttackRange = 200.0f;
	AttackRadius = 50.0f;
	AttackDamage = 10.0f;
	HitPoint = 30.0f;
	CurrentDestroyTime = 0.0f;
	AttackEndComboState();
	AIControllerClass = AAIAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case AAICharacter::EControlMode::Player:
		SpringArm->TargetArmLength = 800.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case AAICharacter::EControlMode::AI:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	default:
		break;
	}
}

void AAICharacter::SetTeamChange(ETeam NewTeam)
{
	CurrentTeam = NewTeam;
	switch (CurrentTeam)
	{
	case ETeam::Red:
		GetMesh()->SetSkeletalMesh(SK_REDWARRIOR);
		Weapon->SetSkeletalMesh(SK_REDWEAPON);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("RedCharacter"));
		break;
	case ETeam::Blue:
		GetMesh()->SetSkeletalMesh(SK_BLUEWARRIOR);
		Weapon->SetSkeletalMesh(SK_BLUEWEAPON);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("BlueCharacter"));
		break;
	default:
		break;
	}
}

void AAICharacter::SetTeam(ETeam NewTeam)
{
	CurrentTeam = NewTeam;
	FName WeaponSocket(TEXT("hand_rSocket"));
	switch (CurrentTeam)
	{
	case ETeam::Red:
		if (SK_REDWARRIOR != nullptr)
		{
			GetMesh()->SetSkeletalMesh(SK_REDWARRIOR);
		}

		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
			if (SK_REDWEAPON != nullptr)
			{
				Weapon->SetSkeletalMesh(SK_REDWEAPON);
			}
			Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		}
		break;
	case ETeam::Blue:
		if (SK_BLUEWARRIOR!= nullptr)
		{
			GetMesh()->SetSkeletalMesh(SK_BLUEWARRIOR);
		}

		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
			if (SK_BLUEWEAPON != nullptr)
			{
				Weapon->SetSkeletalMesh(SK_BLUEWEAPON);
			}
			Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		}
		break;
	default:
		return;
		break;
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (CurrentControlMode)
	{
	case AAICharacter::EControlMode::Player:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	case AAICharacter::EControlMode::AI:
		break;
	default:
		break;
	}
	if (HitPoint <= 0.0f)
	{
		CurrentDestroyTime += DeltaTime;
		if (CurrentDestroyTime >= 3.0f)
		{
			Destroy();
			return;
		}
	}
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIAnim = Cast<UAIAnimInstance>(GetMesh()->GetAnimInstance());
	AIAnim->OnMontageEnded.AddDynamic(this, &AAICharacter::OnAttackMontageEnded);
	AIAnim->OnNextAttackCheck.AddLambda([this]() -> void
	{
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			AIAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	AIAnim->OnAttackHitCheck.AddUObject(this, &AAICharacter::AttackCheck);
}

float AAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	HitPoint -= FinalDamage;
	if (HitPoint <= 0.0f)
	{
		AIAnim->SetDeadAnim();
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	}
	return FinalDamage;
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (IsPlayerControlled())
	{
		SetControlMode(AAICharacter::EControlMode::Player);
	}
	else
	{
		SetControlMode(AAICharacter::EControlMode::AI);

	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AAICharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AAICharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAICharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAICharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AAICharacter::Attack);
}

void AAICharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AAICharacter::EControlMode::Player:
		DirectionToMove.X = NewAxisValue;
		break;
	case AAICharacter::EControlMode::AI:
		break;
	default:
		break;
	}
}

void AAICharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AAICharacter::EControlMode::Player:
		DirectionToMove.Y = NewAxisValue;
		break;
	case AAICharacter::EControlMode::AI:
		break;
	default:
		break;
	}
}

void AAICharacter::LookUp(float NewAxisValue)
{
}

void AAICharacter::Turn(float NewAxisValue)
{
}

void AAICharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		AIAnim->PlayAttackMontage();
		AIAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AAICharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void AAICharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1);
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AAICharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AAICharacter::AttackCheck()
{
	FHitResult HitResult;
	ECollisionChannel CollsionChannel = ECollisionChannel::ECC_EngineTraceChannel1;
	switch (CurrentTeam)
	{
	case ETeam::Red:
		CollsionChannel = ECollisionChannel::ECC_GameTraceChannel3;
		break;
	case ETeam::Blue:
		CollsionChannel = ECollisionChannel::ECC_GameTraceChannel2;
		break;
	default:
		break;
	}
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		CollsionChannel,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bResult)
	{
		auto AiCharacter = Cast<AAICharacter>(HitResult.Actor);
		if (AiCharacter == nullptr)
			return;
		if (AiCharacter->CurrentTeam != CurrentTeam)
		{

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(AttackDamage, DamageEvent, GetController(),this);
		}
	}
}
