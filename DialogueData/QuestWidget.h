// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:

	UQuestWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		URichTextBlock* QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		URichTextBlock* QuestText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* QuestStatusBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		URichTextBlock* GiverNameBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TSubclassOf<UUserWidget> QuestStatus_BP;

	UFUNCTION(BlueprintCallable)
		void PopulateQuestPart(UDataTable* QuestBody, FString CurrentStatus);
};
