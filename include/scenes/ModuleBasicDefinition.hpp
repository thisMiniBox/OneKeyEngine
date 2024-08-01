#pragma once

#include"../WindowElementOKE/ImageLoadOKE.hpp"

#include"ScenesObject.hpp"
#include"Module.hpp"

#include<memory>
#include<unordered_map>


namespace OneKeyEngine
{
namespace Scenes
{
namespace Module
{

class RectTransform:public Module
{
public:
    VEC::Vec2 size;
    VEC::Transform* transform=nullptr;
    RectTransform(float w=100,float h=100);

    void init()override;

    void draw()override;



    static bool is_point_in_triangle(const VEC::Vec2& p, const VEC::Vec2& a, const VEC::Vec2& b, const VEC::Vec2& c);
};


enum class PlotMode
{
    screenScaling,
    fixedPixelSize,
};
enum class ProjectorMode
{
    screen,
    camera,
    world,
};
class Button;
class CameraModule;
class Canvas : public Module
{
public:
    WeaPtr<RectTransform> rectTransform;
    WeaPtr<Camera> outPutCamera;
    ProjectorMode projectorMode=ProjectorMode::screen;
    PlotMode plotMode=PlotMode::screenScaling;
    /// @brief 缩放匹配0：宽度，1：高度
    float matchWidthOrHeight=0;

    Canvas();

    void start()override;
    void draw()override;
};

class Image : public Module
{
public:
    GL::ShaderGL* shader=nullptr;
    GL::Element* vertexBuffer=nullptr;
    std::shared_ptr<WindowControl::Picture> currentImage;
    WeaPtr<RectTransform> rectTransform;

    VEC::Color color=VEC::Color(1,1,1);
    Image(const std::string &filePath = "");
    ~Image();
    std::shared_ptr<WindowControl::Picture> &load_image_file(const std::string &filePath);
    std::shared_ptr<WindowControl::Picture> create_image(int width,int height,const VEC::Color& fillColor=VEC::Color(1.0f,1.0f,1.0f));
    void init()override;

    virtual void draw() override;

};

class Text:public Module
{
    VEC::Vec2 textDrawSize;
    WeaPtr<RectTransform> rectTransform;
    GL::ShaderGL* shader=nullptr;
    GL::Element* vertexBuffer=nullptr;
public:
    TextStyle textStyle;
    std::string showText;
    Text();
    
    void init()override;
    void draw()override;
    float get_str_line_size(char* str)const;
    /// @brief 获取字符在绘图情况下的像素位置
    /// @param pos utf-8情况下的第几个字符
    /// @return 在绘图情况下的位置
    VEC::Vec2 get_character_position(size_t char_index)const;
    size_t get_character_index_from_position(const VEC::Vec2& position) const;
    size_t get_character_count_utf_8()const;
    size_t get_character_pos_in_string(size_t char_index)const;
    void delete_prev_utf8_char(size_t char_index);
    static void delete_prev_utf8_char(std::string& str, size_t char_index);
    // 判断一个字节是否是 UTF-8 字符的起始字节
    static bool is_utf8_start_byte(unsigned char byte);
    // 获取 UTF-8 编码字符的位置
    static size_t utf8_char_position(const std::string& str, size_t char_index);
};

/// @brief 摄像机模块，默认朝向为z的负半轴
class CameraModule : public Module
{
public:
    ShaPtr<Camera> camera=std::make_shared<Camera>();
    CameraModule();

    void transform_to_camera_orientation();

    void init()override;
    void draw()override;
    void size(int w,int h)override;
    Camera* operator->();
};

class UIEventsSystem;
class Button : public Module
{
    void on_press();
    void on_popUp();
    void on_mouse_over();
    void on_mouse_off();
    void on_lose_focus();
    bool focus=false;
public:
    using ButtonCallback=void(*)(Button* button);
    ButtonCallback pressCallback=nullptr;
    ButtonCallback popUpCallback = nullptr;
    ButtonCallback mouseOverCallback = nullptr;
    ButtonCallback mouseOffCallback = nullptr;
    ButtonCallback loseFocusCallback=nullptr;

    WeaPtr<RectTransform> rectTransform;
    WeaPtr<Image> drawAim;
    WeaPtr<UIEventsSystem> eventsSystem;

    VEC::Color defaultColor = VEC::Color(1, 1, 1);
    VEC::Color highlightColor = VEC::Color(1.2, 1.2, 1.2);
    VEC::Color pressColor = VEC::Color(0.7, 0.7, 0.7);
    VEC::Color selectColor = VEC::Color(1, 1, 1);


    Button();
    ~Button();

    void init()override;
    //void draw()override;
    

    virtual void press(){};
    virtual void popUp(){};
    virtual void mouse_over(){};
    virtual void mouse_off(){};
    virtual void lose_focus(){};

    friend class UIEventsSystem;
};

class UIEventsSystem : public Module
{
    static void key_down_callback(WindowDefine::WindowBase* window,KeyCode key);
    static void key_up_callback(WindowDefine::WindowBase* window,KeyCode key);
    static void mouse_move_callback(WindowDefine::WindowBase* window,float x,float y);

    WeaPtr<Button> lastFocus;
    WeaPtr<Button> lastMouseOver;
public:
    WeaPtr<Canvas> canvas;
    std::vector<WeaPtr<Button>> buttonSet;
    bool needUpdateButtonSet = false;

    UIEventsSystem();

    void init()override;
    void start()override;
    void update()override;
    void exit()override;

