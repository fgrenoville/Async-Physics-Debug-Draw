// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "AsyncPhysicsDebugDrawCategoryHandle.h"
#include "AsyncPhysicsDebugDrawShape.h"

/**
 * Internal representation of a single debug draw command.
 *
 * Commands are recorded by draw sources and later consumed on the Game Thread.
 * This struct intentionally stores only lightweight runtime data:
 *  - primitive type
 *  - category handle
 *  - geometry parameters
 *  - visual parameters
 *
 * The static factory helpers keep command construction consistent across sources.
 */
struct FAsyncPhysicsDebugDrawCommand
{
	EAsyncPhysicsDebugDrawShape Shape = EAsyncPhysicsDebugDrawShape::Line;
	FAsyncPhysicsDebugDrawCategoryHandle Category;

	FVector P0 = FVector::ZeroVector;
	FVector P1 = FVector::ZeroVector;
	float Radius = 0.f;
	float ArrowSize = 20.f;

	FColor Color = FColor::White;
	float Thickness = 1.f;
	float Duration = 0.f;
	bool bPersistent = false;

	static FAsyncPhysicsDebugDrawCommand MakeDebugLine(const FVector& A, const FVector& B, const FColor Color, const float Thickness,
		const float Duration, const FAsyncPhysicsDebugDrawCategoryHandle Category, const bool bInPersistent = false)
	{
		FAsyncPhysicsDebugDrawCommand Cmd;
		Cmd.Shape = EAsyncPhysicsDebugDrawShape::Line;
		Cmd.P0 = A;
		Cmd.P1 = B;
		Cmd.Color = Color;
		Cmd.Thickness = Thickness;
		Cmd.Duration = Duration;
		Cmd.Category = Category;
		Cmd.bPersistent = bInPersistent;
		return Cmd;
	}

	static FAsyncPhysicsDebugDrawCommand MakeDebugSphere(const FVector& Center, const float InRadius, const FColor InColor, const float InThickness,
		const float InDuration, const FAsyncPhysicsDebugDrawCategoryHandle InCategory, const bool bInPersistent = false)
	{
		FAsyncPhysicsDebugDrawCommand Cmd;
		Cmd.Shape = EAsyncPhysicsDebugDrawShape::Sphere;
		Cmd.Category = InCategory;
		Cmd.P0 = Center;
		Cmd.Radius = InRadius;
		Cmd.Color = InColor;
		Cmd.Thickness = InThickness;
		Cmd.Duration = InDuration;
		Cmd.bPersistent = bInPersistent;
		return Cmd;
	}

	static FAsyncPhysicsDebugDrawCommand MakeDebugArrow(const FVector& A, const FVector& B, const float InArrowSize, const FColor InColor,
		const float InThickness, const float InDuration, const FAsyncPhysicsDebugDrawCategoryHandle InCategory, const bool bInPersistent = false)
	{
		FAsyncPhysicsDebugDrawCommand Cmd;
		Cmd.Shape = EAsyncPhysicsDebugDrawShape::Arrow;
		Cmd.Category = InCategory;
		Cmd.P0 = A;
		Cmd.P1 = B;
		Cmd.ArrowSize = InArrowSize;
		Cmd.Color = InColor;
		Cmd.Thickness = InThickness;
		Cmd.Duration = InDuration;
		Cmd.bPersistent = bInPersistent;
		return Cmd;
	}
};
