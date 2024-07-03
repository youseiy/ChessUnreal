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
	UPROPERTY(ReplicatedUsing=OnRep_TeamTag)
	FGameplayTag TeamTag;

	UPROPERTY(ReplicatedUsing=OnRep_TurnTag)
	FGameplayTag TurnTag;
	
	
	UFUNCTION()
	void OnRep_TeamTag();
	UFUNCTION()
	void OnRep_TurnTag();

	void OnPlayerReadyCallback();
protected:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(Server,Reliable)
	void Server_SetTeamTag(FGameplayTag Tag);
	UFUNCTION(Server,Reliable)
	void Server_PlayerReady();
	
	FGameplayTag GetTeamTag()const {return TeamTag;}
	
	DECLARE_DELEGATE_OneParam(FOnTurnChanged,FGameplayTag TurnTag)
	FOnTurnChanged OnTurnChanged;
	
	DECLARE_DELEGATE(FOnPlayerReady)
	FOnPlayerReady OnPlayerReady;
	
};