    void update_button_set();
    void click(int x,int y);
    void mouse_move(int x,int y);
    WeaPtr<Button> get_button(int x,int y);
};

class EditText:public Button
{
    size_t charSize=0;
    float timer=0.0f;
    VEC::Color oldPressColor;
    VEC::Vec2 cursorDrawPos;
    //是否在拖动鼠标选词
    bool selectedWords=false;
    //是否处于框选
    bool boxSelect=false;
    //static void mouse_move_callback(WindowDefine::WindowBase* window,float x,float y);
public:
    //光标显示隐藏的间隔时间，单位秒，小于0永不闪烁
    float cursorBlinking=0.5f;
    //是否绘制光标
    bool drawCursor=true;
    //光标线条样式
    LineStyle cursorStyle;
    //当前光标的位置,utf-8字符
    size_t cursorPosition=0;
    //绘制文本
    WeaPtr<Text> text;
    //是否捕获输入
    bool getInput=false;
    //允许编辑
    bool editAble=true;
    //框选位置
    size_t boxSelectNext=0;
    //框选颜色
    VEC::Color boxSelectColor=VEC::Color(0.2f,0.7f,0.9f);

    EditText();
    void start()override;

    void update()override;

    void draw()override;
    
    void press()override;
    void lose_focus()override;

    void delete_last_char();

    void update_cursor_draw();

    void insort_text_in_cursor(const std::string& str);

    void erase_utf_8_character(size_t start,size_t size);

