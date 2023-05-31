// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPatrollingEnemyState.h"
#include "PatrollingEnemy.generated.h"

class USphereComponent;

UCLASS()
class SNEAKANDSEEK_API APatrollingEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrollingEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *EnemyMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *DetectionArea;

	UPROPERTY(EditAnywhere)
	TArray<AActor *> PatrolPoints;

	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPatrollingEnemyState> State;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	int currentPoint = -1;
	void NextPoint();
	AActor *PlayerActor;
	AActor *CurrentTarget;

	bool isPLayerInArea();
};
