#ifndef SPACE_OBJECT_H
#define SPACE_OBJECT_H

#include <QObject>
#include <QColor>
#include <QPainter>

class space_object
{

public:
    space_object(int id, QColor color, int size, int mass, double x, double y, double Vx, double Vy, bool trash, QString title);

    int id() { return m_id; }

    double x() { return m_x; }
    double y() { return m_y; }
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setXY(double x, double y) { m_x = x; m_y = y; }

    double Vx() { return m_vx; }
    double Vy() { return m_vy; }
    void setVx(double Vx) { m_vx = Vx; }
    void setVy(double Vy) { m_vy = Vy; }

    QColor color() { return m_color; }
    void setColor(QColor color) { m_color = color; }

    double size() { return m_size; }
    void setSize(double size)  { m_size = size; }
    double mass() { return m_mass; }

    bool isTrash() { return m_trash; }

    void collapse() { b_collapsed = true; }
    bool isCollapsed() { return b_collapsed; }

    QString title() { return m_title; }

    void draw(QPoint camera, QPainter *p);
    void update();
    void gravity(space_object &obj);
    void collide(space_object &obj);

private:
    int m_id;
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
    QColor m_color;
    double m_size;
    double m_mass;

    bool m_trash;
    bool b_collapsed = false;
    QString m_title;

signals:
    void signalCollided(space_object &o1, space_object &o2);

};

#endif // SPACE_OBJECT_H
