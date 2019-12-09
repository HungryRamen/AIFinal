// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIFinalAssignment.h"
#include "GameFramework/GameModeBase.h"
#include "AIFinalAssignmentGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AIFINALASSIGNMENT_API AAIFinalAssignmentGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AAIFinalAssignmentGameModeBase();

public:
	virtual void PostLogin(APlayerController* NewPlayer)override;
};
