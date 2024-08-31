#pragma once
#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../namespaceDefine.hpp"

#include"DragDetection.hpp"


class FunctionCube:public MOD::Module
{
public:
    OKE::ShaPtr<DragDetection>
        dragDetection;
    OKE::ShaPtr<MOD::RectTransform>
        rectTransform;//拖动区域
    OKE::ShaPtr<MOD::Text>
        label;
    OKE::ShaPtr<MOD::EditText>
        functionNameEdit,//编辑函数名
        functionParameter;//编辑参数名
    OKE::ShaPtr<MOD::Image>
        arrow;

    void init()
    {  
        using namespace OKE;
        auto scenes=object->scenes;
        rectTransform=object->get_module<MOD::RectTransform>();
        if(!rectTransform)
        {
            rectTransform=object->add_module<MOD::RectTransform>();
        }
        rectTransform->size=VEC::Vec2(300,100);

        dragDetection=object->add_module<DragDetection>();
        auto color = VEC::Color(1.0, 0.24705882352941178, 0.37254901960784315);
        dragDetection->dragDetection->defaultColor = color;
        dragDetection->dragDetection->pressColor = color * 1.5;
        dragDetection->dragDetection->selectColor = color;
        dragDetection->dragDetection->highlightColor = color;

        label = TOOL::create_text(scenes, "函数名         参数表", object);
        label->rectTransform.lock()->size = rectTransform->size;
        //label->object->add_module<MOD::DrawRectTransform>();
        label->object->transform.position = VEC::Vec3(5, 25);
        label->textStyle.color=VEC::Color(1,1,1);
        label->textStyle.font.fontPath=R"(C:\Users\Lenovo\AppData\Local\Microsoft\Windows\Fonts\SmileySans-Oblique.otf)";

        functionNameEdit=TOOL::create_edit(scenes,"MyFunction",object,95,45);
        functionNameEdit->object->transform.position=VEC::Vec2(-97.5,-22.5);

        functionParameter=TOOL::create_edit(scenes,"Parameter",object,190,45);
        functionParameter->object->transform.position=VEC::Vec2(50,-22.5);

        arrow=TOOL::create_image(scenes,object,10,110,color);
        auto arrowImage=arrow->currentImage;
        auto arrowSize=arrowImage->image_size();
        for(int x=0;x<arrowSize.width;x++)
        {
            for(int y=0;y<arrowSize.height;y++)
            {
                if(y-100>x)
                    arrowImage->set_pixel(x,y,VEC::Color(0,0,0,0));
            }
        }
        arrow->object->transform.position=VEC::Vec3(-(rectTransform->size.width/2)-5,-5);
    }
    void start()
    {
        using namespace OKE;

    }

    void update()
    {
        using namespace OKE;
    
    }

};

