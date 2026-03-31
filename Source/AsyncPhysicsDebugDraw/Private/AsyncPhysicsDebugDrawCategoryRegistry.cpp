// Copyright (c) [2026] [Federico Grenoville]

#include "AsyncPhysicsDebugDrawCategoryRegistry.h"

#include "AsyncPhysicsDebugDrawCategory.h"
#include "AsyncPhysicsDebugDrawCategorySet.h"
#include "AsyncPhysicsDebugDrawLog.h"

/**
 * Build the runtime registry from the authored category asset.
 *
 * Rules:
 *  - Empty names are ignored.
 *  - Names are normalized to lowercase for case-insensitive lookup.
 *  - Duplicate names are ignored after the first occurrence.
 *  - The registry is capped at 64 categories to match the uint64 filter mask.
 */
bool FAsyncPhysicsDebugDrawCategoryRegistry::BuildFromAsset(const UAsyncPhysicsDebugDrawCategorySet* Asset)
{
	NameToId.Empty();
	Infos.Empty();

	if (!Asset)
		return false;

	uint8 NextId = 0;

	for (const FAsyncPhysicsDebugDrawCategory& Category : Asset->Categories)
	{
		if (Category.Name.IsNone())
			continue;

		FString Key = Category.Name.ToString();
		Key.TrimStartAndEndInline();
		Key.ToLowerInline();

		if (Key.IsEmpty())
			continue;

		if (NameToId.Contains(Key))
			continue;

		if (NextId >= 64)
		{
			UE_LOG(LogAsyncPhysicsDebugDraw, Warning, TEXT("[APDD] More than 64 categories defined, extra categories will be ignored."));
			break;
		}

		NameToId.Add(Key, NextId);

		FAsyncPhysicsDebugDrawCategoryInfo Info;
		Info.Id = NextId;
		Info.DefaultColor = Category.DefaultColor;
		Info.DefaultLifetime = Category.DefaultLifetime;
		Infos.Add(Info);
		
		++NextId;
	}

	return Infos.Num() > 0;
}

const uint8* FAsyncPhysicsDebugDrawCategoryRegistry::FindId(const FString& NormalizedName) const
{
	return NameToId.Find(NormalizedName);
}

const uint8* FAsyncPhysicsDebugDrawCategoryRegistry::FindIdByName(const FName Name) const
{
	FString Key = Name.ToString();
	Key.TrimStartAndEndInline();
	Key.ToLowerInline();

	return NameToId.Find(Key);
}

const FAsyncPhysicsDebugDrawCategoryInfo* FAsyncPhysicsDebugDrawCategoryRegistry::FindInfoById(const uint8 Id) const
{
	return Infos.IsValidIndex(Id) ? &Infos[Id] : nullptr;
}

void FAsyncPhysicsDebugDrawCategoryRegistry::Reset()
{
	NameToId.Empty();
	Infos.Empty();
}
