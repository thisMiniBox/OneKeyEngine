#ifndef __OKE_WINDOW_ELEMENT_H
#define __OKE_WINDOW_ELEMENT_H

#include"../Style_OKE.hpp"
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



    Lines::Lines(VEC::Vec3 *points, int size, bool exclusive) : m_points(points), m_pointSize(size)
    {
        exclusiveData=exclusive;
    }

    Lines::~Lines()
    {
        if(exclusiveData)
            delete[]m_points;
    }
    
    inline WindowDrawResult Lines::draw(WindowDefine::WindowBase *window)
    {
        if (!window || !m_points || m_pointSize < 2)
            return WindowDrawResult::WDR_PARAMETER_ERROR;
        VEC::Vec3 start = transform.calculate_absolute_position(m_points[0]);
        for (int i = 1; i < m_pointSize; ++i)
        {
            VEC::Vec3 end = transform.calculate_absolute_position(m_points[i]);
            auto result = window->draw_line(start, end); 
            if (result != WindowDrawResult::WDR_SUCCESS)
                return result;
            start = end;
        }
        int childCount=transform.get_childs_count();
        for(int i=0;i<childCount;++i)
        {
            auto result = ((WindowControl::WindowElement*)transform.get_child(i)->userData)->draw(window);
            if (result != WindowDrawResult::WDR_SUCCESS)
                return result;
        }
        return WindowDrawResult::WDR_SUCCESS;
    }

    inline void Lines::set_points(VEC::Vec3 *points, int size)
    {
        m_points=points;
        m_pointSize=size;
    }

    inline VEC::Vec3 *Lines::get_points() const
    {
        return m_points;
    }

    inline int Lines::get_points_size() const
    {
        return m_pointSize;
    }

    WindowElement::WindowElement()
    {
        transform.userData=this;
    }

    WindowElement::~WindowElement()
    {
    }

    inline VEC::Transform &WindowElement::get_transform()
    {
        return transform;
    }
    inline const VEC::Transform &WindowElement::get_transform()const
    {
        return transform;
    }


}
    inline WindowDrawResult WindowDefine::WindowBase::draw(WindowControl::WindowElement* control)
    {
        if(control)return control->draw(this);
        return WindowDrawResult::WDR_FALSE;
    }
}

#endif