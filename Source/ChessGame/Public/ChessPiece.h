// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

UCLASS()
class CHESSGAME_API AChessPiece : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PieceRoot;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> PieceMesh;

	UPROPERTY(EditAnywhere)
	uint8 InitBoardID;
	
	uint8 CurrentBoardID;
	

public:
	// Sets default values for this actor's properties
	AChessPiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:
	
};
