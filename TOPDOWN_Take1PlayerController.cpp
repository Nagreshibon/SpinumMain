// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TOPDOWN_Take1PlayerController.h"

#include <Engine/StaticMeshActor.h>


#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/WidgetComponent.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TOPDOWN_Take1Character.h"
#include "Actors/InteractableActor.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestNPC1.h"
#include "GameFramework/SpringArmComponent.h"
#include "TestNPC1_Friendly.h"
#include "UI/FloaterWidget1.h"
#include "DialogueData/BigTalkWidget.h"
#include "DialogueData/BigTalkWidget_Chapter.h"
#include "DialogueData/DialoguePicker.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueData/JournalWidget.h"
#include "Combat/SpellGizmo.h"
#include "UI/IdeaWidget.h"
#include "UI/InventoryWidget.h"
#include "GameFramework/ProjectileMovementComponent.h"
//#include "UObject/ConstructorHelpers.h"

ATOPDOWN_Take1PlayerController::ATOPDOWN_Take1PlayerController()
{
	//Anatomy = CreateDefaultSubobject<UCorpus>(TEXT("Anatomy"));

	//ability handling
	SerialAbilityCounter = 0; 
	
	
	//plunge
	CanPlunge = true;
	PlungeRise = 1.5f;
	RiseHeight = 1500.f;
	PlungeCooldown = 2.0f;
	PlungeMin = 1400.0f;
	PlungeMax = 2300.0f;
	PlungeDamage = 70.0f;
	PlungeDamageRadius = 600.0f;
	AAMoveDelay = 0.4f;
	AAIndex = 1;
	DialogueRange = 350;
	
	//dash
	DashMulti = 1500;
	JumpHeight = 160.0f;
	CanDash = true;
	DashStop = 0.1f;
	DashCooldown = 2.0f;

	//interface
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	DeltaZoom = 50.0f;

	//dialogue handling
	//bIsTalking = false;
	//bIsInTalkRange = false;
	//AssociatedPawn = nullptr;
	
	//HUD test
	//TSharedRef<SVerticalBox> widget = SNew(SVerticalBox) + SVerticalBox::Slot()
	//	.HAlign(HAlign_Center)
	//	.VAlign(VAlign_Center)
	//	[
	//		SNew(SButton)
	//		.Content()
	//	[
	//		SNew(STextBlock)
	//		.Text(FText::FromString(TEXT("Test button")))
	//	]
	//	];
	//GEngine->GameViewport->AddViewportWidgetForPlayer(GetLocalPlayer(), widget, 1);

	//static ConstructorHelpers::FObjectFinder<UUserWidget> BigTalkBPObject(TEXT("WidgetBlueprint'/Game/TopDownCPP/UI/BP_BigTalkWidget1.BP_BigTalkWidget1'"));
	//if (BigTalkBPObject.Succeeded())
	//{
	//	BigTalkWidget_BPRef = BigTalkBPObject.Object;
	//}
	
}

//INPUT

void ATOPDOWN_Take1PlayerController::SidekickDialogue(UDataTable* NewDialoguePicker, int NewIndex)
{


	Sidekick->SetDialogue(NewIndex, NewDialoguePicker);
	Sidekick->TestDialogue(); 
	//StartDialogue(Sidekick);
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawn->IsWalkingToNPC = false; 
	}
	
}

void ATOPDOWN_Take1PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("CameraUnlockToggle", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::CameraUnlockToggle);
	
	InputComponent->BindAction("AutoAttack", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::AutoAttack);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::InteractBranch);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::DoubleJump);
	InputComponent->BindAction("Plunge", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::PlungeBranch);

	InputComponent->BindAction("Ability Q", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::AbilityQ);
	InputComponent->BindAction("Ability W", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::AbilityW);
	InputComponent->BindAction("Ability F", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::AbilityF);
	InputComponent->BindAction("Ability T", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::AbilityT);
	InputComponent->BindAction("Ability Control Q", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::AbilityControlQ);

	InputComponent->BindAction("Zoom In", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::ZoomIn);
	InputComponent->BindAction("Zoom Out", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::ZoomOut);

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATOPDOWN_Take1PlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("ShowJournal", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::ShowJournalWidget);
	InputComponent->BindAction("ShowJournal", IE_Released, this, &ATOPDOWN_Take1PlayerController::HideJournalWidget);

	InputComponent->BindAction("ShowIdeas", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::ToggleIdeaWidget);

	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::ToggleInventoryWidget);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATOPDOWN_Take1PlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATOPDOWN_Take1PlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ATOPDOWN_Take1PlayerController::OnResetVR);

}

// WIDGETS

void ATOPDOWN_Take1PlayerController::ToggleInventoryWidget()
{
	if (InventoryWidget)
	{


		if (!(InventoryWidget->IsInViewport()))
		{
			InventoryWidget->AddToViewport();
			if (UInventoryWidget* MyInventoryWidget = Cast<UInventoryWidget>(InventoryWidget))
			{
				//MyInventoryWidget->ReinitializeList(GetPawn());
			}
		}


		else InventoryWidget->RemoveFromViewport();

	}
}

void ATOPDOWN_Take1PlayerController::ShowIdeaWidget()
{
	if (IdeaWidget)
	{
		IdeaWidget->AddToViewport();

		if (UIdeaWidget* MyIdeaWidget = Cast<UIdeaWidget>(IdeaWidget))
		{

			//MyIdeaWidget->ReinitializeList(GetPawn());
		}
	}
}

void ATOPDOWN_Take1PlayerController::ToggleIdeaWidget()
{
	if (IdeaWidget)
	{
		

		if (!(IdeaWidget->IsInViewport()))
		{
			IdeaWidget->AddToViewport();
			if (UIdeaWidget* MyIdeaWidget = Cast<UIdeaWidget>(IdeaWidget))
			{
				MyIdeaWidget->ReinitializeList(GetPawn());
			}
		}

		
		else IdeaWidget->RemoveFromViewport();

	}
}

