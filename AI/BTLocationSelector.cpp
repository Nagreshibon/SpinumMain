// Fill out your copyright notice in the Description page of Project Settings.


#include "BTLocationSelector.h"
#include "TestTargetPoint.h"
#include "TestAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTLocationSelector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* AICon = Cast<ATestAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();
		ATestTargetPoint* CurrentPoint = Cast<ATestTargetPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

		TArray<AActor*> AvailableTargetPoints = AICon->GetAvailableTargetPoints();

		int32 RandomIndex;

		ATestTargetPoint* NextTargetPoint = nullptr;

		if (AvailableTargetPoints.Num() > 1)
		{

			do
			{
				RandomIndex = FMath::RandRange(0, AvailableTargetPoints.Num() - 1);
				NextTargetPoint = Cast<ATestTargetPoint>(AvailableTargetPoints[RandomIndex]);
			} while (CurrentPoint == NextTargetPoint);

			BlackboardComp->SetValueAsObject("LocationToGo", NextTargetPoint);

			

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}


