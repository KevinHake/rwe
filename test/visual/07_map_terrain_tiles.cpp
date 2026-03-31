// Visual Test 07: Map Terrain Tiles (mapTerrain shader)
//
// Tests texture array sampling for terrain rendering. The mapTerrain shader uses
// sampler2DArray with a 3-component texture coordinate: (u, v, layer_index).
// Each terrain tile references a layer in the texture array.
//
// Current code path:
//   - RenderService::drawMapTerrain() (src/rwe/RenderService.cpp:15-63)
//     Groups visible tiles by texture array, creates GlTextureArrayVertex meshes
//     Vertex format: pos(x,y,z) + texCoord(u,v,w) where w = layer index
//
// INEFFICIENCY: Terrain mesh is recreated every frame (GL_STREAM_DRAW) even
// though terrain geometry never changes. Should create terrain mesh once and
// store in a persistent GPU buffer. Only the camera transform (MVP matrix)
// changes per frame.
//
// Shader: mapTerrain
//   - Vertex: transforms position, passes through 3D texcoord
//   - Fragment: samples texture array at (texCoord.xy, texCoord.z)
//
// Test setup:
//   - 800x600 offscreen render target
//   - Create a small texture array with 4 distinct procedural tile textures:
//     Layer 0: solid green (grass)
//     Layer 1: solid brown (dirt)
//     Layer 2: solid blue (water)
//     Layer 3: solid gray (rock)
//   - Build a 4x4 grid of quads, each referencing a different layer
//   - Use orthographic projection looking straight down
//
// Expected output: A 4x4 grid of colored tiles.

#include "visual_test_harness.h"

int main(int argc, char* argv[])
{
    // TODO: Parse arguments, initialize backend
    // TODO: Create texture array with 4 layers
    // TODO: Build 4x4 terrain grid mesh (GlTextureArrayVertex)
    // TODO: Draw with mapTerrain shader
    // TODO: Save and compare
    return 0;
}
