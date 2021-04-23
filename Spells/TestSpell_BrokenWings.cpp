// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSpell_BrokenWings.h"
#include "UObject/ConstructorHelpers.h" 
#include "TOPDOWN_Take1/TOPDOWN_Take1Character.h"

UTestSpell_BrokenWings::UTestSpell_BrokenWings(const FObjectInitializer& Object) : Super(Object)
{
	HopVertical = 100.f;
	HopMultiplier = 1000.f;
	LongCooldown = 8.f; 
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BrokenWingsMontageObject(TEXT("AnimMontage'/Game/TopDownCPP/Combat/BrokenWings_FX/BrokenWingsMontage2.BrokenWingsMontage2'"));
	if (BrokenWingsMontageObject.Succeeded())
	{
		BrokenWingsMontage = BrokenWingsMontageObject.Object;
	}
}

bool UTestSpell_BrokenWings::PreExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		{
			MyPawn->GetCharacterMovement()->StopMovementImmediately();
			MyController->StopMovement(); 
			
			FString MontageSection = "Attack" + FString::FromInt(MyPawn->BrokenWingsCounter);
			UE_LOG(LogTemp, Warning, TEXT("PLAYING SECTION %s"), *MontageSection);
			MyPawn->PlayAnimationWithDuration(BrokenWingsMontage, AnimationRate, MaxDuration, FName(*MontageSection));
			if (Slash1SFX && Slash2SFX && Slash3SFX)
			{
				switch (MyPawn->BrokenWingsCounter)
				{
				case 1:
					MyPawn->AudioComp->SetSound(Slash1SFX);
					MyPawn->AudioComp->Play();
					break;
				case 2:
					MyPawn->AudioComp->SetSound(Slash2SFX);
					MyPawn->AudioComp->Play();
					break;
				case 3:
					MyPawn->AudioComp->SetSound(Slash3SFX);
					MyPawn->AudioComp->Play();
					break;
				default:
					break;
				}
			}
			
		}
	}
	return true;
}

void UTestSpell_BrokenWings::ExecuteGameplayEffect(FTransform SpellSpawnLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Launching!"));
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		{
			FRotator CurrentRotation = MyPawn->GetActorRotation();

			MyPawn->LaunchCharacter((CurrentRotation.Vector() * HopMultiplier) + FVector(0,0, HopVertical), false, false);

			if (MyPawn->BrokenWingsCounter <= 3)
			{
				//section string reference

				MyPawn->BrokenWingsCounter++;
			}
			/*else
			{

				MyPawn->BrokenWingsCounter = 1;
			}*/
		}
	}
}

float UTestSpell_BrokenWings::GetCooldown()
{
	if (GetWorld())
	{
		if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
			{
				if (MyPawn->BrokenWingsCounter == 4)
				{
					MyPawn->BrokenWingsCounter = 1;
						return LongCooldown;
				}
				else
					return SpellCooldown;
			}
		}
	}
	return LongCooldown;
}
