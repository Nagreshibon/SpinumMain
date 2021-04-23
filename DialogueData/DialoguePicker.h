// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DialogueBody.h"
#include "DialoguePicker.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialoguePicker : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		int DialogueIndex;

	UPROPERTY(EditAnywhere)
		FString IDName;

	UPROPERTY(EditAnywhere)
		UDataTable* DialogueBody;

	UPROPERTY(EditAnywhere)
		bool bLeftSide;

	UPROPERTY(EditAnywhere)
		bool bCustomCamera;
};