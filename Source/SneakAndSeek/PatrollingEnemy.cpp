// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingEnemy.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Algo/Reverse.h"

APatrollingEnemy::APatrollingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
}

void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyMesh->OnComponentHit.AddDynamic(this, &APatrollingEnemy::OnHit);

	NextPoint();
}

void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PatrolPoints.IsEmpty())
	{
		CurrentTarget = PatrolPoints[currentPoint];
		if ((CurrentTarget->GetActorLocation() - GetActorLocation()).Size() < 50)
			NextPoint();
	}

	if (CurrentTarget)
	{
		FVector Direction = CurrentTarget->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SetActorLocation(GetActorLocation() + (Direction * Speed * DeltaTime));

		Direction.Z = 0;
		FRotator PlayerRot = FRotationMatrix::MakeFromX(Direction).Rotator();

		SetActorRotation(FMath::Lerp(GetActorRotation(), PlayerRot, RotationSpeed * DeltaTime));
	}
}

void APatrollingEnemy::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector normalImpulse, const FHitResult &Hit)
{
	if (APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		PlayerCharacter->GetCharacterMovement()->AddImpulse(Direction * 50000);
	}
	Algo::Reverse(PatrolPoints);
	NextPoint();
}

void APatrollingEnemy::NextPoint()
{
	if (currentPoint < 0 || currentPoint >= PatrolPoints.Num() - 1)
		currentPoint = 0;
	else
		currentPoint++;
}
