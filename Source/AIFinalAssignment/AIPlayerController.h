// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIFinalAssignment.h"
#include "GameFramework/PlayerController.h"
#include "AIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AIFINALASSIGNMENT_API AAIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
};
