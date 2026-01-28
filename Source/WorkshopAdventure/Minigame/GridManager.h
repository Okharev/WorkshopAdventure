// GridManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardGameTypes.h"
#include "CardActor.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"
#include "GridManager.generated.h"

UCLASS()
class WORKSHOPADVENTURE_API AGridManager : public AActor
{
    GENERATED_BODY()

public:
    AGridManager();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    bool bKnightUsed = false;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    bool bTowerUsed = false;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    bool bJokerUsed = false;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    bool bServantUsed = false;
    
    TArray<ECardType> InitialPattern;
    
    // --- Existing Config ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Config")
    int32 GridWidth = 5;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Config")
    int32 GridHeight = 3;
    UPROPERTY(EditAnywhere, Category = "Grid Visuals")
    float CardSpacing = 25.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Grid Visuals")
    TSubclassOf<ACardActor> CardActorClass;

    // --- Interaction Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* InteractionZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* TopDownCamera;

    // --- UI Config ---
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameUIClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "UI")
    UUserWidget* GameUIInstance;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    // The Minigame Context (Only Pause, maybe Exit)
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* MinigameMappingContext;
    
    // --- State Data ---
    UPROPERTY()
    TArray<FCardData> GridData;
    UPROPERTY()
    TArray<ACardActor*> CardInstances;
    
    bool bIsGameActive;

public:
    // --- Core Functions ---
    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    void InitializeGrid(const TArray<ECardType>& Pattern);

    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    void CreateGridVisuals();

    // --- NEW: Interaction Functions ---
    // Called when Player presses E
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void EnterMiniGame();

    // Called when Win or Exit
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ExitMiniGame();

    // --- Spells (Updated to check Win) ---
    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    bool TryCastKnightSpell(); // Changed from bool to void for simplicity in binding

    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    bool TryCastTowerSpell();

    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    bool TryCastJokerSpell();

    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    bool TryCastServantSpell();
    
    
    UFUNCTION(BlueprintCallable, Category = "Game Logic")
    void ResetGame();

private:
    bool IsValidCoord(int32 X, int32 Y) const;
    int32 GetIndex(int32 X, int32 Y) const;
    void ToggleCoin(int32 Index);
    void SyncVisualProperties();
    void SyncActorPositions();
    bool CheckWinCondition() const;
    
    // Helper to handle the win logic
    void HandlePostMove();
};