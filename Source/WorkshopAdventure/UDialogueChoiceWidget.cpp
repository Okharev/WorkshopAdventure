// Fill out your copyright notice in the Description page of Project Settings.


#include "UDialogueChoiceWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Dialogue/DialogueComponent.h"

void UUDialogueChoiceWidget::Setup(int32 Index, FText Text, UDialogueComponent* Comp)
{
	ChoiceIndex = Index;
	DialogueComponentRef = Comp;

	if (ChoiceTextLabel)
	{
		ChoiceTextLabel->SetText(Text);
	}
}

void UUDialogueChoiceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChoiceButton)
	{
		// Liaison de l'événement clic du bouton
		ChoiceButton->OnClicked.AddDynamic(this, &UUDialogueChoiceWidget::OnButtonClicked);
	}
}

void UUDialogueChoiceWidget::OnButtonClicked()
{
	if (DialogueComponentRef.IsValid())
	{
		// C'est ici qu'on appelle la fonction C++ du composant
		DialogueComponentRef->SelectChoice(ChoiceIndex);
	}
}