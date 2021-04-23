// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"
#include "Engine/DataTable.h"
#include "QuestBody.h"

UQuestWidget::UQuestWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void UQuestWidget::PopulateQuestPart(UDataTable* QuestBody, FString CurrentStatus)
{

	FQuestBody* bur = QuestBody->FindRow<FQuestBody>(FName(*CurrentStatus), TEXT(""), true);
	
	if (FQuestBody* xak = QuestBody->FindRow<FQuestBody>(FName(*CurrentStatus), TEXT("")))
	{
		QuestText->SetText(xak->QuestText);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Couldn't find status %s"), *(CurrentStatus));

	
		for (auto it : QuestBody->GetRowMap())
		{
			UE_LOG(LogTemp, Warning, TEXT("Iterating through quest body, entry %s"), *(it.Key.ToString()));
			if (FQuestBody* QuestBodyRow = (FQuestBody*)(it.Value))
			{
				UE_LOG(LogTemp, Warning, TEXT("Iterating through quest body, entry %s"), *(it.Key.ToString()));
				//QuestText->SetText(QuestBodyRow->QuestText);
			}
		}
}
