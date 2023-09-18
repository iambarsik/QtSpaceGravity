#include "widget.h"

void Widget::configure()
{


    space_object sun(generateID(), Qt::yellow, 200, 10000, 0, 0, -0.25, 0.0, false, "Солнце");
    addSpaceObject(sun);

    int o = 500;
    for(int i = 0; i < o; i++)  {
        space_object ob(generateID(), Qt::cyan,
                        (double)randomBetween(10,70),
                        (double)randomBetween(100,1000),
                        sun.x() + (double)randomBetween(-Resolution.x()*4, Resolution.x()*4),
                        sun.y() + (double)randomBetween(-Resolution.y()*4, Resolution.y()*4),
                        (double)randomBetween(-100,100)/10.0,
                        (double)randomBetween(-100,100)/10.0,
                        false, QString("Planet %1").arg(i));
        addSpaceObject(ob);
    }
    setCamera(sun, 0.05);

    /*
    space_object sun(generateID(), Qt::yellow, 200, 4000, Resolution.x()/2, Resolution.y()*3, -0.25, 0.0, false, "Солнце");
    addSpaceObject(sun);
    space_object mercury(generateID(), Qt::red, 15,  100, Resolution.x()*0.5, Resolution.y()*2.5, 8.0, 0.0, false, "Меркурий");
    addSpaceObject(mercury);
    space_object venus(generateID(), Qt::gray, 22,  200, Resolution.x()*0.5, Resolution.y(), 4.0, 0.0, false, "Венера");
    addSpaceObject(venus);
    space_object earth(generateID(), Qt::cyan, 25,  300, Resolution.x()*0.5, -Resolution.y()/2, 3.0, 0.0, false, "Земля");
    addSpaceObject(earth);
    space_object moon(generateID(), Qt::gray, 10,  1, earth.x() + 50, earth.y() - 50, 3.75, 0.75, false, "Луна");
    addSpaceObject(moon);
    space_object mars(generateID(), Qt::red, 20,  300, Resolution.x()*0.5, -Resolution.y()*2, 2.5, 0.0, false, "Марс");
    addSpaceObject(mars);
    space_object upiter(generateID(), QColor(188,116,10), 60,  700, Resolution.x()*0.5, -Resolution.y()*4, 2.2, 0.0, false, "Юпитер");
    addSpaceObject(upiter);
    space_object saturn(generateID(), Qt::yellow, 55,  500, Resolution.x()*0.5, -Resolution.y()*7, 1.5, 0.0, false, "Сатурн");
    addSpaceObject(saturn);
    space_object uranus(generateID(), QColor(100,255,255), 40,  300, Resolution.x()*0.5, -Resolution.y()*9, 1.8, 0.0, false, "Уран");
    addSpaceObject(uranus);
    space_object neptune(generateID(), Qt::blue, 40,  300, Resolution.x()*0.5, -Resolution.y()*11, 1.2, 0.0, false, "Нептун");
    addSpaceObject(neptune);

    setCamera(saturn, 0.05);
    */

}

