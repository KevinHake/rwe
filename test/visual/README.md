# Visual Regression Tests

Visual tests for verifying rendering correctness during the OpenGL-to-SDL_gpu migration.

Each test isolates a specific rendering element, renders it offscreen, and saves the
output as a PNG. Tests can run against both the OpenGL backend (for generating reference
images) and the SDL_gpu backend (for comparison).

## Running

```bash
# Build all visual tests
make vt_01 vt_02 vt_03  # etc.

# Generate a reference image from the OpenGL backend
./vt_01 --backend opengl --generate-reference

# Compare SDL_gpu output against the reference
./vt_01 --backend sdl_gpu --reference test/visual/reference/01_basic_color_filled_rect.png

# Just render and save output without comparison
./vt_01 --backend sdl_gpu --output my_output.png
```

Exit code 0 = pass (or no comparison requested), non-zero = fail.
When a comparison fails, a diff image is saved alongside the output.

## Test List

| # | Test | Shader | What it validates |
|---|------|--------|-------------------|
| 01 | basic_color_filled_rect | basicColor | Filled rectangles (bars, fills); also tests blit alternative |
| 02 | basic_color_lines | basicColor | Lines and line strips (nanolathes, selections) |
| 03 | basic_color_triangle | basicColor | Triangles with interpolation and alpha blending |
| 04 | basic_texture_sprite | basicTexture | Textured quad with UV sampling |
| 05 | basic_texture_tinted | basicTexture | Tint uniform (player colors, translucency) |
| 06 | basic_texture_font | basicTexture | Many small quads (text rendering pattern) |
| 07 | map_terrain_tiles | mapTerrain | Texture array sampling for terrain |
| 08 | unit_texture_model | unitTexture | 3D mesh with lighting and sea-level fog |
| 09 | unit_shadow_projection | unitShadow | Shadow projection with stencil masking |
| 10 | unit_build_animation | unitBuild | Construction dissolve effect |
| 11 | flash_effect | flashEffect | Radial weapon flash to dodge mask |
| 12 | world_post_composite | worldPost | Dodge/burn post-processing |
| 13 | multi_pass_scene | all | Full render pipeline integration |

All tests use procedural geometry and textures -- no TA game assets required.

## Architecture

Tests use a shared harness (`harness/visual_test_harness.h`) that handles:
- SDL initialization with a hidden window
- Backend selection (OpenGL FBO or SDL_gpu offscreen texture)
- Pixel readback and PNG output via `PngImage`
- CLI argument parsing

Image comparison (`harness/image_compare.h`) provides pixel-diff with configurable
per-channel tolerance to account for minor GPU vendor differences.

## Target Rendering Architecture

The SDL_gpu backend should adopt a layered, cache-friendly rendering approach
that takes advantage of explicit render targets and load/store operations.

### Frame structure

```
1. 3D scene into world render target (the expensive part)
2. Static chrome texture (1 quad -- nearly free)
3. Dynamic bar fills (blit ops, no shader needed)
4. Dynamic text/sprites into UI "holes" (only what changed)
5. Minimap
6. World-space overlays (health bars, selection rects)
7. Cursor
```

### Static UI chrome

TA's UI is a mostly-static opaque frame (panel borders, button backgrounds, labels).
Render it once into a retained render target texture; draw it as a single textured quad
each frame. Re-render only when the UI mode changes (building menu opened, unit selected,
etc.). SDL_gpu's explicit load/store model makes this natural: use `load: load` to
preserve the previous frame's content rather than `load: clear`.

Semi-static variations (a few different panel modes) can be pre-rendered as separate
overlay textures that swap on mode change.

### Dynamic UI elements

Only a few things actually change per-frame:
- Energy/metal bar fills (axis-aligned solid-color rects -- use blit ops, not shaders)
- Resource numbers (text, updates ~1/sec)
- Selected unit portrait (changes on selection)
- Minimap (small render target, updated per frame)

### Caveats

- **Dirty tracking**: The current code is fully immediate-mode (re-render everything
  every frame). Caching requires tracking when UI state changes to know when to
  re-render the chrome texture. This is a design pattern shift, not just an optimization.
- **Window resize / DPI**: Cached textures are resolution-specific. Must re-render
  on resize or DPI change.
- **Button animations**: Press/release states need either a chrome re-render (simple,
  infrequent) or small dynamic overlays for pressed states.
- **Compositing order**: Render 3D scene first, then draw the opaque chrome on top.
  The chrome naturally covers the border area with no alpha blending needed.
- **TA's 640x480 design**: Chrome should be rendered at native window resolution
  using the original sprite assets, then cached. Scaling a pre-rendered 640x480
  texture to higher resolutions would look blurry.

## Known Rendering Inefficiencies (to fix during migration)

These are documented in test comments and should be addressed when implementing
the SDL_gpu backend:

- **fillColor()**: Allocates new VAO/VBO per rectangle per frame. For axis-aligned
  solid-color rectangles (bars, borders), use SDL_gpu blit operations instead of
  the full shader pipeline. For non-axis-aligned colored geometry, batch into a
  single vertex buffer per frame.
- **drawText()**: Per-character draw calls. Should batch glyphs by texture atlas.
- **drawSpriteBatch()**: Per-sprite texture bind + draw call. Should sort by texture
  and batch vertex data.
- **drawMapTerrain()**: Recreates terrain mesh every frame. Terrain is static and
  should use persistent GPU buffers.
- **drawLine()**: Allocates VAO/VBO for 2 vertices. Should batch into line buffer.
- **UI chrome**: Every panel sprite, button, and label is re-rendered every frame.
  Should be cached in a retained render target (see Target Rendering Architecture).
