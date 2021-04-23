// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/UMG.h"
#include "GameFramework/GameModeBase.h"
#include "TOPDOWN_Take1GameMode.generated.h"

UCLASS(minimalapi)
class ATOPDOWN_Take1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATOPDOWN_Take1GameMode();

	virtual void BeginPlay() override;
	//FReply ButtonClicked();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
		FTimerHandle HUDToggleTimer;
	
		TSharedPtr<SVerticalBox> widget;

	private:
		TSharedPtr<SVerticalBox> Widget;
		TSharedPtr<STextBlock> ButtonLabel;
};


