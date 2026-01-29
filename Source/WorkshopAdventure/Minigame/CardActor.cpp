#include "CardActor.h"

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = true; // We need Tick for movement

	// 1. Create the Hierarchy
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	CardMesh->SetupAttachment(RootScene);

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	CoinMesh->SetupAttachment(CardMesh); // Attached to card so it moves with it
    
	// Default settings
	CoinMesh->SetRelativeLocation(FVector(0, 0, 10.0f)); // Slightly above card
	CoinMesh->SetVisibility(false); // Hide by default

	MoveSpeed = 10.0f;
	bIsMoving = false;
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();
	TargetLocation = GetActorLocation();
}

void ACardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Simple smooth movement logic
	if (bIsMoving)
	{
		FVector CurrentLoc = GetActorLocation();
        
		// VInterpTo creates a smooth slide from A to B
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLocation, DeltaTime, MoveSpeed);
        
		SetActorLocation(NewLoc);

		// Snap to finish if close enough
		if (FVector::DistSquared(NewLoc, TargetLocation) < 1.0f)
		{
			SetActorLocation(TargetLocation);
			bIsMoving = false;
		}
	}
}

void ACardActor::SetTargetPosition(FVector NewPosition)
{
	TargetLocation = NewPosition;
	bIsMoving = true;
}