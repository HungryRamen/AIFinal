// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIFinalAssignment.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class AIFINALASSIGNMENT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	enum class EControlMode
	{
		Player,
		AI,
	};

	void SetTeamChange(ETeam NewTeam);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::Player;
	FVector DirectionToMove = FVector::ZeroVector;
	void SetTeam(ETeam NewTeam);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents()override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
	
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (Bitmask, BitmaskEnum = "ETeam"))
		ETeam CurrentTeam = ETeam::Blue;

	UPROPERTY(EditAnywhere, Category = Spawn)
		USkeletalMesh* SK_BLUEWARRIOR;
	UPROPERTY(EditAnywhere, Category = Spawn)
		USkeletalMesh* SK_REDWARRIOR;
	UPROPERTY(EditAnywhere, Category = Spawn)
		USkeletalMesh* SK_BLUEWEAPON;
	UPROPERTY(EditAnywhere, Category = Spawn)
		USkeletalMesh* SK_REDWEAPON;

	UPROPERTY(VisibleAnywhere, Category = AI)
		FVector EndPos;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float HitPoint;
private:

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackDamage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float CurrentDestroyTime;

	UPROPERTY()
		class UAIAnimInstance* AIAnim;
};
