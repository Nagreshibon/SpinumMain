// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAggro.h"
#include "TestAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../ZombieChar.h"
#include "TOPDOWN_Take1/TestNPC1.h"


EBTNodeResult::Type UBTAggro::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("BTNode: Checking dialogue..."));
	if (ATestAIController* AICon = Cast<ATestAIController>(OwnerComp.GetAIOwner()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("BTNode: Controller cast succesful"));
		if (AZombieChar* CurrentNPC = Cast<AZombieChar>(AICon->GetPawn()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("BTNode: Cast to: %s"), *CurrentNPC->CharName);
			if (CurrentNPC->CurrentTarget) return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
