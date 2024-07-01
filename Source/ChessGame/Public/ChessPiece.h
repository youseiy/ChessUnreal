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
	UPROPERTY(Replicated)
	FVector2D InitBoardID;
	UPROPERTY(Replicated)
	FVector2D CurrentBoardID;
	UPROPERTY(Replicated)
	TArray<AChessTile*> ValidMoves;
public:
	AChessPiece();
protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	virtual void UpdateValidMoves() {}
	const TArray<AChessTile*>& GetValidMoves() const{return ValidMoves;}
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

	
	
	UFUNCTION(Server,Reliable)
	void Server_TryMoveTo(class AChessTile* NewPosition);
	UFUNCTION(Server,Reliable)
	void Server_Capture(class AChessPiece* TargetPiece);
};
