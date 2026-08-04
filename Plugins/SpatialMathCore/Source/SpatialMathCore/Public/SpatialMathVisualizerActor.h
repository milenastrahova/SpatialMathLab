#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpatialMathVisualizerActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SPATIALMATHCORE_API ASpatialMathVisualizerActor
    : public AActor
{
    GENERATED_BODY()

public:
    ASpatialMathVisualizerActor();

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math|Vectors"
    )
    FVector InputVector =
        FVector(300.0f, 220.0f, 120.0f);

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math|Vectors"
    )
    FVector BasisVector =
        FVector(320.0f, 0.0f, 0.0f);

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math|Display"
    )
    float ArrowSize = 22.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math|Display"
    )
    float LineThickness = 4.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math|Display"
    )
    bool bAnimateInput = true;

protected:
    virtual void BeginPlay() override;

private:
    float ElapsedSeconds = 0.0f;

    void DrawVector(
        const FVector& Origin,
        const FVector& Vector,
        const FColor& Color,
        const FString& Label
    ) const;
};
