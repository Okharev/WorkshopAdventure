#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueBlackboard.generated.h"

/**
 * Persiste l'état du dialogue à travers les niveaux.
 */
UCLASS()
class WORKSHOPADVENTURE_API UDialogueBlackboard : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue|State")
	void AddTag(FName Tag);

	UFUNCTION(BlueprintCallable, Category = "Dialogue|State")
	void RemoveTag(FName Tag);

	UFUNCTION(BlueprintPure, Category = "Dialogue|State")
	bool HasTag(FName Tag) const;
	
private:
	UPROPERTY()
	TSet<FName> ActiveTags;
};