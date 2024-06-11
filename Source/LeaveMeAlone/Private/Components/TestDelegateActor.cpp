// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Components/TestDelegateActor.h"
#include "Components/BoxComponent.h"


ATestDelegateActor::ATestDelegateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	check(BoxComponent);
	SetRootComponent(BoxComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BeginOverlap);
	DelegateName = FName(TEXT("TestDelegateActor"));
}

void ATestDelegateActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Magenta, FString::Printf(TEXT("Hello from %s"), *DelegateName.ToString()));
}
