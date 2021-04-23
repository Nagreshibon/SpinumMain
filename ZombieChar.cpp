// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieChar.h"
#include "AI/TestAIController.h"
#include "TimerManager.h"

AZombieChar::AZombieChar()
{
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(FName(TEXT("PawnSense")));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage1Object(TEXT("AnimMontage'/Game/Mixamo/Mutant/Zombie_Attack1Montage.Zombie_Attack1Montage'"));
	if (AttackMontage1Object.Succeeded())
	{
		AttackMontage1 = AttackMontage1Object.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObject(TEXT("AnimMontage'/Game/Mixamo/Mutant/Zombie_Death_mixamo_com_Montage.Zombie_Death_mixamo_com_Montage'"));
	if (DeathMontageObject.Succeeded())
	{
		DeathMontage = DeathMontageObject.Object;
	}
	//AttributeComponent->AttackDamage = 20;
	//AttributeComponent->Health = 100;
}

void AZombieChar::BeginPlay()
{
	Super::BeginPlay();

	if(PawnSense)
	{
		PawnSense->OnSeePawn.AddDynamic(this, &AZombieChar::OnSeePlayer);
		PawnSense->OnHearNoise.AddDynamic(this, &AZombieChar::OnHearNoise);
	}

	if(HealthComponent)
	{
		HealthComponent->HP_MAX = 100;
		HealthComponent->HP = 100; 
	}

	if(AttributeComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SETTING ZOMBIE ATTRIBUTES"));
		AttributeComponent->AttackDamage = 30;
		AttributeComponent->Health = 100;
		AttributeComponent->AttackSpeed = 1.0f;
		AttributeComponent->MeleeRange = 350.0f; 
	}
}

void AZombieChar::OnSeePlayer(APawn* Pawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("PLAYER SPOTTED"));
	if(ATestAIController* MyController = Cast<ATestAIController>(GetController()))
	{
		MyController->SetTargetEnemy(Pawn);
	}
	
	//TestDialogue3(FText::FromString("PLAYER SPOTTED"), 25.0f);
}

float AZombieChar::Attack1(APawn* Target)
{
	/*FVector Source1 = FVector(-2057.553223, 312.233459, 99.269928);
	FRotator Source2 = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;*/

	isAttacking = true;
	float AttackPlayRate = AttributeComponent->AttackSpeed.GetCurrentValue();
	float AttackDuration = PlayAnimMontage(AttackMontage1, AttackPlayRate, FName("Default"));
	/*FTimerDelegate SpawnGizmo = FTimerDelegate::CreateUObject(this, &AZombieChar::Attack1SpawnGizmo, Target);
	GetWorldTimerManager().SetTimer(UnusedHandle, AttackAgain, AttackDuration, false);*/
	
	//FTransform SpawnLocation;
	//GetWorld()->SpawnActor<AMyFirstActor>(AMyFirstActor::StaticClass(), &SpawnLocation);
	//AActor* Gizmo = GetWorld()->SpawnActor<AActor>(Attack1Gizmo, SpawnLocation);
	GetWorldTimerManager().SetTimer(UnusedHandle2, this, &AZombieChar::Attack1Release, AttackDuration, false);

	//UE_LOG(LogTemp, Warning, TEXT("Returning AttackDuration = %f"), AttackDuration);

	return AttackDuration; 
}

void AZombieChar::Attack1SpawnGizmo(APawn* Target)
{
	if(Target)
	{
		if (Attack1Gizmo)
		{
			//UE_LOG(LogTemp, Warning, TEXT("SPAWNING GIZMO2"));
			FTransform SpawnLocation = GetActorTransform();
			/*FQuat Roto = SpawnLocation.GetRotation();
			FQuat Roto2 = Roto * FQuat(0, 0, 1, 0.2);
			SpawnLocation.SetRotation(Roto2); */
			//GetWorld()->SpawnActor<AMyFirstActor>(AMyFirstActor::StaticClass(), &SpawnLocation);
			AActor* Gizmo = GetWorld()->SpawnActor<AActor>(Attack1Gizmo, SpawnLocation);
			Gizmo->SetInstigator(this); 

			FTimerDelegate KillGizmo = FTimerDelegate::CreateUObject(this, &AZombieChar::Attack1KillGizmo, Gizmo);
			GetWorldTimerManager().SetTimer(UnusedHandle, KillGizmo, 0.3f, false);
		}
	}
}

void AZombieChar::Attack1KillGizmo(AActor* Gizmo)
{
	Gizmo->Destroy();
}

void AZombieChar::Attack1Release()
{
	StopAnimMontage(AttackMontage1); 
	isAttacking = false; 
}

void AZombieChar::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
}
