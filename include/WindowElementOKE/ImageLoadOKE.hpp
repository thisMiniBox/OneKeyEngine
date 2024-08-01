#ifndef __OKE_IMAGE_LOAD_H
#define __OKE_IMAGE_LOAD_H

#include<string>
#include<fstream>

#include<iostream>
#include<memory>

#include"WindowElementOKE.hpp"

#ifdef _WIN32
#include<windows.h>
#include"../WindowDefine/WindowWin32_OKE.hpp"
#pragma comment(lib, "Msimg32")
#endif

#define STB_IMAGE_IMPLEMENTATION
#include"../opengl/stb_image.h"

namespace OneKeyEngine
{
namespace WindowControl
{
enum class ImageLoadResult
{
    //成功
    SUCCESS,
    //失败，未定义原因
    FAILURE,
    //参数错误
    PARAMETER_ERROR,
    //文件打开失败
    OPEN_FILE_FAILURE,
    //文件格式错误
    FILE_FORMAT_ERROR,
    //不支持的格式
    UNSUPPORTED,
};

namespace PictureData
{
struct PictureData
{
    unsigned char *data = nullptr;
    uint64_t dataSize = 0;
#ifdef _WIN32
    HBITMAP win32bitmap = nullptr;
#endif
#ifdef __glad_h_
    unsigned int openGLID = 0;
#endif

    ~PictureData()
    {
        if (data)
            stbi_image_free(data);
#ifdef WIN32
        delete_win32_bitmap();
#endif

#ifdef __glad_h_
        delete_openGL_buffer();
#endif
    }
#ifdef WIN32
    void delete_win32_bitmap()
    {
        if (win32bitmap)
            DeleteObject(win32bitmap);
        win32bitmap = nullptr;
    }
#endif
#ifdef __glad_h_
    void delete_openGL_buffer()
    {
        if (openGLID > 0)
            glDeleteTextures(1, &openGLID);
        openGLID = 0;
    }
#endif
};
}
class Picture:public WindowElement
{
    std::shared_ptr<PictureData::PictureData> m_drawData;
    VEC::Size m_imageSize;
    int m_bitDepth=0;
    int m_channels=0;
public:
    VEC::RectangleF crop=VEC::RectangleF(0,0,1,1);
    Picture();
    Picture(const std::string& filePath);
    ~Picture();

    const VEC::Size& image_size()const;
    int width()const;
    int height()const;
    int channels()const;
    void set_draw_rectangle(const VEC::RectangleF& rect);
    
    VEC::Color get_color(int x,int y)const;
    unsigned char *get_pixel_data(int x,int y)const;
    void set_pixel(int x,int y,const VEC::Color&color);
    void clear();
    ImageLoadResult load_image_form_file(const std::string& filePath);
    ImageLoadResult load_image(unsigned char* data,int len);
    /// @brief 直接按照外部色彩数据块当做数据
    /// @param bitmapData 目标数据块，需自行维护，提前删除可能导致指针越界
    /// @param width 图片宽度
    /// @param height 图片高度
    /// @param channel 图片色深1灰度，3彩色，4带透明度彩色
    /// @param bit 每位数据占用的字节数
    /// @return 
    ImageLoadResult set_image(unsigned char*bitmapData,int width,int height,int channel,int bit=1);
    unsigned char* get_data()const;
    ImageLoadResult create_image(int width,int height,VEC::Color defaultFill=VEC::Color(1.0f,1.0f,1.0f),int channels=4);
    std::vector<u_char>flip(bool x,bool y)const;
    /// @brief 将16，32，64字节的数据转换为8比特，通道保持不变
    /// @return 
    std::vector<u_char>get_8bit_image_data()const;

