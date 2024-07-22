// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "ChessPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessPlayerState : public APlayerState
{
	GENERATED_BODY()


	UPROPERTY(ReplicatedUsing=OnRep_TurnTag)
	FGameplayTag TurnTag;


	

	UFUNCTION()
	void OnRep_TurnTag();
	
	void OnTurnChangedCallback(FGameplayTag Turn);
	void OnPlayerReadyCallback();
	void OnMoveFinishedCallback();
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	//This is called to block and give input
	DECLARE_DELEGATE_OneParam(FOnTurnChanged,FGameplayTag TurnTag)
	FOnTurnChanged OnTurnChanged;
	//This is called when a chess piece is moved or capture other piece
	DECLARE_DELEGATE(FOnMoveFinished)
	FOnMoveFinished OnMoveFinished;
	
	

	UFUNCTION(Server,Reliable)
	void Server_PlayerReady();
	UFUNCTION(Server,Reliable)
	void Server_MoveFinished(FGameplayTag Tag);
	


	
	
	DECLARE_DELEGATE(FOnPlayerReady)
	FOnPlayerReady OnPlayerReady;
	
};
