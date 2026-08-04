# Applied Mathematics

## Vector Projection and Rejection

For vector `v` projected onto vector `b`:

```text
projection = b * dot(v, b) / dot(b, b)
rejection  = v - projection
```

The implementation rejects a zero-length basis vector.

## Reflection

A vector reflected across normalized surface normal `n`:

```text
reflected = v - 2 * dot(v, n) * n
```

## Signed Angle

The unsigned angle comes from the clamped dot product. Orientation comes from the reference axis and cross product:

```text
angle = acos(clamp(dot(from, to), -1, 1))
sign  = sign(dot(axis, cross(from, to)))
```

## Field-of-View Cone

A target is inside a cone when:

```text
dot(normalizedForward, normalizedToTarget)
    >= cos(halfAngle)
```

## Closest Point on Segment

The projection parameter is clamped to `[0, 1]`, preventing the result from leaving the finite segment.

## Ray-Plane Intersection

The solution rejects near-zero denominators, which represent parallel or nearly parallel rays.

## Ray-Sphere Intersection

The quadratic discriminant determines whether the ray misses, touches or intersects the sphere.

## Ray-AABB Intersection

The slab method intersects the ray with each axis interval and tracks near/far distances.

## Barycentric Coordinates

Barycentric weights classify a point relative to a triangle. Degenerate triangles are rejected when the denominator approaches zero.

## Transform Spaces

The library wraps transform and inverse-transform operations and verifies:

```text
local -> world -> local
```

within floating-point tolerance.

## Quaternion Rotation

Shortest-arc rotation maps one normalized direction onto another. SLERP produces normalized intermediate rotations and avoids linear interpolation artifacts.

## Analytical Ballistics

For a fixed launch speed, gravity and target displacement, the solver evaluates the discriminant and returns low or high trajectory solutions. Negative discriminants are reported as unreachable targets.

The trajectory evaluator uses:

```text
position(t) =
    start
    + velocity * t
    + 0.5 * gravity * t^2
```