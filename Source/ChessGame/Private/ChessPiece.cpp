// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessPiece.h"


// Sets default values
AChessPiece::AChessPiece()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PieceRoot=CreateDefaultSubobject<USceneComponent>("PieceRoot");

	PieceMesh=CreateDefaultSubobject<UStaticMeshComponent>("PieceMesh");

	RootComponent=PieceRoot;
	PieceMesh->SetupAttachment(GetRootComponent());


	SetReplicates(true);
}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessPiece::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//fix piece scale
	if (IsValid(PieceMesh))
	{
		PieceMesh->SetRelativeScale3D({15.f,15.f,15.f,});
	}
}
