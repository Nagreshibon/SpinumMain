// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "../CustomGameplayEffect.h"
#include "DialogueBody.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogueBody : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FString IDName;

		UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText DialogueText;

		UPROPERTY(EditAnywhere)
		USoundBase* SFX;

		UPROPERTY(EditAnywhere)
		float Duration;
	
		UPROPERTY(EditAnywhere)
		int NextIndex;

		UPROPERTY(EditAnywhere)
		bool bEnd;

		UPROPERTY(EditAnywhere)
		bool bRadialAnswer;

		UPROPERTY(EditAnywhere)
		int RadialAnswersCount;

		UPROPERTY(EditAnywhere)
			TArray<FText> RadialAnswersArr;

		UPROPERTY(EditAnywhere)
			TArray<int> RadialAnswersIndexArr;

		UPROPERTY(EditAnywhere)
			TArray<int> RadialResponsesArr;

		UPROPERTY(EditAnywhere)
			TArray<FString> DialogueAdditional;

		UPROPERTY(EditAnywhere)
			TSubclassOf<UCustomGameplayEffect> Effect; 
			
	
		
};