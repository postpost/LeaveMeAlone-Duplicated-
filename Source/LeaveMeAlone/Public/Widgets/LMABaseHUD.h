// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LMABaseHUD.generated.h"

/**
 * 
 */

class UUserWidget;

UCLASS()
class LEAVEMEALONE_API ALMABaseHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray <UUserWidget*> WidgetsContainer;
};
