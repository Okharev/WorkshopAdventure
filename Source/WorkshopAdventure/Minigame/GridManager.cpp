#include "GridManager.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h" // IMPORTANT


AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. Create Root
    USceneComponent* RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootScene;

    // 2. Create Interaction Zone (Trigger Box)
    InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
    InteractionZone->SetupAttachment(RootComponent);
    InteractionZone->SetBoxExtent(FVector(200, 200, 100));
    InteractionZone->SetCollisionProfileName(TEXT("Trigger"));

    // 3. Create Camera
    TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCamera->SetupAttachment(RootComponent);
    // Default position: High up, looking down
    TopDownCamera->SetRelativeLocation(FVector(500, 400, 800)); 
    TopDownCamera->SetRelativeRotation(FRotator(-90, -90, 0));

    bIsGameActive = false;
}


void AGridManager::EnterMiniGame()
{
    if (bIsGameActive) return;
    bIsGameActive = true;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        // 1. Get the Enhanced Input Local Player Subsystem
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
            {
                // REMOVE the default controls (Stop moving, stop inventory)
                if (DefaultMappingContext)
                {
                    Subsystem->RemoveMappingContext(DefaultMappingContext);
                }

                // ADD the minigame controls (Allow Pause, Allow Exit)
                if (MinigameMappingContext)
                {
                    // Priority 1 ensures it overrides anything else if needed
                    Subsystem->AddMappingContext(MinigameMappingContext, 1); 
                }
            }
        }

        // 2. Set Input Mode to Game And UI
        // We need 'Game' so the 'Pause' key still works, and 'UI' for the buttons.
        PC->SetShowMouseCursor(true);
        FInputModeGameAndUI InputMode;
        
        // Lock mouse to viewport so they don't click outside
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        
        // Ensure UI buttons get priority
        if (GameUIClass) 
        {
            // If we nulled it in Exit, this will run again, creating a fresh HUD with fresh button states
            if (!GameUIInstance)
            {
                GameUIInstance = CreateWidget<UUserWidget>(PC, GameUIClass);
            }

            if (GameUIInstance)
            {
                GameUIInstance->AddToViewport();
                
                // Focus logic...
                PC->SetShowMouseCursor(true);
                FInputModeGameAndUI InputModes;
                InputModes.SetWidgetToFocus(GameUIInstance->TakeWidget());
                PC->SetInputMode(InputModes);
            }
        }
        else
        {
            // Debug Message if you forgot to set the BP variable
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: GameUIClass is NONE in BP_GridManager!"));
        }
        
        PC->SetInputMode(InputMode);

        // 3. Switch Camera
        PC->SetViewTargetWithBlend(this, 1.0f);
    }
}

void AGridManager::ExitMiniGame()
{
    if (!bIsGameActive) return;
    bIsGameActive = false;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        // 1. Revert Input Contexts
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
            {
                // Remove Minigame controls
                if (MinigameMappingContext)
                {
                    Subsystem->RemoveMappingContext(MinigameMappingContext);
                }

                // Restore Default controls (Movement comes back!)
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }

        // 2. Revert Input Mode
        PC->SetShowMouseCursor(false); // Or true if your game usually has a cursor
        PC->SetInputMode(FInputModeGameOnly());

        // 3. Restore Camera
        if (PC->GetPawn())
        {
            PC->SetViewTargetWithBlend(PC->GetPawn(), 1.0f);
        }
        
        // 4. Clean UI
        if (GameUIInstance)
        {
            GameUIInstance->RemoveFromParent();
            GameUIInstance = nullptr;
        }
    }
}

void AGridManager::HandlePostMove()
{
    // Sync Visuals
    SyncVisualProperties();
    
    // Check Win
    if (CheckWinCondition())
    {
        // Add a small delay or UI message before exiting
        // For now, let's just exit
        ExitMiniGame();
        
        // You could also spawn a reward here!
    }
}


// ==========================================
// DATA & GRID SETUP
// ==========================================

void AGridManager::InitializeGrid(const TArray<ECardType>& Pattern)
{
    InitialPattern = Pattern; 

    // Reset Flags
    bKnightUsed = false;
    bTowerUsed = false;
    bJokerUsed = false;
    bServantUsed = false;

    
    GridData.Empty();
    int32 TotalSize = GridWidth * GridHeight;

    for (int32 i = 0; i < TotalSize; i++)
    {
        FCardData NewCard;
        // Use pattern if provided, otherwise default to Servant
        NewCard.Type = (Pattern.IsValidIndex(i)) ? Pattern[i] : ECardType::Servant;
        NewCard.bHasCoin = false;
        NewCard.UniqueID = i; 
        GridData.Add(NewCard);
    }
}

void AGridManager::ResetGame()
{
    // Re-run initialization with the saved pattern
    InitializeGrid(InitialPattern);
    
    // Refresh the visuals
    CreateGridVisuals();
    
    // Note: We don't check win condition here because we just reset.
}

void AGridManager::CreateGridVisuals()
{
    if (!CardActorClass) return;

    // 1. Cleanup old actors
    for (ACardActor* Actor : CardInstances)
    {
        if (Actor) Actor->Destroy();
    }
    CardInstances.Empty();

    // 2. Spawn new ones
    FVector StartLocation = GetActorLocation();

    for (int32 i = 0; i < GridData.Num(); i++)
    {
        int32 X = i % GridWidth;
        int32 Y = i / GridWidth;
        
        // Calculate Position
        FVector SpawnPos = StartLocation + FVector(X * CardSpacing, Y * CardSpacing, 0.0f);
        FRotator SpawnRot = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        // Spawn using our Specific ACardActor class
        ACardActor* NewCard = GetWorld()->SpawnActor<ACardActor>(CardActorClass, SpawnPos, SpawnRot, SpawnParams);
        
        if (NewCard)
        {
            // Set initial target so it doesn't slide from 0,0,0
            NewCard->SetTargetPosition(SpawnPos);
            CardInstances.Add(NewCard);
        }
    }

    // 3. Apply Initial Visuals (Materials/Coins)
    SyncVisualProperties();
}

