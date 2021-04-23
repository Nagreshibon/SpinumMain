// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MydamageType.generated.h"

/**
 * 

*/

UENUM(BlueprintType)
enum class EDamageTypes : uint8 {
	PHYSICAL=0	UMETA(DisplayName = "Physical"),
	MAGIC	UMETA(DisplayName = "Magic"),
	D_TRUE	UMETA(DisplayName = "True"),
	ANIMA	UMETA(DisplayName = "Anima"),
	DARK	UMETA(DisplayName = "Dark"),
	UNDERGUT	UMETA(DisplayName = "Undergut")
};

UCLASS()
class TOPDOWN_TAKE1_API UMyDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UMyDamageType(const FObjectInitializer& ObjectInitializer); 


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EDamageTypes DamageType; 
	
};
