#include "cellitem.h"
#include<QDebug>

double cellItem::cell_h = 0;
double cellItem::cell_w = 0;

QPixmap* cellItem::_flag = nullptr;
QPixmap* cellItem::_initial = nullptr;
QPixmap* cellItem::_blank = nullptr;
QPixmap* cellItem::_explode = nullptr;
QPixmap* cellItem::_question = nullptr;
QPixmap* cellItem::_digit[9] = {nullptr};


cellItem::cellItem(int x, int y, QGraphicsItem *parent, cellStatus status, int digit)
    :QGraphicsPixmapItem(parent)
{
    setPos(x*cell_w,y*cell_h);
    setStatus(status,digit);
}

cellItem::~cellItem()
{

}

void cellItem::setStatus(cellStatus status, int digit)
{
    _status = status;
    switch(_status)
    {
    case INITIAL:
        this->setPixmap(*_initial);
        break;
    case FLAG:
        this->setPixmap(*_flag);
        break;
    case QUESTION:
        this->setPixmap(*_question);
        break;
    case BLANK:
        this->setPixmap(*_blank);
        break;
    case EXPLODE:
        this->setPixmap(*_explode);
        break;
    case DIGIT:
        this->setPixmap(*_digit[digit]);
        break;
    default:
        break;
    }
}

void cellItem::CreatAll()
{
    _initial = new QPixmap("./images/initial.png");
    _question = new QPixmap("./images/question.png");
    _explode = new QPixmap("./images/explode.png");
    _flag = new QPixmap("./images/flag");
    _blank = new QPixmap("./images/blank.png");

    QString pre = "./images/";
    QString end = ".png";
    QString num;
    for(int i = 1;i<9;i++)
    {
        QString filename;
        filename +=pre;
        filename += num.setNum(i);
        filename += end;
        _digit[i] = new QPixmap(filename);
    }

    cell_h = _initial->height();
    cell_w = _initial->width();

    qDebug() << "cell_h = "<<cell_h<<"cell_w" << cell_w;
}

void cellItem::DeleteAll()
{
    delete _initial;
    delete _flag;
    delete _question;
    delete _blank;
    delete _explode;
    for(int i = 1;i<9;i++)
    {
        delete _digit[i];
    }
    qDebug()<<"cell数据清空完成";
}
