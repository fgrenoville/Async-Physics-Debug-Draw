// Copyright (c) [2026] [Federico Grenoville]

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AsyncPhysicsDebugDrawCategoryHandle.h"
#include "AsyncPhysicsDebugDrawBlueprintLibrary.generated.h"

UCLASS()
class ASYNCPHYSICSDEBUGDRAW_API UAsyncPhysicsDebugDrawBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Async Physics Debug Draw")
	static FAsyncPhysicsDebugDrawCategoryHandle FindCategoryHandle(const FName CategoryName);

	UFUNCTION(BlueprintPure, Category = "Async Physics Debug Draw")
	static bool IsCategoryHandleValid(const FAsyncPhysicsDebugDrawCategoryHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe, DisplayName = "APDD Add Line"))
	static void AddDebugLine(UObject* WorldContextObject, UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector Start, const FVector End, const float Thickness = 1.0f, const bool bPersistent = false);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe, DisplayName = "APDD Add Line Explicit"))
	static void AddDebugLineExplicit(UObject* WorldContextObject, UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector Start, const FVector End, const FLinearColor Color, const float Thickness = 1.0f, const float Duration = 0.0f,
		const bool bPersistent = false);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe, DisplayName = "APDD Add Sphere"))
	static void AddDebugSphere(UObject* WorldContextObject, UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector Center, const float Radius, const float Thickness = 1.0f, const bool bPersistent = false);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe, DisplayName = "APDD Add Sphere Explicit"))
	static void AddDebugSphereExplicit(UObject* WorldContextObject, UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector Center, const float Radius, const FLinearColor Color, const float Thickness = 1.0f, const float Duration = 0.0f,
		const bool bPersistent = false);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe, DisplayName = "APDD Add Arrow"))
	static void AddDebugArrow(UObject* WorldContextObject, UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector Start, const FVector End, const float ArrowSize = 20.0f, const float Thickness = 1.0f, const bool bPersistent = false);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe, DisplayName = "APDD Add Arrow Explicit"))
	static void AddDebugArrowExplicit(UObject* WorldContextObject, UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
		const FVector Start, const FVector End, const float ArrowSize, const FLinearColor Color, const float Thickness = 1.0f,
		const float Duration = 0.0f, const bool bPersistent = false);

	UFUNCTION(BlueprintCallable, Category = "Async Physics Debug Draw",
		meta = (WorldContext = "WorldContextObject", BlueprintThreadSafe))
	static void UnregisterOwner(UObject* WorldContextObject, UObject* Owner);
};