void ATOPDOWN_Take1PlayerController::HideIdeaWidget()
{
	if(IdeaWidget) IdeaWidget->RemoveFromViewport();
}

void ATOPDOWN_Take1PlayerController::ShowJournalWidget()
{
	JournalWidget->AddToViewport();
	if (UJournalWidget* MyJournal = Cast<UJournalWidget>(JournalWidget))
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Populating Quest Journal from QuestLog"));
			MyJournal->PopulateQuestLog(MyPawn->QuestLog);
		}
	}
}

void ATOPDOWN_Take1PlayerController::HideJournalWidget()
{
	JournalWidget->RemoveFromViewport();
}


//BEGIN PLAY
void ATOPDOWN_Take1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	
	NPCManager = GetWorld()->SpawnActor<ANPCManager>(Location, Rotation, SpawnInfo);

	CooldownManager = NewObject<UCooldownManager>(this, UCooldownManager::StaticClass());
	
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawn->CameraLock();
	}

	if (BigTalkWidget_BP)
	{
		UE_LOG(LogTemp, Warning, TEXT("MAKING BIGTALK WIDGET FROM BLUEPRINT"));
		BigTalkWidget1 = CreateWidget<UBigTalkWidget>(this, BigTalkWidget_BP, TEXT("BigTalkWidget1"));
		BigTalkWidget1->RemoveFromViewport();
		/*BigTalkWidget1->SetVisibility(ESlateVisibility::Collapsed);*/
		/*UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);*/
	}

	if (BigTalkWidget_Left_BP)
	{
		UE_LOG(LogTemp, Warning, TEXT("MAKING BIGTALK WIDGET LEFT FROM BLUEPRINT"));
		BigTalkWidget1_Left = CreateWidget<UBigTalkWidget>(this, BigTalkWidget_Left_BP, TEXT("BigTalkWidget1_Left"));
		BigTalkWidget1_Left->RemoveFromViewport();
	}

	if (BigTalkWidget_Chapter_BP)
	{
		UE_LOG(LogTemp, Warning, TEXT("MAKING CHAPTER BIGTALK WIDGET FROM BLUEPRINT"));
		BigTalkWidget2 = CreateWidget<UBigTalkWidget_Chapter>(this, BigTalkWidget_Chapter_BP, TEXT("BigTalkWidget_Chapter"));
		BigTalkWidget2->RemoveFromViewport();
	}

	BigTalkWidgetCurrent = BigTalkWidget1;


	if(JournalWidget_BP)
	{
			JournalWidget = CreateWidget<UJournalWidget>(this, JournalWidget_BP, TEXT("JournalWidget"));
	}

	if (IdeaWidget_BP)
	{
		IdeaWidget = CreateWidget<UIdeaWidget>(this, IdeaWidget_BP, TEXT("IdeaWidget"));
		IdeaWidget->RemoveFromViewport(); 
	}

	if (IdeaWidget_BP)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidget_BP, TEXT("InventoryWidget"));
		InventoryWidget->RemoveFromViewport();
	}


	
}

//INTERACT

void ATOPDOWN_Take1PlayerController::InteractBranch()
{
	if (!LockInteraction)
	{
		FHitResult ActorHit;
		GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ActorHit);
		UE_LOG(LogTemp, Warning, TEXT("ACTOR HIT: %s"), *(ActorHit.GetActor())->GetName());

		if (ATestNPC1* MyNPC = Cast<ATestNPC1>(ActorHit.GetActor()))
		{
			if (MyNPC->isEnemy == true)
			{
				//IS AN ENEMY
				UE_LOG(LogTemp, Warning, TEXT("ACTOR HIT IS AN ENEMY"));
				
			}
			else if (MyNPC->isEnemy == false) // && MyNPC->GetClass() == ATestNPC1_Friendly::StaticClass()
			{
				CurrentNPC = MyNPC;
				//IS NOT AN ENEMY
				if (MyNPC->HasLongDialogue)
				{
					StartBigDialogue(MyNPC);
				}
				else if (MyNPC->HasShortDialogue && !MyNPC->HasLongDialogue)
				{
					UE_LOG(LogTemp, Warning, TEXT("ACTOR HIT IS NOT AN ENEMY, isEnemy = %d"), MyNPC->isEnemy)
						StartDialogue(MyNPC);
				}

			}
		}
		else if (AItemPickup* ItemToPickup = Cast<AItemPickup>(ActorHit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("PICKUP HIT: %s"), *(ItemToPickup->Name.ToString()));

			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
			{
				MyPawn->GetCharacterMovement()->StopMovementImmediately();
				FVector MyLocation = MyPawn->GetActorLocation();
				FVector ItemLocation = ItemToPickup->GetActorLocation();
				float Distance = FVector::Dist(ItemLocation, MyLocation);

				if (Distance < MyPawn->AARange)
				{
					AItemPickup* NewItem = NewObject<AItemPickup>(this, ItemToPickup->GetClass());
					MyPawn->Inventory.Add(NewItem);
					
					ItemToPickup->Destroy();
				}
				else
					UAIBlueprintHelperLibrary::SimpleMoveToActor(this, ItemToPickup);
			}
			
		}
		else if (AInteractableActor* InteractableActor = Cast<AInteractableActor>(ActorHit.GetActor()))
		{
			if(InteractableActor->PreExecuteInteractEffect())
			{
				InteractableActor->ExecuteInteractEffect();
			}
		}
	}
}

//COMBAT
	//autoattack
