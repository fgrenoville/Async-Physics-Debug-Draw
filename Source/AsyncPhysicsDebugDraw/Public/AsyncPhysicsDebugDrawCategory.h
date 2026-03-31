// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "AsyncPhysicsDebugDrawCategory.generated.h"

/**
* Authoring-time definition of a debug category.
 *
 * Categories are defined in a data asset and converted at runtime into compact
 * integer identifiers for fast filtering on the Game Thread.
 *
 * Fields:
 *  - Name: unique category name used for lookup and console filtering.
 *  - DefaultColor: fallback draw color used when a caller does not specify one.
 *  - DefaultLifetime: fallback draw duration used when a caller does not specify one.
 */
USTRUCT(BlueprintType, Category = "Async Physics Debug Draw")
struct FAsyncPhysicsDebugDrawCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Async Physics Debug Draw")
	FName Name = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Async Physics Debug Draw")
	FColor DefaultColor = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Async Physics Debug Draw")
	float DefaultLifetime = 0.0f;
};
