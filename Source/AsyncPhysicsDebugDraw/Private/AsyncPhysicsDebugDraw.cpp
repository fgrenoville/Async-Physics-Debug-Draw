// Copyright (c) [2026] [Federico Grenoville]

#include "AsyncPhysicsDebugDraw.h"

#include "AsyncPhysicsDebugDrawCategoryRegistry.h"
#include "AsyncPhysicsDebugDrawSettings.h"
#include "AsyncPhysicsDebugDrawLog.h"

#define LOCTEXT_NAMESPACE "FAsyncPhysicsDebugDrawModule"

void FAsyncPhysicsDebugDrawModule::StartupModule()
{
	RegisterConsoleCallbacks();
	
	RefreshFromSettings();
}

void FAsyncPhysicsDebugDrawModule::ShutdownModule()
{
	UnregisterConsoleCallbacks();
}

FAsyncPhysicsDebugDrawModule& FAsyncPhysicsDebugDrawModule::Get()
{
	return FModuleManager::LoadModuleChecked<FAsyncPhysicsDebugDrawModule>("AsyncPhysicsDebugDraw");
}

/**
 * Resolve a runtime category handle from an authored category name.
 *
 * Handles are intended to be cached by gameplay code for repeated use.
 * However, handles are only valid for the current registry state and should be
 * re-resolved after a category registry refresh.
 */
FAsyncPhysicsDebugDrawCategoryHandle FAsyncPhysicsDebugDrawModule::FindCategoryHandle(const FName Name) const
{
	FAsyncPhysicsDebugDrawCategoryHandle Handle;

	FReadScopeLock Lock(RegistryLock);
	
	if (const uint8* Id = Registry.FindIdByName(Name))
	{
		Handle.Id = *Id;
	}

	return Handle;
}

bool FAsyncPhysicsDebugDrawModule::IsCategoryEnabled(const FAsyncPhysicsDebugDrawCategoryHandle Handle) const
{
	if (!Handle.IsValid())
		return false;

	const uint64 Mask = DrawEnabledMask.Load();
	return (Mask & (1ull << Handle.Id)) != 0;
}

bool FAsyncPhysicsDebugDrawModule::IsEnabled() const
{
	return EnableCVar ? EnableCVar->GetBool() : true;
}

float FAsyncPhysicsDebugDrawModule::GetDurationScale() const
{
	return DurationScaleCVar ? DurationScaleCVar->GetFloat() : 1.0f;
}

bool FAsyncPhysicsDebugDrawModule::GetCategoryDefaults(const FAsyncPhysicsDebugDrawCategoryHandle Handle, FColor& OutColor,
	float& OutLifetime) const
{
	if (!Handle.IsValid())
		return false;

	FReadScopeLock Lock(RegistryLock);
	
	const FAsyncPhysicsDebugDrawCategoryInfo* Info = Registry.FindInfoById(Handle.Id);
	if (!Info)
		return false;

	OutColor = Info->DefaultColor;
	OutLifetime = Info->DefaultLifetime;
	return true;
}

/**
 * Reload runtime state from project settings.
 *
 * This function:
 *  - synchronizes Project Settings into the backing runtime CVars
 *  - rebuilds the category registry from the configured data asset
 *  - rebuilds the active category mask from the current category filter string
 *
 * On failure, the registry is cleared and drawing is effectively disabled.
 */