void ATOPDOWN_Take1PlayerController::AutoAttack()
{
	if (!LockInteraction)
	{
		FHitResult ActorHit;
		GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ActorHit);
		//UE_LOG(LogTemp, Warning, TEXT("ACTOR HIT: %s"), *(ActorHit.GetActor()->GetName()));

		if (ATestNPC1* MyEnemy = Cast<ATestNPC1>(ActorHit.GetActor()))
		{
			if (MyEnemy->isEnemy == true)
			{
				CurrentEnemy = MyEnemy;
				UE_LOG(LogTemp, Warning, TEXT("Trying to attack"));
				AutoAttackActive = true;
				if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
				{
					MyPawn->isAutoAttacking = true;
					MyPawn->CurrentEnemyChar = MyEnemy;
					UE_LOG(LogTemp, Warning, TEXT("Passing attack target"));
					GetWorldTimerManager().SetTimer(UnusedHandle3, this, &ATOPDOWN_Take1PlayerController::AutoAttackRelease, AAMoveDelay, false);
					float Distance = FVector::Dist(MyEnemy->GetActorLocation(), MyPawn->GetActorLocation());
					if (Distance < MyPawn->AARange)
					{
						MyPawn->isPerformingAttack = true;
						AutoAttackStart(CurrentEnemy);


					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Running up to the enemy"));
						MyPawn->IsWalkingToEnemy = true;
						UAIBlueprintHelperLibrary::SimpleMoveToActor(this, MyEnemy);
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not an enemy"));
			}
		}
	}

}

void ATOPDOWN_Take1PlayerController::AutoAttackCancel()
{
	if (ATOPDOWN_Take1Character* MyPawnCast = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawnCast->StopAnimMontage(MyPawnCast->AttackMontage1);
		MyPawnCast->isPerformingAttack = false;
		MyPawnCast->isAutoAttacking = false;
	}
}

void ATOPDOWN_Take1PlayerController::AutoAttackStart(ATestNPC1* MyEnemy)
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		if (MyPawn->isAutoAttacking && MyPawn->isPerformingAttack)
		{
			UE_LOG(LogTemp, Warning, TEXT("Starting AutoAttack"));
			if (AAIndex > 4) AAIndex = 1;

			//section string reference
			FString MontageSection = "Attack" + FString::FromInt(AAIndex);

			float AttackDuration = 1.0f / MyPawn->AttackSpeed;
			float AttackPlayRate = 1.0f * 0.9f * MyPawn->AttackSpeed;
			MyPawn->PlayAnimMontage(MyPawn->AttackMontage1, AttackPlayRate, FName(*MontageSection));
			FTimerDelegate AttackAgain = FTimerDelegate::CreateUObject(this, &ATOPDOWN_Take1PlayerController::AutoAttackStart, MyEnemy);
			GetWorldTimerManager().SetTimer(UnusedHandle4, AttackAgain, AttackDuration, false);
			AAIndex++;
		}
	}
}

void ATOPDOWN_Take1PlayerController::AutoAttackRelease()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawn->IsLanding = false;
		AutoAttackActive = false;
	}
}

void ATOPDOWN_Take1PlayerController::AbilityW()
{
	if (!LockInteraction)
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
		{
			UE_LOG(LogTemp, Warning, TEXT("FIRING ABILITY W"));
			FHitResult SpellTarget;
			GetHitResultUnderCursor(ECC_GameTraceChannel1, false, SpellTarget);

			MyPawn->Ability_Aegis(SpellTarget.ImpactPoint);
		}
	}
}

