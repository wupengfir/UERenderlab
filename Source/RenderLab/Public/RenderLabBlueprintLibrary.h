#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RenderLabBlueprintLibrary.generated.h"

class UTextureRenderTarget2D;

UCLASS()
class RENDERLAB_API URenderLabBlueprintLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RenderLab")
    static void RunComputeShader(
        UTextureRenderTarget2D* OutputRenderTarget);
};