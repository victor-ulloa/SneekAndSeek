// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTurret.generated.h"

class ABaseProjectile;
class UBoxComponent;

UCLASS()
class SNEAKANDSEEK_API ABaseTurret : public AActor
{
	GENERATED_BODY()

public:
	ABaseTurret();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USceneComponent *SpawnPoint;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent *TopHitbox;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<ABaseProjectile> Projectile;
	UPROPERTY(EditAnywhere, Category = "Turret", meta = (AllowPrivateAccess = true))
	float TimeDisabled;
	UPROPERTY(EditAnywhere, Category = "Turret", meta = (AllowPrivateAccess = true))
	float FireRate = 2.f;

private:
	FTimerHandle FireRateTimerHandle;
	bool isEnabled = true;
	bool isOnTop;
	void Fire();

	UFUNCTION()
	void OnOverlapBeginHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnOverlapEndHitBox(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector normalImpulse, const FHitResult &Hit);
};
