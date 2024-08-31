#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../NameSpaceDefine.hpp"
#include<iostream>

class CreateProject:public OneKeyEngine::Scenes::Scenes
{
public:
    static void buttonCallback(CreateProject* scenes)
    {
        scenes->manage->to_scenes("编辑场景");
        return;
    }
    void init()override
    {
        auto cameraObj=create_object("摄像机");
        auto camera=cameraObj->add_module<MOD::CameraModule>();

        auto canvasObj=create_object("画布");
        auto canvas= canvasObj->add_module<MOD::Canvas>();

        auto eventObj=create_object("事件处理器");
        auto eventsSystem=eventObj->add_module<MOD::UIEventsSystem>();
        eventsSystem->canvas=canvas;
        

        canvas->outPutCamera=mainCamera;
        canvas->projectorMode=OKE::Scenes::Module::ProjectorMode::screen;
        canvas->plotMode=MOD::PlotMode::fixedPixelSize;

        auto createProjectButton= TOOL::create_button(this,"创建项目",canvasObj.get(),200,100,30);
        createProjectButton->pressCallbackManager.register_event_f("buttonCallback",std::function(buttonCallback),this);
        createProjectButton->object->add_module<MOD::DrawRectTransform>();
    }
};
