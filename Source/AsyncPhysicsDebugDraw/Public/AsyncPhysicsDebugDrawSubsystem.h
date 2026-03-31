// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AsyncPhysicsDebugDrawSource.h"
#include "AsyncPhysicsDebugDrawSubsystem.generated.h"

/**
 * World subsystem responsible for consuming and executing pending debug draw commands.
 *
 * Responsibilities:
 *  - Own the collection of active debug draw sources for a given world.
 *  - Drain buffered commands on the Game Thread once per tick.
 *  - Forward the resulting primitives to the draw executor.
 *
 * This subsystem centralizes Game Thread execution so gameplay code only needs
 * to enqueue commands through sources, without manually calling any flush step.
 */
UCLASS(Category = "Async Physics Debug Draw")
class ASYNCPHYSICSDEBUGDRAW_API UAsyncPhysicsDebugDrawSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncPhysicsDebugDrawSubsystem& Get(const UObject* WorldContextObject);

	FAsyncPhysicsDebugDrawSource RegisterSource(UObject* Owner);
	void UnregisterSource(FAsyncPhysicsDebugDrawSource& Source);

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	// -------- Public helpers for Blueprint wrappers --------
	void AddLineForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector& A, const FVector& B, const float Thickness = 1.0f, const bool bPersistent = false);
	void AddLineForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector& A, const FVector& B, const FColor Color, const float Thickness, const float Duration,
		const bool bPersistent = false);
	void AddSphereForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector& Center, const float Radius, const float Thickness = 1.0f, const bool bPersistent = false);
	void AddSphereForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector& Center, const float Radius, const FColor Color, const float Thickness, const float Duration,
		const bool bPersistent = false);
	void AddArrowForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector& A, const FVector& B, const float ArrowSize = 20.0f, const float Thickness = 1.0f,
		const bool bPersistent = false);
	void AddArrowForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector& A, const FVector& B, const float ArrowSize, const FColor Color, const float Thickness,
		const float Duration, const bool bPersistent = false);
	void UnregisterOwner(const UObject* Owner);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	mutable FCriticalSection SourcesMutex;
	TArray<TSharedPtr<struct FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>> Sources;

	TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> FindOrAddSourceData(UObject* Owner);
	
	void CompactSources();
};
