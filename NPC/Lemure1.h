// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TestNPC1_Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Lemure1.generated.h"


/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API ALemure1 : public ATestNPC1_Enemy
{
	GENERATED_BODY()

		ALemure1();

		virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UPawnSensingComponent* PawnSense;

	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		void Attack1(APawn* Target);

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

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lemure)
	//	APawn* CurrentTarget;

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
