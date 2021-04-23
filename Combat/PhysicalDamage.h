// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyDamageType.h"
#include "PhysicalDamage.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UPhysicalDamage : public UMyDamageType
{
	GENERATED_BODY()

		UPhysicalDamage(const FObjectInitializer& ObjectInitializer);
};
