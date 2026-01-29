#include "GlassCraftingComponent.h"

// 1. THE MISSING CONSTRUCTOR
UGlassCraftingComponent::UGlassCraftingComponent()
{
	// Standard component setup
	PrimaryComponentTick.bCanEverTick = false; 
}

// 2. THE MISSING CLEARGLASS FUNCTION
void UGlassCraftingComponent::ClearGlass()
{
	CurrentSlots.Empty();
	
	// Notify the UI that the glass is now empty so visuals update
	if (OnGlassUpdated.IsBound())
	{
		OnGlassUpdated.Broadcast(CurrentSlots);
	}
}

bool UGlassCraftingComponent::AddIngredient(UItemDefinition* Ingredient)
{
    if (CurrentSlots.Num() >= MaxSlots) return false;

    CurrentSlots.Add(Ingredient);
    OnGlassUpdated.Broadcast(CurrentSlots);

    if (CurrentSlots.Num() == MaxSlots)
    {
        CheckForRecipe();
    }
    return true;
}

void UGlassCraftingComponent::CheckForRecipe()
{
    // ... (Your existing code)
    TMap<UItemDefinition*, int32> CurrentComposition;
    for (UItemDefinition* Item : CurrentSlots)
    {
        if (CurrentComposition.Contains(Item))
            CurrentComposition[Item]++;
        else
            CurrentComposition.Add(Item, 1);
    }

    for (URecipeDefinition* Recipe : KnownRecipes)
    {
        // Note: Make sure OrderIndependentCompareEqual is valid for your version, 
        // otherwise manually compare keys and values.
        bool bMatches = true;
        
        // Quick size check
        if (Recipe->IngredientsRequired.Num() != CurrentComposition.Num())
        {
            continue; 
        }

        for (auto& Elem : Recipe->IngredientsRequired)
        {
            if (!CurrentComposition.Contains(Elem.Key) || CurrentComposition[Elem.Key] != Elem.Value)
            {
                bMatches = false;
                break;
            }
        }

        if (bMatches)
        {
            OnCocktailCrafted.Broadcast(Recipe->ResultingCocktail);
            ClearGlass(); // This calls the function we just added above
            return;
        }
    }
    
    // Optional: If no recipe matches, you might want to clear the glass or give a "garbage" item
    ClearGlass(); 
}