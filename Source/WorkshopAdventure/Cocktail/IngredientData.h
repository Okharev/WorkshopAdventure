#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h" // Nécessaire pour le DataAsset
#include "IngredientData.generated.h"

// --- Mise à jour de l'Ingrédient ---
USTRUCT(BlueprintType)
struct FIngredientData
{
    GENERATED_BODY()

    // ID Unique pour la logique (ex: "Vodka", "JusOrange")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    FName IngredientID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    FLinearColor FluidColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    FName RequiredUnlockTag;

    FIngredientData()
    {
        IngredientID = NAME_None;
        FluidColor = FLinearColor::Blue;
        RequiredUnlockTag = NAME_None;
    }
    
    // Surcharge de l'opérateur == pour faciliter les comparaisons si besoin
    bool operator==(const FIngredientData& Other) const
    {
        return IngredientID == Other.IngredientID;
    }
};

// --- Structure du Résultat (Le Cocktail Fini) ---
USTRUCT(BlueprintType)
struct FCocktailResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    FText Name;
};

// --- Structure d'une Recette ---
USTRUCT(BlueprintType)
struct FCocktailRecipe
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    FCocktailResult ResultingCocktail;

    // La liste exacte des 5 IDs nécessaires, dans l'ordre (Bas vers Haut)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cocktail")
    TArray<FName> RequiredIngredients;
};

// --- DataAsset pour stocker la liste des recettes ---
UCLASS()
class WORKSHOPADVENTURE_API UCocktailRecipeBook : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipes")
    TArray<FCocktailRecipe> Recipes;
};