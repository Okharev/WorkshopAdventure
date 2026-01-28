#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DialogueComponent.h"
#include "UDialogueChoiceWidget.generated.h"

UCLASS()
class WORKSHOPADVENTURE_API UUDialogueChoiceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Fonction d'initialisation (remplace le "Expose on Spawn")
	void Setup(int32 Index, const FText& Text, UDialogueComponent* Comp);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

protected:
	// "BindWidget" relie cette variable au bouton créé dans l'éditeur UMG (doit avoir le même nom)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ChoiceButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChoiceTextLabel;

private:
	int32 ChoiceIndex;
	
	// Pointeur faible (WeakPtr) est plus sûr pour les références UI -> Gameplay
	TWeakObjectPtr<UDialogueComponent> DialogueComponentRef;
};