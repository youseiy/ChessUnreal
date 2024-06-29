// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/Pawn_.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "ChessTile.h"
#include "Kismet/GameplayStatics.h"


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

TArray<AChessTile*> APawn_::GetValidMoves() const
{
	//todo:otimize the code to use a inverse loop for black and normal loop for white
	if (HasAuthority())
	{
		const auto& Tiles=Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this))->ChessBoard->GetTiles();

		TArray<AChessTile*> ValidTiles;
		
		//White we increase X to find the valid tiles
		if (GetIsWhite())
		{
			for (const auto& Tile: Tiles)
			{
				if (Tile->GetTileID()==FVector2D{CurrentBoardID.X+1,CurrentBoardID.Y} || Tile->GetTileID()==FVector2D{CurrentBoardID.X+2,CurrentBoardID.Y} )
				{
					ValidTiles.Emplace(Tile);

					if (bAlreadyMoved && ValidTiles.Num()>=1)
					{
						break;
					}
				}
			}
			return ValidTiles;
		}
		for (const auto& Tile: Tiles)
		{
			if (Tile->GetTileID()==FVector2D{CurrentBoardID.X-1,CurrentBoardID.Y} || Tile->GetTileID()==FVector2D{CurrentBoardID.X-2,CurrentBoardID.Y} )
			{
				ValidTiles.Emplace(Tile);

				if (bAlreadyMoved && ValidTiles.Num()>=1)
				{
					break;
				}
			}
				
		}
		return ValidTiles;
	}
	return {};
}

void APawn_::MoveTo(AChessTile* NewPosition)
{
	if (!HasAuthority()) return;
	
	bAlreadyMoved=true;
	SetCurrentBoardID(NewPosition->GetTileID());

	
	SetActorLocation(NewPosition->GetActorLocation());
	NewPosition->SetChessPiece(this);

	
	
	
}

void APawn_::Capture(AChessPiece* TargetPiece)
{
	
}

bool APawn_::IsMoveValid(const FVector& NewPosition) const
{
	return false;
}

