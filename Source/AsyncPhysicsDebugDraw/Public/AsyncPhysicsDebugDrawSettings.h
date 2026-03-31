// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "AsyncPhysicsDebugDrawCategorySet.h"
#include "Engine/DeveloperSettings.h"
#include "AsyncPhysicsDebugDrawSettings.generated.h"

/**
 * Project-wide settings for Async Physics Debug Draw.
 *
 * These settings define the startup defaults applied when the module or the
 * gameplay world initializes:
 *  - CategorySet selects the source asset for category definitions.
 *  - EnableDraw controls whether debug drawing is enabled by default.
 *  - DurationMultiplier scales draw lifetimes globally.
 *  - DrawCategories defines the initial category filter string.
 *
 * Runtime behavior:
 *  - The module synchronizes these values into runtime console variables.
 *  - Console changes made during play act as runtime overrides for the current session.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Async Physics Debug Draw"), Category = "Async Physics Debug Draw")
class ASYNCPHYSICSDEBUGDRAW_API UAsyncPhysicsDebugDrawSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
	virtual FName GetSectionName() const override { return TEXT("AsyncPhysicsDebugDraw"); }
	
	UPROPERTY(Config, EditAnywhere, Category = "General")
	TSoftObjectPtr<UAsyncPhysicsDebugDrawCategorySet> CategorySet;

	UPROPERTY(Config, EditAnywhere, Category = "General")
	bool bEnableDraw = true;
	
	UPROPERTY(Config, EditAnywhere, Category = "General")
	float DurationMultiplier = 1.0f;

	UPROPERTY(Config, EditAnywhere, Category = "General")
	FString DrawCategories = TEXT("All");
};
