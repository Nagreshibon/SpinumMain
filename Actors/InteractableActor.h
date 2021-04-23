// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeviceActor.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class TOPDOWN_TAKE1_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ADeviceActor* TargetActor; 
	
	UFUNCTION(BlueprintCallable)
		virtual bool PreExecuteInteractEffect();

	UFUNCTION(BlueprintCallable)
		virtual void ExecuteInteractEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_ExecuteInteractEffect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bActive; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
