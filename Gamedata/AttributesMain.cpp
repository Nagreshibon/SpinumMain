// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesMain.h"

#include "Engine/Engine.h"
#include "GameplayEffectExtension.h"
#include "Kismet/GameplayStatics.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"
//#include "Engine/World.h"

UAttributesMain::UAttributesMain()
{
	PlayerMovementSpeed = 600.0f;
}

void UAttributesMain::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributesMain::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributesMain, PlayerMovementSpeed)))
	{

		if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
			{
				//float temp = MyPawn->GetCharacterMovement()->MaxWalkSpeed;
				//PlayerMovementSpeed.SetBaseValue(temp);
				//PlayerMovementSpeed.SetCurrentValue(temp);
				//UE_LOG(LogTemp, Warning, TEXT("MOVEMENT SPEED SET TO: %f, broadcasting"), PlayerMovementSpeed.GetCurrentValue());
				//OnPlayerMovementSpeedChanged.Broadcast(PlayerMovementSpeed.GetCurrentValue()); 
				
			}
			
		}
		


	}
}
