// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TOPDOWN_Take1Character.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Gamedata/AttributesMain.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD_Default2.h"
#include "UI/HUD_Default1.h"
#include "UI/FloaterWidget1.h"
#include "Combat/MyDamageType.h"

ATOPDOWN_Take1Character::ATOPDOWN_Take1Character()
{
	//init
	bCustomCamera = false; 
	IsJumping = false;
	IsPlunging = false;
	bIsCasting = false; 
	IsWalkingToNPC = false;
	ZoomRate = 50.0f;
	ZoomOutFull = 2200.0f;
	ZoomTick = 0.01f;
	isCameraLocked = true;
	CameraMoveDelta = 15.f;
	AARange = 200.0f;
	AttackSpeed = 1.577f;
	AADamage = 12;
	PushMagnitude = 200;
	bInvulnerable = false; 
	CritChance = 0.15;
	CritMultiplier = 2.5;
	HP = 200;
	HP_MAX = 200;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	HealthComponent->HP = 200;
	HealthComponent->HP_MAX = 200;

	

	//ABILTIES
	AegisCooldown = 12.0f;
	AegisDuration = 3.0f;
	BrokenWingsCounter = 1; 

	/*ConstructorHelpers::FObjectFinder<UGameplayEffect> AegisEffectObject(TEXT("Blueprint'/Game/TopDownCPP/Blueprints/Ability/GE_Test2'"));
	if (AegisEffectObject.Succeeded())
	{
		AegisEffect = AegisEffectObject.Object;
	}*/
	
	//anims
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RiseMontageObject(TEXT("AnimMontage'/Game/Mannequin/Serath_Anim_Retarget/TestMontage.TestMontage'"));
	if (RiseMontageObject.Succeeded())
	{
		RiseFallMontage = RiseMontageObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage2Object(TEXT("AnimMontage'/Game/Mannequin/Serath_Anim_Retarget/AutoAttackMontage2.AutoAttackMontage2'"));
	if (AttackMontage2Object.Succeeded())
	{
		AttackMontage2 = AttackMontage2Object.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage1Object(TEXT("AnimMontage'/Game/Mannequin/Serath_Anim_Retarget/AutoAttackMontage.AutoAttackMontage'"));
	if (AttackMontage1Object.Succeeded())
	{
		AttackMontage1 = AttackMontage1Object.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> LandingMontageObject(TEXT("AnimMontage'/Game/Mannequin/Serath_Anim_Retarget/LandingMontage.LandingMontage'"));
	if (LandingMontageObject.Succeeded())
	{
		LandingMontage = LandingMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TalkMontageObject(TEXT("AnimMontage'/Game/Mannequin/Serath_Anim_Retarget/TalkMontage.TalkMontage'"));
	if (TalkMontageObject.Succeeded())
	{
		TalkMontage = TalkMontageObject.Object;
	}
	
	
	//damage processing

	//ABILITY HANDLING
	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName(TEXT("AbilityComponent")));
	PlayerAttributeComponent = CreateDefaultSubobject<UAttributesMain>(FName(TEXT("AttributeComponent")));

	
	//ATTRIBUTES
	/*PlayerAttributeComponent->SetArmor(Armor);
	PlayerAttributeComponent->SetResistance(Resistance);
	PlayerAttributeComponent->SetDamageReduction(BaseDamageReduction);*/
	//AbilityComponent->SetIsReplicated(true);
	//AbilityComponent->InitAbilityActorInfo(this, this);
		
	//setup floater block
	NameFloaterBlock2 = CreateDefaultSubobject<UWidgetComponent>(FName("NameFloaterBlock2"));
	NameFloaterBlock2->SetupAttachment(RootComponent);
	NameFloaterBlock2->SetWidgetSpace(EWidgetSpace::Screen);
	NameFloaterBlock2->SetHiddenInGame(true);

	//setup audio component
	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->AttachTo(GetRootComponent());

	//weapon particle fx
	WeaponTrailFX = CreateDefaultSubobject<UParticleSystemComponent>(FName("Weapon Trail FX"));
	FAttachmentTransformRules TransRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
	WeaponTrailFX->AttachToComponent(GetMesh(), TransRules, FName("evil_weapon_blades"));
	WeaponTrailFX->SetAutoAttachmentParameters(GetMesh(), FName("FX_Mace_01"), EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative);

	
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//collision additional
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetHiddenInGame(true);
	CollisionSphere->SetSphereRadius(100.f);
	CollisionSphere->SetGenerateOverlapEvents(false);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATOPDOWN_Take1Character::OnOverlapsBegin);

	CollisionSphereBig = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphereBig"));
	CollisionSphereBig->SetupAttachment(RootComponent);
	CollisionSphereBig->SetHiddenInGame(true);
	CollisionSphereBig->SetSphereRadius(250.f);
	CollisionSphereBig->SetGenerateOverlapEvents(false);
	CollisionSphereBig->OnComponentBeginOverlap.AddDynamic(this, &ATOPDOWN_Take1Character::PlungeLandingAnim);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	UnlockedCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("UnlockedCamera"));
	UnlockedCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	UnlockedCameraComponent->bUsePawnControlRotation = false;

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/2D/CIRCLE_RUNE1_Mat'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	//LIGHT
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(FName(TEXT("FollowerLight")));
	LightComponent->SetupAttachment(CursorToWorld);
	
	//TestBox = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("TestBox")));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	//if (SphereVisualAsset.Succeeded()) {
	//	TestBox->SetStaticMesh(SphereVisualAsset.Object);
	//	TestBox->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	//	TestBox->SetWorldScale3D(FVector(0.8f));
	//}
	//TestBox->SetupAttachment(LightComponent);
	
	//PLUNGE DECAL
		//SHOW DECAL
	Target_Runes = CreateDefaultSubobject<UDecalComponent>(TEXT("Target_Runes"));
	Target_Runes->SetupAttachment(this->GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset2(TEXT("Material'/Game/2D/CIRCLE_RUNE1_Mat'"));
	if (DecalMaterialAsset2.Succeeded())
	{
		Target_Runes->SetDecalMaterial(DecalMaterialAsset2.Object);
	}
	Target_Runes->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	Target_Runes->SetHiddenInGame(true, true);
	//Target_Runes->DecalSize = FVector(320.0f, 320.0f, 32.0f);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void ATOPDOWN_Take1Character::BeginPlay()
{
	Super::BeginPlay();

		
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
	{
		AbilityComponent->BindAbilityActivationToInputComponent(MyController->InputComponent, FGameplayAbilityInputBinds("ConfirmInput", "Cancel6Input", "AbilityInput"));

			if (AHUD_Default2* MyHUD = Cast<AHUD_Default2>(MyController->GetHUD()))
			{
				if (UHUD_Default1* MyHUDWidget = Cast<UHUD_Default1>(MyHUD->HUDWidget))
				{
					MyHUDWidget->UpdateHPBar();
				}
			}

	}

	if (AbilityComponent)
	{
		AbilityComponent->InitStats(UAttributesMain::StaticClass(), NULL);
		
		AbilityComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ATOPDOWN_Take1Character::OnActiveGameplayEffectAddedCallback);
		AbilityComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeComponent->GetPlayerMovementSpeedAttribute()).AddUObject(this, &ATOPDOWN_Take1Character::SpeedChanged);
		FGameplayAbilityActorInfo* actorInfo = new FGameplayAbilityActorInfo();
		actorInfo->InitFromActor(this, this, AbilityComponent);
		AbilityComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(actorInfo);
		if (HasAuthority() && Ability)
		{
			AbilityComponent->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		}
		AbilityComponent->InitAbilityActorInfo(this, this);

		//ATTRIBUTES
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetArmorAttribute(), BaseArmor);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetResistanceAttribute(), BaseResistance);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetDamageReductionAttribute(), BaseDamageReduction);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetAttackDamageAttribute(), AADamage);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetAttackSpeedAttribute(), AttackSpeed);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetMeleeRangeAttribute(), AARange);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetHealthAttribute(), HP_MAX);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetCritChanceAttribute(), CritChance);
		AbilityComponent->SetNumericAttributeBase(PlayerAttributeComponent->GetCritPowerAttribute(), CritMultiplier);
		
				
	}

	PlayerAttributeComponent->OnPlayerMovementSpeedChanged.AddDynamic(this, &ATOPDOWN_Take1Character::OnPlayerMovementSpeedChanged);
	

	if (FloaterWidgetBP) NameFloaterBlock2->SetWidgetClass(FloaterWidgetBP);

	if (UFloaterWidget1* MyFloater = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting portrait"));
		MyFloater->PortraitBlock->SetBrushFromTexture(PortraitImage);
	}
}

