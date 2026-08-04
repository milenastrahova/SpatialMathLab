#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpatialMathLabShowcaseActor.generated.h"

UCLASS()
class SPATIALMATHLAB_API ASpatialMathLabShowcaseActor
    : public AActor
{
    GENERATED_BODY()

public:
    ASpatialMathLabShowcaseActor();

    virtual void Tick(float DeltaSeconds) override;

private:
    float ElapsedSeconds = 0.0f;

    void DrawLaboratoryGrid() const;
    void DrawInstructions() const;

    void DrawProjectionStation(
        const FVector& Origin
    ) const;

    void DrawQuaternionStation(
        const FVector& Origin
    ) const;

    void DrawIntersectionStation(
        const FVector& Origin
    ) const;

    void DrawTargetingStation(
        const FVector& Origin
    ) const;

    void DrawBallisticsStation(
        const FVector& Origin
    ) const;

    void DrawStationPlatform(
        const FVector& Origin,
        const FString& Title,
        const FColor& Color
    ) const;

    void DrawArrow(
        const FVector& Start,
        const FVector& Vector,
        const FColor& Color,
        const FString& Label,
        float Thickness = 4.0f
    ) const;

    void DrawBallisticPath(
        const FVector& Start,
        const FVector& Velocity,
        float Gravity,
        float FlightTime,
        const FColor& Color
    ) const;
};