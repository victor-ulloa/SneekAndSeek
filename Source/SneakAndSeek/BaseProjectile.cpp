// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;
	ProjectileMovementComponent->ProjectileGravityScale = .0f;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	FTimerHandle DestroyHandle;
	GetWorldTimerManager().SetTimer(
		DestroyHandle,
		FTimerDelegate::CreateLambda(
			[this]
			{ Destroy(); }),
		TimeToLive, false);
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalizedImpulse, const FHitResult &Hit)
{
	if (APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->SetHealth(PlayerCharacter->GetHealth() - 1);
	}
	Destroy();
}