void ATOPDOWN_Take1PlayerController::AbilityQ()
{

	if (Spell3)
	{
		if (CooldownManager->IsCastable(Spell3))
		{
			AbilityCastSpell(Spell3);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("SPELL3 CAST FAILED"));
}

void ATOPDOWN_Take1PlayerController::AbilityControlQ()
{
	if (Spell4)
	{
		if (CooldownManager->IsCastable(Spell4))
		{
			AbilityCastSpell(Spell4);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("SPELL4 CAST FAILED"));
}

void ATOPDOWN_Take1PlayerController::AbilityT()
{

		if (Spell1)
		{
			if (CooldownManager->IsCastable(Spell1))
			{
				AbilityCastSpell(Spell1);
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("SPELL1 CAST FAILED"));
}

void ATOPDOWN_Take1PlayerController::AbilityF()
{
		if (Spell2)
		{
			if (CooldownManager->IsCastable(Spell2))
			{
				AbilityCastSpell(Spell2);
			}
		}
}

void ATOPDOWN_Take1PlayerController::AbilityCastSpell(TSubclassOf<USpellClass> Spell)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to cast a spell..."));
	if (CastBar)
		CastBar->Interrupt();

	bMoveToMouseCursor = false; 
	
	if(USpellClass* SpellCast = NewObject<USpellClass>(this, Spell))
	//if (USpellClass* SpellCast = CreateDefaultSubobject<UTestSpell1>(this))
	{
		if (SpellCast->bAlwaysCastable || !LockInteraction)
		{
			if(SpellCast->AnimationMontage)
			{
				if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
				{
					if(SpellCast->MaxDuration > 0.f)
					{
						MyPawn->PlayAnimationWithDuration(SpellCast->AnimationMontage, SpellCast->AnimationRate, SpellCast->MaxDuration); 
					}
					else
					{
						MyPawn->PlayAnimation(SpellCast->AnimationMontage, SpellCast->AnimationRate);
					}
				}
			}
			
			FTransform SpellTargetLocation;
			FHitResult SpellTarget;
			switch (SpellCast->SpellRangeType)
			{
			case ESpellRangeTypes::POINT:

				GetHitResultUnderCursor(ECC_Camera, false, SpellTarget);
				SpellTargetLocation.SetLocation(SpellTarget.ImpactPoint);
				break;

			case ESpellRangeTypes::SELF:
				SpellTargetLocation = GetCharacter()->GetActorTransform();
				break;

			case ESpellRangeTypes::SHOT:
				GetHitResultUnderCursor(ECC_Camera, false, SpellTarget);
				SpellTargetLocation.SetLocation(SpellTarget.ImpactPoint);
				break;

			case ESpellRangeTypes::HOMING
					:
						SpellTargetLocation = GetCharacter()->GetActorTransform();
						break;
						default
							:
								SpellTargetLocation = GetCharacter()->GetActorTransform();
								break;

			}

			FVector Direction = SpellTargetLocation.GetLocation() - GetPawn()->GetActorLocation();
			FRotator Rotation = FRotator(0, Direction.ToOrientationRotator().Yaw, 0);
			//Rotation.Pitch = 0;
			if (!SpellCast->bMobile)
			{
				GetPawn()->GetMovementComponent()->StopMovementImmediately();
				if (SpellCast->bTurnTowardsCursor && SpellCast->CastTime > 0.f)
				{
					if (SpellCast->SpellRangeType != ESpellRangeTypes::SELF)
					{
						if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
						{

							MyPawn->MustRotate = true;
							MyPawn->TargetRotation = Rotation;
							MyPawn->bIsCasting = true;
						}
					}
					else
					{
						FHitResult TurnTarget;
						GetHitResultUnderCursor(ECC_Camera, false, TurnTarget);
						FTransform TurnTargetLocation;
						TurnTargetLocation.SetLocation(TurnTarget.ImpactPoint);

						FVector TurnDirection = TurnTargetLocation.GetLocation() - GetPawn()->GetActorLocation();
						FRotator TurnRotation = FRotator(0, TurnDirection.ToOrientationRotator().Yaw, 0);
						if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
						{
							MyPawn->MustRotate = true;
							MyPawn->TargetRotation = TurnRotation;
							MyPawn->bIsCasting = true;
						}
					}
				}
			}

			if (SpellCast->PreCastEffect(SpellTargetLocation))
			{
				UE_LOG(LogTemp, Warning, TEXT("Cast Successful! Cast time = %f"), SpellCast->CastTime);
				if (SpellCast->CastTime > 0)
				{

					UE_LOG(LogTemp, Warning, TEXT("Cast time = %f, showing the cast bar"), SpellCast->CastTime);
					CastBar = CreateWidget<UCastBar>(this, CastBar_BP, TEXT("CastBar"));
					//CastBar->MyController = this;
					CastBar->SpellCast = SpellCast;
					CastBar->SpellClass = Spell; 
					CastBar->SpawnLocation = SpellTargetLocation;
					CastBar->AddToViewport();
					CastBar->ShowBar();
					CastBar->CastDuration = SpellCast->CastTime;
					if (SpellCast->CastGizmo)
					{
						FTransform TargetTransform = GetPawn()->GetActorTransform();
						TargetTransform.SetRotation(Rotation.Quaternion());
						SpellCast->CastGizmoActual = CastBar->CreateCastGizmo(SpellCast->CastGizmo, TargetTransform);
					}
				}
				else
				{
					AbilityCastSpellSuccessful(SpellCast, Spell, SpellTargetLocation);
				}
			}
			else CooldownManager->Add(Spell, SpellCast->GetCooldown()); 
		}
	}
}

void ATOPDOWN_Take1PlayerController::AbilityCastSpellSuccessful(USpellClass* SpellCast, TSubclassOf<USpellClass> Spell, FTransform SpellSpawnLocation)
{
	if(SpellCast->SpellGizmo)
	{
		if(SpellCast->CastTime == 0.f)
		{
			FHitResult TurnTarget;
			GetHitResultUnderCursor(ECC_Camera, false, TurnTarget);
			FTransform TurnTargetLocation;
			TurnTargetLocation.SetLocation(TurnTarget.ImpactPoint);

			FVector TurnDirection = TurnTargetLocation.GetLocation() - GetPawn()->GetActorLocation();
			FRotator TurnRotation = FRotator(0, TurnDirection.ToOrientationRotator().Yaw, 0);

			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
			{
				MyPawn->SetActorRotation(TurnRotation);
			}
		}
		
		if (SpellCast->PreExecuteGameplayEffect(SpellSpawnLocation))
		{
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
			{
				MyPawn->bIsCasting = false;
			}

			AActor* Gizmo;
			SpellCast->ExecuteGameplayEffect(SpellSpawnLocation);
			if (!SpellCast->MultiStage)
			{
				CooldownManager->Add(Spell, SpellCast->GetCooldown());
			}

			if (SpellCast->SpellRangeType == ESpellRangeTypes::SHOT || SpellCast->SpellRangeType == ESpellRangeTypes::SELF)
			{
				Gizmo = GetWorld()->SpawnActor<AActor>(SpellCast->SpellGizmo, GetPawn()->GetActorTransform());

			}
			else
			{
				Gizmo = GetWorld()->SpawnActor<AActor>(SpellCast->SpellGizmo, SpellSpawnLocation);
			}

			if (ASpellGizmo* GizmoCast = Cast<ASpellGizmo>(Gizmo))
			{
				GizmoCast->BeginKill(SpellCast->SpellGizmoDuration);
			}
			

			SpellCast->SpellGizmoActual = Gizmo; 
			if (ASpellGizmo* GizmoCast = Cast<ASpellGizmo>(Gizmo))
			{

				if (SpellCast->CastSFX)
				{
					if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
					{
						MyPawn->AudioComp->SetSound(SpellCast->CastSFX);
						MyPawn->AudioComp->Play();
					}
				}
				
				GizmoCast->BaseDamage = SpellCast->BaseDamage;
				GizmoCast->InstigatorSpell = SpellCast; 

				FVector Direction = SpellSpawnLocation.GetLocation() - GetPawn()->GetActorLocation();
				//FRotator xak = Direction.ToOrientationRotator();
				
				//FVector PlaneProject = FVector::VectorPlaneProject(Direction, FVector(0, 0, 1));
				FRotator Rotation = FRotator(0,Direction.ToOrientationRotator().Yaw,0); 

				if (SpellCast->SpellRangeType == ESpellRangeTypes::SHOT)
				{
					if (!SpellCast->bMobile)
					{

					}
					GizmoCast->SetActorRotation(Rotation);
					GizmoCast->ProjectileMovement->Velocity = Rotation.Vector() * SpellCast->ProjectileVelocity;
					//GizmoCast->ProjectileMovement->SetVelocityInLocalSpace(Rotation.Vector() * SpellCast->ProjectileVelocity);
					//GizmoCast->ProjectileMovement->InitialSpeed = SpellCast->ProjectileVelocity;
				}
				else
				{
					//Gizmo = GetWorld()->SpawnActor<AActor>(SpellCast->SpellGizmo, SpellSpawnLocation);
				}

				//GizmoCast->StunDuration = 
			}

			//UE_LOG(LogTemp, Warning, TEXT("Trying to KillGizmo after %f s"), SpellCast->SpellDuration);
			//FTimerDelegate KillGizmo = FTimerDelegate::CreateUObject(this, &ATOPDOWN_Take1PlayerController::AbilityCastSpellKillGizmo, Gizmo);
			//GetWorldTimerManager().SetTimer(UnusedHandle, KillGizmo, SpellCast->SpellDuration, false);
		}
		else CooldownManager->Add(Spell, SpellCast->GetCooldown());
	}
	
}

void ATOPDOWN_Take1PlayerController::AbilityCastSpellKillGizmo(AActor* Gizmo)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to KillGizmo"));
	if (Gizmo)
	{
		Gizmo->Destroy();
	}
}

//DIALOGUE

void ATOPDOWN_Take1PlayerController::StartDialogue(ATestNPC1* MyNPC)
{
	AutoAttackCancel();
	UE_LOG(LogTemp, Warning, TEXT("STARTING DIALOGUE"));
	//UE_LOG(LogTemp, Warning, TEXT("ACTOR MESSAGE: %s"), *MyNPC->Welcome);
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawn->GetCharacterMovement()->StopMovementImmediately();
		FVector MyLocation = MyPawn->GetActorLocation();
		FVector NPCLocation = MyNPC->GetActorLocation();
		float Distance = FVector::Dist(NPCLocation, MyLocation);
		FRotator NPCRotate = FRotator(0,(MyLocation - NPCLocation).ToOrientationRotator().Yaw,0);
		CurrentNPCRotation = NPCRotate;
		MyNPC->TargetRotation = NPCRotate;
		if (MyNPC->IsRootComponentMovable() && MyNPC->bTurnTowardsPlayer)
		{
			MyNPC->MustRotate = true;
		}

		if (Distance < DialogueRange && MyPawn->IsWalkingToNPC == false) //START DIALOGUE IF CLOSE ENOUGH
		{
			//MyNPC->SetActorRotation(NPCRotate);
			if (MyNPC->IsRootComponentMovable() && MyNPC->bTurnTowardsPlayer)
			{
				MyPawn->MustRotate = true;
				MyPawn->TargetRotation = FRotator(0, (NPCLocation - MyLocation).ToOrientationRotator().Yaw, 0);
			}
				
			if (abs(MyLocation.X-NPCLocation.X) < 110 && abs(MyLocation.Y-NPCLocation.Y) > 15)
			{
				UE_LOG(LogTemp, Warning, TEXT("Firing far zoom"));
				MyPawn->ZoomSize = Distance + 0.5 * (1200 - Distance);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Firing close zoom"));
				MyPawn->ZoomSize = Distance + 0.95 * (1100 - Distance);
			}
			
			MyPawn->ZoomRate = 15;
			MyPawn->ZoomTo();
			//START TALKING
			/*ToggleUI();*/
			InitShortDialogue(MyNPC);
			if (MyNPC->HasShortDialogue && !MyNPC->HasLongDialogue)
			{
				if (ATestNPC1_Friendly* MyNPCCast = Cast<ATestNPC1_Friendly>(MyNPC))
				{
					if (MyNPCCast->bIsInShortDialogue == false)
					{
						UE_LOG(LogTemp, Warning, TEXT("Cast to ATestNPC1_Friendly"));
						if (UWidgetComponent* MyWidget = MyNPCCast->FindComponentByClass<UWidgetComponent>())
						{
							UE_LOG(LogTemp, Warning, TEXT("Cast to WidgetComponent"));
							if (UFloaterWidget1* MyFloater = Cast<UFloaterWidget1>(MyWidget->GetUserWidgetObject()))
							{
								MyFloater->FloaterTextBlock->SetText(FText::FromString("Starting dialogue..."));
								MyNPCCast->TestDialogue();
							}
							//MyWidget->SetHiddenInGame(false);
						}
					}
				}
			}
		}

		else
		{
			//CurrentNPCRotation = NPCRotate;
			CurrentNPCLocation = NPCLocation;
			MyPawn->IsWalkingToNPC = true;
			UAIBlueprintHelperLibrary::SimpleMoveToActor(this, MyNPC);
		}
		
	}	
}

