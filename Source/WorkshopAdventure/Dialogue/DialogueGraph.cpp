// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraph.h"

UDialogueNode* UDialogueGraph::GetNode(int32 Index) const
{
	if (Nodes.IsValidIndex(Index))
	{
		return Nodes[Index];
	}
	return nullptr;
}

UDialogueNode* UDialogueGraph::GetStartNode() const
{
	return GetNode(0);
}