float ATOPDOWN_Take1Character::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	/*Super::TakeDamage(DamageAmount,
		 DamageEvent,
		 EventInstigator,
		 DamageCauser);*/
	if(!bInvulnerable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying damage, amount: %f"), DamageAmount);

		TSubclassOf<UDamageType> DamageClass = DamageEvent.DamageTypeClass;

		if (UMyDamageType* MyDamageClass = Cast<UMyDamageType>(DamageClass->GetDefaultObject()))
		{
			float DamageReduction = AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetDamageReductionAttribute());
			float DamageAmountReduced = FMath::Max(DamageAmount - DamageReduction, 0.f);
			float DamageMultiplier = 1.0f;


			switch (MyDamageClass->DamageType)
			{
			case EDamageTypes::PHYSICAL:
			{
				float CurrentArmor = AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetArmorAttribute());

				if (CurrentArmor >= 0)
				{
					DamageMultiplier = 100 / (100 + CurrentArmor);
					HP = HP - (DamageAmountReduced * DamageMultiplier);
					HealthComponent->HP = HealthComponent->HP - (DamageAmount * DamageMultiplier);

				}
				else
				{
					DamageMultiplier = 2 - (100 / (100 - CurrentArmor));
					HP = HP - (DamageAmountReduced * DamageMultiplier);
					HealthComponent->HP = HealthComponent->HP - (DamageAmount * DamageMultiplier);
				}

				break;
			}

			case EDamageTypes::MAGIC:
			{
				float CurrentMR = AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetResistanceAttribute());

				if (CurrentMR >= 0)
				{
					DamageMultiplier = 100 / (100 + CurrentMR);
					HP = HP - (DamageAmountReduced * DamageMultiplier);
					HealthComponent->HP = HealthComponent->HP - (DamageAmount * DamageMultiplier);

				}
				else
				{
					DamageMultiplier = 2 - (100 / (100 - CurrentMR));
					HP = HP - (DamageAmountReduced * DamageMultiplier);
					HealthComponent->HP = HealthComponent->HP - (DamageAmount * DamageMultiplier);
				}

				break;
			}

			case EDamageTypes::D_TRUE:
			{
				HP = HP - DamageAmount;
				HealthComponent->HP = HealthComponent->HP - DamageAmount;
				break;
			}

			case EDamageTypes::ANIMA:
			{
				break;
			}

			case EDamageTypes::DARK:
			{
				break;
			}



			default:
			{
				HP = HP - DamageAmount;
				HealthComponent->HP = HealthComponent->HP - DamageAmount;
				break;
			}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DamageClass cast failed! %s"), *((DamageEvent.DamageTypeClass->GetDefaultObjectName()).ToString()));

		}

		if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
		{
			if (AHUD_Default2* MyHUD = Cast<AHUD_Default2>(MyController->GetHUD()))
			{
				if (UHUD_Default1* MyHUDWidget = Cast<UHUD_Default1>(MyHUD->HUDWidget))
				{
					GetCharacterMovement()->StopMovementImmediately();
					MyController->AutoAttackCancel();
					MyHUDWidget->UpdateHPBar();
				}
			}
		}

		return DamageAmount;
	}
	return 0;
}

