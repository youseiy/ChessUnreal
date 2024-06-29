// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"

#include "ChessPlayerController.h"
#include "ChessPlayerPawn.h"
#include "ChessPlayerState.h"
#include "ChessGame/ChessGame.h"
#include "Logging/StructuredLog.h"


AChessGameMode::AChessGameMode()
{
	PlayerStateClass=AChessPlayerState::StaticClass();
	PlayerControllerClass=AChessPlayerController::StaticClass();
}

void AChessGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
}

void AChessGameMode::OnPostLogin(AController* NewPlayer)
{

	UE_LOGFMT(LogChessGame,Warning,"PC {a} entered the game!",NewPlayer->GetName());
	
	Super::OnPostLogin(NewPlayer);

	check(PlayerClass);
	
	FString Tag=GetNumPlayers()<=1?TEXT("1"):TEXT("2");
	
	auto* PS=FindPlayerStart(NewPlayer,Tag);
	
	const FTransform PSTransform{PS->GetTransform()};

	FActorSpawnParameters Params;
	Params.Owner=NewPlayer;

	auto PPawn=GetWorld()->SpawnActor<AChessPlayerPawn>(PlayerClass,PSTransform,Params);

	NewPlayer->Possess(PPawn);

	RestartPlayerAtPlayerStart(NewPlayer,PS);
}
