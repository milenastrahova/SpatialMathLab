# Architecture

## Separation of Responsibilities

SpatialMathLab is divided into two layers.

### SpatialMathCore Plugin

The Runtime plugin owns reusable mathematical functionality:

- stateless Blueprint-callable functions;
- result structures for intersections, barycentric coordinates and ballistics;
- numerical edge-case handling;
- debug visualization Actor;
- Unreal Automation Framework tests.

The plugin does not depend on the host laboratory module.

### SpatialMathLab Host Project

The host project owns presentation:

- GameMode;
- free-fly camera;
- camera presets;
- five interactive mathematical stations;
- animated demonstration parameters;
- labels and debug drawing.

This separation allows the mathematical library to be moved into another Unreal project without carrying the portfolio presentation layer.

## Public API

`USpatialMathBlueprintLibrary` exposes the core functionality to both C++ and Blueprints.

Important result types:

- `FSpatialRayHit`;
- `FSpatialBarycentricResult`;
- `FSpatialBallisticSolution`.

## Design Decisions

### Stateless Library Functions

Pure calculations are implemented as stateless functions. This makes them:

- deterministic;
- simple to test;
- usable without Actor lifetime dependencies;
- safe to call from gameplay code or Blueprints.

### Explicit Validity Results

Operations that can fail return validity information instead of silently producing unreliable output. Examples include:

- projection onto a zero-length vector;
- degenerate barycentric triangles;
- invalid ray directions;
- unreachable ballistic targets.

### Numerical Tolerance

Tests use tolerances for floating-point comparisons. Exact equality is avoided for transform, quaternion and trajectory verification.

### Presentation Does Not Own Mathematics

The laboratory visualizer calls the public plugin API. It does not reimplement formulas in the host project.