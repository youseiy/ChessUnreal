// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerState.h"

#include "ChessGameMode.h"
#include "ChessGameState.h"
#include "ChessPlayerController.h"
#include "ChessGame/ChessGame.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

void AChessPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	if (HasAuthority())
	{
		GetWorld()->GetGameState<AChessGameState>()->OnTurnChanged.BindUObject(this,&ThisClass::OnTurnChangedCallback);
	}
	
	OnPlayerReady.BindUObject(this,&ThisClass::OnPlayerReadyCallback);
	OnMoveFinished.BindUObject(this,&ThisClass::OnMoveFinishedCallback);
}

void AChessPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AChessPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChessPlayerState,TurnTag);
}
void AChessPlayerState::OnTurnChangedCallback(FGameplayTag Turn)
{
	if (HasAuthority())
	{
		TurnTag=Turn;
		OnRep_TurnTag();
	}
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
void AChessPlayerState::OnMoveFinishedCallback()
{
	Server_MoveFinished(Cast<AChessPlayerController>(GetPlayerController())->GetTeam());
}
void AChessPlayerState::Server_PlayerReady_Implementation()
{
	Cast<AChessGameMode>(GetWorld()->GetAuthGameMode())->OnPlayerReady.ExecuteIfBound();
}
void AChessPlayerState::Server_MoveFinished_Implementation(FGameplayTag Tag)
{
	check(GetWorld())
	GetWorld()->GetGameState<AChessGameState>()->Server_EndTurn(Cast<AChessPlayerController>(GetPlayerController())->GetTeam());
}





