// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "NPCLocalVariables.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNPCLocalVariables : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int IntValue;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FloatValue;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString StringValue;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Flag;
};
