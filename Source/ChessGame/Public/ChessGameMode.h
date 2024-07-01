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

	
	
protected:
	virtual void GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	AChessGameMode();

	UPROPERTY()
	AChessBoard* ChessBoard;
};
