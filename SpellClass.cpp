// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellClass.h"

#include "TOPDOWN_Take1Character.h"

// Sets default values
USpellClass::USpellClass(const FObjectInitializer& Object) : Super(Object)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	bAlwaysCastable = false;
	bTurnTowardsCursor = true;
	AnimationRate = 1.0f;
	MultiStage = false; 
}

//// Called when the game starts or when spawned
//void ASpellClass::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ASpellClass::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

bool USpellClass::PreExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
	return true;
}

void USpellClass::ExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
	if (AnimMontage)
	{
		if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
			{
				MyPawn->PlayAnimation(AnimMontage, AnimationRate); 
			}
		}
	}
}

void USpellClass::PostExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
}

bool USpellClass::PreCastEffect(FTransform SpellSpawnLocation)
{
	return true; 
}

float USpellClass::GetCooldown()
{
	return SpellCooldown;
}

void USpellClass::Kill()
{
	this->BeginDestroy(); 
}


