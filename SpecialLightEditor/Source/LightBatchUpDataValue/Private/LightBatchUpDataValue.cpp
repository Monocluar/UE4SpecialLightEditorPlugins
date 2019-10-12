// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LightBatchUpDataValue.h"
#include "LightBatchUpDataValueEdMode.h"

#define LOCTEXT_NAMESPACE "FLightBatchUpDataValueModule"

void FLightBatchUpDataValueModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FLightBatchUpDataValueEdMode>(FLightBatchUpDataValueEdMode::EM_LightBatchUpDataValueEdModeId, LOCTEXT("LightBatchUpDataValueEdModeName", "批量修改灯光"), FSlateIcon(), true);
}

void FLightBatchUpDataValueModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FLightBatchUpDataValueEdMode::EM_LightBatchUpDataValueEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLightBatchUpDataValueModule, LightBatchUpDataValue)