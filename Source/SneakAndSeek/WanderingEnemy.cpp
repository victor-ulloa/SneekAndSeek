// Fill out your copyright notice in the Description page of Project Settings.

#include "WanderingEnemy.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AWanderingEnemy::AWanderingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;

	DetectionArea = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionArea"));
	DetectionArea->SetupAttachment(RootComponent);

	TopHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopHitbox"));
	TopHitbox->SetupAttachment(RootComponent);
}

void AWanderingEnemy::BeginPlay()
{
	Super::BeginPlay();

	DetectionArea->OnComponentBeginOverlap.AddDynamic(this, &AWanderingEnemy::OnOverlapBegin);
	DetectionArea->OnComponentEndOverlap.AddDynamic(this, &AWanderingEnemy::OnOverlapEnd);
	TopHitbox->OnComponentBeginOverlap.AddDynamic(this, &AWanderingEnemy::OnOverlapBeginHitBox);
	TopHitbox->OnComponentEndOverlap.AddDynamic(this, &AWanderingEnemy::OnOverlapEndHitBox);
	EnemyMesh->OnComponentHit.AddDynamic(this, &AWanderingEnemy::OnHit);

	State = EWanderingEnemyState::Wandering;

	NextPoint();
}

void AWanderingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EWanderingEnemyState::Wandering:
		if (isPlayerInArea())
		{
			State = EWanderingEnemyState::Chasing;
			break;
		}
		if ((CurrentTargetLocation - GetActorLocation()).Size() < 50)
			NextPoint();
		break;

	case EWanderingEnemyState::Chasing:
		if (!isPlayerInArea())
		{
			State = EWanderingEnemyState::Wandering;
			NextPoint();
			break;
		}
		if (PlayerActor)
			CurrentTargetLocation = PlayerActor->GetActorLocation();
		break;

	case EWanderingEnemyState::Cooldown:
		if ((CurrentTargetLocation - GetActorLocation()).Size() < 50)
			NextPoint();
		break;

	default:
		break;
	}

	if (CurrentTargetLocation != GetActorLocation())
	{
		FVector Direction = CurrentTargetLocation - GetActorLocation();
		Direction.Normalize();
		SetActorLocation(GetActorLocation() + (Direction * Speed * DeltaTime));

		Direction.Z = 0;
		FRotator PlayerRot = FRotationMatrix::MakeFromX(Direction).Rotator();

		SetActorRotation(FMath::Lerp(GetActorRotation(), PlayerRot, RotationSpeed * DeltaTime));
	}
}

void AWanderingEnemy::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		PlayerActor = OtherActor;
}

void AWanderingEnemy::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PlayerActor)
		PlayerActor = nullptr;
}

void AWanderingEnemy::CooldownOver()
{
	State = EWanderingEnemyState::Wandering;
}

void AWanderingEnemy::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector normalImpulse, const FHitResult &Hit)
{
	if (APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		if (isOnTop)
		{
			PlayerCharacter->Jump();
			Destroy();
			return;
		}
		NextPoint();
		FTimerHandle handle;
		State = EWanderingEnemyState::Cooldown;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AWanderingEnemy::CooldownOver, Cooldown, false);
	}
}

void AWanderingEnemy::NextPoint()
{
	FVector NewPointRelative = FVector(FMath::RandRange(-NextPointRange, NextPointRange), FMath::RandRange(-NextPointRange, NextPointRange), 0);
	CurrentTargetLocation = GetActorLocation() + NewPointRelative;
}

bool AWanderingEnemy::isPlayerInArea()
{
	if (PlayerActor)
	{
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), PlayerActor->GetActorLocation());
		return UKismetMathLibrary::DegAcos(FVector::DotProduct(Direction, GetActorForwardVector())) <= 45;
	}
	return false;
}

void AWanderingEnemy::OnOverlapBeginHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		isOnTop = true;
}

void AWanderingEnemy::OnOverlapEndHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		isOnTop = false;
}