// Fill out your copyright notice in the Description page of Project Settings.


#include "CastGizmo.h"
#include "TimerManager.h"

// Sets default values
ACastGizmo::ACastGizmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->AttachTo(GetRootComponent());

}

// Called when the game starts or when spawned
void ACastGizmo::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACastGizmo::StartKillGizmo(float Delay)
{

	KillDuration = Delay; 
	//FTimerDelegate KillGizmoDelegate = FTimerDelegate::CreateUObject(this, &ACastGizmo::KillGizmo);
	//GetGameInstance()->GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ACastGizmo::KillGizmo, Delay, false);
}

void ACastGizmo::KillGizmo()
{
	this->Destroy();
}


// Called every frame
void ACastGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (KillDuration > 0)
	{
		if (KillTimer < KillDuration)
		{
			KillTimer += DeltaTime;
			KillProgress = KillTimer / KillDuration; 
		}
		else
		{
			KillGizmo();
			KillDuration = 0;
			KillTimer = 0; 
		}
	}

}

