#pragma once

// ============================================================================
// GameCoordinates.h
//
// This is the engine's unit contract. It is declared here ONCE, and every
// other subsystem (Camera, PointLight, Transform, Model, and anything added
// later -- physics, audio falloff, AI perception ranges, etc.) is written
// assuming this contract already holds. No subsystem should introduce its
// own unit assumption, convert units internally, or "just multiply by 100"
// locally -- if a conversion is needed, it happens here, at load time, once.
//
// THE CONTRACT:
//   1 engine unit = 1 meter.
//
// Any float representing a position, scale, distance, or radius that lives
// inside a Transform, Camera, or PointLight is a meter value. If you're
// about to write a distance constant and you're not sure if it's "meters",
// it should be -- go find where it's used and confirm, don't guess.
//
// WHERE CONVERSION IS ALLOWED TO HAPPEN:
//   Only at the import/parsing boundary -- i.e. inside Model's constructor,
//   where a foreign asset's native unit gets translated into meters exactly
//   once, before anything is stored in a Node/Transform. After that point,
//   the data is meters and stays meters for its entire lifetime in the
//   engine. See Model::Model in Mesh.cpp for where this is applied.
//
// HANDEDNESS / AXIS CONVENTION (documented here so it isn't re-derived):
//   Left-handed, Y-up -- matches DirectXMath/D3D11's defaults and the
//   aiProcess_ConvertToLeftHanded flag already used in Model's Assimp import.
// ============================================================================

namespace GameCoordinates
{
    constexpr float MetersPerUnit = 1.0f;

    // Conversion helpers -- use these ONLY at the import boundary (asset
    // loading). Never call these on a value that's already inside a
    // Transform/Camera/PointLight; if you find yourself doing that, it means
    // something upstream failed to convert at the boundary and that's the
    // actual bug to fix.
    constexpr float CentimetersToMeters(float cm) noexcept { return cm * 0.01f; }
    constexpr float MetersToCentimeters(float m)  noexcept { return m * 100.0f; }

    // Sanity-check thresholds for imported model bounding boxes (meters).
    // A humanoid character is roughly 1.5-2m tall; a small prop is
    // centimeters to tens of centimeters. Anything wildly outside this is
    // very likely an un-applied or double-applied unit conversion, not an
    // intentionally huge/tiny asset -- so we warn loudly rather than
    // silently rendering something that's 100x too big or too small.
    constexpr float SuspiciouslySmallMeters = 0.001f; // 1mm
    constexpr float SuspiciouslyLargeMeters = 1000.0f; // 1km
}