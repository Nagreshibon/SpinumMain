// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TOPDOWN_Take1/TestNPC1.h"
#include "TOPDOWN_Take1/DialogueData/BigDialogueBody.h"
#include "BigTalk_Answers.generated.h"


/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UBigTalk_Answers : public UUserWidget
{
	GENERATED_BODY()

public:

	UBigTalk_Answers(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* AnswerTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* NumberBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* AnswerButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor UnhoveredColorCPP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ButtonResponse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USoundBase* SFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ClickedResponse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ATestNPC1* CurrentNPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int CurrentResponseCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool EndButton;
};
