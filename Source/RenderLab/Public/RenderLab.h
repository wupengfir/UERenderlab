// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FRenderLabModule : public IModuleInterface
{
private:
	TSharedPtr<
		class FRenderLabViewExtension,
		ESPMode::ThreadSafe> ViewExtension;
public:
	FDelegateHandle PostEngineInitHandle;
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
