// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"

#include "ChessGameplayTags.h"
#include "ChessGameState.h"
#include "ChessPlayerController.h"
#include "ChessPlayerPawn.h"
#include "ChessPlayerState.h"
#include "ChessGame/ChessGame.h"
#include "Logging/StructuredLog.h"
#include "UI/ChessHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogChessGameMode, All, All);

AChessGameMode::AChessGameMode()
{
	PlayerStateClass=AChessPlayerState::StaticClass();
	PlayerControllerClass=AChessPlayerController::StaticClass();
	HUDClass=AChessHUD::StaticClass();
}

void AChessGameMode::OnCheckPlayersReady()
{
	UE_LOGFMT(LogChessGameMode,Warning,"Players Ready: {a}",PlayersReady);
	if (PlayersReady>=2)
	{
		//INIT GAME! 
		GetGameState<AChessGameState>()->Server_InitGame(BoardClass);
	}
}

void AChessGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void AChessGameMode::GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL)
{
	Super::GameWelcomePlayer(Connection, RedirectURL);
	
}

void AChessGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnPlayerReady.BindWeakLambda(this,[this]()
	{
		PlayersReady++;
		if (PlayersReady>=2)
		{
			GetWorld()->GetFirstPlayerController()->GetHUD<AChessHUD>()->RemoveFromViewport(ChessGameplayTags::TAG_Widget_WaitingforPlayers);
			//INIT GAME! 
			GetGameState<AChessGameState>()->Server_InitGame(BoardClass);
		}
		else
		{
			
			GetWorld()->GetFirstPlayerController()->GetHUD<AChessHUD>()->AddToViewport(ChessGameplayTags::TAG_Widget_WaitingforPlayers);
			
			
		}
	});
}

void AChessGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                              FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
}

void AChessGameMode::OnPostLogin(AController* NewPlayer)
{
	FGameplayTag PlayerTeamTag;
	
	//Determine player team based on rand bool 50/50
	if(!bIsWhiteSelected)
	{
		PlayerTeamTag =ChessGameplayTags::TAG_Team_White;
		bIsWhiteSelected=PlayerTeamTag.MatchesTagExact(ChessGameplayTags::TAG_Team_White);
	}
	else
	{
		PlayerTeamTag=ChessGameplayTags::TAG_Team_Black;
	}
	

	
	;
	
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
	//SpawnParams.Owner = NewPlayer;
	

	

	
	AChessPlayerPawn* NewPawn = GetWorld()->SpawnActor<AChessPlayerPawn>(PlayerClass, StartTransform);

	DrawDebugBox(GetWorld(),NewPawn->GetActorLocation(),FVector{500.f,500.f,500.f},FColor::Blue);
	
	if (!NewPawn)
	{
		UE_LOG(LogChessGame, Error, TEXT("Failed to spawn player pawn for %s!"), *NewPlayer->GetName());
		return;
	}

	// Possess the pawn
	NewPlayer->Possess(NewPawn);

	
	
}
