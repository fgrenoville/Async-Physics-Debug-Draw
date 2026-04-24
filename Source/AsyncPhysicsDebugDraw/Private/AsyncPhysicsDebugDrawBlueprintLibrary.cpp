// Copyright (c) [2026] [Federico Grenoville]

#include "AsyncPhysicsDebugDrawBlueprintLibrary.h"

#include "AsyncPhysicsDebugDraw.h"
#include "AsyncPhysicsDebugDrawSubsystem.h"

FAsyncPhysicsDebugDrawCategoryHandle UAsyncPhysicsDebugDrawBlueprintLibrary::FindCategoryHandle(const FName CategoryName)
{
	return FAsyncPhysicsDebugDrawModule::Get().FindCategoryHandle(CategoryName);
}

bool UAsyncPhysicsDebugDrawBlueprintLibrary::IsCategoryHandleValid(const FAsyncPhysicsDebugDrawCategoryHandle Handle)
{
	return Handle.IsValid();
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugLine(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Start, const FVector End, const float Thickness,
	const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddLineForOwner(Owner, Category, Start, End, Thickness, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugLineExplicit(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Start, const FVector End, const FLinearColor Color,
	const float Thickness, const float Duration, const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddLineForOwner(Owner, Category, Start, End, Color.ToFColor(true), Thickness, Duration, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugSphere(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Center, const float Radius, const float Thickness,
	const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddSphereForOwner(Owner, Category, Center, Radius, Thickness, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugSphereExplicit(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Center, const float Radius, const FLinearColor Color,
	const float Thickness, const float Duration, const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddSphereForOwner(Owner, Category, Center, Radius, Color.ToFColor(true), Thickness, Duration, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugArrow(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Start, const FVector End, const float ArrowSize,
	const float Thickness, const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddArrowForOwner(Owner, Category, Start, End, ArrowSize, Thickness, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugArrowExplicit(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Start, const FVector End, const float ArrowSize,
	const FLinearColor Color, const float Thickness, const float Duration, const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddArrowForOwner(Owner, Category, Start, End, ArrowSize, Color.ToFColor(true), Thickness, Duration, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugBox(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Center, const FVector HalfExt, const FRotator& R,
	const float Thickness, const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddBoxForOwner(Owner, Category, Center, HalfExt, R, Thickness, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::AddDebugBoxExplicit(UObject* WorldContextObject, UObject* Owner,
	const FAsyncPhysicsDebugDrawCategoryHandle Category, const FVector Center, const FVector HalfExt, const FRotator& R,
	const FLinearColor Color, const float Thickness, const float Duration, const bool bPersistent)
{
	if (!WorldContextObject || !Owner || !Category.IsValid())
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject)
		.AddBoxForOwner(Owner, Category, Center, HalfExt, R, Color.ToFColor(true), Thickness, Duration, bPersistent);
}

void UAsyncPhysicsDebugDrawBlueprintLibrary::UnregisterOwner(UObject* WorldContextObject, UObject* Owner)
{
	if (!WorldContextObject || !Owner)
		return;

	UAsyncPhysicsDebugDrawSubsystem::Get(WorldContextObject).UnregisterOwner(Owner);
}
