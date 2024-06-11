// LeaveMeAlone game by MagicBit. All rights reserved.


#include "LevelActors/Pickup/LMAHealthPickup.h"
#include "Player/LMADefaultCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/LMAHealthComponent.h"

// Sets default values
ALMAHealthPickup::ALMAHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void ALMAHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALMAHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALMAHealthPickup::PickupWasTaken()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetRootComponent()->SetVisibility(false, true); // скрывает пикап

	FTimerHandle RespawnTimerHandle; // запускает таймер
	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle, this, &ALMAHealthPickup::RespawnPickup, RespawnTime); // через время RespawnTime вызвать функцию RespawnPickup()
}

void ALMAHealthPickup::RespawnPickup()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetRootComponent()->SetVisibility(true, true); // снова показывает пикап
}


bool ALMAHealthPickup::GivePickup(ALMADefaultCharacter* Character)
{
	const auto HealthComponent = Character->GetHealthComponent();
	if (!HealthComponent) return false;
	return HealthComponent->AddHealth(HealthFromPickup);
	
}

void ALMAHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	const auto Character = Cast<ALMADefaultCharacter>(OtherActor);
	if (GivePickup(Character))
	{
		PickupWasTaken();
	}
}


