// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenu1Button.h"
#include "Framework/Application/SlateApplication.h"

URadialMenu1Button::URadialMenu1Button(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URadialMenu1Button::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//float X, Y;
	//APlayerController* PC = GetOwningPlayer();
	//if (PC == NULL)
	//	return;

	//PC->GetMousePosition(X, Y);

	//UWorld* World = GetWorld();
	//UGameViewportClient* ViewportClient = (World != NULL) ? World->GetGameViewport() : NULL;
	//FViewport* Viewport = (ViewportClient != NULL) ? ViewportClient->Viewport : NULL;
	//if (Viewport != NULL)
	//{
	//	FIntPoint AbsM = Viewport->ViewportToVirtualDesktopPixel(FVector2D(X / float(Viewport->GetSizeXY().X), Y / float(Viewport->GetSizeXY().Y)));
	//	bIsMouseOver = MyGeometry.IsUnderLocation(FVector2D(AbsM.X, AbsM.Y));
	//}

	//if (bIsMouseOver)
	//{
	//	PC->bEnableMouseOverEvents = true;
	//	FSlateApplication::Get().CancelDragDrop();
	//	UE_LOG(LogTemp, Warning, TEXT("%d is hovered"), ButtonResponse);
	//	
	//}
}

