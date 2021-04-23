// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "Engine/Classes/Engine/DecalActor.h"
#include "DecalController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API ADecalController : public AController
{
	GENERATED_BODY()

public:
	ADecalController();
	
		UFUNCTION(BlueprintCallable)
		void SpawnDecal_Crack1(FVector& SpawnLocation);

		UPROPERTY(EditDefaultsOnly, Category = Effect)
			TSubclassOf<class ADecalActor> MyDecalActor;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* Crack1;
	
};
