// Fill out your copyright notice in the Description page of Project Settings.


#include "IdeaWidget.h"
#include "Components/TextBlock.h"
#include "TOPDOWN_Take1/DialogueData/NPCLocalVariables.h"

UIdeaWidget::UIdeaWidget(const FObjectInitializer& Object) : Super(Object)
{
	SkillList.Add(TEXT("BRASH"));
	SkillList.Add(TEXT("STRICT"));
	SkillList.Add(TEXT("JESTER"));
	SkillList.Add(TEXT("QUIET"));
	SkillList.Add(TEXT("COLLECTIVIST"));
	SkillList.Add(TEXT("POLITE"));
	SkillList.Add(TEXT("THINKER"));
	SkillList.Add(TEXT("EMPATH"));
	SkillList.Add(TEXT("SELFLESS"));
	SkillList.Add(TEXT("STUBBORN"));
	SkillList.Add(TEXT("DECEIVER"));
	SkillList.Add(TEXT("HOPELESS"));
	SkillList.Add(TEXT("MYSTIC"));

	CounterList.Add(TEXT("BRASH"), Brash);
	CounterList.Add(TEXT("STRICT"), Strict);
	CounterList.Add(TEXT("JESTER"), Jester);
	CounterList.Add(TEXT("QUIET"), Quiet);
	CounterList.Add(TEXT("COLLECTIVIST"), Collectivist);
	CounterList.Add(TEXT("POLITE"), Polite);
	CounterList.Add(TEXT("THINKER"), Thinker);
	CounterList.Add(TEXT("EMPATH"), Empath);
	CounterList.Add(TEXT("SELFLESS"), Selfless);
	CounterList.Add(TEXT("STUBBORN"), Stubborn);
	CounterList.Add(TEXT("DECEIVER"), Deceiver);
	CounterList.Add(TEXT("HOPELESS"), Hopeless);
	CounterList.Add(TEXT("MYSTIC"), Mystic);
}

void UIdeaWidget::ReinitializeList(APawn* Pawn)
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(Pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("Initializing IdeaList reinit"));
		
		for (FString ContextString : SkillList)
		{
			if (FNPCLocalVariables* LocalRow = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*ContextString), TEXT("")))
				//if(FNPCLocalVariables LocalRow = Pawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*ContextString), TEXT("")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Iterating through rows, current string: %s"), *ContextString);
				//if (UTextBlock* CurrentPointer = *CounterList.Find(ContextString))

				if (int* CurrentPointer = CounterList.Find(ContextString))
				{


						//UE_LOG(LogTemp, Warning, TEXT("Found pointer, setting text to %d"), LocalRow->IntValue);
						//(CurrentPointer)->SetText(FText::AsNumber(LocalRow->IntValue));
						
							*CurrentPointer = LocalRow->IntValue; 
							//UE_LOG(LogTemp, Warning, TEXT("TEST"));
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("Element %s not found"), *ContextString);
				}
			}
		}
	}
	//if (FNPCLocalVariables* Locals = Pawn->BigDialoguePicker->FindRow<FDialoguePicker>(FName(*DialogueContextString), TEXT("")))
	//FNPCLocalVariables* 
}

bool UIdeaWidget::Initialize()
{
	Super::Initialize();
	
	return true;
}
