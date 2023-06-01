// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EWanderingEnemyState.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum EWanderingEnemyState
{
     Wandering     UMETA(DisplayName = "Patrolling"),
     Chasing        UMETA(DisplayName = "Chasing"),
     Cooldown       UMETA(DisplayName = "Cooldown"),
};