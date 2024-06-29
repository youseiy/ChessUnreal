// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessTile.generated.h"

UCLASS()
class CHESSGAME_API AChessTile : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> TileMesh;

	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess))
	uint8 BoardID;

public:
	// Sets default values for this actor's properties
	AChessTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

};
