#pragma once
#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../namespaceDefine.hpp"
#include"FunctionCube.hpp"


class ControllerList:public MOD::Module
{
public:
    OKE::ShaPtr<MOD::RectTransform>rectTransform;
    OKE::ShaPtr<MOD::Button> button;
    OKE::ShaPtr<OKE::Scenes::ScenesObject>move;

    int contollerCount=-1;
    OKE::VEC::Vec2 listSize;

    bool wheel=false;
    void init()
    {
        using namespace OKE;

        rectTransform=object->add_module<MOD::RectTransform>();
        // button=object->add_module<MOD::Button>();
        // button->mouseOverCallbackManager.register_event("mouse_in",mouse_in,this);
        // button->mouseOffCallbackManager.register_event("mouse_off",mouse_off,this);

        rectTransform->size=VEC::Vec2(400,1080);
        move=object->scenes->create_object("list",object);
        move->transform.position=VEC::Vec2(-rectTransform->size.width/2+10,rectTransform->size.height/2);
        create_controller();

        
    }
    void update()
    {

        if(move->get_childs_count()!=contollerCount)
        {
            //create_controller();
        }
        if(wheel)
            move->transform.position+=input()->get_wheel()*10;
        
    }
    void create_controller()
    {
        using namespace OKE;
        auto scenes=object->scenes;
        contollerCount = 0;
        listSize = VEC::Vec2(0, 0);

        auto functionButton = scenes->create_object("函数对象",object);
        auto function = functionButton->add_module<FunctionCube>();
        contollerCount++;

        TOOL::Layout layout(scenes,move.get());
        layout.insort_control(functionButton.get());

        layout.use();
    }
    static void mouse_move_callback(ControllerList*cl)
    {

        auto leftBottom=cl->object->transform.position-cl->rectTransform->size/2;
        leftBottom=cl->object->transformMat4*leftBottom;
        
        auto rightUp=cl->object->transform.position+cl->rectTransform->size/2;
        rightUp=cl->object->transformMat4*rightUp;


    }


    static void mouse_in(ControllerList* cl)
    {
        cl->wheel=true;
        std::cout<<"进入"<<std::endl;
    }
    static void mouse_off(ControllerList* cl)
    {
        cl->wheel=false;
        std::cout<<"离开"<<std::endl;
    }
};