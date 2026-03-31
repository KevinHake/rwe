#pragma once

// Pixel-diff image comparison for visual regression tests.
//
// Compares two PngImage instances and reports:
//   - Max per-channel difference
//   - Mean absolute difference across all pixels
//   - Count and percentage of pixels exceeding tolerance
//   - A visual diff image (green=match, red=mismatch, brightness=magnitude)
//
// Usage:
//
//   PngImage actual = ...;   // rendered output
//   PngImage reference = ...; // golden reference
//   auto result = compareImages(actual, reference, tolerance, maxDiffPercent);
//   if (!result.passed) {
//       result.diffImage.write("diff.png");
//       return 1;
//   }
//
// TODO: Implement compareImages()
// TODO: Implement PngImage loading (current PngImage only writes, need read support)
//       Alternative: use stb_image or libpng directly for loading reference PNGs

#include <cstdint>
#include <string>

namespace rwe
{
    struct ImageDiffResult
    {
        bool passed;
        uint8_t maxChannelDiff;     // worst single-channel difference (0-255)
        float meanDiff;             // mean absolute difference across all pixels/channels
        uint32_t diffPixelCount;    // pixels exceeding tolerance
        float diffPixelPercent;     // diffPixelCount / totalPixels * 100
        // TODO: PngImage diffImage; // visual diff output
    };

    // TODO: ImageDiffResult compareImages(
    //     const PngImage& actual,
    //     const PngImage& reference,
    //     uint8_t perChannelTolerance = 2,
    //     float maxDiffPixelPercent = 0.5f);

    // TODO: PngImage loadPng(const std::string& path);
}
