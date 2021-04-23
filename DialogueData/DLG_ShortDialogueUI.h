// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DLG_ShortDialogueStruct.h"
//#include "DLG_SubtitleStruct.h"
#include "DLG_ShortDialogueUI.generated.h"


/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UDLG_ShortDialogueUI : public UUserWidget
{
	GENERATED_BODY()

	public:

	/*This property will be used in order to bind our subtitles
	Binding will make sure to notify the UI if the content of the following
	variable change.*/
	UPROPERTY(BlueprintReadOnly)
		FString SubtitleToDisplay;


	/*Updates the displayed subtitles based on the given array*/
	UFUNCTION(BlueprintCallable, Category = DialogueSystem)
		void UpdateSubtitles(TArray<FSubtitle> Subtitles);

	/*This array will populate our buttons from within the show function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> Questions;

	/*Adds the widget to our viewport and populates the buttons with the given questions*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = DialogueSystem)
		void Show();
	
};
