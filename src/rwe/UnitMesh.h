#ifndef RWE_UNITMESH_H
#define RWE_UNITMESH_H

#include <rwe/GraphicsContext.h>
#include <rwe/math/Vector3f.h>
#include <string>
#include <boost/optional.hpp>


namespace rwe
{
    struct UnitMesh
    {
        struct MoveOperation
        {
            float targetPosition;
            float speed;

            MoveOperation(float targetPosition, float speed);
        };
        struct TurnOperation
        {
            float angle;
            float speed;
        };

        std::string name;
        Vector3f origin;
        std::shared_ptr<ShaderMesh> mesh;
        std::vector<UnitMesh> children;
        bool visible{true};
        Vector3f offset{0.0f, 0.0f, 0.0f};

        boost::optional<MoveOperation> xMoveOperation;
        boost::optional<MoveOperation> yMoveOperation;
        boost::optional<MoveOperation> zMoveOperation;

        void render(
            GraphicsContext& context,
            ShaderProgramIdentifier textureShader,
            ShaderProgramIdentifier colorShader,
            const Matrix4f& modelMatrix,
            const Matrix4f& viewMatrix,
            const Matrix4f& projectionMatrix) const;

        boost::optional<const UnitMesh&> find(const std::string& pieceName) const;

        boost::optional<UnitMesh&> find(const std::string& pieceName);

        void update(float dt);
    };
}

#endif
