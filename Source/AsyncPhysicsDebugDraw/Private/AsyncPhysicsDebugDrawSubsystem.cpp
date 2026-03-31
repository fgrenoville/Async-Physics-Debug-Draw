// Copyright (c) [2026] [Federico Grenoville]

#include "AsyncPhysicsDebugDrawSubsystem.h"

#include "AsyncPhysicsDebugDrawExec.h"
#include "AsyncPhysicsDebugDrawSourceData.h"
#include "AsyncPhysicsDebugDrawStream.h"
#include "Engine/World.h"

UAsyncPhysicsDebugDrawSubsystem& UAsyncPhysicsDebugDrawSubsystem::Get(const UObject* WorldContextObject)
{
	//check(WorldContextObject);
	const UWorld* World = WorldContextObject->GetWorld();
	//check(World);

	UAsyncPhysicsDebugDrawSubsystem* Subsystem = World->GetSubsystem<UAsyncPhysicsDebugDrawSubsystem>();
	//check(Subsystem);

	return *Subsystem;
}

void UAsyncPhysicsDebugDrawSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		FAsyncPhysicsDebugDrawModule::Get().RefreshFromSettings();
	}
}

void UAsyncPhysicsDebugDrawSubsystem::Deinitialize()
{
	FScopeLock Lock(&SourcesMutex);
	Sources.Empty();

	Super::Deinitialize();
}

FAsyncPhysicsDebugDrawSource UAsyncPhysicsDebugDrawSubsystem::RegisterSource(UObject* Owner)
{
	//check(Owner);

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> NewSource =
		MakeShared<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>();

	NewSource->Owner = Owner;

	{
		FScopeLock Lock(&SourcesMutex);
		Sources.Add(NewSource);
	}

	return FAsyncPhysicsDebugDrawSource(NewSource);
}

void UAsyncPhysicsDebugDrawSubsystem::UnregisterSource(FAsyncPhysicsDebugDrawSource& Source)
{
	if (!Source.Data.IsValid())
		return;

	{
		FScopeLock SourceLock(&Source.Data->Mutex);
		Source.Data->bMarkedForRemoval = true;
		Source.Data->PendingStream.Reset();
	}

	{
		FScopeLock Lock(&SourcesMutex);
		Sources.RemoveAll(
			[&Source](const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>& Item)
			{
				return Item == Source.Data;
			});
	}

	Source.Data.Reset();
}

bool UAsyncPhysicsDebugDrawSubsystem::IsTickable() const
{
	return true;
}

TStatId UAsyncPhysicsDebugDrawSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UAsyncPhysicsDebugDrawSubsystem, STATGROUP_Tickables);
}

void UAsyncPhysicsDebugDrawSubsystem::AddLineForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
	const FVector& A, const FVector& B, const float Thickness, const bool bPersistent)
{
	if (!Owner)
		return;

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Source = FindOrAddSourceData(Owner);

	FColor DefaultColor = FColor::White;
	float DefaultLifetime = 0.0f;
	FAsyncPhysicsDebugDrawModule::Get().GetCategoryDefaults(Category, DefaultColor, DefaultLifetime);

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugLine(A, B, DefaultColor, Thickness, DefaultLifetime, Category, bPersistent);

	FScopeLock SourceLock(&Source->Mutex);
	Source->PendingStream.Add(Cmd);
}

void UAsyncPhysicsDebugDrawSubsystem::AddLineForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
	const FVector& A, const FVector& B, const FColor Color, const float Thickness, const float Duration, const bool bPersistent)
{
	if (!Owner)
	{
		return;
	}

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Source = FindOrAddSourceData(Owner);

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugLine(A, B, Color, Thickness, Duration, Category, bPersistent);

	FScopeLock SourceLock(&Source->Mutex);
	Source->PendingStream.Add(Cmd);
}

void UAsyncPhysicsDebugDrawSubsystem::AddSphereForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
	const FVector& Center, const float Radius, const float Thickness, const bool bPersistent)
{
	if (!Owner)
		return;

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Source = FindOrAddSourceData(Owner);

	FColor DefaultColor = FColor::White;
	float DefaultLifetime = 0.0f;
	FAsyncPhysicsDebugDrawModule::Get().GetCategoryDefaults(Category, DefaultColor, DefaultLifetime);

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugSphere(Center, Radius, DefaultColor, Thickness, DefaultLifetime, Category, bPersistent);

	FScopeLock SourceLock(&Source->Mutex);
	Source->PendingStream.Add(Cmd);
}

