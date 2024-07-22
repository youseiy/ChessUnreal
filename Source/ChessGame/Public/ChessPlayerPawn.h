// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class CHESSGAME_API AChessPlayerPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> PlayerRoot;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;

public:
	// Sets default values for this pawn's properties
	AChessPlayerPawn();

protected:
	virtual void UnPossessed() override;
	virtual void PossessedBy(AController* NewController) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
