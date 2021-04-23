// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellClass.h"
#include "CooldownManager.generated.h"


/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UCooldownManager : public UObject
{
	GENERATED_BODY()

	UCooldownManager(const FObjectInitializer& Object);

	TMap<TSubclassOf<USpellClass>, float> CooldownMap;

public:
	UFUNCTION(BlueprintCallable)
	bool IsCastable(TSubclassOf<USpellClass> Spell);

	UFUNCTION(BlueprintCallable)
	float CheckCooldown(TSubclassOf<USpellClass> Spell);

	UFUNCTION(BlueprintCallable)
	void Add(TSubclassOf<USpellClass> Spell, float Cooldown);

	UFUNCTION(BlueprintCallable)
		TMap<TSubclassOf<USpellClass>, float>& GetMap(); 

	UFUNCTION(BlueprintCallable)
	void Remove(TSubclassOf<USpellClass> Spell);

	UFUNCTION(BlueprintCallable)
		void ReduceFlat(TSubclassOf<USpellClass> Spell, float Seconds);

	UFUNCTION(BlueprintCallable)
		void ReducePercentage(TSubclassOf<USpellClass> Spell, float Percentage);

	UFUNCTION(BlueprintCallable)
		void ReduceAllFlat(float Seconds);

	UFUNCTION(BlueprintCallable)
		void ReduceAllPercentage(float Percentage);

	UFUNCTION(BlueprintCallable)
		void ResetAll(); 

	void TickCooldowns(float DeltaTime); 
	
};
