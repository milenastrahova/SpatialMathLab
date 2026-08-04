#include "SpatialMathBlueprintLibrary.h"

namespace
{
constexpr float SpatialEpsilon = 1.0e-6f;

bool IsUsableVector(const FVector& Vector)
{
    return Vector.SizeSquared() > SpatialEpsilon;
}

float SafeSign(const float Value)
{
    return Value < 0.0f ? -1.0f : 1.0f;
}
}

FVector USpatialMathBlueprintLibrary::ProjectVectorOntoVector(
    const FVector& Vector,
    const FVector& OntoVector,
    bool& bSuccess
)
{
    const float Denominator = OntoVector.SizeSquared();

    bSuccess = Denominator > SpatialEpsilon;

    if (!bSuccess)
    {
        return FVector::ZeroVector;
    }

    return OntoVector *
        (
            FVector::DotProduct(Vector, OntoVector) /
            Denominator
        );
}

FVector USpatialMathBlueprintLibrary::RejectVectorFromVector(
    const FVector& Vector,
    const FVector& OntoVector,
    bool& bSuccess
)
{
    const FVector Projection = ProjectVectorOntoVector(
        Vector,
        OntoVector,
        bSuccess
    );

    return bSuccess
        ? Vector - Projection
        : FVector::ZeroVector;
}

FVector USpatialMathBlueprintLibrary::ReflectVectorAcrossNormal(
    const FVector& Vector,
    const FVector& Normal,
    bool& bSuccess
)
{
    const FVector NormalizedNormal =
        Normal.GetSafeNormal();

    bSuccess = !NormalizedNormal.IsNearlyZero();

    if (!bSuccess)
    {
        return FVector::ZeroVector;
    }

    return Vector -
        2.0f *
        FVector::DotProduct(
            Vector,
            NormalizedNormal
        ) *
        NormalizedNormal;
}

float USpatialMathBlueprintLibrary::SignedAngleDegrees(
    const FVector& FromDirection,
    const FVector& ToDirection,
    const FVector& ReferenceAxis,
    bool& bSuccess
)
{
    const FVector From = FromDirection.GetSafeNormal();
    const FVector To = ToDirection.GetSafeNormal();
    const FVector Axis = ReferenceAxis.GetSafeNormal();

    bSuccess =
        !From.IsNearlyZero() &&
        !To.IsNearlyZero() &&
        !Axis.IsNearlyZero();

    if (!bSuccess)
    {
        return 0.0f;
    }

    const float Dot = FMath::Clamp(
        FVector::DotProduct(From, To),
        -1.0f,
        1.0f
    );

    const float UnsignedRadians = FMath::Acos(Dot);

    const float Orientation =
        FVector::DotProduct(
            Axis,
            FVector::CrossProduct(From, To)
        );

    return FMath::RadiansToDegrees(
        UnsignedRadians
    ) * SafeSign(Orientation);
}

FVector USpatialMathBlueprintLibrary::ClosestPointOnSegment(
    const FVector& Point,
    const FVector& SegmentStart,
    const FVector& SegmentEnd,
    float& OutSegmentAlpha
)
{
    const FVector Segment = SegmentEnd - SegmentStart;
    const float SegmentLengthSquared =
        Segment.SizeSquared();

    if (SegmentLengthSquared <= SpatialEpsilon)
    {
        OutSegmentAlpha = 0.0f;
        return SegmentStart;
    }

    OutSegmentAlpha = FMath::Clamp(
        FVector::DotProduct(
            Point - SegmentStart,
            Segment
        ) / SegmentLengthSquared,
        0.0f,
        1.0f
    );

    return SegmentStart + Segment * OutSegmentAlpha;
}

float USpatialMathBlueprintLibrary::DistancePointToSegment(
    const FVector& Point,
    const FVector& SegmentStart,
    const FVector& SegmentEnd
)
{
    float Alpha = 0.0f;

    const FVector Closest =
        ClosestPointOnSegment(
            Point,
            SegmentStart,
            SegmentEnd,
            Alpha
        );

    return FVector::Distance(Point, Closest);
}

