#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueGraph.h" // Nécessaire pour connaître les types de nœuds
#include "DialogueComponent.generated.h"

// Delegates pour communiquer avec le Widget Blueprint (UI)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueUpdated, FText, SpeakerName, FText, DialogueText, const TArray<FDialogueChoice>&, Choices);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORKSHOPADVENTURE_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogueComponent();
	
	// UPROPERTY(EditAnywhere, Category = "Debug")
	// bool bDebugMode = true;

	// Le Graph principal assigné à ce NPC (le "Hub" ou "Root")
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDialogueGraph> DialogueGraph;

	// Appelé quand le dialogue affiche une nouvelle ligne
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueUpdated OnDialogueUpdated;

	// Appelé quand la conversation se termine
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

	// Lance la conversation (utilise le DialogueGraph par défaut)
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void QuitDialogue();

	// Surcharge : Lance un Graph spécifique (utile pour les transitions)
	void StartDialogue(UDialogueGraph* NewGraph);

	// Appelé par l'UI quand le joueur clique sur un bouton
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectChoice(int32 ChoiceIndex);

	void Debug_DisplayCurrentNode(const FText& Speaker, const FText& Text, const TArray<FDialogueChoice>& Choices);
	
protected:
	UPROPERTY(Transient)
	TObjectPtr<UDialogueNode> CurrentNode;

	void SetCurrentNode(UDialogueNode* NewNode);
	
	// Filtre les choix selon les conditions du Blackboard
	TArray<FDialogueChoice> GetValidChoices(UDialogueNode* Node);
};