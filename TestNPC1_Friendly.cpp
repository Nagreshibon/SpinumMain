// Fill out your copyright notice in the Description page of Project Settings.


#include "TestNPC1_Friendly.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "TOPDOWN_Take1PlayerController.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "DialogueData/DialoguePicker.h"
#include "DialogueData/NPCLocalVariables.h"
#include "Misc/DefaultValueHelper.h"
#include "Navigation/PathFollowingComponent.h"
#include "UI/FloaterWidget1.h"
#include "UI/HPFloater.h"
#include "UI/HUD_Default1.h"
#include "UI/HUD_Default2.h"
#include "UI/RadialMenu1.h"

ATestNPC1_Friendly::ATestNPC1_Friendly()
{
	
	//AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	//AudioComp->AttachTo(GetRootComponent());
	
	bIsAnsweringRadial = false;
	bIsInShortDialogue = false;
	isEnemy = false;
	TestValue = 3;
	Welcome = "Hello I'm friendly.";
	
	//if (GetWorld())
	//{
	//UE_LOG(LogTemp, Warning, TEXT("Creating widget from ref"));
	//FloaterWidgetBPRef = CreateWidget<UFloaterWidget1>(GetWorld()->GetFirstPlayerController(), FloaterWidgetBP);
	//}

	//NameFloaterBlock2 = CreateDefaultSubobject<UWidgetComponent>(FName("NameFloaterBlock2"));
	//NameFloaterBlock2->SetupAttachment(RootComponent);
	//NameFloaterBlock2->SetWidgetSpace(EWidgetSpace::Screen);
	//NameFloaterBlock2->SetHiddenInGame(true);

	//RadialMenuBlock1 = CreateDefaultSubobject<UWidgetComponent>(FName("RadialMenuBlock1"));
	//RadialMenuBlock1->SetupAttachment(RootComponent);
	//RadialMenuBlock1->SetWidgetSpace(EWidgetSpace::Screen);
	//RadialMenuBlock1->SetHiddenInGame(true);
	//
	//UE_LOG(LogTemp, Warning, TEXT("Setting widget class from ref"));
//	NameFloaterBlock2->SetWidgetClass(FloaterWidgetBP);
//	NameFloaterBlock2->SetWidget(FloaterWidgetBPRef);


//		if (FloaterWidgetBP)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Setting NameFloaterBlock to created widget"));
//			NameFloaterBlock->SetWidgetClass(FloaterWidgetBP);
//		}

}

void ATestNPC1_Friendly::BeginPlay()
{
	Super::BeginPlay();
	if (FloaterWidgetBP) NameFloaterBlock2->SetWidgetClass(FloaterWidgetBP);
	if (RadialMenuBP) RadialMenuBlock1->SetWidgetClass(RadialMenuBP);

	if (UFloaterWidget1* MyFloater = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting portrait"));
		MyFloater->PortraitBlock->SetBrushFromTexture(PortraitImage);
		if (bHidePortrait)
		{
			MyFloater->PortraitBlock->SetVisibility(ESlateVisibility::Collapsed); 
		}
	}

	if (UHPFloater* HPFloaterBlock = Cast<UHPFloater>(HPFloater1->GetUserWidgetObject()))
	{
		HPFloaterBlock->HPBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor(0,0,255,1));
	}

	////Register the begin and end overlap functions
	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATestNPC1_Friendly::OnBoxOverlap);
	//SphereComp->OnComponentEndOverlap.AddDynamic(this, &ATestNPC1_Friendly::OnBoxEndOverlap);

}

void ATestNPC1_Friendly::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsAnsweringRadial)
	{
		if (RadialMenuBP)
		{
			if (PickedButton > 0)
			{
				RadialMenuBlock1->SetHiddenInGame(true);
				i = i + (PickedButton - 1);
				PickedButton = 0;


				bIsAnsweringRadial = false;
				/*RadialMenuBlock1->SetWidgetClass(RadialMenuBP);*/
				if (URadialMenu1* MyRadialMenu = Cast<URadialMenu1>(RadialMenuBlock1->GetUserWidgetObject()))
				{
					MyRadialMenu->RadialAnswers.Empty();
					MyRadialMenu->ButtonCountCPPMenu = 0;

					MyRadialMenu->RadialMenu1Destroy();
					
				}
				TestDialogue();
			}
		}
	}

	if (bIsInShortDialogue)
	{
		GetCharacterMovement()->StopMovementImmediately();
		if (UPathFollowingComponent* MyFollowingComp = Cast<UPathFollowingComponent>(GetController()->GetPathFollowingAgent()))
		{
			MyFollowingComp->AbortMove(TEXT("ok"));
		}
	}
	
}

