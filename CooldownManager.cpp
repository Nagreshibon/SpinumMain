// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownManager.h"

UCooldownManager::UCooldownManager(const FObjectInitializer& Object) : Super(Object)
{
	
}

bool UCooldownManager::IsCastable(TSubclassOf<USpellClass> Spell)
{
	if (CooldownMap.Find(Spell))
	{
		if (*CooldownMap.Find(Spell) > 0)
			return false;
		else return true; 
	}
	return true;
}

float UCooldownManager::CheckCooldown(TSubclassOf<USpellClass> Spell)
{
	if (CooldownMap.Find(Spell)) return *CooldownMap.Find(Spell);
	return 0.0f;
}

void UCooldownManager::Add(TSubclassOf<USpellClass> Spell, float Cooldown)
{
	CooldownMap.FindOrAdd(Spell) = Cooldown;
	UE_LOG(LogTemp, Warning, TEXT("Adding class: %s"), *(Spell->GetDisplayNameText().ToString()));
}

TMap<TSubclassOf<USpellClass>, float>& UCooldownManager::GetMap()
{
	// TODO: insert return statement here
	return CooldownMap; 
}

void UCooldownManager::Remove(TSubclassOf<USpellClass> Spell)
{
	CooldownMap.Remove(Spell); 
}

void UCooldownManager::ReduceFlat(TSubclassOf<USpellClass> Spell, float Seconds)
{
	if (CooldownMap.Find(Spell))
	{
		*CooldownMap.Find(Spell) = *CooldownMap.Find(Spell) - Seconds; 
	}
}

void UCooldownManager::ReducePercentage(TSubclassOf<USpellClass> Spell, float Percentage)
{
	if (CooldownMap.Find(Spell))
	{
		*CooldownMap.Find(Spell) = *CooldownMap.Find(Spell) - (*CooldownMap.Find(Spell) - Percentage);
	}
}

void UCooldownManager::ReduceAllFlat(float Seconds)
{
	for (auto& Elem : CooldownMap)
	{
		Elem.Value = Elem.Value - Seconds;
		//if (Elem.Value <= 0) CooldownMap.Remove(Elem.Key);
	}
	CooldownMap.ValueSort([](float A, float B) {return A > B; });
}

void UCooldownManager::ReduceAllPercentage(float Percentage)
{
	for (auto& Elem : CooldownMap)
	{
		Elem.Value = Elem.Value - (Elem.Value*Percentage);
		//if (Elem.Value <= 0) CooldownMap.Remove(Elem.Key);
	}
	CooldownMap.ValueSort([](float A, float B) {return A > B; });
}

void UCooldownManager::ResetAll()
{
	CooldownMap.Empty(); 
}

void UCooldownManager::TickCooldowns(float DeltaTime)
{
	for (auto& Elem : CooldownMap)
	{
		Elem.Value = Elem.Value - DeltaTime;
		//if (Elem.Value <= 0) CooldownMap.Remove(Elem.Key);
	}
	CooldownMap.ValueSort([](float A, float B) {return A > B; });
}



