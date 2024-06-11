// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/LMADefaultCharacter.h"
#include "LMAHealthPickup.generated.h"

class USphereComponent;
class ALMADefaultCharacter;

UCLASS()
class LEAVEMEALONE_API ALMAHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMAHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.0f; // время респавна актора

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = 5.0f, ClampMax = 100.0f))
	float HealthFromPickup = 100.0f; //пополнение здоровья

	//TEST ONLY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DiplayName = "My Test value"))
	float EditAnywhere_BlueprintReadWrite_CustomDisplayNameProperty = 1.0f;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //переопределение функции базового класса Актора, срабатывает при коллизии

private:
	bool GivePickup (ALMADefaultCharacter* Character); //был ли подобран пикап
	void PickupWasTaken(); //таймер респавна, будет скрывать пикап на опр-ное время после взятия пикапа (по имени не понятно, что это таймер)
	void RespawnPickup(); //возвращает видимость пикапа через опр-ное кол-во времени
};
