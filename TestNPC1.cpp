// Fill out your copyright notice in the Description page of Project Settings.


#include "TestNPC1.h"

#include "Combat/SpellGizmo.h"
#include "TOPDOWN_Take1PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FloaterWidget1.h"
#include "UI/HPFloater.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATestNPC1::ATestNPC1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bTurnTowardsPlayer = true;
	CharName = "Generic Helper";
	IDName = "GENERIC";
	isEnemy = false;
	TestValue = 2;
	MustRotate = false;
	isDying = false;
	isStunned = false; 
	NPCSize = 1;
	bHidePortrait = true;
	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//COMBAT STATS
	
	//DIALOGUE FLAGS

	bHasUrgentDialogue = false; //used for triggered conversations
	if (DialoguePicker)
	{
		HasShortDialogue = true;
	}
	big_i = 0;
	BigDialogueIndex = 0;

	//LOAD ANIMATIONS
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageMontageObject(TEXT("AnimMontage'/Game/Mannequin/Animations/TakeDamage1.TakeDamage1'"));
	if (DamageMontageObject.Succeeded())
	{
		DamageMontage = DamageMontageObject.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObject(TEXT("AnimMontage'/Game/Mannequin/Animations/Death1.Death1'"));
	if (DeathMontageObject.Succeeded())
	{
		DeathMontage = DeathMontageObject.Object;
	}*/

	//ABILITY HANDLING
	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName(TEXT("AbilityComponent")));
	AttributeComponent = CreateDefaultSubobject<UAttributesMain>(FName(TEXT("AttributeComponent")));
	
	//CREATE COMPONENTS
	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->AttachTo(GetRootComponent());
	
	if (GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating widget from ref"));
		FloaterWidgetBPRef = CreateWidget<UFloaterWidget1>(GetWorld()->GetFirstPlayerController(), FloaterWidgetBP);
	}

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	
	NameFloaterBlock2 = CreateDefaultSubobject<UWidgetComponent>(FName("NameFloaterBlock2"));
	NameFloaterBlock2->SetupAttachment(RootComponent);
	NameFloaterBlock2->SetWidgetSpace(EWidgetSpace::Screen);
	NameFloaterBlock2->SetHiddenInGame(true);

	RadialMenuBlock1 = CreateDefaultSubobject<UWidgetComponent>(FName("RadialMenuBlock1"));
	RadialMenuBlock1->SetupAttachment(RootComponent);
	RadialMenuBlock1->SetWidgetSpace(EWidgetSpace::Screen);
	RadialMenuBlock1->SetHiddenInGame(true);

	UE_LOG(LogTemp, Warning, TEXT("Setting widget class from ref"));
	
		HPFloater1 = CreateDefaultSubobject<UWidgetComponent>(FName("HPFloater1"));
		HPFloater1->SetupAttachment(RootComponent);
		HPFloater1->SetWidgetSpace(EWidgetSpace::Screen);
		HPFloater1->SetHiddenInGame(true);
	
	//GetComponents()->Name->NameFloaterBlock->SetText(FText::FromString(CharName));
	
	//NameFloaterBlock->SetText(FText::FromString(CharName));

	//CustomMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Component"));
	//CustomMeshComponent->AttachTo(RootComponent);
	//CustomMeshComponent->OnBeginCursorOver.AddDynamic(this, &ATestNPC1::CustomOnBeginMouseOver);
	
		OnTakeAnyDamage.AddDynamic(this, &ATestNPC1::TakeDamage);
	
}

float ATestNPC1::CastAbility(TSubclassOf<USpellClass> SpellToCast)
{
	CurrentSpellcast = NewObject<USpellClass>(this, SpellToCast);
	UE_LOG(LogTemp, Warning, TEXT("CastAbility - creating NewObject"));
	bIsCasting = true;
	CastDuration = CurrentSpellcast->CastTime; 
	return CurrentSpellcast->CastTime; 
}