bool USpatialMathBlueprintLibrary::IsDirectionInsideCone(
    const FVector& ForwardDirection,
    const FVector& DirectionToTarget,
    const float HalfAngleDegrees
)
{
    const FVector Forward =
        ForwardDirection.GetSafeNormal();

    const FVector ToTarget =
        DirectionToTarget.GetSafeNormal();

    if (Forward.IsNearlyZero() ||
        ToTarget.IsNearlyZero())
    {
        return false;
    }

    const float ClampedAngle =
        FMath::Clamp(
            HalfAngleDegrees,
            0.0f,
            180.0f
        );

    const float MinimumDot =
        FMath::Cos(
            FMath::DegreesToRadians(
                ClampedAngle
            )
        );

    return FVector::DotProduct(
        Forward,
        ToTarget
    ) >= MinimumDot;
}

FSpatialRayHit USpatialMathBlueprintLibrary::IntersectRayPlane(
    const FVector& RayOrigin,
    const FVector& RayDirection,
    const FVector& PlanePoint,
    const FVector& PlaneNormal,
    const bool bRequireForwardHit
)
{
    FSpatialRayHit Result;

    const FVector Normal =
        PlaneNormal.GetSafeNormal();

    if (Normal.IsNearlyZero() ||
        !IsUsableVector(RayDirection))
    {
        return Result;
    }

    const float Denominator =
        FVector::DotProduct(
            RayDirection,
            Normal
        );

    if (FMath::Abs(Denominator) <= SpatialEpsilon)
    {
        return Result;
    }

    const float Distance =
        FVector::DotProduct(
            PlanePoint - RayOrigin,
            Normal
        ) / Denominator;

    if (bRequireForwardHit && Distance < 0.0f)
    {
        return Result;
    }

    Result.bHit = true;
    Result.Distance = Distance;
    Result.Point =
        RayOrigin + RayDirection * Distance;

    Result.Normal = Normal;

    return Result;
}

FSpatialRayHit USpatialMathBlueprintLibrary::IntersectRaySphere(
    const FVector& RayOrigin,
    const FVector& RayDirection,
    const FVector& SphereCenter,
    const float SphereRadius
)
{
    FSpatialRayHit Result;

    if (!IsUsableVector(RayDirection) ||
        SphereRadius <= 0.0f)
    {
        return Result;
    }

    const FVector Offset =
        RayOrigin - SphereCenter;

    const float A =
        FVector::DotProduct(
            RayDirection,
            RayDirection
        );

    const float B =
        2.0f *
        FVector::DotProduct(
            Offset,
            RayDirection
        );

    const float C =
        FVector::DotProduct(
            Offset,
            Offset
        ) -
        FMath::Square(SphereRadius);

    const float Discriminant =
        B * B - 4.0f * A * C;

    if (Discriminant < 0.0f)
    {
        return Result;
    }

    const float Root =
        FMath::Sqrt(
            FMath::Max(0.0f, Discriminant)
        );

    const float First =
        (-B - Root) / (2.0f * A);

    const float Second =
        (-B + Root) / (2.0f * A);

    float Distance = TNumericLimits<float>::Max();

    if (First >= 0.0f)
    {
        Distance = First;
    }

    if (Second >= 0.0f)
    {
        Distance = FMath::Min(
            Distance,
            Second
        );
    }

    if (Distance == TNumericLimits<float>::Max())
    {
        return Result;
    }

    Result.bHit = true;
    Result.Distance = Distance;
    Result.Point =
        RayOrigin + RayDirection * Distance;

    Result.Normal =
        (Result.Point - SphereCenter).
            GetSafeNormal();

    return Result;
}

