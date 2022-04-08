#ifndef CELLITEM_H
#define CELLITEM_H
#include<QGraphicsPixmapItem>
#include<QMouseEvent>

//cell 的六种状态
enum cellStatus
{
    INITIAL = 0,FLAG,QUESTION,EXPLODE,BLANK,DIGIT
};

class cellItem : public QGraphicsPixmapItem
{
public:
    cellItem(int x,int y,QGraphicsItem* parent = nullptr,cellStatus status  = INITIAL,int digit = -2);
    ~cellItem();

    void setStatus(cellStatus status,int digit = -2);
    inline cellStatus getStatus(){return _status;}

    static void CreatAll();
    static void DeleteAll();

    static double cell_h;
    static double cell_w;

private:
    static QPixmap* _initial;
    static QPixmap* _flag;
    static QPixmap* _question;
    static QPixmap* _explode;
    static QPixmap* _blank;
    static QPixmap* _digit[9];

    cellStatus _status;

};

#endif // CELLITEM_H
