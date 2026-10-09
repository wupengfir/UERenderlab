#include "RenderLabViewExtension.h"

#include "RenderLabShaders.h"

#include "GlobalShader.h"
#include "HAL/IConsoleManager.h"
#include "PostProcess/PostProcessMaterialInputs.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "ScreenPass.h"

static TAutoConsoleVariable<int32> CVarRenderLabEnabled(
    TEXT("r.RenderLab.Enabled"),
    1,
    TEXT("Enable the RenderLab post-process pass."),
    ECVF_RenderThreadSafe);

static TAutoConsoleVariable<float> CVarRenderLabStrength(
    TEXT("r.RenderLab.Strength"),
    1.0f,
    TEXT("RenderLab post-process strength from 0 to 1."),
    ECVF_RenderThreadSafe);

void FRenderLabViewExtension::SubscribeToPostProcessingPass(
    EPostProcessingPass Pass,
    const FSceneView& InView,
    FAfterPassCallbackDelegateArray& InOutPassCallbacks,
    bool bIsPassEnabled)
{
    const bool bShouldSubscribe =
        Pass == EPostProcessingPass::Tonemap &&
        bIsPassEnabled &&
        CVarRenderLabEnabled.GetValueOnRenderThread() != 0;

    if (!bShouldSubscribe)
    {
        return;
    }

    InOutPassCallbacks.Add(
        FAfterPassCallbackDelegate::CreateRaw(
            this,
            &FRenderLabViewExtension::
            PostProcessAfterTonemap_RenderThread));
}

void FRenderLabViewExtension::PostRenderBasePassDeferred_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView, const FRenderTargetBindingSlots& RenderTargets, TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextures)
{
    FRDGTextureRef SceneDepth = RenderTargets.DepthStencil.GetTexture();
    const FRDGTextureDesc OutputDesc =
        FRDGTextureDesc::Create2D(
            SceneDepth->Desc.Extent,
            PF_R32_FLOAT,
            FClearValueBinding::None,
            TexCreate_ShaderResource |
            TexCreate_UAV);

    CustomDepth =
        GraphBuilder.CreateTexture(
            OutputDesc,
            TEXT("RenderLab.SceneDepthOutput"));

    FRenderLabDepthCS::FParameters* PassParameters =
        GraphBuilder.AllocParameters<
        FRenderLabDepthCS::FParameters>();

    PassParameters->InputTexture = SceneDepth;

    PassParameters->OutputTexture =
        GraphBuilder.CreateUAV(CustomDepth);

    PassParameters->ViewRectMin = {0,0};
    PassParameters->ViewSize = SceneDepth->Desc.Extent;
    
    TShaderMapRef<FRenderLabDepthCS> ComputeShader(
        GetGlobalShaderMap(InView.GetFeatureLevel()));

    const FIntVector GroupCount(
        FMath::DivideAndRoundUp(PassParameters->ViewSize.X, 8),
        FMath::DivideAndRoundUp(PassParameters->ViewSize.Y, 8),
        1);

    FComputeShaderUtils::AddPass(
        GraphBuilder,
        RDG_EVENT_NAME("RenderLab.PostProcess"),
        ComputeShader,
        PassParameters,
        GroupCount);
}

FScreenPassTexture
FRenderLabViewExtension::PostProcessAfterTonemap_RenderThread(
    FRDGBuilder& GraphBuilder,
    const FSceneView& View,
    const FPostProcessMaterialInputs& Inputs)
{
    const FScreenPassTexture SceneColor =
        FScreenPassTexture::CopyFromSlice(
            GraphBuilder,
            Inputs.GetInput(
                EPostProcessMaterialInput::SceneColor));

    check(SceneColor.IsValid());

    const FIntPoint ViewSize = SceneColor.ViewRect.Size();

    const FRDGTextureDesc OutputDesc =
        FRDGTextureDesc::Create2D(
            SceneColor.Texture->Desc.Extent,
            PF_FloatRGBA,
            FClearValueBinding::None,
            TexCreate_ShaderResource |
            TexCreate_UAV);

    FRDGTextureRef ComputeOutput =
        GraphBuilder.CreateTexture(
            OutputDesc,
            TEXT("RenderLab.PostProcessOutput"));

    FRenderLabPostProcessCS::FParameters* PassParameters =
        GraphBuilder.AllocParameters<
        FRenderLabPostProcessCS::FParameters>();

    PassParameters->InputTexture = CustomDepth;// SceneColor.Texture;
	PassParameters->InputTextureSampler_clamp_linear = TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
    PassParameters->OutputTexture =
        GraphBuilder.CreateUAV(ComputeOutput);

    PassParameters->ViewRectMin =
        SceneColor.ViewRect.Min;

    PassParameters->ViewSize = ViewSize;

    PassParameters->Strength = FMath::Clamp(
        CVarRenderLabStrength.GetValueOnRenderThread(),
        0.0f,
        1.0f);

    TShaderMapRef<FRenderLabPostProcessCS> ComputeShader(
        GetGlobalShaderMap(View.GetFeatureLevel()));

    const FIntVector GroupCount(
        FMath::DivideAndRoundUp(ViewSize.X, 8),
        FMath::DivideAndRoundUp(ViewSize.Y, 8),
        1);

    FComputeShaderUtils::AddPass(
        GraphBuilder,
        RDG_EVENT_NAME("RenderLab.PostProcess"),
        ComputeShader,
        PassParameters,
        GroupCount);

    const FScreenPassTexture Result(
        ComputeOutput,
        SceneColor.ViewRect);

    if (Inputs.OverrideOutput.IsValid())
    {
        AddDrawTexturePass(
            GraphBuilder,
            View,
            Result,
            Inputs.OverrideOutput);

        return Inputs.OverrideOutput;
    }

    return Result;
}