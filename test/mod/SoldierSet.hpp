#include<OneKeyEngine/OneKeyEngine.hpp>
#include"../namespaceDefine.hpp"
#include<vector>

class Soldier;
class SoldierSet:public MOD::Module
{
public:
    std::vector<OKE::ShaPtr<Soldier>>soldierSet;

    void draw()override
    {
        auto window=get_window();
        window->draw_line(-100,100,100,100);
        window->draw_line(100,100,100,-100);
        window->draw_line(100,-100,-100,-100);
        window->draw_line(-100,-100,-100,100);
    }
};