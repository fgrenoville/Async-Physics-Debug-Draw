// Copyright (c) [2026] [Federico Grenoville]

#include "AsyncPhysicsDebugDrawSettings.h"

#include "HAL/IConsoleManager.h"

static bool G_APDD_Enable = true;
static float G_APDD_DurationScale = 1.0f;
static FString G_APDD_Cats = TEXT("All");

static FAutoConsoleVariableRef CVarApdDebugDrawEnable(
	TEXT("APDD.Enable"),
	G_APDD_Enable,
	TEXT("Enable/disable async physics debug draw."),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarApdDebugDrawDurationScale(
	TEXT("APDD.DurationScale"),
	G_APDD_DurationScale,
	TEXT("Global duration multiplier for async physics debug draw."),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarApdDebugDrawCats(
	TEXT("APDD.Cats"),
	G_APDD_Cats,
	TEXT("Enabled debug draw categories. Use All, None, or comma-separated names."),
	ECVF_Default
);
