// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TOPDOWN_Take1GameMode.h"
#include "TOPDOWN_Take1PlayerController.h"
#include "TOPDOWN_Take1Character.h"
#include "UObject/ConstructorHelpers.h"

ATOPDOWN_Take1GameMode::ATOPDOWN_Take1GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATOPDOWN_Take1PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//WIDGET TEST
	
}

void ATOPDOWN_Take1GameMode::BeginPlay()
{
	Super::BeginPlay();




	
	//widget = SNew(SVerticalBox) + SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)[SNew(SButton).Content()[SNew(STextBlock).Text(FText::FromString(TEXT("Test button")))]];
	//GEngine->GameViewport->AddViewportWidgetForPlayer(GetWorld()->GetFirstLocalPlayerFromController(),widget.ToSharedRef(), 1);
	//GetWorld()->GetTimerManager().SetTimer(HUDToggleTimer, FTimerDelegate::CreateLambda([this]
	//{
	//	if (this->widget->GetVisibility().IsVisible())
	//	{
	//		this->widget->SetVisibility(EVisibility::Hidden);
	//	}
	//	else
	//	{
	//		this->widget->SetVisibility(EVisibility::Visible);
	//	}
	//}), 5, true);
	

	//Widget = SNew(SVerticalBox) + SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
	//[SNew(SButton).OnClicked(FOnClicked::CreateUObject(this, &ATOPDOWN_Take1GameMode::ButtonClicked)).Content()[SAssignNew(ButtonLabel, STextBlock).Text(FText::FromString(TEXT("Click me!")))]];

	//GEngine->GameViewport->AddViewportWidgetForPlayer(GetWorld()->GetFirstLocalPlayerFromController(),Widget.ToSharedRef(), 1);

	//GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	//GEngine->GetFirstLocalPlayerController(GetWorld())->SetInputMode(FInputModeUIOnly().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetWidgetToFocus(Widget));
	
}

//FReply ATOPDOWN_Take1GameMode::ButtonClicked()
//{
//	ButtonLabel->SetText(FText::FromString(TEXT("Clicked!")));
//	return FReply::Handled();
//}

void ATOPDOWN_Take1GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Super::EndPlay(EndPlayReason);
	//GetWorld()->GetTimerManager().ClearTimer(HUDToggleTimer);
}