void ATOPDOWN_Take1PlayerController::StartBigDialogue(ATestNPC1* MyNPC)
{
	AutoAttackCancel();
	MyNPC->GetCharacterMovement()->StopActiveMovement();
	UE_LOG(LogTemp, Warning, TEXT("STARTING BIG DIALOGUE"));
	//UE_LOG(LogTemp, Warning, TEXT("ACTOR MESSAGE: %s"), *MyNPC->Welcome);
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawn->GetCharacterMovement()->StopMovementImmediately();
		FVector MyLocation = MyPawn->GetActorLocation();
		FVector NPCLocation = MyNPC->GetActorLocation();
		float Distance = FVector::Dist(NPCLocation, MyLocation);
		FRotator NPCRotate = FRotator(0, (MyLocation - NPCLocation).ToOrientationRotator().Yaw, 0);
		CurrentNPCRotation = NPCRotate;
		MyNPC->TargetRotation = NPCRotate;
		if (MyNPC->IsRootComponentMovable() && MyNPC->bTurnTowardsPlayer)
		{
			MyNPC->MustRotate = true;
		}
			
		if (Distance < (DialogueRange+MyNPC->NPCSize) && MyPawn->IsWalkingToNPC == false) //START DIALOGUE IF CLOSE ENOUGH
		{
			//MyNPC->SetActorRotation(NPCRotate);
			if (MyNPC->IsRootComponentMovable() && MyNPC->bTurnTowardsPlayer)
			{
				MyPawn->MustRotate = true;
				MyPawn->TargetRotation = FRotator(0, (NPCLocation - MyLocation).ToOrientationRotator().Yaw, 0);
			}

			UE_LOG(LogTemp, Warning, TEXT("Firing close zoom"));
			MyPawn->ZoomSize = Distance + 0.95 * (1100 - Distance);
			MyPawn->ZoomRate = 15;
			MyPawn->ZoomTo();
			
			//START TALKING
			/*ToggleUI();*/
			InitShortDialogue(MyNPC);
			if (ATestNPC1_Friendly* MyNPCCast = Cast<ATestNPC1_Friendly>(MyNPC))
			{
				if (MyNPCCast->bIsInShortDialogue == false)
				{
					//LEFT OR RIGHT
					if (MyNPC->BigDialoguePicker)
					{
						FString DialogueContextString = FString::FromInt(MyNPC->BigDialogueIndex);
						if (FDialoguePicker* DialoguePickerRow = MyNPC->BigDialoguePicker->FindRow<FDialoguePicker>(FName(*DialogueContextString), TEXT("")))
						{
							if ((*DialoguePickerRow).bLeftSide && BigTalkWidget1_Left)
							{
								BigTalkWidgetCurrent = BigTalkWidget1_Left;
							}
							else
							{
								BigTalkWidgetCurrent = BigTalkWidget1; 
							}
						}
					}
					
					
					
					if (BigTalkWidgetCurrent)
					{
						
						UE_LOG(LogTemp, Warning, TEXT("TRYING TO ADD TO VIEWPORT"));
						BigTalkWidgetCurrent->AddToViewport();
						BigTalkWidgetCurrent->SetVisibility(ESlateVisibility::Visible);
						
						if (UBigTalkWidget* MyTalkWidget = Cast<UBigTalkWidget>(BigTalkWidgetCurrent))
						{
							if (MyNPC->BigDialoguePicker)
							{
								UE_LOG(LogTemp, Warning, TEXT("Cast succesful"));
								MyTalkWidget->PortraitImage->SetBrushFromTexture(MyNPC->PortraitImage);
								MyTalkWidget->StartBigTalk(MyNPC);
							}
						}
					}

					//INIT TALK
					//		TestBigDialogue(MyNPC);
				}
			}
		}

		else
		{
			//CurrentNPCRotation = NPCRotate;
			CurrentNPCLocation = NPCLocation;
			MyPawn->IsWalkingToNPC = true;
			UAIBlueprintHelperLibrary::SimpleMoveToActor(this, MyNPC);
		}

	}
}