FSpatialRayHit USpatialMathBlueprintLibrary::IntersectRayAabb(
    const FVector& RayOrigin,
    const FVector& RayDirection,
    const FVector& BoxMinimum,
    const FVector& BoxMaximum
)
{
    FSpatialRayHit Result;

    if (!IsUsableVector(RayDirection))
    {
        return Result;
    }

    const FVector Minimum(
        FMath::Min(BoxMinimum.X, BoxMaximum.X),
        FMath::Min(BoxMinimum.Y, BoxMaximum.Y),
        FMath::Min(BoxMinimum.Z, BoxMaximum.Z)
    );

    const FVector Maximum(
        FMath::Max(BoxMinimum.X, BoxMaximum.X),
        FMath::Max(BoxMinimum.Y, BoxMaximum.Y),
        FMath::Max(BoxMinimum.Z, BoxMaximum.Z)
    );

    float NearDistance =
        -TNumericLimits<float>::Max();

    float FarDistance =
        TNumericLimits<float>::Max();

    int32 NearAxis = INDEX_NONE;
    float NearAxisSign = 0.0f;

    const float Origins[3] = {
        RayOrigin.X,
        RayOrigin.Y,
        RayOrigin.Z
    };

    const float Directions[3] = {
        RayDirection.X,
        RayDirection.Y,
        RayDirection.Z
    };

    const float Minima[3] = {
        Minimum.X,
        Minimum.Y,
        Minimum.Z
    };

    const float Maxima[3] = {
        Maximum.X,
        Maximum.Y,
        Maximum.Z
    };

    for (int32 Axis = 0; Axis < 3; ++Axis)
    {
        if (FMath::Abs(Directions[Axis]) <= SpatialEpsilon)
        {
            if (Origins[Axis] < Minima[Axis] ||
                Origins[Axis] > Maxima[Axis])
            {
                return Result;
            }

            continue;
        }

        float First =
            (Minima[Axis] - Origins[Axis]) /
            Directions[Axis];

        float Second =
            (Maxima[Axis] - Origins[Axis]) /
            Directions[Axis];

        float AxisSign =
            Directions[Axis] > 0.0f
                ? -1.0f
                : 1.0f;

        if (First > Second)
        {
            Swap(First, Second);
            AxisSign *= -1.0f;
        }

        if (First > NearDistance)
        {
            NearDistance = First;
            NearAxis = Axis;
            NearAxisSign = AxisSign;
        }

        FarDistance =
            FMath::Min(
                FarDistance,
                Second
            );

        if (NearDistance > FarDistance)
        {
            return Result;
        }
    }

    if (FarDistance < 0.0f)
    {
        return Result;
    }

    const float Distance =
        NearDistance >= 0.0f
            ? NearDistance
            : FarDistance;

    Result.bHit = true;
    Result.Distance = Distance;
    Result.Point =
        RayOrigin + RayDirection * Distance;

    Result.Normal = FVector::ZeroVector;

    if (NearDistance >= 0.0f &&
        NearAxis != INDEX_NONE)
    {
        if (NearAxis == 0)
        {
            Result.Normal.X = NearAxisSign;
        }
        else if (NearAxis == 1)
        {
            Result.Normal.Y = NearAxisSign;
        }
        else
        {
            Result.Normal.Z = NearAxisSign;
        }
    }

    return Result;
}

FSpatialBarycentricResult
USpatialMathBlueprintLibrary::ComputeBarycentricCoordinates(
    const FVector& Point,
    const FVector& TriangleA,
    const FVector& TriangleB,
    const FVector& TriangleC,
    const float Tolerance
)
{
    FSpatialBarycentricResult Result;

    const FVector Edge0 = TriangleB - TriangleA;
    const FVector Edge1 = TriangleC - TriangleA;
    const FVector ToPoint = Point - TriangleA;

    const float D00 =
        FVector::DotProduct(Edge0, Edge0);

    const float D01 =
        FVector::DotProduct(Edge0, Edge1);

    const float D11 =
        FVector::DotProduct(Edge1, Edge1);

    const float D20 =
        FVector::DotProduct(ToPoint, Edge0);

    const float D21 =
        FVector::DotProduct(ToPoint, Edge1);

    const float Denominator =
        D00 * D11 - D01 * D01;

    if (FMath::Abs(Denominator) <= SpatialEpsilon)
    {
        return Result;
    }

    const float V =
        (D11 * D20 - D01 * D21) /
        Denominator;

    const float W =
        (D00 * D21 - D01 * D20) /
        Denominator;

    const float U = 1.0f - V - W;

    Result.bValid = true;
    Result.Coordinates = FVector(U, V, W);

    Result.bInsideTriangle =
        U >= -Tolerance &&
        V >= -Tolerance &&
        W >= -Tolerance &&
        U <= 1.0f + Tolerance &&
        V <= 1.0f + Tolerance &&
        W <= 1.0f + Tolerance;

    return Result;
}

