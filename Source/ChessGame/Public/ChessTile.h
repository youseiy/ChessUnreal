// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "GameFramework/Actor.h"
#include "ChessTile.generated.h"

class AChessPiece;

UCLASS()
class CHESSGAME_API AChessTile : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> TileRoot;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> TileMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> PlaneShaderMesh;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TObjectPtr<UMaterialInstance> RedMaterialInstance;
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TObjectPtr<UMaterialInstance> GreenMaterialInstance;
	
	FVector2D BoardID;
	
	UPROPERTY()
	AChessPiece* ChessPiece;
	
public:
	// Sets default values for this actor's properties
	AChessTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	AChessPiece* GetChessPiece()const {return ChessPiece;}
	void SetChessPiece(AChessPiece* Piece){ChessPiece=Piece;}
	void SetBoardID(const FVector2D& ID);
	void ShowShader();
	FVector2D GetTileID()const {return BoardID;}
	void DisableShader();
};
