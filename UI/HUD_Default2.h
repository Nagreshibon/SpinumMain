// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Default2.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API AHUD_Default2 : public AHUD
{
	GENERATED_BODY()
public: 
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* HUDWidget;
	
};
