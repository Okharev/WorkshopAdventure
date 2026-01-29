#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueLogic.generated.h"

class UDialogueBlackboard;

// --- Classe de base Condition ---
// EditInlineNew ici dit "On peut créer des instance de cette famille dans l'éditeur"
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, CollapseCategories, BlueprintType)
class WORKSHOPADVENTURE_API UDialogueCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(UDialogueBlackboard* Blackboard) const { return true; }
};

// --- Classe de base Événement ---
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, CollapseCategories, BlueprintType)
class WORKSHOPADVENTURE_API UDialogueEvent : public UObject
{
	GENERATED_BODY()

public:
	virtual void ExecuteEvent(UDialogueBlackboard* Blackboard) {}
};

// --- CORRECTION ICI : Ajouter "EditInlineNew" ---
UCLASS(DisplayName = "Has Gameplay Tag", EditInlineNew, BlueprintType)
class WORKSHOPADVENTURE_API UCondition_HasTag : public UDialogueCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Logic")
	FName TagName;

	UPROPERTY(EditAnywhere, Category = "Logic")
	bool bInvertCondition = false;

	virtual bool CheckCondition(UDialogueBlackboard* Blackboard) const override;
};

UCLASS(DisplayName = "Add Gameplay Tag", EditInlineNew, BlueprintType)
class WORKSHOPADVENTURE_API UEvent_AddTag : public UDialogueEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Logic")
	FName TagName;

	virtual void ExecuteEvent(UDialogueBlackboard* Blackboard) override;
};