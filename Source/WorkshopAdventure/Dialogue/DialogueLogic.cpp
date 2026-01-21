#include "DialogueLogic.h"
#include "DialogueBlackboard.h"

bool UCondition_HasTag::CheckCondition(UDialogueBlackboard* Blackboard) const
{
	if (!Blackboard) return false;
	bool bHasTag = Blackboard->HasTag(TagName);
	return bInvertCondition ? !bHasTag : bHasTag;
}

void UEvent_AddTag::ExecuteEvent(UDialogueBlackboard* Blackboard)
{
	if (Blackboard)
	{
		Blackboard->AddTag(TagName);
	}
}