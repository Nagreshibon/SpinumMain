// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameplayEffect.h"

void UCustomGameplayEffect::ExecuteGameplayEffect(UObject* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("EXECUTING A GAMEPLAY EFFECT"));
	BP_ExecuteGameplayEffect(Target); 
}
