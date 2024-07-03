// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerState.h"

#include "ChessGameMode.h"
#include "ChessGameState.h"
#include "Net/UnrealNetwork.h"

void AChessPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	OnPlayerReady.BindUObject(this,&ThisClass::OnPlayerReadyCallback);
}
void AChessPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChessPlayerState,TeamTag);
	DOREPLIFETIME(AChessPlayerState,TurnTag);
}

void AChessPlayerState::Server_SetTeamTag_Implementation(FGameplayTag Tag)
{
	TeamTag=Tag;
}
void AChessPlayerState::OnRep_TeamTag()
{
	
}
void AChessPlayerState::OnRep_TurnTag()
{
	OnTurnChanged.ExecuteIfBound(TurnTag);
}

void AChessPlayerState::OnPlayerReadyCallback()
{
	OnPlayerReady.Unbind();
	Server_PlayerReady();
}

void AChessPlayerState::Server_PlayerReady_Implementation()
{
	Cast<AChessGameMode>(GetWorld()->GetAuthGameMode())->OnPlayerReady.ExecuteIfBound();
}






