#pragma once
#include"ModuleBasicDefinition.hpp"

namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{
class DrawRectTransform:public Module
{
private:
public:
    ShaPtr<RectTransform> rect;
    VEC::Color color=VEC::Color(0.8,0.7,0.2);
    float lineWidth=5;
    DrawRectTransform(/* args */)
    {

    }
    ~DrawRectTransform()
    {

    }
    void init()override
    {

    }

    void start()override
    {
        rect=object->get_module<RectTransform>();
    }
    void draw() override
    {
        if (!rect)
            return;
        auto window = get_window();
        auto oldTrasform=window->get_transform();

        LineStyle lineStyle;
        lineStyle.width=lineWidth;
        lineStyle.color=color;

        auto p1 = oldTrasform * VEC::Vec4(-rect->size.width / 2, -rect->size.height / 2, 0, 1);
        auto p2 = oldTrasform * VEC::Vec4(rect->size.width / 2, -rect->size.height / 2, 0, 1);
        auto p3 = oldTrasform * VEC::Vec4(rect->size.width / 2, rect->size.height / 2, 0, 1);
        auto p4 = oldTrasform * VEC::Vec4(-rect->size.width / 2, rect->size.height / 2, 0, 1);

        window->set_transform(VEC::Mat4::unit_matrix());
        window->draw_line(p1.x, p1.y, p2.x, p2.y,&lineStyle);
        window->draw_line(p3.x, p3.y, p2.x,p2.y,&lineStyle);
        window->draw_line(p3.x, p3.y, p4.x, p4.y,&lineStyle);
        window->draw_line(p1.x, p1.y, p4.x, p4.y,&lineStyle);
        //std::cout<<pp1.x<<" "<< pp1.y<<" "<<pp2.x<<" "<<pp2.y<<std::endl;
        window->set_transform(oldTrasform);
    }
};

} // namespace Module

} // namespace Scenes
    
} // namespace OneKeyEngine


