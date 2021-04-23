// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TestNPC1_Friendly.h"
#include "Sidekick.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API ASidekick : public ATestNPC1_Friendly
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override; 
	
	UFUNCTION(BlueprintCallable)
		void SetFollowing(bool State);

	UFUNCTION(BlueprintCallable)
		void SetCombat(bool State);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsFollowing;

		virtual void TestDialogue() override; 
	
private:
	
};
