#include "space_object.h"
#include <math.h>
#include <QRadialGradient>

#include <QPen>
#include <QBrush>

space_object::space_object(int id, QColor color, int size, int mass, double x, double y, double Vx, double Vy, bool trash, QString title)  {

    m_id = id;
    m_color = color;
    m_x = x;
    m_y = y;
    m_vx = Vx;
    m_vy = Vy;
    m_size = size;
    m_mass = mass;
    m_trash = trash;
    m_title = title;
}

void space_object::draw(QPoint camera, QPainter *p)    {

/*
      // simple circles

      // reserve pen and brush
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
*/

        // radial gradient circles

        // reserve pen, font and brush
    QPen pen = p->pen();
    QFont font = p->font();
    QBrush brush = p->brush();

        // create radial
    QRadialGradient gr(camera.x() + m_x, camera.y() + m_y, m_size);
    gr.setColorAt(1, Qt::white);
    gr.setColorAt(0.75, Qt::white);
    gr.setColorAt(0, m_color);

    QPen g_pen(QColor(0, 0, 0, 0));
    g_pen.setWidth(0);
    QBrush g_brush(gr);
    p->setPen(g_pen);
    p->setBrush(g_brush);

        // draw object
    p->drawEllipse(camera.x() + (int)m_x - m_size/2,
                   camera.y() + (int)m_y - m_size/2, m_size, m_size);

    //p->setFont(QFont("Arial",14));
    //p->setPen(QPen(Qt::white));
    //p->drawText(camera.x() + m_x, camera.y() + m_y + m_size/2, m_title);

        // restore pen and brush
    p->setPen(pen);
    p->setFont(font);
    p->setBrush(brush);

}

void space_object::update() {

    m_x += m_vx;
    m_y += m_vy;
}

void space_object::gravity(space_object &obj) {

    /*

        F = G*(M*m)/(r*r)

    */

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

void space_object::collide(space_object &obj)   {

    double Lx = m_x - obj.x();
    double Ly = m_y - obj.y();
    double R = sqrt(Lx*Lx + Ly*Ly);
    if(R < (m_size/2 + obj.size()/2))    {

        if(m_mass < obj.mass()) {
            collapse();
            return;
        }
        if(m_mass > obj.mass()) {
            obj.collapse();
            return;
        }
        if(m_mass == obj.mass()) {
            collapse();
            obj.collapse();
            return;
        }
    }
}
