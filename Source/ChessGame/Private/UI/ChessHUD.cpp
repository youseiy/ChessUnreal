// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChessHUD.h"

#include "ChessGameplayTags.h"
#include "Blueprint/UserWidget.h"

void AChessHUD::AddToViewport(const FGameplayTag& WidgetTag)
{
	auto Widget{Widgets.FindChecked(WidgetTag)};

	Widget->AddToViewport();
}

void AChessHUD::RemoveFromViewport(const FGameplayTag& WidgetTag)
{
	auto Widget{Widgets.FindChecked(WidgetTag)};
	if (Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
	}
	
}

void AChessHUD::BeginPlay()
{
	Super::BeginPlay();
	CreateWidgets();
}

void AChessHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void AChessHUD::CreateWidgets()
{
	Widgets.Add(ChessGameplayTags::TAG_Widget_WaitingforPlayers,CreateWidget(GetOwningPlayerController(),WaitingPlayersWidget));
	Widgets.Add(ChessGameplayTags::TAG_Widget_Pause,CreateWidget(GetOwningPlayerController(),PauseWidget));
	Widgets.Add(ChessGameplayTags::TAG_Widget_WaitingforPlayers,CreateWidget(GetOwningPlayerController(),HUDWidget));
}
