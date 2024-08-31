#pragma once
#ifndef __OKE_STYLE_H
#define __OKE_STYLE_HF

#include"Vector_OKE.hpp"

#include<string>
#include<unordered_map>
#include <cstdint>
#include <codecvt>
#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif
#include<glad/glad.h>
#include<functional>
#include <codecvt>
#include<memory>

namespace OneKeyEngine
{


namespace WindowDefine
{
    class WindowBase;
}
namespace WindowControl
{
    class WindowElement;
    class Picture;
}

struct LineStyle
{
    float width=1;
    int spacingSize=0;
    float* spacing=nullptr;
    VEC::Color color;
    ~LineStyle();
    void set_spacing(float* data,int size);
};

// 枚举定义对齐方式
enum class TextAlign {
    Left,
    Center,
    Right,
    Top,
    Bottom
};

// 枚举定义字体风格
enum class FontStyle {
    Regular,
    Bold,
    Italic,
    Underline,
    Strikeout
};

struct Font {
    std::string fontPath;
    int size;
    FontStyle style;

    Font() : fontPath("C:\\Windows\\Fonts\\simfang.ttf"), size(18), style(FontStyle::Regular) {}

    Font(const std::string &fontName, int fontSize, FontStyle fontStyle)
        : fontPath(fontName), size(fontSize), style(fontStyle) {}
    bool operator==(const Font &other) const
    {
        return fontPath == other.fontPath && size == other.size && style == other.style;
    }
};

struct TextStyle {
    Font font;                // 字体信息
    VEC::Color color;         // 文字颜色
    TextAlign horizontalAlign; // 水平对齐方式
    TextAlign verticalAlign;   // 垂直对齐方式
    VEC::Vec2 bounds;          //绘制范围，当x为非正数时为自动大小
    bool wordWrap;            // 自动换行
    bool singleLine;          // 单行显示

    TextStyle(const Font& f = Font(), const VEC::Color& c = VEC::Color(), 
              TextAlign hAlign = TextAlign::Left, TextAlign vAlign = TextAlign::Top, 
              bool wrap = true, bool single = false)
        : font(f), color(c), horizontalAlign(hAlign), verticalAlign(vAlign), wordWrap(wrap), singleLine(single) {}
};

struct Character
{
    uint32_t textureID;   // 字形纹理的ID
    VEC::Size size;     // 字形大小
    VEC::iVec2 bearing; // 从基准线到字形左部/顶部的偏移值
    uint32_t advance;     // 原点距下一个字形原点的距离
};

/// @brief utf-8解码为单字符
/// @param s 会自动偏移
/// @return 对应字符
static char32_t utf8_decode(const char* s,char** next=nullptr) {
    char32_t codepoint = 0;
    const unsigned char* u = reinterpret_cast<const unsigned char*>(s);

    if (u[0] < 0x80)
    {
        codepoint = u[0];
        if (next)
            (*next)++;
    }
    else if (u[0] < 0xE0)
    {
        codepoint = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F);
        if (next)
            (*next) += 2;
    }
    else if (u[0] < 0xF0)
    {
        codepoint = ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F);
        if (next)
            (*next) += 3;
    }
    else
    {
        codepoint = ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) | ((u[2] & 0x3F) << 6) | (u[3] & 0x3F);
        if (next)
            (*next) += 4;
    }

    return codepoint;
}

enum class CoordinateStyle
{
    NormalizedDeviceCoordinates,
    WindowCoordinates,
};