    WindowDrawResult draw(WindowDefine::WindowBase *window)override;

#ifdef _WIN32
    HBITMAP get_win32_image();
    void delete_win32_bitmap();
#endif
#ifdef __glad_h_
    uint32_t get_openGL_id();
    uint32_t get_openGL_id() const;
    void delete_openGL_buffer();
#endif
};


Picture::Picture()
{

}
inline Picture::Picture(const std::string &filePath)
{
    load_image_form_file(filePath);
}
inline Picture::~Picture()
{
    clear();
}
inline const VEC::Size &Picture::image_size() const
{
    return m_imageSize;
}
inline int Picture::width() const
{
    return m_imageSize.width;
}
inline int Picture::height() const
{
    return m_imageSize.height;
}

inline int Picture::channels() const
{
    return m_channels;
}

inline void Picture::set_draw_rectangle(const VEC::RectangleF &rect)
{
    transform.position=VEC::Vec3(rect.left,rect.top);
}

inline VEC::Color Picture::get_color(int x, int y)const
{
    VEC::Color color;
    auto pixel=get_pixel_data(x,y);
    if(pixel!=nullptr)
    {
        if(m_channels==1)
        {
            if(m_bitDepth==1)
            {
                color.r=(float)read<uint8_t>(pixel)/0xff;
                color.g=color.r;
                color.b=color.r;                
            }
            else if(m_bitDepth==2)
            {
                color.r=(float)read<uint16_t>(pixel)/0xffff;
                color.g=color.r;
                color.b=color.r;    
            }
            else if(m_bitDepth==4)
            {
                color.r=(float)read<uint32_t>(pixel)/0xffffffff;
                color.g=color.r;
                color.b=color.r;    
            }
        }
        else if(m_channels==3)
        {
            if(m_bitDepth==1)
            {
                color.r=(float)read<uint8_t>(pixel,&pixel)/0xff;
                color.g=(float)read<uint8_t>(pixel,&pixel)/0xff;
                color.b=(float)read<uint8_t>(pixel,&pixel)/0xff;      
            }
            else if(m_bitDepth==2)
            {
                color.r=(float)read<uint16_t>(pixel,&pixel)/0xffff;
                color.g=(float)read<uint16_t>(pixel,&pixel)/0xffff;
                color.b=(float)read<uint16_t>(pixel,&pixel)/0xffff;  
            }
            else if(m_bitDepth==4)
            {
                color.r=(float)read<uint32_t>(pixel,&pixel)/0xffffffff;
                color.g=(float)read<uint32_t>(pixel,&pixel)/0xffffffff;
                color.b=(float)read<uint32_t>(pixel,&pixel)/0xffffffff; 
            }
        }
        else if(m_channels==4)
        {
            if (m_bitDepth == 1)
            {
                color.r = (float)read<uint8_t>(pixel, &pixel) / 0xff;
                color.g = (float)read<uint8_t>(pixel, &pixel) / 0xff;
                color.b = (float)read<uint8_t>(pixel, &pixel) / 0xff;
                color.a = (float)read<uint8_t>(pixel, &pixel) / 0xff;
            }
            else if (m_bitDepth == 2)
            {
                color.r = (float)read<uint16_t>(pixel, &pixel) / 0xffff;
                color.g = (float)read<uint16_t>(pixel, &pixel) / 0xffff;
                color.b = (float)read<uint16_t>(pixel, &pixel) / 0xffff;
                color.a = (float)read<uint16_t>(pixel, &pixel) / 0xffff;
            }
            else if (m_bitDepth == 4)
            {
                color.r = (float)read<uint32_t>(pixel, &pixel) / 0xffffffff;
                color.g = (float)read<uint32_t>(pixel, &pixel) / 0xffffffff;
                color.b = (float)read<uint32_t>(pixel, &pixel) / 0xffffffff;
                color.a = (float)read<uint32_t>(pixel, &pixel) / 0xffffffff;
            }
        }
    }
    return color;
}
inline unsigned char *Picture::get_pixel_data(int x, int y)const
{
    if(!m_drawData)return nullptr;
    return m_drawData->data+((y*m_imageSize.width+x)*m_bitDepth*m_channels*m_bitDepth);
}
inline void Picture::set_pixel(int x, int y, const VEC::Color &color)
{
    delete_win32_bitmap();
    auto pixel=get_pixel_data(x,y);
    if(pixel!=nullptr)
    {
        if(m_channels==1)
        {

            float g=(color.r+color.g+color.b)/3.0f;
            if(m_bitDepth==1)
            {
                write(pixel,(uint8_t)(g*0xff));                
            }
            else if(m_bitDepth==2)
            {
                write(pixel,(uint16_t)(g*0xffff));         
            }
            else if(m_bitDepth==4)
            {
                write(pixel,(uint16_t)(g*0xffffffff));
            }
        }
        else if(m_channels==3)
        {
            if(m_bitDepth==1)
            {
                pixel=write(pixel,(uint8_t)(color.r*0xff));
                pixel=write(pixel,(uint8_t)(color.g*0xff));   
                pixel=write(pixel,(uint8_t)(color.b*0xff));                   
            }
            else if(m_bitDepth==2)
            {
                pixel=write(pixel,(uint16_t)(color.r*0xffff));
                pixel=write(pixel,(uint16_t)(color.g*0xffff));
                pixel=write(pixel,(uint16_t)(color.b*0xffff));         
            }
            else if(m_bitDepth==4)
            {
                pixel=write(pixel,(uint16_t)(color.r*0xffffffff));
                pixel=write(pixel,(uint16_t)(color.g*0xffffffff));
                pixel=write(pixel,(uint16_t)(color.b*0xffffffff));
            }
        }
        else if(m_channels==4)
        {
            if(m_bitDepth==1)
            {
                pixel=write(pixel,(uint8_t)(color.r*0xff));
                pixel=write(pixel,(uint8_t)(color.g*0xff));   
                pixel=write(pixel,(uint8_t)(color.b*0xff));
                pixel=write(pixel,(uint8_t)(color.a*0xff));                   
            }
            else if(m_bitDepth==2)
            {
                pixel=write(pixel,(uint16_t)(color.r*0xffff));
                pixel=write(pixel,(uint16_t)(color.g*0xffff));
                pixel=write(pixel,(uint16_t)(color.b*0xffff));
                pixel=write(pixel,(uint16_t)(color.a*0xffff));
            }
            else if(m_bitDepth==4)
            {
                pixel=write(pixel,(uint16_t)(color.r*0xffffffff));
                pixel=write(pixel,(uint16_t)(color.g*0xffffffff));
                pixel=write(pixel,(uint16_t)(color.b*0xffffffff));
                pixel=write(pixel,(uint16_t)(color.a*0xffffffff));
            }
        }
    }
}
inline void Picture::clear()
{
    if(m_drawData)
        m_drawData.reset();
}
inline ImageLoadResult Picture::load_image_form_file(const std::string &filePath)
{
    auto data = stbi_load(filePath.c_str(), &m_imageSize.width, &m_imageSize.height, &m_channels, 0);

    if (data)
    {
        m_bitDepth = 1;
        auto newData=new PictureData::PictureData;
        m_drawData.reset(newData);
        m_drawData->data=data;
        m_drawData->dataSize=m_imageSize.width * m_imageSize.height * m_channels;
        return ImageLoadResult::SUCCESS;
    }

    return ImageLoadResult::FAILURE;


    // std::ifstream file(filePath, std::ios::binary);
    // if (!file.is_open()) {
    //     return ImageLoadResult::OPEN_FILE_FAILURE;
    // }
    // file.seekg(0, std::ios::end);
    // std::streampos fileSize = file.tellg(); // 获取文件大小

    // // 确保文件大小为非负
    // if (fileSize < 0) {
    //     return ImageLoadResult::OPEN_FILE_FAILURE;
    // }

    // file.seekg(0, std::ios::beg);
    // std::vector<char> buffer(fileSize);
    
    // // 读取整个文件到vector中
    // file.read(buffer.data(), fileSize);

    // if (!file) {
    //     return ImageLoadResult::OPEN_FILE_FAILURE;
    // }
    // return load_image(buffer.data());
}
inline ImageLoadResult Picture::load_image(unsigned char *imageData,int len)
{
    if(!imageData)
        return ImageLoadResult::PARAMETER_ERROR;
    auto data= stbi_load_from_memory(imageData,len,&m_imageSize.width, &m_imageSize.height, &m_channels, 0);
    if (data)
    {
        m_bitDepth = 1;
        auto newData=new PictureData::PictureData;
        m_drawData.reset(newData);
        m_drawData->data=data;
        m_drawData->dataSize=m_imageSize.width * m_imageSize.height * m_channels;
        return ImageLoadResult::SUCCESS;
    }
    return ImageLoadResult::FAILURE;
    
    // auto hand=read<unsigned int>(data);
    // switch (hand)
    // {
    // case 0x474e5089:
    // //case 0x89504e47:
    //     if(read<unsigned int>(data+4)==0x0a1a0a0d)
    //     //if(read<unsigned int>(data+4)==0x0D0A1A0A)
    //         return load_png(data);
    //     break;
    
    // default:
    //     break;
    // }
    // return ImageLoadResult::UNSUPPORTED;
}
inline ImageLoadResult Picture::set_image(unsigned char *bitmapData, int width, int height, int channel, int bit)
{
    if (!bitmapData)
        return ImageLoadResult::FILE_FORMAT_ERROR;
    if (width == 0 || height == 0)
        return ImageLoadResult::FILE_FORMAT_ERROR;
    m_imageSize.width = width;
    m_imageSize.height = height;
    if (channel == 1 || channel == 3 || channel == 4)
    {
        m_channels = channel;
        m_bitDepth = bit;
    }
    else
        return ImageLoadResult::FILE_FORMAT_ERROR;
    clear();
    std::size_t dataSize =width*height*channel*bit;
    m_drawData.reset(new PictureData::PictureData);
    m_drawData->data=new u_char[dataSize];
    m_drawData->dataSize=dataSize;
    memcpy(m_drawData->data,bitmapData,dataSize);
    return ImageLoadResult::SUCCESS;
}
inline unsigned char *Picture::get_data() const
{
    if(m_drawData)
        return m_drawData->data;
    return nullptr;
}

inline ImageLoadResult Picture::create_image(int width, int height, VEC::Color defaultFill, int channels)
{
    if(width<=0||height<=0||channels>4||channels==2)return ImageLoadResult::FILE_FORMAT_ERROR;
    m_drawData.reset(new PictureData::PictureData);
    m_imageSize=VEC::Size(width,height);
    m_drawData->dataSize = width * height * channels;
    m_drawData->data = new unsigned char[m_drawData->dataSize];
    m_bitDepth = 1;
    m_channels = channels;
    
    for(int i=0;i<=m_drawData->dataSize;i+=channels)
    {
        for(int j=0;j<channels;++j)
        {
            m_drawData->data[i+j]=defaultFill[j]*255.0f;
        }
    }


    return ImageLoadResult::SUCCESS;
}

inline std::vector<u_char> Picture::flip(bool x, bool y) const
{
    auto dataSize=m_drawData->dataSize;
    std::vector<u_char>result(dataSize);
    memcpy(result.data(), m_drawData->data, dataSize);
    int rowSize = m_imageSize.width * m_channels * (m_bitDepth); // Size of one row in bytes
    int pixelSize = m_channels * (m_bitDepth);                   // Size of one pixel in bytes

    // Horizontal flip
    if (x)
    {
        for (int i = 0; i < m_imageSize.height; ++i)
        {
            u_char *rowStart = result.data() + i * rowSize;
            for (int j = 0; j < m_imageSize.width / 2; ++j)
            {
                int leftIndex = j * pixelSize;
                int rightIndex = (m_imageSize.width - 1 - j) * pixelSize;
                for (int k = 0; k < pixelSize; ++k)
                {
                    std::swap(rowStart[leftIndex + k], rowStart[rightIndex + k]);
                }
            }
        }
    }

    // Vertical flip
    if (y)
    {
        std::vector<u_char> rowBuffer(rowSize);
        for (int i = 0; i < m_imageSize.height / 2; ++i)
        {
            u_char *rowTop = result.data() + i * rowSize;
            u_char *rowBottom = result.data() + (m_imageSize.height - 1 - i) * rowSize;
            memcpy(rowBuffer.data(), rowTop, rowSize);
            memcpy(rowTop, rowBottom, rowSize);
            memcpy(rowBottom, rowBuffer.data(), rowSize);
        }
    }

    return result;
}

inline std::vector<u_char> Picture::get_8bit_image_data() const
{
    if(!m_drawData)
        return std::vector<u_char>();
    std::vector<u_char> result(m_imageSize.width*m_imageSize.height*m_channels);
    auto data=result.data();
    for (int y = 0; y < m_imageSize.height; ++y)
    {
        for (int x = 0; x < m_imageSize.width; ++x)
        {
            auto color = get_color(x, y);
            if (m_channels == 1)
            {
                data = write(data, (u_char)(color.r * 255));
            }
            else if (m_channels == 3)
            {
                data = write(data, (u_char)(color.r * 255));
                data = write(data, (u_char)(color.g * 255));
                data = write(data, (u_char)(color.b * 255));
            }
            else if (m_channels == 4)
            {
                data = write(data, (u_char)(color.r * 255));
                data = write(data, (u_char)(color.g * 255));
                data = write(data, (u_char)(color.b * 255));
                data = write(data, (u_char)(color.a * 255));
            }
        }
    }

    return result;
}

inline WindowDrawResult Picture::draw(WindowDefine::WindowBase *window)
{
    return window->draw(this);
}


#ifdef _WIN32
inline HBITMAP Picture::get_win32_image()
{
    if(!m_drawData)
    {
        return nullptr;
    }
    if (m_drawData->win32bitmap)
        return m_drawData->win32bitmap;
    BITMAPINFO bmi = {0};

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_imageSize.width;
    bmi.bmiHeader.biHeight = -m_imageSize.height; // 负值表示顶部到底部的位图
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 8 * m_bitDepth * m_channels;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = m_imageSize.width * m_imageSize.height * m_channels;
    void *pixels;
    auto win32bitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
    if (win32bitmap)
    {
        auto data8bit=get_8bit_image_data();
        // auto dataSize = m_imageSize.height * m_imageSize.width * m_bitDepth * m_channels;
        memcpy(pixels, data8bit.data(), data8bit.size());

        char *pixelsChar = (char *)pixels;
        for (int i = 0; i < data8bit.size(); i += m_channels)
        {
            auto r = pixelsChar[i];
            pixelsChar[i] = pixelsChar[i + 2];
            pixelsChar[i + 2] = r;
        }
        m_drawData->win32bitmap=win32bitmap;
    }
    else
    {
        std::cout << GetLastError() << std::endl;
    }
    return win32bitmap;
}
inline void Picture::delete_win32_bitmap()
{
    if(m_drawData)
        m_drawData->delete_win32_bitmap();
}

#endif
#ifdef __glad_h_
inline uint32_t Picture::get_openGL_id()
{
    if (!m_drawData)
        return 0;
    if (m_drawData->openGLID > 0)
        return m_drawData->openGLID;
    glGenTextures(1,&m_drawData->openGLID);
    glBindTexture(GL_TEXTURE_2D, m_drawData->openGLID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //auto data = this->flip(false, true);
    // if (m_channels == 3)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_imageSize.width, m_imageSize.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    // }
    // else if (m_channels == 4)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageSize.width, m_imageSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    // }
    // else if (m_channels == 1)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_imageSize.width, m_imageSize.height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data());
    // }
    if (m_channels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_imageSize.width, m_imageSize.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_drawData->data);
    }
    else if (m_channels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageSize.width, m_imageSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_drawData->data);
    }
    else if (m_channels == 1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_imageSize.width, m_imageSize.height, 0, GL_RED, GL_UNSIGNED_BYTE, m_drawData->data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    return m_drawData->openGLID;
}

inline uint32_t Picture::get_openGL_id() const
{
    if(m_drawData)
        return m_drawData->openGLID;
    return 0;
}
inline void Picture::delete_openGL_buffer()
{
    if(m_drawData)
        m_drawData->delete_openGL_buffer();
}

#endif

}



namespace WindowDefine
{
#ifdef _WIN32
WindowDrawResult Window2DWin32::draw_image(WindowControl::Picture *image)
{
    if (!image) return WindowDrawResult::WDR_PARAMETER_ERROR;

    HDC hdc = get_current_hdc();
    HBITMAP bitmap = image->get_win32_image();
    if (!bitmap) return WindowDrawResult::WDR_FALSE;

    HDC memDC = CreateCompatibleDC(hdc);
    if (!memDC) return WindowDrawResult::WDR_FALSE;

    HGDIOBJ oldBitmap = SelectObject(memDC, bitmap);
    if (!oldBitmap) {
        DeleteDC(memDC);
        return WindowDrawResult::WDR_FALSE;
    }
    auto imageSize = image->image_size();
    auto tran = get_transform();
    auto oldTran = tran;
    auto drawPosition = image->transform.position;

    auto windowSize=m_windowSize;
    auto windowSize1q2 = windowSize/2;

    auto rotate=image->transform.rotation;
    //rotate=VEC::Quaternion(VEC::Vec3(1,0,0),VEC::angle_to_radian(180.f))*rotate;
    //rotate.x=-rotate.x;
    tran.rotate(rotate);
    switch (coordinateStyle)
    {
        case CoordinateStyle::NormalizedDeviceCoordinates:
        {
            auto scale = VEC::Vec3(
                static_cast<float>(windowSize1q2.width) / imageSize.width,
                static_cast<float>(windowSize1q2.height) / imageSize.height,
                1);
            scale*=image->transform.scale;
            tran.scale(scale);
            drawPosition.x += windowSize1q2.width;
            drawPosition.x /= scale.x;
            drawPosition.y+=imageSize.height;
            tran.translate(VEC::Vec3(0,imageSize.height,0));
            break;
        }
        default:
        {
            tran.scale(image->transform.scale);
            tran.translate(drawPosition);
            break;
        }
    }

    set_transform(tran);


    auto crop = image->crop;
    auto cropPoint = VEC::Vec2(crop.left * imageSize.width, crop.top * imageSize.height);
    auto cropSize = VEC::Vec2(crop.right * imageSize.width, crop.bottom * imageSize.height) - cropPoint;

    BLENDFUNCTION blendFunc;
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.BlendFlags = 0;
    blendFunc.SourceConstantAlpha = 255;
    blendFunc.AlphaFormat = (image->channels() == 4) ? AC_SRC_ALPHA : 0;

    BOOL result = AlphaBlend(
        hdc, 
        // static_cast<int>(drawPosition.x), 
        // static_cast<int>(drawPosition.y), 
        0,
        0,
        static_cast<int>(cropSize.width), 
        static_cast<int>(cropSize.height), 
        memDC, 
        static_cast<int>(cropPoint.x), 
        static_cast<int>(cropPoint.y), 
        static_cast<int>(cropSize.width), 
        static_cast<int>(cropSize.height), 
        blendFunc
    );

    set_transform(oldTran);

    // 清理
    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);

