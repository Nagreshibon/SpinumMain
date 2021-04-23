// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomGameplayEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWN_TAKE1_API UCustomGameplayEffect : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void ExecuteGameplayEffect(UObject* Target = nullptr);
	UFUNCTION(BlueprintImplementableEvent)
		void BP_ExecuteGameplayEffect(UObject* Target = nullptr);
	
};
