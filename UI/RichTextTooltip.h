// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueTooltip.h"
#include "Components/RichTextBlockDecorator.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1PlayerController.h"
#include "RichTextTooltip.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API URichTextTooltip : public URichTextBlockDecorator
{
	GENERATED_BODY()


	public:
		URichTextTooltip(const FObjectInitializer& ObjectInitializer);

		UPROPERTY()
		ATOPDOWN_Take1PlayerController* ControllerPointer;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
			TSubclassOf<UUserWidget> DialogueTooltip_BP;

		UPROPERTY()
			UUserWidget* TooltipWidget;
	
		virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	
};
