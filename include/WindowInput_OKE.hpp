#pragma once
#ifndef __OKE_WINDOW_INPUT_H
#define __OKE_WINDOW_INPUT_H

#include"WindowDefine/Window_OKE.hpp"
#include"scenes/EventManager.hpp"
#include"WindowDefine/WindowGLFW.hpp"
#include<unordered_map>
#include<vector>
#include<queue>

namespace OneKeyEngine
{
    class WindowInput;
    /// @brief 用于保证一个窗口只对应一个监听器，并且用于避免占用窗口的用户指针
    static std::unordered_map<WindowDefine::WindowBase *, WindowInput *> s_inputList;
    /// @brief 监听并管理窗口消息，窗口的按键按下与弹起和鼠标滚轮消息回调会被占用,消息查询后会标记为空,默认字符缓冲区大小为1024bit
    class WindowInput
    {
    private:

        WindowDefine::WindowBase* m_window=nullptr;
        std::unordered_map<KeyCode,char>m_keyStates;
        VEC::Vec2 m_mouseWheel;
        std::queue<char> m_charBuffer;

        static void key_down_monitor(WindowDefine::WindowBase *fromWho, KeyCode keyCode);
        static void key_up_monitor(WindowDefine::WindowBase *fromWho, KeyCode keyCode);
        static void mouse_wheel_callback(WindowDefine::WindowBase* window,float x,float y);
        static void char_input_callback(WindowDefine::WindowBase* window,uint32_t input);
        static void mouse_move_callback(WindowDefine::WindowBase* window,float x,float y);
    public:
        bool isGLFWWinodw=false;
        int bufferSize=1024;
  
        WindowDefine::WindowCallback::KeyCallback keyDownCallback=nullptr,keyUpCallback=nullptr;
        WindowDefine::WindowCallback::MouseWheelCallback mouseWheelCallback=nullptr;
        WindowDefine::WindowCallback::InputCallback charInputCallback=nullptr;
        WindowDefine::WindowCallback::MouseMoveCallback mouseMoveCallback=nullptr;

        /// @brief 在事件发生时调用自定义回调
        EventManager
            keyDownEvent,
            keyUpEvent,
            mouseWheelEvent,
            charInputEvent,
            mouseMoveEvent;

        WindowInput(WindowDefine::WindowBase* aimWindow=nullptr);
        ~WindowInput();
        void set_window(WindowDefine::WindowBase*aimWindow);
        /// @brief 判断按键是否按下，只在窗口为焦点时有效
        /// @param keyCode 
        /// @return 
        bool get_key(KeyCode keyCode);
        /// @brief 获取按键按下消息，获取一次后清空
        /// @param keyCode 
        /// @return 
        bool get_key_down(KeyCode keyCode);
        /// @brief 获取按键抬起消息，获取一次后清空
        /// @param keyCode 
        /// @return 
        bool get_key_up(KeyCode keyCode);
        /// @brief 获取鼠标在窗口客户区位置
        /// @return 
        const VEC::Vec2& get_mouse_position()const;
        VEC::Vec2 get_wheel();
        /// @brief 获取当前输入监测绑定的窗口
        /// @return 
        WindowDefine::WindowBase* get_current_window()const;

        std::string get_char_input();
        void clear_input_buffer();

        static WindowInput*get_window_bind_input(WindowDefine::WindowBase* window);
    };

