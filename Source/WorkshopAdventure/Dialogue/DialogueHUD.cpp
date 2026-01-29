#include "DialogueHUD.h"

void UDialogueHUD::	InitializeDialogue(UDialogueComponent* Comp)
{
	if (!Comp) return;
	LinkedComponent = Comp;

	Comp->OnDialogueUpdated.AddDynamic(this, &UDialogueHUD::OnDialogueUpdated);
	Comp->OnDialogueEnded.AddDynamic(this, &UDialogueHUD::OnDialogueEnded);

	// Binding du bouton Quitter ---
	if (QuitButton)
	{
		// On nettoie d'abord au cas où (bonne pratique)
		QuitButton->OnClicked.RemoveDynamic(this, &UDialogueHUD::OnQuitClicked);
		QuitButton->OnClicked.AddDynamic(this, &UDialogueHUD::OnQuitClicked);
	}
	
    // Configuration input pour la souris
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
	
		PC->bShowMouseCursor = true;
	}
}

void UDialogueHUD::OnDialogueUpdated(FText Speaker, FText Text, const TArray<FDialogueChoice>& Choices)
{
	// 1. Mise à jour du texte
	if (SpeakerNameText) SpeakerNameText->SetText(Speaker);
	if (DialogueContentText) DialogueContentText->SetText(Text);

	// 2. Nettoyage des anciens boutons
	if (ChoicesContainer)
	{
		ChoicesContainer->ClearChildren();
	}

	// 3. Création des nouveaux boutons
	if (ChoiceWidgetClass && ChoicesContainer && LinkedComponent.IsValid())
	{
		for (int32 i = 0; i < Choices.Num(); i++)
		{
            // CreateWidget est l'équivalent C++ du nœud BP "Create Widget"

            if (UUDialogueChoiceWidget* NewBtn = CreateWidget<UUDialogueChoiceWidget>(this, ChoiceWidgetClass))
			{
				NewBtn->Setup(i, Choices[i].ChoiceText, LinkedComponent.Get());
				ChoicesContainer->AddChild(NewBtn);
			}
		}
	}
}

void UDialogueHUD::OnDialogueEnded()
{
	// --- Nettoyage ---
    
    // On se désabonne pour éviter les crashs si le HUD est détruit avant le Component
	if (LinkedComponent.IsValid())
	{
		LinkedComponent->OnDialogueUpdated.RemoveDynamic(this, &UDialogueHUD::OnDialogueUpdated);
		LinkedComponent->OnDialogueEnded.RemoveDynamic(this, &UDialogueHUD::OnDialogueEnded);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}

	RemoveFromParent();
}

void UDialogueHUD::OnQuitClicked()
{
	if (LinkedComponent.IsValid())
	{
		LinkedComponent->QuitDialogue();
	}
}