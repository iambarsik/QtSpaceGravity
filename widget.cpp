#include "widget.h"
#include "ui_widget.h"

/*
 *
 * Auto-stretched screen
 * Auto-rotated screen
 * Big font ENG+RUS
 * Medium font ENG+RUS
 * Small font eng+rus
 * Touch buttons
 *
 *
 *
 *
 */







Widget::Widget(QWidget *parent)
    : QGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setAttribute((Qt::WA_AcceptTouchEvents));

        // get available screens and scale widget
    QScreen* screen = QApplication::screens().at(0);
    QSize size = screen->availableSize();
    this->setGeometry(0,0,size.width(),size.height());

    GameField = QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE);

    #ifdef Q_OS_WINDOWS
        this->setWindowFlags(Qt::FramelessWindowHint);
    #endif

    Resolution.setX(GameField.width());
    Resolution.setY(GameField.height());

        // rotation flag for mobile devices and calculating aspect ration
    bRotateScreen = this->width() > this->height() ? false : true;
    if(bRotateScreen == false)  {
        dX = (double)(size.width())/(double)Resolution.x();
        dY = (double)(size.height())/(double)Resolution.y();
    } else {
        dX = (double)size.width()/(double)Resolution.y();
        dY = (double)(size.height())/(double)Resolution.x();
    }

        // Определяем соотношение экрана, чтоб знать, относительно чего растягивать канву
    bStretchHorizontal = this->width()/this->height() < SCREEN_TILE_WIDTH/SCREEN_TILE_HEIGHT ? true : false;
    if(bRotateScreen == false)  {
        if(bStretchHorizontal == true) {
            fImageZoom = (double)this->width()/(double)Resolution.x();
            fImageX = 0;
            fImageY = (this->height() - ((double)Resolution.y()*fImageZoom))/(2*fImageZoom);
        } else {
            fImageZoom = (double)this->height()/(double)Resolution.y();
            fImageX = (this->width() - ((double)Resolution.x()*fImageZoom))/(2*fImageZoom);
            fImageY = 0;
        }
    } else {
        if(bStretchHorizontal == true) {
            fImageZoom = (double)this->width()/((double)Resolution.y());
            fImageX = 0;   // доработать
            fImageY = 0;   // доработать
        } else {
            fImageZoom = (double)this->height()/((double)Resolution.x());
            fImageX = 0;   // доработать
            fImageY = 0;   // доработать
        }
    }

        // init user fonts
    sSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,!:+-?* abcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя=";
    FONT_IMAGE.load(":/images/font_micro.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_micro[i] = FONT_IMAGE.copy(i*8,0,8,8);
    }
    FONT_IMAGE.load(":/images/font_small.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_small[i] = FONT_IMAGE.copy(i*15,0,15,16);
    }
    FONT_IMAGE.load(":/images/font_medium.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_medium[i] = FONT_IMAGE.copy(i*30,0,30,32);
    }
    FONT_IMAGE.load(":/images/font_big.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_big[i] = FONT_IMAGE.copy(i*45,0,45,48);
    }

        // init buttons
    QImage imON;
    QImage imOFF;
    imON.load(":/images/button_on.png");
    imOFF.load(":/images/button_off.png");
    for(int i = 0; i < BUTTON_COUNT; i++)  {
        TouchKeys[i] = new TouchButton(QRect(
                TouchButtonGeometry[i][0]*TILE_SIZE,
                TouchButtonGeometry[i][1]*TILE_SIZE,
                TouchButtonGeometry[i][2]*TILE_SIZE,
                TouchButtonGeometry[i][3]*TILE_SIZE
                ),
                TouchButtonGeometry[i][4]);
        TouchKeys[i]->SetImage(imON,imOFF);
        myKey[i] = false;
    }


    SetStartValues();

    iForGame = 0;
    iForAnimation = 0;
    GameLoop = new QTimer;
    GameLoop->setTimerType(Qt::PreciseTimer);
    connect(GameLoop, SIGNAL(timeout()), SLOT(GamePlayFunction()));
    GameLoop->setInterval(0);
    GameLoop->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::GamePlayFunction() {

    QApplication::processEvents();
    QTime currentTime= QTime::currentTime();
    int msCurrent = currentTime.msecsSinceStartOfDay();

    if(msCurrent - iForGame > ON_GAME_PERIOD) {
        OnGame();
        iForGame = msCurrent;
    }
    if(msCurrent - iForAnimation > ON_ANIMATION_PERIOD) {
        OnAnimation();
        iForAnimation = msCurrent;
    }
}

void Widget::paintEvent(QPaintEvent *event) {

        // init buffer image
    QImage img(Resolution.x(),Resolution.y(),QImage::Format_RGB32);
    QPainter qScreen;
    qScreen.begin(&img);
    qScreen.setRenderHint(QPainter::Antialiasing,false);

    setColor(Qt::black,Qt::black, Qt::SolidPattern, &qScreen);
    qScreen.drawRect(0,0,Resolution.x(),Resolution.y());

        // ====================================== start user drawing ===============================
        // before zooming

    // draw something

        // zooming
    qScreen.save();
    qScreen.scale(zoom,zoom);

    // draw something
    setColor(Qt::white,Qt::white, Qt::NoBrush, &qScreen);
    qScreen.drawRect(0,0,Resolution.x() - 1,Resolution.y() - 1);

    //OnDrawText(Resolution.x()/4, TILE_SIZE,FONT_SIZE_MEDIUM,"Hello world!", "Здравствуй, Мир!", &qScreen);
    for(int o = 0; o < objects.size(); o++)    {
        objects[o].draw(Camera,&qScreen);
    }


    bool bDebug = false;

    if(bDebug)  {
        for(int o = 0; o < objects.size(); o++)    {
            OnDrawText(50, 15*o, FONT_SIZE_SMALL,QString("x = %1, y = %2").arg(objects[o].x()).arg(objects[o].y()),
                                                 QString("x = %1, y = %2").arg(objects[o].x()).arg(objects[o].y()),&qScreen);
        }
    }


        // after zooming
    qScreen.restore();
        // after zooming

    // draw something




        // ====================================== end user drawing =================================


        // Рисуем сенсорные кнопки, если экран перевёрнут
    if(bRotateScreen)   {
        for(int i = 0; i < BUTTON_COUNT; i++) {
            if(TouchKeys[i]->Enable) TouchKeys[i]->Draw(&qScreen);
            if(Settings.bDevMode)    {
                if(TouchKeys[i]->Value)
                    OnDrawText(12*TILE_SIZE,SCREEN_TILE_HEIGHT/2*TILE_SIZE,Fonts::FONT_SIZE_MEDIUM,
                               QString("Button pressed"),QString("Нажата кнопка"),&qScreen);
            }
        }
    }

    qScreen.end();

        // transposing buffer image to main widget
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing,false);
    setColor(Qt::black,Qt::black, Qt::SolidPattern, &p);
    p.drawRect(0,0,this->width(),this->height());
    if(bRotateScreen == false)   {
        p.scale(fImageZoom,fImageZoom);
        p.drawImage(fImageX,fImageY,img);
    } else {
        p.save();
        p.translate(this->width(),0);
        p.rotate(90);
        p.scale(fImageZoom,fImageZoom);
        p.drawImage(0,0,img);
        p.restore();
    }
    p.end();


    QWidget::paintEvent(event);


}
