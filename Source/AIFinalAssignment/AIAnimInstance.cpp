// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAnimInstance.h"

UAIAnimInstance::UAIAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/AIFinal/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))
		return;
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

	}
}

void UAIAnimInstance::PlayAttackMontage()
{

	Montage_Play(AttackMontage, 1.0f);

}

void UAIAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UAIAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UAIAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UAIAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