void ATestNPC1::AbilityCastSuccessful(USpellClass* SpellCast, FTransform SpellSpawnLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("AbilityCastSuccessful..."));
	if(SpellCast->SpellGizmo)
	{
		AActor* Gizmo;
		SpellCast->ExecuteGameplayEffect(SpellSpawnLocation); 
		if (SpellCast->SpellRangeType == ESpellRangeTypes::SHOT || SpellCast->SpellRangeType == ESpellRangeTypes::SELF)
		{
			Gizmo = GetWorld()->SpawnActor<AActor>(SpellCast->SpellGizmo, GetActorTransform());

		}
		else
		{
			Gizmo = GetWorld()->SpawnActor<AActor>(SpellCast->SpellGizmo, SpellSpawnLocation);
		}

		SpellCast->SpellGizmoActual = Gizmo;

		if (ASpellGizmo* GizmoCast = Cast<ASpellGizmo>(Gizmo))
		{

			if (SpellCast->CastSFX)
			{
				AudioComp->SetSound(SpellCast->CastSFX);
				AudioComp->Play();
			}
			GizmoCast->BaseDamage = SpellCast->BaseDamage;
			GizmoCast->InstigatorSpell = SpellCast;

			FVector Direction = SpellSpawnLocation.GetLocation() - GetActorLocation();
			FRotator Rotation = FRotator(0, Direction.ToOrientationRotator().Yaw, 0);

			if (SpellCast->SpellRangeType == ESpellRangeTypes::SHOT)
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNING ZOMBIE SHOT"));
				if (!SpellCast->bMobile)
				{

				}
				GizmoCast->SetActorRotation(Rotation);
				GizmoCast->ProjectileMovement->Velocity = Rotation.Vector() * SpellCast->ProjectileVelocity;
			}

			
		}
	}
}

void ATestNPC1::AbilityCastSpellKillGizmo(AActor* Gizmo)
{
}

//DAMAGE PROCESSING

void ATestNPC1::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCause)
{
	AADamageAnimations();
	UE_LOG(LogTemp, Warning, TEXT("Animations fired, updating HP bar:"));
	GetWorldTimerManager().SetTimer(UnusedHandleT, this, &ATestNPC1::UpdateHPBar, 0.02f, false);
	UpdateHPBar();
}

void ATestNPC1::Stun(float Duration)
{
	isStunned = true;
	//GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->MaxWalkSpeed = 0.f; 
	FTimerHandle UnusedHandleStun; 
	GetWorldTimerManager().SetTimer(UnusedHandleStun, this, &ATestNPC1::UnStun, Duration, false);
}

void ATestNPC1::UnStun()
{
	UE_LOG(LogTemp, Warning, TEXT("Resetting MaxWalkSpeed to: %f"), BaseWalkSpeed);
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	isStunned = false;

}

// Called when the game starts or when spawned
void ATestNPC1::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(UnusedHandlePost, this, &ATestNPC1::PostBeginPlay, 0.5, false);

	UE_LOG(LogTemp, Warning, TEXT("Spawned NPC: %s"), *IDName);
	
	if (FloaterWidgetBP) NameFloaterBlock2->SetWidgetClass(FloaterWidgetBP);
	
	if (HPWidgetBP)
	{
		UE_LOG(LogTemp, Warning, TEXT("HFloater1: Setting class from ref"));
		HPFloater1->SetWidgetClass(HPWidgetBP);
	}
	
}

//MOUSEOVER
//void ATestNPC1::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
//{
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Mouse Over"));
//	}
//}


void ATestNPC1::AADamageAnimations()
{
	if (HealthComponent->HP >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to play animations"));
		int Random1 = FMath::RandRange(1, 4);
		FString MontageSection = "Damage" + FString::FromInt(Random1);

		UE_LOG(LogTemp, Warning, TEXT("Playing section: %s"), *MontageSection);
		PlayAnimMontage(DamageMontage, 1.5f, FName(*MontageSection));
		UpdateHPBar();
	}
}

