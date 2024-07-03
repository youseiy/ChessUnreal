// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"

#include "ChessGameplayTags.h"
#include "ChessGameState.h"
#include "ChessPlayerController.h"
#include "ChessPlayerPawn.h"
#include "ChessPlayerState.h"
#include "ChessGame/ChessGame.h"
#include "GameFramework/PlayerStart.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogChessGameMode,All,All);

AChessGameMode::AChessGameMode()
{
	PlayerStateClass=AChessPlayerState::StaticClass();
	PlayerControllerClass=AChessPlayerController::StaticClass();
}

void AChessGameMode::OnCheckPlayersReady()
{
	UE_LOGFMT(LogChessGameMode,Warning,"Players Ready: {a}",PlayersReady);
	if (PlayersReady>=2)
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayersReadyTimerHandle);
		//INIT GAME! 
		GetGameState<AChessGameState>()->Server_InitGame(BoardClass);
	}
}

void AChessGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnPlayerReady.BindWeakLambda(this,[this]()
	{
		PlayersReady++;
	});
	
	GetWorld()->GetTimerManager().SetTimer(PlayersReadyTimerHandle,FTimerDelegate::CreateUObject(this,&ThisClass::OnCheckPlayersReady),0.1f,true);
}

void AChessGameMode::GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL)
{
	Super::GameWelcomePlayer(Connection, RedirectURL);
	
}

void AChessGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                              FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
}

void AChessGameMode::OnPostLogin(AController* NewPlayer)
{

	Super::OnPostLogin(NewPlayer);

	// Log player login
	UE_LOG(LogChessGame, Warning, TEXT("Player %s entered the game!"), *NewPlayer->GetName());

	// Determine player team based on the current number of players
	FGameplayTag PlayerTeamTag = (GetNumPlayers() % 2 == 1) ? TAG_Team_Black :TAG_Team_White;

	// Find player start and spawn pawn
	FString Tag = PlayerTeamTag.ToString();
	AActor* PlayerStart = FindPlayerStart(NewPlayer, Tag);
	if (!PlayerStart)
	{
		UE_LOG(LogChessGame, Error, TEXT("Player start not found for tag %s!"), *Tag);
		return;
	}

	// Spawn pawn at player start
	FTransform StartTransform = PlayerStart->GetTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;

	AChessPlayerPawn* NewPawn = GetWorld()->SpawnActor<AChessPlayerPawn>(PlayerClass, StartTransform, SpawnParams);
	if (!NewPawn)
	{
		UE_LOG(LogChessGame, Error, TEXT("Failed to spawn player pawn for %s!"), *NewPlayer->GetName());
		return;
	}

	// Possess the pawn
	NewPlayer->Possess(NewPawn);

	// Set player team tag in PlayerState
	AChessPlayerState* ChessPlayerState = NewPlayer->GetPlayerState<AChessPlayerState>();
	
	if (ChessPlayerState)
	{
		ChessPlayerState->Server_SetTeamTag(PlayerTeamTag);
	}

	// Optionally, set the initial turn in GameState
	AChessGameState* ChessGameState = GetGameState<AChessGameState>();
	if (ChessGameState)
	{
		// Set initial turn based on the first player's team
		ChessGameState->Server_SetCurrentTurn( TAG_Team_White);
	}

	
	
}
