// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributesAnatomy.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TOPDOWN_TAKE1_API UAttributesAnatomy : public UAttributeSet
{
	GENERATED_BODY()

	public:
		UAttributesAnatomy(const FObjectInitializer& Object);

		void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnatomyAttributes")
			FGameplayAttributeData Insanity;
		ATTRIBUTE_ACCESSORS(UAttributesAnatomy, Insanity);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnatomyAttributes")
			FGameplayAttributeData Calm;
		ATTRIBUTE_ACCESSORS(UAttributesAnatomy, Calm);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnatomyAttributes")
			FGameplayAttributeData Sorrow;
		ATTRIBUTE_ACCESSORS(UAttributesAnatomy, Sorrow);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnatomyAttributes")
			FGameplayAttributeData Empath;
		ATTRIBUTE_ACCESSORS(UAttributesAnatomy, Empath);

		
};
