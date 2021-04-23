// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TOPDOWN_Take1/SpellClass.h"
#include "TestSpell1.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UTestSpell1 : public USpellClass
{
	GENERATED_BODY()

public:
		UTestSpell1(const FObjectInitializer& Object);

	//virtual void NativeConstruct() override; 
};