// ==========================================
// SPELL IMPLEMENTATIONS
// ==========================================

bool AGridManager::TryCastKnightSpell()
{
    if (bKnightUsed) return false;
    
    // Constraint: Cannot cast if any Knight is at Y=0 (Up invalid) or Y=Max (Down invalid)
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 i = GetIndex(x, y);
            if (GridData[i].Type == ECardType::Knight)
            {
                if (!IsValidCoord(x, y - 1) || !IsValidCoord(x, y + 1)) return false;
            }
        }
    }

    // Execution
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 i = GetIndex(x, y);
            if (GridData[i].Type == ECardType::Knight)
            {
                ToggleCoin(i);                  // Self
                ToggleCoin(GetIndex(x, y - 1)); // Above
                ToggleCoin(GetIndex(x, y + 1)); // Below
            }
        }
    }

    bKnightUsed = true;
    
    SyncActorPositions(); // Just in case
    HandlePostMove();
    return true;
}

bool AGridManager::TryCastTowerSpell()
{
    if (bTowerUsed) return false;
    
    // Constraint: Cannot cast if Tower is on Right Edge
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 i = GetIndex(x, y);
            if (GridData[i].Type == ECardType::Tower)
            {
                if (!IsValidCoord(x + 1, y)) return false;
            }
        }
    }

    // Execution
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 i = GetIndex(x, y);
            if (GridData[i].Type == ECardType::Tower)
            {
                ToggleCoin(i);                 // Self
                ToggleCoin(GetIndex(x + 1, y)); // Right
            }
        }
    }
    
    bTowerUsed = true;

    SyncActorPositions(); // Just in case
    HandlePostMove();
    return true;
}

bool AGridManager::TryCastJokerSpell()
{
    if (bJokerUsed) return false;

    
    // Constraint: Cannot cast if Joker is on Left Edge
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 i = GetIndex(x, y);
            if (GridData[i].Type == ECardType::Joker)
            {
                if (!IsValidCoord(x - 1, y)) return false;
            }
        }
    }

    // Execution: Swap Data AND Visuals
    // We iterate Left->Right to avoid double processing the same card
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 CurrentIdx = GetIndex(x, y);
            
            // Logic Trigger
            if (GridData[CurrentIdx].Type == ECardType::Joker)
            {
                int32 LeftIdx = GetIndex(x - 1, y);

                // 1. Swap the Logic Data (includes coins)
                GridData.Swap(CurrentIdx, LeftIdx);

                // 2. Swap the Actor References 
                // (So the Joker Actor moves to index X-1 in the list)
                CardInstances.Swap(CurrentIdx, LeftIdx);
            }
        }
    }
    
    bJokerUsed = true;

    SyncActorPositions(); // Just in case
    HandlePostMove();
    
    return true;
}

bool AGridManager::TryCastServantSpell()
{
    if (bServantUsed) return false;

    
    // No constraint mentioned, always succeeds
    for (int32 i = 0; i < GridData.Num(); i++)
    {
        if (GridData[i].Type == ECardType::Servant)
        {
            ToggleCoin(i);
        }
    }

    bServantUsed = true;
    
    SyncActorPositions(); // Just in case
    HandlePostMove();
    return true;
}

bool AGridManager::CheckWinCondition() const
{
    if (GridData.Num() == 0) return false;

    for (const FCardData& Card : GridData)
    {
        if (!Card.bHasCoin) return false;
    }
    return true;
}

// ==========================================
// HELPERS
// ==========================================

bool AGridManager::IsValidCoord(int32 X, int32 Y) const
{
    return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

int32 AGridManager::GetIndex(int32 X, int32 Y) const
{
    return Y * GridWidth + X;
}

void AGridManager::ToggleCoin(int32 Index)
{
    if (GridData.IsValidIndex(Index))
    {
        GridData[Index].bHasCoin = !GridData[Index].bHasCoin;
    }
}

// ==========================================
// VISUAL UPDATES
// ==========================================

void AGridManager::SyncVisualProperties()
{
    if (CardInstances.Num() != GridData.Num()) return;

    for (int32 i = 0; i < GridData.Num(); i++)
    {
        ACardActor* Card = CardInstances[i];
        if (Card)
        {
            // Call the Blueprint Implementable Event on the card
            Card->UpdateCardVisuals(GridData[i].Type, GridData[i].bHasCoin);
        }
    }
}

void AGridManager::SyncActorPositions()
{
    FVector StartLocation = GetActorLocation();

    for (int32 i = 0; i < CardInstances.Num(); i++)
    {
        ACardActor* Card = CardInstances[i];
        if (Card)
        {
            // Calculate where this card SHOULD be now
            int32 X = i % GridWidth;
            int32 Y = i / GridWidth;
            FVector TargetPos = StartLocation + FVector(X * CardSpacing, Y * CardSpacing, 0.0f);

            // Trigger the C++ smooth slide in ACardActor
            Card->SetTargetPosition(TargetPos);
        }
    }
}