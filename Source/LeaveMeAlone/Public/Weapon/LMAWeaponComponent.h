// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSomethingChanged, bool)

class ALMABaseWeapon;
 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	void SpawnWeapon();
	void Fire();
	void StopFire();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage; // ������������ ������

	//FUNC for FOnMontageBlendingOutStartedMCDelegate
	UFUNCTION()
	void OnReloadMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	bool AnimReloading = false;  //����, ������� ����� ������������ ��� ������ �����������
	bool IsFiring = false;

	void Reload(); //������� �����������
	void CheckReload();
	void InitAnimNotify(); //������� ���������������� �����������
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh); //�������-������� �� ������� ���������� �� ��������
	bool CanReload() const; // ������� ��� �����������

	//callback func for delegate FOnEmptyClipSignature
	void OnEmptyClip();

	//For UI->BulletText Binding
	UFUNCTION(BlueprintCallable)
	bool GetCurrentWeaponRef(FAmmoWeapon& AmmoWeapon) const;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ShootFrequency = 0.08f;

	//Delegate to stop firing when sprinting
	void OnSprintingStarted(bool IsSprintingNow);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName SocketName = "r_Weapon_Socket";
	
	FTimerHandle ReloadFinishedTimer;

	bool IsSprinting = false;

	//UFUNCTION(BlueprintCallable)
	//FOnSomethingChanged OnSomethingChanged;

};
