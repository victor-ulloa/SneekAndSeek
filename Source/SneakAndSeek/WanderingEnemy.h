// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWanderingEnemyState.h"
#include "WanderingEnemy.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS()
class SNEAKANDSEEK_API AWanderingEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWanderingEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *EnemyMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *DetectionArea;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent *TopHitbox;

	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	UPROPERTY(EditAnywhere)
	float NextPointRange;

	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EWanderingEnemyState> State;

public:
	virtual void Tick(float DeltaTime) override;

private:
UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector normalImpulse, const FHitResult &Hit);

	UFUNCTION()
	void OnOverlapBeginHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnOverlapEndHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CooldownOver();

	bool isOnTop;
	void NextPoint();
	AActor *PlayerActor;
	FVector CurrentTargetLocation;

	bool isPlayerInArea();
};
