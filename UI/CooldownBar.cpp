// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/CooldownBar.h"

UCooldownBar::UCooldownBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCooldownBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MyController)
	{
		ListBox->ClearChildren();
		if (CooldownBar_ListPart_BP)
		{
			for (auto& Elem : MyController->CooldownManager->GetMap())
			{
				if (Elem.Value > 0)
				{
					//UUserWidget* CurrentItem = CreateWidget<UCooldownBar_ListPart>(MyController, CooldownBar_ListPart_BP, FName(*(Elem.Key->GetDisplayNameText().ToString())));
					UCooldownBar_ListPart* CurrentItem = CreateWidget<UCooldownBar_ListPart>(MyController, CooldownBar_ListPart_BP, FName(*(Elem.Key->GetDisplayNameText().ToString())));

					//UE_LOG(LogTemp, Warning, TEXT("Class: %s"), *Elem.Key->GetDisplayNameText().ToString());
					//UE_LOG(LogTemp, Warning, TEXT("Widget alignment: %s"), *CurrentItem->GetAlignmentInViewport().ToString());

					ListBox->AddChildToVerticalBox(CurrentItem);

					///Elem.Key->GetDefaultObject()

					if (USpellClass* SpellCast = Cast<USpellClass>(Elem.Key->GetDefaultObject()))
					{
						//UE_LOG(LogTemp, Warning, TEXT("Class: %f / %f"), Elem.Value, SpellCast->GetCooldown());
						CurrentItem->Bar->SetPercent(Elem.Value / SpellCast->GetCooldown());
						CurrentItem->Icon->SetBrushFromTexture(SpellCast->Icon);
						ForceLayoutPrepass();
					}
					//else UE_LOG(LogTemp, Warning, TEXT("SpellClass cast failed"));
				}

			}
		}
	}
}

void UCooldownBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (ATOPDOWN_Take1PlayerController* MyControllerCast = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		MyController = MyControllerCast; 
	}
}