// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateTypes.h"
#include "Framework/Text/TextLayout.h"
#include "Framework/Text/ISlateRun.h"
#include "Framework/Text/ITextDecorator.h"
#include "Components/RichTextBlockDecorator.h"
#include "Engine/DataTable.h"
#include "RichTextTooltip2.generated.h"

class ISlateStyle;

UCLASS(Abstract, Blueprintable)
class TOPDOWN_TAKE1_API URichTextBlockTooltip : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	URichTextBlockTooltip(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
		TSubclassOf<UUserWidget> DialogueTooltip_BP;

	UPROPERTY()
		UUserWidget* TooltipWidget;

protected:
	

};
