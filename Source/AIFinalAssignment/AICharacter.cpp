// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIAnimInstance.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f),FRotator(0.0f, -90.0f, 0.0f));

	//SetTeam(ETeam::Blue);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/AIFinal/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AICharacter"));

	SetControlMode(EControlMode::Player);

	IsAttacking = false;
	MaxCombo = 3;
	AttackRange = 200.0f;
	AttackRadius = 50.0f;
	AttackDamage = 10.0f;
	HitPoint = 30.0f;
	AttackEndComboState();
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
		break;
	default:
		break;
	}
}

void AAICharacter::SetTeam(ETeam NewTeam)
{
	CurrentTeam = NewTeam;
	auto WarriorLocation = TEXT("");
	auto WeaponLocation = TEXT("");
	switch (CurrentTeam)
	{
	case ETeam::Red:
		WarriorLocation = TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_solid.SK_CharM_solid'");
		WeaponLocation = TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_AnthraciteAxe/SK_Blade_AnthraciteAxe.SK_Blade_AnthraciteAxe'");
		break;
	case ETeam::Blue:
		WarriorLocation = TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant'");
		WeaponLocation = TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_IceElemental/SK_Blade_IceElemental.SK_Blade_IceElemental'");
		break;
	default:
		return;
		break;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WARRIOR(WarriorLocation);
	if (SK_WARRIOR.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_WARRIOR.Object);
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(WeaponLocation);
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
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
		SetActorEnableCollision(false);
	}
	return FinalDamage;
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
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bResult)
	{
		auto AiCharacter = Cast<AAICharacter>(HitResult.Actor);
		if (AiCharacter->CurrentTeam != CurrentTeam)
		{

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(AttackDamage, DamageEvent, GetController(),this);
		}
	}
}
