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
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PieceRoot;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> PieceMesh;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	bool bIsWhite=false;
	//todo:make this work
	
	FVector2D InitBoardID;
	FVector2D CurrentBoardID;
public:
	AChessPiece();
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	void SetInitBoardID(const FVector2D& BoardID)
	{
		InitBoardID=BoardID;
		SetCurrentBoardID(BoardID);
	};
	void SetCurrentBoardID(const FVector2D& BoardID)
	{
		CurrentBoardID=BoardID;
	};
	bool GetIsWhite()const {return bIsWhite;}
	void OnSelected();
	void OnDeselected();
	void Promote(TSubclassOf<AChessPiece> NewPieceClass);

	
};