    std::string get_box_select_string()const;
};







/******************************函数定义*****************************/

inline void EditText::delete_last_char()
{
    if (text.expired())
        return;
    auto &str = text.lock()->showText;
    if (str.empty())
        return;

    size_t lastCharStart = str.size() - 1;
    while (lastCharStart > 0 && (str[lastCharStart] & 0xC0) == 0x80)
        --lastCharStart;

    str.erase(lastCharStart);
}

inline void EditText::update_cursor_draw()
{
    auto t = text.lock();
    if (!t)
        return;
    cursorDrawPos = t->get_character_position(this->cursorPosition);
}

inline void EditText::insort_text_in_cursor(const std::string&str)
{
    auto t = text.lock();
    if (!t || str.empty())
        return;
    auto strData = const_cast<char *>(str.data());
    while (1)
    {
        auto ch = utf8_decode(strData, &strData);
        if(ch<32)
        {
            if (ch == '\0')
                break;
            if(ch=='\b')
            {
                if (boxSelect && cursorPosition != boxSelectNext)
                {
                    auto left = std::min(cursorPosition, boxSelectNext);
                    auto right = std::max(cursorPosition, boxSelectNext);
                    erase_utf_8_character(left, right - left);
                    cursorPosition=left;
                    boxSelectNext=cursorPosition;
                }
                else
                {
                    t->delete_prev_utf8_char(cursorPosition);
                    if (cursorPosition > 0)
                        cursorPosition--;
                }
                update_cursor_draw();
            }
            continue;
        }
        else
        {
            auto s = utf8_encode(ch);
            auto p = t->get_character_pos_in_string(cursorPosition);
            t->showText.insert(p, s);
            cursorPosition++;
        }
        update_cursor_draw();
    }
    charSize = t->get_character_count_utf_8();
}

inline void EditText::erase_utf_8_character(size_t start, size_t size)
{
    auto t = text.lock();
    if (!t)
        return;
    auto left = start;
    auto right = start + size;
    left = t->get_character_pos_in_string(left);
    right = t->get_character_pos_in_string(right);
    t->showText.erase(left, right - left);
}

inline std::string EditText::get_box_select_string() const
{
    if (!boxSelect||cursorPosition==boxSelectNext)
        return "";
    auto t=text.lock();
    if(!t)
    return "";

    auto left = std::min(cursorPosition, boxSelectNext);
    auto right = std::max(cursorPosition, boxSelectNext);
    left=t->get_character_pos_in_string(left);
    right=t->get_character_pos_in_string(right);
    return t->showText.substr(left,right-left);
}

inline EditText::EditText()
{
    selectColor=VEC::Color(1.2f,1.2f,1.2f);
}

inline void EditText::start()
{
    auto bk=drawAim.lock();
    if(!bk)return;
    cursorStyle.color=-bk->currentImage->get_color(0,0);
}

inline void EditText::update()
{
    if (!getInput)
    {
        return;
    }
    auto aim = text.lock();
    if (!aim)
        return;
    
    auto window=get_window();

    timer += get_delta_time();
    if (timer >= cursorBlinking)
    {
        drawCursor = !drawCursor;
        timer -= cursorBlinking;
    }
    //按键处理

    if (input()->get_key_down(KeyCode::Left))
    {
        if(input()->get_key(KeyCode::Shift))
        {
            if(!boxSelect)
            {
                boxSelectNext=cursorPosition;
            }
            boxSelect=true;
        }
        else
        {
            boxSelect=false;
        }
        if (cursorPosition > 0)
            cursorPosition--;
        update_cursor_draw();
        // cursorPosition=std::max(0ull,cursorPosition-1);
        // auto p = aim->get_character_pos_in_string(cursorPosition);
        // std::cout<<cursorPosition<<" "<<p<<std::endl;
    }
    if (input()->get_key_down(KeyCode::Right))
    {
        if(input()->get_key(KeyCode::Shift))
        {
            if(!boxSelect)
            {
                boxSelectNext=cursorPosition;
            }
            boxSelect=true;
        }
        else
        {
            boxSelect=false;
        }
        cursorPosition = std::min(charSize, cursorPosition + 1);
        update_cursor_draw();
        // auto p = aim->get_character_pos_in_string(cursorPosition);
        //  std::cout<<cursorPosition<<" "<<p<<std::endl;
    }
    if (input()->get_key_down(KeyCode::Up))
    {
        if(input()->get_key(KeyCode::Shift))
        {
            if(!boxSelect)
            {
                boxSelectNext=cursorPosition;
            }
            boxSelect=true;
        }
        else
        {
            boxSelect=false;
        }
        cursorPosition = aim->get_character_index_from_position(cursorDrawPos + VEC::Vec2(0, aim->textStyle.font.size));
        update_cursor_draw();
    }
    if (input()->get_key_down(KeyCode::Down))
    {
        if(input()->get_key(KeyCode::Shift))
        {
            if(!boxSelect)
            {
                boxSelectNext=cursorPosition;
            }
            boxSelect=true;
        }
        else
        {
            boxSelect=false;
        }

        cursorPosition = aim->get_character_index_from_position(cursorDrawPos - VEC::Vec2(0, aim->textStyle.font.size));
        update_cursor_draw();
    }
    if(input()->get_key(KeyCode::CTRL))
    {
        if(input()->get_key_down(KeyCode::V))
        {
            if(boxSelect&&cursorPosition!=boxSelectNext)
            {
                auto left = std::min(cursorPosition, boxSelectNext);
                auto right = std::max(cursorPosition, boxSelectNext);
                erase_utf_8_character(left,right-left);
            }
            auto copy= window->get_clipboard_text();
            insort_text_in_cursor(copy);
        }
        if(input()->get_key_down(KeyCode::C))
        {
            auto selectedStr=get_box_select_string();
            if(!selectedStr.empty())
            {
                window->set_clipboard_text(selectedStr);
            }
        }
        if(input()->get_key_down(KeyCode::X))
        {
            auto selectedStr=get_box_select_string();
            if(!selectedStr.empty())
            {
                window->set_clipboard_text(selectedStr);
            }
            insort_text_in_cursor("\b");
        }
        if(input()->get_key_down(KeyCode::A))
        {
            boxSelect=true;
            cursorPosition=0;
            boxSelectNext=aim->get_character_count_utf_8();
            update_cursor_draw();
        }
    }
    if (input()->get_key_down(KeyCode::MouseLeftButton))
    {
        selectedWords = true;
        boxSelect=true;
    }
    if (input()->get_key_up(KeyCode::MouseLeftButton))
    {
        selectedWords = false;
        if(boxSelectNext==cursorPosition)
        {
            boxSelect=false;
        }
    }
    if(selectedWords)
    {
        auto mousePos=window->NDC_to_pixel_CC(object->get_relative_mouse_position_NDC());
        boxSelectNext=aim->get_character_index_from_position(VEC::Vec2(mousePos.x,mousePos.y));
    }
    auto str = input()->get_char_input();
    insort_text_in_cursor(str);
}

inline void EditText::draw()
{
    if (!getInput)
        return;

    auto window = get_window();
    auto t = text.lock();
    if (!t)
        return;

    if (boxSelect)
    {
        auto startPos = cursorDrawPos;
        auto endPos = t->get_character_position(boxSelectNext);
        
        // 保证 startPos.y >= endPos.y
        if (startPos.y < endPos.y)
            std::swap(startPos, endPos);

        LineStyle lineStyle;
        lineStyle.color = boxSelectColor;
        lineStyle.width = t->textStyle.font.size * window->get_size().height / 2;
        auto rect = rectTransform.lock();
        
        int lineHeight = t->textStyle.font.size;

        for (int y = startPos.y; y >= endPos.y; y -= lineHeight)
        {
            int startX = y == startPos.y ? startPos.x : -rect->size.x / 2;
            int endX = y == endPos.y ? endPos.x : rect->size.x / 2;
            
            window->draw_line(startX, y + lineHeight / 2, endX, y + lineHeight / 2, &lineStyle);
        }
    }

    if (drawCursor)
    {
        auto pos = cursorDrawPos;
        window->draw_line(pos.x, pos.y, pos.x, pos.y + t->textStyle.font.size, &cursorStyle);
    }
}

void EditText::press()
{
    if(!editAble)
        return;
    if(!getInput)
    {
        oldPressColor=pressColor;
    }

    getInput=true;
    input()->get_char_input();
    auto tp=text.lock();
    charSize=tp->get_character_count_utf_8();

    auto window=get_window();
    auto mousePos =object->get_relative_mouse_position_NDC();
    auto xd = window->NDC_to_pixel_CC(mousePos );
    cursorPosition = tp->get_character_index_from_position(VEC::Vec2(xd.x,xd.y));
    update_cursor_draw();
    //std::cout << "cursorPosition:" << cursorPosition << std::endl;
    //std::cout << "xd:" << xd.x << " " << xd.y << std::endl;

    timer=0;

    pressColor=selectColor;
}

inline void EditText::lose_focus()
{
    getInput=false;
    selectedWords=false;
    pressColor=oldPressColor;
}

inline void UIEventsSystem::key_down_callback(WindowDefine::WindowBase *window, KeyCode key)
{
    auto currentSystem=static_cast<UIEventsSystem*>(static_cast<ScenesManage*>(window->userData)->userDataMore);
    if(!currentSystem)
        return;
    if(key==KeyCode::MouseLeftButton)
    {
        currentSystem->click(window->mousePosition.x,window->mousePosition.y);
    }
}

inline void UIEventsSystem::key_up_callback(WindowDefine::WindowBase *window, KeyCode key)
{
    auto currentSystem=static_cast<UIEventsSystem*>(static_cast<ScenesManage*>(window->userData)->userDataMore);
    if (!currentSystem)
        return;
    if (!currentSystem->lastMouseOver.expired())
        currentSystem->lastMouseOver.lock()->on_popUp();
}

inline void UIEventsSystem::mouse_move_callback(WindowDefine::WindowBase *window, float x, float y)
{
    auto currentSystem=static_cast<UIEventsSystem*>(static_cast<ScenesManage*>(window->userData)->userDataMore);
    if(!currentSystem)
        return;
    currentSystem->mouse_move(x,y);
}

UIEventsSystem::UIEventsSystem()
{

}

inline void UIEventsSystem::init()
{
    if(canvas.expired())
    {
        for(int i=0;i<object->scenes->get_objects_size();++i)
        {
            auto obj=object->scenes->get_object(i);
            canvas=obj->get_module<Canvas>();
            if(!canvas.expired())
                break;
        }
        if(canvas.expired())
        {
            std::cout<<"["<<object->name<<"]->UIEventsSystem->warning:未在场景中找到画布对象"<<std::endl;
        }
    }

    
}

inline void UIEventsSystem::start()
{
    auto window=get_window();
    object->scenes->manage->userDataMore=this;
    if (input()->keyDownCallback)
    {
        std::cout << "[" << object->name << "]->UIEventsSystem->error:按键点击回调被占用" << std::endl;
    }
    else
    {
        input()->keyDownCallback = key_down_callback;
    }
    if (window->m_mouseMoveCallback)
    {
        std::cout << "[" << object->name << "]->UIEventsSystem->error:鼠标移动回调被占用" << std::endl;
    }
    else
    {
        window->m_mouseMoveCallback = mouse_move_callback;
    }
    if(input()->keyUpCallback)
    {
        std::cout << "[" << object->name << "]->UIEventsSystem->error:按键弹起回调被占用" << std::endl;
    }
    else
    {
        input()->keyUpCallback = key_up_callback;
    }
}

inline void UIEventsSystem::update()
{
    if (needUpdateButtonSet)
    {
        update_button_set();
        needUpdateButtonSet = false;
    }
}

inline void UIEventsSystem::exit()
{
    object->scenes->manage=nullptr;
}

inline void UIEventsSystem::update_button_set()
{
    if(canvas.expired())
        return;
    auto c=canvas.lock();
    auto buttons=c->object->find_all_modules<Button>(c->object);
    buttonSet.resize(buttons.size());
    for(int i=0;i<buttons.size();++i)
    {
        buttonSet[i]=buttons[i];
    }
}

inline void UIEventsSystem::click(int x, int y)
{
    auto b = get_button(x, y);
    auto last=lastFocus.lock();
    if (b.lock()!=last&&last)
        last->on_lose_focus();
    if (b.expired())
        return;
    auto a=b.lock();

    a->on_press();
    lastFocus=a;
}

inline void UIEventsSystem::mouse_move(int x, int y)
{
    auto b = get_button(x, y).lock();
    if (lastMouseOver.lock() != b)
    {
        if (!lastMouseOver.expired())
            lastMouseOver.lock()->on_mouse_off();
        lastMouseOver = b;
        if (b)
            b->on_mouse_over();
    }
}

inline WeaPtr<Button> UIEventsSystem::get_button(int x, int y)
{
    if (canvas.expired())
        return WeaPtr<Button>();
    auto window = object->scenes->get_window();
    auto mP = window->pixel_to_NDC_LT(x, y);
    for (int i = buttonSet.size() - 1; i >= 0; --i)
    {
        auto button = buttonSet[i].lock();
        auto rectTransform = button->rectTransform.lock();

        auto mat = button->object->transformMat4;

        auto size_2=rectTransform->size*0.5;
        auto p1 = mat * VEC::Vec4(VEC::Vec3(-size_2.width, size_2.height), 1);
        auto p2 = mat * VEC::Vec4(VEC::Vec3(size_2.width, size_2.height), 1);
        auto p3 = mat * VEC::Vec4(VEC::Vec3(-size_2.width, -size_2.height), 1);
        auto p4 = mat * VEC::Vec4(VEC::Vec3(size_2.width, -size_2.height), 1);

        //auto windowSize = object->scenes->mainCamera->screen;
        // std::cout<<"点击了"<<mP.x<<" "<<mP.y<<std::endl;
        // std::cout<<"按钮端点:p1:"<<p1.x<<" "<<p1.y<<" p2:"<<p2.x<<" "<<p2.y<<" p3:"<<p3.x<<" "<<p3.y<<" p4:"<<p4.x<<" "<<p4.y<<std::endl;

        if (rectTransform->is_point_in_triangle(mP, VEC::Vec2(p1.x, p1.y), VEC::Vec2(p2.x, p2.y), VEC::Vec2(p4.x, p4.y)))
        {
            return button;
        }
        if (rectTransform->is_point_in_triangle(mP, VEC::Vec2(p1.x, p1.y), VEC::Vec2(p4.x, p4.y), VEC::Vec2(p3.x, p3.y)))
        {
            return button;
        }

        /*绘制检测框位置*/
        // auto pp1= window->NDC_to_pixel_LT(p1.x,p1.y);
        // auto pp2= window->NDC_to_pixel_LT(p2.x,p2.y);
        // auto pp3= window->NDC_to_pixel_LT(p3.x,p3.y);
        // auto pp4= window->NDC_to_pixel_LT(p4.x,p4.y);
        // auto oldTransform=window->get_transform();
        // window->set_transform(VEC::Mat4::unit_matrix());
        // window->draw_line_(pp1.x,pp1.y,pp2.x,pp2.y);
        // window->draw_line_(pp1.x,pp1.y,pp3.x,pp3.y);
        // window->draw_line_(pp4.x,pp4.y,pp2.x,pp2.y);
        // window->draw_line_(pp4.x,pp4.y,pp3.x,pp3.y);
        // //std::cout<<"按钮端点:p1:"<<pp1.x<<" "<<pp1.y<<" p2:"<<pp2.x<<" "<<pp2.y<<" p3:"<<pp3.x<<" "<<pp3.y<<" p4:"<<pp4.x<<" "<<pp4.y<<std::endl;
        // window->set_transform(oldTransform);
    }
    return WeaPtr<Button>();
}

Button::Button()
{

}

inline Button::~Button()
{
    if(!eventsSystem.expired())
    {
        eventsSystem.lock()->needUpdateButtonSet=true;
    }
}

inline void Button::init()
{
    if (drawAim.expired())
        drawAim = object->get_module<Image>();
    if (rectTransform.expired())
        rectTransform = object->get_module<RectTransform>();
    auto event= object->find_all_modules<UIEventsSystem>(nullptr);
    if (event.empty())
    {
        auto eveSystemOBJ=object->scenes->create_object("EventsSystem");
        eventsSystem=eveSystemOBJ->add_module<UIEventsSystem>();
    }
    else
    {
        eventsSystem = event.front();
    }
    eventsSystem.lock()->update_button_set();
}

inline void Button::on_press()
{
    if (!drawAim.expired())
        drawAim.lock()->color = pressColor;
    press();
    if(pressCallback)
        pressCallback(this);
    focus=true;
}

inline void Button::on_popUp()
{
    if (!drawAim.expired())
        drawAim.lock()->color = highlightColor;
    popUp();
    if(popUpCallback)
        popUpCallback(this);
}

inline void Button::on_mouse_over()
{
    if (!drawAim.expired())
        drawAim.lock()->color =highlightColor;
    mouse_over();
    if(mouseOverCallback)
        mouseOverCallback(this);
}

inline void Button::on_mouse_off()
{
    if (!drawAim.expired())
        if (focus)
            drawAim.lock()->color = selectColor;
        else
            drawAim.lock()->color = defaultColor;
    mouse_off();
    if (mouseOffCallback)
        mouseOffCallback(this);
}

inline void Button::on_lose_focus()
{
    if (!drawAim.expired())
        drawAim.lock()->color = defaultColor;
    lose_focus();
    if(loseFocusCallback)
        loseFocusCallback(this);
    focus=false;
}

RectTransform::RectTransform(float w, float h)
{
    size.width=w;
    size.height=h;
}

inline void RectTransform::init()
{
    transform=&object->transform;
}

inline void RectTransform::draw()
{
    auto window=object->scenes->get_window();
}


inline bool RectTransform::is_point_in_triangle(const VEC::Vec2 &p, const VEC::Vec2 &a, const VEC::Vec2 &b, const VEC::Vec2 &c)
{
    using namespace VEC;
    // 向量的叉积
    auto crossProduct = [](const Vec2& v1, const Vec2& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    };

    // 计算向量
    Vec2 ab = {b.x - a.x, b.y - a.y};
    Vec2 bc = {c.x - b.x, c.y - b.y};
    Vec2 ca = {a.x - c.x, a.y - c.y};
    Vec2 ap = {p.x - a.x, p.y - a.y};
    Vec2 bp = {p.x - b.x, p.y - b.y};
    Vec2 cp = {p.x - c.x, p.y - c.y};

    // 计算叉积
    float cross1 = crossProduct(ab, ap);
    float cross2 = crossProduct(bc, bp);
    float cross3 = crossProduct(ca, cp);

    // 判断叉积的符号是否相同
    bool has_neg = (cross1 < 0) || (cross2 < 0) || (cross3 < 0);
    bool has_pos = (cross1 > 0) || (cross2 > 0) || (cross3 > 0);

    // 如果有正有负，则点在三角形外
    return !(has_neg && has_pos);
}

Canvas::Canvas()
{
}

inline void Canvas::start()
{
    rectTransform=object->get_module<RectTransform>();
    if (outPutCamera.expired())
    {
        outPutCamera = object->scenes->mainCamera;
    }
    if(projectorMode==ProjectorMode::screen)
    {
        
        auto obj=object;
        obj->layer=0x8;
        while(obj->transform.get_parent())
        {
            obj= static_cast<ScenesObject*>(obj->transform.get_parent()->userData);
            obj->layer=0x8;
        }
        //object->scenes->immediate_move_object(object->scenes->get_object_pos(object),-1);
    }
}

inline void Canvas::draw()
{
    if (outPutCamera.lock() != object->scenes->mainCamera)
    {
        return;
    }
    auto camera=outPutCamera.lock();
    switch (this->projectorMode)
    {
    case ProjectorMode::screen:
        switch (this->plotMode)
        {
        case PlotMode::fixedPixelSize:
        {
            auto oldCameraMode = camera->mode;
            auto oldCameraForward=camera->get_forward();
            auto oldCameraPos=camera->get_position();

            camera->mode = CameraMode::Orthographic;
            camera->set_position(VEC::Vec3(0,0,camera->near_+1));
            camera->set_forward(VEC::Vec3(0,0,-1));
           
            object->transformMat4 = camera->matrix4();

            camera->set_position(oldCameraPos);
            camera->set_forward(oldCameraForward);
            camera->mode = oldCameraMode;
        }
        break;
        case PlotMode::screenScaling:
        {
            auto oldCameraMode = camera->mode;
            auto oldCameraForward=camera->get_forward();
            auto oldCameraPos=camera->get_position();

            camera->mode = CameraMode::Orthographic;
            camera->set_position(VEC::Vec3(0,0,camera->near_+1));
            camera->set_forward(VEC::Vec3(0,0,-1));

            auto rect=rectTransform.lock();

            float scaleWidth = camera->screen.width / rect->size.width;
            float scaleHeight = camera->screen.height / rect->size.height;

            // 使用 matchWidthOrHeight 的值来插值宽度和高度之间的缩放因子
            float scale = scaleWidth * (1-matchWidthOrHeight) + scaleHeight * (matchWidthOrHeight);

            // 使用相同的缩放因子来保持宽高比
            object->transformMat4 = camera->matrix4() * VEC::Mat4::scale(
                                                            VEC::Mat4::unit_matrix(),
                                                            VEC::Vec3(scale, scale, 1));
            camera->set_position(oldCameraPos);
            camera->set_forward(oldCameraForward);
            camera->mode = oldCameraMode;
        }
            break;
        default:
            break;
        }
        break;
    case ProjectorMode::camera:
        /* code */
        break;
    case ProjectorMode::world:
        return;
        break;
    default:
        break;
    }
}


Image::Image(const std::string&filePath)
{
    if(filePath.empty())return;
    load_image_file(filePath);
}

inline Image::~Image()
{

}

inline std::shared_ptr<WindowControl::Picture> &Image::load_image_file(const std::string &filePath_)
{
    std::shared_ptr<WindowControl::Picture> image(new WindowControl::Picture);
    
    image->load_image_form_file(filePath_);

    currentImage=image;
    return currentImage;
}

inline std::shared_ptr<WindowControl::Picture> Image::create_image(int width, int height, const VEC::Color &fillColor)
{
    currentImage.reset(new WindowControl::Picture);
    currentImage->create_image(width,height,fillColor);
    return currentImage;
}

inline void Image::init()
{
    if (rectTransform.expired())
    {
        rectTransform = object->get_module<RectTransform>();
    }
    shader=get_shader();
    if (shader->get_id()==0)
    {
        auto vs = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform*vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

        )";
        auto fs = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 color;

void main()
{
    FragColor = texture(ourTexture, TexCoord)*color;
}

        )";

