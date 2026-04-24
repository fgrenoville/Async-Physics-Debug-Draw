// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "AsyncPhysicsDebugDraw.h"
#include "AsyncPhysicsDebugDrawCommand.h"
#include "AsyncPhysicsDebugDrawStream.h"
#include "DrawDebugHelpers.h"

/**
 * Game Thread executor for buffered debug draw commands.
 *
 * The executor applies:
 *  - global enable/disable state
 *  - category filtering through the runtime mask
 *  - global duration scaling
 *
 * Only this stage is allowed to call DrawDebugHelpers.
 */
struct FAsyncPhysicsDebugDrawExec
{
	static void Execute(const UWorld* World, const FAsyncPhysicsDebugDrawStream& Stream)
	{
#if !UE_BUILD_SHIPPING
		if (!World)
			return;

		const FAsyncPhysicsDebugDrawModule& Mod = FAsyncPhysicsDebugDrawModule::Get();
		
		if (!Mod.IsEnabled())
			return;

		const float DurationScale = Mod.GetDurationScale();

		for (const FAsyncPhysicsDebugDrawCommand& Cmd : Stream.Commands)
		{
			if (!Mod.IsCategoryEnabled(Cmd.Category))
				continue;

			const float ScaledDuration = Cmd.Duration * DurationScale;

			switch (Cmd.Shape)
			{
				case EAsyncPhysicsDebugDrawShape::Line:
					DrawDebugLine(World, Cmd.P0, Cmd.P1, Cmd.Color, Cmd.bPersistent, ScaledDuration, 0, Cmd.Thickness);
					break;

				case EAsyncPhysicsDebugDrawShape::Sphere:
					DrawDebugSphere(World, Cmd.P0, Cmd.Radius, 16, Cmd.Color, Cmd.bPersistent, ScaledDuration, 0, Cmd.Thickness);
					break;

				case EAsyncPhysicsDebugDrawShape::Arrow:
					DrawDebugDirectionalArrow(World, Cmd.P0, Cmd.P1, Cmd.ArrowSize, Cmd.Color, Cmd.bPersistent, ScaledDuration, 0, Cmd.Thickness);
					break;

				case EAsyncPhysicsDebugDrawShape::Box:
					DrawDebugBox(World, Cmd.P0, Cmd.HalfExtent, Cmd.Rot.Quaternion(), Cmd.Color, Cmd.bPersistent, ScaledDuration, 0, Cmd.Thickness);
					break;
			}
		}
#endif
	}
};
