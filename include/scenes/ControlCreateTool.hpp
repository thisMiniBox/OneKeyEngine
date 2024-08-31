#pragma once
#include"ModuleBasicDefinition.hpp"

namespace OneKeyEngine
{
namespace CreateControl
{

static ShaPtr<Scenes::Module::Canvas> init_project_2d(Scenes::Scenes *scenes)
{
    if (!scenes)
        return nullptr;
    using namespace Scenes;
    if (!scenes->mainCamera)
    {
        auto cameraObj = scenes->create_object("Camera");
        cameraObj->add_module<Module::CameraModule>();
    }
    auto canvasObj = scenes->create_object("Canvas");
    auto canvasSize = canvasObj->add_module<Module::RectTransform>();
    canvasSize->size = VEC::Vec2(1280, 720);
    
    auto canvas = canvasObj->add_module<Module::Canvas>();
    canvas->outPutCamera=scenes->mainCamera;
    canvas->plotMode=Module::PlotMode::fixedPixelSize;

    auto eventObj = scenes->create_object("EventSystem");
    auto eventsSystem = eventObj->add_module<Module::UIEventsSystem>();
    eventsSystem->canvas = canvas;
    return canvas;
}

/// @brief 创建一个包含rectTransform和指定模板的对象
/// @tparam T 
/// @param scenes 场景
/// @param parent 父对象
/// @param width 宽度
/// @param height 高度
/// @return 
template <typename T>
ShaPtr<T> create_control(Scenes::Scenes *scenes, Scenes::ScenesObject *parent, int width, int height)
{
    auto obj = scenes->create_object(typeid(T).name()); // 根据组件类型动态创建对象
    if (parent)
        obj->transform.set_parent(parent->transform);
    auto rect=obj->get_module<Scenes::Module::RectTransform>();
    if(!rect)
        rect = obj->add_module<Scenes::Module::RectTransform>();
    rect->size = VEC::Vec2(width, height);
    return obj->add_module<T>();
}


static ShaPtr<Scenes::Module::Image> create_image(Scenes::Scenes *scenes, Scenes::ScenesObject *parent = nullptr, int width = 100, int height = 100,VEC::Color color=VEC::Color(1,1,1))
{
    auto image = create_control<Scenes::Module::Image>(scenes, parent, width, height);
    image->create_image(width,height,color);
    return image;
}

static ShaPtr<Scenes::Module::Text> create_text(Scenes::Scenes *scenes, const std::string &text, Scenes::ScenesObject *parent = nullptr, int fontSize = 24)
{
    auto textObj = create_control<Scenes::Module::Text>(scenes, parent, text.size() * fontSize, 50);
    if (textObj)
    {
        auto textComponent = textObj->object->get_module<Scenes::Module::Text>();
        textComponent->showText = text;
        textComponent->textStyle.verticalAlign = TextAlign::Center;
        textComponent->textStyle.horizontalAlign = TextAlign::Left;
        textComponent->textStyle.font.size = fontSize;
    }
    return textObj;
}
static ShaPtr<Scenes::Module::Button> create_button(
    Scenes::Scenes *scenes, const std::string &text, Scenes::ScenesObject *parent = nullptr, 
    int width = 100, int height = 50, int textSize = 18,
    Scenes::Module::Button::ButtonCallback buttonPressCallback=nullptr)
{
    
    auto button = create_control<Scenes::Module::Button>(scenes, parent, width, height);
    if (button)
    {
        auto bk= button->object->add_module<Scenes::Module::Image>();
        bk->create_image(1,1,VEC::Color(1.0f,1.0f,1.0f));
        button->drawAim=bk;
        auto textObj = create_text(scenes, text, button->object);
        if (textObj)
        {
            auto rect = textObj->object->get_module<Scenes::Module::RectTransform>();
            rect->size = VEC::Vec2(width, height);
            auto textComponent = textObj->object->get_module<Scenes::Module::Text>();
            textComponent->textStyle.verticalAlign = TextAlign::Center;
            textComponent->textStyle.horizontalAlign = TextAlign::Center;
            textComponent->textStyle.font.size = textSize;
        }
        if(buttonPressCallback)
            button->pressCallbackManager.register_event_f("buttonPressCallback",std::function(buttonPressCallback),button.get());
    }
    return button;
}

static ShaPtr<Scenes::Module::EditText> create_edit(Scenes::Scenes *scenes, const std::string &defaultText, Scenes::ScenesObject *parent = nullptr, int width = 200, int height = 70)
{
    using namespace Scenes::Module;
    auto editObj = scenes->create_object("EditText");
    if (parent)
        editObj->transform.set_parent(parent->transform);
    auto rect = editObj->add_module<RectTransform>();
    rect->size = VEC::Vec2(width, height);
    auto image = editObj->add_module<Image>();
    image->create_image(1, 1, VEC::Color(1,1,1));
    auto edit = editObj->add_module<EditText>();
    auto text = editObj->add_module<Text>();
    edit->rectTransform = rect;
    edit->drawAim = image;
    edit->text = text;
    text->showText = defaultText;
    text->textStyle.verticalAlign = TextAlign::Top;
    text->textStyle.horizontalAlign = TextAlign::Left;
    return edit;
}

enum class LayoutStyle
{
    //横向布局
    transverse,
    //纵向布局
    portrait,
};
enum class LayoutAlignmentMethod
{
    LeftTop,
    LeftCenter,
    LeftBottom,
    CenterTop,
    CenterCenter,
    CenterBottom,
    RightTop,
    RightCenter,
    RightBottom,
};
class Layout
{
    VEC::Vec2 controlSizeMax=VEC::Vec2(0,0);
    VEC::Vec2 layoutSize=VEC::Vec2(0,0);
public:
    LayoutStyle layoutStyle=LayoutStyle::portrait;
    LayoutAlignmentMethod alignmentMethod=LayoutAlignmentMethod::LeftTop;
    Scenes::Scenes* scenes=nullptr;
    Scenes::ScenesObject* parent=nullptr;
    ShaPtr<Scenes::ScenesObject> layoutObj=nullptr;
    ShaPtr<Scenes::Module::RectTransform> layoutObjRect;
    VEC::Vec2 position=VEC::Vec2(0,0);
    float controlInterval=10;
    Layout(Scenes::Scenes* scenesC,Scenes::ScenesObject* parentC=nullptr);
    /// @brief 插入对象到布局
    /// @param obj 必须是含有rectTransform的对象
    /// @return 
    bool insort_control(Scenes::ScenesObject* obj);