        std::cout << "图片绘制模块着色器：" << shader->load_shader(vs, fs) << std::endl;

    }
    vertexBuffer=get_buffer_element();
    if(vertexBuffer->VAO==0)
    {
        float buf[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // 右上
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 左下
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f   // 左上
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        vertexBuffer->init_buffer(buf, 5 * 4 * sizeof(float), BufferType::StaticDraw);
        vertexBuffer->init_element(indices, 3 * 2 * sizeof(unsigned int), BufferType::StaticDraw);
        vertexBuffer->set_causality(0, 3, 5 * sizeof(float), (void *)0, GL_FLOAT);
        vertexBuffer->set_causality(1, 2, 5 * sizeof(float), (void *)(3 * sizeof(float)), GL_FLOAT);
    }
}

inline void Image::draw()
{
    if(!currentImage)
        return;
    if(rectTransform.expired())
    {
        auto window=get_window();
        window->draw_image(currentImage.get());
        return;
    }
    shader->use();
    shader->set_mat4("transform",VEC::Mat4::scale(object->transformMat4,VEC::Vec3(rectTransform.lock()->size,1)));
    shader->set_color("color",color);
    glBindVertexArray(vertexBuffer->VAO);
    glBindTexture(GL_TEXTURE_2D, currentImage->get_openGL_id());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // auto window = object->scenes->get_window();
    // auto imageSize=currentImage->image_size();
    // if (rectTransform)
    // {
    //    // auto screenSize=VEC::Vec2(object->scenes->mainCamera->screen.width,object->scenes->mainCamera->screen.height)*0.5;
    //     currentImage->transform.scale = VEC::Vec3(rectTransform->size,1);
    // }
    // window->draw(currentImage.get());
}

CameraModule::CameraModule()
{
    
}

inline void CameraModule::transform_to_camera_orientation()
{
    camera->set_position(object->transform.get_world_position());
    auto target=object->transform.calculate_absolute_position(VEC::Vec3(0,0,-1));
    camera->set_target(target);
}

inline void CameraModule::init()
{
    if(!object->scenes->mainCamera)
    {
        object->scenes->mainCamera=camera;
    }
    camera->screen=get_window()->get_size();
}

inline void CameraModule::draw()
{
    // VEC::Vec3 gridPoints[]={
    //     {0,0,-1},
    //     {0.5,0,0},
    //     {-0.5,0,0},
    //     {0,0,-1},
    // };
    // WindowControl::Lines grid(gridPoints,sizeof(gridPoints)/sizeof(gridPoints[0]));
    // grid.transform=object->transform;
    // object->scenes->get_window()->draw(&grid);

}

inline void CameraModule::size(int w, int h)
{
    camera->screen.width = w;
    camera->screen.height = h;
}

inline Camera *CameraModule::operator->()
{
    return camera.get();
}

Text::Text()
{

}

inline void Text::init()
{
    rectTransform=object->get_module<RectTransform>();
    shader=get_shader();
    if (shader->get_id()==0)
    {
        auto vs = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform*vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

        )";
        auto fs = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(ourTexture, TexCoord).r);
    FragColor = color * sampled;
}

        )";

        std::cout << "文本绘制模块着色器：" << shader->load_shader(vs, fs) << std::endl;

    }
    vertexBuffer=get_buffer_element();
    if(vertexBuffer->VAO==0)
    {
        float buf[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // 左上
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        vertexBuffer->init_buffer(buf, 5 * 4 * sizeof(float), BufferType::DynamicDraw);
        vertexBuffer->init_element(indices, 3 * 2 * sizeof(unsigned int), BufferType::StaticDraw);
        vertexBuffer->set_causality(0, 3, 5 * sizeof(float), (void *)0, GL_FLOAT);
        vertexBuffer->set_causality(1, 2, 5 * sizeof(float), (void *)(3 * sizeof(float)), GL_FLOAT);
    }
}

