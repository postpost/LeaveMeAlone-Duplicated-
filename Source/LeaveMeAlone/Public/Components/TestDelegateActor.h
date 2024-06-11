// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestDelegateActor.generated.h"

class UBoxComponent;

UCLASS()
class LEAVEMEALONE_API ATestDelegateActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ATestDelegateActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components");
	UBoxComponent* BoxComponent;
	FName DelegateName;

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32
		OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
