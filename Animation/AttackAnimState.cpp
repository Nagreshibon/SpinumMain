// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimState.h"

void UAttackAnimState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UAttackAnimState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAttackAnimState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
Super::NotifyEnd(MeshComp, Animation);
}
