// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Default1.h"
#include <Kismet/GameplayStatics.h>


#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"

void UHUD_Default1::UpdateHPBar()
{
	if(HPBar)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
			{
				if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
				{
					float CurrentHealth = MyPawn->HealthComponent->HP / MyPawn->HealthComponent->HP_MAX;
					UE_LOG(LogTemp, Warning, TEXT("Setting HPBar: %f %f %f"), MyPawn->HealthComponent->HP, MyPawn->HealthComponent->HP_MAX, CurrentHealth);
					HPBar->SetPercent(CurrentHealth);
				}
			}
		}
		
	}
}
