﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "Pawn_.generated.h"

UCLASS()
class CHESSGAME_API APawn_ : public AChessPiece, public IChessPieceInterface
{
	GENERATED_BODY()



	virtual TArray<FVector> GetValidMoves() const override;
	virtual void MoveTo(const FVector& NewPosition) override;
	virtual void Capture(class AChessPiece* TargetPiece) override;
	virtual bool IsMoveValid(const FVector& NewPosition) const override;
public:
	// Sets default values for this actor's properties
	APawn_();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};