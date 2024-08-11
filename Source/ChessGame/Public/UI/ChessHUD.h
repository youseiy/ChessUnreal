// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "ChessHUD.generated.h"



UCLASS()
class CHESSGAME_API AChessHUD : public AHUD
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMatchCount);
	UPROPERTY(BlueprintAssignable)
	FOnStartMatchCount OnStartMatchCount;

	void AddToViewport(const FGameplayTag& WidgetTag);
	void RemoveFromViewport(const FGameplayTag& WidgetTag);
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	void CreateWidgets();
	
	UPROPERTY()
	TMap<FGameplayTag,UUserWidget*> Widgets;

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> PauseWidget;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> WaitingPlayersWidget;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> HUDWidget;
};
