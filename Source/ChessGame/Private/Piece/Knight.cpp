// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/Knight.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AKnight::AKnight()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AKnight::UpdateValidMoves()
{

	// This is only on the server
	auto* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ChessGameMode) || !IsValid(ChessGameMode->ChessBoard))
	{
		return;
	}

	auto& Tiles = ChessGameMode->ChessBoard->GetTiles();
	ValidMoves.Empty();

	// Determine the direction multipliers based on piece color
	float VerticalMultiplier = GetIsWhite() ? 1 : -1;
	float HorizontalMultiplier = 1; // Horizontal direction remains the same for both colors

	// Define all possible knight move offsets based on piece color
	TArray<FVector2D> KnightMoveOffsets;
	if (GetIsWhite())
	{
		KnightMoveOffsets = {
			{1, 2}, {1, -2}, {-1, 2}, {-1, -2},
			{2, 1}, {2, -1}, {-2, 1}, {-2, -1}
		};
	}
	else // Black
	{
		KnightMoveOffsets = {
			{1, -2}, {1, 2}, {-1, -2}, {-1, 2},
			{2, -1}, {2, 1}, {-2, -1}, {-2, 1}
		};
	}

	// Calculate valid moves for the knight
	for (const FVector2D& Offset : KnightMoveOffsets)
	{
		FVector2D Destination = CurrentBoardID + Offset;

		// Check if the destination is within board limits
		if (ChessGameMode->ChessBoard->IsWithinBoardLimits(Destination))
		{
			AChessTile* Tile = ChessGameMode->ChessBoard->GetTileAt(Destination);

			// Check if the tile is not occupied or occupied by an opponent's piece
			if (!Tile->IsOccupied() || Tile->GetChessPiece()->GetIsWhite() != GetIsWhite())
			{
				ValidMoves.Add(Tile);
			}
		}
	}
}

// Called when the game starts or when spawned
void AKnight::BeginPlay()
{
	Super::BeginPlay();
	
}

