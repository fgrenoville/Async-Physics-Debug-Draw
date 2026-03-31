// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "AsyncPhysicsDebugDrawCommand.h"
#include "AsyncPhysicsDebugDrawLog.h"

/**
 * Bounded command buffer for a single debug draw source.
 *
 * The stream uses an inline allocator and a hard command cap to keep the cost
 * predictable and to avoid runaway debug spam.
 *
 * Notes:
 *  - Commands beyond MaxCommands are dropped.
 *  - This is an internal transport container, not part of the public API.
 */
struct FAsyncPhysicsDebugDrawStream
{
	static constexpr int32 MaxCommands = 512;

	TArray<FAsyncPhysicsDebugDrawCommand, TInlineAllocator<MaxCommands>> Commands;

	void Reset()
	{
#if !UE_BUILD_SHIPPING
		Commands.Reset();
#endif
	}

	void Add(const FAsyncPhysicsDebugDrawCommand& Cmd)
	{
#if !UE_BUILD_SHIPPING
		if (Commands.Num() < MaxCommands)
			Commands.Add(Cmd);
		else
			UE_LOG(LogAsyncPhysicsDebugDraw, Warning, TEXT("[APDD] Command buffer full, dropping draw."));
#endif
	}
};
