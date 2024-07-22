// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameplayTagContainer.h"
#include "ChessGameState.generated.h"

class AChessPlayerController;
class AChessBoard;
/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessGameState : public AGameStateBase
{
	GENERATED_BODY()
private:
	
	UPROPERTY(Replicated)
	TObjectPtr<AChessBoard> ChessBoard;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentTurn)
	FGameplayTag CurrentTurn;

	//These are only set on the server
	TPair<FGameplayTag,APlayerController*> White;
	TPair<FGameplayTag,APlayerController*> Black;


	

	UFUNCTION()
	void OnRep_CurrentTurn();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;



public:
	AChessGameState();


	void Server_InitGame(TSubclassOf<AChessBoard> ChessBoardClass);
	FGameplayTag GetTurn() const{return CurrentTurn;}

	void Server_SetCurrentTurn(FGameplayTag NewTurn);
	
	UFUNCTION(Server,Reliable)
	void Server_SetChessBoard(AChessBoard* Board);
	
	void Server_EndTurn(FGameplayTag Tag);
	
	AChessBoard* GetChessBoard()const{ return ChessBoard;}
	
	DECLARE_DELEGATE_OneParam(FOnTurnChanged,FGameplayTag TurnTag)
	FOnTurnChanged OnTurnChanged;

	
	
};
