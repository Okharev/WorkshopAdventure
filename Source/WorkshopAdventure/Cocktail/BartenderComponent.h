#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IngredientData.h"
#include "BartenderComponent.generated.h"

// Événement quand un cocktail est réussi
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCocktailCrafted, FCocktailResult, CocktailInfo);
// Événement quand le verre change
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGlassUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORKSHOPADVENTURE_API UBartenderComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    UBartenderComponent();

    // --- CONFIGURATION ---
    
    // Référence vers le DataAsset contenant toutes les recettes
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    UCocktailRecipeBook* RecipeBook;

    // La liste de tous les ingrédients possibles dans le jeu
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    TArray<FIngredientData> AllIngredients;

    // --- ÉTAT (STATE) ---

    // Les tags que le joueur possède (ex: "VodkaUnlock")
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    TArray<FName> UnlockedTags;

    // Le contenu actuel du verre
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    TArray<FIngredientData> CurrentMix;

    const int32 MaxSections = 5;

    // --- EVENTS ---

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGlassUpdated OnGlassUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCocktailCrafted OnCocktailCrafted;

    // --- FONCTIONS ---

    // Ajoute un ingrédient et vérifie la recette si plein
    UFUNCTION(BlueprintCallable, Category = "Bartending")
    void AddIngredientToGlass(FIngredientData Ingredient);

    // Débloque un tag (ex: achat au magasin ou progression)
    UFUNCTION(BlueprintCallable, Category = "Bartending")
    void UnlockTag(FName NewTag);

    // Vide le verre
    UFUNCTION(BlueprintCallable, Category = "Bartending")
    void ClearGlass();

    // Vérifie si un tag spécifique est débloqué (Utile pour l'UI)
    UFUNCTION(BlueprintPure, Category = "Bartending")
    bool IsTagUnlocked(FName TagToCheck) const;

private:
    void CheckRecipes();
};