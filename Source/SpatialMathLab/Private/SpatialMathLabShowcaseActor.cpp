#include "SpatialMathLabShowcaseActor.h"

#include "DrawDebugHelpers.h"
#include "SpatialMathBlueprintLibrary.h"

namespace
{
constexpr float StationLifetime = 0.0f;
constexpr uint8 StationDepthPriority = 0;
constexpr float StationThickness = 3.0f;
}

ASpatialMathLabShowcaseActor::
    ASpatialMathLabShowcaseActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASpatialMathLabShowcaseActor::Tick(
    const float DeltaSeconds
)
{
    Super::Tick(DeltaSeconds);

    ElapsedSeconds += DeltaSeconds;


    DrawProjectionStation(
        FVector(-2100.0f, 0.0f, 90.0f)
    );

    DrawQuaternionStation(
        FVector(-1050.0f, 650.0f, 90.0f)
    );

    DrawIntersectionStation(
        FVector(0.0f, 0.0f, 90.0f)
    );

    DrawTargetingStation(
        FVector(1050.0f, 650.0f, 90.0f)
    );

    DrawBallisticsStation(
        FVector(2200.0f, 0.0f, 90.0f)
    );
}

void ASpatialMathLabShowcaseActor::
    DrawLaboratoryGrid() const
{
    constexpr int32 HalfGridLines = 12;
    constexpr float Step = 200.0f;
    constexpr float Extent = HalfGridLines * Step;

    for (int32 Index = -HalfGridLines;
        Index <= HalfGridLines;
        ++Index)
    {
        const float Offset = Index * Step;

        DrawDebugLine(
            GetWorld(),
            FVector(-400.0f, Offset, 2.0f),
            FVector(2200.0f, Offset, 2.0f),
            FColor(35, 55, 75),
            false,
            StationLifetime,
            StationDepthPriority,
            1.0f
        );

        DrawDebugLine(
            GetWorld(),
            FVector(Offset + 900.0f, -500.0f, 2.0f),
            FVector(Offset + 900.0f, 1350.0f, 2.0f),
            FColor(35, 55, 75),
            false,
            StationLifetime,
            StationDepthPriority,
            1.0f
        );
    }
}

void ASpatialMathLabShowcaseActor::
    DrawInstructions() const
{
    DrawDebugString(
        GetWorld(),
        FVector(-720.0f, -650.0f, 980.0f),
        TEXT(
            "SPATIAL MATH LAB\n"
            "WASD move | Q/E vertical | Hold RMB to look | Shift boost"
        ),
        nullptr,
        FColor::White,
        StationLifetime,
        false,
        1.25f
    );
}

void ASpatialMathLabShowcaseActor::
    DrawProjectionStation(
        const FVector& Origin
    ) const
{
    DrawStationPlatform(
        Origin,
        TEXT("01  VECTOR PROJECTION"),
        FColor::Green
    );

    const FVector Basis(360.0f, 0.0f, 0.0f);

    const FVector BaseInput(
        285.0f,
        240.0f,
        160.0f
    );

    const FVector Input =
        FQuat(
            FVector::UpVector,
            ElapsedSeconds * 0.45f
        ).RotateVector(BaseInput);

    bool bProjectionValid = false;

    const FVector Projection =
        USpatialMathBlueprintLibrary::
            ProjectVectorOntoVector(
                Input,
                Basis,
                bProjectionValid
            );

    bool bRejectionValid = false;

    const FVector Rejection =
        USpatialMathBlueprintLibrary::
            RejectVectorFromVector(
                Input,
                Basis,
                bRejectionValid
            );

    DrawArrow(
        Origin,
        Basis,
        FColor::Blue,
        TEXT("basis")
    );

    DrawArrow(
        Origin,
        Input,
        FColor::Yellow,
        TEXT("input")
    );

    if (bProjectionValid)
    {
        DrawArrow(
            Origin,
            Projection,
            FColor::Green,
            TEXT("projection")
        );
    }

    if (bRejectionValid)
    {
        DrawArrow(
            Origin + Projection,
            Rejection,
            FColor::Red,
            TEXT("rejection")
        );

        DrawDebugLine(
            GetWorld(),
            Origin + Input,
            Origin + Projection,
            FColor::Silver,
            false,
            StationLifetime,
            StationDepthPriority,
            1.5f
        );
    }
}

