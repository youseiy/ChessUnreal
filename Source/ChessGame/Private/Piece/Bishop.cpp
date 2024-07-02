// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/Bishop.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "Kismet/GameplayStatics.h"


ABishop::ABishop()
{
	PrimaryActorTick.bCanEverTick =false;
}
void ABishop::BeginPlay()
{
	Super::BeginPlay();
}

void ABishop::UpdateValidMoves()
{
	Super::UpdateValidMoves();
	// This is only on the server
	auto* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ChessGameMode) || !IsValid(ChessGameMode->ChessBoard))
	{
		return;
	}

	ValidMoves.Empty();

	// Directions the bishop can move: top-right, top-left, bottom-right, bottom-left
	TArray<FVector2D> Directions = {
		{1, 1},   // Top-right
		{-1, 1},  // Top-left
		{1, -1},  // Bottom-right
		{-1, -1}  // Bottom-left
	};

	// Iterate over each direction
	for (const FVector2D& Direction : Directions)
	{
		FVector2D CurrentPosition = CurrentBoardID;

		// Continue moving in the current direction until we hit an obstacle or go out of bounds
		for (;;)
		{
			DrawDebugString(GetWorld(), GetActorLocation(), FString::Printf(TEXT("Piece Board ID %0.0f : %0.0f"), CurrentPosition.X, CurrentPosition.Y));
			CurrentPosition += Direction;

			// Check if the new position is within board limits
			if (!ChessGameMode->ChessBoard->IsWithinBoardLimits(CurrentPosition))
			{
				break;
			}

			AChessTile* Tile = ChessGameMode->ChessBoard->GetTileAt(CurrentPosition);
			if (!Tile)
			{
				break;
			}

			DrawDebugString(GetWorld(), Tile->GetActorLocation(), FString::Printf(TEXT("Board ID %0.0f : %0.0f"), CurrentPosition.X, CurrentPosition.Y));

			// Check if the tile is occupied
			if (Tile->IsOccupied())
			{
				if (Tile->GetChessPiece()->GetIsWhite() != GetIsWhite())
				{
					ValidMoves.Add(Tile); // Capture move
				}
				break; // Stop moving in this direction
			}

			ValidMoves.Add(Tile); // Valid move
		}
	}
}



