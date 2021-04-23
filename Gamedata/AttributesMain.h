// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributesMain.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMovementSpeedChangedDelegate, float, PlayerMovementSpeed);

UCLASS()
class TOPDOWN_TAKE1_API UAttributesMain : public UAttributeSet
{
	GENERATED_BODY()

	public:
	UAttributesMain();

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
	FGameplayAttributeData PlayerMovementSpeed;
	ATTRIBUTE_ACCESSORS(UAttributesMain, PlayerMovementSpeed);

	FOnPlayerMovementSpeedChangedDelegate OnPlayerMovementSpeedChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
	FGameplayAttributeData PlayerMovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UAttributesMain, PlayerMovementSpeedMultiplier);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributesMain, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAttributesMain, Armor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UAttributesMain, Resistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UAttributesMain, DamageReduction);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(UAttributesMain, AttackDamage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAttributesMain, ArmorPenetration);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData MagicPenetration;
	ATTRIBUTE_ACCESSORS(UAttributesMain, MagicPenetration);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAttributesMain, AttackSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UAttributesMain, CritChance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData CritPower;
	ATTRIBUTE_ACCESSORS(UAttributesMain, CritPower);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData MeleeRange;
	ATTRIBUTE_ACCESSORS(UAttributesMain, MeleeRange);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		FGameplayAttributeData RangedRange;
	ATTRIBUTE_ACCESSORS(UAttributesMain, RangedRange);
	
	
};