void ASpatialMathLabShowcaseActor::
    DrawQuaternionStation(
        const FVector& Origin
    ) const
{
    DrawStationPlatform(
        Origin,
        TEXT("02  QUATERNION SLERP"),
        FColor::Cyan
    );

    const FVector FromDirection =
        FVector::ForwardVector;

    const FVector ToDirection =
        FVector(0.15f, 0.95f, 0.28f).
            GetSafeNormal();

    bool bRotationValid = false;

    const FQuat TargetRotation =
        USpatialMathBlueprintLibrary::
            MakeShortestArcRotation(
                FromDirection,
                ToDirection,
                bRotationValid
            );

    const float Alpha =
        0.5f +
        0.5f *
        FMath::Sin(
            ElapsedSeconds * 0.9f
        );

    const FQuat CurrentRotation =
        USpatialMathBlueprintLibrary::
            SlerpNormalized(
                FQuat::Identity,
                TargetRotation,
                Alpha
            );

    const FVector CurrentDirection =
        CurrentRotation.RotateVector(
            FromDirection
        );

    DrawArrow(
        Origin,
        FromDirection * 330.0f,
        FColor::Blue,
        TEXT("from")
    );

    DrawArrow(
        Origin,
        ToDirection * 330.0f,
        FColor::Green,
        TEXT("target")
    );

    DrawArrow(
        Origin,
        CurrentDirection * 350.0f,
        FColor::Yellow,
        FString::Printf(
            TEXT("slerp alpha %.2f"),
            Alpha
        ),
        6.0f
    );

    if (bRotationValid)
    {
        FVector Previous =
            Origin + FromDirection * 260.0f;

        constexpr int32 ArcSegments = 24;

        for (int32 Index = 1;
            Index <= ArcSegments;
            ++Index)
        {
            const float ArcAlpha =
                static_cast<float>(Index) /
                static_cast<float>(ArcSegments);

            const FVector ArcDirection =
                USpatialMathBlueprintLibrary::
                    SlerpNormalized(
                        FQuat::Identity,
                        TargetRotation,
                        ArcAlpha
                    ).
                    RotateVector(
                        FromDirection
                    );

            const FVector Current =
                Origin + ArcDirection * 260.0f;

            DrawDebugLine(
                GetWorld(),
                Previous,
                Current,
                FColor::Cyan,
                false,
                StationLifetime,
                StationDepthPriority,
                2.5f
            );

            Previous = Current;
        }
    }
}

