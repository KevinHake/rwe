// Visual Test 06: Font / Text Rendering (basicTexture shader)
//
// Tests rendering many small textured quads in sequence, simulating how the
// engine renders bitmap fonts. TA fonts are stored as GAF sprite series where
// each glyph is a separate sprite with its own bounds.
//
// Current code path:
//   - UiRenderService::drawText() (src/rwe/UiRenderService.cpp:59-85)
//     Iterates UTF-8 codepoints, calls drawSprite() per character
//   - Each character: push matrix, bind texture, set MVP, draw 6 vertices, pop matrix
//
// INEFFICIENCY: Each character is a separate draw call with its own texture bind
// and uniform upload. A 20-character string = 20 draw calls. Should batch all
// glyphs into a single vertex buffer since they share the same font texture atlas.
//
// Test setup:
//   - 800x600 offscreen render target, dark background
//   - Create a procedural "font atlas" texture: 16x16 grid of colored cells (256 glyphs)
//     Each cell is 8x8 pixels, total atlas 128x128
//   - Render 30+ small quads sampling different cells (simulating a text string)
//   - Position quads sequentially left-to-right with variable widths
//   - Verify UV coordinates correctly sample individual cells without bleeding
//
// Expected output: A row of small colored squares from different atlas positions,
// simulating rendered text.

#include "visual_test_harness.h"

int main(int argc, char* argv[])
{
    // TODO: Parse arguments, initialize backend
    // TODO: Create 128x128 procedural font atlas texture
    // TODO: Render 30+ quads sampling different atlas cells
    // TODO: Verify no UV bleeding between adjacent cells
    // TODO: Save and compare
    return 0;
}
