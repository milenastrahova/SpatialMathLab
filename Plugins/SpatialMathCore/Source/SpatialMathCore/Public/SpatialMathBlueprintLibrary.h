#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpatialMathTypes.h"
#include "SpatialMathBlueprintLibrary.generated.h"

UCLASS()
class SPATIALMATHCORE_API USpatialMathBlueprintLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Vectors"
    )
    static FVector ProjectVectorOntoVector(
        const FVector& Vector,
        const FVector& OntoVector,
        bool& bSuccess
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Vectors"
    )
    static FVector RejectVectorFromVector(
        const FVector& Vector,
        const FVector& OntoVector,
        bool& bSuccess
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Vectors"
    )
    static FVector ReflectVectorAcrossNormal(
        const FVector& Vector,
        const FVector& Normal,
        bool& bSuccess
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Vectors"
    )
    static float SignedAngleDegrees(
        const FVector& FromDirection,
        const FVector& ToDirection,
        const FVector& ReferenceAxis,
        bool& bSuccess
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Geometry"
    )
    static FVector ClosestPointOnSegment(
        const FVector& Point,
        const FVector& SegmentStart,
        const FVector& SegmentEnd,
        float& OutSegmentAlpha
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Geometry"
    )
    static float DistancePointToSegment(
        const FVector& Point,
        const FVector& SegmentStart,
        const FVector& SegmentEnd
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Targeting"
    )
    static bool IsDirectionInsideCone(
        const FVector& ForwardDirection,
        const FVector& DirectionToTarget,
        float HalfAngleDegrees
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Intersections"
    )
    static FSpatialRayHit IntersectRayPlane(
        const FVector& RayOrigin,
        const FVector& RayDirection,
        const FVector& PlanePoint,
        const FVector& PlaneNormal,
        bool bRequireForwardHit = true
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Intersections"
    )
    static FSpatialRayHit IntersectRaySphere(
        const FVector& RayOrigin,
        const FVector& RayDirection,
        const FVector& SphereCenter,
        float SphereRadius
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Intersections"
    )
    static FSpatialRayHit IntersectRayAabb(
        const FVector& RayOrigin,
        const FVector& RayDirection,
        const FVector& BoxMinimum,
        const FVector& BoxMaximum
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Geometry"
    )
    static FSpatialBarycentricResult ComputeBarycentricCoordinates(
        const FVector& Point,
        const FVector& TriangleA,
        const FVector& TriangleB,
        const FVector& TriangleC,
        float Tolerance = 0.001f
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Transforms"
    )
    static FVector WorldPointToLocal(
        const FTransform& Transform,
        const FVector& WorldPoint
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Transforms"
    )
    static FVector LocalPointToWorld(
        const FTransform& Transform,
        const FVector& LocalPoint
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Rotations"
    )
    static FQuat MakeShortestArcRotation(
        const FVector& FromDirection,
        const FVector& ToDirection,
        bool& bSuccess
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Rotations"
    )
    static FQuat SlerpNormalized(
        const FQuat& FromRotation,
        const FQuat& ToRotation,
        float Alpha
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Ballistics"
    )
    static FSpatialBallisticSolution SolveBallisticArc(
        const FVector& Start,
        const FVector& Target,
        float LaunchSpeed,
        float GravityMagnitude,
        bool bHighArc
    );

    UFUNCTION(
        BlueprintPure,
        Category = "Spatial Math|Ballistics"
    )
    static FVector EvaluateBallisticPosition(
        const FVector& Start,
        const FVector& LaunchVelocity,
        float GravityMagnitude,
        float TimeSeconds
    );
};
