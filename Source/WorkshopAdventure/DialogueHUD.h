#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h" // Pour le conteneur (ScrollBox ou VerticalBox)
#include "Dialogue/DialogueComponent.h"
#include "UDialogueChoiceWidget.h"
#include "DialogueHUD.generated.h"

UCLASS()
class WORKSHOPADVENTURE_API UDialogueHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Appelé par le PlayerController ou le NPC pour lancer l'UI
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void InitializeDialogue(UDialogueComponent* Comp);

protected:
	// --- Éléments visuels liés à l'UMG ---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeakerNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueContentText;

	// Peut être une ScrollBox ou VerticalBox
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ChoicesContainer; 

	// Référence à la classe du bouton pour pouvoir le spawner (à assigner dans l'éditeur)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUDialogueChoiceWidget> ChoiceWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton; // Ajoute "class" devant UButton si nécessaire ou include Components/Button.h

private:
	// --- Fonctions de Callback (Les "Events Rouges" du Blueprint) ---
    
	UFUNCTION()
	void OnDialogueUpdated(FText Speaker, FText Text, const TArray<FDialogueChoice>& Choices);

	UFUNCTION()
	void OnDialogueEnded();
	
	UFUNCTION()
	void OnQuitClicked();

	TWeakObjectPtr<UDialogueComponent> LinkedComponent;
};