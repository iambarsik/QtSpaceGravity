#include "space_object.h"
#include <math.h>

#include <QPen>
#include <QBrush>

space_object::space_object(int id, QColor color, int size, int mass, double x, double y, double Vx, double Vy, bool trash)  {

    m_id = id;
    m_color = color;
    m_x = x;
    m_y = y;
    m_vx = Vx;
    m_vy = Vy;
    m_size = size;
    m_mass = mass;
    m_trash = trash;
}

void space_object::draw(QPoint camera, QPainter *p)    {

        // reserv pen and brush
    QPen pen = p->pen();
    QBrush brush = p->brush();

        // draw object
    p->setPen(QPen(m_color,1));
    p->setBrush(QBrush(m_color));
    p->drawEllipse(camera.x() + (int)m_x - m_size/2,
                   camera.y() + (int)m_y - m_size/2, m_size, m_size);

        // restore pen and brush
    p->setPen(pen);
    p->setBrush(brush);
}

void space_object::update() {

    m_x += m_vx;
    m_y += m_vy;
}

void space_object::gravity(space_object &obj) {
    /*

        F = G*(M*m)/(r*r)

     * */


    if(m_trash == false || obj.isTrash() == false)    {

        double K = (double)obj.mass()/(double)m_mass;

        double G = 0.001;
        double R = sqrt((m_x - obj.x())*(m_x - obj.x()) + (m_y - obj.y())*(m_y - obj.y()));

        double F = G * (m_mass*obj.mass())/(R*R);

        if(m_x > obj.x() && m_y < obj.y())  {
            double a = acos((obj.y() - m_y)/R);
            double vx = F * cos(a) * K;
            double vy = F * sin(a) * K;
            m_vx -= vy;
            m_vy += vx;
        }
        if(m_x > obj.x() && m_y > obj.y())  {
            double a = acos((m_x - obj.x())/R);
            double vx = F * sin(a) * K;
            double vy = F * cos(a) * K;
            m_vx -= vy;
            m_vy -= vx;
        }
        if(m_x < obj.x() && m_y > obj.y())  {
            double a = acos((obj.y() - m_y)/R);
            double vx = F * cos(a) * K;
            double vy = F * sin(a) * K;
            m_vx += vy;
            m_vy += vx;
        }
        if(m_x < obj.x() && m_y < obj.y())  {
            double a = acos((obj.x() - m_x)/R);
            double vx = F * sin(a) * K;
            double vy = F * cos(a) * K;
            m_vx += vy;
            m_vy += vx;
        }
    }



}