//DIALOGUE HANDLING

//void ATOPDOWN_Take1PlayerController::GeneratePlayerLines(UDataTable& PlayerLines)
//{
//	TArray<FName> PlayerOptions = PlayerLines.GetRowNames();
//
//	for (auto It: PlayerOptions)
//	{
//		FShortDialogue ShortDialogue1 = RetrieveDialogue(&PlayerLines, It);
//
//		if (ShortDialogue1)
//		{
//			Questions.Add(ShortDialogue1->QuestionExcerpt);
//		}
//	}
//
//	AvailableLines = &PlayerLines;
//}
//
//FShortDialogue* ATOPDOWN_Take1PlayerController::RetrieveDialogue(UDataTable* TableToSearch, FName RowName)
//{
//	if (!TableToSearch) return nullptr;
//
//	FString ContextString;
//	return TableToSearch->FindRow<FShortDialogue>(RowName, ContextString);
//}
//
//void ATOPDOWN_Take1PlayerController::Talk(FString Excerpt, TArray<FSubtitle>& Subtitles)
//{
//	TArray<FName> PlayerOptions = AvailableLines->GetRowNames();
//
//	for (auto It : PlayerOptions)
//	{
//		FShortDialogue* ShortDialogue1 = RetrieveDialogue(AvailableLines, It);
//		if (ShortDialogue1 && ShortDialogue1->QuestionExcerpt == Excerpt)
//		{
//			AudioComp->SetSound(ShortDialogue1->SFX);
//			AudioComp->Play();
//
//			Subtitles = ShortDialogue1->Subtitles;
//
//			if (UI && AssociatedPawn && ShortDialogue1->bShouldAIAnswer)
//			{
//				TArray<FSubtitle> SubtitlesToDisplay;
//				float TotalSubsTime = 0.f;
//				for (int32 i = 0; i < Subtitles.Num(); i++)
//				{
//					TotalSubsTime += Subtitles[i].AssociatedTime;
//				}
//				TotalSubsTime += 1.f;
//
//				AssociatedPawn->AnswerToCharacter(It, SubtitlesToDisplay, TotalSubsTime);
//			}
//			else if (!ShortDialogue1->bShouldAIAnswer)
//			{
//				bIsTalking = !bIsTalking;
//				ToggleUI();
//			}
//			break;
//		}
//	}
//	
//}




//DASH

void ATOPDOWN_Take1PlayerController::StopDashing()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		UE_LOG(LogTemp, Warning, TEXT("STOPPING DASH"));
		CanDash = false;
		MyPawn->IsJumping = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATOPDOWN_Take1PlayerController::ResetDash, DashCooldown, false);
	}
}

void ATOPDOWN_Take1PlayerController::ResetDash()
{
	CanDash = true;
}


