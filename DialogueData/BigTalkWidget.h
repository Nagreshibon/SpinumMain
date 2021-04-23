// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "TOPDOWN_Take1/TestNPC1.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "BigTalkWidget.generated.h"



/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UBigTalkWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBigTalkWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void ActivateQuest(FString QuestID);

	UFUNCTION(BlueprintCallable)
	void UpdateQuest(FString QuestID, FString UpdatedStatus);
	
	UFUNCTION(BlueprintCallable)
		void ResetVisited(ATestNPC1* MyNPC);

	UFUNCTION(BlueprintCallable)
		void StartBigTalk(ATestNPC1* MyNPC);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* DialogueScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UUserWidget*> ScrollboxContents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> JournalNotify_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> InfluenceGained_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> InfluenceLost_BP;
	
	UPROPERTY(EditAnywhere)
		bool bCustomCamera;

	
	UPROPERTY()
		int i;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bInsanity;

	UPROPERTY()
		int DialogueIndex;

	UPROPERTY(BlueprintReadOnly)
		ATestNPC1* CurrentNPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool End;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Chapter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* PortraitImage;

	UFUNCTION(BlueprintImplementableEvent)
		void RemoveWidgetInBlueprints();
	
	UFUNCTION(BlueprintImplementableEvent)
		void RedFlash();

	UFUNCTION(BlueprintImplementableEvent)
		void BlackBGOn();

	UFUNCTION(BlueprintImplementableEvent)
		void BlackBGOff();

	UFUNCTION(BlueprintImplementableEvent)
		void BlurBGOn();
	
	UFUNCTION(BlueprintImplementableEvent)
		void BlurBGOff();

	UFUNCTION(BlueprintImplementableEvent)
		void BlackBGInstant();

	UFUNCTION(BlueprintImplementableEvent)
		void ScrollboxScrollDown();

	UFUNCTION(BlueprintImplementableEvent)
		void ScrollboxScrollToWidget(UWidget* WidgetPointer);
	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TSubclassOf<UUserWidget> DialoguePart_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TSubclassOf<UUserWidget> DialogueAnswers_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* ButtonContinue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ButtonContinueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* ScrollBoxMain;

	UPROPERTY()
		FTimerHandle UnusedHandle;

	UPROPERTY()
		FTimerHandle UnusedHandle2;


};

