// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LightBatchUpDataValueEdMode.h"
#include "LightBatchUpDataValueEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FLightBatchUpDataValueEdMode::EM_LightBatchUpDataValueEdModeId = TEXT("EM_LightBatchUpDataValueEdMode");

FLightBatchUpDataValueEdMode::FLightBatchUpDataValueEdMode()
{

}

FLightBatchUpDataValueEdMode::~FLightBatchUpDataValueEdMode()
{

}

void FLightBatchUpDataValueEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FLightBatchUpDataValueEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FLightBatchUpDataValueEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FLightBatchUpDataValueEdMode::UsesToolkits() const
{
	return true;
}




