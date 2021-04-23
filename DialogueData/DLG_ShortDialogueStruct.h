// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "UObject/NoExportTypes.h"
#include "DLG_SubtitleStruct.h"
#include "DLG_ShortDialogueStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FShortDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		/*The question's excerpt - this will be shown inside our UI*/
		UPROPERTY(EditAnywhere)
		FString QuestionExcerpt;

	/*The actual SFX that we're going to play*/
	UPROPERTY(EditAnywhere)
		USoundBase* SFX;

	/*An array of the associated subtitles*/
	UPROPERTY(EditAnywhere)
		TArray<FSubtitle> Subtitles;

	/*True if we want to wait for the AI to respond*/
	UPROPERTY(EditAnywhere)
		bool bShouldAIAnswer;
};