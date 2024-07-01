// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessBoard.h"

#include "ChessGameMode.h"
#include "ChessGameState.h"
#include "ChessPiecesSet.h"
#include "ChessGame/ChessGame.h"
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
		Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this))->ChessBoard=this;;
		BuildBoard();
		GetWorld()->GetGameState<AChessGameState>()->Server_SetChessBoard(this);
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
	
	for (int32 OuterIndex{0}; OuterIndex <  X; OuterIndex++)
	{
		for (int32 InnerIndex{0}; InnerIndex < Y; InnerIndex++)
		{
			FVector2D BoardID{static_cast<float>(OuterIndex),static_cast<float>(InnerIndex)};
			
			FVector SpawnLoc = FVector{
				static_cast<float>(InnerIndex - X / 2),
				static_cast<float>(OuterIndex - Y / 2),
				0.f
			} * 100 + GetActorLocation();

			FTransform SpawnTrans;
			SpawnTrans.SetLocation(SpawnLoc);

			
			// Alternate between black and white tiles
			TSubclassOf<AChessTile> TileClass = ((OuterIndex + InnerIndex) % 2 == 0) ? WhiteTile : BlackTile;

			auto* Tile = InitChessTile(TileClass, SpawnTrans);
			Tile->SetBoardID(BoardID);
			Tiles.Emplace(Tile);

			
			
			if ((OuterIndex + InnerIndex) % 2 == 0)
			{
				WhiteTiles.Emplace(Tile);
			}
			else
			{
				BlackTiles.Emplace(Tile);
			}
			
			// Determine chess piece
			TSubclassOf<AChessPiece> PieceClass =EvaluatePieceClass(OuterIndex,InnerIndex);;
			

			if (PieceClass)
			{
				auto* SpawnedPiece=InitChessPiece(PieceClass,*Tile,SpawnLoc,BoardID);
				Pieces.Add(SpawnedPiece);
				Tile->SetChessPiece(SpawnedPiece);


				UE_LOGFMT(LogChessGame,Warning,"Tile: {X}:{Y} has Chess piece {a}",OuterIndex,InnerIndex,PieceClass?PieceClass->GetName():"none");
			}
			
			
		}
	}

	//Initialize valid moves for all pieces
	for (const auto& Piece : Pieces)
	{
		Piece->UpdateValidMoves();
	}
	
	UE_LOGFMT(LogChessGame,Warning,"Chess board has {a} Tiles, {b} Black and {c} White",Tiles.Num(),WhiteTiles.Num(),BlackTiles.Num());
}

TSubclassOf<AChessPiece> AChessBoard::EvaluatePieceClass(int32 OuterIndex, int32 InnerIndex)
{
	if (OuterIndex == 1) // Pawns
	{
		return WhitePiecesSet->Pawn;
	}
	if (OuterIndex == 6) // Pawns
	{
		return BlackPiecesSet->Pawn;
	}
	if (OuterIndex == 0 || OuterIndex == 7) // Major pieces
	{
		bool bIsWhite = (OuterIndex == 0);
		return GetPieceClass(InnerIndex, bIsWhite);
	}
	return nullptr;
}

AChessPiece* AChessBoard::InitChessPiece(const TSubclassOf<AChessPiece>& PieceClass, AChessTile& OwnerTile,
                                         const FVector& Location, const FVector2D& BoardID)
{
	FTransform Transform{Location};
	AChessPiece* Piece = GetWorld()->SpawnActorDeferred<AChessPiece>(PieceClass,Transform,this,nullptr);
	Piece->SetInitBoardID(BoardID);
	Piece->FinishSpawning(Transform);
	Piece->AttachToActor(&OwnerTile, FAttachmentTransformRules::SnapToTargetIncludingScale);
	return Piece;
	
}

AChessTile* AChessBoard::InitChessTile(TSubclassOf<AChessTile> TileClass,const FTransform& Location)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner=this;
	auto* Tile=GetWorld()->SpawnActor<AChessTile>(TileClass,Location,SpawnParameters);
	Tile->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
	return Tile;
}
void AChessBoard::Server_RequestChessPieceMove_Implementation(AChessPiece* Piece, AChessTile* Tile)
{
	if (!IsValid(Piece) || !IsValid(Tile)) return;
	
	Piece->Server_TryMoveTo(Tile);
}