void ASpatialMathLabShowcaseActor::
    DrawIntersectionStation(
        const FVector& Origin
    ) const
{
    DrawStationPlatform(
        Origin,
        TEXT("03  RAY INTERSECTIONS"),
        FColor::Orange
    );

    const FVector RayOrigin =
        Origin + FVector(-300.0f, -120.0f, 100.0f);

    const FVector SphereCenter =
        Origin + FVector(180.0f, 40.0f, 150.0f);

    constexpr float SphereRadius = 135.0f;

    const FVector AnimatedAim =
        SphereCenter +
        FVector(
            0.0f,
            FMath::Sin(ElapsedSeconds) * 110.0f,
            FMath::Cos(ElapsedSeconds * 0.7f) * 70.0f
        );

    const FVector RayDirection =
        (AnimatedAim - RayOrigin).
            GetSafeNormal();

    const FSpatialRayHit SphereHit =
        USpatialMathBlueprintLibrary::
            IntersectRaySphere(
                RayOrigin,
                RayDirection,
                SphereCenter,
                SphereRadius
            );

    DrawDebugSphere(
        GetWorld(),
        SphereCenter,
        SphereRadius,
        24,
        FColor::Orange,
        false,
        StationLifetime,
        StationDepthPriority,
        2.5f
    );

    DrawDebugLine(
        GetWorld(),
        RayOrigin,
        RayOrigin + RayDirection * 850.0f,
        SphereHit.bHit
            ? FColor::Green
            : FColor::Red,
        false,
        StationLifetime,
        StationDepthPriority,
        4.0f
    );

    DrawDebugSphere(
        GetWorld(),
        RayOrigin,
        22.0f,
        12,
        FColor::Blue,
        false,
        StationLifetime,
        StationDepthPriority,
        2.0f
    );

    if (SphereHit.bHit)
    {
        DrawDebugSphere(
            GetWorld(),
            SphereHit.Point,
            28.0f,
            16,
            FColor::Green,
            false,
            StationLifetime,
            StationDepthPriority,
            4.0f
        );

        DrawArrow(
            SphereHit.Point,
            SphereHit.Normal * 150.0f,
            FColor::Cyan,
            TEXT("surface normal")
        );

        DrawDebugString(
            GetWorld(),
            SphereHit.Point +
            FVector(0.0f, 0.0f, 70.0f),
            FString::Printf(
                TEXT("hit distance %.1f"),
                SphereHit.Distance
            ),
            nullptr,
            FColor::Green,
            StationLifetime,
            false,
            1.0f
        );
    }

    const FVector BoxCenter =
        Origin + FVector(400.0f, -210.0f, 95.0f);

    const FVector BoxExtent(
        90.0f,
        120.0f,
        95.0f
    );

    DrawDebugBox(
        GetWorld(),
        BoxCenter,
        BoxExtent,
        FColor::Purple,
        false,
        StationLifetime,
        StationDepthPriority,
        2.0f
    );
}

