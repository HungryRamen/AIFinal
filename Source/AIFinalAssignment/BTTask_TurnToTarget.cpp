// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AIAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}


EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharaacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharaacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<AAICharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIAIController::TargetKey));

	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - AICharaacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	AICharaacter->SetActorRotation(FMath::RInterpTo(AICharaacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	return EBTNodeResult::Succeeded;
}
