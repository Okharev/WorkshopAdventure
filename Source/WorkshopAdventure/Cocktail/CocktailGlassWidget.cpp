#include "CocktailGlassWidget.h"

#include "Components/Image.h"

void UCocktailGlassWidget::UpdateGlassVisuals(const TArray<FIngredientData>& CurrentMix)
{
	// Parcourir les 5 sections possibles
	for (int32 i = 0; i < 5; i++)
	{
		UImage* TargetImage = GetImageByIndex(i);
		if (!TargetImage) continue;

		if (i < CurrentMix.Num())
		{
			// Si la section est remplie dans le mix
			TargetImage->SetColorAndOpacity(CurrentMix[i].FluidColor);
			TargetImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			// Si la section est vide
			TargetImage->SetVisibility(ESlateVisibility::Hidden); 
			// Ou SetOpacity(0) si tu veux garder la place
		}
	}
}

UImage* UCocktailGlassWidget::GetImageByIndex(int32 Index)
{
	switch (Index)
	{
	case 0: return Section_0;
	case 1: return Section_1;
	case 2: return Section_2;
	case 3: return Section_3;
	case 4: return Section_4;
	default: return nullptr;
	}
}