void ASpatialMathLabShowcaseActor::
    DrawTargetingStation(
        const FVector& Origin
    ) const
{
    DrawStationPlatform(
        Origin,
        TEXT("04  TARGET SELECTION"),
        FColor::Yellow
    );

    const FVector SensorOrigin =
        Origin + FVector(-220.0f, 0.0f, 80.0f);

    const FVector Forward =
        FVector::ForwardVector;

    constexpr float HalfAngleDegrees = 35.0f;
    constexpr float Range = 650.0f;

    const FQuat LeftRotation(
        FVector::UpVector,
        FMath::DegreesToRadians(
            -HalfAngleDegrees
        )
    );

    const FQuat RightRotation(
        FVector::UpVector,
        FMath::DegreesToRadians(
            HalfAngleDegrees
        )
    );

    DrawArrow(
        SensorOrigin,
        Forward * 340.0f,
        FColor::Blue,
        TEXT("forward")
    );

    DrawDebugLine(
        GetWorld(),
        SensorOrigin,
        SensorOrigin +
        LeftRotation.RotateVector(Forward) * Range,
        FColor::Silver,
        false,
        StationLifetime,
        StationDepthPriority,
        2.0f
    );

    DrawDebugLine(
        GetWorld(),
        SensorOrigin,
        SensorOrigin +
        RightRotation.RotateVector(Forward) * Range,
        FColor::Silver,
        false,
        StationLifetime,
        StationDepthPriority,
        2.0f
    );

    TArray<FVector> Targets;

    Targets.Add(
        Origin + FVector(
            270.0f,
            -240.0f,
            80.0f
        )
    );

    Targets.Add(
        Origin + FVector(
            390.0f,
            -95.0f +
            FMath::Sin(
                ElapsedSeconds * 0.8f
            ) *
            45.0f,
            125.0f
        )
    );

    Targets.Add(
        Origin + FVector(
            520.0f,
            70.0f,
            60.0f
        )
    );

    Targets.Add(
        Origin + FVector(
            300.0f,
            280.0f,
            110.0f
        )
    );

    Targets.Add(
        Origin + FVector(
            620.0f,
            190.0f,
            155.0f
        )
    );

    int32 BestIndex = INDEX_NONE;
    float BestScore = -TNumericLimits<float>::Max();

    for (int32 Index = 0;
        Index < Targets.Num();
        ++Index)
    {
        const FVector ToTarget =
            Targets[Index] - SensorOrigin;

        const float Distance = ToTarget.Size();

        const bool bInside =
            Distance <= Range &&
            USpatialMathBlueprintLibrary::
                IsDirectionInsideCone(
                    Forward,
                    ToTarget,
                    HalfAngleDegrees
                );

        if (bInside)
        {
            const FVector Direction =
                ToTarget.GetSafeNormal();

            const float Alignment =
                FVector::DotProduct(
                    Forward,
                    Direction
                );

            const float DistanceScore =
                1.0f -
                FMath::Clamp(
                    Distance / Range,
                    0.0f,
                    1.0f
                );

            const float Score =
                Alignment * 0.75f +
                DistanceScore * 0.25f;

            if (Score > BestScore)
            {
                BestScore = Score;
                BestIndex = Index;
            }
        }
    }

    for (int32 Index = 0;
        Index < Targets.Num();
        ++Index)
    {
        const FVector ToTarget =
            Targets[Index] - SensorOrigin;

        const bool bInside =
            ToTarget.Size() <= Range &&
            USpatialMathBlueprintLibrary::
                IsDirectionInsideCone(
                    Forward,
                    ToTarget,
                    HalfAngleDegrees
                );

        const bool bSelected =
            Index == BestIndex;

        const FColor Color =
            bSelected
                ? FColor::Green
                : (
                    bInside
                        ? FColor::Yellow
                        : FColor::Red
                );

        DrawDebugSphere(
            GetWorld(),
            Targets[Index],
            bSelected ? 44.0f : 30.0f,
            16,
            Color,
            false,
            StationLifetime,
            StationDepthPriority,
            bSelected ? 5.0f : 2.0f
        );

        DrawDebugString(
            GetWorld(),
            Targets[Index] +
            FVector(0.0f, 0.0f, 55.0f),
            bSelected
                ? TEXT("SELECTED")
                : (
                    bInside
                        ? TEXT("candidate")
                        : TEXT("outside FOV")
                ),
            nullptr,
            Color,
            StationLifetime,
            false,
            0.9f
        );

        if (bSelected)
        {
            DrawDebugLine(
                GetWorld(),
                SensorOrigin,
                Targets[Index],
                FColor::Green,
                false,
                StationLifetime,
                StationDepthPriority,
                5.0f
            );
        }
    }
}

