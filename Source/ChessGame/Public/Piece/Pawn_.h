// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "Pawn_.generated.h"

class AChessTile;

UCLASS()
class CHESSGAME_API APawn_ : public AChessPiece, public IChessPieceInterface
{
	GENERATED_BODY()

	bool bAlreadyMoved=false;
	

	virtual void UpdateValidMoves() override;
	virtual void MoveTo(AChessTile* NewPosition) override;
	virtual void Capture(class AChessPiece* TargetPiece) override;
	virtual bool IsMoveValid(const FVector& NewPosition) const override;

	virtual void Server_TryMoveTo_Implementation(AChessTile* NewPosition) override;

	
public:
	// Sets default values for this actor's properties
	APawn_();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
