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
    
	FVector2D DiagonalLeft = { CurrentBoardID.X + (GetIsWhite() ? 1 : -1), CurrentBoardID.Y - 1 };
	FVector2D DiagonalRight = { CurrentBoardID.X + (GetIsWhite() ? 1 : -1), CurrentBoardID.Y + 1 };

	// Iterate over tiles to find valid moves
	for (const auto& Tile : Tiles)
	{
		FVector2D TileID = Tile->GetTileID();

		if (TileID == OneStep || (TileID == TwoStep && !bAlreadyMoved))
		{
			// Check if the tile is empty
			if (!Tile->IsOccupied())
			{
				ValidMoves.Add(Tile);
			}

			if (bAlreadyMoved && ValidMoves.Num() >= 1)
			{
				break;
			}
		}
		else if (TileID == DiagonalLeft || TileID == DiagonalRight)
		{
			// Check if the tile has an enemy piece
			if (Tile->IsOccupied() && Tile->GetChessPiece()->GetIsWhite() != GetIsWhite())
			{
				ValidMoves.Add(Tile);
			}
		}
	}
}
void APawn_::Server_TryMoveTo_Implementation(AChessTile* NewPosition)
{
	bAlreadyMoved=true;
	Super::Server_TryMoveTo_Implementation(NewPosition);
}


