// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CooldownManager.h"
#include "CoreMinimal.h"
#include "DialogueData/BigTalkWidget.h"
#include "TOPDOWN_Take1Character.h"
#include "GameFramework/PlayerController.h"
#include "TestNPC1.h"
#include "UI/CastBar.h"
#include "NPC/Sidekick.h"
#include "NPCManager.h"
#include "SpellClass.h"
#include "Spells/TestSpell1.h"
#include "TOPDOWN_Take1PlayerController.generated.h"

UCLASS()
class ATOPDOWN_Take1PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATOPDOWN_Take1PlayerController();
	
	virtual void BeginPlay() override;


	//NPC MANAGER
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ANPCManager* NPCManager;
	
	//SIDEKICK

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sidekick")
	ASidekick* Sidekick;

	UFUNCTION(BlueprintCallable)
		void SidekickDialogue(UDataTable* NewDialoguePicker, int NewIndex);

	//INVENTORY POINTERS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<UUserWidget> InventoryWidget_BP;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* InventoryWidget;

	UFUNCTION(BlueprintCallable)
		void ToggleInventoryWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		ATestNPC1* ItemHelperActor;

	
	//IDEAS POINTERS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ideas")
		TSubclassOf<UUserWidget> IdeaWidget_BP;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* IdeaWidget;

	UFUNCTION(BlueprintCallable)
		void ShowIdeaWidget();

	UFUNCTION(BlueprintCallable)
		void ToggleIdeaWidget(); 

	UFUNCTION(BlueprintCallable)
		void HideIdeaWidget();
	
	//JOURNAL POINTERS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		TSubclassOf<UUserWidget> JournalWidget_BP;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* JournalWidget; 
	
	UFUNCTION(BlueprintCallable)
		void ShowJournalWidget();

	UFUNCTION(BlueprintCallable)
		void HideJournalWidget();
		
	

	//INTERACT

	bool LockInteraction;
	
	UFUNCTION(BlueprintCallable)
		void InteractBranch();

	UFUNCTION(BlueprintCallable)
		void StartDialogue(ATestNPC1* MyNPC);

	UFUNCTION(BlueprintCallable)
		void StartBigDialogue(ATestNPC1* MyNPC);

	UFUNCTION(BlueprintImplementableEvent)
		void InitShortDialogue(ATestNPC1* MyNPC);

	FVector CurrentNPCLocation;
	FRotator CurrentNPCRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATestNPC1* CurrentNPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		ATestNPC1* CurrentEnemy;

	//DIALOGUE HANDLING
	UPROPERTY()
		float DialogueRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TSubclassOf<UUserWidget> BigTalkWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TSubclassOf<UUserWidget> BigTalkWidget_Left_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TSubclassOf<UUserWidget> BigTalkWidget_Chapter_BP;

	
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* BigTalkWidget1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UUserWidget* BigTalkWidgetCurrent;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* BigTalkWidget1_Left;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* BigTalkWidget2;
	
	
	//COMBAT
		//autoattack
	UFUNCTION()
		void AutoAttack();
	UFUNCTION()
		void AutoAttackStart(ATestNPC1* MyEnemy);
	UPROPERTY()
		bool AutoAttackActive;
	UFUNCTION()
		void AutoAttackCancel();

	void AutoAttackRelease();

	//ABILITIES
	UFUNCTION()
		void AbilityQ();
	UFUNCTION()
		void AbilityControlQ();
	UFUNCTION()
		void AbilityW();
	UFUNCTION()
		void AbilityT();
	UFUNCTION()
		void AbilityF();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<UUserWidget> CastBar_BP;
	UPROPERTY()
		UCastBar* CastBar; 
	UFUNCTION(BlueprintCallable)
		void AbilityCastSpell(TSubclassOf<USpellClass> Spell);
	UFUNCTION(BlueprintCallable)
		void AbilityCastSpellSuccessful(USpellClass* SpellCast, TSubclassOf<USpellClass> Spell, FTransform SpellSpawnLocation);
	UFUNCTION(BlueprintCallable)
		void AbilityCastSpellKillGizmo(AActor* Gizmo); 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<USpellClass> Spell1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<USpellClass> Spell2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<USpellClass> Spell3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<USpellClass> Spell4;
	UPROPERTY()
		USpellClass* TwoStageSpell;
	UPROPERTY()
		int SerialAbilityCounter;
	UPROPERTY()
		UCooldownManager* CooldownManager;
	
	//dash
	
	UFUNCTION(BlueprintCallable)
		void DoubleJump();

	UPROPERTY()
		int DoubleJumpCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpHeight;

	void SetNewMoveDestination(const FVector DestLocation);

	bool CanDash;
	float DashStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DashMulti;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DashCooldown;
	
	UFUNCTION()
		void StopDashing();

	UFUNCTION()
		void ResetDash();

	//PLUNGE

	UPROPERTY()
		float PlungeDamage;

	UPROPERTY()
		float PlungeDamageRadius;
	
	UFUNCTION(BlueprintCallable)
		void PlungeBranch();

	UFUNCTION(BlueprintCallable)
		void Ascend();

	UFUNCTION(BlueprintCallable)
		void Plunge();

		bool CanPlunge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlungeRise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlungeCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RiseHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlungeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlungeMax;

	UFUNCTION()
		void PitchNuller();

	UFUNCTION()
		void StopPlunging();

	UFUNCTION()
		void ResetPlunge();

	float FlightTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector PlungeLocation;

	UPROPERTY()
		FVector CrackLocation;
	
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	FVector VelocityRotated;

	//COMBAT
	float AAMoveDelay;
	
	/*virtual void Landed(const FHitResult& Hit);*/

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */


	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

private:
	int AAIndex;
	
	UFUNCTION()
		void ZoomIn();
	
	UFUNCTION()
		void ZoomOut();

	UFUNCTION(BlueprintCallable)
		void CameraUnlockToggle();

	float DeltaZoom;

	UPROPERTY()
		FTimerHandle UnusedHandle;
	UPROPERTY()
		FTimerHandle UnusedHandle2;
	UPROPERTY()
		FTimerHandle UnusedHandle3;
	UPROPERTY()
		FTimerHandle UnusedHandle4;
	
};


