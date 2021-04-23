// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Button.h"
#include "RadialMenu1Button.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API URadialMenu1Button : public UUserWidget
{
	GENERATED_BODY()

public:
	URadialMenu1Button(const FObjectInitializer& ObjectInitializer);
	
	bool bIsMouseOver;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ButtonResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* CrossButton;

};
