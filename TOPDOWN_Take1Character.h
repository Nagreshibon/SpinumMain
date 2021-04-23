// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/PointLightComponent.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "TOPDOWN_Take1PlayerController.h"
#include "TimerManager.h"
#include "GameplayEffectTypes.h"
#include "DecalController.h"
#include "TestNPC1.h"
#include "AbilitySystemInterface.h"
#include "Actors/ItemPickup.h"
#include "TOPDOWN_Take1Character.generated.h"


UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "Use Spell 1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Spell 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "Use Spell 3"),
	UseAbility4 UMETA(DisplayName = "Use Spell 4"),
	WeaponAbility UMETA(DisplayName = "Use Weapon"), //This finally maps the fifth ability(here designated to be your weaponability, or auto-attack, or whatever) to action mapping "WeaponAbility".
	//You may also do something like define an enum element name that is not actually mapped to an input, for example if you have a passive ability that isn't supposed to have an input. This isn't usually necessary though as you usually grant abilities via input ID,
	//which can be negative while enums cannot. In fact, a constant called "INDEX_NONE" exists for the exact purpose of rendering an input as unavailable, and it's simply defined as -1.
	//Because abilities are granted by input ID, which is an int, you may use enum elements to describe the ID anyway however, because enums are fancily dressed up ints.
};

UCLASS(Blueprintable, config=Game)
class ATOPDOWN_Take1Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UAbilitySystemComponent* GetAbilitySystemComponent() const override //We add this function, overriding it from IAbilitySystemInterface.
	{
		return AbilityComponent;
	}
	
public:
	ATOPDOWN_Take1Character();

	virtual void BeginPlay() override;

	//combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool isAutoAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool isAbilityAnimation;
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void PlayAnimation(UAnimMontage* AnimToPlay, float Rate);
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void PlayAnimationWithDuration(UAnimMontage* AnimToPlay, float Rate, float MaxDuration);
	void PlayAnimationWithDuration(UAnimMontage* AnimToPlay, float Rate, float MaxDuration, FName Section);
		void KillAnimation();
		FTimerHandle UnusedHandleAnim; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		ATestNPC1* CurrentEnemyChar;
	bool IsWalkingToEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bInvulnerable;
	
	float AARange;
	float AADamage;
	float PushMagnitude;
	UFUNCTION()
		void AutoAttackDealDamage();
	
	UFUNCTION()
		void LaunchNuller();

	//INVENTORY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<AItemPickup*> Inventory; 
	
	//LIGHT COMPONENT
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPointLightComponent* LightComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TestBox;


	//PLAYER ABILITIES
	UFUNCTION(BlueprintCallable)
		void Ability_Aegis(FVector Target);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AegisCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AegisDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> AegisEffect; 
	
		bool AegisDisabled = false;
		UPROPERTY()
	FTimerHandle handle;
	
	UFUNCTION()
		void Ability_Aegis_KillGizmo(AActor* Gizmo);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UParticleSystemComponent* WeaponTrailFX; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		TSubclassOf<class AActor> AegisGizmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsCasting; 

	//collision additional
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionSphereBig;
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
		void OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void PlungeLandingAnim(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void PostInitializeComponents() override;
	
	//BASE STATS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		int HP_MAX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		int HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float CritChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float CritMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float BaseArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float BaseResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
		float BaseDamageReduction;

	//COMBAT HANDLING
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			AController* EventInstigator,
			AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		TSubclassOf<UDamageType> AutoAttackDamageType; 
	
	//ABILITY HANDLING
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* AbilityComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		TSubclassOf<class UGameplayAbility> Ability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAttributes")
		class UAttributesMain* PlayerAttributeComponent;

	UFUNCTION(BlueprintCallable)
	UAttributesMain* GetAttributeComponent()
	{
		return PlayerAttributeComponent;
	}
	
	UFUNCTION()
		void OnPlayerMovementSpeedChanged(float NewPlayerMovementSpeed);
	UFUNCTION()
		virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
		virtual void SpeedChanged(const FOnAttributeChangeData& Data);

	
	UPROPERTY(EditAnywhere, Category = DialogueSystem)
		TSubclassOf<UUserWidget> FloaterWidgetBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* PortraitImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = DialogueSystem)
		UAudioComponent* AudioComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString IDName;
	UPROPERTY(EditAnywhere)
		UWidgetComponent* NameFloaterBlock2;
	//PLUNGE DECAL
	UFUNCTION(BlueprintCallable)
		void SpawnDecal_Crack1(FVector CrackLocationV);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		TSubclassOf<class ADecalActor> ToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UParticleSystem* ToSpawn2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* NPCLocals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* QuestLog;


	//UFUNCTION(BlueprintCallable)
	//	void DoubleJump();

	//UPROPERTY()
	//	int DoubleJumpCounter;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	float JumpHeight;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isPerformingAttack;
	bool IsJumping;
	bool IsPlunging;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTalking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLanding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAscending;
	bool IsWalkingToNPC;
	bool IsInDialogue;


	//zoom out while ascending
	UPROPERTY()
		float ZoomOutFull;
	UFUNCTION(BlueprintCallable)
		void ZoomTo();

	float ZoomRate;
	float ZoomSize;
	float ZoomTick; 
	float OriginalBoomLength;

	UPROPERTY()
		FTimerHandle UnusedHandle;

	//Rotate towards NPC
	bool MustRotate;
	FRotator TargetRotation;

	//ANIMATIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* RiseFallMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* AttackMontage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* AttackMontage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* LandingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimMontage* TalkMontage;
	
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* Target_Runes;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		bool bCustomCamera; 

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* UnlockedCameraComponent;

	UFUNCTION()
		void DialogueCameraControl(FVector TargetLocation, float TargetZoom, float DeltaTime, int NPCSize), DialogueCameraControl(ACharacter* TargetCharacter, float TargetZoom, float DeltaTime);

	float DialogueCamera_CurrentTime;
	float DialogueCamera_TargetZoom;
	float DialogueCamera_TargetTime;

	UPROPERTY()
		FVector DialogueCamera_TargetVector;
	
	UFUNCTION()
		void CameraUnlock();
	UFUNCTION()
		void CameraLock();
	UFUNCTION()
		void Zoom(float DeltaZoom);
	UPROPERTY()
		bool isCameraLocked;
	bool isDialogueCamera;
	UPROPERTY()
	float CameraMoveDelta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		ATestNPC1* CameraDialogueCharacter;

	//ABILITY HANDLES
	int BrokenWingsCounter; 
	
protected:

	//virtual void SetupInputComponent();

	virtual void Landed(const FHitResult& Hit);

private:

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

};

