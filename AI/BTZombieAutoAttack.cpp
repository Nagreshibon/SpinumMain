// Fill out your copyright notice in the Description page of Project Settings.


#include "BTZombieAutoAttack.h"
#include "TestAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../ZombieChar.h"
#include "TOPDOWN_Take1/TestNPC1.h"
#include "TOPDOWN_Take1/Gamedata/AttributesMain.h"
#include "TimerManager.h"

EBTNodeResult::Type UBTZombieAutoAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("BTNode: Trying to attack..."));
	if (ATestAIController* AICon = Cast<ATestAIController>(OwnerComp.GetAIOwner()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("BTNode: Controller cast succesful"));
		if (AZombieChar* CurrentNPC = Cast<AZombieChar>(AICon->GetPawn()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("BTNode: Cast to: %s"), *CurrentNPC->CharName);
			if (CurrentNPC->CurrentTarget)
			{
				
				float Distance = FVector::Dist(CurrentNPC->GetActorLocation(), CurrentNPC->CurrentTarget->GetActorLocation());
				FVector MyLocation = CurrentNPC->GetActorLocation();
				FVector NPCLocation = CurrentNPC->CurrentTarget->GetActorLocation(); 
				FRotator TargetRotation = FRotator(0, (NPCLocation - MyLocation).ToOrientationRotator().Yaw, 0);
				FRotator CurrentRotation = FRotator(0, CurrentNPC->GetActorForwardVector().ToOrientationRotator().Yaw, 0);
				float Phi = TargetRotation.Yaw - CurrentRotation.Yaw; 

				//UE_LOG(LogTemp, Warning, TEXT("Trying to attack, Distance = %f, Phi = %f, MeleeRange = %f"), Distance, Phi, CurrentNPC->AttributeComponent->MeleeRange.GetCurrentValue());
				if (Distance < CurrentNPC->AttributeComponent->MeleeRange.GetCurrentValue() && !CurrentNPC->isAttacking && abs(Phi)<70.0f)
				{
					//GetWorldTimerManager().SetTimer(UnusedHandle, this, &UBTZombieAutoAttack::FinishLatentTask, CurrentNPC->Attack1(CurrentNPC->CurrentTarget), false);
					//void FinishLatentTask(UBehaviorTreeComponent & OwnerComp, EBTNodeResult::Type TaskResult) const;
					//EBTNodeResult::Type Result = EBTNodeResult::Succeeded;
					//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					
					FTimerDelegate AttackReleaseDelegate = FTimerDelegate::CreateLambda([=, &OwnerComp]()
					{
						AttackRelease(OwnerComp); 
					});
					GetWorld()->GetTimerManager().SetTimer(UnusedHandle, AttackReleaseDelegate, CurrentNPC->Attack1(CurrentNPC->CurrentTarget), false);
					//UE_LOG(LogTemp, Warning, TEXT("Calling AttackRelease..."));
					
					
					CurrentNPC->Attack1(CurrentNPC->CurrentTarget);
					return EBTNodeResult::InProgress;
				}
				else return EBTNodeResult::Failed; 
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTZombieAutoAttack::AttackRelease(UBehaviorTreeComponent& OwnerComp)
{
	//UE_LOG(LogTemp, Warning, TEXT("Executing AttackRelease..."));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

//EBTNodeResult::Type UBTZombieAutoAttack::AttackTimer()
//{
//	float delayTime = 1.5f; 
//	
//	FTimerHandle handle;
//	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
//		return EBTNodeResult::Succeeded;
//	}, delayTime, 1);
//
//	return EBTNodeResult::Failed; 
//}
