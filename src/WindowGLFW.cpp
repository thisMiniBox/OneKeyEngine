#include<glad/glad.h>
#include<OneKeyEngine/WindowDefine/WindowGLFW.hpp>
#include<iostream>




namespace OneKeyEngine
{
namespace WindowDefine
{
int WindowGL_GLFW::windowCount=0;
GL::LineStrip *WindowGL_GLFW::get_line_base() const
{
    if(s_lineBase)return s_lineBase;
    s_lineBase=new GL::LineStrip({{0,0,0},{1,0,0}});
    return s_lineBase;
}
void WindowGL_GLFW::on_size(GLFWwindow *window, int width, int height)
{
    auto extract = (WindowGL_GLFW *)glfwGetWindowUserPointer(window);
    extract->m_windowSize=VEC::Size(width,height);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    if (extract->m_windowSizeCallback)
    {
        extract->m_windowSizeCallback(extract, width, height);
    }
    
}


WindowGL_GLFW::~WindowGL_GLFW()
{
    clear_buffer();
    close();
}
// 键盘按键回调
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto extract = (WindowGL_GLFW *)glfwGetWindowUserPointer(window);
    glfwMakeContextCurrent(window);
    auto keyCode=extract->glfw_key_code_to_oke_key_code(key);
    if (action == GLFW_PRESS)
    {
        if (extract->m_keyDownCallback)
        {
            extract->m_keyDownCallback(extract, keyCode);
        }
        switch (keyCode)
        {
        case KeyCode::Enter:
        case KeyCode::Tab:
        case KeyCode::Backspace:
            if (extract->m_inputCallback)
            {
                extract->m_inputCallback(extract, (char)keyCode);
            }
            break;

        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (extract->m_keyUpCallback)
        {
            extract->m_keyUpCallback(extract, keyCode);
        }
    }
    else if(action==GLFW_REPEAT)
    {
        switch (keyCode)
        {
        case KeyCode::Enter:
        case KeyCode::Tab:
        case KeyCode::Backspace:
            if (extract->m_inputCallback)
            {
                extract->m_inputCallback(extract, (char)keyCode);
            }
            break;

        default:
            break;
        }
    }

}

// 鼠标按键回调
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    auto extract = (WindowGL_GLFW *)glfwGetWindowUserPointer(window);
    glfwMakeContextCurrent(window);

    if (action == GLFW_PRESS)
    {
        if (extract->m_keyDownCallback)
        {
            extract->m_keyDownCallback(extract, extract->glfw_key_code_to_oke_key_code(button));
        }
        if (extract->m_mouseButtonDown)
        {
            extract->m_mouseButtonDown(extract, extract->glfw_key_code_to_oke_key_code(button));
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (extract->m_keyUpCallback)
        {
            extract->m_keyUpCallback(extract, extract->glfw_key_code_to_oke_key_code(button));
        }
        if (extract->m_mouseButtonUp)
        {
            extract->m_mouseButtonUp(extract, extract->glfw_key_code_to_oke_key_code(button));
        }
    }
}

// 光标位置回调
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    auto extract = (WindowGL_GLFW *)glfwGetWindowUserPointer(window);
    glfwMakeContextCurrent(window);

    extract->mousePosition.x=xpos;
    extract->mousePosition.y=ypos;
    if (extract->m_mouseMoveCallback)
    {
        extract->m_mouseMoveCallback(extract, xpos,ypos);
    }
}

// 滚轮事件回调
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto extract = (WindowGL_GLFW *)glfwGetWindowUserPointer(window);

    glfwMakeContextCurrent(window);
    if (extract->m_mouseWheelCallbck)
    {
        extract->m_mouseWheelCallbck(extract, xoffset,yoffset);
    }
}
void char_callback(GLFWwindow *window, unsigned int codepoint)
{
    auto extract = (WindowGL_GLFW *)glfwGetWindowUserPointer(window);

    glfwMakeContextCurrent(window);
    if (extract->m_inputCallback)
    {
        extract->m_inputCallback(extract, codepoint);
    }
}

