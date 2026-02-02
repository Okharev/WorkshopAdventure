#include "DialogueAsset.h" // Ensure the header is included

bool UDialogueCondition::IsMet_Implementation(const APlayerController* Player, const AActor* NPC)
{
	// Default behavior: Return true so dialogue proceeds if no condition logic is added.
	return true; 
}

void UDialogueEvent::ExecuteEvent_Implementation(APlayerController* Player, AActor* NPC)
{
	// Default behavior: Do nothing.
	// This allows you to have empty events without crashing.
}