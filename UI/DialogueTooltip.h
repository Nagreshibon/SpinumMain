// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "DialogueTooltip.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UDialogueTooltip : public UUserWidget
{
	GENERATED_BODY()

	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		URichTextBlock* TooltipTextBlock; 
	
};
