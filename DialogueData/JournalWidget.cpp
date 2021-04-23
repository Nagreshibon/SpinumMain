// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalWidget.h"
#include "Engine/DataTable.h"
#include "QuestBody.h"
#include "QuestPicker.h"
#include "QuestWidget.h"

void UJournalWidget::PopulateQuestLog(UDataTable* QuestLog)
{
	if (QuestLog)
	{
		for (auto it : QuestLog->GetRowMap())
		{
			if (FQuestPicker* QuestPickerRow = (FQuestPicker*)(it.Value))
			{
				if ((QuestPickerRow->Status != "0") && (QuestPickerRow->Status != ""))
				{
					FName CurrentWidgetName = it.Key;
					if (QuestPart_BP)
					{
						UE_LOG(LogTemp, Warning, TEXT("Iterating through quest log, entry %s"), *(it.Key.ToString()));
						if (UUserWidget* CurrentWidget = CreateWidget<UQuestWidget>(GetWorld()->GetFirstPlayerController(), QuestPart_BP, CurrentWidgetName))
						{
							if (UQuestWidget* CurrentWidgetCast = Cast<UQuestWidget>(CurrentWidget))
							{
								UE_LOG(LogTemp, Warning, TEXT("Iterating through quest log"));
								
								CurrentWidgetCast->QuestName->SetText(QuestPickerRow->DisplayName);
								CurrentWidgetCast->GiverNameBox->SetText(QuestPickerRow->QuestGiverDisplayName);
								if (QuestPickerRow->Status == "999")
								{
									QuestScrollBoxCompleted->AddChildToVerticalBox(CurrentWidget);
								}
								else
								{
									QuestScrollBox->AddChildToVerticalBox(CurrentWidget);
								}
								CurrentWidgetCast->PopulateQuestPart(QuestPickerRow->QuestBody, QuestPickerRow->Status);

							}
						}
					}
				}
			}
		}
	}
}

void UJournalWidget::ActivateQuest(UDataTable* QuestLog, FString QuestID)
{
	UE_LOG(LogTemp, Warning, TEXT("JournalWidget->Activating Quest %s"), *QuestID);
	if (FQuestPicker* QuestPickerRow = QuestLog->FindRow<FQuestPicker>(FName(*QuestID), TEXT("")))
	{
		QuestPickerRow->Status = "1";
	}
}

void UJournalWidget::UpdateQuest(UDataTable* QuestLog, FString QuestID, FString UpdatedStatus)
{
	if (FQuestPicker* QuestPickerRow = QuestLog->FindRow<FQuestPicker>(FName(*QuestID), TEXT("")))
	{
		QuestPickerRow->Status = UpdatedStatus;
	}
}
