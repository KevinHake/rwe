// Visual Test 09: Unit Shadow Projection (unitShadow shader + stencil)
//
// Tests the shadow rendering pipeline, which projects 3D unit geometry flat
// onto the ground plane and uses the stencil buffer to avoid double-darkening
// where shadows overlap.
//
// Current code path:
//   - GameScene::renderWorld() (src/rwe/game/GameScene.cpp:753-779)
//     1. Enable stencil buffer
//     2. Disable color writes, enable stencil writes
//     3. Render shadow meshes with unitShadow shader (writes to stencil only)
//     4. Switch to stencil-as-mask mode, enable color writes
//     5. fillScreen(0, 0, 0, 0.5) -- semi-transparent black only where stencil=1
//     6. Disable stencil
//
// Shader uniforms:
//   - vpMatrix: view-projection (no model rotation for shadow)
//   - modelMatrix: model transform
//   - groundHeight: y-coordinate to project shadows onto
//
// The unitShadow vertex shader flattens the y-coordinate to groundHeight,
// creating a flat shadow shape. The fragment shader discards transparent texels.
//
// COMPLEXITY: This test requires a depth-stencil attachment on the offscreen
// framebuffer. For OpenGL, the FBO needs GL_DEPTH24_STENCIL8. For SDL_gpu,
// use SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT.
//
// Test setup:
//   - 800x600 offscreen render target with depth-stencil attachment
//   - Create a simple elevated mesh (cube floating above ground)
//   - Draw a ground-colored background plane first
//   - Execute the full shadow pipeline (stencil write → mask → overlay)
//   - Two overlapping shadow casters to verify stencil prevents double-darkening
//
// Expected output: Dark shadow shapes on the ground. Where two shadows overlap,
// the darkness should be the same as a single shadow (not darker).

#include "visual_test_harness.h"

int main(int argc, char* argv[])
{
    // TODO: Parse arguments, initialize backend
    // TODO: Create offscreen target with depth-stencil
    // TODO: Draw ground plane
    // TODO: Execute stencil shadow pipeline
    // TODO: Verify overlapping shadows don't double-darken
    // TODO: Save and compare
    return 0;
}
