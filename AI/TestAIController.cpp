// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAIController.h"
#include "TestTargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "TOPDOWN_Take1/TestNPC1.h"
#include "TOPDOWN_Take1/ZombieChar.h"

ATestAIController::ATestAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	LocationToGoKey = "LocationToGo";

}


void ATestAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ATestNPC1* AIChar = Cast<ATestNPC1>(InPawn))
	{

		if (AIChar)
		{
			if (AIChar->BehaviorTree)
			{
				if (AIChar->BehaviorTree->BlackboardAsset)
				{
					BlackboardComp->InitializeBlackboard(*(AIChar->BehaviorTree->BlackboardAsset));
				}

				//location finder
				//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestTargetPoint::StaticClass(), BotTargetPoints);
				//UE_LOG(LogTemp, Warning, TEXT("BTNode: Looking for TargetPoints with tag: %s"), *AIChar->IDName);
				if (*AIChar->IDName)
				{
					UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATestTargetPoint::StaticClass(), FName(*AIChar->IDName), BotTargetPoints);
				}

				if (AIChar->BehaviorTree)
				{
					BehaviorComp->StartTree(*AIChar->BehaviorTree);
				}
			}
		}
	}
}

void ATestAIController::SetTargetEnemy(APawn* NewTarget)
{
	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsObject("TargetEnemy", NewTarget);
		if(AZombieChar* MyZombie = Cast<AZombieChar>(GetPawn()))
		{
			MyZombie->CurrentTarget = NewTarget;
		}
		//UE_LOG(LogTemp, Warning, TEXT("SETTING TARGET ENEMY KEY TO PAWN"));
	}
}

void ATestAIController::SetPlayerCharacter(APawn* PlayerCharacter)
{
	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsObject("PlayerCharacter", PlayerCharacter);
		//UE_LOG(LogTemp, Warning, TEXT("SETTING PLAYER CHARACTER KEY ON THE CONTROLLER"));
	}
}
