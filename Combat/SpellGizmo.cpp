// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellGizmo.h"


#include "Kismet/GameplayStatics.h"
#include "TOPDOWN_Take1/TestNPC1.h"

// Sets default values
ASpellGizmo::ASpellGizmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot; 
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	CollisionMesh->SetupAttachment(RootComponent); 
	CollisionMesh->SetHiddenInGame(true);
	CollisionMesh->SetGenerateOverlapEvents(false);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpellGizmo::OnOverlapsBegin);
	
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(RootComponent); 
	//ProjectileMovement->InitializeComponent(); 

}

ASpellGizmo::~ASpellGizmo()
{
}

void ASpellGizmo::OnOverlapsBegin_Implementation(UPrimitiveComponent* Comp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ATestNPC1* EnemyCast = Cast<ATestNPC1>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT ENEMY - %s"), *OtherActor->GetHumanReadableName());
		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn(), DamageClass);
		if(StunDuration > 0) EnemyCast->Stun(StunDuration);
		
	}
}

void ASpellGizmo::BeginKill(float Lifetime)
{
	if (Lifetime > 0.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trying to kill gizmo after %f lifetime"), Lifetime);
		FTimerDelegate KillGizmoDelegate = FTimerDelegate::CreateUObject(this, &ASpellGizmo::Kill);
		GetGameInstance()->GetWorld()->GetTimerManager().SetTimer(UnusedHandleGizmo, this, &ASpellGizmo::Kill, Lifetime, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lifetime set to 0, the gizmo will live indefinitely"));
	}
}

void ASpellGizmo::Kill()
{
	//UE_LOG(LogTemp, Warning, TEXT("Killing gizmo"));
	if (this)
	{
		this->Destroy();
	}
}

// Called when the game starts or when spawned
void ASpellGizmo::BeginPlay()
{
	Super::BeginPlay();


	
	
}

// Called every frame
void ASpellGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

