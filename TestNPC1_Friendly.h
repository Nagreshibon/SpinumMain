// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "TestNPC1.h"
#include "Components/WidgetComponent.h"
#include "UI/FloaterWidget1.h"
#include "TestNPC1_Friendly.generated.h"


/**
 * 
 */
UCLASS()
class TOPDOWN_TAKE1_API ATestNPC1_Friendly : public ATestNPC1
{
	GENERATED_BODY()


public:
	ATestNPC1_Friendly();
	
	//DIALOGUE HANDLING
	bool bIsInShortDialogue;

	UFUNCTION(BlueprintImplementableEvent)
		void ShowRadialMenu(ATestNPC1* MyNPC);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int DialogueIndex;

	UFUNCTION(BlueprintCallable)
		void SetDialogue(int NewIndex, UDataTable* NewDialoguePicker);

	UFUNCTION(BlueprintCallable)
		virtual void TestDialogue();

	UFUNCTION(BlueprintCallable)
		void TestDialogue2();

	UFUNCTION(BlueprintCallable)
		void TestDialogue3(FText TextIn, float Duration);

	UFUNCTION(BlueprintCallable)
		void HideFloater();
	
	UFUNCTION(BlueprintCallable)
		void HideFloaterChar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ButtonCountCPP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PickedButton;

	int i, k;
	
protected:
	
	virtual void BeginPlay() override;

	//UPROPERTY()
	//UFloaterWidget1* FloaterWidgetBPRef;

	UPROPERTY()
		FTimerHandle UnusedHandle;
	UPROPERTY()
		FTimerHandle UnusedHandle2;
	UPROPERTY()
		FTimerHandle UnusedHandle3;
	UPROPERTY()
		FTimerHandle UnusedHandle4;


	float duration;
	bool bIsAnsweringRadial;

	virtual void Tick(float DeltaSeconds) override;
	
	
	//UFUNCTION()
	//	void OnBoxOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//	void OnBoxEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex);

	//UFUNCTION()
	//	void Talk(USoundBase* SFX, TArray<FSubtitle> Subs);


	
protected:

	//UPROPERTY(VisibleAnywhere)
	//	UAudioComponent* AudioComp;
	
};
