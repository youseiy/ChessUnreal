// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessBoard.h"

#include "ChessGame/Public/ChessPiece.h"
#include "ChessGame/Public/ChessTile.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


// Sets default values
AChessBoard::AChessBoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	SetReplicates(true);
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();

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

			Tiles.Emplace(Tile);

			if ((OuterIndex + InnerIndex) % 2 == 0)
			{
				WhiteTiles.Emplace(Tile);
			}
			else
			{
				BlackTiles.Emplace(Tile);
			}
			
			Tile->FinishSpawning(SpawnTrans);

			DrawDebugString(GetWorld(),Tile->GetActorLocation(),FString::Printf(TEXT("Tile ID: X: %ls %ls"),*FString::SanitizeFloat(OuterIndex,0),*FString::SanitizeFloat(InnerIndex,0)),nullptr,FColor::Blue,5.f,false,1);
			Tile->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
	UE_LOGFMT(LogTemp,Warning,"Chess board has {a} Tiles, {b} Black and {c} White",Tiles.Num(),WhiteTiles.Num(),BlackTiles.Num());
}
void AChessBoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	
}

