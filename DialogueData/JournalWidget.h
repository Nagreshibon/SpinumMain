// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "JournalWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UJournalWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
        void ActivateQuest(UDataTable* QuestLog, FString QuestID);

    UFUNCTION(BlueprintCallable)
        void UpdateQuest(UDataTable* QuestLog, FString QuestID, FString UpdatedStatus);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
        UVerticalBox* QuestScrollBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
        UVerticalBox* QuestScrollBoxCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
        TSubclassOf<UUserWidget> QuestPart_BP;

    UFUNCTION(BlueprintCallable)
        void PopulateQuestLog(UDataTable* QuestLog); 
};
