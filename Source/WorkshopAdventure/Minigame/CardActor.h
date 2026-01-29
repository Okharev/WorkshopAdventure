#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardGameTypes.h" // Include your Enum/Structs here
#include "CardActor.generated.h"

UCLASS()
class WORKSHOPADVENTURE_API ACardActor : public AActor
{
	GENERATED_BODY()
    
public:    
	ACardActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CardMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CoinMesh;

	// --- Movement State ---
	// Where this card *should* be. Used for smooth sliding.
	FVector TargetLocation;
	bool bIsMoving;
	float MoveSpeed;

public:
	// Called by GridManager to initiate a slide (e.g., Joker swap)
	UFUNCTION(BlueprintCallable, Category = "Card Logic")
	void SetTargetPosition(FVector NewPosition);

	// Called by GridManager to update looks (e.g., set Knight material, show Coin)
	// We make this BlueprintImplementable so you can handle Materials in BP easily
	UFUNCTION(BlueprintImplementableEvent, Category = "Card Logic")
	void UpdateCardVisuals(ECardType CardType, bool bHasCoin);
};