// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenderLab.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#include "RenderLabViewExtension.h"
#include "RenderingThread.h"
#include "SceneViewExtension.h"
#define LOCTEXT_NAMESPACE "FRenderLabModule"

void FRenderLabModule::StartupModule()
{
	const TSharedPtr<IPlugin> Plugin =
        IPluginManager::Get().FindPlugin(TEXT("RenderLab"));

    checkf(Plugin.IsValid(), TEXT("RenderLab plugin was not found"));

    const FString ShaderDirectory = FPaths::ConvertRelativePathToFull(
        FPaths::Combine(
            Plugin->GetBaseDir(),
            TEXT("Source/RenderLab/Shaders")
        )
    );

    const FString VirtualShaderDirectory = TEXT("/Plugin/RenderLab");

    // 防止 Live Coding 或模块重载时重复添加相同映射。
    if (!AllShaderSourceDirectoryMappings().Contains(VirtualShaderDirectory))
    {
        AddShaderSourceDirectoryMapping(
            VirtualShaderDirectory,
            ShaderDirectory
        );
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("RenderLab shader directory: %s"),
        *ShaderDirectory
    );
	UE_LOG(LogTemp, Log, TEXT("RenderLab module started."));

    if (GEngine)
    {
        ViewExtension =
            FSceneViewExtensions::NewExtension<
            FRenderLabViewExtension>();
    }
    else
    {
        PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddLambda(
            [this]()
            {
                ViewExtension =
                    FSceneViewExtensions::NewExtension<
                    FRenderLabViewExtension>();
                UE_LOG(
                    LogTemp,
                    Log,
                    TEXT("RenderLab view extension registered after engine init."));
			});
    }


    UE_LOG(
        LogTemp,
        Log,
        TEXT("RenderLab view extension registered."));

}



void FRenderLabModule::ShutdownModule()
{
    if (PostEngineInitHandle.IsValid())
    {
        FCoreDelegates::OnPostEngineInit.Remove(
            PostEngineInitHandle);

        PostEngineInitHandle.Reset();
    }
    if (ViewExtension.IsValid())
    {
        FlushRenderingCommands();
        ViewExtension.Reset();
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRenderLabModule, RenderLab)