    return result ? WindowDrawResult::WDR_SUCCESS : WindowDrawResult::WDR_FALSE;
}

#endif
#ifdef __glad_h_
WindowDrawResult WindowOpenGLWin32::draw_image(WindowControl::Picture* image)
{
    if(!image)return WindowDrawResult::WDR_PARAMETER_ERROR;
    if(s_imageShader.get_id()==0)
    {
        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "layout (location = 1) in vec2 aTexCoord;\n"
                                         "\n"
                                         "out vec2 TexCoord;\n"
                                         "\n"
                                         "uniform mat4 transform;\n"
                                         "\n"
                                         "void main()\n"
                                         "{\n"
                                         "    gl_Position =  transform* vec4(aPos, 1.0f);\n"
                                         "    TexCoord = aTexCoord;\n"
                                         "}\n";
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "\n"
                                           "in vec2 TexCoord;\n"
                                           "\n"
                                           "uniform sampler2D ourTexture;\n"
                                           "uniform vec4 crop;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                           "    if(TexCoord.x<crop.x||TexCoord.x>crop.z||TexCoord.y<crop.y||TexCoord.y>crop.w)\n"
                                           "        FragColor=vec4(0,0,0,0);\n"
                                           "    else\n"
                                           "        FragColor = texture(ourTexture, TexCoord);\n"
                                           "}\n";
        s_imageShader.load_shader(vertexShaderSource, fragmentShaderSource);
        float vertices[] = {
            //     ---- 位置 ----   - 纹理坐标 -
             1.0f,  0.0f, 0.0f,     1.0f, 1.0f,   // 右下
             1.0f, 1.0f, 0.0f,      1.0f, 0.0f,  // 右上
            -0.0f, 1.0f, 0.0f,     0.0f, 0.0f, // 左上
            -0.0f,  0.0f, 0.0f,     0.0f, 1.0f   // 左下
        };
        
        unsigned int indices[] = {  
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        s_imageVectex.init_buffer(vertices,sizeof(vertices));
        s_imageVectex.init_element(indices,sizeof(indices));

        s_imageVectex.set_causality(0,3,5*sizeof(float),(void*)0,GL_FLOAT);
        s_imageVectex.set_causality(1,2,5*sizeof(float),(void*)(3*sizeof(float)),GL_FLOAT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    s_imageShader.use();
    VEC::Mat4 tran=VEC::Mat4::unit_matrix();
    if(coordinateStyle==CoordinateStyle::WindowCoordinates)
    {
        auto pos=image->transform.position;
        tran.translate(pixel_to_NDC_LT(pos.x,pos.y));
        VEC::Vec3 ratio(pixel_to_NDC_LT(image->width(),image->height()));
        ratio.x+=1;
        ratio.y=1-ratio.y;
        tran.scale(ratio*image->transform.scale);
    }
    else if(coordinateStyle==CoordinateStyle::NormalizedDeviceCoordinates)
    {
        tran=image->transform.to_world_matrix();
    }
    auto crop=image->crop;
    auto y=crop.y;
    crop.y=1-crop.w;
    crop.w=1-y;

    tran=get_transform()*tran;
    s_imageShader.set_mat4("transform",tran);
    s_imageShader.set_vec4("crop", crop);
    glBindTexture(GL_TEXTURE_2D, image->get_openGL_id());
    glBindVertexArray(s_imageVectex.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return WindowDrawResult::WDR_SUCCESS;
}
#endif

}



}

#endif