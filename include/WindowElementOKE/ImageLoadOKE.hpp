#pragma once
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

#include"../../stb_image.h"

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

}




}

#endif