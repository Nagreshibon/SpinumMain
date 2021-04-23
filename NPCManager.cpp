// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCManager.h"

// Sets default values
ANPCManager::ANPCManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ANPCManager::Add(AActor* InActor)
{
	if(ATestNPC1* MyNPC = Cast<ATestNPC1>(InActor))
	{
		List.Add(MyNPC->IDName, MyNPC); 
		return true;
	}
	return false;
}

ATestNPC1* ANPCManager::GetByName(FString InName)
{
	ATestNPC1* OutNPC = *List.Find(InName);
	
	return OutNPC;
}

// Called when the game starts or when spawned
void ANPCManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

