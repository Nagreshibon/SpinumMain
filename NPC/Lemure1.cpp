// Fill out your copyright notice in the Description page of Project Settings.


#include "Lemure1.h"

ALemure1::ALemure1()
{
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(FName(TEXT("PawnSense")));
}

void ALemure1::BeginPlay()
{
	Super::BeginPlay();
	

	if (PawnSense)
	{
	PawnSense->OnSeePawn.AddDynamic(this, &ALemure1::OnSeePlayer);
	PawnSense->OnHearNoise.AddDynamic(this, &ALemure1::OnHearNoise);
	}

	if (HealthComponent)
	{
	HealthComponent->HP_MAX = 100;
	HealthComponent->HP = 100;
	}

	if (AttributeComponent)
	{
	}
	
}

void ALemure1::OnSeePlayer(APawn* Pawn)
{
}

void ALemure1::Attack1(APawn* Target)
{
}

void ALemure1::Attack1SpawnGizmo(APawn* Target)
{
	if (Target)
	{
		if (Attack1Gizmo)
		{
			FTransform SpawnLocation = GetActorTransform();

			AActor* Gizmo = GetWorld()->SpawnActor<AActor>(Attack1Gizmo, SpawnLocation);
			Gizmo->SetInstigator(this);

			FTimerDelegate KillGizmo = FTimerDelegate::CreateUObject(this, &ALemure1::Attack1KillGizmo, Gizmo);
			GetWorldTimerManager().SetTimer(UnusedHandle, KillGizmo, 0.3f, false);
		}
	}
}

void ALemure1::Attack1KillGizmo(AActor* Gizmo)
{
	Gizmo->Destroy();
}

void ALemure1::Attack1Release()
{
	isAttacking = false;
}

void ALemure1::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
}
