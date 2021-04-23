// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Image.h"
#include "FloaterWidget1.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FUIPlantInfo
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadOnly, Category = Plant)
		FText ShortName;
	UPROPERTY(BlueprintReadOnly, Category = Plant)
		UTexture2D* Icon = nullptr;
};

UCLASS(Abstract)
class TOPDOWN_TAKE1_API UFloaterWidget1 : public UUserWidget
{
	GENERATED_BODY()

public:
	UFloaterWidget1(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (MultiLine = "true"))
		class UTextBlock* FloaterTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* PortraitBlock;
	
};
