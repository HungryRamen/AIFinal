// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterSpawnerActor.h"
#include "AICharacter.h"
// Sets default values
AAICharacterSpawnerActor::AAICharacterSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxSpawnTime = 10.0f;
	CurrentSpawnTime = MaxSpawnTime;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (MESH.Succeeded())
	{
		Mesh->SetStaticMesh(MESH.Object);
	}

	Capsule->SetCollisionProfileName(TEXT("Spectator"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AAICharacterSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAICharacterSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentSpawnTime += DeltaTime;
	if (CurrentSpawnTime >= MaxSpawnTime)
	{
		SpawnCharacter();
		CurrentSpawnTime = 0.0f;
	}
}

void AAICharacterSpawnerActor::SpawnCharacter()
{
	FActorSpawnParameters test;
	AAICharacter* temp = nullptr;
	int32 Count = 0;
	do
	{
		if (Count++ >= 3)
			break;
		FVector l = GetActorLocation();
		FRotator r = GetActorRotation();
		l.X += FMath::Rand() % 1000 - 500;
		l.Y += FMath::Rand() % 1000 - 500;
		temp = (AAICharacter*)GetWorld()->SpawnActor(AAICharacter::StaticClass(), &l, &r, test);
	} while (temp == nullptr);
	if(temp != nullptr)
		temp->SetTeamChange(CurrentTeam);
	else
		ABLOG_S(Warning)	
}