    ShaPtr<Scenes::ScenesObject> use();

    /// @brief 按照布局移动锚点
    /// @param point 要移动的锚点
    /// @param objSize 已使用的控件大小
    void move_anchor_point(VEC::Vec2 *point, const VEC::Vec2 &objSize);
    /// @brief 根据当前布局和对齐锚点计算实际控件位置
    /// @param point 当前锚点
    /// @param objSize 当前要绘制的控件大小
    /// @return 控件实际绘制位置
    VEC::Vec2 get_control_pos(const VEC::Vec2 &point, const VEC::Vec2 &objSize);

    VEC::Vec2 get_start_anchor_point() const;
};

inline void Layout::move_anchor_point(VEC::Vec2 *point, const VEC::Vec2 &objSize)
{
    switch (layoutStyle)
    {
    case LayoutStyle::portrait:
        switch (alignmentMethod)
        {
        case LayoutAlignmentMethod::CenterBottom:
        case LayoutAlignmentMethod::LeftBottom:
        case LayoutAlignmentMethod::RightBottom:
            point->y += objSize.height + controlInterval;
            break;

        default:
            point->y -= objSize.height + controlInterval;
            break;
        }
        break;
    case LayoutStyle::transverse:
        switch (alignmentMethod)
        {
        case LayoutAlignmentMethod::RightBottom:
        case LayoutAlignmentMethod::RightTop:
        case LayoutAlignmentMethod::RightCenter:
            point->x -= objSize.width + controlInterval;
            break;

        default:
            point->x += objSize.width + controlInterval;
            break;
        }
        break;
    default:
        point->y -= objSize.height + controlInterval;
        break;
    }
}

inline VEC::Vec2 Layout::get_control_pos(const VEC::Vec2 &point, const VEC::Vec2 &objSize)
{
    switch (alignmentMethod)
    {
    case LayoutAlignmentMethod::LeftTop:
        return VEC::Vec2(point.x + objSize.width / 2, point.y - objSize.height / 2);
        break;
    case LayoutAlignmentMethod::LeftCenter:
        return VEC::Vec2(point.x + objSize.width / 2, point.y- objSize.height / 2);
        break;
    case LayoutAlignmentMethod::LeftBottom:
        return VEC::Vec2(point.x + objSize.width / 2, point.y + objSize.height / 2);
        break;
    case LayoutAlignmentMethod::RightTop:
        return VEC::Vec2(point.x - objSize.width / 2, point.y - objSize.height / 2);
        break;
    case LayoutAlignmentMethod::RightCenter:
        return VEC::Vec2(point.x+ layoutObjRect->size.width / 2-this->controlInterval, point.y - objSize.height / 2);
        break;
    case LayoutAlignmentMethod::RightBottom:
        return VEC::Vec2(point.x - objSize.width / 2, point.y + objSize.height / 2);
        break;
    case LayoutAlignmentMethod::CenterTop:
        return VEC::Vec2(point.x+ layoutObjRect->size.width / 2-this->controlInterval, point.y - objSize.height / 2);
        break;
    case LayoutAlignmentMethod::CenterBottom:
        return VEC::Vec2(point.x, point.y + objSize.height / 2);
        break;
    default:
        return point;
        break;
    }
}

inline VEC::Vec2 Layout::get_start_anchor_point() const
{
    auto &size = layoutSize;

    switch (alignmentMethod)
    {
    //更改为靠近0点左对齐
    case LayoutAlignmentMethod::LeftTop:
        return position+VEC::Vec2(controlInterval,-controlInterval);
    case LayoutAlignmentMethod::LeftCenter:

    case LayoutAlignmentMethod::CenterTop:
        return position+VEC::Vec2(controlInterval - layoutObjRect->size.width / 2, size.height / 2 - controlInterval);
        break;
    case LayoutAlignmentMethod::RightTop:
    case LayoutAlignmentMethod::RightCenter:
        return position+VEC::Vec2(-controlInterval + layoutObjRect->size.width / 2, size.height/2-controlInterval);
        break;
    case LayoutAlignmentMethod::RightBottom:
        return position+VEC::Vec2(-controlInterval + layoutObjRect->size.width / 2, -size.height / 2 + controlInterval);
        break;
    case LayoutAlignmentMethod::LeftBottom:
        return position+VEC::Vec2(controlInterval - layoutObjRect->size.width / 2, -size.height / 2 + controlInterval);
        break;
    case LayoutAlignmentMethod::CenterBottom:
        return position+VEC::Vec2(controlInterval - layoutObjRect->size.width / 2, -size.height / 2 + controlInterval);
        break;

    default:
        break;
    }
    return position+VEC::Vec2(0, size.height / 2 - controlInterval);
}

inline Layout::Layout(Scenes::Scenes *scenesC, Scenes::ScenesObject *parentC) : scenes(scenesC), parent(parentC)
{
    layoutObj=scenesC->create_object("Layout");
    if(parentC)
        layoutObj->transform.set_parent(parentC->transform);
    layoutObjRect=layoutObj->add_module<Scenes::Module::RectTransform>();
}

inline bool Layout::insort_control(Scenes::ScenesObject* obj)
{
    if (!layoutObj || !obj)
        return false;
    auto object=obj;
    auto rect=object->get_module<Scenes::Module::RectTransform>();
    if(!rect)
        return false;
    layoutObj->transform.set_child(object->transform);
    controlSizeMax.width=std::max(controlSizeMax.width,rect->size.width);
    controlSizeMax.height=std::max(controlSizeMax.height,rect->size.height);
    layoutSize+=rect->size+VEC::Vec2(controlInterval,controlInterval);
    return true;
}
inline ShaPtr<Scenes::ScenesObject> Layout::use()
{
    layoutSize += VEC::Vec2(controlInterval, controlInterval);
    switch (layoutStyle)
    {
    case LayoutStyle::portrait:
        layoutObjRect->size = VEC::Vec2(controlSizeMax.width + 2 * controlInterval, layoutSize.height);
        break;
    case LayoutStyle::transverse:
        layoutObjRect->size = VEC::Vec2(layoutSize.width, controlSizeMax.height + 2 * controlInterval);
        break;
    default:
        break;
    }
    VEC::Vec2 anchorPoint=get_start_anchor_point();

    //std::cout<<"布局锚点："<<anchorPoint.x<<" "<<anchorPoint.y<<std::endl;
    VEC::Vec2 lastPos=anchorPoint;

    for (int i = 0; i < layoutObj->transform.get_childs_count(); ++i)
    {
        auto childObj=(Scenes::ScenesObject*)layoutObj->transform.get_child(i)->userData;
        auto rect=childObj->get_module<Scenes::Module::RectTransform>();
        childObj->transform.position=get_control_pos(anchorPoint,rect->size);
        move_anchor_point(&anchorPoint,rect->size);

        auto move=anchorPoint-lastPos;
        // std::cout<<"控件大小："<<rect->size.x<<" "<<rect->size.y<<std::endl;
        // std::cout<<"控件位置："<<childObj->transform.position.x<<" "<<childObj->transform.position.y<<std::endl;
        lastPos=anchorPoint;
    }
    return layoutObj;
}
}
}