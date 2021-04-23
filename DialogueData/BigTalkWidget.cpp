// Fill out your copyright notice in the Description page of Project Settings.


#include "BigTalkWidget.h"


#include "BigDialogueBody.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DialoguePicker.h"
#include "Misc/DefaultValueHelper.h"
#include "NPCLocalVariables.h"
#include "BigTalkWidget_Chapter.h"
#include "Kismet/GameplayStatics.h"
#include "TOPDOWN_Take1/UI/BigTalk_Answers_Chapter.h"
#include "TimerManager.h"
#include "TOPDOWN_Take1/TestNPC1_Friendly.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1PlayerController.h"
#include "TOPDOWN_Take1/UI/BigTalk_Answers.h"
#include "TOPDOWN_Take1/UI/BigTalk_DialoguePart.h"
#include "JournalWidget.h"

UBigTalkWidget::UBigTalkWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	i = 0;
	bInsanity = false;
	bCustomCamera = false;
	
}

void UBigTalkWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBigTalkWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBigTalkWidget::ResetVisited(ATestNPC1* MyNPC)
{
	if (MyNPC)
	{

		if (MyNPC->BigDialoguePicker)
		{
			UDataTable* DataTable = MyNPC->BigDialoguePicker;
			for (auto it : DataTable->GetRowMap())
			{
				if (FDialoguePicker* DialoguePickerRow = (FDialoguePicker*)(it.Value))
				{
					UDataTable* DataTable2 = (*DialoguePickerRow).DialogueBody;
					for (auto it2 : DataTable2->GetRowMap())
					{
						if (FBigDialogueBody* DialogueBodyRow = (FBigDialogueBody*)(it2.Value))
						{
							(*DialogueBodyRow).bVisited = false; 
						}
					}
				}
			}
		}
	}
}

