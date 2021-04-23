// Fill out your copyright notice in the Description page of Project Settings.


#include "CastBar.h"
#include "TimerManager.h"
#include "../Combat/CastGizmo.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1PlayerController.h"

UCastBar::UCastBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CastTimer = 0;
	CastDuration = 0; 
}

void UCastBar::ShowBar()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UCastBar::ResetBar()
{
	//if (CastGizmo) CastGizmo->Destroy();
	if (ACastGizmo* MyGizmo = Cast<ACastGizmo>(CastGizmo))
	{
		MyGizmo->StartKillGizmo(SpellCast->CastGizmoKillDelay);
	}
	this->SetVisibility(ESlateVisibility::Collapsed);
	CastTimer = 0;
	CastDuration = 0;
}

void UCastBar::StartCastBar(float Duration)
{
	CastDuration = Duration;
}

void UCastBar::CastSpellSuccessful()
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		MyController->AbilityCastSpellSuccessful(SpellCast, SpellClass, SpawnLocation); 
	}
}

void UCastBar::Interrupt()
{
	ResetBar(); 
}

void UCastBar::MoveInterrupt()
{
	if (SpellCast)
	{
		if (!SpellCast->bMobile)
		{
			if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
				{
					MyPawn->bIsCasting = false; 
				}
			}
			
			ResetBar();
		}
	}
}

AActor* UCastBar::CreateCastGizmo(TSubclassOf<AActor> GizmoClass, FTransform TargetTransform)
{
	//CastGizmo = GetWorld()->SpawnActor<AActor>(GizmoClass, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorTransform());
	CastGizmo = GetWorld()->SpawnActor<AActor>(GizmoClass, TargetTransform);
	//FTimerDelegate KillGizmo = FTimerDelegate::CreateUObject(this, &UCastBar::KillCastGizmo, CastGizmo);
	//GetGameInstance()->GetWorld()->GetTimerManager().SetTimer(UnusedHandle, KillGizmo, SpellCast->CastGizmoKillDelay, false);
	return CastGizmo; 
}

void UCastBar::KillCastGizmo(AActor* Gizmo)
{
	if(ACastGizmo* MyGizmo = Cast<ACastGizmo>(Gizmo))
	{
		MyGizmo->StartKillGizmo(SpellCast->CastGizmoKillDelay); 
	}
	
}

void UCastBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCastBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CastDuration > 0)
	{
		if (CastTimer < CastDuration)
		{
			CastTimer += InDeltaTime;
			CastProgressBar->SetPercent(CastTimer / CastDuration);
			if(CastGizmo)
			{
				if (ACastGizmo* MyCastGizmo = Cast<ACastGizmo>(CastGizmo))
				{
					MyCastGizmo->GizmoProgress = CastTimer / CastDuration; 
				}
			}
		}
		else
		{
			CastSpellSuccessful(); 
			ResetBar();
		}
	}
}