void ATOPDOWN_Take1Character::OnPlayerMovementSpeedChanged(float NewPlayerMovementSpeed)
{
	if (NewPlayerMovementSpeed < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVEMENT SPEED LESS THAN 0"));
		return;
	}

	//GetCharacterMovement()->MaxWalkSpeed = NewPlayerMovementSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("READING PLAYER MOVEMENT SPEED FROM MOVECOMP, post-change: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ATOPDOWN_Take1Character::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("EFFECT APPLICATION CALLBACK"));
}

void ATOPDOWN_Take1Character::SpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	UE_LOG(LogTemp, Warning, TEXT("SpeedChanged: READING PLAYER MOVEMENT SPEED FROM MOVECOMP, post-change: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

//collision additional

void ATOPDOWN_Take1Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (RootComponent)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATOPDOWN_Take1Character::OnOverlapsBegin);
		//CollisionSphereBig->OnComponentBeginOverlap.AddDynamic(this, &ATOPDOWN_Take1Character::PlungeLandingAnim);
		//CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ATOPDOWN_Take1Character::OnOverlapsEnd);
	}
}

void ATOPDOWN_Take1Character::PlungeLandingAnim(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BIG OVERLAP DETECTED"));
	IsLanding = true;
	PlayAnimMontage(LandingMontage, 1.9f, FName(""));
}

