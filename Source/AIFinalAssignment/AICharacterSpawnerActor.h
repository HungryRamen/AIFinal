// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIFinalAssignment.h"
#include "GameFramework/Actor.h"
#include "AICharacterSpawnerActor.generated.h"

UCLASS()
class AIFINALASSIGNMENT_API AAICharacterSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAICharacterSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnCharacter();

	UPROPERTY(VisibleAnywhere, Category = Collision)
		UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, Category = Visual)
		UStaticMeshComponent* Mesh;
public:
	UPROPERTY(EditAnywhere, Category = Spawn)
		float MaxSpawnTime;
	UPROPERTY(VisibleAnywhere, Category = Spawn)
		float CurrentSpawnTime;
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (Bitmask,BitmaskEnum = "ETeam"))
		ETeam CurrentTeam = ETeam::Blue;
};
