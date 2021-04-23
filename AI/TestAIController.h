// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TestAIController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API ATestAIController : public AAIController
{
	GENERATED_BODY()

	ATestAIController();
public:
	virtual void OnPossess(APawn* InPawn) override;
	void SetTargetEnemy(APawn* NewTarget);
	void SetPlayerCharacter(APawn* PlayerCharacter);

	

	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetEnemyKey;

	TArray<AActor*> BotTargetPoints;

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }

	
};
