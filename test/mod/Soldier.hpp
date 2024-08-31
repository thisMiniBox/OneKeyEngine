#pragma once
#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../namespaceDefine.hpp"
#include"SoldierSet.hpp"


class Soldier:public MOD::Module
{
public:
    float maximumView=50;
    //std::string imagePath="C:\\Users\\Lenovo\\Pictures\\opengl\\container2.png";
    OKE::ShaPtr<MOD::Image> image;
    OKE::ShaPtr<OKE::Scenes::ScenesObject> otherSoldier;
    OKE::ShaPtr<OKE::Scenes::ScenesObject> otherSoldier2;
    OKE::ShaPtr<Soldier>moveAim;

    OKE::VEC::Vec3 move;

    OKE::VEC::RectangleF limit=OKE::VEC::RectangleF(-70,70,70,-70);

    OKE::ShaPtr<OKE::Scenes::ScenesObject> startToken;
    float moveSpeed=5;

    float moveTimeInOneDirection=0;
    float patrolTime=1;//寻路更新时间


    float HP=100;
    float ATK=20;
    float attackRange=5;

    float attackCooldown=1;
    float awaitAttack=0;

    float hitRate=1;

    float randSize=1;

    std::string factions="red";

    void init()override
    {
        startToken=object->scenes->find_object("start");
        srand((unsigned int)time(NULL));
        image= object->add_module<MOD::Image>();
        //image->load_image_file(imagePath);
        move=OKE::VEC::Vec3(rand()-RAND_MAX/2,rand()-RAND_MAX/2,0).normalized();
    }
    
    void update()override
    {
        if(!startToken->enable)
            return;
        //方向修正计算
        moveTimeInOneDirection += 1.f/30.f;
        if (moveTimeInOneDirection > patrolTime+(((float)rand()/RAND_MAX)-randSize/2))
        {
            moveTimeInOneDirection -= patrolTime;
            auto nearEne = find_the_nearest_enemy();
            if (nearEne)
            {
                moveAim = nearEne;
                move = (moveAim->object->transform.position - object->transform.position).normalized();
            }
            else
            {
                moveAim = nullptr;
                move = OKE::VEC::Vec3(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, 0).normalized();
            }
        }

        if(moveAim)
        {
            auto div=(moveAim->object->transform.position-object->transform.position);
            if(div.x*div.x+div.y*div.y<attackRange*attackRange)
            {
                if(awaitAttack>attackCooldown&&(float)rand()/RAND_MAX<hitRate)
                {
                    moveAim->HP-=ATK;
                    awaitAttack=0;
                }
            }
        }

        if(HP<=0)
        {
            object->scenes->delete_object(object->scenes->get_object(object));
        }
        awaitAttack+=1/30.f;
        auto& position=object->transform.position;
        position+=move*moveSpeed*1/30.f;

        if(position.x<limit.left&&move.x<0)
        {
            move.x=std::abs(move.x);
        }
        else if(position.x>limit.right&&move.x>0)
        {
            move.x=-std::abs(move.x);
        }
        if(position.y<limit.bottom&&move.y<0)
        {
            move.y=std::abs(move.y);
        }
        else if(position.y>limit.top&&move.y>0)
        {
            move.y=-std::abs(move.y);
        }    
        
    }



    OKE::ShaPtr<Soldier> find_the_nearest_enemy()const
    {
        auto& position=object->transform.position;
        float minD=maximumView*maximumView;
        OKE::ShaPtr<Soldier> result;
        for(int i=0;i<otherSoldier->get_childs_count();i++)
        {
            auto ene=otherSoldier->get_child(i);
            auto d=ene->transform.position-position;
            float squares=d.x*d.x+d.y*d.y;
            if(squares<minD)
            {
                minD=squares;
                auto obj=object->scenes->get_object(ene);
                if(obj)
                    result=obj->get_module<Soldier>();
            }
        }
        if(otherSoldier2)
        for(int i=0;i<otherSoldier2->get_childs_count();i++)
        {
            auto ene=otherSoldier2->get_child(i);
            auto d=ene->transform.position-position;
            float squares=d.x*d.x+d.y*d.y;
            if(squares<minD)
            {
                minD=squares;
                auto obj=object->scenes->get_object(ene);
                if(obj)
                    result=obj->get_module<Soldier>();
            }
        } 
        return result;
    }
};