WindowCreateResult OneKeyEngine::WindowDefine::WindowGL_GLFW::create_window(const std::string &titleName, int x, int y, int width, int height, WindowBase *parent) 
{
    if (windowCount == 0)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    if(window)
    {
        close();
    }
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "GLFW窗口创建失败" << std::endl;
        return WindowCreateResult::WCR_WINDOW_CREATE_FAILED;
    }
    glfwMakeContextCurrent(window);

    static bool gladInit=false;
    if(!gladInit)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return WindowCreateResult::WCR_GLAD_INIT_FAILED;
        }
        gladInit = true;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowUserPointer(window,this);

    glfwSetFramebufferSizeCallback(window, on_size);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCharCallback(window, char_callback);

    m_windowSize.width=width;
    m_windowSize.height=height;
    windowCount+=1;
    return WindowCreateResult::WCR_SUCCESS;
}

int WindowGL_GLFW::close()
{
    if(window)
        glfwDestroyWindow(window);
    window = nullptr;
    windowCount -= 1;
    if (windowCount == 0)
    {
        glfwTerminate();
    }
    return 0;
}

int WindowGL_GLFW::set_size_(int width, int height) const
{
    if (window)
        glfwSetWindowSize(window, width, height);
    return 0;
}
int WindowGL_GLFW::get_size_(int *width, int *height) const
{
    glfwGetWindowSize(window,width,height);
    return 0;
}
int WindowGL_GLFW::set_position(int x, int y) const
{
    glfwSetWindowPos(window,x,y);
    return 0;
}
int WindowGL_GLFW::get_position(int *x, int *y) const
{
    glfwGetWindowPos(window,x,y);
    return 0;
}
void WindowGL_GLFW::set_window_title(const std::string &title) const
{
    glfwSetWindowTitle(window,title.c_str());
}
std::string WindowGL_GLFW::get_window_title() const
{
    return glfwGetWindowTitle(window);
}
WindowDrawResult WindowGL_GLFW::draw_line_(float x1, float y1, float x2, float y2, LineStyle *style) const
{
    if (style && style->width > 1)
    {
        auto start=VEC::Vec2(x1,y1);
        auto end=VEC::Vec2(x2,y2);
        auto dir=end-start;
        dir=VEC::Vec2(-dir.y,dir.x).normalized()*style->width*0.5f;
        std::vector<float>points={
            x1+dir.x,y1+dir.y,
            x1-dir.x,y1-dir.y,
            x2-dir.x,y2-dir.y,
            x2+dir.x,y2+dir.y,
        };

        return draw_fill_polygonal_(0,0,points.data(),4,style->color.r,style->color.g,style->color.b,style->color.a);
    }

    auto shader = get_line_shader();
    shader->use();
    if (style)
        shader->set_color("ourColor", style->color);
    else
        shader->set_color("ourColor", VEC::Color(0, 0, 0, 1));
    VEC::Vec3 start = pixel_to_NDC_LT(x1, y1);
    VEC::Vec3 end = pixel_to_NDC_LT(x2, y2);
    VEC::Vec3 f = end - start;
    float rad = VEC::angle_between(f, VEC::Vec3(1, 0, 0));
    if (f.y < 0)
    {
        rad = -rad;
    }
    float len = (f).length();
    VEC::Mat4 tran(1.0f);
    tran.translate(start);
    tran.scale(VEC::Vec3(len));
    tran.rotate(VEC::Vec3(0, 0, 1), rad);
    shader->set_mat4("transform", transform*tran);

    get_line_base()->draw(*shader);

    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult WindowGL_GLFW::draw_pixel_(float x, float y, float r, float g, float b, float a) const
{
    auto shader=get_line_shader();
    shader->use();
    shader->set_color("ourColor", VEC::Color(r,g,b,a));
    VEC::Mat4 tran(1.0f);
    tran.translate(pixel_to_NDC_LT(x,y));
    tran.scale(VEC::Vec3(1.0f/m_windowSize.width*2));
    //tran.rotate(VEC::Vec3(0,1,0),VEC::angle_to_radian(90.0f));
    shader->set_mat4("transform",tran);
    get_line_base()->draw(*shader);
    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult WindowGL_GLFW::draw_rectangle_(float x, float y, float width, float height, LineStyle *style) const
{
    draw_line_(x,y,x+width,y,style);
    draw_line_(x+width,y,x+width,y+height);
    draw_line_(x+width,y+height,x,y+height);
    draw_line_(x,y,x,y+height);
    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult WindowGL_GLFW::draw_polygonal_(float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
{
    VEC::Vec2 start(x + *points, y + *(points + 1));
    VEC::Vec2 end = start;
    for (auto i = 1u; i < pointsSize; ++i)
    {
        end.x = x + *(points + 2 * i);
        end.y = y + *(points + 2 * i + 1);
        draw_line_(start.x, start.y, end.x, end.y, style);
        start = end;
    }
    draw_line_(start.x, start.y, x + *points, y + *(points + 1), style);
    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult WindowGL_GLFW::draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r, float g, float b, float a) const
{
  static GL::Buffer buffer;
    static bool needInit = true;

    if (needInit)
    {
        int max_polygon_vertices=8;
        // 初始顶点缓冲区
        buffer.init_buffer(nullptr, sizeof(float)*3*max_polygon_vertices, BufferType::DynamicDraw);
        buffer.set_causality(0, 3, 3 * sizeof(float), (void*)0, GL_FLOAT);
        needInit = false;
    }

    // 更新顶点数据
    std::vector<float> vertices(pointsArraySize * 3);
    for (uint32_t i = 0; i < pointsArraySize; ++i)
    {
        auto nPos = pixel_to_NDC_LT(points[i * 2 + 0] + x, points[i * 2 + 1] + y);
        vertices[i * 3 + 0] = nPos.x; // X 坐标
        vertices[i * 3 + 1] = nPos.y; // Y 坐标
        vertices[i * 3 + 2] = 0.0f;   // Z 坐标，假设在二维空间中Z为0
    }
    buffer.update_buffer(vertices.data(), vertices.size() * sizeof(float));

    // 使用着色器程序
    auto shader = get_line_shader();
    shader->use();
    //auto tran = VEC::Mat4::unit_matrix();
    shader->set_mat4("transform",transform);
    shader->set_color("ourColor", r, g, b, a);

    // 绘制多边形
    glBindVertexArray(buffer.VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, pointsArraySize);
    glBindVertexArray(0);

    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult WindowGL_GLFW::draw_text_(float x, float y, const char *text, TextStyle *style) const
{
   if(!text)
        return WindowDrawResult::WDR_PARAMETER_ERROR;
    if (!style)
    {
        TextStyle textStyle;
        return draw_text_(x,y,text,&textStyle);
    }
    switch_openGL();
#ifdef FT2BUILD_H_
    if (style)
        y = m_windowSize.height - y - style->font.size;
    else
        y = m_windowSize.height - y-24;

    // 设置默认文本颜色和比例
    float scale = 1.0f;                 // 默认比例
    VEC::Color color(1.0f, 1.0f, 1.0f); // 默认颜色为白色

    if (style)
    {
        color = style->color;
    }
    // 获取字体
    auto font = get_free_type_font(style);

    if (s_textVectex.VAO == 0)
    {
        s_textVectex.init_buffer(nullptr,sizeof(GLfloat) * 6 * 4,BufferType::DynamicDraw);
        s_textVectex.set_causality(0,4,4 * sizeof(GLfloat),0,GL_FLOAT);
        // glGenVertexArrays(1, &VAO);
        // glGenBuffers(1, &VBO);
        // glBindVertexArray(VAO);
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);
    }

    // 激活纹理单元和绑定VAO
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(s_textVectex.VAO);
    // 使用文本着色器
    auto textShader = get_text_shader();
    textShader->use();

    // 设置投影矩阵
    auto projection = VEC::ortho(0, m_windowSize.width, 0, m_windowSize.height);
    textShader->set_mat4("projection", projection);
    textShader->set_vec4("textColor",color);

    std::string strText=text;
    char* strData=const_cast<char*>( strText.data());
    // 循环绘制每个字符
    while(*strData!='\0')
    {
        char32_t str=utf8_decode(strData,&strData);
        if(str=='\0')
            break;
        Character *ch = get_char_character(str, font);
        if (!ch)
            continue;

        GLfloat xpos = x + ch->bearing.x * scale;
        GLfloat ypos = y - (ch->size.y - ch->bearing.y) * scale;

        GLfloat w = ch->size.x * scale;
        GLfloat h = ch->size.y * scale;

        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        // 绑定字符纹理
        glBindTexture(GL_TEXTURE_2D, ch->textureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, s_textVectex.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 移动到下一个字符位置
        x += (ch->advance >> 6) * scale; // 位移6位来获得单位为像素的值 (2^6 = 64)
    }

    // 解绑VAO
    glBindVertexArray(0);
    // 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);

    return WindowDrawResult::WDR_SUCCESS;
#else
    std::cout<<R"(
无法绘制gl文本，需要链接freetype库：https://github.com/ubawurinna/freetype-windows-binaries
#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_OUTLINE_H
    )"<<std::endl;
    return WindowDrawResult::WDR_FALSE;
#endif
}
WindowDrawResult WindowGL_GLFW::clear_background_(float r, float g, float b, float a) const
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult WindowGL_GLFW::get_pixel(int x, int y, float *r, float *g, float *b) const
{
    GLfloat pixel[4];
    // 读取像素颜色
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, pixel);
    *r = pixel[0];
    *g = pixel[1];
    *b = pixel[2];
    return WindowDrawResult::WDR_SUCCESS;
}
void WindowGL_GLFW::set_transform(const VEC::Mat4 &mat)
{
    transform=mat;
}
VEC::Mat4 WindowGL_GLFW::get_transform() const
{
    return transform;
}
void WindowGL_GLFW::set_mouse_position(const VEC::Vec2 &pos)
{
    glfwSetCursorPos(window, pos.x, pos.y);
}
void WindowGL_GLFW::set_mode(ModeType mode, ModeValue value) const
{
        switch (mode)
    {
    case ModeType::ShowCursor:
        switch (value)
        {
        case ModeValue::True:
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
            break;
        case ModeValue::False:
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
            break;
        default:
            break;
        }
        break;
    case ModeType::CaptureCursor:
        switch (value)
        {
        case ModeValue::True:
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED );
            break;
        case ModeValue::False:
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
            break;
        default:
            break;
        }
        break;
    case ModeType::MaxWindow:
        switch (value)
        {
        case ModeValue::True:
            glfwMaximizeWindow(window);
            break;
        case ModeValue::False:
            glfwRestoreWindow(window);
            break;
        default:
            break;
        } 
        break;
    case ModeType::MinWindow:
        switch (value)
        {
        case ModeValue::True:
            glfwIconifyWindow(window);
            break;
        case ModeValue::False:
            glfwRestoreWindow(window);
            break;
        default:
            break;
        } 
        break;
    case ModeType::FullScreen:
    {
        static VEC::Rectangle windowRect;
        static bool fillWindow=false;
        switch (value)
        {
        case ModeValue::True:
        {
            if (fillWindow)
                break;
            get_position(&windowRect.x, &windowRect.y);
            get_size_(&windowRect.z, &windowRect.w);

            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            // 获取显示器的视频模式
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            // 设置窗口为全屏模式
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

            fillWindow = true;
        }
            break;
        case ModeValue::False:
            if (!fillWindow)
                break;
            glfwSetWindowMonitor(window, NULL, windowRect.x,windowRect.y,windowRect.z,windowRect.w, 0);
            fillWindow=false;
            break;
        default:
            break;
        }
    }

        break;
    default:
        break;
    }
}
int WindowGL_GLFW::processing_event()
{
    glfwPollEvents();
    return 0;
}
int WindowGL_GLFW::should_draw()
{
    processing_event();
    auto nowTime = get_time();
    m_realUpdateTime = nowTime - m_lastRefreshTime;
    m_lastRefreshTime = nowTime;
    if(m_drawCallback)
    {
        m_drawCallback(this);
    }
    refresh_screen();
    return !glfwWindowShouldClose(window);
}
float WindowGL_GLFW::set_frame_rate(float frameRate)
{
    glfwWindowHint(GLFW_REFRESH_RATE, frameRate);
    return 1.0f/frameRate;
}
void WindowGL_GLFW::refresh_screen() const
{
    glfwSwapBuffers(window);
}
bool WindowGL_GLFW::set_clipboard_text(const std::string &str) const
{
    glfwSetClipboardString(window, str.c_str());
    return true;
}
std::string WindowGL_GLFW::get_clipboard_text() const
{
    return std::string(glfwGetClipboardString(window));
}

void WindowGL_GLFW::set_view(const VEC::Rectangle &rect) const
{
    glViewport(rect.x, rect.y, rect.right - rect.left, rect.bottom - rect.top);
}

KeyCode WindowGL_GLFW::glfw_key_code_to_oke_key_code(int code)
{
    switch (code)
    {
        case GLFW_KEY_A: return KeyCode::A;
        case GLFW_KEY_B: return KeyCode::B;
        case GLFW_KEY_C: return KeyCode::C;
        case GLFW_KEY_D: return KeyCode::D;
        case GLFW_KEY_E: return KeyCode::E;
        case GLFW_KEY_F: return KeyCode::F;
        case GLFW_KEY_G: return KeyCode::G;
        case GLFW_KEY_H: return KeyCode::H;
        case GLFW_KEY_I: return KeyCode::I;
        case GLFW_KEY_J: return KeyCode::J;
        case GLFW_KEY_K: return KeyCode::K;
        case GLFW_KEY_L: return KeyCode::L;
        case GLFW_KEY_M: return KeyCode::M;
        case GLFW_KEY_N: return KeyCode::N;
        case GLFW_KEY_O: return KeyCode::O;
        case GLFW_KEY_P: return KeyCode::P;
        case GLFW_KEY_Q: return KeyCode::Q;
        case GLFW_KEY_R: return KeyCode::R;
        case GLFW_KEY_S: return KeyCode::S;
        case GLFW_KEY_T: return KeyCode::T;
        case GLFW_KEY_U: return KeyCode::U;
        case GLFW_KEY_V: return KeyCode::V;
        case GLFW_KEY_W: return KeyCode::W;
        case GLFW_KEY_X: return KeyCode::X;
        case GLFW_KEY_Y: return KeyCode::Y;
        case GLFW_KEY_Z: return KeyCode::Z;
        case GLFW_KEY_0: return KeyCode::A0;
        case GLFW_KEY_1: return KeyCode::A1;
        case GLFW_KEY_2: return KeyCode::A2;
        case GLFW_KEY_3: return KeyCode::A3;
        case GLFW_KEY_4: return KeyCode::A4;
        case GLFW_KEY_5: return KeyCode::A5;
        case GLFW_KEY_6: return KeyCode::A6;
        case GLFW_KEY_7: return KeyCode::A7;
        case GLFW_KEY_8: return KeyCode::A8;
        case GLFW_KEY_9: return KeyCode::A9;
        case GLFW_KEY_LEFT: return KeyCode::Left;
        case GLFW_KEY_RIGHT: return KeyCode::Right;
        case GLFW_KEY_UP: return KeyCode::Up;
        case GLFW_KEY_DOWN: return KeyCode::Down;
        case GLFW_KEY_ENTER: return KeyCode::Enter;
        case GLFW_KEY_ESCAPE: return KeyCode::Escape;
        case GLFW_KEY_SPACE: return KeyCode::Space;
        case GLFW_KEY_LEFT_SHIFT: return KeyCode::LeftShift;
        case GLFW_KEY_RIGHT_SHIFT: return KeyCode::RightShift;
        case GLFW_KEY_LEFT_CONTROL: return KeyCode::LeftCtrl;
        case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RightCtrl;
        case GLFW_KEY_LEFT_ALT: return KeyCode::LeftAlt;
        case GLFW_KEY_RIGHT_ALT: return KeyCode::RightAlt;
        case GLFW_KEY_TAB: return KeyCode::Tab;
        case GLFW_KEY_BACKSPACE: return KeyCode::Backspace;
        case GLFW_KEY_INSERT: return KeyCode::Insert;
        case GLFW_KEY_DELETE: return KeyCode::Delete;
        case GLFW_KEY_HOME: return KeyCode::Home;
        case GLFW_KEY_END: return KeyCode::End;
        case GLFW_KEY_PAGE_UP: return KeyCode::PageUp;
        case GLFW_KEY_PAGE_DOWN: return KeyCode::PageDown;
        case GLFW_KEY_F1: return KeyCode::F1;
        case GLFW_KEY_F2: return KeyCode::F2;
        case GLFW_KEY_F3: return KeyCode::F3;
        case GLFW_KEY_F4: return KeyCode::F4;
        case GLFW_KEY_F5: return KeyCode::F5;
        case GLFW_KEY_F6: return KeyCode::F6;
        case GLFW_KEY_F7: return KeyCode::F7;
        case GLFW_KEY_F8: return KeyCode::F8;
        case GLFW_KEY_F9: return KeyCode::F9;
        case GLFW_KEY_F10: return KeyCode::F10;
        case GLFW_KEY_F11: return KeyCode::F11;
        case GLFW_KEY_F12: return KeyCode::F12;
        case GLFW_KEY_F13: return KeyCode::F13;
        case GLFW_KEY_F14: return KeyCode::F14;
        case GLFW_KEY_F15: return KeyCode::F15;
        case GLFW_KEY_F16: return KeyCode::F16;
        case GLFW_KEY_F17: return KeyCode::F17;
        case GLFW_KEY_F18: return KeyCode::F18;
        case GLFW_KEY_F19: return KeyCode::F19;
        case GLFW_KEY_F20: return KeyCode::F20;
        case GLFW_KEY_F21: return KeyCode::F21;
        case GLFW_KEY_F22: return KeyCode::F22;
        case GLFW_KEY_F23: return KeyCode::F23;
        case GLFW_KEY_F24: return KeyCode::F24;
        case GLFW_KEY_CAPS_LOCK: return KeyCode::Capital;
        case GLFW_KEY_SCROLL_LOCK: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_NUM_LOCK: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_PRINT_SCREEN: return KeyCode::Snapshout;
        case GLFW_KEY_PAUSE: return KeyCode::Pause;
        case GLFW_KEY_MENU: return KeyCode::Apps;
        case GLFW_KEY_KP_0: return KeyCode::Numpad0;
        case GLFW_KEY_KP_1: return KeyCode::Numpad1;
        case GLFW_KEY_KP_2: return KeyCode::Numpad2;
        case GLFW_KEY_KP_3: return KeyCode::Numpad3;
        case GLFW_KEY_KP_4: return KeyCode::Numpad4;
        case GLFW_KEY_KP_5: return KeyCode::Numpad5;
        case GLFW_KEY_KP_6: return KeyCode::Numpad6;
        case GLFW_KEY_KP_7: return KeyCode::Numpad7;
        case GLFW_KEY_KP_8: return KeyCode::Numpad8;
        case GLFW_KEY_KP_9: return KeyCode::Numpad9;
        case GLFW_KEY_KP_DECIMAL: return KeyCode::Dcimal;
        case GLFW_KEY_KP_DIVIDE: return KeyCode::Divide;
        case GLFW_KEY_KP_MULTIPLY: return KeyCode::Multiple;
        case GLFW_KEY_KP_SUBTRACT: return KeyCode::Subtract;
        case GLFW_KEY_KP_ADD: return KeyCode::Add;
        case GLFW_KEY_KP_ENTER: return KeyCode::Enter;
        case GLFW_KEY_KP_EQUAL: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_LEFT_SUPER: return KeyCode::LeftWin;
        case GLFW_KEY_RIGHT_SUPER: return KeyCode::RightWin;
        case GLFW_KEY_LEFT_BRACKET: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_RIGHT_BRACKET: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_SEMICOLON: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_APOSTROPHE: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_GRAVE_ACCENT: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_COMMA: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_PERIOD: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_SLASH: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_BACKSLASH: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_WORLD_1: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_KEY_WORLD_2: return KeyCode::None; // 对应的KeyCode未定义
        case GLFW_MOUSE_BUTTON_LEFT:return KeyCode::MouseLeftButton;
        case GLFW_MOUSE_BUTTON_RIGHT:return KeyCode::MouseRightButton;
        case GLFW_MOUSE_BUTTON_MIDDLE:return KeyCode::MouseMiddleButton;
        case GLFW_MOUSE_BUTTON_4:return KeyCode::MouseButtonX1;
        case GLFW_MOUSE_BUTTON_5:return KeyCode::MouseButtonX2;
        default: return KeyCode::None;
    }
}

FT_Face WindowGL_GLFW::get_free_type_font(const TextStyle *style) const
{
    switch_openGL();
    if(style==nullptr)
    {
        TextStyle sty;
        return get_free_type_font(&sty);
    }
    auto p = s_face.find(style->font);
    if (p != s_face.end())
        return p->second;

    if (s_ft == nullptr) {
        if (FT_Init_FreeType(&s_ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return nullptr;
        }
    }

    std::string fontPath = style->font.fontPath;
    if (fontPath.empty()) {
        std::cout << "ERROR::FREETYPE: Font not found in system " << style->font.fontPath << std::endl;
        return nullptr;
    }

    FT_Face face;
    if (FT_New_Face(s_ft, fontPath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font " << fontPath << std::endl;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(face, 0, style->font.size);

    // 实现加粗效果
    if (style->font.style == FontStyle::Bold) {
        FT_Outline_Embolden(&face->glyph->outline, 1 << 6);  // 单位是1/64像素
    }
    // 实现斜体效果
    if (style->font.style == FontStyle::Italic) {
        FT_Matrix matrix;
        matrix.xx = 0x10000L;
        matrix.xy = 0x6000L;  // 斜体变换矩阵
        matrix.yx = 0;
        matrix.yy = 0x10000L;
        FT_Set_Transform(face, &matrix, nullptr);
    }
    else
    {
        FT_Matrix identity_matrix = {0x10000L, 0, 0, 0x10000L};
        FT_Vector zero_vector = {0, 0};
        FT_Set_Transform(face, &identity_matrix, &zero_vector);
    }

    s_face.insert(std::make_pair(style->font, face));
    return face;
}

void WindowGL_GLFW::switch_openGL()const
{
    static GLFWwindow* lastWindow=nullptr;
    if(lastWindow==window)
        return;
    lastWindow=window;
    glfwMakeContextCurrent(window);
}

GL::ShaderGL *WindowGL_GLFW::get_line_shader() const
{
    if(s_shaderBase)return s_shaderBase;
    const char *vertexShaderBaseSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "\n"
                                     "uniform mat4 transform;\n"
                                     "void main()\n"
                                     "{\n"
                                     "    gl_Position = transform*vec4(aPos, 1.0f);\n"
                                     "}\n";
    const char *fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "\n"
                                    "uniform vec4 ourColor;\n"
                                    "\n"
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = vec4(ourColor);\n"
                                    "}\n";
    s_shaderBase=new GL::ShaderGL(vertexShaderBaseSource,fragmentShaderSource,true);
    return s_shaderBase;
}

GL::ShaderGL *WindowGL_GLFW::get_text_shader() const
{
    if (s_textShader)
        return s_textShader;
    // 着色器源代码
    const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection*vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
)";
    s_textShader=new GL::ShaderGL;
    s_textShader->load_shader(vertexShaderSource,fragmentShaderSource);
    return s_textShader;
}
Character* WindowGL_GLFW::get_char_character(char32_t ch, FT_Face ft_font) const
{
    switch_openGL();
    // 查找字体
    auto ft_char = s_characters.find(ft_font);
    if (ft_char == s_characters.end())
    {
        // 如果未找到字体，插入新的字体映射
        auto result = s_characters.insert({ft_font, {}});
        if (!result.second)
        {
            return nullptr; // 插入失败
        }
        ft_char = result.first;
    }

    // 查找字符纹理
    auto charTexture = ft_char->second.find(ch);
    if (charTexture != ft_char->second.end())
    {
        return &(charTexture->second); // 找到
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 加载字符到FT_Face
    if (FT_Load_Char(ft_font, ch, FT_LOAD_DEFAULT | FT_LOAD_RENDER))
    {
        std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
        return nullptr;
    }

    // 生成纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        ft_font->glyph->bitmap.width,
        ft_font->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        ft_font->glyph->bitmap.buffer
    );

    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 储存字符供之后使用
    Character character = {
        texture,
        VEC::iVec2(ft_font->glyph->bitmap.width, ft_font->glyph->bitmap.rows),
        VEC::iVec2(ft_font->glyph->bitmap_left, ft_font->glyph->bitmap_top),
        (GLuint)ft_font->glyph->advance.x
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    // 插入字符纹理到字体映射
    auto inserted = ft_char->second.insert({ch, character});

    return &inserted.first->second;
}

void WindowGL_GLFW::clear_char_texture(uint32_t id) const
{
    switch_openGL();
    for(auto font=s_characters.begin();font!=s_characters.end();font++)
    {
        for (auto ch = font->second.begin(); ch != font->second.end(); ch++)
        {
            if(ch->second.textureID==id)
            {
                glDeleteTextures(1,&id);
                font->second.erase(ch);
                if(font->second.size()==0)
                {
                    s_characters.erase(font);
                }
            }
            return;
        }
    }
}
void WindowGL_GLFW::clear_buffer() const
{
    switch_openGL();
    if(s_lineBase)
    {
        delete s_lineBase;
        s_lineBase=nullptr;
    }
    if(s_shaderBase)
    {
        delete s_shaderBase;
        s_shaderBase=nullptr;
    }
    if(s_textShader)
    {
        delete s_textShader;
        s_textShader=nullptr;
    }
    for(auto p:s_face)
    {
        if(p.second)
        {
            FT_Done_Face(p.second);
        }
    }
    s_face.clear();
    if (s_ft)
        FT_Done_FreeType(s_ft);
    s_ft = nullptr;
    for(auto f:s_characters)
    {
        for(auto c:f.second)
        {
            glDeleteTextures(1,&c.second.textureID);
        }
    }
    s_characters.clear();
}


}
}