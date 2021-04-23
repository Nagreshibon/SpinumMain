// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTZombieAutoAttack.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UBTZombieAutoAttack : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void AttackRelease(UBehaviorTreeComponent& OwnerComp); 

	UPROPERTY()
		FTimerHandle UnusedHandle;

	/*UFUNCTION()
	EBTNodeResult::Type AttackTimer(); */
};
