// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialMenu1.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API URadialMenu1 : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue System")
		int ButtonCountCPPMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue System")
		TArray<FText> RadialAnswers;

	UFUNCTION(BlueprintImplementableEvent)
		void RadialMenu1Reconstruct();

	UFUNCTION(BlueprintImplementableEvent)
		void RadialMenu1Destroy();
};
