// CardGameTy	pes.h
#pragma once

#include "CoreMinimal.h"
#include "CardGameTypes.generated.h"

UENUM(BlueprintType)
enum class ECardType : uint8
{
	Servant UMETA(DisplayName = "Servant"),
	Knight  UMETA(DisplayName = "Knight"),
	Tower   UMETA(DisplayName = "Tower"),
	Joker   UMETA(DisplayName = "Joker")
};

USTRUCT(BlueprintType)
struct FCardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType Type = ECardType::Servant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasCoin = false;

	// Helper ID to track movement for animations
	int32 UniqueID = -1; 
};