void ATOPDOWN_Take1Character::PlayAnimation(UAnimMontage* AnimToPlay, float Rate)
{
	//UE_LOG(LogTemp, Warning, TEXT("PLAYING ABILITY ANIM"));
	isAbilityAnimation = true;
	PlayAnimMontage(AnimToPlay, Rate, FName(""));
	FTimerDelegate KillGizmoDelegate = FTimerDelegate::CreateUObject(this, &ATOPDOWN_Take1Character::KillAnimation);
	GetGameInstance()->GetWorld()->GetTimerManager().SetTimer(UnusedHandleAnim, this, &ATOPDOWN_Take1Character::KillAnimation, AnimToPlay->GetPlayLength()*Rate, false);
}

void ATOPDOWN_Take1Character::PlayAnimationWithDuration(UAnimMontage* AnimToPlay, float Rate, float MaxDuration)
{
	//UE_LOG(LogTemp, Warning, TEXT("PLAYING ABILITY ANIM WITH MAX DURATION"));
	isAbilityAnimation = true;
	PlayAnimMontage(AnimToPlay, Rate, FName(""));
	FTimerDelegate KillGizmoDelegate = FTimerDelegate::CreateUObject(this, &ATOPDOWN_Take1Character::KillAnimation);
	GetGameInstance()->GetWorld()->GetTimerManager().SetTimer(UnusedHandleAnim, this, &ATOPDOWN_Take1Character::KillAnimation, MaxDuration, false);
}

void ATOPDOWN_Take1Character::PlayAnimationWithDuration(UAnimMontage* AnimToPlay, float Rate, float MaxDuration, FName Section)
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYING ABILITY ANIM WITH MAX DURATION AND SECTION"));
	isAbilityAnimation = true;
	PlayAnimMontage(AnimToPlay, Rate, Section);
	FTimerDelegate KillGizmoDelegate = FTimerDelegate::CreateUObject(this, &ATOPDOWN_Take1Character::KillAnimation);
	GetGameInstance()->GetWorld()->GetTimerManager().SetTimer(UnusedHandleAnim, this, &ATOPDOWN_Take1Character::KillAnimation, MaxDuration, false);
}

void ATOPDOWN_Take1Character::KillAnimation()
{
	//UE_LOG(LogTemp, Warning, TEXT("KILLING ANIM"));
	isAbilityAnimation = false; 
}


void ATOPDOWN_Take1Character::OnOverlapsBegin_Implementation(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP DETECTED"));
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
	{
		MyController->PitchNuller();
	}
	
}


void ATOPDOWN_Take1Character::Zoom(float DeltaZoom)
{
	CameraBoom->TargetArmLength = (CameraBoom->TargetArmLength) + DeltaZoom;
	if ((CameraBoom->TargetArmLength) > 2000.0f) CameraBoom->TargetArmLength = 2000.0f;
	if ((CameraBoom->TargetArmLength) < 300.0f) CameraBoom->TargetArmLength = 300.0f;
}

void ATOPDOWN_Take1Character::DialogueCameraControl(FVector TargetLocation, float TargetZoom, float DeltaTime, int NPCSize)
{
	DialogueCamera_CurrentTime = 0;
	DialogueCamera_TargetZoom = TargetZoom;
	DialogueCamera_TargetTime = DeltaTime;
	FVector CameraBoomVector = FVector(-200-NPCSize, 0+NPCSize,-1*TargetZoom*sin(60));
	DialogueCamera_TargetVector = TargetLocation + CameraBoomVector;
	isDialogueCamera = true;
	
}

