// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CastGizmo.generated.h"

UCLASS()
class TOPDOWN_TAKE1_API ACastGizmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACastGizmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GizmoProgress;

	UFUNCTION(BlueprintCallable)
		void StartKillGizmo(float Delay);

	UFUNCTION(BlueprintCallable)
		void KillGizmo(); 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KillProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* CastGizmoSFX;

	UPROPERTY(VisibleAnywhere, Category = SFX)
		UAudioComponent* AudioComp;
	
	float KillDuration; 
	float KillTimer; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
		FTimerHandle UnusedHandle; 

};
