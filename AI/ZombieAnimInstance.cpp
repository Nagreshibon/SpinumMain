// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "../ZombieChar.h"
#include "../TOPDOWN_Take1Character.h"
#include "Math/Rotator.h"

void UZombieAnimInstance::UpdateAnimationProperties()
{
	// Try to get the Pawn being animated and return if a nullptr.
	APawn* ZombiePawn = TryGetPawnOwner();
	if (ZombiePawn == nullptr) return;

	// Try to cast the Pawn to our ZombieCharacter since that's the only
	// thing we want to animate.
	AZombieChar* ZombieCharacter = Cast<AZombieChar>(ZombiePawn);
	if (ZombieCharacter == nullptr) return;

	// Set the variables that are dependent on states.
	bIsRoaming = ZombieCharacter->State == ZombieStates::ROAM;
	bIsChasing = ZombieCharacter->State == ZombieStates::CHASE;
	bIsAttacking = ZombieCharacter->State == ZombieStates::ATTACK;
	bIsDying = ZombieCharacter->State == ZombieStates::DEAD;
}