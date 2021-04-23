// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "TOPDOWN_Take1/CustomGameplayEffect.h"
#include "BigDialogueBody.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FBigDialogueBody : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FString IDName;

		UPROPERTY(EditAnywhere)
		FString DisplayName;

	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText DialogueText;

	UPROPERTY(EditAnywhere)
		USoundBase* SFX;

	UPROPERTY(EditAnywhere)
		float Duration;

	UPROPERTY(EditAnywhere)
		int NextIndex;

	UPROPERTY(EditAnywhere)
		int NextIndexInsanity;

	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText InsanityText;

	UPROPERTY(EditAnywhere)
		int InsanityChance;

	UPROPERTY(EditAnywhere)
		bool bEnd;

	UPROPERTY(EditAnywhere)
		bool bAnswers;

	UPROPERTY(EditAnywhere)
		bool bCameraPan;

	UPROPERTY(EditAnywhere)
		TArray<int> AnswersIndexArr;

	UPROPERTY(EditAnywhere)
		TArray<FString> DialogueAdditional;

	UPROPERTY(EditAnywhere)
		FString SkillRaise;
	
	UPROPERTY(EditAnywhere)
		FString SkillCheck;

	UPROPERTY(EditAnywhere)
		int SkillCheckScore;

	UPROPERTY(EditAnywhere)
		bool bVisited;

	UPROPERTY(EditAnywhere)
		bool bInactive;

	UPROPERTY(EditAnywhere)
		FString FlagName;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCustomGameplayEffect> Effect;
	
};
