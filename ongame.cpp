#include "widget.h"

void Widget::OnGame()   {

    OnTouch();
        // main app logic in timer period 15ms (edit in defines.h)

    for(int o = 0; o < objects.size(); o++)    {

        for(int i = 0; i < objects.size(); i++)    {
            if(objects[o].id() != objects[i].id())
                objects[o].gravity(objects[i]);
        }

        objects[o].update();
    }

    repaint();
}
