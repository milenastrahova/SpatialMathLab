# SpatialMathLab

<p align="center">
  <strong>Unreal Engine 5.8 C++ Spatial Mathematics Toolkit</strong><br>
  Reusable Runtime plugin, five interactive demonstrations and 25 verified automation tests.
</p>

<p align="center">
  <img src="media/spatialmathlab-overview.png" alt="SpatialMathLab overview" width="100%">
</p>

## Overview

SpatialMathLab demonstrates applied 3D mathematics through a reusable Unreal Engine Runtime plugin and an interactive laboratory.

The project translates linear algebra and computational geometry into practical gameplay and engine-facing systems:

- vector decomposition and reflection;
- signed angles and field-of-view classification;
- closest-point calculations;
- ray-plane, ray-sphere and ray-AABB intersections;
- barycentric coordinates;
- local/world transform conversion;
- shortest-arc quaternion rotation and normalized SLERP;
- target scoring;
- analytical low/high ballistic trajectories.

## Verified Engineering Evidence

| Area | Evidence |
|---|---|
| Engine | Unreal Engine 5.8 |
| Language | C++ |
| Architecture | Reusable `SpatialMathCore` Runtime plugin |
| Integration | Blueprint-callable public API |
| Visual validation | Five interactive debug stations |
| Automated validation | 25/25 tests passed |
| Edge cases | Degenerate vectors, misses, parallel rays and unreachable targets |
| Build | Clean Editor build with `BuildSettingsVersion.V7` |
| Release | [`v1.0.0`](https://github.com/milenastrahova/SpatialMathLab/releases/tag/v1.0.0) |

## Interactive Laboratory

### 01 â€” Vector Projection

Decomposes an animated input vector into projection and rejection components relative to a basis vector.

Practical uses include slope movement, steering, surface alignment and directional filtering.

<p align="center">
  <img src="media/vector-projection.png" alt="Vector projection demonstration" width="88%">
</p>

### 02 â€” Quaternion SLERP

Visualizes shortest-arc rotation and normalized spherical interpolation between two directions.

Practical uses include camera rotation, lock-on transitions, aiming and orientation blending.

<p align="center">
  <img src="media/quaternion-slerp.png" alt="Quaternion SLERP demonstration" width="88%">
</p>

### 03 â€” Ray Intersections

Visualizes a ray-sphere hit, contact point, hit distance, surface normal and a ray-AABB demonstration.

Practical uses include aiming, interaction traces, visibility checks and spatial queries.

<p align="center">
  <img src="media/ray-intersections.png" alt="Ray intersection demonstration" width="88%">
</p>

### 04 â€” Target Selection

Classifies moving targets using range, field-of-view cone checks, dot-product alignment and distance weighting.

Practical uses include lock-on systems, aim assistance and contextual targeting.

<p align="center">
  <img src="media/target-selection.png" alt="Target selection demonstration" width="88%">
</p>

### 05 â€” Ballistic Solutions

Solves and visualizes analytical low-arc and high-arc projectile trajectories under gravity.

Practical uses include projectile weapons, AI ranged combat, trajectory previews and artillery systems.

<p align="center">
  <img src="media/ballistic-solutions.png" alt="Ballistic trajectory demonstration" width="88%">
</p>

## SpatialMathCore Plugin

The reusable Runtime plugin owns the mathematical implementation and validation.

### Public API

`USpatialMathBlueprintLibrary` exposes stateless C++ functions to C++ and Blueprints.

Main result structures:

- `FSpatialRayHit`;
- `FSpatialBarycentricResult`;
- `FSpatialBallisticSolution`.

### Design Principles

- deterministic stateless calculations;
- explicit validity results;
- safe handling of invalid and degenerate inputs;
- floating-point tolerance instead of exact equality;
- separation between reusable mathematics and presentation;
- tests focused on both successful and failure cases.

## Automated Testing

All 25 `SpatialMathCore` tests passed again after the final layout and camera changes.

<p align="center">
  <img src="media/automation-tests-passed.png" alt="SpatialMathCore automation tests passed" width="88%">
</p>

Test coverage includes:

- projection, rejection and reflection;
- positive and negative signed angles;
- zero-length vectors;
- closest points and endpoint clamping;
- point-to-segment distance;
- target inclusion and exclusion;
- ray hits, misses and parallel cases;
- barycentric inside/outside classification;
- transform round trips;
- shortest-arc quaternion mapping;
- SLERP endpoints;
- reachable and unreachable ballistic solutions.

Reproduce the test run with:

```powershell
powershell -ExecutionPolicy Bypass -File ".\Tools\Run-SpatialMathCoreTests.ps1"
```

Detailed results: [docs/TESTING.md](docs/TESTING.md)

## Architecture

```text
SpatialMathLab/
|-- Plugins/
|   `-- SpatialMathCore/
|       `-- Source/
|           `-- SpatialMathCore/
|               |-- Public/
|               `-- Private/
|                   `-- Tests/
|-- Source/
|   `-- SpatialMathLab/
|-- Content/
|-- Config/
|-- Tools/
|-- docs/
`-- media/
```

The plugin contains reusable mathematics. The host project contains the laboratory, animation, camera controls and portfolio presentation.

Further documentation:

- [Architecture](docs/ARCHITECTURE.md)
- [Applied Mathematics](docs/MATHEMATICS.md)
- [Testing](docs/TESTING.md)
- [Portfolio Summary](docs/PORTFOLIO_SUMMARY.md)

## Camera Controls

| Key | Action |
|---|---|
| `0` | Overview |
| `1` | Vector Projection |
| `2` | Quaternion SLERP |
| `3` | Ray Intersections |
| `4` | Target Selection |
| `5` | Ballistic Solutions |
| `WASD` | Free movement |
| `Q / E` | Move down / up |
| `Shift` | Faster movement |
| Hold `RMB` | Free look |

## Build

Requirements:

- Unreal Engine 5.8;
- Visual Studio with **Game development with C++**;
- Windows 64-bit.

Open `SpatialMathLab.uproject`. Unreal may rebuild the project modules automatically.

## Portfolio Relevance

SpatialMathLab provides reviewable evidence of:

- applied linear algebra and 3D geometry;
- Unreal Engine C++ architecture;
- reusable plugin development;
- Blueprint API design;
- numerical edge-case handling;
- automated testing;
- debug visualization;
- gameplay-facing targeting and projectile mathematics.

### CV Summary

> Developed a reusable Unreal Engine 5.8 C++ plugin for vector decomposition, signed angles, transform-space conversion, quaternion interpolation, ray intersections, target selection and analytical ballistic trajectories. Added an interactive five-station laboratory, Blueprint APIs and 25 automation tests covering numerical and degenerate edge cases.

## Release

Latest release:

[SpatialMathLab v1.0.0](https://github.com/milenastrahova/SpatialMathLab/releases/tag/v1.0.0)

## Author

**Milena Strahova**

- [GitHub](https://github.com/milenastrahova)
- [ArtStation](https://www.artstation.com/milenastrahova)