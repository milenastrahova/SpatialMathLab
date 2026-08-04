# Testing

## Verified Result

After the final camera and layout changes:

- expected tests: **25**;
- test failures: **0**;
- Unreal process exit: **normal**;
- test group: `SpatialMathCore`.

The automated runner observed 52 success log markers because Unreal may emit more than one success marker during a test run. The implemented test count remains 25.

## Covered Areas

### Vectors

- projection;
- zero-vector projection rejection;
- rejection;
- reflection;
- positive and negative signed angles.

### Geometry and Targeting

- closest point inside a segment;
- endpoint clamping;
- point-to-segment distance;
- target inside and outside a cone.

### Intersections

- ray-plane hit;
- parallel ray-plane miss;
- ray-sphere hit and miss;
- ray-AABB hit and miss.

### Barycentric Coordinates

- inside classification;
- outside classification;
- weight sum verification.

### Transforms and Quaternions

- local/world/local round trip;
- shortest-arc direction mapping;
- SLERP endpoints.

### Ballistics

- low-arc target hit;
- high-arc angle comparison;
- unreachable-target rejection.

## Reproduce

Close Unreal Editor and run:

```powershell
powershell -ExecutionPolicy Bypass -File ".\Tools\Run-SpatialMathCoreTests.ps1"
```

The script:

1. launches `UnrealEditor-Cmd`;
2. runs `Automation RunTest SpatialMathCore`;
3. writes an absolute Unreal log;
4. verifies the normal exit code;
5. verifies at least 25 success markers;
6. rejects any failure markers.