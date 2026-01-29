#include "DialogueComponent.h"
#include "DialogueBlackboard.h"
#include "Kismet/GameplayStatics.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::StartDialogue()
{
	StartDialogue(DialogueGraph);
}

void UDialogueComponent::QuitDialogue()
{
	SetCurrentNode(nullptr);
}

void UDialogueComponent::StartDialogue(UDialogueGraph* NewGraph)
{
	if (NewGraph)
	{
		// Mise à jour du pointeur pour que le composant sache où il est
		DialogueGraph = NewGraph;
		SetCurrentNode(DialogueGraph->GetStartNode());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogueComponent: Tentative de lancer un graph null."));
		OnDialogueEnded.Broadcast();
	}
}

void UDialogueComponent::SetCurrentNode(UDialogueNode* NewNode)
{
	CurrentNode = NewNode;
	
	if (CurrentNode) {
		UE_LOG(LogTemp, Warning, TEXT("Dialogue Started: %s"), *CurrentNode->DialogueText.ToString());
	} else {
		UE_LOG(LogTemp, Error, TEXT("CurrentNode is NULL!"));
	}

	if (!CurrentNode)
	{
		OnDialogueEnded.Broadcast();
		// Debug : Annoncer la fin
		// if (bDebugMode && GEngine)
		// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("--- FIN DU DIALOGUE ---"));
		return;
	}

	// 1. Filtrer les choix disponibles
	const TArray<FDialogueChoice> ValidChoices = GetValidChoices(CurrentNode);

	// 2. Gestion de l'Auto-Router (Nœud de redirection sans UI)
	if (CurrentNode->bIsAutoRouter && ValidChoices.Num() > 0)
	{
		// On prend le premier choix valide automatiquement
		// Note : On passe 0 car c'est l'index dans le tableau FILTRÉ
		SelectChoice(0); 
		return;
	}

	// 3. Mise à jour de l'UI
	OnDialogueUpdated.Broadcast(CurrentNode->SpeakerName, CurrentNode->DialogueText, ValidChoices);
	
	// if (bDebugMode)
	// {
	// 	Debug_DisplayCurrentNode(CurrentNode->SpeakerName, CurrentNode->DialogueText, ValidChoices);
	// }
}

void UDialogueComponent::SelectChoice(int32 ChoiceIndex)
{
	if (!CurrentNode) return;

	// On doit recalculer les choix valides pour être sûr que l'index UI correspond
	TArray<FDialogueChoice> ValidChoices = GetValidChoices(CurrentNode);

	if (!ValidChoices.IsValidIndex(ChoiceIndex))
	{
		OnDialogueEnded.Broadcast();
		return;
	}

	const FDialogueChoice& SelectedChoice = ValidChoices[ChoiceIndex];

	// --- Exécution des Événements ---
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);

	if (UDialogueBlackboard* Blackboard = GameInstance ? GameInstance->GetSubsystem<UDialogueBlackboard>() : nullptr)
	{
		for (UDialogueEvent* Event : SelectedChoice.Events)
		{
			if (Event) Event->ExecuteEvent(Blackboard);
		}
	}

	// --- Navigation ---
	
	// Cas A : Saut vers un autre Graph (Hub & Spoke)
	if (SelectedChoice.TargetGraph)
	{
		StartDialogue(SelectedChoice.TargetGraph);
		return;
	}

	// Cas B : Navigation normale dans le même Graph
	// you are an expert in unreal engine 5 
	UDialogueNode* NextNode = CurrentNode->GetGraph()->GetNode(SelectedChoice.TargetNodeIndex);
	SetCurrentNode(NextNode);
}

TArray<FDialogueChoice> UDialogueComponent::GetValidChoices(UDialogueNode* Node)
{
	TArray<FDialogueChoice> FilteredChoices;
	if (!Node) return FilteredChoices;

	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UDialogueBlackboard* Blackboard = GameInstance ? GameInstance->GetSubsystem<UDialogueBlackboard>() : nullptr;

	for (const FDialogueChoice& Choice : Node->Choices)
	{
		bool bIsConditionMet = true;

		// Vérification des conditions
		for (UDialogueCondition* Condition : Choice.Conditions)
		{
			if (Condition && !Condition->CheckCondition(Blackboard))
			{
				bIsConditionMet = false;
				break;
			}
		}

		if (bIsConditionMet)
		{
			FilteredChoices.Add(Choice);
		}
	}

	return FilteredChoices;
}

void UDialogueComponent::Debug_DisplayCurrentNode(const FText& Speaker, const FText& Text, const TArray<FDialogueChoice>& Choices)
{
	if (!GEngine) return;

	// 1. Afficher le nom (Vert)
	//	
	// 
	// 
	const FString Header = FString::Printf(TEXT("SPEAKING: %s"), *Speaker.ToString());
	GEngine->AddOnScreenDebugMessage(1, 100.f, FColor::Green, Header);
	
	// 2. Afficher le texte (Blanc)
	GEngine->AddOnScreenDebugMessage(2, 100.f, FColor::White, Text.ToString());

	// 3. Afficher les choix (Jaune)
	for (int32 i = 0; i < Choices.Num(); i++)
	{
		FString ChoiceStr = FString::Printf(TEXT("[%d] -> %s"), i + 1, *Choices[i].ChoiceText.ToString());
		// La clé (3 + i) permet d'avoir une ligne par choix sans qu'elles s'écrasent
		GEngine->AddOnScreenDebugMessage(3 + i, 100.f, FColor::Yellow, ChoiceStr);
	}
}