void ATOPDOWN_Take1Character::DialogueCameraControl(ACharacter* TargetCharacter, float TargetZoom, float DeltaTime)
{
	if (TargetCharacter)
	{
		DialogueCamera_CurrentTime = 0;
		DialogueCamera_TargetZoom = TargetZoom;
		DialogueCamera_TargetTime = DeltaTime;
		FVector CameraBoomVector = FVector(0, TargetZoom * cos(60), TargetZoom * sin(60));
		DialogueCamera_TargetVector = TargetCharacter->GetActorLocation() + CameraBoomVector;
		isDialogueCamera = true;
	}
}

void ATOPDOWN_Take1Character::CameraUnlock()
{
	UE_LOG(LogTemp, Warning, TEXT("Unlocking camera"));
	UnlockedCameraComponent->SetWorldLocation(TopDownCameraComponent->GetComponentLocation());
	UnlockedCameraComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,true));
	TopDownCameraComponent->SetActive(false);
	UnlockedCameraComponent->SetActive(true);
	isCameraLocked = false;
}

void ATOPDOWN_Take1Character::CameraLock()
{
	isDialogueCamera = false;
	UE_LOG(LogTemp, Warning, TEXT("Locking camera"));
	UnlockedCameraComponent->SetWorldLocation(TopDownCameraComponent->GetComponentLocation());
	UnlockedCameraComponent->SetActive(false);
	TopDownCameraComponent->SetActive(true);
	isCameraLocked = true;
}

//AUTOATTACK HANDLING
void ATOPDOWN_Take1Character::AutoAttackDealDamage()
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
	{
		if (CurrentEnemyChar)
		{
			float Distance = FVector::Dist(GetActorLocation(), CurrentEnemyChar->GetActorLocation());
			FRotator MyRotation = GetActorRotation();
			FRotator NPCRotate = FRotator(0, (CurrentEnemyChar->GetActorLocation() - GetActorLocation()).ToOrientationRotator().Yaw, 0);

			float DeltaYawEnemy = MyRotation.Yaw - NPCRotate.Yaw;
						
			if (abs(DeltaYawEnemy) < 20.0f && Distance < AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetMeleeRangeAttribute()))
			{
				
				float DamageDealt;
				if (CritChance >= FMath::RandRange(0.0f,1.0f))
				{					
					DamageDealt = AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetAttackDamageAttribute()) * AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetCritPowerAttribute());
					UE_LOG(LogTemp, Warning, TEXT("Crit! %f"), DamageDealt);
				}
				else
				{
					DamageDealt = AbilityComponent->GetNumericAttribute(PlayerAttributeComponent->GetAttackDamageAttribute());
				}

				
				FRotator PushRotator = FRotator((CurrentEnemyChar->GetActorLocation() - GetActorLocation()).ToOrientationRotator());
				FVector PushVector = PushRotator.Vector()*PushMagnitude;

				//TSubclassOf<UDamageType> P; //damage type placeholder
				
				UGameplayStatics::ApplyDamage(CurrentEnemyChar, DamageDealt, MyController, this, AutoAttackDamageType);
				
				UE_LOG(LogTemp, Warning, TEXT("Pushing: %s"), *PushVector.ToString());
				CurrentEnemyChar->LaunchCharacter(PushVector, false, false);
			}
		}
	}
}


//LANDING
void ATOPDOWN_Take1Character::Landed(const FHitResult& Hit)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("LANDED"));
		MyController->DoubleJumpCounter = 0;
		GetCharacterMovement()->MaxWalkSpeed = 600;

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Hit.ImpactPoint);

		//UE_LOG(LogTemp, Warning, TEXT("Landing location: %s"), *(Hit.Location).ToString());
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		FRotator foo = GetActorRotation();
		SetActorRotation(FRotator(0, foo.Yaw, 0));

		if (IsJumping == true)	 MyController->StopDashing();
		if (IsPlunging == true)	 
		{
			UE_LOG(LogTemp, Warning, TEXT("Stop plunging called from landing"));
			MyController->StopPlunging();
		}

		if (IsJumping == true)	 MyController->CanDash = false;
		if (IsPlunging == true)	 MyController->CanPlunge = false;

		if (IsAscending == true)
		{
			ZoomSize = OriginalBoomLength;
			ZoomRate = 70;
			ZoomTo();
			IsAscending = false;
			Target_Runes->SetHiddenInGame(true, true);
		}
	}
}

