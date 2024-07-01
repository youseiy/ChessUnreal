// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessTile.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AChessTile::AChessTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TileRoot=CreateDefaultSubobject<USceneComponent>("TileRoot");
	TileMesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	PlaneShaderMesh=CreateDefaultSubobject<UStaticMeshComponent>("PlaneShaderMesh");
	
	RootComponent=TileRoot;
	TileMesh->SetupAttachment(GetRootComponent());
	PlaneShaderMesh->SetupAttachment(GetRootComponent());

	TileMesh->SetCollisionProfileName("ChessTile");
	
	PlaneShaderMesh->SetVisibility(false);
	
	bReplicates=true;
}

void AChessTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChessTile,BoardID)
	DOREPLIFETIME(AChessTile,ChessPiece)
}

// Called when the game starts or when spawned
void AChessTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessTile::SetBoardID(const FVector2D& ID)
{
	BoardID=ID;
}

void AChessTile::ShowShader()
{
	PlaneShaderMesh->SetVisibility(true);
	PlaneShaderMesh->SetMaterial(0,GreenMaterialInstance);
}

void AChessTile::DisableShader()
{
	PlaneShaderMesh->SetVisibility(false);
}



