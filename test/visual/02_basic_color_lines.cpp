// Visual Test 02: Lines and Line Strips (basicColor shader)
//
// Tests line rendering primitives:
//   - GL_LINES: Used for nanolathe construction beams, debug overlays, unit order lines
//   - GL_LINE_STRIP: Used for selection rectangles around units
//
// Current code paths:
//   - RenderService::drawBatch() (src/rwe/RenderService.cpp:141-162)
//     Creates a single mesh from ColoredMeshBatch::lines, draws with GL_LINES
//   - RenderService::drawLineStripsBatch() (src/rwe/RenderService.cpp:249-259)
//     Each selected unit's selection rectangle is a separate line strip draw call
//   - UiRenderService::drawLine() (src/rwe/UiRenderService.cpp:297-312)
//     Allocates a VAO+VBO for just 2 vertices per line (!)
//
// INEFFICIENCY: drawLine() creates a mesh for every single line drawn.
// drawLineStripsBatch() does per-unit draw calls. Both should batch.
//
// Test setup:
//   - 800x600 offscreen render target, dark background
//   - Draw several line segments (GL_LINES):
//     1. Horizontal white line
//     2. Vertical green line
//     3. Diagonal red line
//   - Draw a line strip forming a rectangle (simulating unit selection):
//     5 vertices: A->B->C->D->A (closed quad)
//   - Use orthographic projection
//
// Expected output: Several colored lines and a rectangular outline.

#include "visual_test_harness.h"

int main(int argc, char* argv[])
{
    // TODO: Parse arguments, initialize backend
    // TODO: Draw GL_LINES segments
    // TODO: Draw GL_LINE_STRIP rectangle (5 vertices to close the quad)
    // TODO: Save and compare
    return 0;
}