void ASpatialMathLabShowcaseActor::
    DrawBallisticsStation(
        const FVector& Origin
    ) const
{
    DrawStationPlatform(
        Origin,
        TEXT("05  BALLISTIC SOLUTIONS"),
        FColor::Magenta
    );

    const FVector Start =
        Origin + FVector(-320.0f, 0.0f, 55.0f);

    const FVector Target =
        Origin +
        FVector(
            610.0f,
            FMath::Sin(
                ElapsedSeconds * 0.45f
            ) *
            120.0f,
            210.0f
        );

    constexpr float LaunchSpeed = 1200.0f;
    constexpr float Gravity = 980.0f;

    const FSpatialBallisticSolution LowArc =
        USpatialMathBlueprintLibrary::
            SolveBallisticArc(
                Start,
                Target,
                LaunchSpeed,
                Gravity,
                false
            );

    const FSpatialBallisticSolution HighArc =
        USpatialMathBlueprintLibrary::
            SolveBallisticArc(
                Start,
                Target,
                LaunchSpeed,
                Gravity,
                true
            );

    DrawDebugSphere(
        GetWorld(),
        Start,
        28.0f,
        16,
        FColor::Blue,
        false,
        StationLifetime,
        StationDepthPriority,
        3.0f
    );

    DrawDebugSphere(
        GetWorld(),
        Target,
        48.0f,
        20,
        FColor::Red,
        false,
        StationLifetime,
        StationDepthPriority,
        4.0f
    );

    DrawDebugString(
        GetWorld(),
        Target + FVector(0.0f, 0.0f, 75.0f),
        TEXT("moving target"),
        nullptr,
        FColor::Red,
        StationLifetime,
        false,
        1.0f
    );

    if (LowArc.bValid)
    {
        DrawBallisticPath(
            Start,
            LowArc.LaunchVelocity,
            Gravity,
            LowArc.FlightTime,
            FColor::Cyan
        );

        DrawDebugString(
            GetWorld(),
            Start + FVector(0.0f, -90.0f, 100.0f),
            FString::Printf(
                TEXT("low arc %.1f deg"),
                LowArc.LaunchAngleDegrees
            ),
            nullptr,
            FColor::Cyan,
            StationLifetime,
            false,
            1.0f
        );
    }

    if (HighArc.bValid)
    {
        DrawBallisticPath(
            Start,
            HighArc.LaunchVelocity,
            Gravity,
            HighArc.FlightTime,
            FColor::Magenta
        );

        DrawDebugString(
            GetWorld(),
            Start + FVector(0.0f, 90.0f, 130.0f),
            FString::Printf(
                TEXT("high arc %.1f deg"),
                HighArc.LaunchAngleDegrees
            ),
            nullptr,
            FColor::Magenta,
            StationLifetime,
            false,
            1.0f
        );
    }
}

void ASpatialMathLabShowcaseActor::
    DrawStationPlatform(
        const FVector& Origin,
        const FString& Title,
        const FColor& Color
    ) const
{
    DrawDebugBox(
        GetWorld(),
        Origin - FVector(0.0f, 0.0f, 70.0f),
        FVector(390.0f, 340.0f, 15.0f),
        Color,
        false,
        StationLifetime,
        StationDepthPriority,
        2.0f
    );

    DrawDebugString(
        GetWorld(),
        Origin + FVector(-300.0f, -280.0f, 360.0f),
        Title,
        nullptr,
        Color,
        StationLifetime,
        false,
        1.15f
    );
}

void ASpatialMathLabShowcaseActor::DrawArrow(
    const FVector& Start,
    const FVector& Vector,
    const FColor& Color,
    const FString& Label,
    const float Thickness
) const
{
    const FVector End = Start + Vector;

    DrawDebugDirectionalArrow(
        GetWorld(),
        Start,
        End,
        24.0f,
        Color,
        false,
        StationLifetime,
        StationDepthPriority,
        Thickness
    );

    DrawDebugString(
        GetWorld(),
        End + FVector(0.0f, 0.0f, 28.0f),
        Label,
        nullptr,
        Color,
        StationLifetime,
        false,
        0.95f
    );
}

void ASpatialMathLabShowcaseActor::
    DrawBallisticPath(
        const FVector& Start,
        const FVector& Velocity,
        const float Gravity,
        const float FlightTime,
        const FColor& Color
    ) const
{
    constexpr int32 Segments = 40;

    FVector Previous = Start;

    for (int32 Index = 1;
        Index <= Segments;
        ++Index)
    {
        const float Alpha =
            static_cast<float>(Index) /
            static_cast<float>(Segments);

        const float Time = FlightTime * Alpha;

        const FVector Current =
            USpatialMathBlueprintLibrary::
                EvaluateBallisticPosition(
                    Start,
                    Velocity,
                    Gravity,
                    Time
                );

        DrawDebugLine(
            GetWorld(),
            Previous,
            Current,
            Color,
            false,
            StationLifetime,
            StationDepthPriority,
            4.0f
        );

        Previous = Current;
    }
}