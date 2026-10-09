#pragma once

#include "SceneViewExtension.h"
class FRDGTexture;
class FRenderLabViewExtension final
    : public FSceneViewExtensionBase
{
public:

    FRDGTexture* CustomDepth;

    explicit FRenderLabViewExtension(
        const FAutoRegister& AutoRegister)
        : FSceneViewExtensionBase(AutoRegister)
    {
    }

    virtual void SetupViewFamily(
        FSceneViewFamily& InViewFamily) override
    {
    }

    virtual void SetupView(
        FSceneViewFamily& InViewFamily,
        FSceneView& InView) override
    {
    }

    virtual void BeginRenderViewFamily(
        FSceneViewFamily& InViewFamily) override
    {
    }

    virtual void SubscribeToPostProcessingPass(
        EPostProcessingPass Pass,
        const FSceneView& InView,
        FAfterPassCallbackDelegateArray& InOutPassCallbacks,
        bool bIsPassEnabled) override;


    void PostRenderBasePassDeferred_RenderThread(
        FRDGBuilder& GraphBuilder, 
        FSceneView& InView, 
        const FRenderTargetBindingSlots& RenderTargets, 
        TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextures) override;
private:
    FScreenPassTexture PostProcessAfterTonemap_RenderThread(
        FRDGBuilder& GraphBuilder,
        const FSceneView& View,
        const FPostProcessMaterialInputs& Inputs);
};