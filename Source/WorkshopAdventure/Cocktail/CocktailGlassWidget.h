#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "IngredientData.h"
#include "CocktailGlassWidget.generated.h"

UCLASS()
class WORKSHOPADVENTURE_API UCocktailGlassWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Fonction appelée par le Component pour mettre à jour les couleurs
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateGlassVisuals(const TArray<FIngredientData>& CurrentMix);

protected:
	// --- Liaison avec le Designer UMG ---
	// Il faut créer 5 images dans ton Widget Blueprint et les nommer EXACTEMENT comme ci-dessous
	// L'ordre est important : Section_0 est le BAS, Section_4 est le HAUT.
    
	UPROPERTY(meta = (BindWidget))
	UImage* Section_0; // Bas

	UPROPERTY(meta = (BindWidget))
	UImage* Section_1;

	UPROPERTY(meta = (BindWidget))
	UImage* Section_2;

	UPROPERTY(meta = (BindWidget))
	UImage* Section_3;

	UPROPERTY(meta = (BindWidget))
	UImage* Section_4; // Haut

private:
	// Helper pour accéder aux images via un index
	UImage* GetImageByIndex(int32 Index);
};