// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Weapon/LMAWeaponComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::SpawnWeapon() 
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		Weapon->SetShootFrequency(ShootFrequency);
		//ѕополн€ем магазин
		Weapon->ChangeClip();
		// получаем владельца оружи€, кастуем к ACharacter и сохран€ем в переменную
		const auto Character = Cast<ACharacter>(GetOwner());
		
		if (Character)
		{
			FAttachmentTransformRules WeaponAttachmentRule(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), WeaponAttachmentRule, SocketName);
		}

		//Sign for a delegate OnEmptyClipSignature
		Weapon->OnEmptyClipSignature.AddUObject(this, &ULMAWeaponComponent::OnEmptyClip);
	}
}

void ULMAWeaponComponent::Fire() 
{
	if (Weapon && !AnimReloading && !IsSprinting)
	{
		Weapon->Fire(); //вызывает ф-цию в ALMABaseWeapon
		IsFiring = true;
	}
}

void ULMAWeaponComponent::StopFire()
{
	if (Weapon)
		Weapon->StopFire();
}

void ULMAWeaponComponent::InitAnimNotify() 
{
	if (!ReloadMontage)
		return;
	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinished = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinished)
		{
			ReloadFinished->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh) 
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && !Weapon->IsCurrentClipFull();
}

void ULMAWeaponComponent::OnEmptyClip()
{
	CheckReload();
}

bool ULMAWeaponComponent::GetCurrentWeaponRef(FAmmoWeapon& AmmoWeapon) const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon(); //ссылка на структуру оружи€
		return true;
	}
	return false;
}

void ULMAWeaponComponent::OnSprintingStarted(bool IsSprintingNow) 
{
	IsSprinting = IsSprintingNow;
}

void ULMAWeaponComponent::OnReloadMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) 
{
	if (AnimMontage == ReloadMontage)
	{
		Weapon->ChangeClip();
	}
}

void ULMAWeaponComponent::Reload()
{
	CheckReload();
}

void ULMAWeaponComponent::CheckReload() 
{
	if (!CanReload())
		return;
	AnimReloading = true;
	if (!Weapon->IsCurrentClipFull())
	{
		if (IsFiring)
		{
			StopFire();
		}
		
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			Character->PlayAnimMontage(ReloadMontage);
			Character->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddUniqueDynamic(
				this, &ULMAWeaponComponent::OnReloadMontageBlendingOut);
		}
	}
}