enum class BufferType : GLenum
{
    StaticDraw = GL_STATIC_DRAW,
    StaticRead = GL_STATIC_READ,
    StaticCopy = GL_STATIC_COPY,
    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY,
    StreamDraw = GL_STREAM_DRAW,
    StreamRead = GL_STREAM_READ,
    StreamCopy = GL_STREAM_COPY
};
enum class ModeType
{
    ShowCursor,
    CaptureCursor,
    MaxWindow,
    MinWindow,
    FullScreen,
};
enum class ModeValue
{
    False,
    True,
};
enum class KeyCode
{
    None = 0,
    MouseLeftButton = 1,
    MouseRightButton = 2,
    CanCel = 3,
    MouseMiddleButton = 4,
    MouseButtonX1 = 5,
    MouseButtonX2 = 6,
    Backspace = 8,
    Tab = 9,
    Claer = 0x0C,
    Enter = 0x0D,
    Shift = 0x10,
    CTRL = 0x11,
    Alt = 0x12,
    Pause = 0x13,//
    Capital = 0x14,
    Escape=0x1B,
    Space = 0x20,
    PageUp = 0x21,
    PageDown = 0x22,
    End = 0x23,
    Home = 0x24,
    Left = 0x25,
    Up = 0x26,
    Right = 0x27,
    Down = 0x28,
    Select = 0x29,
    Print = 0x2A,
    Execute = 0x2B,
    Snapshout = 0x2C,
    Insert = 0x2D,
    Delete = 0x2E,
    Help = 0x2F,
    A0 = 0x30,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A = 0x41,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U, 
    V,
    W,
    X,
    Y,
    Z,
    LeftWin = 0x5B,
    RightWin = 0x5C,
    Apps = 0x5D,
    Sleep = 0x5F,
    Numpad0 = 0x60,
    Numpad1 = 0x61,
    Numpad2 = 0x62,
    Numpad3 = 0x63,
    Numpad4 = 0x64,
    Numpad5 = 0x65,
    Numpad6 = 0x66,
    Numpad7 = 0x67,
    Numpad8 = 0x68,
    Numpad9 = 0x69,
    Multiple = 0x6A,
    Add = 0x6B,
    Separator = 0x6C,
    Subtract = 0x6D,
    Dcimal = 0x6E,
    Divide = 0x6F,
    F1 = 0x70,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    LeftShift = 0xA0,
    RightShift,
    LeftCtrl,
    RightCtrl,
    LeftAlt,
    RightAlt,
};

enum class WindowCreateResult
{
    WCR_SUCCESS = 0,
    WCR_FALSE,
    WCR_PARAMETER_ERROR,
    WCR_WINDOW_CREATED,
    WCR_GLAD_INIT_FAILED,

    WCR_WIN32_WINODW_CLASS_REGISTRATION_FAILED,
    WCR_WINDOW_CREATE_FAILED,

    WCR_NONSUPPORT,
};
enum class WindowDrawResult
{
    WDR_SUCCESS = 0,
    WDR_FALSE,

    WDR_PARAMETER_ERROR,
    WDR_WINDOW_NOT_CREATED,

    WDR_WIN32_FAILED_TO_OBTAIN_DEVICE_HANDLE,
    WDR_WIN32_PEN_CREATED_FAILED,
    WDR_WIN32_BRUSH_CREATE_FAILED,
    WDR_WIN32_FONT_CREATE_FAILED,

    WDR_OPENGL_BUFFER_CREATE_FAILED,
    WDR_OPENGL_CONTEXT_UNAVAILABLE,

    WDR_NONSUPPORT,
};
/// @brief 通过系统方式读取（大端和小端的字节序）
/// @tparam DataType 
/// @param data 
/// @param end 
/// @return 
template<typename DataType>
static DataType read(char* data,char** end=nullptr)
{
    if (end)
        *end = data + sizeof(DataType);
    DataType *result = (DataType *)data;
    return *result;
}
template<typename DataType>
static char* write(char* ptr,DataType&& aim)
{
    *((DataType *)ptr)=aim;
    return ptr + sizeof(DataType);
}
template<typename DataType>
static DataType read(unsigned char* data,unsigned char** end=nullptr)
{
    if (end)
        *end = data + sizeof(DataType);
    DataType *result = (DataType *)data;
    return *result;
}
template<typename DataType>
static unsigned char* write(unsigned char* ptr,DataType&& aim)
{
    *((DataType *)ptr)=aim;
    return ptr + sizeof(DataType);
}

/// @brief 大端序列读取
/// @tparam DataType 
/// @param data 
/// @param end 
/// @return 
template<typename DataType>
static DataType read_big_endian(char* data, char** end = nullptr)
{
    DataType value = 0;
    // 手动按字节读取并转换为大端
    for (size_t i = 0; i < sizeof(DataType); ++i)
    {
        value = (value << 8) | static_cast<uint8_t>(*data++);
    }
    if (end)
        *end = data;

    return value;
}
template<>
int read_big_endian<int>(char* data, char** end)
{
    int value = ((int)(*(unsigned char*)data++) << 24) |
                ((int)(*(unsigned char*)data++) << 16) |
                ((int)(*(unsigned char*)data++) << 8)  |
                ((*(unsigned char*)data++));
    
    if (end)
        *end = data;

    return value;
}
template<>
unsigned int read_big_endian<unsigned int>(char* data, char** end)
{
    unsigned int value = ((unsigned int)(*(unsigned char*)data++) << 24) |
                        ((unsigned int)(*(unsigned char*)data++) << 16) |
                        ((unsigned int)(*(unsigned char*)data++) << 8)  |
                        (*(unsigned char*)data++);
    
    if (end)
        *end = data;

    return value;
}
/// @brief 小端序列读取，大多数计算机都是小端，直接用read效率更高
/// @tparam DataType 
/// @param data 
/// @param end 
/// @return 
template<typename DataType>
static DataType read_little_endian(char* data, char** end = nullptr)
{
    DataType value = 0;
    for (size_t i = 0; i < sizeof(DataType); ++i)
    {
        value |= static_cast<DataType>(*data++) << (i * 8);
    }
    return value;
}

