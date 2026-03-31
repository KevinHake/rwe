// Visual Test 04: Textured Sprite (basicTexture shader)
//
// Tests basic texture sampling with a procedural texture. This is the foundation
// for all sprite rendering:
//   - Map features (trees, rocks, metal spots, geothermal vents)
//   - UI elements (buttons, panel backgrounds)
//   - Projectile sprites, explosion sprites
//   - Cursor rendering
//
// Current code path:
//   - RenderService::drawSpriteBatch() (src/rwe/RenderService.cpp:235-248)
//     Per-sprite: bind texture, set MVP matrix, set tint, draw 6 vertices
//   - UiRenderService::drawSprite() (src/rwe/UiRenderService.cpp:89-112)
//     Draws a single sprite using its pre-built mesh
//
// Shader uniforms:
//   - mvpMatrix: transforms quad vertices
//   - tint: RGBA color multiplied with texture sample (vec4)
//
// Test setup:
//   - 800x600 offscreen render target, dark background
//   - Create a procedural 64x64 checkerboard texture (black/white)
//   - Upload to GPU as a texture
//   - Render a textured quad centered on screen with tint=(1,1,1,1)
//   - Render a second smaller quad in the corner to test UV accuracy
//
// Expected output: Checkerboard pattern on a quad, with sharp texel boundaries
// (nearest-neighbor filtering matches the engine's GL_NEAREST setting).

#include "visual_test_harness.h"

int main(int argc, char* argv[])
{
    // TODO: Parse arguments, initialize backend
    // TODO: Create 64x64 checkerboard texture (alternating 8x8 blocks)
    // TODO: Upload texture to GPU
    // TODO: Create textured quad mesh (GlTexturedVertex: pos + uv)
    // TODO: Draw with basicTexture shader, tint=(1,1,1,1)
    // TODO: Save and compare
    return 0;
}
