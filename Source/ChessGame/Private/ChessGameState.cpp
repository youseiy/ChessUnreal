// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameState.h"

#include "ChessBoard.h"
#include "ChessGameplayTags.h"
#include "ChessPlayerController.h"

#include "ChessGame/ChessGame.h"
#include "GameFramework/PlayerState.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

AChessGameState::AChessGameState()
{
	
}

void AChessGameState::Server_InitGame( TSubclassOf<AChessBoard> ChessBoardClass)
{
	if (HasAuthority())
	{
		
		
		auto* WhitePC=Cast<AChessPlayerController>( PlayerArray[0]->GetPlayerController());

		 WhitePC->Server_SetTeam(TAG_Team_White);
		
		White.Key=TAG_Team_White;
		White.Value=WhitePC;
		

		auto* BlackPC=Cast<AChessPlayerController>( PlayerArray[1]->GetPlayerController());

		Black.Key=TAG_Team_Black;
		Black.Value=BlackPC;
		
		BlackPC->Server_SetTeam(TAG_Team_Black);
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner=this;
		ChessBoard=GetWorld()->SpawnActor<AChessBoard>(ChessBoardClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParameters);

		FTimerHandle StartMatchTimer;
		GetWorld()->GetTimerManager().SetTimer(StartMatchTimer,FTimerDelegate::CreateWeakLambda(this,[this]()
		{
			//Server_SetCurrentTurn(TAG_Team_White);
			Cast<AChessPlayerController>(White.Value)->Client_StartTurn();
			
		}),1.f,false);
	}
}


void AChessGameState::Server_EndTurn(FGameplayTag Tag)
{
	ensure(CurrentTurn.MatchesTagExact(Tag));

	Server_SetCurrentTurn(CurrentTurn==TAG_Team_Black?TAG_Team_White:TAG_Team_Black);
	
}
void AChessGameState::BeginPlay()
{
	Super::BeginPlay();
	
}
void AChessGameState::Server_SetCurrentTurn(FGameplayTag NewTurn)
{
	if (!HasAuthority()) return;
	
	CurrentTurn=NewTurn;

	if (White.Key==NewTurn)
	{
		Cast<AChessPlayerController>(White.Value)->Client_StartTurn();
		Cast<AChessPlayerController>(Black.Value)->Client_EndTurn();
	}
	else
	{
		Cast<AChessPlayerController>(Black.Value)->Client_StartTurn();
		Cast<AChessPlayerController>(White.Value)->Client_EndTurn();
	}
	UE_LOGFMT(LogChessGame,Warning,"NEW TURN: {a}",NewTurn.GetTagName());
	
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
	
	
}