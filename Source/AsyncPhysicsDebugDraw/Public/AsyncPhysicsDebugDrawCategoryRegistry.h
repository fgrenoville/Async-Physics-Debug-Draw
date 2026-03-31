// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "AsyncPhysicsDebugDrawCategoryInfo.h"

class UAsyncPhysicsDebugDrawCategorySet;

/**
 * Runtime registry mapping authored category names to compact runtime identifiers.
 *
 * The registry is rebuilt from the configured category data asset and provides:
 *  - fast lookup from normalized category name to category id
 *  - access to per-category default metadata
 *
 * Important:
 *  - IDs are assigned at runtime and are not stable across registry rebuilds.
 */
class FAsyncPhysicsDebugDrawCategoryRegistry
{
public:
	bool BuildFromAsset(const UAsyncPhysicsDebugDrawCategorySet* Asset);
	const uint8* FindId(const FString& NormalizedName) const;
	const uint8* FindIdByName(const FName Name) const;
	const FAsyncPhysicsDebugDrawCategoryInfo* FindInfoById(const uint8 Id) const;
	void Reset();
	int32 Num() const { return Infos.Num(); }

private:
	TMap<FString, uint8> NameToId;
	TArray<FAsyncPhysicsDebugDrawCategoryInfo> Infos;
};
