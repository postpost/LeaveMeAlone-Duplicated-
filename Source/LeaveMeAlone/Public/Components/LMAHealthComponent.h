// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAHealthComponent();
	
	// Delegates
	FOnHealthChanged OnHealthChanged;
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }
		
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	//Pickup system
	bool AddHealth(float NewHealth); //добавляет здоровье, и возвращает true, если прибавлено
	bool IsHealthFull() const; //проверка на заполненность здоровья

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);
};
