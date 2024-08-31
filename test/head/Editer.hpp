#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../NameSpaceDefine.hpp"
#include"../mod/Soldier.hpp"
#include<iostream>
#include <fstream>

class Editer:public OneKeyEngine::Scenes::Scenes
{
public:
    OKE::ShaPtr<MOD::Canvas> canvas;
    OKE::ShaPtr<MOD::EditText> outputEdit;
    OKE::ShaPtr<MOD::Text> showRedStrength,showBlueForce;
    OKE::ShaPtr<MOD::Button>createSoldierButton;

    OKE::ShaPtr<OKE::Scenes::ScenesObject>redSet,blueSet;
    OKE::ShaPtr<OKE::Scenes::ScenesObject> startToken;

    OKE::ShaPtr<OKE::Scenes::ScenesObject> tankRedSet,tankBlueSet;

    std::vector<std::vector<int>> soldierCount;
    bool fullWinodw=false;



    static void buttonCallback(MOD::Button* button)
    {
        button->object->scenes->manage->to_scenes("起始场景");
        return;
    }
    void init()override
    {

        auto cameraObj=create_object("摄像机");
        auto camera=cameraObj->add_module<MOD::CameraModule>();
        backgroundColor=OKE::VEC::Color(0.8,0.8,0.8);
        cameraObj->transform.position=OKE::VEC::Vec3(0,0,-3000);

        auto canvasObj=create_object("画布");
        auto canvasSize=canvasObj->add_module<MOD::RectTransform>();
        canvasSize->size=OKE::VEC::Vec2(1920,1080);
        canvas= canvasObj->add_module<MOD::Canvas>();
        canvas->outPutCamera=mainCamera;
        canvas->projectorMode=OKE::Scenes::Module::ProjectorMode::screen;
        canvas->plotMode=MOD::PlotMode::screenScaling;
        canvas->matchWidthOrHeight=1;

        auto eventObj=create_object("事件处理器");
        auto eventsSystem=eventObj->add_module<MOD::UIEventsSystem>();
        eventsSystem->canvas=canvas;
        
        eventObj->add_module<SoldierSet>();

        redSet=create_object("红方士兵合集");
        blueSet=create_object("蓝方士兵合集");
        tankRedSet=create_object("红方坦克合集");
        tankBlueSet=create_object("蓝方坦克合集");
        startToken=create_object("start");
        startToken->enable=false;

        create_right_output_edit();
        create_left_menu();
    }

    void start()override
    {
        // srand((unsigned int)time(NULL));
        for(int i=0;i<6400;i++)
        {
            create_red_soldier(createSoldierButton.get());
            
        }
        for(int i=0;i<5700;i++)
        {
            create_blue_soldier(createSoldierButton.get());
        }
        for(int i=0;i<30;i++)
        {
            create_red_tank(createSoldierButton.get());
            
        }
        for(int i=0;i<20;i++)
        {
            create_blue_tank(createSoldierButton.get());
        }
    }
    
    void update()override
    {
        using namespace OKE;
        auto window=get_window();

        if(input()->get_key_down(KeyCode::Escape))
        {
            if (fullWinodw)
            {
                window->set_mode(ModeType::FullScreen,ModeValue::False);
            }
            else
            {
                window->set_mode(ModeType::FullScreen);
            }
            fullWinodw=!fullWinodw;
        }
        if(input()->get_key_down(KeyCode::Tab))
        {
            std::cout<<outputEdit->text.lock()->showText<<std::endl;
        }

        const float cameraMoveSpeed=30;
        if(input()->get_key(KeyCode::W))
        {
            mainCamera->move(mainCamera->get_up() * get_delta_time()*cameraMoveSpeed);
        }
        if (input()->get_key(KeyCode::S))
        {
            mainCamera->move(-mainCamera->get_up() * get_delta_time()*cameraMoveSpeed);
        }
        if (input()->get_key(KeyCode::A))
        {
            mainCamera->move(-mainCamera->get_right() * get_delta_time()*cameraMoveSpeed);
        }
        if (input()->get_key(KeyCode::D))
        {
            mainCamera->move(mainCamera->get_right() * get_delta_time()*cameraMoveSpeed);
        }
        if (input()->get_key(KeyCode::Space))
        {
            mainCamera->move(-mainCamera->get_forward() * get_delta_time()*cameraMoveSpeed);
        }
        if (input()->get_key(KeyCode::LeftShift))
        {
            mainCamera->move(mainCamera->get_forward() * get_delta_time()*cameraMoveSpeed);
        }
        //添加士兵快捷键
        if(input()->get_key_down(KeyCode::A1))
        {
            create_red_soldier(createSoldierButton.get());
        }
        if(input()->get_key_down(KeyCode::A2))
        {
            create_blue_soldier(createSoldierButton.get());
        }
        //统计红蓝方数量
        if(!redSet)
        {
            redSet=find_object("红方士兵合集");
        }
        else
        {
            int count=redSet->get_childs_count();
            showRedStrength->showText="红方兵力数量:"+std::to_string(count);
            static int last = count;
            if (last - count > 50)
            {
                last = count;
                for (int i = 0; i < 10; i++)
                    create_red_soldier(createSoldierButton.get());
            }
        }
        if(!blueSet)
        {
            blueSet=find_object("蓝方士兵合集");
        }
        else
        {
            int count = blueSet->get_childs_count();
            showBlueForce->showText = "蓝方兵力数量:" + std::to_string(blueSet->get_childs_count());
            static int last = count;
            if (last - count > 50)
            {
                last = count;
                for (int i = 0; i < 5; i++)
                    create_blue_soldier(createSoldierButton.get());
            }
        }

        if(startToken->enable)
        {
            auto rowData={
                redSet->get_childs_count(),blueSet->get_childs_count(),
                tankRedSet->get_childs_count(),tankBlueSet->get_childs_count(),
            };
            soldierCount.push_back(rowData);
        }
    }
    void save_count()const
    {
        auto filename="D:\\count.csv";
        std::ofstream file(filename, std::ios::out | std::ios::trunc );

        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return;
        }

