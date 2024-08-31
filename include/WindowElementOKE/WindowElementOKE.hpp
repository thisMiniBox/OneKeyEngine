#pragma once
#ifndef __OKE_WINDOW_ELEMENT_H
#define __OKE_WINDOW_ELEMENT_H

#include"../WindowDefine/Window_OKE.hpp"
#include<string>

namespace OneKeyEngine
{
namespace WindowControl
{
    class WindowElement;

    class WindowElement
    {
    protected:
        
    public:
        VEC::Transform transform;
        bool exclusiveData=false;
        WindowElement();
        virtual ~WindowElement();
        virtual WindowDrawResult draw(WindowDefine::WindowBase *window) = 0;

        VEC::Transform& get_transform();
        const VEC::Transform& get_transform()const;

        template <typename ChildElement>
        ChildElement *to_child()
        {
            return dynamic_cast<ChildElement *>(this);
        }
    };

    class Lines:public WindowElement
    {
    private:
        VEC::Vec3* m_points=nullptr;
        int m_pointSize=0;
    public:
        Lines(VEC::Vec3*points=nullptr,int size=0,bool exclusive=false);
        ~Lines();

        WindowDrawResult draw(WindowDefine::WindowBase *window)override;

        void set_points(VEC::Vec3*points=nullptr,int size=0);
        VEC::Vec3* get_points()const;
        int get_points_size()const;
    };
}

}

#endif