    inline void WindowInput::key_down_monitor(WindowDefine::WindowBase *fromWho, KeyCode keyCode)
    {
        auto f = s_inputList.find(fromWho);
        if (f == s_inputList.end())
            return;
        WindowInput *aimInput = f->second;
        aimInput->m_keyStates[keyCode] = 2;
        if (aimInput->isGLFWWinodw)
        {
            switch (keyCode)
            {
            case KeyCode::LeftShift:
            case KeyCode::RightShift:
                key_down_monitor(fromWho, KeyCode::Shift);
                break;
            case KeyCode::LeftAlt:
            case KeyCode::RightAlt:
                key_down_monitor(fromWho, KeyCode::Alt);
                break;
            case KeyCode::LeftCtrl:
            case KeyCode::RightCtrl:
                key_down_monitor(fromWho, KeyCode::CTRL);
                break;
            default:
                break;
            }
        }
        else
        {
#ifdef _WIN32

        if (keyCode == KeyCode::Shift)
        {
            // 检查左右Shift键的状态
            if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
            {
                key_down_monitor(fromWho, KeyCode::LeftShift);
            }
            if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
            {
                key_down_monitor(fromWho, KeyCode::RightShift);
            }
        }
        else if (keyCode == KeyCode::CTRL)
        {
            if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
            {
                key_down_monitor(fromWho, KeyCode::LeftCtrl);
            }
            if (GetAsyncKeyState(VK_RCONTROL) & 0x8000)
            {
                key_down_monitor(fromWho, KeyCode::RightCtrl);
            }
        }
        else if (keyCode == KeyCode::Alt)
        {
            if (GetAsyncKeyState(VK_LMENU) & 0x8000)
            {
                key_down_monitor(fromWho, KeyCode::LeftAlt);
            }
            if (GetAsyncKeyState(VK_RMENU) & 0x8000)
            {
                key_down_monitor(fromWho, KeyCode::RightAlt);
            }
        }
#endif
        }
        if(aimInput->keyDownCallback)
            aimInput->keyDownCallback(fromWho,keyCode);
        //aimInput->keyDownCallback.call_all_void(fromWho,keyCode);
    }

