#ifndef FIELDDATA_H
#define FIELDDATA_H

#include"cellitem.h"
#include<vector>
#include<QGraphicsView>
#include<QMessageBox>
#include<QTimer>
using namespace std;

#define Field FieldData::getInstance()

typedef vector<int> CellColumn;
typedef vector<CellColumn> CellMatrix;

typedef vector<cellItem*> itemColumn;
typedef vector<itemColumn> itemMatrix;

class FieldData
{
public:
    ~FieldData();

    static FieldData* getInstance();

    void DeleteField();

    inline int getHeight(){return _height;}
    inline int getWidth(){return _width;}
    inline int getMines(){return _mines;}
    inline int getSpecial(){return _special;}

    void costumizeWHMS(int width,int height,int mines,int special);

    void reset();

    void visitCell(int x,int y,Qt::MouseButton Btn);

    static vector<int> direct_x;
    static vector<int> direct_y;

    inline QGraphicsView* getView(){return _view;}

    bool JudgeIsWin();

    int left_mines,used_time;

private:
    FieldData();

    CellMatrix _cells;

    itemMatrix _items;

    static FieldData* _ins;

    static QGraphicsView* _view;
    QGraphicsScene* _scene;

    int _width;
    int _height;
    int _mines;
    int _special;

    int first_x,first_y;

    QTimer* uTimer = nullptr;

    void initCells();
    void initItems();

    void deployScene();
    void deployMines();

    void updateSurrounding(int x,int y);

    void creatAItem(int x,int y);

    void block_blank(int x,int y);

    void showACell(int x,int y);

    void game_over();

    void showAllMines();

    void leftDigit(int x,int y);

    void firstEvent();

};

#endif // FIELDDATA_H
