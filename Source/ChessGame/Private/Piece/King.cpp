// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/King.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "Kismet/GameplayStatics.h"


AKing::AKing()
{
	PrimaryActorTick.bCanEverTick = false;
}
void AKing::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AKing::InCheck() const 
{
	return false;
}

void AKing::UpdateValidMoves()
{
	Super::UpdateValidMoves();
	// This is only on the server
	auto* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ChessGameMode) || !IsValid(ChessGameMode->ChessBoard))
	{
		return;
	}

	ValidMoves.Empty();

	// Directions the king can move: all adjacent squares
	TArray<FVector2D> Directions = {
		{1, 0},    // Right
		{-1, 0},   // Left
		{0, 1},    // Up
		{0, -1},   // Down
		{1, 1},    // Top-right
		{-1, 1},   // Top-left
		{1, -1},   // Bottom-right
		{-1, -1}   // Bottom-left
	};

	// Iterate over each direction
	for (const FVector2D& Direction : Directions)
	{
		FVector2D CurrentPosition = CurrentBoardID + Direction;

		// Check if the new position is within board limits
		if (!ChessGameMode->ChessBoard->IsWithinBoardLimits(CurrentPosition))
		{
			continue;
		}

		AChessTile* Tile = ChessGameMode->ChessBoard->GetTileAt(CurrentPosition);
		if (!Tile)
		{
			continue;
		}

		DrawDebugString(GetWorld(), Tile->GetActorLocation(), FString::Printf(TEXT("Board ID %0.0f : %0.0f"), CurrentPosition.X, CurrentPosition.Y));

		// Check if the tile is occupied
		if (Tile->IsOccupied())
		{
			if (Tile->GetChessPiece()->GetIsWhite() != GetIsWhite())
			{
				ValidMoves.Add(Tile); // Capture move
			}
		}
		else
		{
			ValidMoves.Add(Tile); // Valid move
		}
	}
}


