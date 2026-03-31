// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "AsyncPhysicsDebugDrawStream.h"

/**
 * Internal shared state for a registered debug draw source.
 *
 * This object stores:
 *  - the per-source pending command stream
 *  - the owning UObject (used for lifetime tracking)
 *  - synchronization primitives for safe access across threads
 *
 * It is intentionally hidden behind FAsyncPhysicsDebugDrawSource.
 */
struct FAsyncPhysicsDebugDrawSourceData
{
	FCriticalSection Mutex;
	FAsyncPhysicsDebugDrawStream PendingStream;
	TWeakObjectPtr<UObject> Owner;
	TAtomic<bool> bMarkedForRemoval = false;
};
