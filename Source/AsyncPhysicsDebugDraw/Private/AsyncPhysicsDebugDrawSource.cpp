// Copyright (c) [2026] [Federico Grenoville]

#include "AsyncPhysicsDebugDrawSource.h"

#include "AsyncPhysicsDebugDraw.h"
#include "AsyncPhysicsDebugDrawCommand.h"
#include "AsyncPhysicsDebugDrawSourceData.h"

void FAsyncPhysicsDebugDrawSource::Reset() const
{
	if (!Data.IsValid())
		return;

	FScopeLock Lock(&Data->Mutex);
	Data->PendingStream.Reset();
}

void FAsyncPhysicsDebugDrawSource::AddDebugLine(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B,
	const float Thickness, const bool bPersistent) const
{
	FColor DefaultColor = FColor::White;
	float DefaultLifetime = 0.0f;

	FAsyncPhysicsDebugDrawModule::Get().GetCategoryDefaults(Category, DefaultColor, DefaultLifetime);

	AddDebugLine(Category, A, B, DefaultColor, Thickness, DefaultLifetime, bPersistent);
}

void FAsyncPhysicsDebugDrawSource::AddDebugLine(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B,
	const FColor Color, const float Thickness, const float Duration, const bool bPersistent) const
{
	if (!Data.IsValid())
		return;

	FAsyncPhysicsDebugDrawCommand Cmd = FAsyncPhysicsDebugDrawCommand::MakeDebugLine(A, B, Color, Thickness, Duration, Category, bPersistent);

	FScopeLock Lock(&Data->Mutex);
	Data->PendingStream.Add(Cmd);
}

void FAsyncPhysicsDebugDrawSource::AddDebugSphere(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& Center,
	const float Radius, const float Thickness, const bool bPersistent) const
{
	FColor DefaultColor = FColor::White;
	float DefaultLifetime = 0.0f;

	FAsyncPhysicsDebugDrawModule::Get().GetCategoryDefaults(Category, DefaultColor, DefaultLifetime);

	AddDebugSphere(Category, Center, Radius, DefaultColor, Thickness, DefaultLifetime, bPersistent);
}

void FAsyncPhysicsDebugDrawSource::AddDebugSphere(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& Center,
	const float Radius, const FColor Color, const float Thickness, const float Duration, const bool bPersistent) const
{
	if (!Data.IsValid())
		return;

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugSphere(Center, Radius, Color, Thickness, Duration, Category, bPersistent);

	FScopeLock Lock(&Data->Mutex);
	Data->PendingStream.Add(Cmd);
}

void FAsyncPhysicsDebugDrawSource::AddDebugArrow(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A,
	const FVector& B, const float ArrowSize, const float Thickness, const bool bPersistent) const
{
	FColor DefaultColor = FColor::White;
	float DefaultLifetime = 0.0f;

	FAsyncPhysicsDebugDrawModule::Get().GetCategoryDefaults(Category, DefaultColor, DefaultLifetime);

	AddDebugArrow(Category, A, B, ArrowSize, DefaultColor, Thickness, DefaultLifetime, bPersistent);
}

void FAsyncPhysicsDebugDrawSource::AddDebugArrow(const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector& A, const FVector& B,
	const float ArrowSize, const FColor Color, const float Thickness, const float Duration, const bool bPersistent) const
{
	if (!Data.IsValid())
		return;

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugArrow(A, B, ArrowSize, Color, Thickness, Duration, Category, bPersistent);

	FScopeLock Lock(&Data->Mutex);
	Data->PendingStream.Add(Cmd);
}
