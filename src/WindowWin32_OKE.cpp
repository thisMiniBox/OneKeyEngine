#include <OneKeyEngine/WindowDefine/WindowWin32_OKE.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace OneKeyEngine
{
namespace WindowDefine
{
    
HBITMAP Window2DWin32::create_canvas(int w, int h)
{
    delete_canvas();
    m_paintHDC = CreateCompatibleDC(m_paintStruct.hdc);
    m_canvas=CreateCompatibleBitmap(m_paintStruct.hdc,w,h);
    SelectObject(m_paintHDC,m_canvas);

    SetGraphicsMode(m_paintHDC, GM_ADVANCED);
    SetBkMode(m_paintHDC, TRANSPARENT);
    return m_canvas;
}

void Window2DWin32::delete_canvas()
{
    if (m_canvas)
        DeleteObject(m_canvas);
    if (m_paintHDC)
        DeleteDC(m_paintHDC);
    m_paintHDC=nullptr;
    m_canvas=nullptr;
}

Window2DWin32::Window2DWin32(Window2DWin32 *parent)
{
    m_lastRefreshTime = get_time();
    m_shouldUpdateTime = m_lastRefreshTime + m_updateTime;
}

Window2DWin32::~Window2DWin32()
{
    delete_canvas();
    close();
}
int Window2DWin32::close()
{
    if(m_windowHandle)
        return DestroyWindow(m_windowHandle);
    m_windowHandle=nullptr;
    return 0;
}
int Window2DWin32::set_size_(int width, int height) const
{
    return set_size_win32(width,height);
}
int Window2DWin32::set_size_win32(int width, int height) const
{
    if(m_windowHandle==nullptr)
        return 0;
    return SetWindowPos(m_windowHandle, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}
int Window2DWin32::get_size_(int *width, int *height) const
{
    RECT rect;
    if(GetClientRect(m_windowHandle,&rect))
    {
        if (width)
            *width = rect.right;
        if (height)
            *height = rect.bottom;
        return 1;
    }
    return 0;
}
int Window2DWin32::set_position(int x, int y) const
{
    if(m_windowHandle==nullptr)
        return 0;
    return SetWindowPos(m_windowHandle, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}
int Window2DWin32::get_position(int *x, int *y) const
{
    if (x == nullptr || y == nullptr)
        return 0;
    RECT rect;
    if (GetWindowRect(m_windowHandle, &rect))
    {
        *x = rect.left;
        *y = rect.top;
        return 1;
    }
    return 0;
}
void Window2DWin32::set_window_title(const std::string &title) const
{
    auto titleW=utf8_to_utf16(title);
    SetWindowTextW(m_windowHandle,titleW.c_str());
}
std::string Window2DWin32::get_window_title() const
{
    std::string title;
    int length= GetWindowTextLengthA(m_windowHandle)+1;
    title.resize(length);
    GetWindowTextA(m_windowHandle,title.data(),length);
    title.pop_back();
    return title;
}
WindowDrawResult Window2DWin32::draw_line_(float x1, float y1, float x2, float y2, LineStyle *style) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_line_HDC(hdc, x1, y1, x2, y2, style);
    // ReleaseDC(m_windowHandle, hdc);
    return result;
}
WindowDrawResult Window2DWin32::draw_pixel_(float x, float y, float colorR, float colorG, float colorB,float colorA)const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_pixel_HDC(hdc,x,y,colorR,colorG,colorB);
    //ReleaseDC(m_windowHandle, hdc);
    return result;
}
WindowDrawResult Window2DWin32::draw_rectangle_(float x, float y, float width, float height,LineStyle* style)const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_rectangle_HDC(hdc,x, y, width, height);
    return result;
}

WindowDrawResult Window2DWin32::draw_polygonal_(float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_polygonal_HDC(hdc,x, y, points, pointsSize,style);
    return result;
}

WindowDrawResult Window2DWin32::draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r, float g, float b, float a) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_fill_polygonal_HDC(hdc,x, y, points, pointsArraySize,r,g,b,a);
    return result;
}

WindowDrawResult Window2DWin32::draw_text_(float x, float y, const char *text,TextStyle *style) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = draw_text_HDC(hdc,x, y, text,style);
    //ReleaseDC(m_windowHandle, hdc);
    return result;
}

WindowDrawResult Window2DWin32::clear_background_(float r, float g, float b,float a) const
{
    if (m_windowHandle == nullptr)
        return WindowDrawResult::WDR_WINDOW_NOT_CREATED;
    HDC hdc = get_current_hdc();
    WindowDrawResult result = clear_background_HDC(hdc,RGB(r*255,g*255,b*255));
    return result;
}

