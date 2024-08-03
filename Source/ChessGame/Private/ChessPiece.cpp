// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessPiece.h"

#include "ChessGameplayTags.h"
#include "ChessTile.h"
#include "ChessGame/ChessGame.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"



// Sets default values
AChessPiece::AChessPiece()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PieceRoot=CreateDefaultSubobject<USceneComponent>("PieceRoot");
	PieceMesh=CreateDefaultSubobject<UStaticMeshComponent>("PieceMesh");

	RootComponent=PieceRoot;
	PieceMesh->SetupAttachment(GetRootComponent());

	PieceMesh->SetCollisionProfileName("ChessPiece");
	bReplicates=true;

	PrimaryActorTick.bCanEverTick = false;
}

void AChessPiece::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void AChessPiece::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//fix model scale
	if (IsValid(PieceMesh))
	{
		PieceMesh->SetRelativeScale3D({15.f,15.f,15.f,});
	}
}

void AChessPiece::SetInitBoardID(const FVector2D& BoardID)
{
	DefaultBoardID=BoardID;
	SetCurrentBoardID(BoardID);
}

void AChessPiece::SetCurrentBoardID(const FVector2D& BoardID)
{
	CurrentBoardID=BoardID;
}

FGameplayTag AChessPiece::GetTeam() const
{
	return DefaultBoardID.X==0 || DefaultBoardID.X==1? TAG_Team_White:TAG_Team_Black;
}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	OnPieceCaptured.BindUObject(this,&AChessPiece::OnPieceCapturedCallback);
}

void AChessPiece::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnPieceCaptured.Unbind();
}

void AChessPiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChessPiece,ValidMoves)
	DOREPLIFETIME(AChessPiece,DefaultBoardID)
	DOREPLIFETIME(AChessPiece,CurrentBoardID)
}
void AChessPiece::Server_Capture_Implementation(AChessPiece* TargetPiece)
{
	if (TargetPiece->OnPieceCaptured.ExecuteIfBound())
	{
		UE_LOGFMT(LogChessGame,Warning,"ChessPiece {a} captured {b}",GetName(),TargetPiece->GetName());
	}
	
}

void AChessPiece::Server_TryMoveTo_Implementation(AChessTile* NewPosition)
{
	SetCurrentBoardID(NewPosition->GetTileID());
	AnimatedTranslation(NewPosition->GetActorLocation());
	NewPosition->SetChessPiece(this);
	UpdateValidMoves();
}
void AChessPiece::AnimatedTranslation(const FVector& Target)
{
	if (!GetWorld()) return;
	

	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle,FTimerDelegate::CreateWeakLambda(this,[Target,this]()
	{
		FVector StartLoc{GetActorLocation()};
		FVector ToTarget{FMath::VInterpTo( StartLoc,Target,GetWorld()->GetDeltaSeconds(),10.f)};
		
		SetActorLocation(ToTarget);
		if (GetActorLocation().Equals(Target,0.1f))
		{
			GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
		}
		
	}),1.f/60.f,true);
}

void AChessPiece::OnPieceCapturedCallback()
{
	
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle,FTimerDelegate::CreateWeakLambda(this,[this]()
	{
		FVector StartLoc{GetActorLocation()};
		FVector Target{StartLoc+FVector{0.f,0.f,150.f}};
		FVector ToTarget{FMath::VInterpTo( StartLoc,Target,GetWorld()->GetDeltaSeconds(),15.f)};
		
		SetActorLocation(ToTarget);
		if (GetActorLocation().Equals(Target,0.1f))
		{
			GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
			SetLifeSpan(0.1);
		}
		
	}),1.f/60.f,true);
}
