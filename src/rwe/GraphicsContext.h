#ifndef RWE_GRAPHICSCONTEXT_H
#define RWE_GRAPHICSCONTEXT_H

#include "GlMesh.h"
#include "SelectionMesh.h"
#include "ShaderHandle.h"
#include "ShaderMesh.h"
#include "ShaderProgramHandle.h"
#include <GL/glew.h>
#include <SDL.h>
#include <memory>
#include <rwe/ColorPalette.h>
#include <rwe/MapFeature.h>
#include <rwe/MapTerrain.h>
#include <rwe/Mesh.h>
#include <rwe/Sprite.h>
#include <rwe/SpriteSeries.h>
#include <rwe/TextureHandle.h>
#include <rwe/camera/AbstractCamera.h>
#include <rwe/geometry/CollisionMesh.h>
#include <rwe/math/Vector3f.h>

namespace rwe
{
#pragma pack(1)
    struct GlTexturedVertex
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat u;
        GLfloat v;

        GlTexturedVertex() = default;
        GlTexturedVertex(const Vector3f& pos, const Vector2f& texCoord);
    };

    struct GlColoredVertex
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat r;
        GLfloat g;
        GLfloat b;

        GlColoredVertex() = default;
        GlColoredVertex(const Vector3f& pos, const Vector3f& color);
    };
#pragma pack()

    struct AttribMapping
    {
        std::string name;
        GLuint location;

        AttribMapping(const std::string& name, GLuint location);
    };

    class GraphicsException : public std::runtime_error
    {
    public:
        explicit GraphicsException(const std::string& __arg);

        explicit GraphicsException(const char* string);
    };

    class OpenGlException : public GraphicsException
    {
    public:
        explicit OpenGlException(GLenum error);
    };

    class GraphicsContext
    {
    public:
        void clear();

        TextureHandle createTexture(const Grid<Color>& image);

        TextureHandle createTexture(unsigned int width, unsigned int height, const std::vector<Color>& image);

        TextureHandle createTexture(unsigned int width, unsigned int height, const Color* image);

        TextureHandle createColorTexture(Color c);

        /**
         * Multiplies the current OpenGL matrix with the specified matrix.
         * If the current OpenGL matrix is C and the coordinates to be transformed are v,
         * meaning that the current transformation would be C * v,
         * then calling this with an argument M replaces the current transformation with (C * M) * v.
         * Intuitively this means that any transformation you pass in here
         * will be done directly on the coordinates,
         * and the existing tranformation will be done on the result of that.
         */
        void multiplyMatrix(const Matrix4f& m);

        void pushMatrix();

        void popMatrix();

        void drawShaderMesh(
            const ShaderMesh& mesh,
            ShaderProgramIdentifier textureShader,
            ShaderProgramIdentifier colorShader,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            float seaLevel);

        void drawWireframeSelectionMesh(
            const GlMesh& mesh,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            ShaderProgramIdentifier shader);

        void enableDepth();

        void enableDepthWrites();

        void disableDepthWrites();

        void enableCulling();

        void disableDepth();

        ShaderHandle compileVertexShader(const std::string& source);

        ShaderHandle compileFragmentShader(const std::string& source);

        ShaderProgramHandle linkShaderProgram(ShaderIdentifier vertexShader, ShaderIdentifier fragmentShader, const std::vector<AttribMapping>& attribs);

        void beginUnitShadow();

        void endUnitShadow();

        GlMesh createTexturedMesh(const std::vector<GlTexturedVertex>& vertices, GLenum usage);

        GlMesh createColoredMesh(const std::vector<GlColoredVertex>& vertices, GLenum usage);

        void drawLinesMesh(
            const GlMesh& mesh,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            ShaderProgramIdentifier shader);

        void drawTrisMesh(
            const GlMesh& mesh,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            ShaderProgramIdentifier shader);

        void drawSprite(
            const GlMesh& mesh,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            float alpha,
            ShaderProgramIdentifier shader);

    private:
        ShaderHandle compileShader(GLenum shaderType, const std::string& source);

        VboHandle genBuffer();
        VaoHandle genVertexArray();
        void bindBuffer(GLenum type, VboIdentifier id);
        void unbindBuffer(GLenum type);
        void bindVertexArray(VaoIdentifier id);
        void unbindVertexArray();
        void drawMesh(
            GLenum mode,
            const GlMesh& mesh,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            ShaderProgramIdentifier shader);
        void drawUnitMesh(
            const GlMesh& mesh,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix,
            float seaLevel,
            ShaderProgramIdentifier shader);
    };
}

#endif
