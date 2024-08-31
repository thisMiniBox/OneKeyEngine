#include <OneKeyEngine/WindowElementOKE/WindowElementOKE.hpp>


namespace OneKeyEngine
{
namespace WindowControl
{
        

    Lines::Lines(VEC::Vec3 *points, int size, bool exclusive) : m_points(points), m_pointSize(size)
    {
        exclusiveData=exclusive;
    }

    Lines::~Lines()
    {
        if(exclusiveData)
            delete[]m_points;
    }
    
    WindowDrawResult Lines::draw(WindowDefine::WindowBase *window)
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

    void Lines::set_points(VEC::Vec3 *points, int size)
    {
        m_points=points;
        m_pointSize=size;
    }

    VEC::Vec3 *Lines::get_points() const
    {
        return m_points;
    }

    int Lines::get_points_size() const
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

    VEC::Transform &WindowElement::get_transform()
    {
        return transform;
    }
    const VEC::Transform &WindowElement::get_transform()const
    {
        return transform;
    }

} // namespace WindowControl
WindowDrawResult WindowDefine::WindowBase::draw(WindowControl::WindowElement* control)
{
    if(control)return control->draw(this);
    return WindowDrawResult::WDR_FALSE;
} 
} // namespace OneKeyEngine
