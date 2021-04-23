// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCheckDialogue.h"
#include "TestAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TOPDOWN_Take1/TestNPC1.h"


EBTNodeResult::Type UBTCheckDialogue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("BTNode: Checking dialogue..."));
	if (ATestAIController* AICon = Cast<ATestAIController>(OwnerComp.GetAIOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("BTNode: Controller cast succesful"));
		if (ATestNPC1* CurrentNPC = Cast<ATestNPC1>(AICon->GetPawn()))
		{
			UE_LOG(LogTemp, Warning, TEXT("BTNode: Cast to: %s"), *CurrentNPC->CharName);
			if (CurrentNPC->bIsInDialogue == true) return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Succeeded;
}
