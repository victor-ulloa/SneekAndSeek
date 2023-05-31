// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingEnemy.h"

// Sets default values
APatrollingEnemy::APatrollingEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();
	NextPoint();
}

// Called every frame
void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PatrolPoints.IsEmpty())
	{
		FVector Direction = PatrolPoints[currentPoint]->GetActorLocation() - GetActorLocation();
		if (Direction.Size() < 50)
		{
			NextPoint();
		}
		Direction.Normalize();
		SetActorLocation(GetActorLocation() + (Direction * Speed * DeltaTime));
	}
}

void APatrollingEnemy::NextPoint()
{
	if (currentPoint < 0 || currentPoint >= PatrolPoints.Num() - 1)
	{
		currentPoint = 0;
	}
	else
	{
		currentPoint++;
	}
}

bool APatrollingEnemy::HasReachedTarget()
{
	return false;
}
