// Copyright Epic Games, Inc. All Rights Reserved.

#include "RichTextTooltip2.h"
#include "UObject/SoftObjectPtr.h"
#include "Rendering/DrawElements.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SlateTextLayout.h"
#include "Slate/SlateGameResources.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Math/UnrealMathUtility.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SBox.h"
#include "Misc/DefaultValueHelper.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Package.h"

#define LOCTEXT_NAMESPACE "UMG"


class SRichInlineTooltip : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRichInlineTooltip)
	{}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs)
	{
		
			ChildSlot
				[
					SNew(STextBlock)
					.ShadowColorAndOpacity(FLinearColor::Black)
				.ColorAndOpacity(FLinearColor::Red)
				.ShadowOffset(FIntPoint(-1, 1))
				//.Font(FSlateFontInfo("Verdana", 16))
				.Text(LOCTEXT("Hello", "Hello!"))
				];
	}
};

class FRichInlineTooltip : public FRichTextDecorator
{
public:
	FRichInlineTooltip(URichTextBlock* InOwner, URichTextBlockTooltip* InDecorator)
		: FRichTextDecorator(InOwner)
		, Decorator(InDecorator)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		
		return (RunParseResult.Name == TEXT("TOOLTIP"));
	}

	//virtual TSharedRef<ISlateRun> Create(const TSharedRef<FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style) override;
	
	//TSharedRef<ISlateRun> FRichInlineTooltip::Create(const TSharedRef<class FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef< FString >& InOutModelText, const ISlateStyle* Style)

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) 
	{

		/*for (const TPair<FString, FTextRange>& Pair : RunParseResultGlobal.MetaData)
		{
			RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
		}
		*/
		return SNew(SRichInlineTooltip);
	}

private:
	URichTextBlockTooltip* Decorator;
};

//TSharedRef<ISlateRun> FRichInlineTooltip::Create(const TSharedRef<FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style)
//{
//	FRunInfo RunInfo(RunParseResult.Name);
//	for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
//	{
//		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
//	}
//
//	FTextRange ModelRange;
//	ModelRange.BeginIndex = InOutModelText->Len();
//	*InOutModelText += OriginalText.Mid(RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex);
//	ModelRange.EndIndex = InOutModelText->Len();
//	FText TooltipText;
//
//	//return CreateRun(TextLayout, RunInfo, InOutModelText, CreateTextBlockStyle(RunInfo, TooltipText), ModelRange);
//}


/////////////////////////////////////////////////////
// URichTextBlockImageDecorator

URichTextBlockTooltip::URichTextBlockTooltip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> URichTextBlockTooltip::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichInlineTooltip(InOwner, this));
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
