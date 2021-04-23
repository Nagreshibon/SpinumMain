// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "IdeaWidget_Element.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UIdeaWidget_Element : public UUserWidget
{
	GENERATED_BODY()

		UIdeaWidget_Element(const FObjectInitializer& ObjectInitializer);
public:
	
		UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		URichTextBlock* IdeaNameBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		URichTextBlock* IdeaDescriptionBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVerticalBox* PriceBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDataTable* IdeaDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString IDName;

	UFUNCTION(BlueprintCallable)
		void ReinitElement();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idea")
		TSubclassOf<UUserWidget> IdeaWidget_Element_Price_BP;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idea")
		TMap<FString, int> CostMap; 


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idea")
		bool bDiscovered; 
	
};