// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1PlayerController.h"
#include "CooldownBar.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UCooldownBar : public UUserWidget
{
	GENERATED_BODY()
		UCooldownBar(const FObjectInitializer& ObjectInitializer);

		ATOPDOWN_Take1PlayerController* MyController;

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

		virtual void NativeConstruct() override;
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldowns")
		TSubclassOf<UUserWidget> CooldownBar_ListPart_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* ListBox;

	UPROPERTY()
		TArray<UCooldownBar_ListPart*> WidgetList;

	
};


UCLASS()
class TOPDOWN_TAKE1_API UCooldownBar_ListPart : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* Bar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;
};