        // 如果文件为空，或者文件之前被清空，则先写入标题
        if (file.tellp() == 0) {
            file << "红方士兵,蓝方士兵,红方坦克,蓝方坦克,\n";
        }

        for (const auto& pair : soldierCount) {
            for(auto& val:pair)
            {
                file << val << ",";
            }
            file<<'\n';
        }

        file.close();
    }
    void create_right_output_edit()
    {
        // auto title=TOOL::create_text(this,"lisp程序输出",canvas->object,50);
        // title->rectTransform.lock()->size=OKE::VEC::Vec2(420,80);
        // title->object->transform.position=OKE::VEC::Vec2(750,500);
        // title->textStyle.horizontalAlign=OKE::TextAlign::Center;
        // title->textStyle.color=OKE::VEC::Color(0.3,0.8,0.2);

        // outputEdit=TOOL::create_edit(this,"",canvas->object,420,1000);
        // outputEdit->text.lock()->textStyle.font.size=28;
        // outputEdit->object->transform.position=OKE::VEC::Vec2(750,-40);

        auto rightObj=create_object("rightButtonList");
        rightObj->transform.set_parent(canvas->object->transform);
        rightObj->transform.position=OKE::VEC::Vec3(860,0);

        createSoldierButton=TOOL::create_button(this,"创建红方士兵",canvas->object);
        createSoldierButton->pressCallbackManager.register_event("create_red_soldier",create_red_soldier,createSoldierButton.get());
        //createSoldierButton->object->transform.position=OKE::VEC::Vec3(860,40);
        auto createSoldierButtonBlue=TOOL::create_button(this,"创建蓝方士兵",canvas->object);
        createSoldierButtonBlue->pressCallbackManager.register_event("create_blue_soldier",create_blue_soldier,createSoldierButton.get());
        //createSoldierButtonBlue->object->transform.position=OKE::VEC::Vec3(860,-40);

        auto startButton=TOOL::create_button(this,"开始",canvas->object);
        startButton->pressCallbackManager.register_event("start_pause",start_pause,createSoldierButton.get());

        auto saveButton=TOOL::create_button(this,"保存",canvas->object);
        saveButton->pressCallbackManager.register_event("save_data",save_data,createSoldierButton.get());

        TOOL::Layout layout(this,rightObj.get());
        layout.alignmentMethod=TOOL::LayoutAlignmentMethod::CenterCenter;

        layout.insort_control(createSoldierButton->object);
        layout.insort_control(createSoldierButtonBlue->object);
        layout.insort_control(startButton->object);
        layout.insort_control(saveButton->object);
        layout.use();

    }   
    
    void create_left_menu()
    {
        using namespace OKE;
        auto leftObjSet=create_object("左边的对象");
        leftObjSet->transform.set_parent(canvas->object->transform);

        showRedStrength=TOOL::create_text(this,"红方兵力:",leftObjSet.get(),30);
        showRedStrength->textStyle.color=OKE::VEC::Color(0.80784,0.568627,0.470588);
        //showRedStrength->object->add_module<MOD::DrawRectTransform>();

        showBlueForce=TOOL::create_text(this,"蓝方兵力:",leftObjSet.get(),30);
        showBlueForce->textStyle.color=OKE::VEC::Color(0.337255, 0.611765, 0.843137);
        //showBlueForce->object->add_module<MOD::DrawRectTransform>();


        leftObjSet->transform.position=VEC::Vec3(-960,540);

        TOOL::Layout layout(this,leftObjSet.get());
        layout.controlInterval=10;
        layout.insort_control(showRedStrength->object);
        layout.insort_control(showBlueForce->object);


        layout.use();
    }

    static void create_red_soldier(MOD::Button* button)
    {
        auto& scenes=button->object->scenes;
        auto soldierSetObj= scenes->find_object("红方士兵合集");
        if(!soldierSetObj)
        {
            soldierSetObj=scenes->create_object("红方士兵合集");
        }
        auto soldierObj= scenes->create_object("红方士兵"+std::to_string(soldierSetObj->transform.get_childs_count()));
        soldierSetObj->transform.set_child(soldierObj->transform);
        auto soldier= soldierObj->add_module<Soldier>();
        soldier->factions="red";
        soldier->image->create_image(1,1,OKE::VEC::Color(1,0.5,0.5));
        soldier->otherSoldier=scenes->find_object("蓝方士兵合集");
        soldier->otherSoldier2=scenes->find_object("蓝方坦克合集");

        //soldier->object->transform.position=OKE::VEC::Vec3(rand()%100-50,rand()%100-50);
        soldier->object->transform.position=OKE::VEC::Vec3(50,0);
        soldier->ATK=20;
        soldier->hitRate=0.9;
    }

    static void create_blue_soldier(MOD::Button* button)
    {
        auto& scenes=button->object->scenes;
        auto soldierSetObj= scenes->find_object("蓝方士兵合集");
        if(!soldierSetObj)
        {
            soldierSetObj=scenes->create_object("蓝方士兵合集");
        }
        auto soldierObj= scenes->create_object("蓝方士兵"+std::to_string(soldierSetObj->transform.get_childs_count()));
        soldierSetObj->transform.set_child(soldierObj->transform);
        auto soldier= soldierObj->add_module<Soldier>();
        soldier->factions="blue";
        soldier->image->create_image(1,1,OKE::VEC::Color(0.5,0.5,1));
        soldier->otherSoldier=scenes->find_object("红方士兵合集");
        soldier->otherSoldier2=scenes->find_object("红方坦克合集");

        //soldier->object->transform.position=OKE::VEC::Vec3(rand()%100-50,rand()%100-50);
        soldier->object->transform.position=OKE::VEC::Vec3(-50,0);
        soldier->ATK=25;
        soldier->hitRate=0.9;
    }
    static void create_red_tank(MOD::Button* button)
    {
        auto& scenes=button->object->scenes;
        auto soldierSetObj= scenes->find_object("红方坦克合集");
        auto soldierObj= scenes->create_object("红方坦克"+std::to_string(soldierSetObj->transform.get_childs_count()));
        soldierSetObj->transform.set_child(soldierObj->transform);
        auto soldier= soldierObj->add_module<Soldier>();
        soldier->factions="red";
        soldier->image->create_image(1,1,OKE::VEC::Color(1,0,0));
        soldier->otherSoldier=scenes->find_object("蓝方士兵合集");
        soldier->otherSoldier2=scenes->find_object("蓝方坦克合集");
        //soldier->object->transform.position=OKE::VEC::Vec3(rand()%100-50,rand()%100-50);
        soldier->object->transform.position=OKE::VEC::Vec3(50,0);
        soldier->ATK=100;
        soldier->HP=1000;
        soldier->attackCooldown=3;
        soldier->attackRange=10;
        soldier->maximumView=70;
        soldier->moveSpeed=7;
        soldier->hitRate=0.8;
    }

    static void create_blue_tank(MOD::Button* button)
    {
        auto& scenes=button->object->scenes;
        auto soldierSetObj= scenes->find_object("蓝方坦克合集");
        auto soldierObj= scenes->create_object("蓝方士兵"+std::to_string(soldierSetObj->transform.get_childs_count()));
        soldierSetObj->transform.set_child(soldierObj->transform);
        auto soldier= soldierObj->add_module<Soldier>();
        soldier->factions="blue";
        soldier->image->create_image(1,1,OKE::VEC::Color(1,0,1));
        soldier->otherSoldier=scenes->find_object("红方士兵合集");
        soldier->otherSoldier2=scenes->find_object("红方坦克合集");
        //soldier->object->transform.position=OKE::VEC::Vec3(rand()%100-50,rand()%100-50);
        soldier->object->transform.position=OKE::VEC::Vec3(-50,0);
        soldier->ATK=100;
        soldier->HP=1000;
        soldier->attackCooldown=3;
        soldier->attackRange=10;
        soldier->maximumView=70;
        soldier->moveSpeed=7;
        soldier->hitRate=0.8;
    }

    static void start_pause(MOD::Button* button)
    {
        auto startObj = button->object->scenes->find_object("start");
        startObj->enable = !startObj->enable;
        auto text = button->object->get_child(0);
        auto buttonText = text->get_module<MOD::Text>();
        if (startObj->enable)
        {

            buttonText->showText = "暂停";
        }
        else
        {

            buttonText->showText = "继续";
        }
    }

    static void save_data(MOD::Button* button)
    {
        auto editScenes=dynamic_cast<Editer*>(button->object->scenes);
        editScenes->save_count();
    }
};


