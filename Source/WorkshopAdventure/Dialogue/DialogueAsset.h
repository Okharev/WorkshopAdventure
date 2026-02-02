#pragma once

#include "DialogueCondition.h"
#include "DialogueAsset.generated.h"

// Enum to distinguish speakers
UENUM(BlueprintType)
enum class EDialogueSpeaker : uint8 {
	NPC,
	Player
};

// A single line of text
USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDialogueSpeaker Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* AudioClip; // Optional voiceover
};

// A "Segment" is a chunk of conversation (News, Asking for Drink, etc.)
USTRUCT(BlueprintType)
struct FDialogueSegment
{
	GENERATED_BODY()

	// 1. Conditions required to START this segment
	// If these fail, the NPC repeats the previous segment or a generic "waiting" line
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<UDialogueCondition*> Conditions;

	// 2. The actual conversation lines (Back and forth)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueLine> Lines;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FDialogueLine> FailureLines;

	// 3. Events to fire when this segment FINISHES (e.g., Give Note)
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<UDialogueEvent*> OnFinishedEvents;
};

// The Main Data Asset
UCLASS(BlueprintType)
class WORKSHOPADVENTURE_API UDialogueAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueSegment> Segments;
};