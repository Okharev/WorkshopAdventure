#pragma once

#include "DialogueCondition.generated.h"

UCLASS(Abstract, EditInlineNew, DefaultToInstanced, BlueprintType, Blueprintable)
class WORKSHOPADVENTURE_API UDialogueCondition : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
	bool bNegate = false;
	
	// Returns true if the condition is met	x
	UFUNCTION(BlueprintNativeEvent)
	bool IsMet(const APlayerController* Player, const AActor* NPC);
};

// Base class for things that happen (e.g., GiveItem, OpenDoor)
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, BlueprintType, Blueprintable)
class WORKSHOPADVENTURE_API UDialogueEvent : public UObject
{
	GENERATED_BODY()
public:
	
	// Executes the effect
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteEvent(APlayerController* Player, AActor* NPC);
};