inline void Text::draw()
{
    auto window=get_window();
    
    if(!is_opengl_window())
    {
        auto pos= object->transform.position;
        window->draw_text(pos.x,pos.y,showText.c_str(),&textStyle);
        return;
    }

    auto openglWindow=(WindowDefine::WindowOpenGLWin32*)(window);

    shader->use();
    shader->set_mat4("transform",object->transformMat4);
    shader->set_color("color",textStyle.color);


    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertexBuffer->VAO);
    auto font=openglWindow->get_free_type_font(&textStyle);
    int x=0;
    int y=0;
    auto strData=showText.data();

    float scale=1.0f;
    auto rect=rectTransform.lock();
    if (!rectTransform.expired())
    {
        //std::cout<<textDrawSize.height<<std::endl;
        switch (textStyle.verticalAlign)
        {
        case TextAlign::Top:
            y = rect->size.height / 2 - textStyle.font.size;
            break;
        case TextAlign::Center:
            y = (+textDrawSize.height-textStyle.font.size) / 2;
            break;
        case TextAlign::Bottom:
            y = -rect->size.height / 2+ textDrawSize.height;
            break;
        default:
            break;
        }
    }
    textDrawSize.height=y;
    while(1)
    {
        if(*strData=='\0')
            break;
        auto lineSize=get_str_line_size(strData);
        lineSize=std::min(lineSize,rect->size.width);
        textDrawSize.width = lineSize;

        if (!rect)
        {
            scale = 1.0f / textStyle.font.size;
        }
        else
        {
            switch (textStyle.horizontalAlign)
            {
            case TextAlign::Left:
                x = -rect->size.width / 2;
                break;
            case TextAlign::Right:
                x = +rect->size.width / 2 - textDrawSize.width;
                break;
            case TextAlign::Center:
                x = -textDrawSize.width / 2;
                break;
            default:
                break;
            }

        }
        auto startX=x;
        if(y<-window->get_size().height)
            break;
        while(1)
        {
            char* last=strData;
            char32_t str = utf8_decode(strData, &strData);
            if (str == '\0')
            {
                strData = "\0";
                break;
            }
            Character *ch = openglWindow->get_char_character(str, font);
            if (!ch)
                continue;
            if (!textStyle.singleLine)
                if (str == '\n')
                {
                    y -= textStyle.font.size;
                    break;
                }
                else if(textStyle.wordWrap && (x + (ch->advance >> 6) * scale > startX+rect->size.width))
                {
                    y -= textStyle.font.size;
                    strData=last;
                    break;
                }

            GLfloat xpos = x + ch->bearing.x * scale;
            GLfloat ypos = y - (ch->size.y - ch->bearing.y) * scale;

            GLfloat w = ch->size.x * scale;
            GLfloat h = ch->size.y * scale;

            // 对每个字符更新VBO
            GLfloat vertices[4][5] = {
                {xpos, ypos + h, 0.0f, 0.0f, 0.0f},
                {xpos, ypos, 0.0f, 0.0f, 1.0f},
                {xpos + w, ypos, 0.0f, 1.0f, 1.0f},
                {xpos + w, ypos + h, 0.0f, 1.0f, 0.0f}};

            // 绑定字符纹理
            glBindTexture(GL_TEXTURE_2D, ch->textureID);
            vertexBuffer->update_buffer(vertices, sizeof(vertices));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // 移动到下一个字符位置
            x += (ch->advance >> 6) * scale;
        }
    }
    textDrawSize.height-=y;
    //textDrawSize.height+=textStyle.font.size;
    
    // 解绑VAO
    glBindVertexArray(0);
    // 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
}
inline float Text::get_str_line_size(char *str) const
{
    if(*str=='\0')
    {
        return 0;
    }
    float x=0;
    auto openglWindow=(WindowDefine::WindowOpenGLWin32*)(get_window());
    auto font=openglWindow->get_free_type_font(&textStyle);
    while (1)
    {
        char32_t c = utf8_decode(str, &str);
        if (c == '\0' || c == '\n')
            return x;
        Character *ch = openglWindow->get_char_character(c, font);
        if (!ch)
            continue;
        x += (ch->advance >> 6);
    }
    return 0.0f;
}
inline VEC::Vec2 Text::get_character_position(size_t pos) const
{
     auto window=get_window();
    
    if(!is_opengl_window())
    {
        return VEC::Vec2();
    }
    size_t p=0;
    auto openglWindow=(WindowDefine::WindowOpenGLWin32*)(window);
    auto font=openglWindow->get_free_type_font(&textStyle);
    int x=0;
    int y=0;
    auto strData=(char*)showText.data();
    auto textDrawSizeCopy=textDrawSize;
    float scale=1.0f;
    auto rect=rectTransform.lock();
    if (!rectTransform.expired())
    {
        //std::cout<<textDrawSize.height<<std::endl;
        switch (textStyle.verticalAlign)
        {
        case TextAlign::Top:
            y = rect->size.height / 2 - textStyle.font.size;
            break;
        case TextAlign::Center:
            y = (+textDrawSizeCopy.height-textStyle.font.size) / 2;
            break;
        case TextAlign::Bottom:
            y = -rect->size.height / 2+ textDrawSizeCopy.height;
            break;
        default:
            break;
        }
    }
    textDrawSizeCopy.height=y;
    while(1)
    {
        if(*strData=='\0')
            break;
        auto lineSize=get_str_line_size(strData);
        lineSize=std::min(lineSize,rect->size.width);
        textDrawSizeCopy.width = lineSize;

        if (!rect)
        {
            scale = 1.0f / textStyle.font.size;
        }
        else
        {
            switch (textStyle.horizontalAlign)
            {
            case TextAlign::Left:
                x = -rect->size.width / 2;
                break;
            case TextAlign::Right:
                x = +rect->size.width / 2 - textDrawSizeCopy.width;
                break;
            case TextAlign::Center:
                x = -textDrawSizeCopy.width / 2;
                break;
            default:
                break;
            }

        }
        auto startX=x;
        while(1)
        {
            char* last=strData;
            char32_t str = utf8_decode(strData, &strData);
            if (str == '\0')
            {
                strData = "\0";
                break;
            }
            Character *ch = openglWindow->get_char_character(str, font);
            if (!ch)
                continue;
            if (!textStyle.singleLine)
                if (str == '\n')
                {
                    y -= textStyle.font.size;
                    break;
                }
                else if(textStyle.wordWrap && (x + (ch->advance >> 6) * scale > startX+rect->size.width))
                {
                    y -= textStyle.font.size;
                    strData=last;
                    break;
                }
            if (p == pos)
                return VEC::Vec2(x, y);
            p++;

            // 移动到下一个字符位置
            x += (ch->advance >> 6) * scale;
        }
    }
    return VEC::Vec2(x, y);
}
inline size_t Text::get_character_index_from_position(const VEC::Vec2& position) const 
{
    auto window=get_window();
    if(!is_opengl_window())
    {
        return 0;
    }
    size_t p=0;
    auto openglWindow=(WindowDefine::WindowOpenGLWin32*)(window);
    auto font=openglWindow->get_free_type_font(&textStyle);
    int x=0;
    int y=0;
    auto strData=(char*)showText.data();
    auto textDrawSizeCopy=textDrawSize;
    float scale=1.0f;
    auto rect=rectTransform.lock();
    if (!rectTransform.expired())
    {
        //std::cout<<textDrawSize.height<<std::endl;
        switch (textStyle.verticalAlign)
        {
        case TextAlign::Top:
            y = rect->size.height / 2 - textStyle.font.size;
            break;
        case TextAlign::Center:
            y = (+textDrawSizeCopy.height-textStyle.font.size) / 2;
            break;
        case TextAlign::Bottom:
            y = -rect->size.height / 2+ textDrawSizeCopy.height;
            break;
        default:
            break;
        }
    }
    textDrawSizeCopy.height=y;
    while(1)
    {
        if(*strData=='\0')
            break;
        auto lineSize=get_str_line_size(strData);
        lineSize=std::min(lineSize,rect->size.width);
        textDrawSizeCopy.width = lineSize;

        if (!rect)
        {
            scale = 1.0f / textStyle.font.size;
        }
        else
        {
            switch (textStyle.horizontalAlign)
            {
            case TextAlign::Left:
                x = -rect->size.width / 2;
                break;
            case TextAlign::Right:
                x = +rect->size.width / 2 - textDrawSizeCopy.width;
                break;
            case TextAlign::Center:
                x = -textDrawSizeCopy.width / 2;
                break;
            default:
                break;
            }

        }
        auto startX=x;
        while(1)
        {
            char* last=strData;
            char32_t str = utf8_decode(strData, &strData);
            if (str == '\0')
            {
                strData = "\0";
                break;
            }
            Character *ch = openglWindow->get_char_character(str, font);
            if (!ch)
                continue;
            if (!textStyle.singleLine)
                if (str == '\n')
                {
                    y -= textStyle.font.size;
                    break;
                }
                else if(textStyle.wordWrap && (x + (ch->advance >> 6) * scale > startX+rect->size.width))
                {
                    y -= textStyle.font.size;
                    strData=last;
                    break;
                }
            if(position.x>=x&&position.x<=(x+(ch->advance >> 6) * scale)&&y<=position.y&&position.y<=y+textStyle.font.size)
            {
                if(position.x>(x+(ch->advance >> 6) * scale*0.5f))
                    return p+1;
                return p;
            }
                
            p++;

            // 移动到下一个字符位置
            x += (ch->advance >> 6) * scale;
        }
    }
    return p;
}
inline size_t Text::get_character_count_utf_8() const
{
    size_t charCount = 0;
    size_t index = 0;
    while (index < showText.length())
    {
        if ((showText[index] & 0xC0) != 0x80)
        { // 判断是否是 UTF-8 字符的起始字节
            ++charCount;
        }
        ++index;
    }
    return charCount;
}
inline size_t Text::get_character_pos_in_string(size_t char_index) const
{
    return utf8_char_position(showText,char_index);
}
inline void Text::delete_prev_utf8_char(size_t char_index)
{
    delete_prev_utf8_char(showText,char_index);
}
inline void Text::delete_prev_utf8_char(std::string &str, size_t char_index)
{
    if (char_index == 0)
    {
        // 如果是第一个字符，无需删除
        return;
    }

    // 找到要删除字符的起始位置
    size_t char_pos = utf8_char_position(str, char_index);

    // 找到前一个字符的起始位置
    size_t prev_char_pos = char_pos;
    do
    {
        --prev_char_pos;
    } while (prev_char_pos > 0 && !is_utf8_start_byte(static_cast<unsigned char>(str[prev_char_pos])));

    // 删除前一个字符到当前字符之间的内容
    str.erase(prev_char_pos, char_pos - prev_char_pos);
}

inline bool Text::is_utf8_start_byte(unsigned char byte)
{
    return (byte & 0xC0) != 0x80; // UTF-8 起始字节的高位不是 10
}
inline size_t Text::utf8_char_position(const std::string &str, size_t char_index)
{
    size_t pos = 0;
    size_t utf8_index = 0;

    while (pos < str.length())
    {
        if ((str[pos] & 0xC0) != 0x80)
        { 
            if (utf8_index == char_index)
            {
                return pos; 
            }
            ++utf8_index;
        }
        ++pos;
    }

    return str.length();
}
} // namespace Module
}
}