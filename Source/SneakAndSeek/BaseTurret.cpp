// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTurret.h"
#include "BaseProjectile.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ABaseTurret::ABaseTurret()
{
    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    RootComponent = TurretMesh;

    SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
    SpawnPoint->SetupAttachment(RootComponent);

    TopHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopHitbox"));
    TopHitbox->SetupAttachment(RootComponent);
}

void ABaseTurret::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABaseTurret::Fire, FireRate, true);

    TopHitbox->OnComponentBeginOverlap.AddDynamic(this, &ABaseTurret::OnOverlapBeginHitBox);
	TopHitbox->OnComponentEndOverlap.AddDynamic(this, &ABaseTurret::OnOverlapEndHitBox);
	TurretMesh->OnComponentHit.AddDynamic(this, &ABaseTurret::OnHit);
}

void ABaseTurret::Fire()
{
    FVector Location = SpawnPoint->GetComponentLocation();
    GetWorld()->SpawnActor<ABaseProjectile>(Projectile, SpawnPoint->GetComponentLocation(), GetActorRotation());
}

void ABaseTurret::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector normalImpulse, const FHitResult &Hit)
{

    if (APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
    {
        if (isOnTop)
        {
            PlayerCharacter->Jump();
            isEnabled = false;
            FTimerHandle DisabledTimerHandle;
            GetWorldTimerManager().SetTimer(
                DisabledTimerHandle,
                FTimerDelegate::CreateLambda(
                    [this]
                    { isEnabled = true; }),
                TimeDisabled, false);
            return;
        }
        FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
        Direction.Normalize();
        PlayerCharacter->GetCharacterMovement()->AddImpulse(Direction * 50000);
        PlayerCharacter->SetHealth(PlayerCharacter->GetHealth() - 1);
    }
}

void ABaseTurret::OnOverlapBeginHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor->IsA(APlayerCharacter::StaticClass()))
        isOnTop = true;
}

void ABaseTurret::OnOverlapEndHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->IsA(APlayerCharacter::StaticClass()))
        isOnTop = false;
}
