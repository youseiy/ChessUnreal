// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::FindGame_Implementation()
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateGameButton->OnClicked.AddDynamic(this,&ThisClass::UMainMenuWidget::UMainMenuWidget::OnCreateGameButtonClicked);
	JoinGameButton->OnClicked.AddDynamic(this,&ThisClass::UMainMenuWidget::UMainMenuWidget::OnJoinGameButtoClicked);
	QuitButton->OnClicked.AddDynamic(this,&ThisClass::UMainMenuWidget::OnQuitButtonButtonClicked);
}

void UMainMenuWidget::CreateGame_Implementation()
{
}

void UMainMenuWidget::OnCreateGameButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("CliCked"));
	CreateGame();
}

void UMainMenuWidget::OnJoinGameButtoClicked()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("CliCked"));
	FindGame();
}

void UMainMenuWidget::OnQuitButtonButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("Quitting..."));

	UKismetSystemLibrary::QuitGame(GetWorld(),GetOwningPlayer(),EQuitPreference::Quit,true);
}

