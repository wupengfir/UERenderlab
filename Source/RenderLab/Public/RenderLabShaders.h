#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

class FRenderLabCS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FRenderLabCS);
    SHADER_USE_PARAMETER_STRUCT(FRenderLabCS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
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