// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "BigTalk_DialoguePart.generated.h"


/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UBigTalk_DialoguePart : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		URichTextBlock* DialogueChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		URichTextBlock* DialogueText;
	
};
