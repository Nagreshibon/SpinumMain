// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "TOPDOWN_Take1PlayerController.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HP_MAX = 100;
	HP = HP_MAX;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* Owner = GetOwner();
	if(Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health component initiated, Owner cast succesful"));
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCause)
{
	if (Damage <= 0)
	{
		return;
	}

	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (AActor* MyPawn = Cast<AActor>(MyController->GetPawn()))
		{
			if (ATestNPC1* Owner = Cast<ATestNPC1>(GetOwner()))
			{
				if (DamageCause == MyPawn && Owner->isEnemy == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("Ignoring damage caused by player"));
					return;
				}
			}
		}
	}
	
	HP = FMath::Clamp(HP - Damage, 0.0f, HP_MAX);
	UE_LOG(LogTemp, Warning, TEXT("Damage taken! Current HP (component): %f"), HP);
	
}