//PLUNGE LANDING DECAL

void ATOPDOWN_Take1Character::SpawnDecal_Crack1(FVector CrackLocationV)
{
		UE_LOG(LogTemp, Warning, TEXT("TRYING TO SPAWN DECAL"));
		FHitResult HitResult;
		//TSubclassOf<ADecalActor> CrackDecal = MyDecalActor;
	
		float RandomRotation = FMath::RandRange(0.0f, 360.0f);
		FRotator Rotation = FRotator(0.0f, RandomRotation, 0);
		if (ToSpawn)
		{
			FActorSpawnParameters spawnParams;

			GetWorld()->SpawnActor<ADecalActor>(ToSpawn, CrackLocationV, Rotation, spawnParams);

			UE_LOG(LogTemp, Warning, TEXT("CRACK DECAL EXISTS"));
		}
		if (ToSpawn2)
		{
			FActorSpawnParameters spawnParams;

			//GetWorld()->SpawnActor<UParticleSystem>(ToSpawn2, CrackLocationV, Rotation, spawnParams);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ToSpawn2, CrackLocationV);

			UE_LOG(LogTemp, Warning, TEXT("CRACK DECAL EXISTS"));
		}
}

void ATOPDOWN_Take1Character::LaunchNuller()
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("NULLIFIED"));
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());
		//MyController->StopDashing();
	}
	
}

void ATOPDOWN_Take1Character::Ability_Aegis(FVector Target)
{
	if (!AegisDisabled) {
		
		FTransform SpawnLocation = GetActorTransform();
		if (AegisGizmo)
		{
			AegisDisabled = true;
			AActor* Gizmo = GetWorld()->SpawnActor<AActor>(AegisGizmo, SpawnLocation);
			Gizmo->SetActorLocationAndRotation(SpawnLocation.GetLocation(), SpawnLocation.GetRotation());
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
			Gizmo->AttachToActor(this, AttachRules);

			FTimerDelegate KillGizmo = FTimerDelegate::CreateUObject(this, &ATOPDOWN_Take1Character::Ability_Aegis_KillGizmo, Gizmo);
			GetWorldTimerManager().SetTimer(UnusedHandle, KillGizmo, AegisDuration, false);
			
			
				
				GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
					UE_LOG(LogTemp, Warning, TEXT("Re-enabling Aegis after cooldown"));
					AegisDisabled = false;
				}, AegisCooldown, 1);
		}
		FVector MyLocation = GetActorLocation();
		FRotator TargetRotator = FRotator(0, (Target-MyLocation).ToOrientationRotator().Yaw, 0);
		SetActorRotation(TargetRotator);

		float DashMulti = 2000.f;
		float JumpHeight = 100.f;

		LaunchNuller(); 
		GetCharacterMovement()->StopMovementImmediately(); 
		FVector foo = GetActorForwardVector();
		FVector bee = FVector::VectorPlaneProject(foo, FVector(0, 0, 1));
		FVector bar = FVector(DashMulti * bee.X, DashMulti * bee.Y, JumpHeight);

		LaunchCharacter(bar, true, true);

		AbilityComponent->ApplyGameplayEffectToSelf(AegisEffect.GetDefaultObject(), 1.0f, AbilityComponent->MakeEffectContext()); 
	}
}

void ATOPDOWN_Take1Character::Ability_Aegis_KillGizmo(AActor* Gizmo)
{
	if (Gizmo)
	{
		Gizmo->Destroy();
	}
	else UE_LOG(LogTemp, Warning, TEXT("NO GIZMO FOUND"));
}

