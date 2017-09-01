#ifndef RWE_MAINMENUSCENE_H
#define RWE_MAINMENUSCENE_H

#include <rwe/SceneManager.h>
#include <rwe/ui/UiPanel.h>
#include <rwe/TextureService.h>
#include <rwe/camera/UiCamera.h>

namespace rwe
{
    class UiPanelScene : public SceneManager::Scene
    {
    private:
        UiPanel panel;
        UiCamera camera;

    public:
        explicit UiPanelScene(UiPanel&& panel);

        void render(GraphicsContext& context) override;

        void onMouseDown(MouseButtonEvent event) override;

        void onMouseUp(MouseButtonEvent event) override;

        void onMouseMove(MouseMoveEvent event) override;
    };
}

#endif
