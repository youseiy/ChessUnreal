// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ChessGameMode.generated.h"

class AChessBoard;
class AChessPlayerPawn;
/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessGameMode : public AGameModeBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<AChessPlayerPawn> PlayerClass;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSubclassOf<AChessBoard> BoardClass;

	bool bIsWhiteSelected=false;
	bool bIsBlackSelected=false;

	int32 PlayersReady{0};
	
	FTimerHandle PlayersReadyTimerHandle;

	void OnCheckPlayersReady();
protected:
	virtual void PostInitializeComponents() override;
	virtual void GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	AChessGameMode();

	UPROPERTY()
	AChessBoard* ChessBoard;

	DECLARE_DELEGATE(FOnPlayerReady)
	FOnPlayerReady OnPlayerReady;
};
