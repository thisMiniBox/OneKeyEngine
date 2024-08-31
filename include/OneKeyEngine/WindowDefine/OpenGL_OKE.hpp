#pragma once

#ifdef __glad_h_
#include"../Vector_OKE.hpp"
#include<string>
#include<iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include<algorithm>

namespace OneKeyEngine
{

namespace GL
{
class ShaderGL
{
private:
    uint32_t shaderProgramID = 0;

public:
    ShaderGL(const char *vertexPath = nullptr, const char *fragmentPath = nullptr,bool isCode=false);
    ~ShaderGL();
    uint32_t get_id() const;

    std::string load_shader_from_file(const char *vertexPath, const char *fragmentPath);
    std::string load_shader(const char *vShaderCode, const char *fShaderCode);

    void delete_program();

    void use()const;

    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_loat(const std::string &name, float value) const;
    void set_vec4(const std::string &name, float x,float y,float z,float w)const;
    void set_vec4(const std::string &name, const VEC::Vec4& vec4)const;
    void set_color(const std::string &name, float r,float g,float b,float a=1.0f)const;
    void set_color(const std::string &name, const VEC::Color& color)const;
    void set_mat4(const std::string& name,const VEC::Mat4& mat4)const;
};


class Buffer
{
public:
    uint32_t VAO = 0, VBO = 0;
    Buffer(const void* data=nullptr,uint32_t bufferSize=0,BufferType type=BufferType::StaticDraw);
    ~Buffer();

    void use();
    void init_buffer(const void* data,uint32_t bufferSize,BufferType type=BufferType::StaticDraw);
    void update_buffer(const void* data,uint32_t bufferSize,uint32_t startPoint=0)const;
    void set_causality(uint32_t index,uint32_t size,uint32_t stride,void* start,GLenum dataType)const;
};
class Element:public Buffer
{
    public:
    uint32_t EBO=0;
    Element();
    ~Element();
    void init_element(const void* data,uint32_t bufferSize,BufferType type=BufferType::StaticDraw);
    void update_element(const void* data,uint32_t bufferSize,uint32_t startPoint=0)const;
};


class LineStrip {
public:
    /* 数据 */
    std::vector<VEC::Vec3> vertices;

    /* 函数 */
    LineStrip(const std::vector<VEC::Vec3>& vertices) 
        : vertices(vertices) {
        setup_line_strip();
    }
    ~LineStrip();

    void draw(const ShaderGL& shader)const;

private:
    /* 渲染数据 */
    unsigned int VAO, VBO;

    /* 函数 */
    void setup_line_strip();
};


inline Buffer::Buffer(const void* data,uint32_t bufferSize,BufferType type)
{
    if(data&&bufferSize>0)
    {
        init_buffer(data,bufferSize,type);
    }
}

inline Buffer::~Buffer()
{
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
}

inline void Buffer::use()
{
    if (VAO == 0)
        glBindVertexArray(VAO);
}

inline void Buffer::init_buffer(const void *data, uint32_t bufferSize, BufferType type)
{
    if (VAO == 0)
        glGenVertexArrays(1, &VAO);
    if (VBO == 0)
        glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, data, (GLenum)type);
    // glBindVertexArray(0);
}

inline void Buffer::update_buffer(const void *data, uint32_t bufferSize, uint32_t startPoint) const
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, startPoint, bufferSize, data);
    // glBindVertexArray(0);
}

inline void Buffer::set_causality(uint32_t index, uint32_t size, uint32_t stride, void *start, GLenum dataType) const
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, dataType, dataType, stride, start);
    // glBindVertexArray(0);
}

inline Element::Element()
{
}

inline Element::~Element()
{
    glDeleteBuffers(1,&EBO);
}

inline void Element::init_element(const void *data, uint32_t bufferSize, BufferType type)
{
    if (EBO == 0)
        glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 使用正确的缓冲区对象
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, (GLenum)type);
    glBindVertexArray(0);
}

inline void Element::update_element(const void *data, uint32_t bufferSize, uint32_t startPoint) const
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, startPoint, bufferSize, data);
    glBindVertexArray(0);
}

inline void LineStrip::setup_line_strip()
{
    // 创建VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VEC::Vec3), &vertices[0], GL_STATIC_DRAW);

    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VEC::Vec3), (void *)0);

    glBindVertexArray(0);
}
inline LineStrip::~LineStrip()
{
    if(VAO>0)
    {
        glDeleteVertexArrays(1,&VAO);
        VAO=0;
    }
    if(VBO>0)
    {
        glDeleteBuffers(1,&VBO);
    }
}
inline void LineStrip::draw(const ShaderGL &shader) const
{
    // 使用传入的shader程序
    shader.use();
    
    // 绑定VAO
    glBindVertexArray(VAO);

    // 绘制线条
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size()));

    // 解绑VAO
    glBindVertexArray(0);
}


inline ShaderGL::ShaderGL(const char* vertexPath, const char* fragmentPath,bool isCode)
{
    if (vertexPath && fragmentPath)
    {
        if (isCode)
        {
            std::cout << load_shader(vertexPath, fragmentPath) << std::endl;
        }
        else
        {
            std::cout << load_shader_from_file(vertexPath, fragmentPath) << std::endl;
        }
    }
}

inline ShaderGL::~ShaderGL()
{
    delete_program();
}

inline uint32_t ShaderGL::get_id() const
{
    return shaderProgramID;
}

inline std::string ShaderGL::load_shader_from_file(const char *vertexPath, const char *fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();       
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();     
    }
    catch(std::ifstream::failure e)
    {
        return "打开文件失败\n";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    return load_shader(vShaderCode,fShaderCode);
}

inline std::string ShaderGL::load_shader(const char *vShaderCode, const char *fShaderCode)
{
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        glDeleteShader(vertex);
        return std::string("顶点着色器编译失败！\n")+infoLog;
    };
    // 片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // 打印编译错误（如果有的话）
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return std::string("片段着色器编译失败！\n")+infoLog;
    };
    delete_program();
    // 着色器程序
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertex);
    glAttachShader(shaderProgramID, fragment);
    glLinkProgram(shaderProgramID);
    // 打印连接错误（如果有的话）
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        return std::string("着色器链接失败！\n")+infoLog;
    }
    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return "着色器编译成功\n";
}

inline void ShaderGL::delete_program()
{
    if (shaderProgramID > 0)
        glDeleteProgram(shaderProgramID);
    shaderProgramID = 0;
}

inline void ShaderGL::use()const
{
    if (shaderProgramID >= 0)
    {
        glUseProgram(shaderProgramID);
    }
}

inline void ShaderGL::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

inline void ShaderGL::set_int(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

inline void ShaderGL::set_loat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

inline void ShaderGL::set_vec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z, w);
}

inline void ShaderGL::set_vec4(const std::string &name, const VEC::Vec4 &vec4) const
{
    set_vec4(name, vec4.r, vec4.g, vec4.b, vec4.a);
}

inline void ShaderGL::set_color(const std::string &name, float r, float g, float b, float a) const
{
    set_vec4(name,r,g,b,a);
}

inline void ShaderGL::set_color(const std::string &name, const VEC::Color &color) const
{
    set_vec4(name,color.r,color.g,color.b,color.a);
}

inline void ShaderGL::set_mat4(const std::string &name, const VEC::Mat4 &mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_TRUE, (float *)mat4.data);
}

} // namespace GL


}
#endif