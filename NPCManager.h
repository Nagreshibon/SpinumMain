// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNPC1.h"
#include "GameFramework/Actor.h"
#include "NPCManager.generated.h"

UCLASS()
class TOPDOWN_TAKE1_API ANPCManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, ATestNPC1*> List;

	UFUNCTION(BlueprintCallable)
		TMap<FString, ATestNPC1*> GetList() { return List; }

	UFUNCTION()
		bool Add(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		ATestNPC1* GetByName(FString InName);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