void ATestNPC1_Friendly::HideFloater()
{
	if (NameFloaterBlock2)
	{
		NameFloaterBlock2->SetHiddenInGame(true);
		bIsInShortDialogue = false;
	}
	if(ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (AHUD_Default2* MyHUD = Cast<AHUD_Default2>(MyController->GetHUD()))
		{
			if (UHUD_Default1* MyHUDWidget = Cast<UHUD_Default1>(MyHUD->HUDWidget))
			{
				MyHUDWidget->TalkPortrait->SetVisibility(ESlateVisibility::Hidden); 
			}
		}
	}
}

void ATestNPC1_Friendly::HideFloaterChar()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to cast world"));
	UWorld* World = GetWorld();
	if (World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to cast char"));
		if (ATOPDOWN_Take1Character* MyCharacter = Cast<ATOPDOWN_Take1Character>(UGameplayStatics::GetPlayerCharacter(World, 0)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Character cast succesful, hiding the floater"));
			MyCharacter->NameFloaterBlock2->SetHiddenInGame(true);
		}
	}
}

void ATestNPC1_Friendly::SetDialogue(int NewIndex, UDataTable* NewDialoguePicker)
{
	DialoguePicker = NewDialoguePicker;
	DialogueIndex = NewIndex; 	
}

void ATestNPC1_Friendly::TestDialogue()
{
	
	if (URadialMenu1* MyRadialMenu = Cast<URadialMenu1>(RadialMenuBlock1->GetUserWidgetObject()))
	{
		if (UFloaterWidget1* MyFloaterWidget = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
		{

			//conver DialogueIndex to string
			FString DialogueContextString = FString::FromInt(DialogueIndex);
			if (FDialoguePicker* DialoguePickerRow = DialoguePicker->FindRow<FDialoguePicker>(FName(*DialogueContextString), TEXT("")))
			{
				UE_LOG(LogTemp, Warning, TEXT("DialoguePickerRow cast"));
				if (UDataTable* DialogueBodyRow = (*DialoguePickerRow).DialogueBody)
				{
					UE_LOG(LogTemp, Warning, TEXT("DialogueBodyRow cast"));

					//convert i to string
					FString iString = FString::FromInt(i);
					UE_LOG(LogTemp, Warning, TEXT("i = %s"), *iString);
					if (FDialogueBody* foo = DialogueBodyRow->FindRow<FDialogueBody>(FName(*iString), TEXT("")))
					{
						UE_LOG(LogTemp, Warning, TEXT("foo cast"));

						//NPC IsInShortDialogue
						bIsInShortDialogue = true;
						bool bReset = false;
						
						FText bar = (*foo).DialogueText;
						float Duration;
						//Line duration
						if ((*foo).Duration == 0) Duration = 2.5f; else Duration = (*foo).Duration;

						if((*foo).Effect)
						{
							UCustomGameplayEffect* EffectInstance = NewObject<UCustomGameplayEffect>(this, (*foo).Effect->StaticClass());
							EffectInstance->ExecuteGameplayEffect(); 
						}
						
						//Dialogue Text
						if ((*foo).DialogueAdditional.Num() > 0)
						{
							UE_LOG(LogTemp, Warning, TEXT("Running preprint"));
							
							int ArrNum = (*foo).DialogueAdditional.Num();
							UE_LOG(LogTemp, Warning, TEXT("Array Num: %d"), ArrNum);

							int j = 0;
							while (j < ArrNum)
							{
								UE_LOG(LogTemp, Warning, TEXT("J: %d"), j);
								if (j != ArrNum)
								{
									FString read = ((*foo).DialogueAdditional)[j];
									UE_LOG(LogTemp, Warning, TEXT("Read string: %s"), *read);
									
									if (read == "RANDOM_GOSSIP" && j+1<ArrNum)
									{
										int i_random; 
										TArray<FString> RandArray;
										FString RandString = ((*foo).DialogueAdditional)[j + 1];
										int32 min, max;

										RandString.ParseIntoArray(RandArray, TEXT(" "), true);
										FDefaultValueHelper::ParseInt(RandArray[0], min);
										FDefaultValueHelper::ParseInt(RandArray[1], max);

										if (min && max)
										{
											UE_LOG(LogTemp, Warning, TEXT("Random range: %d %d"), min, max);
											i_random = FMath::RandRange(min, max);
										}
										FString rString = FString::FromInt(i_random);

										if (FDialogueBody* xak = DialogueBodyRow->FindRow<FDialogueBody>(FName(*rString), TEXT("")))
										{
											bar = (*xak).DialogueText;
											if ((*xak).Duration == 0) { Duration = 2.5f; }
											else Duration = (*xak).Duration;
										}

										j++;
										
									}

								}
								j++;
							}
						}
						
						
						
						
						//end dialogue marker
						bool End = (*foo).bEnd;
						
						//index
						if ((*foo).NextIndex != 0) i = (*foo).NextIndex; else i++;

						//set text

						//if CHAR talking
						if ((*foo).IDName == "CHAR")
						{
							UWorld* World = GetWorld();
							if (World)
							{
								if (ATOPDOWN_Take1Character* MyCharacter = Cast<ATOPDOWN_Take1Character>(UGameplayStatics::GetPlayerCharacter(World, 0)))
								{
									UE_LOG(LogTemp, Warning, TEXT("Showing character's floater"));
									MyCharacter->NameFloaterBlock2->SetHiddenInGame(false);
									if (UFloaterWidget1* MyFloaterWidgetChar = Cast<UFloaterWidget1>(MyCharacter->NameFloaterBlock2->GetUserWidgetObject()))
									{
										HideFloater();
										MyFloaterWidgetChar->FloaterTextBlock->SetText(bar);

										if ((*foo).SFX)
										{
											MyCharacter->AudioComp->SetSound((*foo).SFX);
											MyCharacter->AudioComp->Play();
										}
										
										UE_LOG(LogTemp, Warning, TEXT("Firing HideFloaterChar in %f"), Duration);
										GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATestNPC1_Friendly::HideFloaterChar, Duration, false);
										Duration = Duration + 0.5f;
									}
								}
							}
						}
						else
						{
							if ((*foo).SFX)
							{
								AudioComp->SetSound((*foo).SFX);
								AudioComp->Play();
							}
							NameFloaterBlock2->SetHiddenInGame(false);
							MyFloaterWidget->FloaterTextBlock->SetText(bar);
						}
						
						//set radial menu
						if ((*foo).bRadialAnswer)
						{
							int ArrNumRad = (*foo).RadialAnswersArr.Num();
							if (ArrNumRad > 0)
							{
								//constructing menu
								UE_LOG(LogTemp, Warning, TEXT("Loading RadialAnswers into target array"));
								MyRadialMenu->RadialAnswers = (*foo).RadialAnswersArr;
								MyRadialMenu->ButtonCountCPPMenu = (*foo).RadialAnswersArr.Num();
								RadialMenuBlock1->SetHiddenInGame(false);

								//awaiting answer
								bIsAnsweringRadial = true;

								i = (*foo).RadialAnswersIndexArr[0];
								/*MyRadialMenu->RadialMenu1Reconstruct();*/
								
								
							}
						}
						else
						{
							// end or continue, depending on DialogueAdditional
							if (((*foo).DialogueAdditional).Num() == 0)
							{
								UE_LOG(LogTemp, Warning, TEXT("DialogueAdditional empty"));
								if (End)
								{
									DialogueIndex++;
									i = 0;
									GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::HideFloater, Duration, false);
								}
								else
								{
									UE_LOG(LogTemp, Warning, TEXT("Firing TestDialogue again in %f"), Duration);
									GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, Duration, false);
								}
							}
							else if ((*foo).DialogueAdditional.Num() > 0)
							{
								int ArrNum = (*foo).DialogueAdditional.Num();
								UE_LOG(LogTemp, Warning, TEXT("Array Num: %d"), ArrNum);

								int j = 0;
								while (j < ArrNum)
								{
									UE_LOG(LogTemp, Warning, TEXT("J: %d"), j);
									if (j != ArrNum)
									{
										FString read = ((*foo).DialogueAdditional)[j];
										UE_LOG(LogTemp, Warning, TEXT("Read string: %s"), *read);

										if (read == "DLG_INDEX_RESET")
										{
											UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to 0"));
											DialogueIndex = 0;
											i = 0;
											bReset = true; 
										}
										else if (read == "ZOOM" && j + 1 < ArrNum)
										{
											if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
											{
												FString ZoomString = ((*foo).DialogueAdditional)[j + 1];
												int32 ZoomInt;
												FDefaultValueHelper::ParseInt(ZoomString, ZoomInt);
												MyPawn->ZoomSize = ZoomInt;
												MyPawn->ZoomTo();
												UE_LOG(LogTemp, Warning, TEXT("Firing TestDialogue again in %f"), Duration);
												GetWorldTimerManager().SetTimer(UnusedHandle4, this, &ATestNPC1_Friendly::TestDialogue, Duration, false);
											}
										}
										else if (read == "DLG_INDEX_NEXT")
										{
											FString IndexString = ((*foo).DialogueAdditional)[j + 1];

											FDefaultValueHelper::ParseInt(IndexString, DialogueIndex);
											UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to %d"), DialogueIndex);
										}
										else if (read == "MOVE" && (j+1) < ArrNum)
										{
											TArray<FString> VectorArray;
											FString VectorString = ((*foo).DialogueAdditional)[j+1];

											VectorString.ParseIntoArray(VectorArray, TEXT(" "), true);
											FVector Move = FVector(FCString::Atof(*VectorArray[0]), FCString::Atof(*VectorArray[1]), FCString::Atof(*VectorArray[2]));
											UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Move);
											
										}
										else if (read == "RANDOM_RESPONSE" && j+1<ArrNum)
										{
											TArray<FString> RandArray;
											FString RandString = ((*foo).DialogueAdditional)[j + 1];
											int32 min, max;
											
											RandString.ParseIntoArray(RandArray, TEXT(" "), true);
											FDefaultValueHelper::ParseInt(RandArray[0], min);
											FDefaultValueHelper::ParseInt(RandArray[1], max);

											if (min && max)
											{
												UE_LOG(LogTemp, Warning, TEXT("Random range: %d %d"), min,max);
												i = FMath::RandRange(min, max);
											}
											UE_LOG(LogTemp, Warning, TEXT("Setting i to: %d"), i);
											UE_LOG(LogTemp, Warning, TEXT("Firing TestDialogue again in %f"), Duration);
											GetWorldTimerManager().SetTimer(UnusedHandle3, this, &ATestNPC1_Friendly::TestDialogue, Duration, false);
										}
										else if (read == "SWITCH_I")
										{
											UE_LOG(LogTemp, Warning, TEXT("Initializing switch"));
												FString SwitchString = ((*foo).DialogueAdditional)[j + 1];											
												TArray<FString> SwitchArr;
												SwitchString.ParseIntoArray(SwitchArr, TEXT(" "), true);
												if (SwitchArr.Num() > 2)
												{
													FString VarName = SwitchArr[0];
													int CaseCount = (SwitchArr.Num() - 1) / 2;
													UE_LOG(LogTemp, Warning, TEXT("VarName: %s, cases: %d"), *VarName, CaseCount);
													int l = 1;
													
													while (l<SwitchArr.Num())
													{
														UE_LOG(LogTemp, Warning, TEXT("Processing cases"));
														if (FNPCLocalVariables* LOCALS = NPCLocals->FindRow<FNPCLocalVariables>(FName(*VarName), TEXT("")))
														{															
															UE_LOG(LogTemp, Warning, TEXT("Executing cases"));
															UE_LOG(LogTemp, Warning, TEXT("%s int value: %d"), *VarName, (*LOCALS).IntValue);
															int condition, target;
															
															FDefaultValueHelper::ParseInt(SwitchArr[l], condition);
															FDefaultValueHelper::ParseInt(SwitchArr[l+1], target);
															if ((*LOCALS).IntValue == condition)
															{
																i = target;
																UE_LOG(LogTemp, Warning, TEXT("Firing TestDialogue again in %f"), Duration);
																GetWorldTimerManager().SetTimer(UnusedHandle3, this, &ATestNPC1_Friendly::TestDialogue, Duration, false);
															}															
															
														}
														l=l+2;
													}
												}											
										}
										else
										{											
											if (End)
											{
												i = 0;
												if (!bReset)
												{
													DialogueIndex++;
												}
											}
										}
										
									}
									j++;
								}
								GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::HideFloater, Duration, false);
							}
						}
						UE_LOG(LogTemp, Warning, TEXT("Reading line: %s"), (*bar.ToString()));
					}
				}

			}
		}
	}
}
	
