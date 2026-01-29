#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueLogic.h" 
#include "DialogueNode.generated.h"

class UDialogueCondition;
class UDialogueEvent;
class UDialogueGraph; // Forward declaration essentielle

/**
 * Représente un lien/choix vers un autre nœud ou un autre Graph.
 */
USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText ChoiceText;

	UPROPERTY(EditAnywhere, Instanced, Category = "Logic")
	TArray<TObjectPtr<UDialogueCondition>> Conditions;

	UPROPERTY(EditAnywhere, Instanced, Category = "Logic")
	TArray<TObjectPtr<UDialogueEvent>> Events;

	// Si > -1, on va vers ce nœud dans le graph actuel
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flow")
	int32 TargetNodeIndex = -1;

	// NOUVEAU : Si assigné, on ferme le graph actuel et on charge celui-ci
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flow")
	TObjectPtr<UDialogueGraph> TargetGraph;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class WORKSHOPADVENTURE_API UDialogueNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta=(MultiLine=true))
	FText SpeakerName;
	
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta=(MultiLine=true))
	FText DialogueText;

	// Si vrai, le composant choisira automatiquement le premier choix valide sans attendre le joueur
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flow")
	bool bIsAutoRouter = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flow")
	TArray<FDialogueChoice> Choices;

	UDialogueGraph* GetGraph() const;
};