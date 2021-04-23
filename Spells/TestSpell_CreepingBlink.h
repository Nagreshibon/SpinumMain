// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SpellClass.h"
#include "TestSpell_CreepingBlink.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWN_TAKE1_API UTestSpell_CreepingBlink : public USpellClass
{
	GENERATED_BODY()

		UTestSpell_CreepingBlink(const FObjectInitializer& Object);

	virtual void ExecuteGameplayEffect(FTransform SpellSpawnLocation) override;

	virtual bool PreExecuteGameplayEffect(FTransform SpellSpawnLocation) override;

	virtual bool PreCastEffect(FTransform SpellSpawnLocation) override; 

public: 
	UFUNCTION(BlueprintCallable)
	void PostBlinkEffects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZoomSize; 
};
