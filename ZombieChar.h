// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNPC1_Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "ZombieChar.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ZombieStates : uint8 {
	IDLE	UMETA(DisplayName = "IDLE"),
	ROAM	UMETA(DisplayName = "ROAM"),
	CHASE	UMETA(DisplayName = "CHASE"),
	ATTACK	UMETA(DisplayName = "ATTACK"),
	DEAD	UMETA(DisplayName = "DEAD"),
};

UCLASS()
class TOPDOWN_TAKE1_API AZombieChar : public ATestNPC1_Enemy
{
	GENERATED_BODY()

		AZombieChar();
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPawnSensingComponent* PawnSense;

	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		float Attack1(APawn* Target);

	UFUNCTION()
		void Attack1SpawnGizmo(APawn* Target);

	UFUNCTION()
	void Attack1KillGizmo(AActor* Gizmo);

	UFUNCTION()
		void Attack1Release();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		TSubclassOf<class AActor> Attack1Gizmo;

	UFUNCTION()
		void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSprinting = false;

	// The current state of the ZombieCharacter.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
		ZombieStates State = ZombieStates::IDLE;

	// The previous state of the ZombieCharacter.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
		ZombieStates PreviousState = State;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie)
		APawn* CurrentTarget;*/

	//ANIMATIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* AttackMontage1;
	
	UPROPERTY()
		FTimerHandle UnusedHandle;
	UPROPERTY()
		FTimerHandle UnusedHandle7;
	UPROPERTY()
		FTimerHandle UnusedHandle8;
};
