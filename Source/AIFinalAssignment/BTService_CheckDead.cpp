// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckDead.h"
#include "AIAIController.h"
#include "AICharacter.h"
#include "BehaviorTree//BlackboardComponent.h"

UBTService_CheckDead::UBTService_CheckDead()
{
	NodeName = TEXT("CheckDead");
	Interval = 0.5f;
}

void UBTService_CheckDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto DeadCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (DeadCharacter == nullptr)
		return;
	if (DeadCharacter->HitPoint <= 0.0f)
		OwnerComp.StopTree();
}
