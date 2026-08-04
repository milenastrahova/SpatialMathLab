#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "SpatialMathBlueprintLibrary.h"
#include "SpatialMathVisualizerActor.h"

namespace
{
constexpr EAutomationTestFlags SpatialTestFlags =
    EAutomationTestFlags::EditorContext |
    EAutomationTestFlags::EngineFilter;

bool NearlyEqualVector(
    const FVector& A,
    const FVector& B,
    const float Tolerance = 0.001f
)
{
    return A.Equals(B, Tolerance);
}

bool NearlyEqualFloat(
    const float A,
    const float B,
    const float Tolerance = 0.001f
)
{
    return FMath::IsNearlyEqual(
        A,
        B,
        Tolerance
    );
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialProjectionTest,
    "SpatialMathCore.Vector.Projection",
    SpatialTestFlags
)

bool FSpatialProjectionTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = false;

    const FVector Result =
        USpatialMathBlueprintLibrary::
            ProjectVectorOntoVector(
                FVector(3.0f, 4.0f, 0.0f),
                FVector::ForwardVector,
                bSuccess
            );

    TestTrue(TEXT("Projection succeeds"), bSuccess);
    TestTrue(
        TEXT("Projection result"),
        NearlyEqualVector(
            Result,
            FVector(3.0f, 0.0f, 0.0f)
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialProjectionDegenerateTest,
    "SpatialMathCore.Vector.ProjectionDegenerate",
    SpatialTestFlags
)

bool FSpatialProjectionDegenerateTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = true;

    const FVector Result =
        USpatialMathBlueprintLibrary::
            ProjectVectorOntoVector(
                FVector::OneVector,
                FVector::ZeroVector,
                bSuccess
            );

    TestFalse(TEXT("Zero basis is rejected"), bSuccess);
    TestTrue(
        TEXT("Degenerate result is zero"),
        Result.IsNearlyZero()
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRejectionTest,
    "SpatialMathCore.Vector.Rejection",
    SpatialTestFlags
)

bool FSpatialRejectionTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = false;

    const FVector Result =
        USpatialMathBlueprintLibrary::
            RejectVectorFromVector(
                FVector(3.0f, 4.0f, 0.0f),
                FVector::ForwardVector,
                bSuccess
            );

    TestTrue(TEXT("Rejection succeeds"), bSuccess);
    TestTrue(
        TEXT("Rejection result"),
        NearlyEqualVector(
            Result,
            FVector(0.0f, 4.0f, 0.0f)
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialReflectionTest,
    "SpatialMathCore.Vector.Reflection",
    SpatialTestFlags
)

bool FSpatialReflectionTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = false;

    const FVector Result =
        USpatialMathBlueprintLibrary::
            ReflectVectorAcrossNormal(
                FVector(1.0f, -1.0f, 0.0f),
                FVector::UpVector,
                bSuccess
            );

    TestTrue(TEXT("Reflection succeeds"), bSuccess);
    TestTrue(
        TEXT("Reflection result"),
        NearlyEqualVector(
            Result,
            FVector(1.0f, -1.0f, 0.0f)
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialSignedAnglePositiveTest,
    "SpatialMathCore.Vector.SignedAnglePositive",
    SpatialTestFlags
)

bool FSpatialSignedAnglePositiveTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = false;

    const float Angle =
        USpatialMathBlueprintLibrary::
            SignedAngleDegrees(
                FVector::ForwardVector,
                FVector::RightVector,
                FVector::UpVector,
                bSuccess
            );

    TestTrue(TEXT("Angle succeeds"), bSuccess);
    TestTrue(
        TEXT("Positive 90 degrees"),
        NearlyEqualFloat(Angle, 90.0f)
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialSignedAngleNegativeTest,
    "SpatialMathCore.Vector.SignedAngleNegative",
    SpatialTestFlags
)

bool FSpatialSignedAngleNegativeTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = false;

    const float Angle =
        USpatialMathBlueprintLibrary::
            SignedAngleDegrees(
                FVector::ForwardVector,
                -FVector::RightVector,
                FVector::UpVector,
                bSuccess
            );

    TestTrue(TEXT("Angle succeeds"), bSuccess);
    TestTrue(
        TEXT("Negative 90 degrees"),
        NearlyEqualFloat(Angle, -90.0f)
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialClosestPointInteriorTest,
    "SpatialMathCore.Geometry.ClosestPointInterior",
    SpatialTestFlags
)

bool FSpatialClosestPointInteriorTest::RunTest(
    const FString& Parameters
)
{
    float Alpha = 0.0f;

    const FVector Result =
        USpatialMathBlueprintLibrary::
            ClosestPointOnSegment(
                FVector(5.0f, 3.0f, 0.0f),
                FVector::ZeroVector,
                FVector(10.0f, 0.0f, 0.0f),
                Alpha
            );

    TestTrue(
        TEXT("Interior point"),
        NearlyEqualVector(
            Result,
            FVector(5.0f, 0.0f, 0.0f)
        )
    );

    TestTrue(
        TEXT("Interior alpha"),
        NearlyEqualFloat(Alpha, 0.5f)
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialClosestPointClampTest,
    "SpatialMathCore.Geometry.ClosestPointClamp",
    SpatialTestFlags
)

bool FSpatialClosestPointClampTest::RunTest(
    const FString& Parameters
)
{
    float Alpha = 1.0f;

    const FVector Result =
        USpatialMathBlueprintLibrary::
            ClosestPointOnSegment(
                FVector(-5.0f, 2.0f, 0.0f),
                FVector::ZeroVector,
                FVector(10.0f, 0.0f, 0.0f),
                Alpha
            );

    TestTrue(
        TEXT("Clamped to start"),
        Result.IsNearlyZero()
    );

    TestTrue(
        TEXT("Alpha is zero"),
        NearlyEqualFloat(Alpha, 0.0f)
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialDistanceToSegmentTest,
    "SpatialMathCore.Geometry.DistanceToSegment",
    SpatialTestFlags
)

bool FSpatialDistanceToSegmentTest::RunTest(
    const FString& Parameters
)
{
    const float Distance =
        USpatialMathBlueprintLibrary::
            DistancePointToSegment(
                FVector(5.0f, 3.0f, 0.0f),
                FVector::ZeroVector,
                FVector(10.0f, 0.0f, 0.0f)
            );

    TestTrue(
        TEXT("Distance is three"),
        NearlyEqualFloat(Distance, 3.0f)
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialConeInsideTest,
    "SpatialMathCore.Targeting.ConeInside",
    SpatialTestFlags
)

bool FSpatialConeInsideTest::RunTest(
    const FString& Parameters
)
{
    const FVector Direction =
        FVector(1.0f, 1.0f, 0.0f).
            GetSafeNormal();

    TestTrue(
        TEXT("45-degree target inside 50-degree cone"),
        USpatialMathBlueprintLibrary::
            IsDirectionInsideCone(
                FVector::ForwardVector,
                Direction,
                50.0f
            )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialConeOutsideTest,
    "SpatialMathCore.Targeting.ConeOutside",
    SpatialTestFlags
)

bool FSpatialConeOutsideTest::RunTest(
    const FString& Parameters
)
{
    TestFalse(
        TEXT("Side target outside 30-degree cone"),
        USpatialMathBlueprintLibrary::
            IsDirectionInsideCone(
                FVector::ForwardVector,
                FVector::RightVector,
                30.0f
            )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRayPlaneHitTest,
    "SpatialMathCore.Intersection.RayPlaneHit",
    SpatialTestFlags
)

bool FSpatialRayPlaneHitTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialRayHit Hit =
        USpatialMathBlueprintLibrary::
            IntersectRayPlane(
                FVector::ZeroVector,
                FVector::UpVector,
                FVector(0.0f, 0.0f, 10.0f),
                FVector::UpVector
            );

    TestTrue(TEXT("Ray hits plane"), Hit.bHit);
    TestTrue(
        TEXT("Plane hit point"),
        NearlyEqualVector(
            Hit.Point,
            FVector(0.0f, 0.0f, 10.0f)
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRayPlaneParallelTest,
    "SpatialMathCore.Intersection.RayPlaneParallel",
    SpatialTestFlags
)

bool FSpatialRayPlaneParallelTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialRayHit Hit =
        USpatialMathBlueprintLibrary::
            IntersectRayPlane(
                FVector::ZeroVector,
                FVector::ForwardVector,
                FVector(0.0f, 0.0f, 10.0f),
                FVector::UpVector
            );

    TestFalse(TEXT("Parallel ray misses"), Hit.bHit);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRaySphereHitTest,
    "SpatialMathCore.Intersection.RaySphereHit",
    SpatialTestFlags
)

bool FSpatialRaySphereHitTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialRayHit Hit =
        USpatialMathBlueprintLibrary::
            IntersectRaySphere(
                FVector::ZeroVector,
                FVector::ForwardVector,
                FVector(10.0f, 0.0f, 0.0f),
                2.0f
            );

    TestTrue(TEXT("Ray hits sphere"), Hit.bHit);
    TestTrue(
        TEXT("Sphere distance"),
        NearlyEqualFloat(
            Hit.Distance,
            8.0f
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRaySphereMissTest,
    "SpatialMathCore.Intersection.RaySphereMiss",
    SpatialTestFlags
)

bool FSpatialRaySphereMissTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialRayHit Hit =
        USpatialMathBlueprintLibrary::
            IntersectRaySphere(
                FVector::ZeroVector,
                FVector::ForwardVector,
                FVector(10.0f, 10.0f, 0.0f),
                2.0f
            );

    TestFalse(TEXT("Ray misses sphere"), Hit.bHit);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRayAabbHitTest,
    "SpatialMathCore.Intersection.RayAabbHit",
    SpatialTestFlags
)

bool FSpatialRayAabbHitTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialRayHit Hit =
        USpatialMathBlueprintLibrary::
            IntersectRayAabb(
                FVector(-10.0f, 0.0f, 0.0f),
                FVector::ForwardVector,
                FVector(-1.0f, -1.0f, -1.0f),
                FVector(1.0f, 1.0f, 1.0f)
            );

    TestTrue(TEXT("Ray hits AABB"), Hit.bHit);
    TestTrue(
        TEXT("AABB hit point"),
        NearlyEqualVector(
            Hit.Point,
            FVector(-1.0f, 0.0f, 0.0f)
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialRayAabbMissTest,
    "SpatialMathCore.Intersection.RayAabbMiss",
    SpatialTestFlags
)

bool FSpatialRayAabbMissTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialRayHit Hit =
        USpatialMathBlueprintLibrary::
            IntersectRayAabb(
                FVector(-10.0f, 5.0f, 0.0f),
                FVector::ForwardVector,
                FVector(-1.0f, -1.0f, -1.0f),
                FVector(1.0f, 1.0f, 1.0f)
            );

    TestFalse(TEXT("Ray misses AABB"), Hit.bHit);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialBarycentricInsideTest,
    "SpatialMathCore.Geometry.BarycentricInside",
    SpatialTestFlags
)

bool FSpatialBarycentricInsideTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialBarycentricResult Result =
        USpatialMathBlueprintLibrary::
            ComputeBarycentricCoordinates(
                FVector(0.25f, 0.25f, 0.0f),
                FVector::ZeroVector,
                FVector(1.0f, 0.0f, 0.0f),
                FVector(0.0f, 1.0f, 0.0f)
            );

    TestTrue(TEXT("Barycentric valid"), Result.bValid);
    TestTrue(TEXT("Point inside"), Result.bInsideTriangle);
    TestTrue(
        TEXT("Coordinates sum to one"),
        NearlyEqualFloat(
            Result.Coordinates.X +
            Result.Coordinates.Y +
            Result.Coordinates.Z,
            1.0f
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialBarycentricOutsideTest,
    "SpatialMathCore.Geometry.BarycentricOutside",
    SpatialTestFlags
)

bool FSpatialBarycentricOutsideTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialBarycentricResult Result =
        USpatialMathBlueprintLibrary::
            ComputeBarycentricCoordinates(
                FVector(2.0f, 2.0f, 0.0f),
                FVector::ZeroVector,
                FVector(1.0f, 0.0f, 0.0f),
                FVector(0.0f, 1.0f, 0.0f)
            );

    TestTrue(TEXT("Barycentric valid"), Result.bValid);
    TestFalse(TEXT("Point outside"), Result.bInsideTriangle);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialTransformRoundTripTest,
    "SpatialMathCore.Transform.RoundTrip",
    SpatialTestFlags
)

bool FSpatialTransformRoundTripTest::RunTest(
    const FString& Parameters
)
{
    const FTransform Transform(
        FRotator(10.0f, 35.0f, 5.0f),
        FVector(100.0f, -50.0f, 25.0f),
        FVector(1.5f, 0.75f, 2.0f)
    );

    const FVector LocalPoint(
        12.0f,
        -7.0f,
        3.0f
    );

    const FVector World =
        USpatialMathBlueprintLibrary::
            LocalPointToWorld(
                Transform,
                LocalPoint
            );

    const FVector Restored =
        USpatialMathBlueprintLibrary::
            WorldPointToLocal(
                Transform,
                World
            );

    TestTrue(
        TEXT("Transform round trip"),
        NearlyEqualVector(
            Restored,
            LocalPoint,
            0.01f
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialShortestArcTest,
    "SpatialMathCore.Rotation.ShortestArc",
    SpatialTestFlags
)

bool FSpatialShortestArcTest::RunTest(
    const FString& Parameters
)
{
    bool bSuccess = false;

    const FQuat Rotation =
        USpatialMathBlueprintLibrary::
            MakeShortestArcRotation(
                FVector::ForwardVector,
                FVector::RightVector,
                bSuccess
            );

    const FVector Rotated =
        Rotation.RotateVector(
            FVector::ForwardVector
        );

    TestTrue(TEXT("Shortest arc succeeds"), bSuccess);
    TestTrue(
        TEXT("Forward rotates to right"),
        NearlyEqualVector(
            Rotated,
            FVector::RightVector,
            0.01f
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialSlerpEndpointsTest,
    "SpatialMathCore.Rotation.SlerpEndpoints",
    SpatialTestFlags
)

bool FSpatialSlerpEndpointsTest::RunTest(
    const FString& Parameters
)
{
    const FQuat Start = FQuat::Identity;

    const FQuat End(
        FVector::UpVector,
        FMath::DegreesToRadians(90.0f)
    );

    const FQuat AtStart =
        USpatialMathBlueprintLibrary::
            SlerpNormalized(
                Start,
                End,
                0.0f
            );

    const FQuat AtEnd =
        USpatialMathBlueprintLibrary::
            SlerpNormalized(
                Start,
                End,
                1.0f
            );

    TestTrue(
        TEXT("Slerp start"),
        AtStart.Equals(Start, 0.001f)
    );

    TestTrue(
        TEXT("Slerp end"),
        AtEnd.Equals(End, 0.001f)
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialBallisticLowArcTest,
    "SpatialMathCore.Ballistics.LowArc",
    SpatialTestFlags
)

bool FSpatialBallisticLowArcTest::RunTest(
    const FString& Parameters
)
{
    const FVector Start = FVector::ZeroVector;
    const FVector Target(1000.0f, 0.0f, 0.0f);

    const FSpatialBallisticSolution Solution =
        USpatialMathBlueprintLibrary::
            SolveBallisticArc(
                Start,
                Target,
                1200.0f,
                980.0f,
                false
            );

    TestTrue(TEXT("Ballistic solution valid"), Solution.bValid);

    if (!Solution.bValid)
    {
        return false;
    }

    const FVector Evaluated =
        USpatialMathBlueprintLibrary::
            EvaluateBallisticPosition(
                Start,
                Solution.LaunchVelocity,
                980.0f,
                Solution.FlightTime
            );

    TestTrue(
        TEXT("Trajectory reaches target"),
        NearlyEqualVector(
            Evaluated,
            Target,
            1.0f
        )
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialBallisticHighArcTest,
    "SpatialMathCore.Ballistics.HighArc",
    SpatialTestFlags
)

bool FSpatialBallisticHighArcTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialBallisticSolution Low =
        USpatialMathBlueprintLibrary::
            SolveBallisticArc(
                FVector::ZeroVector,
                FVector(1000.0f, 0.0f, 0.0f),
                1200.0f,
                980.0f,
                false
            );

    const FSpatialBallisticSolution High =
        USpatialMathBlueprintLibrary::
            SolveBallisticArc(
                FVector::ZeroVector,
                FVector(1000.0f, 0.0f, 0.0f),
                1200.0f,
                980.0f,
                true
            );

    TestTrue(TEXT("Low valid"), Low.bValid);
    TestTrue(TEXT("High valid"), High.bValid);

    TestTrue(
        TEXT("High arc angle is larger"),
        High.LaunchAngleDegrees >
        Low.LaunchAngleDegrees
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialBallisticUnreachableTest,
    "SpatialMathCore.Ballistics.Unreachable",
    SpatialTestFlags
)

bool FSpatialBallisticUnreachableTest::RunTest(
    const FString& Parameters
)
{
    const FSpatialBallisticSolution Solution =
        USpatialMathBlueprintLibrary::
            SolveBallisticArc(
                FVector::ZeroVector,
                FVector(100000.0f, 0.0f, 0.0f),
                100.0f,
                980.0f,
                false
            );

    TestFalse(
        TEXT("Unreachable target rejected"),
        Solution.bValid
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpatialVisualizerNoDefaultTickCostTest,
    "SpatialMathCore.Visualization.ActorClassAvailable",
    SpatialTestFlags
)

bool FSpatialVisualizerNoDefaultTickCostTest::RunTest(
    const FString& Parameters
)
{
    TestNotNull(
        TEXT("Visualizer class exists"),
        ASpatialMathVisualizerActor::StaticClass()
    );

    return true;
}

#endif
