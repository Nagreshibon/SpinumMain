// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack1.h"
#include "Engine.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"

void UAttack1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogTemp, Warning, TEXT("Attack notification 1!"));

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComp & Owner exist"));
		ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MeshComp->GetOwner());
		if (MyPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Calling MyPawn AADealDamage"));
			MyPawn->AutoAttackDealDamage();
		}
	}
}