    inline void WindowInput::key_up_monitor(WindowDefine::WindowBase *fromWho, KeyCode keyCode)
    {
        auto f=s_inputList.find(fromWho);
        if(f==s_inputList.end())return;
        WindowInput* aimInput=f->second;
        aimInput->m_keyStates[keyCode]=-1;

        if (aimInput->isGLFWWinodw)
        {
            switch (keyCode)
            {
            case KeyCode::LeftShift:
            case KeyCode::RightShift:
                key_up_monitor(fromWho, KeyCode::Shift);
                break;
            case KeyCode::LeftAlt:
            case KeyCode::RightAlt:
                key_up_monitor(fromWho, KeyCode::Alt);
                break;
            case KeyCode::LeftCtrl:
            case KeyCode::RightCtrl:
                key_up_monitor(fromWho, KeyCode::CTRL);
                break;
            default:
                break;
            }
        }
        else
        {
#ifdef _WIN32
            if (keyCode == KeyCode::Shift)
            {
                // 检查左右Shift键的状态
                if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000))
                {
                    key_up_monitor(fromWho, KeyCode::LeftShift);
                }
                if (!(GetAsyncKeyState(VK_RSHIFT) & 0x8000))
                {
                    key_up_monitor(fromWho, KeyCode::RightShift);
                }
            }
            else if (keyCode == KeyCode::CTRL)
            {
                if (!(GetAsyncKeyState(VK_LCONTROL) & 0x8000))
                {
                    key_down_monitor(fromWho, KeyCode::LeftCtrl);
                }
                if (!(GetAsyncKeyState(VK_RCONTROL) & 0x8000))
                {
                    key_down_monitor(fromWho, KeyCode::RightCtrl);
                }
            }
            else if (keyCode == KeyCode::Alt)
            {
                if (!(GetAsyncKeyState(VK_LMENU) & 0x8000))
                {
                    key_down_monitor(fromWho, KeyCode::LeftAlt);
                }
                if (!(GetAsyncKeyState(VK_RMENU) & 0x8000))
                {
                    key_down_monitor(fromWho, KeyCode::RightAlt);
                }
            }
#endif
        }

        if (aimInput->keyUpCallback)
            aimInput->keyUpCallback(fromWho, keyCode);

        //aimInput->keyUpCallback.call_all_void(fromWho,keyCode);
    }

    inline void WindowInput::mouse_wheel_callback(WindowDefine::WindowBase *window, float x, float y)
    {
        auto f = s_inputList.find(window);
        if (f == s_inputList.end())
            return;
        WindowInput *aimInput = f->second;
        if (x != 0)
            aimInput->m_mouseWheel.x = x;
        if (y != 0)
            aimInput->m_mouseWheel.y = y;
        if(aimInput->mouseWheelCallback)
            aimInput->mouseWheelCallback(window,x,y);
        //aimInput->mouseWheelCallback.call_all_void(window,x,y);
    }

    inline void WindowInput::char_input_callback(WindowDefine::WindowBase *window, uint32_t input)
    {
        auto f = s_inputList.find(window);
        if (f == s_inputList.end())
            return;
        WindowInput *aimInput = f->second;
#ifdef _WIN32
        auto ch = static_cast<wchar_t>(input);
        char utf8Str[4];
        int length = WideCharToMultiByte(CP_UTF8, 0, &ch, 1, utf8Str, 4, NULL, NULL);
        for (int i = 0; i < length; ++i)
        {
            aimInput->m_charBuffer.push(utf8Str[i]);
        }
        length = aimInput->m_charBuffer.size() - aimInput->bufferSize;
        for (int i = 0; i < length; ++i)
        {
            aimInput->m_charBuffer.pop();
        }
#endif

        if(aimInput->charInputCallback)
            aimInput->charInputCallback(window,input);

        //aimInput->charInputCallback.call_all_void(window,input);
    }

    inline void WindowInput::mouse_move_callback(WindowDefine::WindowBase *window, float x, float y)
    {
        auto f = s_inputList.find(window);
        if (f == s_inputList.end())
            return;
        WindowInput *aimInput = f->second;
        if(aimInput->mouseMoveCallback)
            aimInput->mouseMoveCallback(window,x,y);
        aimInput->mouseMoveEvent.call_all_void();
    }

    inline WindowInput::WindowInput(WindowDefine::WindowBase *aimWindow)
    {
        set_window(aimWindow);
    }

    inline WindowInput::~WindowInput()
    {
        set_window(nullptr);
    }

    inline void WindowInput::set_window(WindowDefine::WindowBase *aimWindow)
    {
        if(m_window)
        {
            m_window->set_key_down_callback(nullptr);
            m_window->set_key_up_callback(nullptr);
            m_window->set_mouse_wheel_callback(nullptr);
            m_window->set_input_callback(nullptr);
            s_inputList.erase(m_window);
        }
        m_window=aimWindow;
        if(!aimWindow)return;
        auto f=s_inputList.find(aimWindow);
        if(f!=s_inputList.end())
        {
            auto* other=f->second;
            other->set_window(nullptr);
        }
        m_window->set_key_down_callback(key_down_monitor);
        m_window->set_key_up_callback(key_up_monitor);
        m_window->set_mouse_wheel_callback(mouse_wheel_callback);
        m_window->set_input_callback(char_input_callback);
        m_window->set_mouse_move_callback(mouse_move_callback);
        s_inputList[aimWindow]=this;     

        if(dynamic_cast<WindowDefine::WindowGL_GLFW*>(m_window))
        {
            isGLFWWinodw=true;
        }
    }
    inline bool WindowInput::get_key(KeyCode keyCode)
    {
        if(!m_window->in_focus())
        {
            m_keyStates.clear();
            return false;
        }
        auto p = m_keyStates.find(keyCode);
        if (p == m_keyStates.end())
            return false;
        return p->second>0;
    }

    inline bool WindowInput::get_key_down(KeyCode keyCode)
    {
        if (!m_window->in_focus())
        {
            m_keyStates.clear();
            return false;
        }
        auto p = m_keyStates.find(keyCode);
        if (p == m_keyStates.end())
            return false;
        if(p->second==2)
        {
            p->second=1;
            return true;
        }
        return false;
    }

    inline bool WindowInput::get_key_up(KeyCode keyCode)
    {
        if (!m_window->in_focus())
        {
            m_keyStates.clear();
            return false;
        }
        auto p = m_keyStates.find(keyCode);
        if (p == m_keyStates.end())
            return false;
        if(p->second==-1)
        {
            p->second=0;
            return true;
        }
        return false;
    }

    inline const VEC::Vec2 &WindowInput::get_mouse_position() const
    {
        return m_window->mousePosition;
    }

    inline VEC::Vec2 WindowInput::get_wheel()
    {
        VEC::Vec2 result = m_mouseWheel;
        m_mouseWheel.x=0;
        m_mouseWheel.y=0;
        return result;
    }

    inline WindowDefine::WindowBase *WindowInput::get_current_window() const
    {
        return m_window;
    }

    inline std::string WindowInput::get_char_input()
    {
        std::string input;
        while (!m_charBuffer.empty())
        {
            char c = m_charBuffer.front();
            input.push_back(c);
            m_charBuffer.pop();
        }
        return input;
    }

    inline void WindowInput::clear_input_buffer()
    {
        while (!m_charBuffer.empty())
        {
            m_charBuffer.pop();
        }
    }


    inline WindowInput *WindowInput::get_window_bind_input(WindowDefine::WindowBase *window)
    {
        auto f = s_inputList.find(window);
        if (f == s_inputList.end())
            return nullptr;
        return f->second;
    }

} // namespace OneKeyEngine




#endif