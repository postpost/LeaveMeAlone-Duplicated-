// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LMAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ALMAPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void BeginSpectatingState() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	TSoftObjectPtr<UWorld> OnDeadLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	float DateTimeRate = 2.0f;

private:
	FTimerHandle OnUnPossessTimerHandle;

	void LoadDeathLevel();

	UFUNCTION()
	virtual void OnUnPossess() override;
};
