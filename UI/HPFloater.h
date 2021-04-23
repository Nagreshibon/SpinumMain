// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HPFloater.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UHPFloater : public UUserWidget
{
	GENERATED_BODY()

	public:
	UHPFloater(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HPBar;
	
};
