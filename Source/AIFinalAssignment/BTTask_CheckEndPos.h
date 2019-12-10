// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckEndPos.generated.h"

/**
 * 
 */
UCLASS()
class AIFINALASSIGNMENT_API UBTTask_CheckEndPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_CheckEndPos();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Ownercomp, uint8* NodeMemory) override;
};
