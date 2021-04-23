// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "SpellClass.generated.h"

UENUM(BlueprintType)
enum class ESpellRangeTypes : uint8 {
	SELF	UMETA(DisplayName = "Self"),
	POINT	UMETA(DisplayName = "Point-and-Click"),
	SHOT	UMETA(DisplayName = "Skillshot"),
	HOMING	UMETA(DisplayName = "Homing"),
};

UCLASS(Blueprintable)
class TOPDOWN_TAKE1_API USpellClass : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USpellClass(const FObjectInitializer& Object);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		TSubclassOf<class AActor> SpellGizmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		TSubclassOf<class AActor> CastGizmo;

	AActor* SpellGizmoActual;
	AActor* CastGizmoActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		float CastGizmoKillDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		float SpellGizmoDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		ESpellRangeTypes SpellRangeType; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CastRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CastTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ProjectileVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpellDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpellCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bMobile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAlwaysCastable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTurnTowardsCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* CastSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Icon;

	UFUNCTION(BlueprintCallable)
		virtual bool PreExecuteGameplayEffect(FTransform SpellSpawnLocation);
	
	UFUNCTION(BlueprintCallable)
		virtual void ExecuteGameplayEffect(FTransform SpellSpawnLocation);

	UFUNCTION(BlueprintCallable)
		virtual void PostExecuteGameplayEffect(FTransform SpellSpawnLocation);

	UFUNCTION(BlueprintCallable)
		virtual bool PreCastEffect(FTransform SpellSpawnLocation);

	UFUNCTION(BlueprintCallable)
		virtual float GetCooldown();
	
	UFUNCTION(BlueprintCallable)
		void Kill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MultiStage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* AnimationMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		float AnimationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		float MaxDuration;
	
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
