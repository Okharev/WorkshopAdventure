#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueAsset.h"
#include "DialogueComponent.generated.h"

// Define the delegate for when dialogue ends
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueSegmentFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueRequested, const TArray<FDialogueLine>&, Lines, UDialogueComponent*, Component);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORKSHOPADVENTURE_API UDialogueComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UDialogueComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    
    bool bInFailureState = false;

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnDialogueRequested OnDialogueRequested;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    UDialogueAsset* DialogueAsset;

    // Tracks current progress (0 = first segment)
    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
    int32 CurrentSegmentIndex = 0;

    // Dispatcher for when the UI closes
    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnDialogueSegmentFinished OnDialogueFinished;

    	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Identity")
    FText NPCName;

    // Le portrait du PNJ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Identity")
    UTexture2D* NPCImage;
    
    // Helper to store who we are talking to
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
    APlayerController* CachedPlayerController;

    // Starts the interaction
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StartDialogue(APlayerController* Player);

    // Called by the UI when it closes
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void FinishSegment();
};