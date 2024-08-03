// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPieceInterface.h"
#include "GameplayTagContainer.h"
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
	
	UPROPERTY(Replicated)
	FVector2D DefaultBoardID;
	UPROPERTY(Replicated)
	FVector2D CurrentBoardID;
	UPROPERTY(Replicated)
	TArray<AChessTile*> ValidMoves;

	FTimerHandle AnimationTimerHandle;
	virtual void AnimatedTranslation(const FVector& Target);

	void OnPieceCapturedCallback();
public:
	AChessPiece();
protected:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	DECLARE_DELEGATE(FOnPieceCaptured)
	FOnPieceCaptured OnPieceCaptured;
	
	
	virtual void UpdateValidMoves() {}
	const TArray<AChessTile*>& GetValidMoves() const{return ValidMoves;}
	void SetInitBoardID(const FVector2D& BoardID);
	void SetCurrentBoardID(const FVector2D& BoardID);
	bool GetIsWhite()const {return bIsWhite;}
	FGameplayTag GetTeam()const;
	FVector2D GetDefaultBoardID() const {return DefaultBoardID;}

	
	
	UFUNCTION(Server,Reliable)
	void Server_TryMoveTo(class AChessTile* NewPosition);
	UFUNCTION(Server,Reliable)
	void Server_Capture(class AChessPiece* TargetPiece);
};
