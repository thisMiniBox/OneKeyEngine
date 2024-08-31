#include<OneKeyEngine/OneKeyEngine.hpp>

#include"head/CreateProject.hpp"
#include"head/EditProject.hpp"
#include"head/Editer.hpp"

#include<GLFW/glfw3.h>

using namespace OneKeyEngine;

int main(int argc, char** argv) {

        SetConsoleOutputCP(CP_UTF8);

        WindowDefine::WindowGL_GLFW window;
        //WindowDefine::WindowOpenGLWin32 window2;
        window.create_window("",0,0,800,600);
        window.set_window_title("起始窗口");
        window.set_frame_rate(30);


        // window2.create_window();
        // window2.set_window_title("参考窗口");
        // window2.set_frame_rate(30);

        OKE::Scenes::ScenesManage manage(&window);

    //    OKE::Scenes::ScenesManage manage2(&window2);

        auto id= manage.add_scenes<CreateProject>("起始场景");
        auto editid= manage.add_scenes<EditProject>("编辑场景");
        std::cout<<id<<std::endl;
        manage.to_scenes("起始场景");

        // auto editid2= manage2.add_scenes<Editer2>("编辑场景");
        // std::cout<<editid2<<std::endl;
        // manage2.to_scenes(editid2);
    
    while (window.should_draw())
    {
        /* code */
    }
    


    return 0;
}
