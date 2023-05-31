// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrollingEnemy.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "PatrolPoints")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


	int currentPoint = -1;

	void NextPoint();

	bool HasReachedTarget();
};
