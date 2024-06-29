// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AChessPlayerPawn::AChessPlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PlayerRoot=CreateDefaultSubobject<USceneComponent>("Root");
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComponent=CreateDefaultSubobject<UCameraComponent>("Camera");

	RootComponent=PlayerRoot;
	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	bReplicates=true;
}

// Called when the game starts or when spawned
void AChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AChessPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	
}