WindowDrawResult Window2DWin32::get_pixel(int x, int y, float *r, float *g, float *b) const
{
    HDC hdc = get_current_hdc();
    if (!hdc)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    if (x > m_windowSize.width || y > m_windowSize.height || x < 0 || y < 0)
        return WindowDrawResult::WDR_PARAMETER_ERROR;
    COLORREF color = GetPixel(hdc, x, y);
    *r = (color & 0xFF)/255.0f;
    *g = ((color >> 8) & 0xFF)/255.0f;
    *b = ((color >> 16) & 0xFF)/255.0f;
    return WindowDrawResult::WDR_SUCCESS;
}
void Window2DWin32::set_transform(const VEC::Mat4 & mat4)
{

    XFORM xform;
    xform.eM11 = mat4[0][0];
    xform.eM12 = mat4[0][1];
    xform.eM21 = mat4[1][0];
    xform.eM22 = mat4[1][1];
    xform.eDx = mat4[0][3];
    xform.eDy = mat4[1][3];
    SetWorldTransform(get_current_hdc(),&xform);
}
VEC::Mat4 Window2DWin32::get_transform() const
{
    XFORM xform;
    HDC hdc = get_current_hdc();
    if (!GetWorldTransform(hdc, &xform))
    {
        return VEC::Mat4(1.0);
    }
    VEC::Mat4 result(1.0);
    result[0][0] = xform.eM11;
    result[0][1] = xform.eM12;
    result[1][0] = xform.eM21;
    result[1][1] = xform.eM22;
    result[3][0] = xform.eDx;
    result[3][1] = xform.eDy;
    return result;
}
void Window2DWin32::set_mouse_position(const VEC::Vec2 &pos)
{
    SetCursorPos(pos.x, pos.y);
    POINT point;
    point.x=pos.x;
    point.y=pos.y;
    ScreenToClient(m_windowHandle,&point);
    mousePosition=VEC::Vec2(point.x,point.y);
}
void Window2DWin32::set_mode(ModeType mode, ModeValue value) const
{
    switch (mode)
    {
    case ModeType::ShowCursor:
        switch (value)
        {
        case ModeValue::True:
            ShowCursor(true);
            break;
        case ModeValue::False:
            ShowCursor(false);
            break;
        default:
            break;
        }
        break;
    case ModeType::CaptureCursor:
        switch (value)
        {
        case ModeValue::True:
            SetCapture(m_windowHandle);
            break;
        case ModeValue::False:
            ReleaseCapture();
            break;
        default:
            break;
        }
        break;
    case ModeType::MaxWindow:
        switch (value)
        {
        case ModeValue::True:
            ShowWindow(m_windowHandle,SW_MAXIMIZE);
            break;
        case ModeValue::False:
            ShowWindow(m_windowHandle,SW_NORMAL);
            break;
        default:
            break;
        } 
        break;
    case ModeType::MinWindow:
        switch (value)
        {
        case ModeValue::True:
            ShowWindow(m_windowHandle,SW_MINIMIZE);
            break;
        case ModeValue::False:
            ShowWindow(m_windowHandle, SW_NORMAL);
            break;
        default:
            break;
        } 
        break;
    case ModeType::FullScreen:
    {
        static HWND pParentWndSave = NULL;        // 父窗口句柄
        static DWORD dwWindowStyleSave = 0;       // 窗口风格
        static RECT rcWndRectSave = {0, 0, 0, 0}; // 窗口位置
        static BOOL bEnterFullView = FALSE;       // 进入全屏？

        switch (value)
        {
        case ModeValue::True:
            if (!bEnterFullView) // 非全屏模式
            {
                dwWindowStyleSave = GetWindowLong(m_windowHandle, GWL_STYLE);                         // 保存窗口风格
                GetWindowRect(m_windowHandle, &rcWndRectSave);                                        // 保存窗口位置
                pParentWndSave = SetParent(m_windowHandle, NULL);                                     // 保存父窗口句柄/设置父窗口
                HWND hDesk;
                RECT rc;
                hDesk = GetDesktopWindow();
                GetWindowRect(hDesk, &rc);
                SetWindowLong(m_windowHandle, GWL_STYLE, WS_BORDER);
                SetWindowPos(m_windowHandle, HWND_TOPMOST, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);
                bEnterFullView = TRUE;                                           // 进入全屏模式
            }
            break;
        case ModeValue::False:
            if (bEnterFullView) // 是全屏模式
            {
                SetWindowLong(m_windowHandle, GWL_STYLE, dwWindowStyleSave); // 恢复窗口风格
                SetParent(m_windowHandle, pParentWndSave);                   // 恢复父窗口句柄
                POINT ptLT = {rcWndRectSave.left, rcWndRectSave.top};
                ScreenToClient(m_windowHandle, &ptLT);
                MoveWindow(m_windowHandle, ptLT.x, ptLT.y,
                           rcWndRectSave.right - rcWndRectSave.left, rcWndRectSave.bottom - rcWndRectSave.top,
                           TRUE);       // 恢复原始位置
                bEnterFullView = FALSE; // 退出全屏模式
            }
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
int Window2DWin32::processing_event()
{
    if (get_event())
    {
        execution_event();
        return 1;
    }
    return 0;
}
int Window2DWin32::should_draw()
{
    while (1)
    {
        if(get_event()<2)
        {
            execution_event();
            if (m_message.hwnd == m_windowHandle && m_message.message == WM_PAINT)
            {
                return 1;
            }
            if(m_message.message==WM_QUIT)
            {
                break;
            }
        }
        else
        {
            Sleep(1);
        }
    }
    return 0;
}
float Window2DWin32::set_frame_rate(float frameRate)
{
    if (frameRate == 0)
    {
        m_updateTime = -1;
        KillTimer(m_windowHandle,__WINDOW_UPDATE_TIMER);
        return 0;
    }
    if(m_updateTime==1.0 / frameRate)
        return m_updateTime;
    KillTimer(m_windowHandle,__WINDOW_UPDATE_TIMER);
    m_updateTime = 1.0 / frameRate;
    SetTimer(m_windowHandle,__WINDOW_UPDATE_TIMER,m_updateTime,NULL);
    return m_updateTime;
}
void Window2DWin32::refresh_screen() const
{
    InvalidateRect(m_windowHandle,nullptr,false);
    //UpdateWindow(m_windowHandle);
}

bool Window2DWin32::set_clipboard_text(const std::string &text) const
{
    // 打开剪贴板
    if (!OpenClipboard(m_windowHandle))
        return false;

    // 清空剪贴板内容
    EmptyClipboard();

    // 将UTF-8字符串转换为宽字符（Unicode）
    int wide_length = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    if (wide_length == 0)
    {
        CloseClipboard();
        return false;
    }

    std::wstring wide_text(wide_length, 0);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wide_text[0], wide_length);

    // 分配内存并设置剪贴板数据
    HGLOBAL hClipboardData;
    size_t size = (wide_text.length() + 1) * sizeof(wchar_t);
    hClipboardData = GlobalAlloc(GMEM_MOVEABLE, size);
    if (hClipboardData == nullptr)
    {
        CloseClipboard();
        return false;
    }

    wchar_t *clipboardText = static_cast<wchar_t *>(GlobalLock(hClipboardData));
    if (clipboardText == nullptr)
    {
        GlobalFree(hClipboardData);
        CloseClipboard();
        return false;
    }

    // 复制数据到剪贴板内存
    memcpy(clipboardText, wide_text.c_str(), size);

    GlobalUnlock(hClipboardData);
    SetClipboardData(CF_UNICODETEXT, hClipboardData);
    CloseClipboard();

    return true;
}
std::string Window2DWin32::get_clipboard_text() const
{
    // 打开剪贴板
    if (!OpenClipboard(m_windowHandle))
        return "";

    // 获取剪贴板中数据句柄
    HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
    if (hClipboardData == nullptr)
    {
        CloseClipboard();
        return "";
    }

    // 锁定数据句柄并获取数据指针
    wchar_t* clipboardText = static_cast<wchar_t*>(GlobalLock(hClipboardData));
    if (clipboardText == nullptr)
    {
        CloseClipboard();
        return "";
    }

    // 将宽字符转换为UTF-8字符串
    int utf8_length = WideCharToMultiByte(CP_UTF8, 0, clipboardText, -1, nullptr, 0, nullptr, nullptr);
    if (utf8_length == 0)
    {
        GlobalUnlock(hClipboardData);
        CloseClipboard();
        return "";
    }

    std::string utf8_text(utf8_length, 0);
    WideCharToMultiByte(CP_UTF8, 0, clipboardText, -1, &utf8_text[0], utf8_length, nullptr, nullptr);

    // 释放句柄和关闭剪贴板
    GlobalUnlock(hClipboardData);
    CloseClipboard();

    return utf8_text;
}
KeyCode Window2DWin32::virtual_key_to_key_code(WPARAM wParam)
{
    switch (wParam) 
    {
        case VK_LBUTTON: return KeyCode::MouseLeftButton;
        case VK_RBUTTON: return KeyCode::MouseRightButton;
        case VK_CANCEL: return KeyCode::CanCel;
        case VK_MBUTTON: return KeyCode::MouseMiddleButton;
        case VK_XBUTTON1: return KeyCode::MouseButtonX1;
        case VK_XBUTTON2: return KeyCode::MouseButtonX2;
        case VK_BACK: return KeyCode::Backspace;
        case VK_TAB: return KeyCode::Tab;
        case VK_CLEAR: return KeyCode::Claer;
        case VK_RETURN: return KeyCode::Enter;
        case VK_SHIFT: return KeyCode::Shift;
        case VK_CONTROL: return KeyCode::CTRL;
        case VK_MENU: return KeyCode::Alt;
        case VK_PAUSE: return KeyCode::Pause;
        case VK_CAPITAL: return KeyCode::Capital;
        case VK_ESCAPE: return KeyCode::Escape;
        case VK_SPACE: return KeyCode::Space;
        case VK_PRIOR: return KeyCode::PageUp;
        case VK_NEXT: return KeyCode::PageDown;
        case VK_END: return KeyCode::End;
        case VK_HOME: return KeyCode::Home;
        case VK_LEFT: return KeyCode::Left;
        case VK_UP: return KeyCode::Up;
        case VK_RIGHT: return KeyCode::Right;
        case VK_DOWN: return KeyCode::Down;
        case VK_SELECT: return KeyCode::Select;
        case VK_PRINT: return KeyCode::Print;
        case VK_EXECUTE: return KeyCode::Execute;
        case VK_SNAPSHOT: return KeyCode::Snapshout;
        case VK_INSERT: return KeyCode::Insert;
        case VK_DELETE: return KeyCode::Delete;
        case VK_HELP: return KeyCode::Help;
        case '0': return KeyCode::A0;
        case '1': return KeyCode::A1;
        case '2': return KeyCode::A2;
        case '3': return KeyCode::A3;
        case '4': return KeyCode::A4;
        case '5': return KeyCode::A5;
        case '6': return KeyCode::A6;
        case '7': return KeyCode::A7;
        case '8': return KeyCode::A8;
        case '9': return KeyCode::A9;
        case 'A': return KeyCode::A;
        case 'B': return KeyCode::B;
        case 'C': return KeyCode::C;
        case 'D': return KeyCode::D;
        case 'E': return KeyCode::E;
        case 'F': return KeyCode::F;
        case 'G': return KeyCode::G;
        case 'H': return KeyCode::H;
        case 'I': return KeyCode::I;
        case 'J': return KeyCode::J;
        case 'K': return KeyCode::K;
        case 'L': return KeyCode::L;
        case 'M': return KeyCode::M;
        case 'N': return KeyCode::N;
        case 'O': return KeyCode::O;
        case 'P': return KeyCode::P;
        case 'Q': return KeyCode::Q;
        case 'R': return KeyCode::R;
        case 'S': return KeyCode::S;
        case 'T': return KeyCode::T;
        case 'U': return KeyCode::U;
        case 'V': return KeyCode::V;
        case 'W': return KeyCode::W;
        case 'X': return KeyCode::X;
        case 'Y': return KeyCode::Y;
        case 'Z': return KeyCode::Z;
        case VK_LWIN: return KeyCode::LeftWin;
        case VK_RWIN: return KeyCode::RightWin;
        case VK_APPS: return KeyCode::Apps;
        case VK_SLEEP: return KeyCode::Sleep;
        case VK_NUMPAD0: return KeyCode::Numpad0;
        case VK_NUMPAD1: return KeyCode::Numpad1;
        case VK_NUMPAD2: return KeyCode::Numpad2;
        case VK_NUMPAD3: return KeyCode::Numpad3;
        case VK_NUMPAD4: return KeyCode::Numpad4;
        case VK_NUMPAD5: return KeyCode::Numpad5;
        case VK_NUMPAD6: return KeyCode::Numpad6;
        case VK_NUMPAD7: return KeyCode::Numpad7;
        case VK_NUMPAD8: return KeyCode::Numpad8;
        case VK_NUMPAD9: return KeyCode::Numpad9;
        case VK_MULTIPLY: return KeyCode::Multiple;
        case VK_ADD: return KeyCode::Add;
        case VK_SEPARATOR: return KeyCode::Separator;
        case VK_SUBTRACT: return KeyCode::Subtract;
        case VK_DECIMAL: return KeyCode::Dcimal;
        case VK_DIVIDE: return KeyCode::Divide;
        case VK_F1: return KeyCode::F1;
        case VK_F2: return KeyCode::F2;
        case VK_F3: return KeyCode::F3;
        case VK_F4: return KeyCode::F4;
        case VK_F5: return KeyCode::F5;
        case VK_F6: return KeyCode::F6;
        case VK_F7: return KeyCode::F7;
        case VK_F8: return KeyCode::F8;
        case VK_F9: return KeyCode::F9;
        case VK_F10: return KeyCode::F10;
        case VK_F11: return KeyCode::F11;
        case VK_F12: return KeyCode::F12;
        case VK_F13: return KeyCode::F13;
        case VK_F14: return KeyCode::F14;
        case VK_F15: return KeyCode::F15;
        case VK_F16: return KeyCode::F16;
        case VK_F17: return KeyCode::F17;
        case VK_F18: return KeyCode::F18;
        case VK_F19: return KeyCode::F19;
        case VK_F20: return KeyCode::F20;
        case VK_F21: return KeyCode::F21;
        case VK_F22: return KeyCode::F22;
        case VK_F23: return KeyCode::F23;
        case VK_F24: return KeyCode::F24;
        case VK_LSHIFT: return KeyCode::LeftShift;
        case VK_RSHIFT: return KeyCode::RightShift;
        case VK_LCONTROL: return KeyCode::LeftCtrl;
        case VK_RCONTROL: return KeyCode::RightCtrl;
        case VK_LMENU: return KeyCode::LeftAlt;
        case VK_RMENU: return KeyCode::RightAlt;
        default: return KeyCode::None;
    }
}
void Window2DWin32::quaternion_to_matrix(const VEC::Quaternion &q, XFORM &xForm)
{
    float ww = q.w * q.w;
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;

    xForm.eM11 = ww + xx - yy - zz;
    xForm.eM12 = 2 * (xy + wz);
    xForm.eM21 = 2 * (xy - wz);
    xForm.eM22 = ww - xx + yy - zz;
}
// double Window2DWin32::get_time() const
// {
//     LARGE_INTEGER time;
//     if (!QueryPerformanceCounter(&time))
//     {
//         return -1;
//     }

//     return (double)time.QuadPart / frequency.QuadPart;
// }
int Window2DWin32::get_event()
{
    // if(m_updateTime>0)
    // {
    //     auto nowTime = get_time();
    //     if (nowTime > m_shouldUpdateTime)
    //     {
    //         m_shouldUpdateTime = nowTime + m_updateTime;
    //         refresh_screen();
    //     }
    // }
    if(PeekMessage(&m_message, NULL, 0, 0, PM_REMOVE))
    {
        if (m_message.message == WM_QUIT)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 2;
}

int Window2DWin32::execution_event()
{
    TranslateMessage(&m_message);
    DispatchMessageW(&m_message);
    return 1;
}
LRESULT CALLBACK Window2DWin32::WindowCAllback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window2DWin32 *mainWind = reinterpret_cast<Window2DWin32 *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    switch (message)
    {
    case WM_KEYDOWN:
    {
        mainWind->on_keyDown(wParam, lParam);
        break;
    }

    case WM_KEYUP:
    {
        mainWind->on_keyUp(wParam, lParam);
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    case WM_MOUSEMOVE:
    {
        mainWind->on_mouse_callback(message, wParam, lParam);
        break;
    }
    case WM_CHAR:
    {
        mainWind->on_input(wParam,lParam);
        break;
    }

    // case WM_IME_CHAR:
    // {
    //     mainWind->on_input(wParam, lParam);
    //     break;
    // }
    case WM_TIMER:
    {
        if(wParam==__WINDOW_UPDATE_TIMER)
        {
            mainWind->refresh_screen();
        }
        break;
    }
    case WM_PAINT:
    {
        mainWind->on_paint();
        break;
    }
    case WM_SIZE:
    {
        mainWind->on_size(wParam,lParam);
        break;
    }
    case WM_SETFOCUS:
    {
        mainWind->m_inFocus=true;
        break;
    }
    case WM_KILLFOCUS:
    {
        mainWind->m_inFocus=false;
        break;
    }
    case WM_CREATE:
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        mainWind = (Window2DWin32 *)pcs->lpCreateParams;
        ::SetWindowLongPtrW(
            hWnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(mainWind));
        POINT mousePos;
        GetCursorPos(&mousePos);
        mainWind->mousePosition.x = mousePos.x;
        mainWind->mousePosition.y = mousePos.y;
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
int Window2DWin32::on_paint()
{
    auto nowTime = get_time();
    m_realUpdateTime = nowTime - m_lastRefreshTime;
    m_lastRefreshTime = nowTime;
    begin_paint();
    if (m_drawCallback)
        m_drawCallback(this);
    end_paint();
    return 0;
}
int Window2DWin32::on_keyDown(WPARAM wParam, LPARAM lParam)
{
    if (m_keyDownCallback)
        m_keyDownCallback(this, virtual_key_to_key_code(wParam));
    return 0;
}

int Window2DWin32::on_keyUp(WPARAM wParam, LPARAM lParam)
{
    if (m_keyUpCallback)
        m_keyUpCallback(this, virtual_key_to_key_code(wParam));
    return 0;
}
static std::string convert_to_UTF8(const std::wstring& utf16Char) {
    std::vector<char> utf8Str(4*utf16Char.size()); // 一个UTF-16字符最多可以转换为4个UTF-8字节
    int length = WideCharToMultiByte(CP_UTF8, 0, utf16Char.c_str(), utf16Char.size(), utf8Str.data(), utf8Str.size(), NULL, NULL);
    return std::string(utf8Str.data(), length);
}
int Window2DWin32::on_input(WPARAM wParam, LPARAM lParam)
{
    // static wchar_t charBuffer;
    // if(wParam>128)
    // {
    //     if(charBuffer!=0)
    //     {
    //         std::cout << "第二个字符：" << std::hex << wParam << std::endl;
    //         charBuffer|=(wParam<<8);
    //         if(m_inputCallback)
    //             m_inputCallback(this,charBuffer);
    //         charBuffer=0;
    //         return 0;
    //     }   
    //     else
    //     {
    //         std::cout << "第一个字符：" << std::hex << wParam << std::endl;
    //         charBuffer=wParam;
    //         return 1;
    //     }
    // }

    if(m_inputCallback)
        m_inputCallback(this,wParam);
    return 0;
}

int Window2DWin32::on_size(WPARAM wParam, LPARAM lParam)
{
    //std::cout<<"onSize"<<std::endl;
    m_windowSize=VEC::Size(LOWORD(lParam),HIWORD(lParam));
    if(m_canvas)
        create_canvas(LOWORD(lParam),HIWORD(lParam));
    if(m_windowSizeCallback)
        m_windowSizeCallback(this,m_windowSize.width,m_windowSize.height);
    else
        return DefWindowProc(m_windowHandle,WM_SIZE,wParam,lParam);
    return 0;
}

int Window2DWin32::on_mouse_callback(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        if (m_keyDownCallback)
            m_keyDownCallback(this, KeyCode::MouseLeftButton);
        if (m_mouseButtonDown)
            m_mouseButtonDown(this, KeyCode::MouseLeftButton);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        if (m_keyDownCallback)
            m_keyDownCallback(this, KeyCode::MouseRightButton);
        if (m_mouseButtonDown)
            m_mouseButtonDown(this, KeyCode::MouseRightButton);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        if (m_keyDownCallback)
            m_keyDownCallback(this, KeyCode::MouseMiddleButton);
        if (m_mouseButtonDown)
            m_mouseButtonDown(this, KeyCode::MouseMiddleButton);
        break;
    }
    case WM_MBUTTONUP:
    {
        if (m_keyUpCallback)
            m_keyUpCallback(this, KeyCode::MouseMiddleButton);
        if (m_mouseButtonUp)
            m_mouseButtonUp(this, KeyCode::MouseMiddleButton);
        break;
    }
    case WM_LBUTTONUP:
    {
        if (m_keyUpCallback)
            m_keyUpCallback(this, KeyCode::MouseLeftButton);
        if (m_mouseButtonUp)
            m_mouseButtonUp(this, KeyCode::MouseLeftButton);
        break;
    }
    case WM_RBUTTONUP:
    {
        if (m_keyUpCallback)
            m_keyUpCallback(this, KeyCode::MouseRightButton);
        if (m_mouseButtonUp)
            m_mouseButtonUp(this, KeyCode::MouseRightButton);
        break;
    }
    case WM_MOUSEMOVE:
    {
        mousePosition.x = LOWORD(lParam);
        mousePosition.y = HIWORD(lParam);
        if (m_mouseMoveCallback)
        {
            m_mouseMoveCallback(this, mousePosition.x, mousePosition.y);
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        short w=HIWORD(wParam);
        if (m_mouseWheelCallbck)
            m_mouseWheelCallbck(this, 0, (float)w / (WHEEL_DELTA));
        break;
    }
    case WM_MOUSEHWHEEL:
    {
        short w=HIWORD(wParam);
        if (m_mouseWheelCallbck)
            m_mouseWheelCallbck(this, (float)w / (WHEEL_DELTA), 0);
        break;
    }
    default:
        break;
    }
    return 0;
}

WindowDrawResult Window2DWin32::draw_line_HDC(HDC hdc, int x1, int y1, int x2, int y2, LineStyle *style)const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    if (style)
    {
        HPEN hpen;
        if (style->spacing == nullptr)
        {
            hpen = CreatePen(PS_SOLID, 0, RGB(style->color.r*255, style->color.g*255, style->color.b*255));
        }
        else
        {
            hpen = CreatePen(PS_DASH, 0, RGB(style->color.r*255, style->color.g*255, style->color.b*255));
        }
        if(hpen==nullptr)
        {
            return WindowDrawResult::WDR_WIN32_PEN_CREATED_FAILED;
        }
        HPEN oldhpen = (HPEN)SelectObject(hdc, hpen);
        auto result= MoveToEx(hdc, x1, y1, NULL);
        result =LineTo(hdc, x2, y2);
        SelectObject(hdc, oldhpen);
        DeleteObject(hpen);
        if(!result)return WindowDrawResult::WDR_FALSE;
    }
    else
    {
        auto result= MoveToEx(hdc, x1, y1, NULL);
        if(!result)return WindowDrawResult::WDR_FALSE;
        result =LineTo(hdc, x2, y2);
    }
    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult Window2DWin32::draw_pixel_HDC(HDC hdc, int x, int y, float colorR, float colorG, float colorB)const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    COLORREF color;
    if(colorR>1||colorG>1||colorB>1)
    {
        color=RGB(colorR,colorG,colorB);
    }
    else
    {
        color=RGB(colorR*255,colorG*255,colorB*255);
    }
    if(SetPixel(hdc,x,y,color)==color)
    {
        return WindowDrawResult::WDR_SUCCESS;
    }
    return WindowDrawResult::WDR_FALSE;
}
WindowDrawResult Window2DWin32::draw_rectangle_HDC(HDC hdc, int x, int y, int width, int height, LineStyle *style)const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    RECT rectangle={x,y,x+width,y+height};
    HBRUSH rectLine;
    if(style)
    {
        auto& color=style->color;
        rectLine=CreateSolidBrush(RGB(color.r,color.g,color.b));
    }
    else
    {
        rectLine=CreateSolidBrush(RGB(0,0,0));
    }
    if(rectLine==nullptr)
        return WindowDrawResult::WDR_WIN32_BRUSH_CREATE_FAILED;
    auto result=FrameRect(hdc,&rectangle,rectLine);
    DeleteObject(rectLine);
    if(result)
    {
        return WindowDrawResult::WDR_SUCCESS;
    }
    return WindowDrawResult::WDR_FALSE;
}
WindowDrawResult Window2DWin32::clear_background_HDC(HDC hdc, COLORREF color)const
{
    if(!hdc)return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    RECT windowSize;
    if(!GetClientRect(m_windowHandle,&windowSize))return WindowDrawResult::WDR_FALSE;
    HBRUSH brush=CreateSolidBrush(color);
    if(!brush)return WindowDrawResult::WDR_WIN32_BRUSH_CREATE_FAILED;
    FillRect(hdc,&windowSize,brush);
    DeleteObject(brush);
    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult Window2DWin32::draw_text_HDC(HDC hdc, int x, int y, const char *text, const TextStyle *style) const
{
    if (hdc == nullptr)
        return WindowDrawResult::WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE;
    if (text == nullptr)
        return WindowDrawResult::WDR_PARAMETER_ERROR;
    if (style == nullptr)
    {
        TextStyle textStyle;
        return draw_text_HDC(hdc, x, y, text, &textStyle);
    }

    HFONT hFont = nullptr;
    if (style)
    {
        hFont = font_cache.get_font(style->font);
        if (hFont == nullptr)
            return WindowDrawResult::WDR_WIN32_FONT_CREATE_FAILED;
    }
    HFONT oldFont = nullptr;
    if (hFont)
    {
        oldFont = (HFONT)SelectObject(hdc, hFont);
    }

    if (style)
    {
        SetTextColor(hdc, style->color.to_win32_gdi_color());
    }
    else
    {
        SetTextColor(hdc, RGB(0, 0, 0));
    }
    UINT format = 0;
    if (style)
    {
        switch (style->horizontalAlign)
        {
        case TextAlign::Left:
            format |= DT_LEFT;
            break;
        case TextAlign::Center:
            format |= DT_CENTER;
            break;
        case TextAlign::Right:
            format |= DT_RIGHT;
            break;
        }

        switch (style->verticalAlign)
        {
        case TextAlign::Top:
            format |= DT_TOP;
            break;
        case TextAlign::Center:
            format |= DT_VCENTER;
            break;
        case TextAlign::Bottom:
            format |= DT_BOTTOM;
            break;
        }

        if (style->wordWrap)
        {
            format |= DT_WORDBREAK;
        }

        if (style->singleLine)
        {
            format |= DT_SINGLELINE;
        }
    }
    else
    {
        format = DT_LEFT | DT_TOP;
    }

    // Convert UTF-8 text to wide string
    int wlen = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    if (wlen == 0)
    {
        return WindowDrawResult::WDR_FALSE;
    }
    std::wstring wtext(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, text, -1, &wtext[0], wlen);

    RECT rect = {x, y, x, y};
    if (style->bounds.x <= 0)
    {
        DrawTextW(hdc, wtext.c_str(), -1, &rect, format | DT_CALCRECT);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        rect = {x, y, x + width, y + height};
    }
    else
    {
        rect = {x, y, x + (long)style->bounds.width, y + (long)style->bounds.height};
    }
    int result = DrawTextW(hdc, wtext.c_str(), -1, &rect, format);

    if (hFont)
    {
        SelectObject(hdc, oldFont);
    }

    return (result > 0) ? WindowDrawResult::WDR_SUCCESS : WindowDrawResult::WDR_FALSE;
}
WindowDrawResult Window2DWin32::draw_polygonal_HDC(HDC hdc, float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
{
    std::vector<POINT> point(pointsSize+1);
    for (uint32_t i = 0; i < pointsSize; ++i)
    {
        POINT p = { static_cast<LONG>(*points + x), static_cast<LONG>(*(points + 1) + y) };
        point[i] = p;
        points += 2;
    }
    point[pointsSize]=point[0];
    HPEN hPen=nullptr;
    HPEN hOldPen=nullptr;
    if (style)
    {
        if (style->spacing)
            hPen = CreatePen(PS_DOT, style->width, style->color.to_win32_gdi_color()); // 蓝色边框
        else
            hPen = CreatePen(PS_SOLID, style->width, style->color.to_win32_gdi_color()); // 蓝色边框
        hOldPen = (HPEN)SelectObject(hdc, hPen);
    }
    Polyline(hdc,point.data(),pointsSize+1);
    if(style)
    {
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }

    return WindowDrawResult::WDR_SUCCESS;
}
WindowDrawResult Window2DWin32::draw_fill_polygonal_HDC(HDC hdc, float x, float y, float *points, uint32_t pointsSize, float r, float g, float b, float a) const
{
    std::vector<POINT> point(pointsSize);
    for (uint32_t i = 0; i < pointsSize; ++i)
    {
        POINT p = { static_cast<LONG>(*(points) + x), static_cast<LONG>(*(points + 1) + y) };
        point[i] = p;
        points += 2;
    }
    
    // 创建颜色并设置透明度
    COLORREF fillColor = RGB(static_cast<BYTE>(r * 255), static_cast<BYTE>(g * 255), static_cast<BYTE>(b * 255));
    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // 使用透明的画笔
    HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // 设置混合模式以支持透明度
    BLENDFUNCTION blendFunction = { AC_SRC_OVER, 0, static_cast<BYTE>(a * 255), 0 };
    AlphaBlend(hdc, 0, 0, 0, 0, hdc, 0, 0, 0, 0, blendFunction);

    // 绘制填充多边形
    Polygon(hdc, point.data(), pointsSize);

    // 恢复旧的画刷和画笔
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);

    return WindowDrawResult::WDR_SUCCESS;
}
HDC Window2DWin32::get_current_hdc() const
{
    if(m_paintHDC)return m_paintHDC;
    return m_paintStruct.hdc;
}
HWND Window2DWin32::get_handle() const
{
    return m_windowHandle;
}
std::string Window2DWin32::get_font_path(const std::string &fontName) const
{
    HKEY hKey;
    // 打开注册表键
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cerr << "ERROR:: Could not open registry key" << std::endl;
        return "";
    }

    char value[256];          // 用于存储注册表中的键名（即字体名称）
    DWORD value_length = 256; // 键名长度
    DWORD type = 0;           // 注册表值的类型（通常为REG_SZ）
    DWORD index = 0;          // 用于枚举注册表值的索引
    char data[256];           // 用于存储注册表中的值（即字体文件名）
    DWORD data_length = 256;  // 值的长度

    // 枚举注册表中的所有值
    while (RegEnumValueA(hKey, index, value, &value_length, nullptr, &type, reinterpret_cast<LPBYTE>(data), &data_length) == ERROR_SUCCESS) {
        // 如果找到与指定字体名称匹配的注册表值
        if (std::string(value).find(fontName) != std::string::npos) {
            // 构建完整的字体文件路径
            std::string fontPath = "C:\\Windows\\Fonts\\" + std::string(data);
            // 关闭注册表键
            RegCloseKey(hKey);
            // 返回找到的字体文件路径
            return fontPath;
        }
        // 重置长度变量以用于下一次枚举
        value_length = 256;
        data_length = 256;
        // 增加索引以枚举下一个注册表值
        index++;
    }

    // 如果没有找到匹配的字体名称，关闭注册表键并返回错误信息
    RegCloseKey(hKey);
    std::cerr << "ERROR:: Could not find font file path for " << fontName << std::endl;
    return "";
}
HDC Window2DWin32::begin_paint()
{
    HDC hdc= BeginPaint(m_windowHandle,&m_paintStruct);
    if(!m_paintHDC)
    {
        create_canvas(m_windowSize.width,m_windowSize.height);
    }
    return hdc;
}
void Window2DWin32::end_paint()
{
    BitBlt(m_paintStruct.hdc, 0, 0, m_windowSize.width,m_windowSize.height, m_paintHDC, 0, 0, SRCCOPY);
    EndPaint(m_windowHandle,&m_paintStruct);
}
WindowCreateResult Window2DWin32::create_window(const std::string& titleName,int x,int y,int width,int height,WindowBase* parentWindow)
{
    if (m_windowHandle) {
        return WindowCreateResult::WCR_WINDOW_CREATED;
    }
    
    std::wstring titleNameW=utf8_to_utf16(titleName);
    HWND parent;
    auto detection = parentWindow->is_derived_class<Window2DWin32>(parentWindow);
    if (detection == nullptr)
    {
        parent = nullptr;
    }
    else
    {
        parent = detection->m_windowHandle;
    }
    HINSTANCE hInstance=GetModuleHandleA(nullptr);
    WNDCLASSEXW windowClass = {};
    if (GetClassInfoExW(hInstance, c_windowClassName, &windowClass) == 0) {
        WNDCLASSEXW wc = { 0 };
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowCAllback;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = c_windowClassName;
        wc.hbrBackground = NULL;

        // 注册窗口类
        if (!RegisterClassExW(&wc)) {
            return WindowCreateResult::WCR_WIN32_WINODW_CLASS_REGISTRATION_FAILED;
        }
    }



    if (parent) {
        m_windowHandle = CreateWindowW(
            c_windowClassName,
            titleNameW.c_str(),
            WS_CHILD | WS_VISIBLE,
            x, y, width, height,
            parent, NULL,
            hInstance, this);
    } else {
        m_windowHandle = CreateWindowW(
            c_windowClassName,
            titleNameW.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            x, y, width, height,
            NULL, NULL,
            hInstance, this);
    }
    if (!m_windowHandle) {
        return WindowCreateResult::WCR_WINDOW_CREATE_FAILED;
    }
    return WindowCreateResult::WCR_SUCCESS;
}



//win32OpenGL窗口定义******************************************************

#ifdef __glad_h_

GL::ShaderGL *WindowOpenGLWin32::get_line_shader() const
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

GL::ShaderGL *WindowOpenGLWin32::get_text_shader() const
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

FT_Face WindowOpenGLWin32::get_free_type_font(const TextStyle *style) const
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

Character* WindowOpenGLWin32::get_char_character(char32_t ch, FT_Face ft_font) const
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

void WindowOpenGLWin32::clear_char_texture(uint32_t id) const
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

bool WindowOpenGLWin32::inited_glad() const
{
    if(glCreateBuffers)
        return true;
    return false;
}

GL::LineStrip *WindowOpenGLWin32::get_line_base() const
{
    if(s_lineBase)return s_lineBase;
    s_lineBase=new GL::LineStrip({{0,0,0},{1,0,0}});
    return s_lineBase;
}

int WindowOpenGLWin32::on_paint()
{
    switch_openGL();
    if(m_drawCallback)
        m_drawCallback(this);
    auto nowTime=get_time();
    m_realUpdateTime = nowTime - m_lastRefreshTime;
    m_lastRefreshTime = nowTime;
    swap_buffer();
    ValidateRect(m_windowHandle, NULL);
    return 0;
}

int WindowOpenGLWin32::on_size(WPARAM wParam, LPARAM lParam)
{
    switch_openGL();
    m_windowSize = VEC::Size(LOWORD(lParam), HIWORD(lParam));
    if (inited_glad())
        glViewport(0, 0, m_windowSize.width, m_windowSize.height);
    if (m_windowSizeCallback)
        m_windowSizeCallback(this, m_windowSize.width, m_windowSize.height);

    else
        return DefWindowProc(m_windowHandle, WM_SIZE, wParam, lParam);
    return 0;
}
#ifdef FT2BUILD_H_
//FT_Library WindowOpenGLWin32::s_ft=nullptr;
// std::unordered_map<Font,FT_Face> WindowOpenGLWin32::s_face;
// std::unordered_map<FT_Face,std::unordered_map<GLuint, Character>> WindowOpenGLWin32::s_characters;
#endif

WindowOpenGLWin32::WindowOpenGLWin32()
{
}

WindowOpenGLWin32::~WindowOpenGLWin32() 
{
    clear_buffer();
    wglMakeCurrent(NULL, NULL);
    if(m_hglrc)
        wglDeleteContext(m_hglrc);
    if(m_hdc)
        ReleaseDC(m_windowHandle,m_hdc);
    if(s_lineBase)
    {
        delete s_lineBase;
    }
    if(s_shaderBase)
    {
        delete s_shaderBase;
    }
    if(s_textShader)
    {
        delete s_textShader;
    }
}

WindowCreateResult WindowOpenGLWin32::create_window(const std::string& titleName,int x,int y,int width,int height,WindowBase* parentWindow)
{
    auto result = Window2DWin32::create_window(titleName, x, y, width, height,parentWindow);
    if (result != WindowCreateResult::WCR_SUCCESS)
    {
        std::cerr << "Failed to create base window." << std::endl;
        return result;
    }

    HDC hdc = GetDC(m_windowHandle);
    if (!hdc)
    {
        std::cerr << "Failed to get device context." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }
    m_hdc = hdc;

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0)
    {
        std::cerr << "Failed to choose pixel format." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        std::cerr << "Failed to set pixel format." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }

    m_hglrc = wglCreateContext(hdc);
    if (!m_hglrc)
    {
        std::cerr << "Failed to create OpenGL context." << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }

    if (!wglMakeCurrent(hdc, m_hglrc))
    {
        std::cerr << "OpenGL句柄创建失败" << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }
    if(!switch_openGL())
    {
        std::cout << "切换OpenGL失败" << std::endl;
    }
    auto glGetStringPtr = (const GLubyte *(*)(GLenum))wglGetProcAddress("glGetString");
    if (glGetStringPtr == nullptr)
    {
        std::cerr << "使用wglGetProcAddress加载glGetString失败" << std::endl;
    }
    else
    {
        std::cerr << "使用wglGetProcAddress加载glGetString成功" << std::endl;
        const char *version = (const char *)glGetStringPtr(GL_VERSION);
        std::cout << "OpenGL version: " << version << std::endl;
    }
    std::cout << "设备句柄: " << hdc << std::endl;
    std::cout << "OpenGL句柄: " << m_hglrc << std::endl;
    if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
    {
        std::cout << "GLAD初始化失败" << std::endl;
        return WindowCreateResult::WCR_FALSE;
    }
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     std::cout << "Failed to initialize GLAD" << std::endl;

    // }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    return WindowCreateResult::WCR_SUCCESS;
}

HDC WindowOpenGLWin32::get_current_hdc() const
{
    return m_hdc;
}

bool WindowOpenGLWin32::switch_openGL() const
{
    if (m_hglrc == nullptr)
        return false;
    static HGLRC last=nullptr;
    if(m_hglrc==last)
        return false;
    last=m_hglrc;
    return wglMakeCurrent(m_hdc, m_hglrc);
}

void WindowOpenGLWin32::swap_buffer() const
{
    SwapBuffers(m_hdc);
}

void WindowOpenGLWin32::clear_buffer() const
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

WindowDrawResult WindowOpenGLWin32::draw_line_(float x1, float y1, float x2, float y2, LineStyle *style) const
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
    shader->set_mat4("transform", m_mat4*tran);

    get_line_base()->draw(*shader);

    return WindowDrawResult::WDR_SUCCESS;
}

WindowDrawResult WindowOpenGLWin32::draw_pixel_(float x, float y, float r, float g, float b, float a)const
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

WindowDrawResult WindowOpenGLWin32::draw_rectangle_(float x, float y, float width, float height,LineStyle* style)const
{
    draw_line_(x,y,x+width,y,style);
    draw_line_(x+width,y,x+width,y+height);
    draw_line_(x+width,y+height,x,y+height);
    draw_line_(x,y,x,y+height);
    return WindowDrawResult::WDR_SUCCESS;
}

WindowDrawResult WindowOpenGLWin32::draw_polygonal_(float x, float y, float *points, uint32_t pointsSize, LineStyle *style) const
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

WindowDrawResult WindowOpenGLWin32::draw_fill_polygonal_(float x, float y, float *points, uint32_t pointsArraySize, float r, float g, float b, float a) const
{
    static GL::Buffer buffer;
    static bool needInit = true;

    if (needInit)
    {
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
    shader->set_mat4("transform",m_mat4);
    shader->set_color("ourColor", r, g, b, a);

    // 绘制多边形
    glBindVertexArray(buffer.VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, pointsArraySize);
    glBindVertexArray(0);

    return WindowDrawResult::WDR_SUCCESS;
}

WindowDrawResult WindowOpenGLWin32::draw_text_(float x, float y, const char *text,TextStyle *style) const
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
    auto &hglrc = m_hglrc;
    if (style)
        y = m_windowSize.height - y - style->font.size;
    else
        y = m_windowSize.height - y-24;

    // 检查OpenGL上下文
    if (!hglrc)
    {
        std::cerr << "OpenGL context is not available" << std::endl;
        return WindowDrawResult::WDR_OPENGL_CONTEXT_UNAVAILABLE;
    }

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

WindowDrawResult WindowOpenGLWin32::clear_background_(float r, float g, float b,float a) const
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
    return WindowDrawResult::WDR_SUCCESS;
}

void WindowOpenGLWin32::set_transform(const VEC::Mat4 &m)
{
    m_mat4=m;
}
VEC::Mat4 WindowOpenGLWin32::get_transform() const
{
    return m_mat4;
}
#endif
} // namespace WinodwDefine

} // namespace OneKeyEngine