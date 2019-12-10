// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIFinalAssignment.h"
#include "AIController.h"
#include "AIAIController.generated.h"

/**
 * 
 */
UCLASS()
class AIFINALASSIGNMENT_API AAIAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	AAIAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName EndPosKey;
	static const FName TargetKey;
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
};