FVector USpatialMathBlueprintLibrary::WorldPointToLocal(
    const FTransform& Transform,
    const FVector& WorldPoint
)
{
    return Transform.InverseTransformPosition(
        WorldPoint
    );
}

FVector USpatialMathBlueprintLibrary::LocalPointToWorld(
    const FTransform& Transform,
    const FVector& LocalPoint
)
{
    return Transform.TransformPosition(
        LocalPoint
    );
}

FQuat USpatialMathBlueprintLibrary::MakeShortestArcRotation(
    const FVector& FromDirection,
    const FVector& ToDirection,
    bool& bSuccess
)
{
    const FVector From =
        FromDirection.GetSafeNormal();

    const FVector To =
        ToDirection.GetSafeNormal();

    bSuccess =
        !From.IsNearlyZero() &&
        !To.IsNearlyZero();

    return bSuccess
        ? FQuat::FindBetweenNormals(From, To).
            GetNormalized()
        : FQuat::Identity;
}

FQuat USpatialMathBlueprintLibrary::SlerpNormalized(
    const FQuat& FromRotation,
    const FQuat& ToRotation,
    const float Alpha
)
{
    return FQuat::Slerp(
        FromRotation.GetNormalized(),
        ToRotation.GetNormalized(),
        FMath::Clamp(Alpha, 0.0f, 1.0f)
    ).GetNormalized();
}

FSpatialBallisticSolution
USpatialMathBlueprintLibrary::SolveBallisticArc(
    const FVector& Start,
    const FVector& Target,
    const float LaunchSpeed,
    const float GravityMagnitude,
    const bool bHighArc
)
{
    FSpatialBallisticSolution Result;

    if (LaunchSpeed <= SpatialEpsilon ||
        GravityMagnitude <= SpatialEpsilon)
    {
        return Result;
    }

    const FVector Delta = Target - Start;
    const FVector HorizontalDelta(
        Delta.X,
        Delta.Y,
        0.0f
    );

    const float HorizontalDistance =
        HorizontalDelta.Size();

    if (HorizontalDistance <= SpatialEpsilon)
    {
        return Result;
    }

    const float VerticalDistance = Delta.Z;
    const float SpeedSquared =
        FMath::Square(LaunchSpeed);

    const float Gravity = GravityMagnitude;

    const float Discriminant =
        FMath::Square(SpeedSquared) -
        Gravity *
        (
            Gravity *
            FMath::Square(HorizontalDistance) +
            2.0f *
            VerticalDistance *
            SpeedSquared
        );

    if (Discriminant < 0.0f)
    {
        return Result;
    }

    const float Root =
        FMath::Sqrt(
            FMath::Max(0.0f, Discriminant)
        );

    const float Numerator =
        bHighArc
            ? SpeedSquared + Root
            : SpeedSquared - Root;

    const float Tangent =
        Numerator /
        (Gravity * HorizontalDistance);

    const float AngleRadians =
        FMath::Atan(Tangent);

    const float Cosine = FMath::Cos(AngleRadians);
    const float Sine = FMath::Sin(AngleRadians);

    if (FMath::Abs(Cosine) <= SpatialEpsilon)
    {
        return Result;
    }

    const FVector HorizontalDirection =
        HorizontalDelta / HorizontalDistance;

    Result.bValid = true;

    Result.LaunchVelocity =
        HorizontalDirection *
        (LaunchSpeed * Cosine) +
        FVector::UpVector *
        (LaunchSpeed * Sine);

    Result.FlightTime =
        HorizontalDistance /
        (LaunchSpeed * Cosine);

    Result.LaunchAngleDegrees =
        FMath::RadiansToDegrees(AngleRadians);

    return Result;
}

FVector USpatialMathBlueprintLibrary::EvaluateBallisticPosition(
    const FVector& Start,
    const FVector& LaunchVelocity,
    const float GravityMagnitude,
    const float TimeSeconds
)
{
    const float Time =
        FMath::Max(0.0f, TimeSeconds);

    const FVector Gravity(
        0.0f,
        0.0f,
        -FMath::Abs(GravityMagnitude)
    );

    return Start +
        LaunchVelocity * Time +
        0.5f * Gravity * Time * Time;
}
