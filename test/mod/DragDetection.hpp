#pragma once
#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../namespaceDefine.hpp"


class DragDetection:public MOD::Module
{
private:

public:
    OKE::ShaPtr<MOD::Button>
        dragDetection;
    OKE::ShaPtr<MOD::Canvas>
        canvas;
    bool follow = false;
    OKE::VEC::Vec2 shift;
    void init()
    {
        using namespace OKE;
        auto scenes=object->scenes;
        canvas=scenes->find_object("Canvas")->get_module<MOD::Canvas>();
        auto size=VEC::Vec2(300,100);
        auto rect=object->get_module<MOD::RectTransform>();
        if(rect)
        {
            size=rect->size;
        }

        dragDetection = TOOL::create_button(scenes, "", object, size.width,size.height);

        dragDetection->pressCallbackManager.register_event("seize_on",seize_on,dragDetection.get());
        dragDetection->popUpCallbackManager.register_event("release",release,dragDetection.get()) ;
        dragDetection->userData = this;
    }

    void update()
    {
        if(follow)
        {
            object->transform.position=canvas->screen_to_canvas(get_window()->mousePosition)+shift;
        }
    }

    static void seize_on(MOD::Button* button)
    {
        auto cube=static_cast<DragDetection*>(button->userData);
        cube->follow=true;
        button->object->transform.calculate_absolute_position(OKE::VEC::Vec3(0,0,0));
        auto canvasObj= cube->object->scenes->find_object("Canvas");
        auto canvas=canvasObj->get_module<MOD::Canvas>();
        auto mousePos= canvas->screen_to_canvas(button->get_window()->mousePosition);
        cube->shift=OKE::VEC::Vec2(cube->object->transform.position.x,cube->object->transform.position.y)-mousePos;
    }
    static void release(MOD::Button* button)
    {
        auto cube=static_cast<DragDetection*>(button->userData);
        cube->follow=false;
    }
};


