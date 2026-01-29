#include "DialogueNode.h"
#include "DialogueGraph.h" // Nécessaire pour connaître le type UDialogueGraph

UDialogueGraph* UDialogueNode::GetGraph() const
{
	return Cast<UDialogueGraph>(GetOuter());
}