void ATestNPC1::Say(FText InText, float Duration, USoundBase* SFX)
{
	if (UFloaterWidget1* MyFloaterWidget = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
	{
		NameFloaterBlock2->SetHiddenInGame(false);
		MyFloaterWidget->FloaterTextBlock->SetText(InText);
		if (SFX)
		{
			AudioComp->SetSound(SFX);
			AudioComp->Play();
		}
		if (Duration > 0)
		{
			GetWorldTimerManager().SetTimer(UnusedHandle22, this, &ATestNPC1::HideFloater_Simple, Duration, false);
		}
		else
		{
			GetWorldTimerManager().SetTimer(UnusedHandle22, this, &ATestNPC1::HideFloater_Simple, 2.5f, false);
		}
	}
}

void ATestNPC1::HideFloater_Simple()
{
	NameFloaterBlock2->SetHiddenInGame(true); 
}

void ATestNPC1::UpdateHPBar()
{
	UE_LOG(LogTemp, Warning, TEXT("HPBar update!"));
	
	//if (HealthComponent->HP < HealthComponent->HP_MAX && !isDying)
	if (HealthComponent->HP < HealthComponent->HP_MAX && !isDying)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to show HP Bar"));
		HPFloater1->SetHiddenInGame(false);
		if (UHPFloater* HPFloaterBlock = Cast<UHPFloater>(HPFloater1->GetUserWidgetObject()))
		{
			float HPPercentage = HealthComponent->HP / HealthComponent->HP_MAX;
			HPFloaterBlock->HPBar->SetPercent(HPPercentage);
		}

		if (HealthComponent->HP == 0) //death animation
		{
			HPFloater1->SetHiddenInGame(true);
			UE_LOG(LogTemp, Warning, TEXT("Trying to play death animations"));
			int Random1 = FMath::RandRange(1, 3);
			FString MontageSection = "Dying" + FString::FromInt(Random1);

			UE_LOG(LogTemp, Warning, TEXT("Playing section: %s"), *MontageSection);
			PlayAnimMontage(DeathMontage, 1.f, FName(*MontageSection));
			isDying = true;

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DetachFromControllerPendingDestroy();
			SetActorEnableCollision(false);

			
			UWorld* World = GetWorld();
			if (World)
			{
				if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
				{
					MyController->AutoAttackCancel();
				}
			}
		}
	}
}


// Called every frame
void ATestNPC1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsCasting)
	{
		if (CastTimer < CastDuration)
		{
				CastTimer += DeltaTime;
		}
		else
		{
				AbilityCastSuccessful(CurrentSpellcast, CurrentTargetLocation);
				CastDuration = 0;
				CastTimer = 0;
				bIsCasting = false; 
		}

	}
	

	if(isStunned)
	{
	}
	
	if (MustRotate==true) //ROTATE TOWARDS PLAYER AT DIALOGUE START
	{
			//UE_LOG(LogTemp, Warning, TEXT("TRYING TO ROTATE, TARGET ROTATION: %s"), *TargetRotation.ToString());
			FRotator MyRotation = GetActorRotation();
			float DeltaYaw = MyRotation.Yaw - TargetRotation.Yaw;
			//UE_LOG(LogTemp, Warning, TEXT("MYROTATION: %s, TARGET ROTATION: %s, DELTAYAW: %f"), *MyRotation.ToString(), *TargetRotation.ToString(), DeltaYaw);
			float YawChange = 4.9f;
			if (DeltaYaw > 0 && DeltaYaw < 180) YawChange = -YawChange;

			SetActorRotation(FRotator(0, (MyRotation.Yaw + YawChange), 0));

			if (DeltaYaw < 5 && DeltaYaw > -5) MustRotate = false;
	}

	//check HP


}

void ATestNPC1::PostBeginPlay()
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trying to add NPC to the list: %s"), *IDName);
		if (MyController->NPCManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding NPC to the list: %s"), *IDName);
			MyController->NPCManager->Add(this);
		}
	}
}

// Called to bind functionality to input
//void ATestNPC1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