void ATOPDOWN_Take1PlayerController::DoubleJump()
{
	UE_LOG(LogTemp, Warning, TEXT("TRYING TO DASH"));
	if (CanDash == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("CANDASH TRUE"));
		if (DoubleJumpCounter <= 1)
		{
			AutoAttackCancel();
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
			{
				if (MyPawn->IsPlunging == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("ISPLUNGING FALSE"));
					FHitResult HitTurn;
					GetHitResultUnderCursor(ECC_Camera, false, HitTurn);

					if (HitTurn.bBlockingHit)
					{
						FVector ActorLocation = MyPawn->GetActorLocation();
						FVector CursorFV = HitTurn.ImpactPoint;
						FRotator xak = (CursorFV - ActorLocation).ToOrientationRotator();
						FRotator CursorR = CursorFV.Rotation();

						MyPawn->SetActorRotation(FRotator(0, xak.Yaw, 0));
					}

					FVector foo = MyPawn->ACharacter::GetActorForwardVector();
					FVector bee = FVector::VectorPlaneProject(foo, FVector(0, 0, 1));
					FVector bar = FVector(DashMulti * bee.X, DashMulti * bee.Y, JumpHeight);

					//FRotator xak = (bar.ToOrientationRotator());
					//FRotator xak2 = FRotator(0, xak.Yaw, 0);
					//MyPawn->SetActorRotation(xak2);

					UE_LOG(LogTemp, Warning, TEXT("original vector: %s"), *MyPawn->ACharacter::GetActorForwardVector().ToString());
					UE_LOG(LogTemp, Warning, TEXT("original velocity: %s"), *MyPawn->ACharacter::GetVelocity().ToString());
					UE_LOG(LogTemp, Warning, TEXT("rotated velocity: %s"), *VelocityRotated.ToString());
					UE_LOG(LogTemp, Warning, TEXT("modified vector: %s"), *bar.ToString());

					MyPawn->IsJumping = true;

					MyPawn->GetCharacterMovement()->BrakingFrictionFactor = 0.f;
					MyPawn->GetCharacterMovement()->MaxWalkSpeed = 3000;

					MyPawn->LaunchNuller();

					MyPawn->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
					MyPawn->ACharacter::LaunchCharacter(bar, true, true);

					DoubleJumpCounter++;
					/*			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATOPDOWN_Take1PlayerController::StopDashing, DashStop, false);*/
				}
			}
		}
	}
}

//PLUNGE

void ATOPDOWN_Take1PlayerController::PlungeBranch()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		if (MyPawn->IsPlunging == false) Ascend();
		else Plunge(); 
	}
}

void ATOPDOWN_Take1PlayerController::StopPlunging()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		UE_LOG(LogTemp, Warning, TEXT("STOPPING PLUNGE"));
		MyPawn->Target_Runes->SetHiddenInGame(true, true);
		MyPawn->IsPlunging = false;
		CrackLocation = MyPawn->GetActorLocation()-FVector(0,0,90);
		MyPawn->GetCharacterMovement()->StopMovementImmediately();

		TSubclassOf<UDamageType> P; //damage type placeholder
		TArray<AActor*> IgnoreActors; 
		UGameplayStatics::ApplyRadialDamage(GetWorld(), PlungeDamage, MyPawn->GetActorLocation(), PlungeDamageRadius, P, IgnoreActors, MyPawn, this, true, ECC_Visibility);
		MyPawn->SpawnDecal_Crack1(CrackLocation);
		MyPawn->CollisionSphere->SetGenerateOverlapEvents(false);
		MyPawn->CollisionSphereBig->SetGenerateOverlapEvents(false);

		
		GetWorldTimerManager().SetTimer(UnusedHandle3, this, &ATOPDOWN_Take1PlayerController::AutoAttackRelease, 1.0f, false);
		
		GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATOPDOWN_Take1PlayerController::ResetPlunge, PlungeCooldown, false);
	}
}

void ATOPDOWN_Take1PlayerController::ResetPlunge()
{
	UE_LOG(LogTemp, Warning, TEXT("RESETTING CANPLUNGE"));
	CanPlunge = true;
}

void ATOPDOWN_Take1PlayerController::Ascend()
{
	
	UE_LOG(LogTemp, Warning, TEXT("TRYING TO PLUNGE"));
	if (CanPlunge == true)
	{
		AutoAttackCancel();
		UE_LOG(LogTemp, Warning, TEXT("CANPLUNGE TRUE"));
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
		{
			if (MyPawn->GetCharacterMovement()->IsFalling() == false)
			{


				//MyPawn->PlayerAttributeComponent->PlayerMovementSpeed.SetCurrentValue(1200);
				//UE_LOG(LogTemp, Warning, TEXT("READING PLAYER MOVEMENT SPEED FROM MOVECOMP, post-change: %f"), MyPawn->PlayerAttributeComponent->PlayerMovementSpeed.GetCurrentValue());


				
				PlungeLocation = MyPawn->GetActorLocation();
				UE_LOG(LogTemp, Warning, TEXT("ISFALLING FALSE"));
				FHitResult HitTurn;
				GetHitResultUnderCursor(ECC_Visibility, false, HitTurn);
				float const Distance = FVector::Dist(HitTurn.ImpactPoint, MyPawn->GetActorLocation());

					if (HitTurn.bBlockingHit)
					{
						FVector ActorLocation = MyPawn->GetActorLocation();
						FVector CursorFV = HitTurn.ImpactPoint;
						FRotator xak = (CursorFV - ActorLocation).ToOrientationRotator();
						FRotator CursorR = CursorFV.Rotation();

						MyPawn->SetActorRotation(FRotator(0, xak.Yaw, 0));
					}

					FVector bar = FVector(0, 0, RiseHeight);
					MyPawn->LaunchNuller();
					MyPawn->ACharacter::LaunchCharacter(bar, true, true);
				
					//animation
					UE_LOG(LogTemp, Warning, TEXT("Trying to play animation"));
					MyPawn->PlayAnimMontage(MyPawn->RiseFallMontage, 1.f, FName("Rising"));
					

					MyPawn->IsAscending = true;
					MyPawn->IsPlunging = true;
					MyPawn->OriginalBoomLength = MyPawn->CameraBoom->TargetArmLength;
					MyPawn->ZoomSize = 2000;
					MyPawn->ZoomRate = 60;
					MyPawn->ZoomTo();
					CanPlunge = false;

					//GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATOPDOWN_Take1PlayerController::Plunge, PlungeRise, false);
			}
		}
		
	}
	
}

