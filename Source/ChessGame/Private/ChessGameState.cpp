// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameState.h"

#include "ChessBoard.h"
#include "ChessGame/ChessGame.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

AChessGameState::AChessGameState()
{
	
}

void AChessGameState::Server_SetChessBoard_Implementation(AChessBoard* Board)
{
	ChessBoard=Board;
	ChessBoard->SetOwner(GetWorld()->GetFirstPlayerController());
}

void AChessGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChessGameState,ChessBoard)
}

void AChessGameState::BeginPlay()
{
	Super::BeginPlay();

	
}