void FAsyncPhysicsDebugDrawModule::RefreshFromSettings()
{
	const UAsyncPhysicsDebugDrawSettings* Settings = GetDefault<UAsyncPhysicsDebugDrawSettings>();

	if (!Settings)
	{
		{
			FWriteScopeLock Lock(RegistryLock);
			Registry.Reset();
		}

		DrawEnabledMask.Store(0);
		UE_LOG(LogAsyncPhysicsDebugDraw, Warning, TEXT("[APDD] Refresh failed: settings not found. Registry cleared, draw mask reset."));
		return;
	}

	// Sync project settings into runtime CVars; console overrides applied after this take precedence.
	if (EnableCVar)
		EnableCVar->Set(Settings->bEnableDraw ? 1 : 0, ECVF_SetByConsole);

	if (DurationScaleCVar)
		DurationScaleCVar->Set(Settings->DurationMultiplier, ECVF_SetByConsole);

	if (DrawCatsCVar)
		DrawCatsCVar->Set(*Settings->DrawCategories, ECVF_SetByConsole);

	UAsyncPhysicsDebugDrawCategorySet* CategorySet = Settings->CategorySet.LoadSynchronous();
	if (!CategorySet)
	{
		{
			FWriteScopeLock Lock(RegistryLock);
			Registry.Reset();
		}

		DrawEnabledMask.Store(0);
		UE_LOG(LogAsyncPhysicsDebugDraw, Warning, TEXT("[APDD] Refresh failed: CategorySet not found. Registry cleared, draw mask reset."));
		return;
	}

	const FString Csv = DrawCatsCVar ? DrawCatsCVar->GetString() : FString();

	uint64 NewMask = 0;
	int32 NumCategories = 0;

	{
		FWriteScopeLock Lock(RegistryLock);

		Registry.BuildFromAsset(CategorySet);
		NumCategories = Registry.Num();

		if (DrawCatsCVar)
			NewMask = ParseMaskFromCsv(Csv, Registry);
	}

	DrawEnabledMask.Store(NewMask);

	UE_LOG(LogAsyncPhysicsDebugDraw, Verbose, TEXT("[APDD] CategorySet loaded. Num categories: %d"), NumCategories);
	UE_LOG(LogAsyncPhysicsDebugDraw, Verbose, TEXT("[APDD] Cats='%s' -> Mask=%llu"), *Csv, NewMask);
}

uint64 FAsyncPhysicsDebugDrawModule::ParseMaskFromCsv(const FString& Csv, const FAsyncPhysicsDebugDrawCategoryRegistry& Registry)
{
	TArray<FString> Tokens;
	Csv.ParseIntoArray(Tokens, TEXT(","), true);

	uint64 Mask = 0;

	for (FString Token : Tokens)
	{
		Token.TrimStartAndEndInline();
		Token.ToLowerInline();

		if (Token == TEXT("all") || Token == TEXT("*"))
			return ~0ull;

		if (Token == TEXT("none"))
			return 0ull;

		if (const uint8* Id = Registry.FindId(Token))
			Mask |= (1ull << *Id);
	}

	return Mask;
}

void FAsyncPhysicsDebugDrawModule::RebuildDrawMaskFromCurrentCVars()
{
	if (!DrawCatsCVar)
	{
		DrawEnabledMask.Store(0);
		UE_LOG(LogAsyncPhysicsDebugDraw, Warning, TEXT("[APDD] DrawCatsCVar NOT FOUND"));
		return;
	}

	const FString Csv = DrawCatsCVar->GetString();

	uint64 NewMask = 0;
	{
		FReadScopeLock Lock(RegistryLock);
		NewMask = ParseMaskFromCsv(Csv, Registry);
	}

	DrawEnabledMask.Store(NewMask);

	UE_LOG(LogAsyncPhysicsDebugDraw, Verbose, TEXT("[APDD] Cats='%s' -> Mask=%llu"), *Csv, NewMask);
}

void FAsyncPhysicsDebugDrawModule::OnDrawCatsChanged(IConsoleVariable* Var)
{
	RebuildDrawMaskFromCurrentCVars();
}

void FAsyncPhysicsDebugDrawModule::RegisterConsoleCallbacks()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();

	DrawCatsCVar = ConsoleManager.FindConsoleVariable(TEXT("APDD.Cats"));
	EnableCVar = ConsoleManager.FindConsoleVariable(TEXT("APDD.Enable"));
	DurationScaleCVar = ConsoleManager.FindConsoleVariable(TEXT("APDD.DurationScale"));
	
	if (DrawCatsCVar)
	{
		DrawCatsCVar->SetOnChangedCallback(
			FConsoleVariableDelegate::CreateRaw(this, &FAsyncPhysicsDebugDrawModule::OnDrawCatsChanged)
		);
	}
}

void FAsyncPhysicsDebugDrawModule::UnregisterConsoleCallbacks()
{
	if (DrawCatsCVar)
	{
		DrawCatsCVar->SetOnChangedCallback(FConsoleVariableDelegate());
		DrawCatsCVar = nullptr;
	}

	EnableCVar = nullptr;
	DurationScaleCVar = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncPhysicsDebugDrawModule, AsyncPhysicsDebugDraw)
