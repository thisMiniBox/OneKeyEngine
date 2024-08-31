#pragma once

#ifndef __OneKeyEngine_HPP_
#define __OneKeyEngine_HPP_

//需要链接的

#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_OUTLINE_H
//结束

//#include"Internet/Client.hpp"

#include "ClassType_OKE.hpp"
#include"Vector_OKE.hpp"
#include"Style_OKE.hpp"

#include"WindowDefine/Window_OKE.hpp"
#ifdef _WIN32
#include"WindowDefine/WindowWin32_OKE.hpp"

#endif
#include"WindowDefine/WindowGLFW.hpp"

#include"WindowInput_OKE.hpp"
#include"WindowElementOKE/WindowElementOKE.hpp"
#include"WindowElementOKE/ImageLoadOKE.hpp"
#include"scenes/Scenes.hpp"
#include"scenes/ModuleBasicDefinition.hpp"
#include"scenes/ShowControlModule.hpp"
#include"scenes/ControlCreateTool.hpp"
#include"scenes/Camera.hpp"
#include"WindowDefine/OpenGL_OKE.hpp"


#endif