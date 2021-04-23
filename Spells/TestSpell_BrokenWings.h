// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SpellClass.h"
#include "TestSpell_BrokenWings.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UTestSpell_BrokenWings : public USpellClass
{
	GENERATED_BODY()
		UTestSpell_BrokenWings(const FObjectInitializer& Object);
	
		virtual bool PreExecuteGameplayEffect(FTransform SpellSpawnLocation) override;

		virtual void ExecuteGameplayEffect(FTransform SpellSpawnLocation) override;

		virtual float GetCooldown() override; 

		class UAnimMontage* BrokenWingsMontage;

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hop Parameters")
			float LongCooldown;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hop Parameters")
			float HopMultiplier;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hop Parameters")
			float HopVertical;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			USoundBase* Slash1SFX;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			USoundBase* Slash2SFX;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			USoundBase* Slash3SFX;
};
