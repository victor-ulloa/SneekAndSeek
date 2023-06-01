// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingEnemy.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
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

	TopHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopHitbox"));
	TopHitbox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();

	DetectionArea->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemy::OnOverlapBegin);
	DetectionArea->OnComponentEndOverlap.AddDynamic(this, &APatrollingEnemy::OnOverlapEnd);
	TopHitbox->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemy::OnOverlapBeginHitBox);
	TopHitbox->OnComponentEndOverlap.AddDynamic(this, &APatrollingEnemy::OnOverlapEndHitBox);
	EnemyMesh->OnComponentHit.AddDynamic(this, &APatrollingEnemy::OnHit);

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
		if (isPlayerInArea())
		{
			State = EPatrollingEnemyState::Chasing;
			break;
		}
		if (!PatrolPoints.IsEmpty())
		{
			CurrentTarget = PatrolPoints[currentPoint];
			if ((CurrentTarget->GetActorLocation() - GetActorLocation()).Size() < 50)
				NextPoint();
		}
		break;

	case EPatrollingEnemyState::Chasing:
		if (!isPlayerInArea())
		{
			State = EPatrollingEnemyState::Patrolling;
			break;
		}
		CurrentTarget = PlayerActor;
		break;

	case EPatrollingEnemyState::Cooldown:
		if (!PatrolPoints.IsEmpty())
		{
			CurrentTarget = PatrolPoints[currentPoint];
			if ((CurrentTarget->GetActorLocation() - GetActorLocation()).Size() < 50)
				NextPoint();
		} else {
			CurrentTarget = nullptr;
		}
		break;

	default:
		break;
	}

	if (CurrentTarget)
	{
		FVector Direction = CurrentTarget->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SetActorLocation(GetActorLocation() + (Direction * Speed * DeltaTime));

		Direction.Z= 0;
		FRotator PlayerRot = FRotationMatrix::MakeFromX(Direction).Rotator();

		SetActorRotation(FMath::Lerp(GetActorRotation(), PlayerRot, RotationSpeed * DeltaTime));
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
		PlayerActor = nullptr;
}

void APatrollingEnemy::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector normalImpulse, const FHitResult &Hit)
{
	if (APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		if (isOnTop)
		{
			PlayerCharacter->Jump();
			Destroy();
			return;
		}
		FTimerHandle handle;
		State = EPatrollingEnemyState::Cooldown;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &APatrollingEnemy::CooldownOver, Cooldown, false);
	}
}

void APatrollingEnemy::CooldownOver()
{
	State = EPatrollingEnemyState::Patrolling;
}

void APatrollingEnemy::NextPoint()
{
	if (currentPoint < 0 || currentPoint >= PatrolPoints.Num() - 1)
		currentPoint = 0;
	else
		currentPoint++;
}

bool APatrollingEnemy::isPlayerInArea()
{
	if (PlayerActor)
	{
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), PlayerActor->GetActorLocation());
		return UKismetMathLibrary::DegAcos(FVector::DotProduct(Direction, GetActorForwardVector())) <= 45;
	}
	return false;
}

void APatrollingEnemy::OnOverlapBeginHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		isOnTop = true;
}

void APatrollingEnemy::OnOverlapEndHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		isOnTop = false;
}
