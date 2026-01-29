// Fill out your copyright notice in the Description page of Project Settings.


#include "UDialogueChoiceWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DialogueComponent.h"

void UUDialogueChoiceWidget::Setup(const int32 Index, const FText& Text, UDialogueComponent* Comp)
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