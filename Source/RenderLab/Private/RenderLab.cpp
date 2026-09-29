// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenderLab.h"

#define LOCTEXT_NAMESPACE "FRenderLabModule"

void FRenderLabModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("RenderLab module started."));
}

void FRenderLabModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenderLabModule, RenderLab)