// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "AsyncPhysicsDebugDrawCategory.h"
#include "Engine/DataAsset.h"
#include "AsyncPhysicsDebugDrawCategorySet.generated.h"

/**
 * Data asset containing the list of debug categories available to the plugin.
 *
 * This asset is referenced from project settings and converted into a compact
 * runtime registry during initialization.
 *
 * Typical usage:
 *  - Create one category set asset for the project.
 *  - Assign it in Project Settings.
 *  - Resolve category handles at runtime by category name.
 */
UCLASS(BlueprintType, Category = "Async Physics Debug Draw")
class ASYNCPHYSICSDEBUGDRAW_API UAsyncPhysicsDebugDrawCategorySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Async Physics Debug Draw")
	TArray<FAsyncPhysicsDebugDrawCategory> Categories;
};
