// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChessHUD.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessHUD : public AHUD
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> PauseWidget;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> WaitingPlayersWidget;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> HUDWidget;
	
};
