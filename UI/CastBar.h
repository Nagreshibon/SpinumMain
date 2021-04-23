// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TOPDOWN_Take1/SpellClass.h"

#include "Components/ProgressBar.h"
#include "CastBar.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API UCastBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UCastBar(const FObjectInitializer& ObjectInitializer);

	float CastTimer;
	float CastDuration;

	UPROPERTY()
		TSubclassOf<USpellClass> SpellClass; 
	
	UPROPERTY()
		USpellClass* SpellCast;

	UPROPERTY()
		FTransform SpawnLocation;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* CastProgressBar; 

	UFUNCTION(BlueprintCallable)
		void ShowBar();

	UFUNCTION(BlueprintCallable)
		void ResetBar();
	
	UFUNCTION(BlueprintCallable)
		void Interrupt();

	UFUNCTION(BlueprintCallable)
		void MoveInterrupt();

	UFUNCTION(BlueprintCallable)
		void StartCastBar(float Duration);

	UFUNCTION(BlueprintCallable)
		void CastSpellSuccessful();

	UFUNCTION()
		AActor* CreateCastGizmo(TSubclassOf<AActor> GizmoClass, FTransform TargetTransform);
	UFUNCTION()
		void KillCastGizmo(AActor* Gizmo);

	UPROPERTY()
		FTimerHandle UnusedHandle; 
	
	
	UPROPERTY()
		AActor* CastGizmo;


	virtual void NativeConstruct() override; 
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
