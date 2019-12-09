// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpectorPawn.h"

// Sets default values
AAISpectorPawn::AAISpectorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 800.0f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (MESH.Succeeded())
	{
		Mesh->SetStaticMesh(MESH.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ZERO(TEXT("Material'/Game/AIFinal/Material/Zero.Zero'"));
	if (ZERO.Succeeded())
	{
		Mesh->SetMaterial(0, ZERO.Object);
	}

	Capsule->SetCollisionProfileName(TEXT("Spectator"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AAISpectorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAISpectorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}
}

// Called to bind functionality to input
void AAISpectorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AAISpectorPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AAISpectorPawn::LeftRight);
}

void AAISpectorPawn::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void AAISpectorPawn::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
}

