// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "AIAIController.h"
#include "AICharacter.h"
#include "BehaviorTree//BlackboardComponent.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;

}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto DeadCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn || DeadCharacter->HitPoint <= 0.0f) return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World)
		return;

	TArray<FOverlapResult> OverlapResults;
	ECollisionChannel CollsionChannel = ECollisionChannel::ECC_EngineTraceChannel1;
	switch (DeadCharacter->CurrentTeam)
	{
	case ETeam::Red:
		CollsionChannel = ECollisionChannel::ECC_GameTraceChannel3;
		break;
	case ETeam::Blue:
		CollsionChannel = ECollisionChannel::ECC_GameTraceChannel2;
		break;
	default:
		break;
	}
	FCollisionQueryParams CollsionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CollsionChannel,
		FCollisionShape::MakeSphere(DetectRadius),
		CollsionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapReulst : OverlapResults)
		{
			auto HitAICharacter = Cast<AAICharacter>(OverlapReulst.GetActor());
			if (HitAICharacter != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIAIController::TargetKey, HitAICharacter);

			}


		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIAIController::TargetKey, nullptr);
	}
}
