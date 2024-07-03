// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameState.h"

#include "ChessBoard.h"

#include "ChessGame/ChessGame.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

AChessGameState::AChessGameState()
{
	
}

void AChessGameState::Server_InitGame( TSubclassOf<AChessBoard> ChessBoardClass)
{
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner=this;
		ChessBoard=GetWorld()->SpawnActor<AChessBoard>(ChessBoardClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParameters);
	}
}
void AChessGameState::BeginPlay()
{
	Super::BeginPlay();
	
}
void AChessGameState::Server_SetCurrentTurn_Implementation(FGameplayTag NewTurn)
{
	CurrentTurn=NewTurn;

	for (const auto& PlayerState : PlayerArray)
	{
		
	}
	
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
	DOREPLIFETIME(AChessGameState,CurrentTurn)
}

void AChessGameState::OnRep_CurrentTurn()
{
	OnTurnChanged.ExecuteIfBound(CurrentTurn);
}