// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LoadingScreenDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=LoadingScreen,defaultconfig)
class LOADINGSCREENRUNTIME_API ULoadingScreenDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Config,Category=LoadingScreenWidget)
	TSoftClassPtr<class UUserWidget> LoadingScreen;
public:
	UPROPERTY(EditAnywhere,Category=LoadingScreenAttributes)
	float MinimumLoadingScreenDisplayTime{2.f};
	

	TSoftClassPtr<UUserWidget> GetLoadingScreenWidget() {return LoadingScreen;}
};
