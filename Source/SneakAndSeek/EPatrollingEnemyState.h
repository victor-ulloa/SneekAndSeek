// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EPatrollingEnemyState.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum EPatrollingEnemyState
{
     Patrolling     UMETA(DisplayName = "Patrolling"),
     Chasing        UMETA(DisplayName = "Chasing"),
     Cooldown       UMETA(DisplayName = "Cooldown"),
};