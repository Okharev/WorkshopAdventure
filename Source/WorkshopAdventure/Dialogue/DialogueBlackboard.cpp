#include "DialogueBlackboard.h"

void UDialogueBlackboard::AddTag(FName Tag)
{
	ActiveTags.Add(Tag);
}

void UDialogueBlackboard::RemoveTag(FName Tag)
{
	ActiveTags.Remove(Tag);
}

bool UDialogueBlackboard::HasTag(FName Tag) const
{
	return ActiveTags.Contains(Tag);
}
