#include "BartenderComponent.h"

UBartenderComponent::UBartenderComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBartenderComponent::UnlockTag(FName NewTag)
{
    // On évite les doublons
    if (!UnlockedTags.Contains(NewTag) && !NewTag.IsNone())
    {
        UnlockedTags.Add(NewTag);
        // On notifie l'UI que quelque chose a changé (pour dégriser les boutons)
        OnGlassUpdated.Broadcast();
    }
}

bool UBartenderComponent::IsTagUnlocked(FName TagToCheck) const
{
    // Si le tag est "None", l'ingrédient est débloqué par défaut (ex: Jus d'orange de base)
    if (TagToCheck.IsNone())
    {
        return true;
    }
    return UnlockedTags.Contains(TagToCheck);
}

void UBartenderComponent::AddIngredientToGlass(FIngredientData Ingredient)
{
    if (CurrentMix.Num() < MaxSections)
    {
        CurrentMix.Add(Ingredient);
        OnGlassUpdated.Broadcast();

        // Vérification immédiate si le verre est plein
        if (CurrentMix.Num() == MaxSections)
        {
            CheckRecipes();
        }
    }
}

void UBartenderComponent::CheckRecipes()
{
    if (!RecipeBook)
    {
        UE_LOG(LogTemp, Error, TEXT("RecipeBook is missing in BartenderComponent!"));
        ClearGlass();
        return;
    }

    bool bRecipeFound = false;
    FCocktailResult FoundCocktail;

    for (const FCocktailRecipe& Recipe : RecipeBook->Recipes)
    {
        if (Recipe.RequiredIngredients.Num() != MaxSections) continue;

        bool bIngredientsMatch = true;

        for (int32 i = 0; i < MaxSections; i++)
        {
            // Comparaison des IDs
            if (CurrentMix[i].IngredientID != Recipe.RequiredIngredients[i])
            {
                bIngredientsMatch = false;
                break;
            }
        }

        if (bIngredientsMatch)
        {
            FoundCocktail = Recipe.ResultingCocktail;
            bRecipeFound = true;
            break;
        }
    }

    if (bRecipeFound)
    {
        OnCocktailCrafted.Broadcast(FoundCocktail);
        // Ne vide pas le verre tout de suite, laisse l'UI le faire après l'anim
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Mélange raté !"));
        ClearGlass();
    }
}

void UBartenderComponent::ClearGlass()
{
    CurrentMix.Empty();
    OnGlassUpdated.Broadcast();
}