void ATestNPC1_Friendly::TestDialogue2() //OBSOLETE
{
	if (URadialMenu1* MyRadialMenu = Cast<URadialMenu1>(RadialMenuBlock1->GetUserWidgetObject()))
	{
		if (UFloaterWidget1* MyFloaterWidget = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
		{
			bIsInShortDialogue = true;
			if (IDName == "GENERIC")
			{
				switch (i)
				{
				case 0:
					MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("I have no time for you..."));
					duration = 4.0f;
					i = 32768;
					GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
					break;
				default:
					duration = 0;
					i = 0;
					NameFloaterBlock2->SetHiddenInGame(true);
					bIsInShortDialogue = false;
					break;
				}
			}
			else if (IDName == "WOLF_1")
			{
				switch (DialogueIndex)
				{
				case 0:
					switch (i)
					{
					case 0:
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("The Black Tree beckons..."));
						duration = 4.0f;
						DialogueIndex = 1;
						i = 32768;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					default:
						duration = 0;
						i = 0;
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						break;
					}
					break;
				case 1:
					switch (i)
					{
					case 0:
						//MyRadialMenu->RadialMenu1Reconstruct();
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("This city... is a blight."));
						duration = 3.0f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					case 1:
						DialogueIndex = 2;
						RadialMenuBlock1->SetHiddenInGame(false);
						MyRadialMenu->ButtonCountCPPMenu = 6;
						//MyRadialMenu->RadialMenu1Reconstruct();
						duration = 4.0f;
						i=32678;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					default:
						duration = 0;
						i = 0;
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						break;
					}
					break;
				case 2:
					switch (i)
					{
					case 0:
						MyRadialMenu->ButtonCountCPPMenu = 7;
						MyRadialMenu->RadialMenu1Reconstruct();
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("I must find Brother Ash."));
						duration = 1.5f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					case 1:
						UE_LOG(LogTemp, Warning, TEXT("Friedrich running"));
						UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), FVector(400, 400, 0));
						i = 32768;
						DialogueIndex = 3;
						duration = 1.5f;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					default:
						duration = 0;
						i = 0;
						UE_LOG(LogTemp, Warning, TEXT("Hiding Floater"));
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						break;
					}
					break;
				default:
					duration = 0;
					i = 0;
					NameFloaterBlock2->SetHiddenInGame(true);
					bIsInShortDialogue = false;
					break;
				}
			}
			else if (IDName == "FIDEX_1")
			{
				switch (DialogueIndex)
				{
				case 0:
					switch (i)
					{
					case 0:
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("Hello traveller..."));
						duration = 2.0f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					case 1:
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("First time at the Maw, eh? You do look quite fresh. And the mayflies are telling, of course."));
						duration = 6.5f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					case 2:
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("Come, I'll show you around."));
						DialogueIndex = 33;
						duration = 4.5f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					case 3:
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						duration = 0;
						i = 0;
						break;
					case 32768:
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						duration = 0;
						i = 0;
						break;
					default:
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						break;
					}
					break;
				case 33:
					switch (i)
					{
					case 0:
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("Follow me!"));
						duration = 2.0f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					case 1:
						MyFloaterWidget->FloaterTextBlock->SetText(FText::FromString("There is much to be seen!"));
						DialogueIndex = 34;
						duration = 2.0f;
						i++;
						GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATestNPC1_Friendly::TestDialogue, duration, false);
						break;
					default:
						NameFloaterBlock2->SetHiddenInGame(true);
						bIsInShortDialogue = false;
						duration = 0;
						i = 0;
						break;
					}
					break;
				default:
					NameFloaterBlock2->SetHiddenInGame(true);
					bIsInShortDialogue = false;
					duration = 0;
					i = 0;
					break;
				}
			}
			else
			{
				NameFloaterBlock2->SetHiddenInGame(true);
				bIsInShortDialogue = false;
				duration = 0;
				i = 0;
			}
		}
	}
}
void ATestNPC1_Friendly::TestDialogue3(FText TextIn, float Duration)
{
	if (UFloaterWidget1* MyFloaterWidget = Cast<UFloaterWidget1>(NameFloaterBlock2->GetUserWidgetObject()))
	{
		NameFloaterBlock2->SetHiddenInGame(false);
		MyFloaterWidget->FloaterTextBlock->SetText(TextIn);
		GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATestNPC1_Friendly::HideFloaterChar, Duration, false);		
	}
}
//OBSOLETE

/*			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATOPDOWN_Take1PlayerController::StopDashing, DashStop, false);*/