void UBigTalkWidget::StartBigTalk(ATestNPC1* MyNPC)
{
	if (MyNPC)
	{
		FString DialogueContextString = FString::FromInt(MyNPC->BigDialogueIndex);
		if (FDialoguePicker* DialoguePickerRow = MyNPC->BigDialoguePicker->FindRow<FDialoguePicker>(FName(*DialogueContextString), TEXT("")))
		{
			//disable autopanning for custom dialogue sequences
			if ((*DialoguePickerRow).bCustomCamera)
			{
				bCustomCamera = true;
			}
			else bCustomCamera = false;
		}
	}
	
	
	ScrollBoxMain->ScrollToEnd();
	ScrollboxScrollDown();

	if (End)
	{
		MyNPC->CurrentResponse = "INVALID";
		MyNPC->big_i = 0;
		i = 0;
		MyNPC->BigDialogueIndex++;
		End = false;
		DialogueScrollBox->ClearChildren();
		MyNPC->bIsInDialogue = false;

		if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			MyController->LockInteraction = false;
			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
			{
				//MyPawn->DialogueCameraControl(MyPawn->GetActorLocation(), 1400, 50);
				//MyPawn->GetWorldTimerManager().SetTimer(UnusedHandle, MyPawn, &ATOPDOWN_Take1Character::CameraLock, 2.0f, false);
				MyPawn->CameraLock();
				MyController->SetViewTargetWithBlend(MyPawn, 0.5f);
				MyPawn->bCustomCamera = false; 
			}
		}
		SetVisibility(ESlateVisibility::Collapsed);
		RemoveFromViewport();
		RemoveWidgetInBlueprints(); 

		if (Chapter) // CHAPTER INTERFACE
		{
			UE_LOG(LogTemp, Warning, TEXT("Running chapter code"));
			if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				Chapter = false;
				MyController->BigTalkWidget2->AddToViewport();
				MyController->BigTalkWidget2->SetVisibility(ESlateVisibility::Visible);

				if (UBigTalkWidget_Chapter* MyTalkWidget = Cast<UBigTalkWidget_Chapter>(MyController->BigTalkWidget2))
				{
					TArray<AActor*> NPCFinder;
					UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATestNPC1_Friendly::StaticClass(), FName("CHAPTER"), NPCFinder);
					if (NPCFinder.Num() > 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("Found %d actors of tag CHAPTER"), NPCFinder.Num());
						ATestNPC1_Friendly* ChapterNPC = Cast<ATestNPC1_Friendly>(NPCFinder[0]);
						MyTalkWidget->StartBigTalk(ChapterNPC);
					}
				}
			}
		}
	}
	else
	{
	UE_LOG(LogTemp, Warning, TEXT("Casting BigDialoguePicker"));
	if (MyNPC->BigDialoguePicker)
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting DialoguePart BP"));
		if (DialoguePart_BP)
		{
			FString DialogueContextString = FString::FromInt(MyNPC->BigDialogueIndex);
			if (FDialoguePicker* DialoguePickerRow = MyNPC->BigDialoguePicker->FindRow<FDialoguePicker>(FName(*DialogueContextString), TEXT("")))
			{
								
				UE_LOG(LogTemp, Warning, TEXT("DialoguePickerRow cast"));
				if (UDataTable* DialogueBodyRow = (*DialoguePickerRow).DialogueBody)
				{
					UE_LOG(LogTemp, Warning, TEXT("DialogueBodyRow cast"));

					//convert i to string
					FString iString = FString::FromInt(MyNPC->big_i);
					UE_LOG(LogTemp, Warning, TEXT("big_i = %s"), *iString);

					if (FBigDialogueBody* foo = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*iString), TEXT("")))
					{
						//invalidate previous response
						if (FBigDialogueBody* xak_previous = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*MyNPC->CurrentResponse),""))
						{
							(*xak_previous).bVisited = true;



							
							////play player anim
							//if ((*xak_previous).IDName == "CHAR")
							//{
							//	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
							//	{
							//		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
							//		{
							//			UE_LOG(LogTemp, Warning, TEXT("PLAYING PLAYER ANIMATION"));
							//			MyPawn->PlayAnimMontage(MyPawn->TalkMontage, 1.0, FName("Talk1"));
							//		}
							//	}
							//}


							

							if ((*xak_previous).SkillRaise!="") //SkillRaise
							{
								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
								{
									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
									{
										if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*(*xak_previous).SkillRaise), TEXT("")))
										{
											LOCALS->IntValue++;
										}
									}
								}
							}

							//DIALOGUE OPTIONS - big_i modifiers, fired before printing
							
							if ((*xak_previous).DialogueAdditional.Num() > 0) //options exist, read through them:
							{
								int ArrNum = (*foo).DialogueAdditional.Num();
								int j = 0;

								while (j < ArrNum)
								{
									if (j != ArrNum) //safety check
									{
										FString read = ((*xak_previous).DialogueAdditional)[j];
										UE_LOG(LogTemp, Warning, TEXT("Read string: %s"), *read);

										if (read == "RANDOM_RESPONSE")
										{
											TArray<FString> RandArray;
											FString RandString = ((*xak_previous).DialogueAdditional)[j + 1];
											int32 min, max;

											RandString.ParseIntoArray(RandArray, TEXT(" "), true);
											FDefaultValueHelper::ParseInt(RandArray[0], min);
											FDefaultValueHelper::ParseInt(RandArray[1], max);

											if (min && max)
											{
												UE_LOG(LogTemp, Warning, TEXT("Random range: %d %d"), min, max);
												MyNPC->big_i = FMath::RandRange(min, max);
											}
											UE_LOG(LogTemp, Warning, TEXT("Setting i to: %d"), MyNPC->big_i);
										}

										else if (read == "FLAG" && j+1<(*xak_previous).DialogueAdditional.Num())
										{
											UE_LOG(LogTemp, Warning, TEXT("Initializing flag"));
											TArray<FString> IndexArray;
											FString IndexString = ((*xak_previous).DialogueAdditional)[j + 1];

											IndexString.ParseIntoArray(IndexArray, TEXT(" "), true);
											int TrueIndex = FCString::Atoi(*IndexArray[1]);
											int FalseIndex = FCString::Atoi(*IndexArray[2]);
											FString VarName = IndexArray[0]; 

											if (FNPCLocalVariables* LOCALS = MyNPC->NPCLocals->FindRow<FNPCLocalVariables>(FName(*VarName), TEXT("")))
											{
												if ((*LOCALS).Flag)
												{
													MyNPC->big_i = TrueIndex;
												}
												else
												{
													MyNPC->big_i = FalseIndex; 
												}
											}
											
										}

										else if (read == "FLAG_CHAR" && j + 1 < (*xak_previous).DialogueAdditional.Num())
										{
											if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
											{
												if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
												{
													UE_LOG(LogTemp, Warning, TEXT("Initializing flag"));
													TArray<FString> IndexArray;
													FString IndexString = ((*xak_previous).DialogueAdditional)[j + 1];

													IndexString.ParseIntoArray(IndexArray, TEXT(" "), true);
													int TrueIndex = FCString::Atoi(*IndexArray[1]);
													int FalseIndex = FCString::Atoi(*IndexArray[2]);
													FString VarName = IndexArray[0];

													if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*VarName), TEXT("")))
													{
														if ((*LOCALS).Flag)
														{
															MyNPC->big_i = TrueIndex;
														}
														else
														{
															MyNPC->big_i = FalseIndex;
														}
													}
												}
											}

										}
										
										else if (read == "SWITCH_I")
										{
											UE_LOG(LogTemp, Warning, TEXT("Initializing switch"));
											FString SwitchString = ((*xak_previous).DialogueAdditional)[j + 1];
											TArray<FString> SwitchArr;
											SwitchString.ParseIntoArray(SwitchArr, TEXT(" "), true);
											if (SwitchArr.Num() > 2)
											{
												FString VarName = SwitchArr[0];
												int CaseCount = (SwitchArr.Num() - 1) / 2;
												UE_LOG(LogTemp, Warning, TEXT("VarName: %s, cases: %d"), *VarName, CaseCount);
												int l = 1;

												while (l < SwitchArr.Num())
												{
													UE_LOG(LogTemp, Warning, TEXT("Processing cases"));
													if (FNPCLocalVariables* LOCALS = MyNPC->NPCLocals->FindRow<FNPCLocalVariables>(FName(*VarName), TEXT("")))
													{
														UE_LOG(LogTemp, Warning, TEXT("Executing cases"));
														UE_LOG(LogTemp, Warning, TEXT("%s int value: %d"), *VarName, (*LOCALS).IntValue);
														int condition, target;

														FDefaultValueHelper::ParseInt(SwitchArr[l], condition);
														FDefaultValueHelper::ParseInt(SwitchArr[l + 1], target);
														if ((*LOCALS).IntValue == condition)
														{
															MyNPC->big_i = target;
														}

													}
													l = l + 2;
												}
											}
										}
										
									}
									j++;
								}
							}
						}
						
						
						
						UE_LOG(LogTemp, Warning, TEXT("foo cast"));

						//saving cast to a global variable
						CurrentNPC = MyNPC;

						//NPC IsInDialogue
						MyNPC->bIsInDialogue = true;
						if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
						{
							MyController->LockInteraction = true;
						}

						if (!(*foo).bCameraPan && !bCustomCamera)
						{
							if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
							{
								if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
								{
									MyPawn->DialogueCameraControl(MyNPC->GetActorLocation(), 1100 + 5*MyNPC->NPCSize, 500, MyNPC->NPCSize);
								}
							}
						}
						
						//Dialogue Text
						FText bar = (*foo).DialogueText;

						float Duration = (*foo).Duration;

						//end dialogue marker
						End = (*foo).bEnd;

						//index
						if (bInsanity)
						{
							UE_LOG(LogTemp, Warning, TEXT("INSANITY = TRUE, setting big_i to the insanity option"));
							if ((*foo).NextIndexInsanity != 0) MyNPC->big_i = (*foo).NextIndexInsanity; else MyNPC->big_i++;
							bInsanity = false; 
						}
						else
						{
							if ((*foo).NextIndex != 0) MyNPC->big_i = (*foo).NextIndex; else MyNPC->big_i++;
						}

						//set button text

						if (End)
						{
							ButtonContinue->SetVisibility(ESlateVisibility::Visible);
							ButtonContinueText->SetText(FText::FromString("End"));
						}
						else if (!(*foo).bAnswers)
						{
							ButtonContinue->SetVisibility(ESlateVisibility::Visible);
							ButtonContinueText->SetText(FText::FromString("Continue..."));
						}
						else
						{
							ButtonContinue->SetVisibility(ESlateVisibility::Collapsed);
						}

						//ZYXYZ camera
						if((*foo).IDName == "ZYXYZ")
						{
							if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
							{
								if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
								{
									MyPawn->DialogueCameraControl(MyController->Sidekick->GetActorLocation(), 1200, 500, 50);
								}
							}
						}

						if((*foo).IDName != MyNPC->IDName)
						{
							if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
							{
								if(ATestNPC1* MyCameraNPC = MyController->NPCManager->GetByName((*foo).IDName))
								{
									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
									{
										MyPawn->DialogueCameraControl(MyCameraNPC->GetActorLocation(), 1100 + 5 * MyNPC->NPCSize, 500, MyNPC->NPCSize);
									}
								}
							}
						}
						
						
						//play sound
						if ((*foo).SFX)
						{
							if ((*foo).IDName == "CHAR")
							{
								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
								{
									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
									{
										MyPawn->AudioComp->SetSound((*foo).SFX);
										MyPawn->AudioComp->Play();
									}
								}
							}
							else if ((*foo).IDName == "ZYXYZ")
							{
								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
								{
									if (ATOPDOWN_Take1Character* MySidekick = Cast<ATOPDOWN_Take1Character>(MyController->Sidekick))
									{
										MySidekick->AudioComp->SetSound((*foo).SFX);
										MySidekick->AudioComp->Play();
									}
								}
							}
							else
							{
								MyNPC->AudioComp->SetSound((*foo).SFX);
								MyNPC->AudioComp->Play();
							}
						}

						
							UE_LOG(LogTemp, Warning, TEXT("Creating a DialoguePart widget..."));

							FString CurrentWidgetName = "Part" + FString::FromInt(i);
							UUserWidget* CurrentWidget = CreateWidget<UBigTalk_DialoguePart>(GetWorld()->GetFirstPlayerController(), DialoguePart_BP, FName(*CurrentWidgetName));
							/*ScrollboxContents.Add(CurrentWidget);*/

						
							if (CurrentWidget)
							{

								UE_LOG(LogTemp, Warning, TEXT("CurrentWidget succesfully created"));

								

								if (UBigTalk_DialoguePart* MyCurrentWidget = Cast<UBigTalk_DialoguePart>(CurrentWidget))
								{
									if((*foo).IDName=="NARRATOR")
									{
										PortraitImage->SetVisibility(ESlateVisibility::Hidden);
										MyCurrentWidget->DialogueChar->SetVisibility(ESlateVisibility::Collapsed);
										MyCurrentWidget->DialogueText->SetText(bar);
										i++;
									}
									else if((*foo).IDName == "ZYXYZ")
									{
										MyCurrentWidget->DialogueChar->SetText(FText::FromString("<WhiteB>Zyxyz:</>"));
										MyCurrentWidget->DialogueText->SetText(bar);
										i++; 
									}
									else
									{
										PortraitImage->SetBrushFromTexture(MyNPC->PortraitImage);
										PortraitImage->SetVisibility(ESlateVisibility::Visible);
										UE_LOG(LogTemp, Verbose, TEXT("Setting up the widget"));
										FString CharNameStyled;

										if ((*foo).DisplayName != "")
										{											
											CharNameStyled = "<WhiteB>" + (*foo).DisplayName + ":" + "</>";
										}
										else
										{
											CharNameStyled = "<WhiteB>" + MyNPC->CharName + ":" + "</>";
										}
										MyCurrentWidget->DialogueChar->SetText(FText::FromString(CharNameStyled));
										MyCurrentWidget->DialogueText->SetText(bar);										
										i++;
									}
									MyCurrentWidget->ForceLayoutPrepass();

								}

								DialogueScrollBox->AddChildToVerticalBox(CurrentWidget); //add to scrollbox
								ScrollboxScrollToWidget(CurrentWidget);
								ScrollboxScrollDown();
							}
							UE_LOG(LogTemp, Warning, TEXT("Scroll offset: %f"), ScrollBoxMain->GetScrollOffsetOfEnd());
							ScrollBoxMain->SetScrollOffset(ScrollBoxMain->GetScrollOffset() + 200);

							//write out answers
							if ((*foo).bAnswers)
							{
								
									if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
									{
										if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
										{
											if (!bCustomCamera)
											{
												MyPawn->DialogueCameraControl(MyPawn->GetActorLocation(), 1100, 500, 1);
											}
										}
									}

								
								if ((*foo).AnswersIndexArr.Num() > 0)
								{
									for (int k = 0; k < (*foo).AnswersIndexArr.Num(); k++) //iterate through all indexes
									{

										FString kString = FString::FromInt((*foo).AnswersIndexArr[k]);
										UE_LOG(LogTemp, Warning, TEXT("kString = %s"), *kString);

										if (FBigDialogueBody* xak = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*kString), TEXT("")))
										{
											bool FlagCheck = true;  //init flag check

											if((*xak).FlagName != "")
											{
												if (FNPCLocalVariables* LOCALS = MyNPC->NPCLocals->FindRow<FNPCLocalVariables>(FName(*(*xak).FlagName), TEXT("")))
												{
													if (LOCALS->Flag)
													{
														FlagCheck = true;
													}
													else FlagCheck = false; 
												}
											}
											
											if (!(*xak).bInactive && FlagCheck)
											{
												if ((*xak).SkillCheck != "") //skillcheck
												{
													if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
													{
														if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
														{
															if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*(*xak).SkillCheck), TEXT("")))
															{
																if (LOCALS->IntValue >= (*xak).SkillCheckScore) //main check
																{
																	FString CurrentResponseName = "Response" + FString::FromInt(k);
																	UUserWidget* CurrentResponse = CreateWidget<UBigTalk_Answers>(GetWorld()->GetFirstPlayerController(), DialogueAnswers_BP, FName(*CurrentResponseName));

																	DialogueScrollBox->ForceLayoutPrepass(); 
																	DialogueScrollBox->AddChildToVerticalBox(CurrentResponse);

																	if (UBigTalk_Answers* MyCurrentResponse = Cast<UBigTalk_Answers>(CurrentResponse))
																	{
																		MyCurrentResponse->CurrentNPC = MyNPC;
																		MyCurrentResponse->ButtonResponse = (*xak).NextIndex;
																		MyCurrentResponse->ClickedResponse = kString;
																		UE_LOG(LogTemp, Warning, TEXT("End = %d"), (*xak).bEnd);
																		MyCurrentResponse->EndButton = (*xak).bEnd;
																		MyCurrentResponse->CurrentResponseCount = (*foo).AnswersIndexArr.Num();
																		MyCurrentResponse->CurrentIndex = k + 1; 
																		FString nString = FString::FromInt(k+1) + ". ";
																		MyCurrentResponse->NumberBlock->SetText(FText::FromString(nString));
																		MyCurrentResponse->AnswerTextBlock->SetText((*xak).DialogueText);
																		MyCurrentResponse->AnswerTextBlock->ForceLayoutPrepass();


																		FLinearColor CurrentColor = FLinearColor(0.666667, 0, 0, 1.0f);
																		if ((*xak).SkillCheck == "SECRETS")
																		{
																			CurrentColor = FLinearColor(0.372f, 0.054f, 0.505f, 1.0f);
																		}
																		else if ((*xak).SkillCheck == "ARCHIVIST")
																		{
																			CurrentColor = FLinearColor(0.886, 0.427, 0.094, 1.0f);
																		}
																		else if ((*xak).SkillCheck == "DECEIVER")
																		{
																			CurrentColor = FLinearColor(0.580, 0.098, 0.250, 1.0f);
																		}
																		else if ((*xak).SkillCheck == "HUNTER")
																		{
																			CurrentColor = FLinearColor(0.094, 0.874, 0.886, 1.0f);
																		}
																		else if ((*xak).SkillCheck == "ICONOCLAST")
																		{
																			CurrentColor = FLinearColor(0.941, 0.807, 0.039, 1.0f);
																		}

																		MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(CurrentColor);
																		MyCurrentResponse->UnhoveredColorCPP = CurrentColor;

																		if ((*xak).bVisited)
																		{
																			UE_LOG(LogTemp, Warning, TEXT("bVisited = true, setting color"), *kString);

																			MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(FLinearColor(CurrentColor.R, CurrentColor.G, CurrentColor.B, 0.5f));
																			MyCurrentResponse->UnhoveredColorCPP = FLinearColor(CurrentColor.R, CurrentColor.G, CurrentColor.B, 0.5f);
																		}
																		ScrollboxScrollToWidget(CurrentResponse);
																		ScrollboxScrollDown();
																	}
																}

															}
														}
													}

												}
												else { //noskillcheck

													FString CurrentResponseName = "Response" + FString::FromInt(k);
													UUserWidget* CurrentResponse = CreateWidget<UBigTalk_Answers>(GetWorld()->GetFirstPlayerController(), DialogueAnswers_BP, FName(*CurrentResponseName));

													DialogueScrollBox->ForceLayoutPrepass(); 
													DialogueScrollBox->AddChildToVerticalBox(CurrentResponse);
													UE_LOG(LogTemp, Warning, TEXT("Adding response: %s"), *CurrentResponse->GetName());
																										
													if (UBigTalk_Answers* MyCurrentResponse = Cast<UBigTalk_Answers>(CurrentResponse))
													{
														MyCurrentResponse->CurrentNPC = MyNPC;
														MyCurrentResponse->ButtonResponse = (*xak).NextIndex;
														MyCurrentResponse->ClickedResponse = kString;
														UE_LOG(LogTemp, Warning, TEXT("End = %d"), (*xak).bEnd);
														MyCurrentResponse->EndButton = (*xak).bEnd;
														MyCurrentResponse->CurrentResponseCount = (*foo).AnswersIndexArr.Num();
														MyCurrentResponse->CurrentIndex = k + 1;
														FString nString = FString::FromInt(k + 1) + ". ";
														MyCurrentResponse->NumberBlock->SetText(FText::FromString(nString));
														MyCurrentResponse->AnswerTextBlock->SetText((*xak).DialogueText);

														//set sound
														if((*xak).SFX)
														{
															MyCurrentResponse->SFX = (*xak).SFX; 
														}

														FString AnswerString = (*xak).DialogueText.ToString(); 
														UE_LOG(LogTemp, Warning, TEXT("Setting answer text to %s"), *AnswerString);
														
														MyCurrentResponse->AnswerTextBlock->ForceLayoutPrepass();
														MyCurrentResponse->ForceLayoutPrepass(); 
														if ((*xak).bVisited)
														{
															UE_LOG(LogTemp, Warning, TEXT("bVisited = true, setting color"), *kString);
															MyCurrentResponse->NumberBlock->SetColorAndOpacity(FLinearColor(0.3f, 0, 0, 1.0f));
															MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(FLinearColor(0.3f, 0, 0, 1.0f));
															MyCurrentResponse->UnhoveredColorCPP = FLinearColor(0.3f, 0, 0, 1.0f);
														}
													}
												}
												/*ScrollboxScrollToWidget(CurrentResponse);
												ScrollboxScrollDown();*/
											}
										}
									}
								}
							}
							//end writing out answers

							
							ScrollBoxMain->ForceLayoutPrepass(); 
							ScrollBoxMain->ScrollToEnd();
							ScrollboxScrollDown();
						
							if ((*foo).DialogueAdditional.Num() > 0) //DIALOGUE OPTIONS 2 - post printing
							{
								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
								{
									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
									{
										int ArrNum = (*foo).DialogueAdditional.Num();
										int j = 0;

										while (j < ArrNum)
										{
											if (j != ArrNum) //safety check
											{
												FString read = ((*foo).DialogueAdditional)[j];
												UE_LOG(LogTemp, Warning, TEXT("Read string: %s"), *read);

												if (read == "DLG_INDEX_RESET")
												{
													UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to 0"));
													MyNPC->BigDialogueIndex = -1;
													MyNPC->big_i = 0;
												}

												else if (read == "DLG_INDEX_NEXT")
												{
													FString IndexString = ((*foo).DialogueAdditional)[j + 1];

													FDefaultValueHelper::ParseInt(IndexString, MyNPC->BigDialogueIndex);
													UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to %d"), MyNPC->BigDialogueIndex);
												}

												else if (read == "ACTIVATE_QUEST" && (j + 1) < ArrNum)
												{
													FString QuestString = ((*foo).DialogueAdditional)[j + 1];

													ActivateQuest(QuestString);


													UE_LOG(LogTemp, Warning, TEXT("Activating quest %s"), *QuestString);
												}

												else if (read == "INFLUENCE" && (j + 1) < ArrNum)
												{
													FString InfluenceString = ((*foo).DialogueAdditional)[j + 1];
													TArray<FString> InfluenceArray;
													InfluenceString.ParseIntoArray(InfluenceArray, TEXT(" "), true);
													//INFLUENCE_RAISE
													//[name] [amount]



													if (InfluenceArray.Num() == 2)
													{

														if (InfluenceGained_BP)
														{
															if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*(InfluenceArray[0])), TEXT("")))
															{
																int InfluenceChange = FCString::Atoi(*InfluenceArray[1]);

																if (InfluenceChange != 0)
																{
																	LOCALS->IntValue = LOCALS->IntValue + InfluenceChange;
																	if (InfluenceChange > 0)
																	{
																		UE_LOG(LogTemp, Warning, TEXT("Influence gained: %s, %d"), *(InfluenceArray[0]), InfluenceChange);

																		if (UUserWidget* InfluenceWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InfluenceGained_BP, TEXT("InfluenceGained")))
																		{
																			DialogueScrollBox->AddChildToVerticalBox(InfluenceWidget);
																		}
																	}
																	else
																	{
																		UE_LOG(LogTemp, Warning, TEXT("Influence lost: %s, %d"), *(InfluenceArray[0]), InfluenceChange);
																		if (UUserWidget* InfluenceWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InfluenceLost_BP, TEXT("InfluenceLost")))
																		{
																			DialogueScrollBox->AddChildToVerticalBox(InfluenceWidget);
																		}

																	}
																}
															}
														}
													}





												}

												else if (read == "UPDATE_QUEST" && (j + 1) < ArrNum)
												{
													//FString QuestString = ((*foo).DialogueAdditional)[j + 1];

													TArray<FString> QuestUpdateArray;
													FString QuestUpdateString = ((*foo).DialogueAdditional)[j + 1];
													QuestUpdateString.ParseIntoArray(QuestUpdateArray, TEXT(" "), true);


													UpdateQuest(QuestUpdateArray[0], QuestUpdateArray[1]);


													UE_LOG(LogTemp, Warning, TEXT("Updating quest %s to status %s"), *(QuestUpdateArray[0]), *(QuestUpdateArray[1]));
												}

												else if (read == "MOVE" && (j + 1) < ArrNum)
												{
													TArray<FString> VectorArray;
													FString VectorString = ((*foo).DialogueAdditional)[j + 1];

													VectorString.ParseIntoArray(VectorArray, TEXT(" "), true);
													FVector Move = FVector(FCString::Atof(*VectorArray[0]), FCString::Atof(*VectorArray[1]), FCString::Atof(*VectorArray[2]));
													UAIBlueprintHelperLibrary::SimpleMoveToLocation(MyNPC->GetController(), Move);

												}
												else if (read == "DEACTIVATE" && (j + 1) < ArrNum)
												{
													FString IndexString = ((*foo).DialogueAdditional)[j + 1];
													//int Index = FCString::Atoi(*IndexString);
													if (FBigDialogueBody* pff = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*IndexString), TEXT("")))
													{
														(*pff).bInactive = true;
													}
												}

												else if (read == "FLAG2" && (j + 1) < ArrNum)
												{
													UE_LOG(LogTemp, Warning, TEXT("Initializing flag"));
													TArray<FString> IndexArray;
													FString IndexString = (*foo).DialogueAdditional[j + 1];

													IndexString.ParseIntoArray(IndexArray, TEXT(" "), true);
													int TrueIndex = FCString::Atoi(*IndexArray[1]);
													int FalseIndex = FCString::Atoi(*IndexArray[2]);
													FString VarName = IndexArray[0];

													if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*VarName), TEXT("")))
													{
														if ((*LOCALS).Flag)
														{
															MyNPC->big_i = TrueIndex;
														}
														else
														{
															MyNPC->big_i = FalseIndex;
														}
													}

												}

												else if (read == "TEST") { UE_LOG(LogTemp, Warning, TEXT("DialogueAdditionals - TEST")); }
												else if (read == "REDFLASH") { RedFlash(); }
												else if (read == "BLACKBGON") { BlackBGOn(); }
												else if (read == "BLACKBGOFF") { BlackBGOff(); }
												else if (read == "BLURBGON") { BlurBGOn(); }
												else if (read == "BLURBGOFF") { BlurBGOff(); }
												else if (read == "BLACKINSTANT") { BlackBGInstant(); }
												else if (read == "CHAPTER") { Chapter = true; UE_LOG(LogTemp, Warning, TEXT("Chapter = true")); }

												j++;
											}
										}
									}
								}
							}

						if((*foo).Effect)
						{
							if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
							{
								if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
								{
									UCustomGameplayEffect* EffectObject = NewObject<UCustomGameplayEffect>(this, (*foo).Effect);
									EffectObject->ExecuteGameplayEffect(MyPawn);
								}
							}
						}

						
						//UE_LOG(LogTemp, Warning, TEXT("Creating widget 2"));
						//UUserWidget* CurrentWidget2 = CreateWidget<UBigTalk_DialoguePart>(GetWorld()->GetFirstPlayerController(), DialoguePart_BP, TEXT("CurrentWidgetName2"));
						///*ScrollboxContents.Add(CurrentWidget);*/

						//if (CurrentWidget2)
						//{

						//	UE_LOG(LogTemp, Warning, TEXT("CurrentWidget2 succesfully created"));

						//	DialogueScrollBox->AddChildToVerticalBox(CurrentWidget2);

						//	if (UBigTalk_DialoguePart* MyCurrentWidget2 = Cast<UBigTalk_DialoguePart>(CurrentWidget2))
						//	{
						//		UE_LOG(LogTemp, Warning, TEXT("Setting up widget 2"));
						//		FString CharNameStyled = "<WhiteB>" + MyNPC->CharName + ":" + "</>";
						//		MyCurrentWidget2->DialogueChar->SetText(FText::FromString(CharNameStyled));
						//		MyCurrentWidget2->DialogueText->SetText(FText::FromString("Testing testing 2 2 2 2 "));
						//	}
						//}
						//
						}
					}
				}
			}
		}
	}
}

