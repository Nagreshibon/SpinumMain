// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicDamage.h"

UMagicDamage::UMagicDamage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DamageType = EDamageTypes::MAGIC;
}