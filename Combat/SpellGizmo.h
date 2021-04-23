// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TOPDOWN_Take1/SpellClass.h"
#include "SpellGizmo.generated.h"


UCLASS()
class TOPDOWN_TAKE1_API ASpellGizmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellGizmo();

	~ASpellGizmo();

	UPROPERTY()
		USceneComponent* DefaultSceneRoot; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* CollisionMesh; 

	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		virtual void BeginKill(float Lifetime);

	UFUNCTION()
		virtual void Kill();

	UPROPERTY()
		FTimerHandle UnusedHandleGizmo; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USpellClass* InstigatorSpell; 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float BaseDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float StunDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* SpellGizmoSFX;

	UPROPERTY(VisibleAnywhere, Category = SFX)
		UAudioComponent* AudioComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UDamageType> DamageClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UProjectileMovementComponent* ProjectileMovement; 

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
