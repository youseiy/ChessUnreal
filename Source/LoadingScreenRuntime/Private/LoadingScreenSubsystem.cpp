// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenSubsystem.h"

#include "LoadingScreenDeveloperSettings.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"


void ULoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::EndLoadingScreen);
}


void ULoadingScreenSubsystem::BeginLoadingScreen(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreen;
	
	LoadingScreen.MinimumLoadingScreenDisplayTime=GetDefault<ULoadingScreenDeveloperSettings>()->MinimumLoadingScreenDisplayTime;

	/* YOU CAN USE THESES TO HAVE BETTER CONTROL OVER THE LOADING SCREEN
	LoadingScreen.MoviePaths
	LoadingScreen.bAllowEngineTick
	LoadingScreen.bMoviesAreSkippable
	LoadingScreen.bAllowInEarlyStartup
	LoadingScreen.bAutoCompleteWhenLoadingCompletes
	LoadingScreen.NewTestLoadingScreenWidget()*/

	
	UUserWidget* CreatedUMGWidget = CreateWidget<UUserWidget>( GetOuterUGameInstance(),GetMutableDefault<ULoadingScreenDeveloperSettings>()->GetLoadingScreenWidget().LoadSynchronous());

	//Get the underlying slate and set it
	LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void ULoadingScreenSubsystem::EndLoadingScreen(UWorld* InLoadedWorld)
{
	
}
