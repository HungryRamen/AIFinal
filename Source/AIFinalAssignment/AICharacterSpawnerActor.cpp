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
	CurrentTeam = ETeam::Blue;
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
	FVector l = GetActorLocation();
	FRotator r = GetActorRotation();
	auto a = (AAICharacter*)GetWorld()->SpawnActor(AAICharacter::StaticClass(), &l, &r, test);
	a->SetTeam(CurrentTeam);
}

