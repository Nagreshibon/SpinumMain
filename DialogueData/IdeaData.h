// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "IdeaData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FIdeaData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FText DisplayName;

		UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText ShortDescription;

		UPROPERTY(EditAnywhere, meta = (MultiLine = "true"))
		FText LongDescription;
	
		UPROPERTY(EditAnywhere)
		UTexture2D* IdeaImage;

		UPROPERTY(EditAnywhere)
			UTexture2D* IdeaImageLarge;

		UPROPERTY(EditAnywhere)
			TArray<FString> Properties;

		UPROPERTY(EditAnywhere)
			TArray<FString> Cost;

		UPROPERTY(EditAnywhere)
			bool bIsActive;

		UPROPERTY(EditAnywhere)
			bool bIsBelief;

	
	
};
