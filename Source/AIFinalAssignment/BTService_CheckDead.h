// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIFinalAssignment.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDead.generated.h"

/**
 * 
 */
UCLASS()
class AIFINALASSIGNMENT_API UBTService_CheckDead : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckDead();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
