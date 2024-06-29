﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessTile.h"


// Sets default values
AChessTile::AChessTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TileMesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent=TileMesh;


	SetReplicates(true);
}

// Called when the game starts or when spawned
void AChessTile::BeginPlay()
{
	Super::BeginPlay();
	
}