void ATOPDOWN_Take1PlayerController::PitchNuller()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		UE_LOG(LogTemp, Warning, TEXT("PLUNGE STOPPED"))
		FRotator foo = MyPawn->GetActorRotation();
		MyPawn->SetActorRotation(FRotator(0, foo.Yaw, 0));
		MyPawn->GetCharacterMovement()->StopMovementImmediately();
		StopPlunging();
	}
}

void ATOPDOWN_Take1PlayerController::Plunge()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		FHitResult HitTurn;
		GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitTurn);
		FHitResult HitResult2 = HitTurn;
		
		if (HitTurn.bBlockingHit)
		{
			float Distance = FVector::Dist(HitResult2.ImpactPoint, MyPawn->GetActorLocation());
			FVector ActorLocation = MyPawn->GetActorLocation();
			float ActorDeltaZ = (ActorLocation - PlungeLocation).Z;
			UE_LOG(LogTemp, Warning, TEXT("Delta Z: %f"), ActorDeltaZ);

			if (Distance > PlungeMin && ActorDeltaZ > 400 && Distance < PlungeMax)
			{
				//DECAL CONTROL
				MyPawn->IsAscending = false;
				MyPawn->Target_Runes->SetHiddenInGame(true, true);
				
				FVector Destination = HitResult2.ImpactPoint - FVector(0, 0, 1000.f);
				FRotator xak = (Destination-ActorLocation).ToOrientationRotator();
				MyPawn->SetActorRotation(xak);

				FVector foo = xak.Vector();
				//FVector foo = MyPawn->GetActorForwardVector();
				FVector bar = 10000 * foo;

				UE_LOG(LogTemp, Warning, TEXT("modified vector: %s"), *bar.ToString());
				MyPawn->ACharacter::LaunchCharacter(bar, true, true);

				FVector CurrentVelo = MyPawn->GetVelocity();
				FlightTimer = 1*(Distance / bar.Size());

				MyPawn->CollisionSphere->SetGenerateOverlapEvents(true);
				MyPawn->CollisionSphereBig->SetGenerateOverlapEvents(true);
				
				MyPawn->ZoomSize = MyPawn->OriginalBoomLength;
				MyPawn->ZoomRate = abs(MyPawn->OriginalBoomLength-MyPawn->CameraBoom->TargetArmLength)/(FlightTimer/MyPawn->ZoomTick);
				MyPawn->ZoomTo();

				AutoAttackActive = true;
				
				UE_LOG(LogTemp, Warning, TEXT("Current distance: %f"), Distance);
				UE_LOG(LogTemp, Warning, TEXT("Current bar: %s"), *bar.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Current velo: %s"), *CurrentVelo.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Current bar size: %f"), bar.Size());
				UE_LOG(LogTemp, Warning, TEXT("Flight timer: %f"), FlightTimer);
				MyPawn->GetCharacterMovement()->BrakingFrictionFactor = 20000.f;
				
				//GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATOPDOWN_Take1PlayerController::PitchNuller, FlightTimer, false);
				
			}
			/*else MyPawn->Target_Runes->SetHiddenInGame(true, true);*/
		}

		
	}
}





//DEFAULT FUNCTIONS

void ATOPDOWN_Take1PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
			// keep updating the destination every tick while desired
			if (bMoveToMouseCursor)
			{
				if(CastBar)
				{
					CastBar->MoveInterrupt(); 
				}
				MoveToMouseCursor();
			}
			CooldownManager->TickCooldowns(DeltaTime);

}

//INTERFACE

void ATOPDOWN_Take1PlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATOPDOWN_Take1PlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
					UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ATOPDOWN_Take1PlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ATOPDOWN_Take1PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	if (ATOPDOWN_Take1Character* MyPawnCast = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		MyPawnCast->KillAnimation();
		if (!LockInteraction)
		{
			float const Distance = FVector::Dist(DestLocation, MyPawnCast->GetActorLocation());

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if ((Distance > 120.0f) && !AutoAttackActive)
			{

					if (MyPawnCast->isAutoAttacking && MyPawnCast->isPerformingAttack)
					{
						AutoAttackCancel();
					}
				//UE_LOG(LogTemp, Warning, TEXT("destination vector: %s"), *DestLocation.ToString());
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
			}
		}
	}
}

void ATOPDOWN_Take1PlayerController::OnSetDestinationPressed()
{
	if (CastBar)
	{
		CastBar->MoveInterrupt();
	}
	
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
		if (MyPawn->IsJumping==false && !LockInteraction)
		{
			// set flag to keep updating destination until released
			bMoveToMouseCursor = true;
		}
	}
}

void ATOPDOWN_Take1PlayerController::OnSetDestinationReleased()
{
	if (CastBar)
	{
		CastBar->MoveInterrupt();
	}
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ATOPDOWN_Take1PlayerController::ZoomIn()
{
	if (!LockInteraction)
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
		{
			MyPawn->Zoom(-DeltaZoom);
		}
	}
}

void ATOPDOWN_Take1PlayerController::ZoomOut()
{
	if (!LockInteraction)
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
		{
			MyPawn->Zoom(DeltaZoom);
		}
	}
}

void ATOPDOWN_Take1PlayerController::CameraUnlockToggle()
{
	if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(GetPawn()))
	{
			if (MyPawn->isCameraLocked)
			{

				MyPawn->CameraUnlock();
				UE_LOG(LogTemp, Warning, TEXT("Unlocking camera"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Locking camera"));
				MyPawn->CameraLock();
			}
	}
}
