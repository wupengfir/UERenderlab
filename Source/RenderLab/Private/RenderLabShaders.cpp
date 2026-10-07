#include "RenderLabShaders.h"

IMPLEMENT_GLOBAL_SHADER(
    FRenderLabCS,
    "/Plugin/RenderLab/Private/RenderLab.usf",
    "MainCS",
    SF_Compute
);

IMPLEMENT_GLOBAL_SHADER(
    FRenderLabPostProcessCS,
    "/Plugin/RenderLab/Private/RenderLabPostProcess.usf",
    "MainCS",
    SF_Compute
);