#include "RenderLabBlueprintLibrary.h"

#include "RenderLabShaders.h"

#include "Engine/TextureRenderTarget2D.h"
#include "GlobalShader.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "RenderTargetPool.h"
#include "TextureResource.h"

void URenderLabBlueprintLibrary::RunComputeShader(
    UTextureRenderTarget2D* OutputRenderTarget)
{
    if (!IsValid(OutputRenderTarget))
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("RenderLab: OutputRenderTarget is invalid."));
        return;
    }

    FTextureRenderTargetResource* RenderTargetResource =
        OutputRenderTarget->GameThread_GetRenderTargetResource();

    const FIntPoint OutputSize(
        OutputRenderTarget->SizeX,
        OutputRenderTarget->SizeY);

    ENQUEUE_RENDER_COMMAND(RenderLab_RunComputeShader)(
        [RenderTargetResource, OutputSize]
        (FRHICommandListImmediate& RHICmdList)
        {
            FRDGBuilder GraphBuilder(RHICmdList);

            FRDGTextureRef OutputTexture =
                GraphBuilder.RegisterExternalTexture(
                    CreateRenderTarget(
                        RenderTargetResource->GetRenderTargetTexture(),
                        TEXT("RenderLab.OutputTexture")));

            FRenderLabCS::FParameters* PassParameters =
                GraphBuilder.AllocParameters<
                    FRenderLabCS::FParameters>();

            PassParameters->OutputSize = OutputSize;
            PassParameters->OutputTexture =
                GraphBuilder.CreateUAV(
                    FRDGTextureUAVDesc(OutputTexture));

            TShaderMapRef<FRenderLabCS> ComputeShader(
                GetGlobalShaderMap(
                    GMaxRHIFeatureLevel));

            const FIntVector GroupCount(
                FMath::DivideAndRoundUp(OutputSize.X, 8),
                FMath::DivideAndRoundUp(OutputSize.Y, 8),
                1);

            FComputeShaderUtils::AddPass(
                GraphBuilder,
                RDG_EVENT_NAME("RenderLab.FillTexture"),
                ComputeShader,
                PassParameters,
                GroupCount);

            GraphBuilder.Execute();
        });
}