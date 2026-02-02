#include "DialogueComponent.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this to false unless you actually need to update something every frame
	PrimaryComponentTick.bCanEverTick = false; 
}

// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// DialogueComponent.cpp

void UDialogueComponent::StartDialogue(APlayerController* Player)
{
    if (!DialogueAsset) return;
    CachedPlayerController = Player;

    if (!DialogueAsset->Segments.IsValidIndex(CurrentSegmentIndex)) return;

    FDialogueSegment& Segment = DialogueAsset->Segments[CurrentSegmentIndex];

    // 1. CHECK CONDITIONS
    bool bConditionsMet = true;
    for (UDialogueCondition* Cond : Segment.Conditions)
    {
        // Note: Assure-toi d'utiliser ta version avec "bNegate" si tu l'as ajoutée
        if (Cond && !Cond->IsMet(Player, GetOwner())) 
        {
            bConditionsMet = false;
            break; 
        }
    }

    // --- NOUVELLE LOGIQUE ---
    if (bConditionsMet)
    {
        // Cas : SUCCÈS (J'ai la bière)
        bInFailureState = false;
        OnDialogueRequested.Broadcast(Segment.Lines, this);
    }
    else
    {
        // Cas : ÉCHEC (Pas de bière) -> On joue la ligne de fallback
        if (Segment.FailureLines.Num() > 0)
        {
            bInFailureState = true;
            OnDialogueRequested.Broadcast(Segment.FailureLines, this);
        }
        else
        {
            // Pas de fallback défini ? On ne fait rien ou on log
            UE_LOG(LogTemp, Warning, TEXT("Condition failed and no FailureLines defined."));
        }
    }
    // ------------------------
}

void UDialogueComponent::FinishSegment()
{
    if (!DialogueAsset) return;
    
    // Si on vient de finir un dialogue d'échec ("Je n'ai pas la bière")
    if (bInFailureState)
    {
        // 1. On ferme juste l'UI.
        // 2. IMPORTANT : On NE change PAS CurrentSegmentIndex.
        //    La prochaine fois qu'on parle, on re-testera la condition du MÊME segment.
        // 3. On N'exécute PAS les événements (on ne retire pas la bière qu'on n'a pas).
        
        OnDialogueFinished.Broadcast();
        return; 
    }

    // --- Ci-dessous : Logique standard (Succès) ---

    if (!DialogueAsset->Segments.IsValidIndex(CurrentSegmentIndex)) return;
    FDialogueSegment& Segment = DialogueAsset->Segments[CurrentSegmentIndex];

    // 1. EXECUTE EVENTS (Ex: Remove Item)
    for (UDialogueEvent* Event : Segment.OnFinishedEvents)
    {
        if (Event) Event->ExecuteEvent(CachedPlayerController, GetOwner());
    }

    // 2. ADVANCE INDEX
    if (DialogueAsset->Segments.IsValidIndex(CurrentSegmentIndex + 1))
    {
        CurrentSegmentIndex++;
    }

    // 3. BROADCAST
    OnDialogueFinished.Broadcast();
}
