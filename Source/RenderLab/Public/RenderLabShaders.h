#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

class FRenderLabCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FRenderLabCS);
    SHADER_USE_PARAMETER_STRUCT(FRenderLabCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER(FIntPoint, OutputSize)

        SHADER_PARAMETER_RDG_TEXTURE_UAV(
            RWTexture2D<float4>,
            OutputTexture
        )
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(
        const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(
            Parameters.Platform,
            ERHIFeatureLevel::SM5);
    }
};

class FRenderLabPostProcessCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FRenderLabPostProcessCS);

    SHADER_USE_PARAMETER_STRUCT(
        FRenderLabPostProcessCS,
        FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_RDG_TEXTURE(
            Texture2D,
            InputTexture)
        SHADER_PARAMETER_SAMPLER(SamplerState,InputTextureSampler_clamp_linear)
        SHADER_PARAMETER_RDG_TEXTURE_UAV(
            RWTexture2D<float4>,
            OutputTexture)

        SHADER_PARAMETER(FIntPoint, ViewRectMin)
        SHADER_PARAMETER(FIntPoint, ViewSize)
        SHADER_PARAMETER(float, Strength)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(
        const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(
            Parameters.Platform,
            ERHIFeatureLevel::SM5);
    }
};


class FRenderLabDepthCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FRenderLabDepthCS);

    SHADER_USE_PARAMETER_STRUCT(
        FRenderLabDepthCS,
        FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_RDG_TEXTURE(
            Texture2D,
            InputTexture)

        SHADER_PARAMETER_RDG_TEXTURE_UAV(
            RWTexture2D<float>,
            OutputTexture)

        SHADER_PARAMETER(FIntPoint, ViewRectMin)
        SHADER_PARAMETER(FIntPoint, ViewSize)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(
        const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(
            Parameters.Platform,
            ERHIFeatureLevel::SM5);
    }
};