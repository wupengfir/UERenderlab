#include "RenderLabShaders.h"

IMPLEMENT_GLOBAL_SHADER(
    FRenderLabCS,
    "/Plugin/RenderLab/Private/RenderLab.usf",
    "MainCS",
    SF_Compute
);