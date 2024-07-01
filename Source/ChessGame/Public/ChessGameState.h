// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ChessGameState.generated.h"

class AChessBoard;
/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(Replicated)
	TObjectPtr<AChessBoard> ChessBoard;
	
	

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
public:
	AChessGameState();
	UFUNCTION(Server,Reliable)
	void Server_SetChessBoard(AChessBoard* Board);

	
	AChessBoard* GetChessBoard()const{ return ChessBoard;}
};
