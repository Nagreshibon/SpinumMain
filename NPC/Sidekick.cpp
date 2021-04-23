// Fill out your copyright notice in the Description page of Project Settings.


#include "Sidekick.h"

#include "TOPDOWN_Take1/AI/TestAIController.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1PlayerController.h"

void ASidekick::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ATestAIController* MyController = Cast<ATestAIController>(GetController()))
		{
			MyController->SetPlayerCharacter(GetWorld()->GetFirstPlayerController()->GetPawn());
			if(ATOPDOWN_Take1PlayerController* ControllerCast = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				ControllerCast->Sidekick = this; 
			}

			//UE_LOG(LogTemp, Warning, TEXT("Setting PlayerCharacter blackboard key for Sidekick class"));
		}
	}
}

void ASidekick::SetFollowing(bool State)
{
	bIsFollowing = State;

	if (ATestAIController* MyController = Cast<ATestAIController>(GetController()))
	{
		

		if (State)
		{
			MyController->SetPlayerCharacter(GetWorld()->GetFirstPlayerController()->GetPawn());
			UE_LOG(LogTemp, Warning, TEXT("Setting PlayerCharacter blackboard key for Sidekick class"));
		}
		else
		{
			MyController->SetPlayerCharacter(nullptr); 
		}
	}
}

void ASidekick::SetCombat(bool State)
{
	if(State)
	{
		
	}
	else
	{
		
	}
	
}

void ASidekick::TestDialogue()
{
	//code

	Super::TestDialogue(); 
	
}
