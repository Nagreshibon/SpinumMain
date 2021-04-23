// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/DeviceActor.h"

// Sets default values
ADeviceActor::ADeviceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bActive = false; 

}

bool ADeviceActor::PreExecuteDeviceEffect()
{
	return true; 
}

void ADeviceActor::ExecuteDeviceEffect()
{
}

// Called when the game starts or when spawned
void ADeviceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeviceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