void ATOPDOWN_Take1Character::ZoomTo()
{
	if (abs(CameraBoom->TargetArmLength - ZoomSize) > ZoomRate + 1)
	{
		if ((CameraBoom->TargetArmLength - ZoomSize) < ((-ZoomRate) - 1))
		{
			CameraBoom->TargetArmLength = CameraBoom->TargetArmLength + ZoomRate;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATOPDOWN_Take1Character::ZoomTo, ZoomTick, false);
		}

		if ((CameraBoom->TargetArmLength - ZoomSize) > (ZoomRate + 1))
		{
			CameraBoom->TargetArmLength = CameraBoom->TargetArmLength - ZoomRate;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATOPDOWN_Take1Character::ZoomTo, ZoomTick, false);
		}
	}
}

void ATOPDOWN_Take1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
		if (CursorToWorld != nullptr)
		{
			if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
			{
				if (UWorld* World = GetWorld())
				{
					FHitResult HitResult;
					FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
					FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
					FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
					Params.AddIgnoredActor(this);
					World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
					FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
					CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
					//LightComponent->SetWorldLocation(HitResult.Location);
					
				}
			}
			else if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				FHitResult TraceHitResult;
				PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
				FVector CursorFV = TraceHitResult.ImpactNormal;
				FRotator CursorR = CursorFV.Rotation();
				CursorToWorld->SetWorldLocation(TraceHitResult.Location);
				CursorToWorld->SetWorldRotation(CursorR);

				if (IsAscending == true)  // PLUNGE LIVE ROTATION & TARGET DECAL 
				{
					if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
					{
						//OriginalBoomLength = CameraBoom->TargetArmLength;
						//while (CameraBoom->TargetArmLength < ZoomOutFull)
						//{
						//	CameraBoom->TargetArmLength = CameraBoom->TargetArmLength + 100;
						//}
						
						FVector ActorLocation = GetActorLocation();
						FVector CursorLocation = TraceHitResult.ImpactPoint;
						FRotator xak = (CursorLocation - ActorLocation).ToOrientationRotator();
						SetActorRotation(FRotator(0, xak.Yaw, 0));
						
						Target_Runes->SetWorldLocation((MyController->PlungeLocation) + FVector(0,0,-90));
						Target_Runes->SetHiddenInGame(false, true);
						Target_Runes->DecalSize = FVector(320.0f, 7500.0f, 7500.0f);
						/*Target_Runes->SetWorldRotation(CursorR);*/
						/*Target_Runes->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());*/
						Target_Runes->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
					}
				}

				//if (IsPlunging == true)
				//{
				//	if (CameraBoom->TargetArmLength > OriginalBoomLength)
				//	{
				//		while (CameraBoom->TargetArmLength > OriginalBoomLength)
				//		{
				//			CameraBoom->TargetArmLength = CameraBoom->TargetArmLength - 100; 
				//		}
				//	}
				//}

				if (IsWalkingToNPC == true) // WALKING UP TO AN NPC
				{
					if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
					{
						float Distance = FVector::Dist(GetActorLocation(), MyController->CurrentNPCLocation);
						//UE_LOG(LogTemp, Warning, TEXT("Distance remaining = %f"), Distance)
							if (Distance < (MyController->DialogueRange-10))
							{
								IsWalkingToNPC = false;
								GetCharacterMovement()->StopActiveMovement();
								if (MyController->CurrentNPC->HasLongDialogue)
								{
									MyController->StartBigDialogue(MyController->CurrentNPC);
								}
								else if (MyController->CurrentNPC->HasShortDialogue && !MyController->CurrentNPC->HasLongDialogue)
								{
									MyController->StartDialogue(MyController->CurrentNPC);
								}
							}
						
					}
				}
				if (isAutoAttacking)
				{
					FRotator MyRotation = GetActorRotation();
					FRotator NPCRotate = FRotator(0, (CurrentEnemyChar->GetActorLocation() - GetActorLocation()).ToOrientationRotator().Yaw, 0);

					float DeltaYawEnemy = MyRotation.Yaw - NPCRotate.Yaw;

					if (abs(DeltaYawEnemy) > 10.0f)
					{
						TargetRotation = NPCRotate;
						MustRotate = true;
						//UE_LOG(LogTemp, Warning, TEXT("TRYING TO ATTACK ROTATE, TARGET ROTATION: %s"), *TargetRotation.ToString());
					}
					else
					{
						MustRotate = false;
					}
					
					float Distance = FVector::Dist(GetActorLocation(), CurrentEnemyChar->GetActorLocation()); //walk up if too far
					if (Distance > AARange)
					{
						IsWalkingToEnemy = true;
						UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentEnemyChar);
					}
				}
				
				if (IsWalkingToEnemy == true)
					if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
					{

						if (isAutoAttacking == true)
						{
							float Distance = FVector::Dist(GetActorLocation(), CurrentEnemyChar->GetActorLocation());
							if (Distance < AARange)
							{
								UE_LOG(LogTemp, Warning, TEXT("Reached AARange, pausing movement, starting autoattack"))
								IsWalkingToEnemy = false;
								GetCharacterMovement()->StopActiveMovement();
								isPerformingAttack = true;
								MyController->AutoAttackStart(MyController->CurrentEnemy);
								
							}
							else
							{								
								IsWalkingToEnemy = true;
								/*UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(),CurrentEnemyChar);*/
							}

						}

					}

				if (MustRotate == true) //ROTATE TOWARDS PLAYER AT DIALOGUE START
				{
					//UE_LOG(LogTemp, Warning, TEXT("TRYING TO ROTATE, TARGET ROTATION: %s"), *TargetRotation.ToString());
					FRotator MyRotation = GetActorRotation();
					float DeltaYaw = MyRotation.Yaw - TargetRotation.Yaw;
					//UE_LOG(LogTemp, Warning, TEXT("MYROTATION: %s, TARGET ROTATION: %s, DELTAYAW: %f"), *MyRotation.ToString(), *TargetRotation.ToString(), DeltaYaw);
					float YawChange = 10.0f;
					if (DeltaYaw > 0) YawChange = -YawChange;

					SetActorRotation(FRotator(0, (MyRotation.Yaw + YawChange), 0));

					if (DeltaYaw < (YawChange+1) && DeltaYaw > -(YawChange+1))
					{
						SetActorRotation(FRotator(0, TargetRotation.Yaw, 0));
						MustRotate = false;
					}
				}

				if (!bCustomCamera)
				{

					if (!isCameraLocked && !isDialogueCamera)
					{
						if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetController()))
						{
							int32 ScreenX, ScreenY;
							float MouseX, MouseY;
							float DeltaX, DeltaY;
							MyController->GetViewportSize(ScreenX, ScreenY);
							MyController->GetMousePosition(MouseX, MouseY);

							if (MouseX >= 0 && MouseX < 0.07 * ScreenX)
							{
								DeltaX = -1.0f * CameraMoveDelta;
							}
							else if (MouseX >= 0.93 * ScreenX && MouseX <= ScreenX)
							{
								DeltaX = CameraMoveDelta;
							}
							else
							{
								DeltaX = 0;
							}

							if (MouseY >= 0 && MouseY < 0.07 * ScreenY)
							{
								DeltaY = CameraMoveDelta;
							}
							else if (MouseY >= 0.93 * ScreenY && MouseY <= ScreenY)
							{
								DeltaY = -1.0f * CameraMoveDelta;
							}
							else
							{
								DeltaY = 0;
							}

							UnlockedCameraComponent->SetWorldLocation(UnlockedCameraComponent->GetComponentLocation() + FVector(DeltaY, DeltaX, 0));

						}
					}

					if (isDialogueCamera)
					{
						if (DialogueCamera_CurrentTime < DialogueCamera_TargetTime)
						{
							FVector CameraPositionPlaceholder = UnlockedCameraComponent->GetComponentLocation();
							TopDownCameraComponent->SetActive(false);
							UnlockedCameraComponent->SetActive(true);
							UnlockedCameraComponent->SetWorldLocation(FMath::Lerp(CameraPositionPlaceholder, DialogueCamera_TargetVector, DialogueCamera_CurrentTime / DialogueCamera_TargetTime));
							DialogueCamera_CurrentTime = DialogueCamera_CurrentTime + 1.0f;
						}
					}
				}
				
				if (IsInDialogue == true)
				{
					GetCharacterMovement()->StopActiveMovement();
				}
			}
		}
}
