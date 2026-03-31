#pragma once

// Visual test harness for offscreen rendering to PNG.
//
// Supports two backends:
//   - OpenGL: Creates a hidden SDL window with GL context, renders to FBO,
//     reads pixels with glReadPixels.
//   - SDL_gpu: Creates a hidden SDL window with SDL_GPUDevice, renders to
//     offscreen texture, downloads via transfer buffer.
//
// Usage in a test:
//
//   int main(int argc, char* argv[])
//   {
//       VisualTestConfig config;
//       if (!parseArgs(argc, argv, config)) return 1;
//
//       if (config.backend == Backend::OpenGL)
//       {
//           auto ctx = createOpenGlContext(config);
//           // ... render using GraphicsContext / ShaderService ...
//           saveFramebuffer(ctx, config);
//       }
//       else
//       {
//           auto ctx = createSdlGpuContext(config);
//           // ... render using SDL_gpu calls ...
//           saveOffscreenTexture(ctx, config);
//       }
//
//       if (config.hasReference())
//           return compareWithReference(config) ? 0 : 1;
//       return 0;
//   }
//
// TODO: Implement OpenGL context setup (hidden window + FBO + glReadPixels)
// TODO: Implement SDL_gpu context setup (hidden window + offscreen texture + download)
// TODO: Implement CLI argument parsing
// TODO: Implement PNG save via PngImage from src/rwe/util/png_write.h

#include <cstdint>
#include <string>

namespace rwe
{
    enum class Backend
    {
        OpenGL,
        SdlGpu
    };

    struct VisualTestConfig
    {
        Backend backend = Backend::OpenGL;
        uint32_t width = 800;
        uint32_t height = 600;
        std::string outputPath;
        std::string referencePath;
        bool generateReference = false;
        uint8_t tolerance = 2; // per-channel tolerance (0-255)
        float maxDiffPercent = 0.5f; // max % of pixels allowed to differ

        bool hasReference() const { return !referencePath.empty(); }
    };

    // TODO: bool parseArgs(int argc, char* argv[], VisualTestConfig& config);

    // TODO: OpenGL context wrapper
    // struct OpenGlTestContext { SDL_Window* window; SDL_GLContext gl; FBO fbo; };
    // OpenGlTestContext createOpenGlContext(const VisualTestConfig& config);
    // void saveFramebuffer(const OpenGlTestContext& ctx, const VisualTestConfig& config);

    // TODO: SDL_gpu context wrapper
    // struct SdlGpuTestContext { SDL_Window* window; SDL_GPUDevice* device; SDL_GPUTexture* target; };
    // SdlGpuTestContext createSdlGpuContext(const VisualTestConfig& config);
    // void saveOffscreenTexture(const SdlGpuTestContext& ctx, const VisualTestConfig& config);
}