void UAsyncPhysicsDebugDrawSubsystem::AddSphereForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
	const FVector& Center, const float Radius, const FColor Color, const float Thickness, const float Duration, const bool bPersistent)
{
	if (!Owner)
		return;

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Source = FindOrAddSourceData(Owner);

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugSphere(Center, Radius, Color, Thickness, Duration, Category, bPersistent);

	FScopeLock SourceLock(&Source->Mutex);
	Source->PendingStream.Add(Cmd);
}

void UAsyncPhysicsDebugDrawSubsystem::AddArrowForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
	const FVector& A, const FVector& B, const float ArrowSize, const float Thickness, const bool bPersistent)
{
	if (!Owner)
		return;

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Source = FindOrAddSourceData(Owner);

	FColor DefaultColor = FColor::White;
	float DefaultLifetime = 0.0f;
	FAsyncPhysicsDebugDrawModule::Get().GetCategoryDefaults(Category, DefaultColor, DefaultLifetime);

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugArrow(A, B, ArrowSize, DefaultColor, Thickness, DefaultLifetime, Category, bPersistent);

	FScopeLock SourceLock(&Source->Mutex);
	Source->PendingStream.Add(Cmd);
}

void UAsyncPhysicsDebugDrawSubsystem::AddArrowForOwner(UObject* Owner, const FAsyncPhysicsDebugDrawCategoryHandle Category,
	const FVector& A, const FVector& B, const float ArrowSize, const FColor Color, const float Thickness, const float Duration,
	const bool bPersistent)
{
	if (!Owner)
		return;

	const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> Source = FindOrAddSourceData(Owner);

	const FAsyncPhysicsDebugDrawCommand Cmd =
		FAsyncPhysicsDebugDrawCommand::MakeDebugArrow(A, B, ArrowSize, Color, Thickness, Duration, Category, bPersistent);

	FScopeLock SourceLock(&Source->Mutex);
	Source->PendingStream.Add(Cmd);
}

void UAsyncPhysicsDebugDrawSubsystem::UnregisterOwner(const UObject* Owner)
{
	if (!Owner)
		return;

	FScopeLock Lock(&SourcesMutex);

	for (const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>& Source : Sources)
	{
		if (Source.IsValid() && Source->Owner.Get() == Owner)
		{
			Source->bMarkedForRemoval.Store(true);
			FScopeLock SourceLock(&Source->Mutex);
			Source->PendingStream.Reset();
		}
	}
}

void UAsyncPhysicsDebugDrawSubsystem::Tick(float DeltaTime)
{
	TArray<TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>> LocalSources;

	{
		FScopeLock Lock(&SourcesMutex);
		LocalSources = Sources;
	}

	const UWorld* World = GetWorld();
	if (!World)
		return;

	for (const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>& Source : LocalSources)
	{
		if (!Source.IsValid())
			continue;

		if (Source->bMarkedForRemoval)
			continue;

		if (!Source->Owner.IsValid())
			continue;

		FAsyncPhysicsDebugDrawStream LocalStream;

		{
			FScopeLock SourceLock(&Source->Mutex);
			Swap(LocalStream.Commands, Source->PendingStream.Commands);
		}

		if (LocalStream.Commands.Num() > 0)
			FAsyncPhysicsDebugDrawExec::Execute(World, LocalStream);
	}

	CompactSources();
}

TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> UAsyncPhysicsDebugDrawSubsystem::
FindOrAddSourceData(UObject* Owner)
{
	//check(Owner);

	FScopeLock Lock(&SourcesMutex);

	for (const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>& Source : Sources)
	{
		if (!Source.IsValid())
			continue;

		if (Source->bMarkedForRemoval.Load())
			continue;

		if (Source->Owner.Get() == Owner)
			return Source;
	}

	TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe> NewSource =
		MakeShared<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>();

	NewSource->Owner = Owner;
	Sources.Add(NewSource);

	return NewSource;
}

void UAsyncPhysicsDebugDrawSubsystem::CompactSources()
{
	FScopeLock Lock(&SourcesMutex);

	Sources.RemoveAll(
		[](const TSharedPtr<FAsyncPhysicsDebugDrawSourceData, ESPMode::ThreadSafe>& Source)
		{
			if (!Source.IsValid())
				return true;

			if (Source->bMarkedForRemoval)
				return true;

			return !Source->Owner.IsValid();
		});
}
