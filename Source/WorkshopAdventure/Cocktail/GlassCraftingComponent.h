// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDefinition.h"
#include "RecipeDefinition.h"
#include "GlassCraftingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGlassUpdated, const TArray<UItemDefinition*>&, Slots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCocktailCrafted, UItemDefinition*, NewCocktail);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORKSHOPADVENTURE_API UGlassCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGlassCraftingComponent();

	// The fixed size of the glass
	const int32 MaxSlots = 5;

	// Adds an ingredient to the first available slot
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool AddIngredient(UItemDefinition* Ingredient);

	// Clears the glass
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void ClearGlass();

	// List of all known recipes (Assign this in Editor or load via AssetRegistry)
	UPROPERTY(EditDefaultsOnly, Category = "Crafting")
	TArray<URecipeDefinition*> KnownRecipes;

	UPROPERTY(BlueprintAssignable, Category = "Crafting")
	FOnGlassUpdated OnGlassUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Crafting")
	FOnCocktailCrafted OnCocktailCrafted;

protected:
	UPROPERTY()
	TArray<UItemDefinition*> CurrentSlots;

	void CheckForRecipe();
};
