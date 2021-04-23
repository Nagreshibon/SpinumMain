// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSpell_CreepingBlink.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"

UTestSpell_CreepingBlink::UTestSpell_CreepingBlink(const FObjectInitializer& Object) : Super(Object)
{
	ZoomSize = 1800;
	bAlwaysCastable = true;
	SpellCooldown = 5.0f; 
}

void UTestSpell_CreepingBlink::ExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		MyController->LockInteraction = true; 
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		{
			MyPawn->bInvulnerable = true;
			MyPawn->GetMesh()->SetHiddenInGame(true);

			MyPawn->OriginalBoomLength = MyPawn->CameraBoom->TargetArmLength;
			MyPawn->ZoomSize = ZoomSize;
			MyPawn->ZoomRate = 60;
			MyPawn->ZoomTo();
		}
	}
}

bool UTestSpell_CreepingBlink::PreCastEffect(FTransform SpellSpawnLocation)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (MyController->TwoStageSpell == nullptr)
		{
			//MyController->TwoStageSpell = this;
			return true;
		}
		else
		{
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
			{
				MyController->TwoStageSpell->SpellGizmoActual->K2_DestroyActor();
				//MyPawn->SetActorLocation(MyController->TwoStageSpell->SpellGizmoActual->GetActorLocation());
				//MyController->TwoStageSpell->Kill();
				PostBlinkEffects();
			}
			return false;
		}
	}
	return false;
}

bool UTestSpell_CreepingBlink::PreExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (MyController->TwoStageSpell == nullptr)
		{
			MyController->TwoStageSpell = this;
			return true;
		}
		//else
		//{
		//	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		//	{
		//		MyController->TwoStageSpell->SpellGizmoActual->K2_DestroyActor(); 
		//		//MyPawn->SetActorLocation(MyController->TwoStageSpell->SpellGizmoActual->GetActorLocation());
		//		//MyController->TwoStageSpell->Kill();
		//		PostBlinkEffects(); 
		//	}
		//	return false; 
		//}
		return false; 
	}
	return false;
}

void UTestSpell_CreepingBlink::PostBlinkEffects()
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		{
			MyPawn->bIsCasting = false;
			MyPawn->GetMesh()->SetHiddenInGame(false);
			MyPawn->bInvulnerable = false;
			
			MyPawn->ZoomSize = MyPawn->OriginalBoomLength;
			MyPawn->ZoomRate = 80;
			MyPawn->ZoomTo();
		}
		MyController->LockInteraction = false; 
		MyController->TwoStageSpell = nullptr;
		//MyController->CooldownManager->Add(StaticClass(), SpellCooldown); 
	}
}
