// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.h"
#include "RecipeDefinition.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOPADVENTURE_API URecipeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// The key is the Ingredient, the value is how many 'parts' of the glass it takes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe")
	TMap<UItemDefinition*, int32> IngredientsRequired;

	// The resulting cocktail
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe")
	UItemDefinition* ResultingCocktail;

	// Helper to validate logic
	bool Matches(const TMap<UItemDefinition*, int32>& CurrentIngredients) const;
	
};
