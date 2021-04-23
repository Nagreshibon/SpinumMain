// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_Attack1.h"
#include "Engine.h"
#include "TOPDOWN_Take1/ZombieChar.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"

void UZombie_Attack1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogTemp, Warning, TEXT("Zombie Attack notification 1!"));

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComp & Owner exist"));
		if(AZombieChar* MyPawn = Cast<AZombieChar>(MeshComp->GetOwner()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Calling Zombie AADealDamage"));
			MyPawn->Attack1SpawnGizmo(MyPawn->CurrentTarget); 
		}
	}
}
