// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"
#include "Blueprint/UserWidget.h"
#include "HUD_Default1.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UHUD_Default1 : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* TalkPortrait; 

	void UpdateHPBar();
	
};
