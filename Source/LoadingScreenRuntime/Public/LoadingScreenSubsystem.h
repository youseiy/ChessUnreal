// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LOADINGSCREENRUNTIME_API ULoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	UFUNCTION(BlueprintCallable)
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION(BlueprintCallable)
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	
};
