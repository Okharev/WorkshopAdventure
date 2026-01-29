#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueNode.h"
#include "DialogueGraph.generated.h"

/**
 * L'asset conteneur pour une conversation.
 */
UCLASS(BlueprintType)
class WORKSHOPADVENTURE_API UDialogueGraph : public UDataAsset
{
	GENERATED_BODY()

public:
	// Instanced permet de créer les nœuds directement "dans" cet asset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Nodes")
	TArray<TObjectPtr<UDialogueNode>> Nodes;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UDialogueNode* GetNode(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UDialogueNode* GetStartNode() const;
};