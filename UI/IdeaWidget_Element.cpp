// Fill out your copyright notice in the Description page of Project Settings.


#include "IdeaWidget_Element.h"
#include "TOPDOWN_Take1/DialogueData/IdeaData.h"

UIdeaWidget_Element::UIdeaWidget_Element(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	static ConstructorHelpers::FObjectFinder<UDataTable> IdeaDataTableObject(TEXT("DataTable'/Game/TopDownCPP/DialogueData/IdeaData/Ideas_Main.Ideas_Main'"));
	if (IdeaDataTableObject.Succeeded())
	{
		IdeaDataTable = IdeaDataTableObject.Object;
	}

	ReinitElement();
	//if (FIdeaData* LocalRow = IdeaDataTable->FindRow<FIdeaData>(FName(*IDName), TEXT("")))
	//{
	//	IdeaNameBlock->SetText(LocalRow->DisplayName);
	//	IdeaDescriptionBlock->SetText(LocalRow->ShortDescription);

	//	//todo image

	//	for (auto& Str : LocalRow->Cost)
	//	{
	//		TArray<FString> PriceArray;
	//		Str.ParseIntoArray(PriceArray, TEXT(" "), true);
	//		if (PriceArray.Num() == 2)
	//		{
	//			CostMap.Add(PriceArray[0], FCString::Atoi(*PriceArray[1]));
	//			UE_LOG(LogTemp, Warning, TEXT("Adding entry to CostMap: %s %s"), *PriceArray[0], *PriceArray[1]);
	//		}
	//	}
	//}
	
}

void UIdeaWidget_Element::ReinitElement()
{
	if (FIdeaData* LocalRow = IdeaDataTable->FindRow<FIdeaData>(FName(*IDName), TEXT("")))
	{
		IdeaNameBlock->SetText(LocalRow->DisplayName);
		IdeaDescriptionBlock->SetText(LocalRow->ShortDescription);
		bDiscovered = LocalRow->bIsActive; 
		//todo image

		for (auto& Str : LocalRow->Cost)
		{
			TArray<FString> PriceArray;
			Str.ParseIntoArray(PriceArray, TEXT(" "), true);
			if (PriceArray.Num() == 2)
			{
				CostMap.Add(PriceArray[0], FCString::Atoi(*PriceArray[1]));
				UE_LOG(LogTemp, Warning, TEXT("Adding entry to CostMap: %s %s"), *PriceArray[0], *PriceArray[1]);
			}
		}
	}
}
