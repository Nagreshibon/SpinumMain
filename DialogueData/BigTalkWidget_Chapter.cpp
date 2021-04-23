// Fill out your copyright notice in the Description page of Project Settings.


#include "BigTalkWidget_Chapter.h"

#include "BigDialogueBody.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DialoguePicker.h"
#include "Misc/DefaultValueHelper.h"
#include "NPCLocalVariables.h"
#include "TimerManager.h"
#include "TOPDOWN_Take1/TOPDOWN_Take1PlayerController.h"
#include "TOPDOWN_Take1/UI/BigTalk_Answers_Chapter.h"
#include "TOPDOWN_Take1/UI/BigTalk_DialoguePart.h"

UBigTalkWidget_Chapter::UBigTalkWidget_Chapter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	i = 0;

}

void UBigTalkWidget_Chapter::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBigTalkWidget_Chapter::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBigTalkWidget_Chapter::StartBigTalk(ATestNPC1* MyNPC)
{

	ScrollBoxMain->ScrollToEnd();
	ScrollboxScrollDown();

	if (End)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ending dialogue"));
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
			}
		}
		SetVisibility(ESlateVisibility::Collapsed);
		RemoveFromViewport();
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
							if (FBigDialogueBody* xak_previous = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*MyNPC->CurrentResponse), ""))
							{
								(*xak_previous).bVisited = true;

								if ((*xak_previous).SkillRaise != "") //SkillRaise
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

											else if (read == "FLAG" && j + 2 < (*xak_previous).DialogueAdditional.Num())
											{
												UE_LOG(LogTemp, Warning, TEXT("Initializing flag"));
												TArray<FString> IndexArray;
												FString IndexString = ((*xak_previous).DialogueAdditional)[j + 2];

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

							if (!(*foo).bCameraPan)
							{
								if (ATOPDOWN_Take1PlayerController* MyController = Cast<ATOPDOWN_Take1PlayerController>(GetWorld()->GetFirstPlayerController()))
								{
									if (ATOPDOWN_Take1Character* MyPawn = Cast<ATOPDOWN_Take1Character>(MyController->GetPawn()))
									{
										MyPawn->DialogueCameraControl(MyNPC->GetActorLocation(), 1100 + 5 * MyNPC->NPCSize, 500, MyNPC->NPCSize);
									}
								}
							}

							//Dialogue Text
							FText bar = (*foo).DialogueText;

							float Duration = (*foo).Duration;

							//end dialogue marker
							End = (*foo).bEnd;

							//index
							/*if (bInsanity)
							{
								UE_LOG(LogTemp, Warning, TEXT("INSANITY = TRUE, setting big_i to the insanity option"));
								if ((*foo).NextIndexInsanity != 0) MyNPC->big_i = (*foo).NextIndexInsanity; else MyNPC->big_i++;
								bInsanity = false;
							}
							else
							{*/
								if ((*foo).NextIndex != 0) MyNPC->big_i = (*foo).NextIndex; else MyNPC->big_i++;
							//}
								
							//set text

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
									if ((*foo).IDName == "NARRATOR")
									{
										PortraitImage->SetVisibility(ESlateVisibility::Hidden);
										MyCurrentWidget->DialogueChar->SetVisibility(ESlateVisibility::Collapsed);
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
										MyCurrentWidget->DialogueText->ForceLayoutPrepass();
										MyCurrentWidget->ForceLayoutPrepass();
										i++;
									}

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
										MyPawn->DialogueCameraControl(MyPawn->GetActorLocation(), 1100, 500, 1);
									}
								}
								if ((*foo).AnswersIndexArr.Num() > 0)
								{
									for (int k = 0; k < (*foo).AnswersIndexArr.Num(); k++) //iterate through the answers
									{

										FString kString = FString::FromInt((*foo).AnswersIndexArr[k]);
										UE_LOG(LogTemp, Warning, TEXT("kString = %s"), *kString);

										if (FBigDialogueBody* xak = DialogueBodyRow->FindRow<FBigDialogueBody>(FName(*kString), TEXT("")))
										{

											bool FlagCheck = true;  //init flag check

											if ((*xak).FlagName != "")
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
																	FString CurrentResponseName = "ResponseChapter" + FString::FromInt(k);
																	UUserWidget* CurrentResponse = CreateWidget<UBigTalk_Answers_Chapter>(GetWorld()->GetFirstPlayerController(), DialogueAnswers_Chapter_BP, FName(*CurrentResponseName));

																	DialogueScrollBox->AddChildToVerticalBox(CurrentResponse);

																	if (UBigTalk_Answers_Chapter* MyCurrentResponse = Cast<UBigTalk_Answers_Chapter>(CurrentResponse))
																	{
																		MyCurrentResponse->CurrentNPC = MyNPC;
																		MyCurrentResponse->ButtonResponse = (*xak).NextIndex;
																		MyCurrentResponse->AnswerInsanityChance = (*xak).InsanityChance;
																		UE_LOG(LogTemp, Warning, TEXT("Printing an answer with Insanity, %d chance"), MyCurrentResponse->AnswerInsanityChance);
																		MyCurrentResponse->InsanityText = (*xak).InsanityText;
																		MyCurrentResponse->NextIndexInsanity = (*xak).NextIndexInsanity;
																		MyCurrentResponse->ClickedResponse = kString;
																		UE_LOG(LogTemp, Warning, TEXT("End = %d"), (*xak).bEnd);
																		MyCurrentResponse->EndButton = (*xak).bEnd;
																		MyCurrentResponse->CurrentResponseCount = (*foo).AnswersIndexArr.Num();
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

													FString CurrentResponseName = "ResponseChapter" + FString::FromInt(k);
													UUserWidget* CurrentResponse = CreateWidget<UBigTalk_Answers_Chapter>(GetWorld()->GetFirstPlayerController(), DialogueAnswers_Chapter_BP, FName(*CurrentResponseName));

													DialogueScrollBox->AddChildToVerticalBox(CurrentResponse);

													if (UBigTalk_Answers_Chapter* MyCurrentResponse = Cast<UBigTalk_Answers_Chapter>(CurrentResponse))
													{
														MyCurrentResponse->CurrentNPC = MyNPC;
														MyCurrentResponse->ButtonResponse = (*xak).NextIndex;
														MyCurrentResponse->ClickedResponse = kString;
														MyCurrentResponse->AnswerInsanityChance = (*xak).InsanityChance;
														MyCurrentResponse->NextIndexInsanity = (*xak).NextIndexInsanity;
														UE_LOG(LogTemp, Warning, TEXT("Printing an answer with Insanity, %d chance"), MyCurrentResponse->AnswerInsanityChance);
														MyCurrentResponse->InsanityText = (*xak).InsanityText;
														UE_LOG(LogTemp, Warning, TEXT("End = %d"), (*xak).bEnd);
														MyCurrentResponse->EndButton = (*xak).bEnd;
														MyCurrentResponse->CurrentResponseCount = (*foo).AnswersIndexArr.Num();
														MyCurrentResponse->AnswerTextBlock->SetText((*xak).DialogueText);
														MyCurrentResponse->AnswerTextBlock->ForceLayoutPrepass();
														if ((*xak).bVisited)
														{
															UE_LOG(LogTemp, Warning, TEXT("bVisited = true, setting color"), *kString);
															MyCurrentResponse->AnswerTextBlock->SetColorAndOpacity(FLinearColor(0.3f, 0, 0, 1.0f));
															MyCurrentResponse->UnhoveredColorCPP = FLinearColor(0.3f, 0, 0, 1.0f);
														}
													}
												}
											}
										}
										//to next answer
									}
								}
							}
							//end writing out answers

							ScrollBoxMain->ScrollToEnd();
							ScrollboxScrollDown();

							if ((*foo).DialogueAdditional.Num() > 0) //DIALOGUE OPTIONS 2 - post printing
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
											MyNPC->BigDialogueIndex = 0;
											MyNPC->big_i = 0;
										}

										else if (read == "DLG_INDEX_NEXT")
										{
											FString IndexString = ((*foo).DialogueAdditional)[j + 1];

											FDefaultValueHelper::ParseInt(IndexString, MyNPC->BigDialogueIndex);
											UE_LOG(LogTemp, Warning, TEXT("Setting DialogueIndex to %d"), MyNPC->BigDialogueIndex);
										}

										else if (read == "MOVE" && (j + 1) < ArrNum)
										{
											TArray<FString> VectorArray;
											FString VectorString = ((*foo).DialogueAdditional)[j + 1];

											VectorString.ParseIntoArray(VectorArray, TEXT(" "), true);
											FVector Move = FVector(FCString::Atof(*VectorArray[0]), FCString::Atof(*VectorArray[1]), FCString::Atof(*VectorArray[2]));
											UAIBlueprintHelperLibrary::SimpleMoveToLocation(MyNPC->GetController(), Move);

										}
										else if (read == "TEST") { UE_LOG(LogTemp, Warning, TEXT("DialogueAdditionals - TEST")); }
										else if (read == "REDFLASH") { RedFlash(); }
										else if (read == "BLACKBGON") { BlackBGOn(); }
										else if (read == "BLACKBGOFF") { BlackBGOff(); }
										else if (read == "BLURBGON") { BlurBGOn(); }
										else if (read == "BLURBGOFF") { BlurBGOff(); }
										else if (read == "BLACKINSTANT") { BlackBGInstant(); }

										j++;
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