#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../namespaceDefine.hpp"
#include"../mod/FunctionCube.hpp"
#include"../mod/ControllerList.hpp"

class EditProject:public OKE::Scenes::Scenes
{
private:
    OKE::ShaPtr<MOD::Canvas>canvas;
    OKE::ShaPtr<MOD::Button>
        createFunction;
    OKE::ShaPtr<OKE::Scenes::ScenesObject>
        list;
public:
    void init()override
    {
        create_ui();
        create_base();
        test();
    }


    void create_ui()
    {
        using namespace OKE;
        canvas=TOOL::init_project_2d(this);
        canvas->plotMode=MOD::PlotMode::screenScaling;
        canvas->rectTransform->size=VEC::Vec2(1920,1080);

        backgroundColor=VEC::Color(1,1,1);
        createFunction=TOOL::create_button(this,"创建函数",canvas->object);


        TOOL::Layout layout(this,canvas->object);
        layout.position=VEC::Vec2(-100,0);
        layout.insort_control(createFunction->object);
        layout.use();


        create_controller_list();
    }
    void create_controller_list()
    {
        using namespace OKE; 
        list=create_object("控件列表",canvas->object);
        list->add_module<ControllerList>();
        list->add_module<MOD::DrawRectTransform>();
        
        list->transform.position=VEC::Vec2(-760,0);
    }
    void create_base()
    {
        using namespace OKE;

    }
    void test()
    {

    }

    
};

