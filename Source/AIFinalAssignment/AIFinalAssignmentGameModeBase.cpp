// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFinalAssignmentGameModeBase.h"
#include "AICharacter.h"
#include "AIPlayerController.h"
#include "AISpectorPawn.h"

AAIFinalAssignmentGameModeBase::AAIFinalAssignmentGameModeBase()
{
	DefaultPawnClass = AAICharacter::StaticClass();
	PlayerControllerClass = AAIPlayerController::StaticClass();
}

void AAIFinalAssignmentGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
