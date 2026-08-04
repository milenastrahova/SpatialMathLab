#pragma once

#include "CoreMinimal.h"
#include "SpatialMathTypes.generated.h"

USTRUCT(BlueprintType)
struct SPATIALMATHCORE_API FSpatialRayHit
{
    GENERATED_BODY()

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    bool bHit = false;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    float Distance = 0.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    FVector Point = FVector::ZeroVector;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    FVector Normal = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct SPATIALMATHCORE_API FSpatialBarycentricResult
{
    GENERATED_BODY()

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    bool bValid = false;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    bool bInsideTriangle = false;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    FVector Coordinates = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct SPATIALMATHCORE_API FSpatialBallisticSolution
{
    GENERATED_BODY()

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    bool bValid = false;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    FVector LaunchVelocity = FVector::ZeroVector;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    float FlightTime = 0.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Spatial Math"
    )
    float LaunchAngleDegrees = 0.0f;
};
