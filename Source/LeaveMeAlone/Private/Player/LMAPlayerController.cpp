// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LMADefaultCharacter.h"
#include "Components/LMAHealthComponent.h"

void ALMAPlayerController::BeginPlay() 
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ALMAPlayerController::BeginSpectatingState() 
{
	SetControlRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}

void ALMAPlayerController::LoadDeathLevel()
{
	if (!OnDeadLevel.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, OnDeadLevel);
	}
}

void ALMAPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player is dead")));
	GetWorldTimerManager().SetTimer(OnUnPossessTimerHandle, this, &ALMAPlayerController::LoadDeathLevel, DateTimeRate, false);
}
