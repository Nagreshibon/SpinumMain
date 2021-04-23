// Fill out your copyright notice in the Description page of Project Settings.


#include "TestNPC1_Enemy.h"
#include "Components/TextBlock.h"
#include "UI/FloaterWidget1.h"

ATestNPC1_Enemy::ATestNPC1_Enemy()
{
	isEnemy = true;
	TestValue = 7;
	/*HPFloater1->SetHiddenInGame(false);*/
}

void ATestNPC1_Enemy::TestDialogue3(FText TextIn, float Duration)
{

		UE_LOG(LogTemp, Warning, TEXT("PLAYER SPOTTED: ENABLING WIDGET"));
		NameFloaterBlock2->SetHiddenInGame(false);
	if (UFloaterWidget1* MyFloaterWidget = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER SPOTTED: ENABLING WIDGET 2"));
		MyFloaterWidget->FloaterTextBlock->SetText(TextIn);
		GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATestNPC1_Enemy::HideFloater, Duration, false);
	}
}

void ATestNPC1_Enemy::HideFloater()
{
NameFloaterBlock2->SetHiddenInGame(true);
}
