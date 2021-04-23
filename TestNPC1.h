// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "HealthComponent.h"
#include "UI/FloaterWidget1.h"
#include "SpellClass.h"
#include "Sound/SoundBase.h"
#include "AbilitySystemInterface.h"
#include "TestNPC1.generated.h"


UCLASS()
class TOPDOWN_TAKE1_API ATestNPC1 : public ACharacter
//class ATestNPC1 : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestNPC1();
	
	//use ability

	UFUNCTION(BlueprintCallable)
		float CastAbility(TSubclassOf<USpellClass> SpellToCast);
	UFUNCTION(BlueprintCallable)
		void AbilityCastSuccessful(USpellClass* SpellCast, FTransform SpellSpawnLocation);
	UFUNCTION(BlueprintCallable)
		void AbilityCastSpellKillGizmo(AActor* Gizmo);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<USpellClass> Spell1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		USpellClass* CurrentSpellcast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		APawn* CurrentTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		FTransform CurrentTargetLocation; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		bool bIsCasting; 
	float CastDuration;
	float CastTimer;

	
	
	
		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool isEnemy;
		bool MustRotate;
		FRotator TargetRotation;
		int TestValue;
		FString Welcome;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		bool isDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		bool isStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		int NPCSize;

	//ABILITY HANDLING
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* AbilityComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCAttributes")
		class UAttributesMain* AttributeComponent;

	
	//BASE STATS
		UFUNCTION(BlueprintCallable)
			void UpdateHPBar();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
			UHealthComponent* HealthComponent;

	//COMBAT STATS
		UFUNCTION(BlueprintCallable)
			void Stun(float Duration);
		UFUNCTION(BlueprintCallable)
			void UnStun();

	//DAMAGE PROCESSING
		UFUNCTION()
			void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCause);

	//ANIMATIONS
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
			class UAnimMontage* DamageMontage;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
			class UAnimMontage* DeathMontage;
		UFUNCTION()
			void AADamageAnimations();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
			bool isAttacking; 
	

	//DIALOGUE
		UFUNCTION(BlueprintCallable)
			void Say(FText InText, float Duration = 0.f, USoundBase* SFX = nullptr);
		UFUNCTION()
			void HideFloater_Simple();
	
		UPROPERTY()
			FTimerHandle UnusedHandle22; 
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool bTurnTowardsPlayer; 
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool HasLongDialogue;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool HasShortDialogue;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int BigDialogueIndex;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			int big_i;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FString CurrentResponse;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool bHasUrgentDialogue;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool bHidePortrait; 

		UPROPERTY(BlueprintReadOnly)
		bool bIsInDialogue;

		//DIALOGUE WIDGETS

		UPROPERTY(EditAnywhere, Category = DialogueSystem)
			TSubclassOf<UUserWidget> FloaterWidgetBP;

		UPROPERTY(EditAnywhere, Category = DialogueSystem)
			TSubclassOf<UUserWidget> RadialMenuBP;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
			UWidgetComponent* NameFloaterBlock2;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			UWidgetComponent* RadialMenuBlock1;

		UPROPERTY(VisibleAnywhere)
			UAudioComponent* AudioComp;

		//BASE INFO

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FString CharName;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			FString IDName;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
			UDataTable* BigDialoguePicker;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
			UDataTable* DialoguePicker;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
			UDataTable* NPCLocals;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			UTexture2D* PortraitImage;

	////MOUSE INTERFACE
	////Static Mesh Component to interact with
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//		UStaticMeshComponent* CustomMeshComponent;

	////Function to handle the interaction
	//	UFUNCTION()
	//		void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, Category = DialogueSystem)
		TSubclassOf<UUserWidget> HPWidgetBP;
	UPROPERTY(EditAnywhere)
		UWidgetComponent* HPFloater1;

	//ANIMATIONS



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//

protected:
	UPROPERTY()
		UFloaterWidget1* FloaterWidgetBPRef;

	UPROPERTY()
		FTimerHandle UnusedHandleT;

	UPROPERTY()
		float BaseWalkSpeed;

	UFUNCTION()
		void PostBeginPlay();

	UPROPERTY()
		FTimerHandle UnusedHandlePost; 

};
