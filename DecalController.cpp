// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalController.h"
#include "2D/MyDecalActor.h"
#include "Engine/DecalActor.h"
#include "Materials/Material.h"
#include "Components/DecalComponent.h"

ADecalController::ADecalController()
{
}

void ADecalController::SpawnDecal_Crack1(FVector& SpawnLocation)
{
	FHitResult HitResult;
	TSubclassOf<ADecalActor> CrackDecal = MyDecalActor;

	FRotator Rotation = FRotator(0.0f, -90.0f, 0.0f);
	if (CrackDecal)
		AActor* DecalInstance = GetWorld()->SpawnActor(CrackDecal, &SpawnLocation, &Rotation);

	
	//Crack1 = NewObject<USceneComponent>(this, UDecalComponent);
	//Crack1->SetupAttachment(this->GetRootComponent());
	//static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAssetCrack(TEXT("Material'/Game/2D/CIRCLE_RUNE1_Mat'"));
	//if (DecalMaterialAssetCrack.Succeeded())
	//{
	//	Crack1->SetDecalMaterial(DecalMaterialAssetCrack.Object);
	//}
	//Crack1->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	//Crack1->SetWorldLocation(SpawnLocation);
	//Crack1->SetHiddenInGame(false, true);
}