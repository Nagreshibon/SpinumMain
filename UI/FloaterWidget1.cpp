// Fill out your copyright notice in the Description page of Project Settings.


#include "FloaterWidget1.h"
#include "UMG/Public/Components/TextBlock.h"

UFloaterWidget1::UFloaterWidget1(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UFloaterWidget1::NativeConstruct()
{
	// Do some custom setup

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();

	if (FloaterTextBlock)
	{
//		FloaterTextBlock->SetText(FText::FromString("HELLO CHANGING TEXT"));
		//FText testingtext = FloaterTextBlock->GetText();
		//UE_LOG(LogTemp, Warning, TEXT("Setting i to: %s"), *testingtext.ToString());
	}
}


void UFloaterWidget1::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here
}