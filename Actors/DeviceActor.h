// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeviceActor.generated.h"

UCLASS()
class TOPDOWN_TAKE1_API ADeviceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeviceActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bActive; 
	
	UFUNCTION(BlueprintImplementableEvent)
		void ActivateDevice();

	UFUNCTION(BlueprintImplementableEvent)
		void DeactivateDevice();

	UFUNCTION(BlueprintCallable)
		virtual bool PreExecuteDeviceEffect();
	
	UFUNCTION(BlueprintCallable)
		virtual void ExecuteDeviceEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
