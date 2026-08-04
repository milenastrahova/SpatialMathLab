#include "SpatialMathVisualizerActor.h"

#include "DrawDebugHelpers.h"
#include "SpatialMathBlueprintLibrary.h"

ASpatialMathVisualizerActor::
    ASpatialMathVisualizerActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASpatialMathVisualizerActor::BeginPlay()
{
    Super::BeginPlay();
}

void ASpatialMathVisualizerActor::Tick(
    const float DeltaSeconds
)
{
    Super::Tick(DeltaSeconds);

    ElapsedSeconds += DeltaSeconds;

    FVector AnimatedInput = InputVector;

    if (bAnimateInput)
    {
        const float Angle =
            ElapsedSeconds * 0.65f;

        AnimatedInput =
            FQuat(
                FVector::UpVector,
                Angle
            ).
            RotateVector(InputVector);
    }

    bool bProjectionValid = false;

    const FVector Projection =
        USpatialMathBlueprintLibrary::
            ProjectVectorOntoVector(
                AnimatedInput,
                BasisVector,
                bProjectionValid
            );

    bool bRejectionValid = false;

    const FVector Rejection =
        USpatialMathBlueprintLibrary::
            RejectVectorFromVector(
                AnimatedInput,
                BasisVector,
                bRejectionValid
            );

    const FVector Origin = GetActorLocation();

    DrawVector(
        Origin,
        BasisVector,
        FColor::Blue,
        TEXT("Basis vector")
    );

    DrawVector(
        Origin,
        AnimatedInput,
        FColor::Yellow,
        TEXT("Input vector")
    );

    if (bProjectionValid)
    {
        DrawVector(
            Origin,
            Projection,
            FColor::Green,
            TEXT("Projection")
        );
    }

    if (bRejectionValid)
    {
        DrawVector(
            Origin + Projection,
            Rejection,
            FColor::Red,
            TEXT("Rejection")
        );
    }

    DrawDebugLine(
        GetWorld(),
        Origin + AnimatedInput,
        Origin + Projection,
        FColor::Silver,
        false,
        0.0f,
        0,
        1.5f
    );

    DrawDebugSphere(
        GetWorld(),
        Origin,
        24.0f,
        16,
        FColor::White,
        false,
        0.0f,
        0,
        2.0f
    );
}

void ASpatialMathVisualizerActor::DrawVector(
    const FVector& Origin,
    const FVector& Vector,
    const FColor& Color,
    const FString& Label
) const
{
    const FVector End = Origin + Vector;

    DrawDebugDirectionalArrow(
        GetWorld(),
        Origin,
        End,
        ArrowSize,
        Color,
        false,
        0.0f,
        0,
        LineThickness
    );

    DrawDebugString(
        GetWorld(),
        End + FVector(0.0f, 0.0f, 24.0f),
        Label,
        nullptr,
        Color,
        0.0f,
        false,
        1.0f
    );
}
