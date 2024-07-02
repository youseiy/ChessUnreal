// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "Queen.generated.h"

UCLASS()
class CHESSGAME_API AQueen : public AChessPiece
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQueen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void UpdateValidMoves() final override;
};
