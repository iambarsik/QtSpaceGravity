#include "widget.h"
#include <QCursor>

void Widget::SetStartValues()   {
    Settings.bDevMode = true;
    Settings.bEnglish = true;

    //QImage buf;
    //buf.load(":/images/logo.png");

    Camera.setX(0);
    Camera.setY(0);
    bMousePressed = false;


    space_object sun(generateID(), Qt::yellow, 50, 1000, Resolution.x()/2, Resolution.y()/2, 0.0, 0.0, false);
    space_object earth(generateID(), Qt::cyan, 10,  100, Resolution.x()*0.5, Resolution.y()*0.3, 3.0, 0.0, false);
    space_object moon(generateID(), Qt::gray,   5,   10, Resolution.x()*0.5, Resolution.y()*0.25, 2.9, 0.0, false);
    space_object uranus(generateID(), Qt::darkCyan, 15,  200, Resolution.x()*0.5, Resolution.y()*0.8, -3.5, 0.0, false);
    objects.push_back(sun);
    objects.push_back(earth);
    objects.push_back(moon);
    objects.push_back(uranus);

    int count = 200;
    for(int i = 0; i < count; i++) {
        space_object point(generateID(), Qt::white,   1,   5, (Resolution.x()*0.5 - (count*3)/2) + 3*i, Resolution.y()*0.25, 2.5, 1.0, true);
        objects.push_back(point);
    }

}

bool Widget::event(QEvent *event)   {
    switch (event->type())  {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:  {
            QTouchEvent* pte = static_cast<QTouchEvent*>(event);
            listKeys = pte->touchPoints();
            update();
        } break;

        default: return QWidget::event(event);
    }
    return true;
}

void Widget::mousePressEvent(QMouseEvent *event)    {
    MouseBuffer = QCursor::pos();
    bMousePressed = true;
    Q_UNUSED(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    bMousePressed = false;
    Q_UNUSED(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(bMousePressed)   {
        QPoint current = QCursor::pos();
        int x = current.x();
        int y = current.y();
        Camera.setX(Camera.x() + (x - MouseBuffer.x()));
        Camera.setY(Camera.y() + (y - MouseBuffer.y()));
        MouseBuffer = current;
    }
    Q_UNUSED(event);
}

void Widget::OnKeys(int Key, bool press)    {

    switch(Key) {
        case Qt::Key_Up:
        case Qt::Key_W: {
            myKey[BUTTON_UP] = press;
        } break;
        case Qt::Key_Right:
        case Qt::Key_D: {
            myKey[BUTTON_RIGHT] = press;
        } break;
        case Qt::Key_Down:
        case Qt::Key_S: {
            myKey[BUTTON_DOWN] = press;
        } break;
        case Qt::Key_Left:
        case Qt::Key_A: {
            myKey[BUTTON_LEFT] = press;
        } break;
        case Qt::Key_Space: {

        } break;
        case Qt::Key_Return:
        case Qt::Key_J: {
            myKey[BUTTON_ACT1] = press;
        } break;
        case Qt::Key_K: {
            myKey[BUTTON_ACT2] = press;
        } break;
        case Qt::Key_U: {
            myKey[BUTTON_ACT3] = press;
        } break;
        case Qt::Key_I: {
            myKey[BUTTON_ACT4] = press;
        } break;
        case Qt::Key_Q: {
            myKey[BUTTON_ACT5] = press;
        } break;
        case Qt::Key_E: {
            myKey[BUTTON_ACT6] = press;
        } break;
        case Qt::Key_H: {
            myKey[BUTTON_ACT7] = press;
        } break;
        case Qt::Key_Y: {
            myKey[BUTTON_ACT8] = press;
        } break;
        case Qt::Key_P: {

        } break;
        case Qt::Key_L: {

        } break;
        case Qt::Key_Escape: {
           exit(0);
        } break;
    }
}

void Widget::OnTouch()  {
    for(int i = 0; i < BUTTON_COUNT; i++) {
        TouchKeys[i]->Value = false;
    }
    int tpx;
    int tpy;
    double tpdX;
    double tpdY;
    foreach (QTouchEvent::TouchPoint tp, listKeys)  {
        if(bRotateScreen == false)  {
            tpx = tp.pos().x();
            tpy = tp.pos().y();
            tpdX = dX;
            tpdY = dY;
        } else {
            tpx = tp.pos().y();
            tpy = (this->width() - tp.pos().x());
            tpdX = dY;
            tpdY = dX;
        }
        switch (tp.state()) {
            case Qt::TouchPointReleased:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = false;
                    }
                }
            break;
            case Qt::TouchPointStationary:
            case Qt::TouchPointPressed:
            case Qt::TouchPointMoved:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = true;
                    }
                }
            break;
        }
    }
    for(int i = 0; i < BUTTON_COUNT; i++) {
        if(TouchKeys[i]->oldValue != TouchKeys[i]->Value)    {
            OnKeys(TouchKeys[i]->Code,TouchKeys[i]->Value);
            TouchKeys[i]->oldValue = TouchKeys[i]->Value;
        }
    }
}

void Widget::OnDrawText(int x, int y, int size, QString text_eng, QString text_rus, QPainter *pnt)  {
    QString text;
    if(Settings.bEnglish == true)   {
        text = text_eng;
    } else {
        text = text_rus;
    }
    switch(size)    {
        case FONT_SIZE_MICRO:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_micro[i].width(),y,imgSymbol_micro[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_SMALL:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_small[i].width(),y,imgSymbol_small[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_MEDIUM:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_medium[i].width(),y,imgSymbol_medium[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_LARGE:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_big[i].width(),y,imgSymbol_big[i]);
                    }
                }
            }
        break;
    }
}

void Widget::setColor(QColor brush, QColor pen, Qt::BrushStyle style, QPainter *qScr) {
    qScr->setBrush(QBrush(brush, style));
    qScr->setPen(QPen(pen));
}

void Widget::keyPressEvent(QKeyEvent *event)    {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),true);
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)  {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),false);
    QWidget::keyReleaseEvent(event);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    /*
    if(event->angleDelta().y() > 0)
        zoom += 0.1;
    else
        zoom -= 0.1;
        */

}