void UBigTalkWidget::ActivateQuest(FString QuestID)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		{
			if(UJournalWidget* MyJournalWidget = Cast<UJournalWidget>(MyController->JournalWidget))
			{
				MyJournalWidget->ActivateQuest(MyPawn->QuestLog, QuestID);
				if (JournalNotify_BP)
				{
					UUserWidget* MyNotify = CreateWidget<UUserWidget>(this, JournalNotify_BP, TEXT("JournalUpdated"));
					DialogueScrollBox->AddChildToVerticalBox(MyNotify); 
				}
			}
		}
	}
}

void UBigTalkWidget::UpdateQuest(FString QuestID, FString UpdatedStatus)
{
	if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
		{
			if (UJournalWidget* MyJournalWidget = Cast<UJournalWidget>(MyController->JournalWidget))
			{
				MyJournalWidget->UpdateQuest(MyPawn->QuestLog, QuestID, UpdatedStatus);
				if (JournalNotify_BP)
				{
					UUserWidget* MyNotify = CreateWidget<UUserWidget>(this, JournalNotify_BP, TEXT("JournalUpdated"));
					DialogueScrollBox->AddChildToVerticalBox(MyNotify);
				}
			}
		}
	}
}
//
//void UBigTalkWidget::StartBigTalk(ATestNPC1* MyNPC)
//{
//
//	ScrollBoxMain->ScrollToEnd();
//	ScrollboxScrollDown();
//
//	if (End)
//	{
//		MyNPC->CurrentResponse = "INVALID";
//		MyNPC->big_i = 0;
//		i = 0;
//		MyNPC->BigDialogueIndex++;
//		End = false;
//		DialogueScrollBox->ClearChildren();
//		MyNPC->bIsInDialogue = false;
//
//		if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//		{
//			MyController->LockInteraction = false;
//			if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
//			{
//				//MyPawn->DialogueCameraControl(MyPawn->GetActorLocation(), 1400, 50);
//				//MyPawn->GetWorldTimerManager().SetTimer(UnusedHandle, MyPawn, &ATOPDOWN_Take1Character::CameraLock, 2.0f, false);
//				MyPawn->CameraLock();
//			}
//		}
//		SetVisibility(ESlateVisibility::Collapsed);
//		RemoveFromViewport();
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Casting BigDialoguePicker"));
//		if (MyNPC->BigDialoguePicker)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Casting DialoguePart BP"));
//			if (DialoguePart_BP)
//			{
//				FString DialogueContextString = FString::FromInt(MyNPC->BigDialogueIndex);
//				if (FDialoguePicker* DialoguePickerRow = MyNPC->BigDialoguePicker->FindRow<FDialoguePicker>(FName(*DialogueContextString), TEXT("")))
//				{
//					UE_LOG(LogTemp, Warning, TEXT("DialoguePickerRow cast"));
//					if (UDataTable* DialogueBodyRow = (*DialoguePickerRow).DialogueBody)
//					{
//						UE_LOG(LogTemp, Warning, TEXT("DialogueBodyRow cast"));
//
//						//convert i to string
//						FString iString = FString::FromInt(MyNPC->big_i);
//						UE_LOG(LogTemp, Warning, TEXT("big_i = %s"), *iString);
//
//						if (FBigDialogueBody* foo = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*iString), TEXT("")))
//						{
//							//invalidate previous response
//							if (FBigDialogueBody* xak_previous = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*MyNPC->CurrentResponse), ""))
//							{
//								(*xak_previous).bVisited = true;
//
//								if ((*xak_previous).SkillRaise != "") //SkillRaise
//								{
//									if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//									{
//										if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
//										{
//											if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*(*xak_previous).SkillRaise), TEXT("")))
//											{
//												LOCALS->IntValue++;
//											}
//										}
//									}
//								}
//
//								//DIALOGUE OPTIONS - big_i modifiers, fired before printing
//
//								if ((*xak_previous).DialogueAdditional.Num() > 0) //options exist, read through them:
//								{
//									int ArrNum = (*foo).DialogueAdditional.Num();
//									int j = 0;
//
//									while (j < ArrNum)
//									{
//										if (j != ArrNum) //safety check
//										{
//											FString read = ((*xak_previous).DialogueAdditional)[j];
//											UE_LOG(LogTemp, Warning, TEXT("Read string: %s"), *read);
//
//											if (read == "RANDOM_RESPONSE")
//											{
//												TArray<FString> RandArray;
//												FString RandString = ((*xak_previous).DialogueAdditional)[j + 1];
//												int32 min, max;
//
//												RandString.ParseIntoArray(RandArray, TEXT(" "), true);
//												FDefaultValueHelper::ParseInt(RandArray[0], min);
//												FDefaultValueHelper::ParseInt(RandArray[1], max);
//
//												if (min && max)
//												{
//													UE_LOG(LogTemp, Warning, TEXT("Random range: %d %d"), min, max);
//													MyNPC->big_i = FMath::RandRange(min, max);
//												}
//												UE_LOG(LogTemp, Warning, TEXT("Setting i to: %d"), MyNPC->big_i);
//											}
//
//											else if (read == "SWITCH_I")
//											{
//												UE_LOG(LogTemp, Warning, TEXT("Initializing switch"));
//												FString SwitchString = ((*xak_previous).DialogueAdditional)[j + 1];
//												TArray<FString> SwitchArr;
//												SwitchString.ParseIntoArray(SwitchArr, TEXT(" "), true);
//												if (SwitchArr.Num() > 2)
//												{
//													FString VarName = SwitchArr[0];
//													int CaseCount = (SwitchArr.Num() - 1) / 2;
//													UE_LOG(LogTemp, Warning, TEXT("VarName: %s, cases: %d"), *VarName, CaseCount);
//													int l = 1;
//
//													while (l < SwitchArr.Num())
//													{
//														UE_LOG(LogTemp, Warning, TEXT("Processing cases"));
//														if (FNPCLocalVariables* LOCALS = MyNPC->NPCLocals->FindRow<FNPCLocalVariables>(FName(*VarName), TEXT("")))
//														{
//															UE_LOG(LogTemp, Warning, TEXT("Executing cases"));
//															UE_LOG(LogTemp, Warning, TEXT("%s int value: %d"), *VarName, (*LOCALS).IntValue);
//															int condition, target;
//
//															FDefaultValueHelper::ParseInt(SwitchArr[l], condition);
//															FDefaultValueHelper::ParseInt(SwitchArr[l + 1], target);
//															if ((*LOCALS).IntValue == condition)
//															{
//																MyNPC->big_i = target;
//															}
//
//														}
//														l = l + 2;
//													}
//												}
//											}
//
//										}
//										j++;
//									}
//								}
//							}
//
//
//
//							UE_LOG(LogTemp, Warning, TEXT("foo cast"));
//
//							//saving cast to a global variable
//							CurrentNPC = MyNPC;
//
//							//NPC IsInDialogue
//							MyNPC->bIsInDialogue = true;
//							if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//							{
//								MyController->LockInteraction = true;
//							}
//
//							if (!(*foo).bCameraPan)
//							{
//								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//								{
//									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
//									{
//										MyPawn->DialogueCameraControl(MyNPC->GetActorLocation(), 1100 + 5 * MyNPC->NPCSize, 500, MyNPC->NPCSize);
//									}
//								}
//							}
//
//							//Dialogue Text
//							FText bar = (*foo).DialogueText;
//
//							float Duration = (*foo).Duration;
//
//							//end dialogue marker
//							End = (*foo).bEnd;
//
//							//index
//							if ((*foo).NextIndex != 0) MyNPC->big_i = (*foo).NextIndex; else MyNPC->big_i++;
//
//							//set text
//
//							if (End)
//							{
//								ButtonContinue->SetVisibility(ESlateVisibility::Visible);
//								ButtonContinueText->SetText(FText::FromString("End"));
//							}
//							else if (!(*foo).bAnswers)
//							{
//								ButtonContinue->SetVisibility(ESlateVisibility::Visible);
//								ButtonContinueText->SetText(FText::FromString("Continue..."));
//							}
//							else
//							{
//								ButtonContinue->SetVisibility(ESlateVisibility::Collapsed);
//							}
//
//							//play sound
//							if ((*foo).SFX)
//							{
//								if ((*foo).IDName == "CHAR")
//								{
//									if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//									{
//										if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
//										{
//											MyPawn->AudioComp->SetSound((*foo).SFX);
//											MyPawn->AudioComp->Play();
//										}
//									}
//								}
//								else
//								{
//									MyNPC->AudioComp->SetSound((*foo).SFX);
//									MyNPC->AudioComp->Play();
//								}
//							}
//
//
//							UE_LOG(LogTemp, Warning, TEXT("Creating a DialoguePart widget..."));
//
//							FString CurrentWidgetName = "Part" + FString::FromInt(i);
//							UUserWidget* CurrentWidget = CreateWidget<UBigTalk_DialoguePart>(GetWorld()->GetFirstPlayerController(), DialoguePart_BP, FName(*CurrentWidgetName));
//							/*ScrollboxContents.Add(CurrentWidget);*/
//
//
//							if (CurrentWidget)
//							{
//
//								UE_LOG(LogTemp, Warning, TEXT("CurrentWidget succesfully created"));
//
//
//
//								if (UBigTalk_DialoguePart* MyCurrentWidget = Cast<UBigTalk_DialoguePart>(CurrentWidget))
//								{
//									if ((*foo).IDName == "NARRATOR")
//									{
//										PortraitImage->SetVisibility(ESlateVisibility::Hidden);
//										MyCurrentWidget->DialogueChar->SetVisibility(ESlateVisibility::Collapsed);
//										MyCurrentWidget->DialogueText->SetText(bar);
//										i++;
//									}
//									else
//									{
//										PortraitImage->SetBrushFromTexture(MyNPC->PortraitImage);
//										PortraitImage->SetVisibility(ESlateVisibility::Visible);
//										UE_LOG(LogTemp, Verbose, TEXT("Setting up the widget"));
//										FString CharNameStyled;
//
//										if ((*foo).DisplayName != "")
//										{
//											CharNameStyled = "<WhiteB>" + (*foo).DisplayName + ":" + "</>";
//										}
//										else
//										{
//											CharNameStyled = "<WhiteB>" + MyNPC->CharName + ":" + "</>";
//										}
//										MyCurrentWidget->DialogueChar->SetText(FText::FromString(CharNameStyled));
//										MyCurrentWidget->DialogueText->SetText(bar);
//										MyCurrentWidget->DialogueText->ForceLayoutPrepass();
//										MyCurrentWidget->ForceLayoutPrepass();
//										i++;
//									}
//
//								}
//
//								DialogueScrollBox->AddChildToVerticalBox(CurrentWidget); //add to scrollbox
//								ScrollboxScrollToWidget(CurrentWidget);
//								ScrollboxScrollDown();
//							}
//							UE_LOG(LogTemp, Warning, TEXT("Scroll offset: %f"), ScrollBoxMain->GetScrollOffsetOfEnd());
//							ScrollBoxMain->SetScrollOffset(ScrollBoxMain->GetScrollOffset() + 200);
//
//							//write out answers
//							if ((*foo).bAnswers)
//							{
//								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//								{
//									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
//									{
//										MyPawn->DialogueCameraControl(MyPawn->GetActorLocation(), 1100, 500, 1);
//									}
//								}
//								if ((*foo).AnswersIndexArr.Num() > 0)
//								{
//									for (int k = 0; k < (*foo).AnswersIndexArr.Num(); k++)
//									{
//
//										FString kString = FString::FromInt((*foo).AnswersIndexArr[k]);
//										UE_LOG(LogTemp, Warning, TEXT("kString = %s"), *kString);
//
//										if (FBigDialogueBody* xak = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*kString), TEXT("")))
//										{
//
//											if ((*xak).SkillCheck != "") //skillcheck
//											{
//												if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
//												{
//													if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
//													{
//														if (FNPCLocalVariables* LOCALS = MyPawn->NPCLocals->FindRow<FNPCLocalVariables>(FName(*(*xak).SkillCheck), TEXT("")))
//														{
//															if (LOCALS->IntValue >= (*xak).SkillCheckScore) //main check
//															{
//																FString CurrentResponseName = "Response" + FString::FromInt(k);
//																UUserWidget* CurrentResponse = CreateWidget<UBigTalk_Answers>(GetWorld()->GetFirstPlayerController(), DialogueAnswers_BP, FName(*CurrentResponseName));
//
//																DialogueScrollBox->AddChildToVerticalBox(CurrentResponse);
//
//																if (UBigTalk_Answers* MyCurrentResponse = Cast<UBigTalk_Answers>(CurrentResponse))
//																{
//																	MyCurrentResponse->CurrentNPC = MyNPC;
//																	MyCurrentResponse->ButtonResponse = (*xak).NextIndex;
//																	MyCurrentResponse->ClickedResponse = kString;
//																	UE_LOG(LogTemp, Warning, TEXT("End = %d"), (*xak).bEnd);
//																	MyCurrentResponse->EndButton = (*xak).bEnd;
//																	MyCurrentResponse->CurrentResponseCount = (*foo).AnswersIndexArr.Num();
//																	MyCurrentResponse->AnswerTextBlock->SetText((*xak).DialogueText);
//																	MyCurrentResponse->AnswerTextBlock->ForceLayoutPrepass();
//
//
//																	FLinearColor CurrentColor = FLinearColor(0.666667, 0, 0, 1.0f);
//																	if ((*xak).SkillCheck == "SECRETS")
//																	{
//																		CurrentColor = FLinearColor(0.372f, 0.054f, 0.505f, 1.0f);
//																	}
//																	else if ((*xak).SkillCheck == "ARCHIVIST")
//																	{
//																		CurrentColor = FLinearColor(0.886, 0.427, 0.094, 1.0f);
//																	}
//																	else if ((*xak).SkillCheck == "DECEIVER")
//																	{
//																		CurrentColor = FLinearColor(0.580, 0.098, 0.250, 1.0f);
//																	}
//																	else if ((*xak).SkillCheck == "HUNTER")
//																	{
//																		CurrentColor = FLinearColor(0.094, 0.874, 0.886, 1.0f);
//																	}
//																	else if ((*xak).SkillCheck == "ICONOCLAST")
//																	{
//																		CurrentColor = FLinearColor(0.941, 0.807, 0.039, 1.0f);
//																	}
//
//																	MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(CurrentColor);
//																	MyCurrentResponse->UnhoveredColorCPP = CurrentColor;
//
//																	if ((*xak).bVisited)
//																	{
//																		UE_LOG(LogTemp, Warning, TEXT("bVisited = true, setting color"), *kString);
//
//																		MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(FLinearColor(CurrentColor.R, CurrentColor.G, CurrentColor.B, 0.5f));
//																		MyCurrentResponse->UnhoveredColorCPP = FLinearColor(CurrentColor.R, CurrentColor.G, CurrentColor.B, 0.5f);
//																	}
//																	ScrollboxScrollToWidget(CurrentResponse);
//																	ScrollboxScrollDown();
//																}
//															}
//
//														}
//													}
//												}
//
//											}
//											else { //noskillcheck
//
//												FString CurrentResponseName = "Response" + FString::FromInt(k);
//												UUserWidget* CurrentResponse = CreateWidget<UBigTalk_Answers>(GetWorld()->GetFirstPlayerController(), DialogueAnswers_BP, FName(*CurrentResponseName));
//
//												DialogueScrollBox->AddChildToVerticalBox(CurrentResponse);
//
//												if (UBigTalk_Answers* MyCurrentResponse = Cast<UBigTalk_Answers>(CurrentResponse))
//												{
//													MyCurrentResponse->CurrentNPC = MyNPC;
//													MyCurrentResponse->ButtonResponse = (*xak).NextIndex;
//													MyCurrentResponse->ClickedResponse = kString;
//													UE_LOG(LogTemp, Warning, TEXT("End = %d"), (*xak).bEnd);
//													MyCurrentResponse->EndButton = (*xak).bEnd;
//													MyCurrentResponse->CurrentResponseCount = (*foo).AnswersIndexArr.Num();
//													MyCurrentResponse->AnswerTextBlock->SetText((*xak).DialogueText);
//													MyCurrentResponse->AnswerTextBlock->ForceLayoutPrepass();
//													if ((*xak).bVisited)
//													{
//														UE_LOG(LogTemp, Warning, TEXT("bVisited = true, setting color"), *kString);
//														MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(FLinearColor(0.3f, 0, 0, 1.0f));
//														MyCurrentResponse->UnhoveredColorCPP = FLinearColor(0.3f, 0, 0, 1.0f);
//													}
//												}
//												ScrollboxScrollToWidget(CurrentResponse);
//												ScrollboxScrollDown();
//											}
//										}
//									}
//								}
//							}
//							//end writing out answers
//
//							ScrollBoxMain->ScrollToEnd();
//							ScrollboxScrollDown();
//
//							if ((*foo).DialogueAdditional.Num() > 0) //DIALOGUE OPTIONS 2 - post printing
//							{
//								int ArrNum = (*foo).DialogueAdditional.Num();
//								int j = 0;
//
//								while (j < ArrNum)
//								{
//									if (j != ArrNum) //safety check
//									{
//										FString read = ((*foo).DialogueAdditional)[j];
//										UE_LOG(LogTemp, Warning, TEXT("Read string: %s"), *read);
//
//										if (read == "DLG_INDEX_RESET")
//										{
//											UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to 0"));
//											MyNPC->BigDialogueIndex = 0;
//											MyNPC->big_i = 0;
//										}
//
//										else if (read == "DLG_INDEX_NEXT")
//										{
//											FString IndexString = ((*foo).DialogueAdditional)[j + 1];
//
//											FDefaultValueHelper::ParseInt(IndexString, MyNPC->BigDialogueIndex);
//											UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to %d"), MyNPC->BigDialogueIndex);
//										}
//
//										else if (read == "MOVE" && (j + 1) < ArrNum)
//										{
//											TArray<FString> VectorArray;
//											FString VectorString = ((*foo).DialogueAdditional)[j + 1];
//
//											VectorString.ParseIntoArray(VectorArray, TEXT(" "), true);
//											FVector Move = FVector(FCString::Atof(*VectorArray[0]), FCString::Atof(*VectorArray[1]), FCString::Atof(*VectorArray[2]));
//											UAIBlueprintHelperLibrary::SimpleMoveToLocation(MyNPC->GetController(), Move);
//
//										}
//										else if (read == "TEST") { UE_LOG(LogTemp, Warning, TEXT("DialogueAdditionals - TEST")); }
//										else if (read == "REDFLASH") { RedFlash(); }
//										else if (read == "BLACKBGON") { BlackBGOn(); }
//										else if (read == "BLACKBGOFF") { BlackBGOff(); }
//										else if (read == "BLURBGON") { BlurBGOn(); }
//										else if (read == "BLURBGOFF") { BlurBGOff(); }
//										else if (read == "BLACKINSTANT") { BlackBGInstant(); }
//
//										j++;
//									}
//								}
//							}
//
//
//							//UE_LOG(LogTemp, Warning, TEXT("Creating widget 2"));
//							//UUserWidget* CurrentWidget2 = CreateWidget<UBigTalk_DialoguePart>(GetWorld()->GetFirstPlayerController(), DialoguePart_BP, TEXT("CurrentWidgetName2"));
//							///*ScrollboxContents.Add(CurrentWidget);*/
//
//							//if (CurrentWidget2)
//							//{
//
//							//	UE_LOG(LogTemp, Warning, TEXT("CurrentWidget2 succesfully created"));
//
//							//	DialogueScrollBox->AddChildToVerticalBox(CurrentWidget2);
//
//							//	if (UBigTalk_DialoguePart* MyCurrentWidget2 = Cast<UBigTalk_DialoguePart>(CurrentWidget2))
//							//	{
//							//		UE_LOG(LogTemp, Warning, TEXT("Setting up widget 2"));
//							//		FString CharNameStyled = "<WhiteB>" + MyNPC->CharName + ":" + "</>";
//							//		MyCurrentWidget2->DialogueChar->SetText(FText::FromString(CharNameStyled));
//							//		MyCurrentWidget2->DialogueText->SetText(FText::FromString("Testing testing 2 2 2 2 "));
//							//	}
//							//}
//							//
//						}
//					}
//				}
//			}
//		}
//	}
//}
