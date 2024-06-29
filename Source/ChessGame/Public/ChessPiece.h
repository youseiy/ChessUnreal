// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPieceInterface.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

UCLASS(Abstract)
class CHESSGAME_API AChessPiece : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PieceRoot;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> PieceMesh;

	//todo:make this work
	UPROPERTY(EditAnywhere)
	uint8 InitBoardID;
	uint8 CurrentBoardID;
	
public:
	AChessPiece();
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	void OnSelected();
	void OnDeselected();
	void Promote(TSubclassOf<AChessPiece> NewPieceClass);

	
};
