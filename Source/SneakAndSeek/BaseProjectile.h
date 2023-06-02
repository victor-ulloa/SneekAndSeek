// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class SNEAKANDSEEK_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent *ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	float TimeToLive;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION() 
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalizedImpulse, const FHitResult& Hit);
};
