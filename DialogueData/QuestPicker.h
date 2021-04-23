// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "QuestPicker.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FQuestPicker : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FText DisplayName;

	UPROPERTY(EditAnywhere)
		FText QuestGiverDisplayName;

	UPROPERTY(EditAnywhere)
		FString QuestGiverIDName;

	UPROPERTY(EditAnywhere)
		UDataTable* QuestBody;

	UPROPERTY(EditAnywhere)
		FString Status;

	//0 - inactive
	//1 - completed
	//2 - botched
	//anything else - in progress
	

};
