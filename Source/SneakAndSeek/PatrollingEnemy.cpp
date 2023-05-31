// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingEnemy.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APatrollingEnemy::APatrollingEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;

	DetectionArea = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionArea"));
	DetectionArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();

	DetectionArea->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemy::OnOverlapBegin);
	DetectionArea->OnComponentEndOverlap.AddDynamic(this, &APatrollingEnemy::OnOverlapEnd);

	State = EPatrollingEnemyState::Patrolling;

	NextPoint();
}

// Called every frame
void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EPatrollingEnemyState::Patrolling:
		if (isPLayerInArea())
		{
			State = EPatrollingEnemyState::Chasing;
			break;
		}
		if (!PatrolPoints.IsEmpty())
		{
			FVector Direction = PatrolPoints[currentPoint]->GetActorLocation() - GetActorLocation();
			if (Direction.Size() < 50)
				NextPoint();
			Direction.Normalize();
			SetActorLocation(GetActorLocation() + (Direction * Speed * DeltaTime));
		}
		break;

	case EPatrollingEnemyState::Chasing:
		if (!isPLayerInArea())
		{
			State = EPatrollingEnemyState::Patrolling;
			break;
		}

		
		break;

	case EPatrollingEnemyState::Cooldown:
		/* code */
		break;

	default:
		break;
	}

	if (isPLayerInArea())
	{
		// change state
	}
}

void APatrollingEnemy::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		PlayerActor = OtherActor;
}

void APatrollingEnemy::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PlayerActor)
	{
		PlayerActor = nullptr;
	}
}

void APatrollingEnemy::NextPoint()
{
	if (currentPoint < 0 || currentPoint >= PatrolPoints.Num() - 1)
		currentPoint = 0;
	else
		currentPoint++;
}

bool APatrollingEnemy::isPLayerInArea()
{
	if (PlayerActor)
	{
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), PlayerActor->GetActorLocation());
		return UKismetMathLibrary::DegAcos(FVector::DotProduct(Direction, GetActorForwardVector())) <= 45;
	}
	return false;
}
