// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMADefaultCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/LMAHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/LMAWeaponComponent.h"
#include <Player/LMAPlayerController.h>
#include <Widgets/LMABaseHUD.h>
#include "Blueprint/UserWidget.h"
#include "Engine/Level.h"


// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true); //данное условие не позволит нашей камере поворачиваться в момент поворота персонажа, 
														//в жанре TDS камера статична над игроком
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f)); //FRotator(Pitch (nose up/down_Y), Yaw(vertical rotation_X), Roll(Z))
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false; //запрещает камере вращаться относительно SpringArmComponent

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Jump functions improvements
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 650;

	//Health Component
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	//Weapon Component
	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	//Подписка на делегат получения урона
	OnHealthChanged(HealthComponent->GetHealth()); //сначала вызывается BeginPlay() у компонентов
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

	//Подписка на делегат FOnDeath из HealthComponent
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);

	Stamina = MaxStamina;

	InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
	InitialAcceleration = GetCharacterMovement()->MaxAcceleration;
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor(); //включать курсор только, когда персонаж НЕ_МЕРТВ
	}
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("CameraZoom", this, &ALMADefaultCharacter::CameraZoom);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALMADefaultCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALMADefaultCharacter::OnStopJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprint);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);
	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::MoveForward(float Value) 
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) 
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::CameraZoom(float Value)
{
	float ZoomFactor = Value * ZoomMultiplier;
	SpringArmComponent->TargetArmLength += ZoomFactor;
	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength, 150, 1400);
}

void ALMADefaultCharacter::OnStartJump() 
{
	bPressedJump = true;
}

void ALMADefaultCharacter::OnStopJump() 
{
	bPressedJump = false;
}

void ALMADefaultCharacter::OnDeath() 
{
	CurrentCursor->DestroyRenderState_Concurrent();

	auto PlayerController = Cast<ALMAPlayerController>(Controller);
	if (PlayerController)
	{
		auto HUD = Cast<ALMABaseHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			for (auto Widget : HUD->WidgetsContainer)
			{
				//	#include "Blueprint/UserWidget.h"
				Widget->RemoveFromParent();
			}
		}
	}
	
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	//Load UI
	//GetWorldTimerManager().SetTimer(OnDeadUITimerHandle, this, &ALMADefaultCharacter::LoadUIOnDead, 1.0f, false);
}

void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));

		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

//void ALMADefaultCharacter::LoadUIOnDead() 
//{
//	if (OnDeadLevel)
//	{
//		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), OnDeadLevel);
//	}
//
//	GetWorldTimerManager().ClearTimer(OnDeadUITimerHandle);
//}

void ALMADefaultCharacter::StartSprint()
{
	IsSprinting = true;
	if (WeaponComponent)
	{
		WeaponComponent->OnSprintingStarted(IsSprinting);
	}
	if (Stamina>0)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetCharacterMovement()->MaxAcceleration = SprintAcceleration;
		// Timer to decrease Stamina
		GetWorld()->GetTimerManager().SetTimer(StaminaDecreaseTimerHandle, this, &ALMADefaultCharacter::DecreaseStamina, 0.5f, true);
	}
}

void ALMADefaultCharacter::StopSprint() 
{
	IsSprinting = false;
	if (WeaponComponent)
	{
		WeaponComponent->OnSprintingStarted(IsSprinting);
	}
	//Вернуть исходные значения
	GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
	GetCharacterMovement()->MaxAcceleration = InitialAcceleration;

	// Switch Stamina decrease off
	GetWorld()->GetTimerManager().ClearTimer(StaminaDecreaseTimerHandle);

	// Timer to restore Stamina
	GetWorld()->GetTimerManager().SetTimer(StaminaRestoreTimerHandle, this, &ALMADefaultCharacter::RestoreStamina, 5.0f, false);

}

void ALMADefaultCharacter::DecreaseStamina()
{
	if (IsSprinting)
	{
		Stamina = FMath::Clamp(Stamina - StaminaDamage, 0.0f, MaxStamina);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Stamina = %f"), Stamina));
	}
	if (Stamina == 0 && IsSprinting == true || FMath::IsNearlyZero(GetVelocity().Length()))
	{
		IsStaminaRestored = false;
		StopSprint();
	}
}

void ALMADefaultCharacter::RestoreStamina()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		if (Stamina == 0 || !PC->IsInputKeyDown(EKeys::LeftShift))
		{
			Stamina = MaxStamina;
			IsStaminaRestored = true;
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Stamina restored = %f"), Stamina));
			GetWorld()->GetTimerManager().ClearTimer(StaminaRestoreTimerHandle);
		}

		if (PC->IsInputKeyDown(EKeys::LeftShift))
			StartSprint();
	}

}
