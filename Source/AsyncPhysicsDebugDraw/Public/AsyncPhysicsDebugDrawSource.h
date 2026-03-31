// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "AsyncPhysicsDebugDrawCategoryHandle.h"

struct FAsyncPhysicsDebugDrawSourceData;

/**
 * Per-owner debug draw writer.
 *
 * A source is the public-facing entry point used by gameplay code to enqueue
 * debug primitives from async physics or other simulation code without drawing
 * directly from non-Game Thread contexts.
 *
 * Design:
 *  - Sources are move-only to make ownership and lifetime clearer.
 *  - Commands are buffered internally and later consumed by the world subsystem.
 *  - The short overloads use per-category default color and lifetime.
 *
 * Typical usage:
 *  - Register a source through the subsystem in BeginPlay / initialization.
 *  - Cache category handles once.
 *  - Call AddLine / AddSphere / AddArrow from simulation code.
 *  - Unregister the source when the owner is destroyed.
 */
class ASYNCPHYSICSDEBUGDRAW_API FAsyncPhysicsDebugDrawSource
{
public:
	FAsyncPhysicsDebugDrawSource() = default;
	~FAsyncPhysicsDebugDrawSource() = default;

	FAsyncPhysicsDebugDrawSource(const FAsyncPhysicsDebugDrawSource&) = delete;
	FAsyncPhysicsDebugDrawSource& operator=(const FAsyncPhysicsDebugDrawSource&) = delete;

	FAsyncPhysicsDebugDrawSource(FAsyncPhysicsDebugDrawSource&&) = default;
	FAsyncPhysicsDebugDrawSource& operator=(FAsyncPhysicsDebugDrawSource&&) = default;
	
	bool IsValid() const
	{
		return Data.IsValid();
	}

	void Reset() const;
	
	void AddDebugLine(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B,
		const float Thickness = 1.0f, const bool bPersistent = false) const;
	void AddDebugLine(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B,
		const FColor Color, const float Thickness, const float Duration, const bool bPersistent = false) const;

	void AddDebugSphere(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& Center, const float Radius,
		const float Thickness = 1.0f, const bool bPersistent = false) const;
	void AddDebugSphere(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& Center, const float Radius,
		const FColor Color, const float Thickness, const float Duration, const bool bPersistent = false) const;

	void AddDebugArrow(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B,
		const float ArrowSize = 20.0f, const float Thickness = 1.0f, const bool bPersistent = false) const;
	void AddDebugArrow(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B, const float ArrowSize,
		const FColor Color, const float Thickness, const float Duration, const bool bPersistent = false) const;

private:
	TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Data;

	explicit FAsyncPhysicsDebugDrawSource(TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> InData)
		: Data(MoveTemp(InData))
	{ }

	friend class UAsyncPhysicsDebugDrawSubsystem;
};
