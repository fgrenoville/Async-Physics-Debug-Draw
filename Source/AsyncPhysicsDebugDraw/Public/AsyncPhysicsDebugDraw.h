// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "AsyncPhysicsDebugDrawCategoryHandle.h"
#include "AsyncPhysicsDebugDrawCategoryRegistry.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"

/**
 * Runtime module for Async Physics Debug Draw.
 *
 * Responsibilities:
 *  - Own the category registry built from project settings / data assets.
 *  - Expose lightweight category handles to gameplay code.
 *  - Evaluate runtime filtering through console variables.
 *  - Provide shared runtime state used by the draw subsystem and draw sources.
 *
 * Notes:
 *  - Category handles are compact runtime identifiers, not stable asset references.
 *  - Rebuilding the category registry may invalidate previously resolved handles.
 *  - Project Settings are applied as startup defaults and synchronized into runtime CVars.
 */
class ASYNCPHYSICSDEBUGDRAW_API FAsyncPhysicsDebugDrawModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FAsyncPhysicsDebugDrawModule& Get();

	FAsyncPhysicsDebugDrawCategoryHandle FindCategoryHandle(const FName Name) const;
	bool IsCategoryEnabled(const FAsyncPhysicsDebugDrawCategoryHandle Handle) const;
	bool IsEnabled() const;
	float GetDurationScale() const;
	bool GetCategoryDefaults(const FAsyncPhysicsDebugDrawCategoryHandle Handle, FColor& OutColor, float& OutLifetime) const;
	void RefreshFromSettings();
	
private:
	FAsyncPhysicsDebugDrawCategoryRegistry Registry;
	mutable FRWLock RegistryLock;
	
	TAtomic<uint64> DrawEnabledMask { 0 };

	IConsoleVariable* DrawCatsCVar = nullptr;
	IConsoleVariable* EnableCVar = nullptr;
	IConsoleVariable* DurationScaleCVar = nullptr;
	
	static uint64 ParseMaskFromCsv(const FString& Csv, const FAsyncPhysicsDebugDrawCategoryRegistry& Registry);
	void RebuildDrawMaskFromCurrentCVars();
	void OnDrawCatsChanged(IConsoleVariable* Var);
	void RegisterConsoleCallbacks();
	void UnregisterConsoleCallbacks();
};
