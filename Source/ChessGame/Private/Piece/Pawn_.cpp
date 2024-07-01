// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/Pawn_.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "ChessTile.h"
#include "ChessGame/ChessGame.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APawn_::APawn_()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APawn_::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UpdateValidMoves();
		UE_LOGFMT(LogChessGame,Warning,"Current valid moves: {a}",ValidMoves.Num());
	}
	
	
}

void APawn_::UpdateValidMoves()
{
	//this is only on the server
	auto* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ChessGameMode) || !IsValid(ChessGameMode->ChessBoard))
	{
		return;
	}

	auto& Tiles = ChessGameMode->ChessBoard->GetTiles();
	ValidMoves.Empty();

	FVector2D OneStep = { CurrentBoardID.X + (GetIsWhite() ? 1 : -1), CurrentBoardID.Y };
	FVector2D TwoStep = { CurrentBoardID.X + 2 * (GetIsWhite() ? 1 : -1), CurrentBoardID.Y };

	// Iterate over tiles to find valid moves
	for (const auto& Tile : Tiles)
	{
		FVector2D TileID = Tile->GetTileID();

		if (TileID == OneStep || (TileID == TwoStep && !bAlreadyMoved))
		{
			ValidMoves.Add(Tile);

			if (bAlreadyMoved && ValidMoves.Num() >= 1)
			{
				break;
			}
		}
	}
}
void APawn_::MoveTo(AChessTile* NewPosition)
{
	if (!HasAuthority()) return;
	
	

}
void APawn_::Capture(AChessPiece* TargetPiece)
{
	
}

bool APawn_::IsMoveValid(const FVector& NewPosition) const
{
	return false;
}

void APawn_::Server_TryMoveTo_Implementation(AChessTile* NewPosition)
{
	Super::Server_TryMoveTo_Implementation(NewPosition);

	
	bAlreadyMoved=true;
	SetCurrentBoardID(NewPosition->GetTileID());
	AnimatedTranslation(NewPosition->GetActorLocation());
	NewPosition->SetChessPiece(this);
	UpdateValidMoves();
	
}


