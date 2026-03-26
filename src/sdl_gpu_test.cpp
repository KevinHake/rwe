#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

// Matches the GlColoredVertex layout: pos(3) + color(3)
struct ColoredVertex
{
    float x, y, z;
    float r, g, b;
};

// Matches the uniform block in basicColor.vert (set=1, binding=0)
struct VertexUniforms
{
    float mvpMatrix[16];
};

// Matches the uniform block in basicColor.frag (set=3, binding=0)
struct FragmentUniforms
{
    float alpha;
};

static std::vector<uint8_t> readFile(const char* path)
{
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f)
    {
        fprintf(stderr, "Failed to open: %s\n", path);
        exit(1);
    }
    auto size = f.tellg();
    f.seekg(0);
    std::vector<uint8_t> data(size);
    f.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

static SDL_GPUShader* loadShader(
    SDL_GPUDevice* device,
    const char* path,
    SDL_GPUShaderStage stage,
    uint32_t numSamplers,
    uint32_t numUniformBuffers)
{
    auto code = readFile(path);

    SDL_GPUShaderCreateInfo info = {};
    info.code = code.data();
    info.code_size = code.size();
    info.entrypoint = "main";
    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.stage = stage;
    info.num_samplers = numSamplers;
    info.num_storage_textures = 0;
    info.num_storage_buffers = 0;
    info.num_uniform_buffers = numUniformBuffers;

    auto shader = SDL_CreateGPUShader(device, &info);
    if (!shader)
    {
        fprintf(stderr, "Failed to create shader from %s: %s\n", path, SDL_GetError());
        exit(1);
    }
    return shader;
}

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    auto* window = SDL_CreateWindow("SDL_gpu test", 800, 600, 0);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

    auto* device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    if (!device)
    {
        fprintf(stderr, "SDL_CreateGPUDevice failed: %s\n", SDL_GetError());
        return 1;
    }

    printf("GPU driver: %s\n", SDL_GetGPUDeviceDriver(device));

    if (!SDL_ClaimWindowForGPUDevice(device, window))
    {
        fprintf(stderr, "SDL_ClaimWindowForGPUDevice failed: %s\n", SDL_GetError());
        return 1;
    }

    // Load SPIR-V shaders
    // vertex shader: 1 uniform buffer (VertexUniforms)
    auto* vertShader = loadShader(device, "shaders/gpu/basicColor.vert.spv", SDL_GPU_SHADERSTAGE_VERTEX, 0, 1);
    // fragment shader: 1 uniform buffer (FragmentUniforms)
    auto* fragShader = loadShader(device, "shaders/gpu/basicColor.frag.spv", SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 1);

    // Create graphics pipeline
    SDL_GPUVertexBufferDescription vbDesc = {};
    vbDesc.slot = 0;
    vbDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vbDesc.instance_step_rate = 0;
    vbDesc.pitch = sizeof(ColoredVertex);

    SDL_GPUVertexAttribute vertAttrs[2] = {};
    // position: location 0, vec3
    vertAttrs[0].buffer_slot = 0;
    vertAttrs[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertAttrs[0].location = 0;
    vertAttrs[0].offset = offsetof(ColoredVertex, x);
    // color: location 1, vec3
    vertAttrs[1].buffer_slot = 0;
    vertAttrs[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
    vertAttrs[1].location = 1;
    vertAttrs[1].offset = offsetof(ColoredVertex, r);

    SDL_GPUVertexInputState vertexInput = {};
    vertexInput.num_vertex_buffers = 1;
    vertexInput.vertex_buffer_descriptions = &vbDesc;
    vertexInput.num_vertex_attributes = 2;
    vertexInput.vertex_attributes = vertAttrs;

    SDL_GPURasterizerState rasterizer = {};
    rasterizer.fill_mode = SDL_GPU_FILLMODE_FILL;
    rasterizer.cull_mode = SDL_GPU_CULLMODE_BACK;
    rasterizer.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

    SDL_GPUMultisampleState multisample = {};
    multisample.sample_count = SDL_GPU_SAMPLECOUNT_1;

    SDL_GPUDepthStencilState depthStencil = {};
    depthStencil.enable_depth_test = false;
    depthStencil.enable_depth_write = false;
    depthStencil.enable_stencil_test = false;

    SDL_GPUColorTargetBlendState blendState = {};
    blendState.enable_blend = true;
    blendState.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    blendState.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    blendState.color_blend_op = SDL_GPU_BLENDOP_ADD;
    blendState.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    blendState.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    blendState.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    blendState.color_write_mask = SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G | SDL_GPU_COLORCOMPONENT_B | SDL_GPU_COLORCOMPONENT_A;

    auto swapchainFormat = SDL_GetGPUSwapchainTextureFormat(device, window);

    SDL_GPUColorTargetDescription colorTarget = {};
    colorTarget.format = swapchainFormat;
    colorTarget.blend_state = blendState;

    SDL_GPUGraphicsPipelineTargetInfo targetInfo = {};
    targetInfo.num_color_targets = 1;
    targetInfo.color_target_descriptions = &colorTarget;
    targetInfo.has_depth_stencil_target = false;

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.vertex_shader = vertShader;
    pipelineInfo.fragment_shader = fragShader;
    pipelineInfo.vertex_input_state = vertexInput;
    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    pipelineInfo.rasterizer_state = rasterizer;
    pipelineInfo.multisample_state = multisample;
    pipelineInfo.depth_stencil_state = depthStencil;
    pipelineInfo.target_info = targetInfo;

    auto* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
    if (!pipeline)
    {
        fprintf(stderr, "SDL_CreateGPUGraphicsPipeline failed: %s\n", SDL_GetError());
        return 1;
    }

    // Shaders can be released after pipeline creation
    SDL_ReleaseGPUShader(device, vertShader);
    SDL_ReleaseGPUShader(device, fragShader);

    // Create vertex buffer with a colored triangle
    // clang-format off
    ColoredVertex vertices[] = {
        { 0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f}, // top, red
        {-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f}, // bottom-left, green
        { 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f}, // bottom-right, blue
    };
    // clang-format on

    SDL_GPUBufferCreateInfo bufInfo = {};
    bufInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    bufInfo.size = sizeof(vertices);
    auto* vertexBuffer = SDL_CreateGPUBuffer(device, &bufInfo);

    // Upload vertex data via transfer buffer
    SDL_GPUTransferBufferCreateInfo tbInfo = {};
    tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tbInfo.size = sizeof(vertices);
    auto* transferBuffer = SDL_CreateGPUTransferBuffer(device, &tbInfo);

    auto* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    memcpy(mapped, vertices, sizeof(vertices));
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    auto* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(device);
    auto* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);
    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = transferBuffer;
    src.offset = 0;
    SDL_GPUBufferRegion dst = {};
    dst.buffer = vertexBuffer;
    dst.offset = 0;
    dst.size = sizeof(vertices);
    SDL_UploadToGPUBuffer(copyPass, &src, &dst, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);

    printf("Pipeline created, entering main loop.\n");

    auto renderFrame = [&](SDL_GPUCommandBuffer* cmdBuf, SDL_GPUTexture* targetTexture) {
        SDL_GPUColorTargetInfo colorTargetInfo = {};
        colorTargetInfo.texture = targetTexture;
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorTargetInfo.clear_color = {0.1f, 0.1f, 0.2f, 1.0f};

        auto* renderPass = SDL_BeginGPURenderPass(cmdBuf, &colorTargetInfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

        SDL_GPUBufferBinding vbBinding = {};
        vbBinding.buffer = vertexBuffer;
        vbBinding.offset = 0;
        SDL_BindGPUVertexBuffers(renderPass, 0, &vbBinding, 1);

        // Push vertex uniforms: identity matrix
        VertexUniforms vu = {};
        // Identity matrix (column-major)
        vu.mvpMatrix[0] = 1.0f;
        vu.mvpMatrix[5] = 1.0f;
        vu.mvpMatrix[10] = 1.0f;
        vu.mvpMatrix[15] = 1.0f;
        SDL_PushGPUVertexUniformData(cmdBuf, 0, &vu, sizeof(vu));

        // Push fragment uniforms: full opacity
        FragmentUniforms fu = {};
        fu.alpha = 1.0f;
        SDL_PushGPUFragmentUniformData(cmdBuf, 0, &fu, sizeof(fu));

        SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

        SDL_EndGPURenderPass(renderPass);
    };

    // If --screenshot is passed, render one frame to an offscreen texture, save it, and exit
    bool screenshotMode = false;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--screenshot") == 0)
        {
            screenshotMode = true;
        }
    }

    if (screenshotMode)
    {
        const Uint32 w = 800;
        const Uint32 h = 600;

        // Create offscreen render target
        SDL_GPUTextureCreateInfo texInfo = {};
        texInfo.type = SDL_GPU_TEXTURETYPE_2D;
        texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        texInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
        texInfo.width = w;
        texInfo.height = h;
        texInfo.layer_count_or_depth = 1;
        texInfo.num_levels = 1;
        texInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;
        auto* offscreenTex = SDL_CreateGPUTexture(device, &texInfo);

        // Render
        auto* cmdBuf = SDL_AcquireGPUCommandBuffer(device);
        renderFrame(cmdBuf, offscreenTex);

        // Download the texture via a transfer buffer
        SDL_GPUTransferBufferCreateInfo dlTbInfo = {};
        dlTbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
        dlTbInfo.size = w * h * 4;
        auto* dlTransferBuffer = SDL_CreateGPUTransferBuffer(device, &dlTbInfo);

        auto* dlCopyPass = SDL_BeginGPUCopyPass(cmdBuf);
        SDL_GPUTextureRegion texRegion = {};
        texRegion.texture = offscreenTex;
        texRegion.w = w;
        texRegion.h = h;
        texRegion.d = 1;
        SDL_GPUTextureTransferInfo transferDst = {};
        transferDst.transfer_buffer = dlTransferBuffer;
        transferDst.offset = 0;
        SDL_DownloadFromGPUTexture(dlCopyPass, &texRegion, &transferDst);
        SDL_EndGPUCopyPass(dlCopyPass);

        auto* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdBuf);
        SDL_WaitForGPUFences(device, true, &fence, 1);
        SDL_ReleaseGPUFence(device, fence);

        // Read pixels and save as BMP via SDL_Surface
        auto* pixels = SDL_MapGPUTransferBuffer(device, dlTransferBuffer, false);
        auto* surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, pixels, w * 4);
        if (surface)
        {
            SDL_SaveBMP(surface, "sdl_gpu_test.bmp");
            printf("Screenshot saved to sdl_gpu_test.bmp\n");
            SDL_DestroySurface(surface);
        }
        else
        {
            fprintf(stderr, "Failed to create surface: %s\n", SDL_GetError());
        }
        SDL_UnmapGPUTransferBuffer(device, dlTransferBuffer);
        SDL_ReleaseGPUTransferBuffer(device, dlTransferBuffer);
        SDL_ReleaseGPUTexture(device, offscreenTex);
    }
    else
    {
        // Interactive mode
        bool running = true;
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
                {
                    running = false;
                }
            }

            auto* cmdBuf = SDL_AcquireGPUCommandBuffer(device);
            if (!cmdBuf)
            {
                fprintf(stderr, "SDL_AcquireGPUCommandBuffer failed: %s\n", SDL_GetError());
                break;
            }

            SDL_GPUTexture* swapchainTexture = nullptr;
            Uint32 swapW, swapH;
            if (!SDL_AcquireGPUSwapchainTexture(cmdBuf, window, &swapchainTexture, &swapW, &swapH))
            {
                fprintf(stderr, "SDL_AcquireGPUSwapchainTexture failed: %s\n", SDL_GetError());
                break;
            }

            if (swapchainTexture)
            {
                renderFrame(cmdBuf, swapchainTexture);
            }

            SDL_SubmitGPUCommandBuffer(cmdBuf);
        }
    }

    SDL_WaitForGPUIdle(device);
    SDL_ReleaseGPUBuffer(device, vertexBuffer);
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
    SDL_ReleaseWindowFromGPUDevice(device, window);
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
