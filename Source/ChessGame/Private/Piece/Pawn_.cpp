// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/Pawn_.h"


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

TArray<FVector> APawn_::GetValidMoves() const
{
	return {};
}

void APawn_::MoveTo(const FVector& NewPosition)
{
	
}

void APawn_::Capture(AChessPiece* TargetPiece)
{
	
}

bool APawn_::IsMoveValid(const FVector& NewPosition) const
{
	return false;
}