template<typename DataType>
static DataType read_bit(const DataType& data,int start,int length)
{
    if (start < 0 || length <= 0 || start + length > static_cast<int>(sizeof(DataType) * 8)) {
        throw std::out_of_range("Invalid start or length");
    }
    DataType mask = (static_cast<DataType>(1) << length) - 1;

    return (data >> start) & mask;
}
#ifdef _WIN32

static std::wstring utf8_to_utf16(const std::string& utf8String)
{
    int wideStringLength = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
    if (wideStringLength == 0)
    {
        throw std::runtime_error("Failed to determine required buffer size for conversion.");
    }

    std::unique_ptr<wchar_t[]> wideString(new wchar_t[wideStringLength]);

    if (MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, wideString.get(), wideStringLength) == 0)
    {
        throw std::runtime_error("Failed to convert UTF-8 string to UTF-16.");
    }

    return std::wstring(wideString.get());
}
#endif

class BinaryStreamRead
{
public:
    char* data;
    unsigned int currentChar;
    char bitPtr=-1;
    BinaryStreamRead(unsigned char* Data=nullptr):data((char*)Data){}
    BinaryStreamRead(char* Data=nullptr):data(Data){}
    int read_bite(char length)
    {
        if(bitPtr<0)
        {
            currentChar=read<int>(data);
            bitPtr=0;
        }
        int result=read_bit(currentChar,bitPtr,length);
        bitPtr+=length;
        if(bitPtr>7)
        {
            bitPtr-=8;
            data++;
            currentChar=read<int>(data);
        }
        return result;
    }
    unsigned int read_uint()
    {
        return read<unsigned int>(data, &data);
    }
    int read_int()
    {
        return read<int>(data,&data);
    }
    char read_char()
    {
        return read<char>(data,&data);
    }
    unsigned char read_uchar()
    {
        return read<unsigned char>(data,&data);
    }
    short read_short()
    {
        return read<short>(data,&data);
    }
    unsigned short read_ushort()
    {
        return read<unsigned short>(data,&data);
    }
    long long read_int64()
    {
        return read<long long>(data,&data);
    }
    unsigned long long read_uint64()
    {
        return read<unsigned long long>(data,&data);
    }
    template<class DataType>
    DataType read_()
    {
        return read<DataType>(data,&data);
    }
};


static int key_code_to_win32_virtual_key(KeyCode keyCode)
{
    return (int)keyCode;
}
inline LineStyle::~LineStyle()
{
    delete[] spacing;
}
inline void LineStyle::set_spacing(float* data,int size)
{
    if(spacing!=nullptr)
    {
        delete[] spacing;
    }
    spacing=new float[size];
    for(int i=0;i<size;++i)
    {
        spacing[i]=data[i];
    }
}

}// namespace OneKeyEngine

namespace std {
    template <>
    struct hash<OneKeyEngine::Font> {
        size_t operator()(const OneKeyEngine::Font& font) const {
            return hash<std::string>()(font.fontPath) ^ hash<int>()(font.size) ^ hash<int>()(static_cast<int>(font.style));
        }
    };
}

namespace OneKeyEngine
{
namespace Enhancement
{
#ifdef _WIN32
    class FontCacheWin32
    {
    public:
        HFONT get_font(const Font &font)
        {
            auto it = font_cache.find(font);
            if (it != font_cache.end())
            {
                return it->second;
            }

            int fontWeight = (font.style == FontStyle::Bold) ? FW_BOLD : FW_NORMAL;
            DWORD fontItalic = (font.style == FontStyle::Italic) ? TRUE : FALSE;

            HFONT hFont = CreateFontA(
                font.size, 0, 0, 0, fontWeight, fontItalic,
                (font.style == FontStyle::Underline) ? TRUE : FALSE,
                (font.style == FontStyle::Strikeout) ? TRUE : FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, font.fontPath.c_str());

            if (hFont != nullptr)
            {
                font_cache[font] = hFont;
            }
            return hFont;
        }
        FontCacheWin32(){};
        ~FontCacheWin32()
        {
            for (auto &pair : font_cache)
            {
                DeleteObject(pair.second);
            }
        }

    private:
        std::unordered_map<Font, HFONT> font_cache;
    };
#endif


} // namespace enhancement
} // namespace OneKeyEngine



#endif