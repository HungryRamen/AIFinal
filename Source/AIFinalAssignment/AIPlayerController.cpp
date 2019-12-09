// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayerController.h"

void AAIPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAIPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}