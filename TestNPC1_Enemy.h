// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNPC1.h"
#include "TestNPC1_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API ATestNPC1_Enemy : public ATestNPC1
{
	GENERATED_BODY()

protected:
	
	ATestNPC1_Enemy();

	

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//		bool isEnemy;

	void TestDialogue3(FText TextIn, float Duration);

	void HideFloater();

	UPROPERTY()
		FTimerHandle UnusedHandle2;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	float TestValue;
	//
};
