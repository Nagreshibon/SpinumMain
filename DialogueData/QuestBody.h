// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "QuestBody.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FQuestBody : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		FText DisplayName;

	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText QuestText;

	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText TaskText;

	UPROPERTY(EditAnywhere)
		USoundBase* SFX;

	
	UPROPERTY(EditAnywhere)
		bool bEnd;

	UPROPERTY(EditAnywhere)
		TArray<FString> Rewards;

	UPROPERTY(EditAnywhere)
		int State;

};
