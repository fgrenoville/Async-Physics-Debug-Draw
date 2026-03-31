// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "AsyncPhysicsDebugDrawCategoryHandle.generated.h"

/**
 * Lightweight runtime handle for a debug category.
 *
 * Handles are resolved from category names through the module registry and are
 * intended for repeated use in hot paths such as async physics ticks.
 *
 * Important:
 *  - A handle is only meaningful for the current registry state.
 *  - If categories are rebuilt from settings or assets, previously resolved handles
 *    may become invalid or refer to different category data.
 */
USTRUCT(BlueprintType, Category = "Async Physics Debug Draw")
struct FAsyncPhysicsDebugDrawCategoryHandle
{
	GENERATED_BODY()

private:
	UPROPERTY()
	uint8 Id = 255;

public:
	bool IsValid() const { return Id != 255; }

	friend class FAsyncPhysicsDebugDrawModule;
};
