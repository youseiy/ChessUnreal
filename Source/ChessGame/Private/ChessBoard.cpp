// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessBoard.h"

#include "ChessGameMode.h"
#include "ChessPiecesSet.h"
#include "ChessGame/Public/ChessPiece.h"
#include "ChessGame/Public/ChessTile.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


// Sets default values
AChessBoard::AChessBoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	bReplicates=true;
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		//todo:refactor
		Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this))->ChessBoard=this;;
		BuildBoard();
	}

	
}
void AChessBoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	
}

TSubclassOf<AChessPiece> AChessBoard::GetPieceClass(int32 Index, bool bIsWhite) const
{
	switch (Index)
	{
	case 0: case 7: return bIsWhite ? WhitePiecesSet->Rook: BlackPiecesSet->Rook;
	case 1: case 6: return bIsWhite ? WhitePiecesSet->Knight : BlackPiecesSet->Knight;
	case 2: case 5: return bIsWhite ?  WhitePiecesSet->Bishop :  BlackPiecesSet->Bishop;
	case 3: return bIsWhite ? WhitePiecesSet->Queen : BlackPiecesSet->Queen;
	case 4: return bIsWhite ? WhitePiecesSet->King : BlackPiecesSet->King;
	default: return nullptr;
	}
}

void AChessBoard::BuildBoard()
{
	constexpr int32 X{8};
	constexpr int32 Y{8};
	
	if (!IsValid(BlackTile ) || !IsValid(BlackTile)) return;
	
	for (auto OuterIndex{0}; OuterIndex <  X; OuterIndex++)
	{
		for (auto InnerIndex{0}; InnerIndex < Y; InnerIndex++)
		{
			FVector SpawnLoc = FVector{
				static_cast<float>(InnerIndex - X / 2),
				static_cast<float>(OuterIndex - Y / 2),
				0.f
			} * 100 + GetActorLocation();

			FTransform SpawnTrans;
			SpawnTrans.SetLocation(SpawnLoc);

			
			

			// Alternate between black and white tiles
			TSubclassOf<AChessTile> TileClass = ((OuterIndex + InnerIndex) % 2 == 0) ? WhiteTile : BlackTile;

			auto* Tile = GetWorld()->SpawnActorDeferred<AChessTile>(TileClass, SpawnTrans);
			Tile->SetBoardID({static_cast<float>(OuterIndex),static_cast<float>(InnerIndex)});
			
			Tiles.Emplace(Tile);
			
			Tile->FinishSpawning(SpawnTrans);
			
			DrawDebugString(GetWorld(),Tile->GetActorLocation(),FString::Printf(TEXT("Tile ID: X: %ls %ls"),*FString::SanitizeFloat(OuterIndex,0),*FString::SanitizeFloat(InnerIndex,0)),nullptr,FColor::Blue,5.f,false,1);
			Tile->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
			
			if ((OuterIndex + InnerIndex) % 2 == 0)
			{
				WhiteTiles.Emplace(Tile);
			}
			else
			{
				BlackTiles.Emplace(Tile);
			}
			
			// Determine chess piece
			TSubclassOf<AChessPiece> PieceClass = nullptr;

			if (OuterIndex == 1)
			{
				PieceClass = WhitePiecesSet->Pawn;
			}
			else if (OuterIndex == 6)
			{
				PieceClass = BlackPiecesSet->Pawn;
				
			}
			else if (OuterIndex == 0 || OuterIndex == 7)
			{
				bool bIsWhite = (OuterIndex == 0);
				PieceClass = GetPieceClass(InnerIndex, bIsWhite);
			}
			

			// Spawn chess piece if applicable
			if (PieceClass)
			{
				AChessPiece* Piece = GetWorld()->SpawnActor<AChessPiece>(PieceClass, Tile->GetActorLocation(), FRotator::ZeroRotator);
				
				FVector2D BoardID{static_cast<float>(OuterIndex),static_cast<float>(InnerIndex)};
				Piece->SetInitBoardID(BoardID);
				Tile->SetChessPiece(Piece);
				Piece->AttachToActor(Tile, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
		}
	}
	UE_LOGFMT(LogTemp,Warning,"Chess board has {a} Tiles, {b} Black and {c} White",Tiles.Num(),WhiteTiles.Num(),BlackTiles.Num());
}

