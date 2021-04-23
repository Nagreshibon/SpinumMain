// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"
#include "IdeaWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UIdeaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UIdeaWidget(const FObjectInitializer& Object);

	UFUNCTION(BlueprintCallable)
	void ReinitializeList(APawn* Pawn);

	virtual bool Initialize() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FString> SkillList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, int> CounterList; 

	//POINT COUNTERS

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Brash;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Strict;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Jester;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Quiet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Collectivist;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Polite;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Thinker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Empath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Selfless;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Stubborn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Deceiver;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Hopeless;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		int Mystic;

	
};
