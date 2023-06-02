// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PatrollingEnemy.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS()
class SNEAKANDSEEK_API APatrollingEnemy : public AActor
{
	GENERATED_BODY()

public:
	APatrollingEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *EnemyMesh;

	UPROPERTY(EditAnywhere)
	TArray<AActor *> PatrolPoints;

	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	UPROPERTY(EditAnywhere)
	float Cooldown;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit);

	int currentPoint = -1;
	void NextPoint();
	AActor *CurrentTarget;

};
