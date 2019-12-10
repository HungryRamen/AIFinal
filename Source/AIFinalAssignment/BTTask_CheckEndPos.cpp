// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckEndPos.h"
#include "AIAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckEndPos::UBTTask_CheckEndPos()
{
	NodeName = TEXT("CheckEndPos");
}

EBTNodeResult::Type UBTTask_CheckEndPos::ExecuteTask(UBehaviorTreeComponent& Ownercomp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(Ownercomp, NodeMemory);

	auto AICharacter = Cast<AAICharacter>(Ownercomp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter)
		return EBTNodeResult::Failed;

	Ownercomp.GetBlackboardComponent()->SetValueAsVector(AAIAIController::EndPosKey, AICharacter->EndPos);
	return EBTNodeResult::Succeeded;

}
