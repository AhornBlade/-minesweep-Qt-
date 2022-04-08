#include "fielddata.h"
#include <QDebug>
#include<ctime>
#include<queue>
#include<QObject>
#include"mainwindow.h"
using namespace std;

FieldData* FieldData::_ins = Q_NULLPTR;

vector<int> FieldData::direct_x = {-1,-1,-1,0,0,1,1,1};
vector<int> FieldData::direct_y = {-1,0,1,-1,1,-1,0,1};

QGraphicsView* FieldData::_view = nullptr;

int remain_mines,remain_INITIAL;

int left_mines,used_time;

bool isFirst;

FieldData::FieldData()
    :_width(20),_height(15),_mines(50),_special(-1)
{
    qDebug()<<"构造函数";
    cellItem::CreatAll();
    qDebug()<<"CreatAll执行结束";
    _scene = nullptr;
    uTimer = new QTimer(_view);
    QObject::connect(uTimer,&QTimer::timeout,[=]()
    {
        used_time++;
        MainWindow::timeNumber->display(used_time);
    });
    srand(time(nullptr));
    reset();
}

void FieldData::updateSurrounding(int x, int y)
{
    int nx,ny;
    for(int i = 0;i<8;++i)
    {
        nx = x+direct_x[i];
        ny = y+direct_y[i];
        if(nx >= 0 && nx < _width && ny >= 0 && ny < _height && _cells[nx][ny] != -1)
            _cells[nx][ny]++;
    }
}

void FieldData::creatAItem(int x, int y)
{
    cellItem* item = new cellItem(x,y);
    _scene->addItem(item);
    _items[x][y] = item;
}

void FieldData::block_blank(int x, int y)
{
    showACell(x,y);
    queue<pair<int,int>> q;
    q.push({x,y});
    while(!q.empty())
    {
        x = q.front().first,y = q.front().second;
        if(_cells[x][y] == 0)
        {
            int nx,ny;
            for(int i = 0;i<8;i++)
            {
                nx = x+direct_x[i],ny = y+direct_y[i];
                if(nx >= 0 && nx < _width && ny >= 0 && ny < _height
                        && _items[nx][ny]->getStatus() == INITIAL)
                {
                    showACell(nx,ny);
                    q.push({nx,ny});
                }
            }
        }
        q.pop();
    }
}

void FieldData::showACell(int x, int y)
{
    if(_cells[x][y] == 0)
    {
        remain_INITIAL--;
        _items[x][y]->setStatus(BLANK);
    }
    else if(_cells[x][y] == -1)
    {
        _items[x][y]->setStatus(EXPLODE);
        game_over();
    }
    else
    {
        remain_INITIAL--;
        _items[x][y]->setStatus(DIGIT,_cells[x][y]);
    }
}

void FieldData::game_over()
{
    showAllMines();
    uTimer->stop();
    QMessageBox::information(_view,"Boom","You lost!,new game will start");
    reset();
}

void FieldData::showAllMines()
{
    for(int i = 0;i<_width;i++)
    {
        for(int j = 0;j<_height;j++)
        {
            if(_cells[i][j] == -1)
                _items[i][j]->setStatus(EXPLODE);
        }
    }
}

void FieldData::leftDigit(int x, int y)
{
    int _digit = _cells[x][y];
    if(_digit<=0)
        return;
    int nx,ny;
    for(int i = 0;i<8;i++)
    {
        nx = x+direct_x[i],ny = y+direct_y[i];
        if(nx >= 0 && nx < _width && ny>=0 && ny < _height &&
                _items[nx][ny]->getStatus() == FLAG)
            _digit--;
    }
    if(_digit != 0)
        return;
    bool hasMines = 0;
    for(int i = 0;i<8;i++)
    {
        nx = x+direct_x[i],ny = y+direct_y[i];
        if(nx >= 0 && nx < _width && ny>=0 && ny < _height)
        {
            if(_cells[nx][ny] == -1 && (_items[nx][ny]->getStatus() == INITIAL
                                        || _items[nx][ny]->getStatus() == QUESTION))
                hasMines = 1;
            else if(_items[nx][ny]->getStatus() != DIGIT)
                visitCell(nx,ny,Qt::LeftButton);
        }
    }
    if(hasMines)
        game_over();
}

FieldData::~FieldData()
{
    cellItem::DeleteAll();
    DeleteField();
    delete _scene;
    _cells.clear();
    _items.clear();
}

FieldData *FieldData::getInstance()
{
    qDebug()<<"getInstance成功运行";
    if(!_view)
        _view = new QGraphicsView;
    if(_ins == Q_NULLPTR)
        _ins = new FieldData;
    return _ins;

}

//清空
void FieldData::DeleteField()
{
    delete _ins;
    _ins = nullptr;

}

void FieldData::costumizeWHMS(int width, int height, int mines,int special)
{
    _width = width;
    _height = height;
    _mines = mines;
    _special = special;
}

void FieldData::reset()
{
    initCells();
    initItems();
    deployScene();
    deployMines();
    isFirst = true;
    remain_mines = _mines;
    remain_INITIAL = _height*_width;
    left_mines = _mines;
    used_time = 0;
    uTimer->stop();
    first_x = -1;
    first_y = -1;
    MainWindow::mineNumber->display(left_mines);
    MainWindow::timeNumber->display(used_time);
    //qDebug()<<"reset执行结束";
}

void FieldData::visitCell(int x, int y, Qt::MouseButton Btn)
{
    if(x<0||y<0||x>=_width||y>=_height)
        return;
    //qDebug()<< "x = "<< x<<"y = "<<y;
    cellItem* item = _items[x][y];
    //qDebug()<<item;
    switch (Btn)
    {
    case Qt::LeftButton:
        if(item->getStatus() == INITIAL||item->getStatus() == QUESTION)
        {
            if(_cells[x][y] == -1)
            {
                if(isFirst)
                {
                    reset();
                    visitCell(x,y,Btn);
                }
                else
                {
                    item->setStatus(EXPLODE);
                    game_over();
                }
            }
            else if(_cells[x][y] == 0)
            {
                block_blank(x,y);
                if(isFirst)
                {
                    isFirst = false;
                    first_x = x;
                    first_y = y;
                    uTimer->start(1000);
                }
                JudgeIsWin();
            }
            else
            {
                item->setStatus(DIGIT,_cells[x][y]);
                if(isFirst)
                {
                    isFirst = false;
                    first_x = x;
                    first_y = y;
                    uTimer->start(1000);
                }
                remain_INITIAL--;
                JudgeIsWin();
            }
        }
        else if(item->getStatus() == DIGIT)
        {
            leftDigit(x,y);
            JudgeIsWin();
        }
        break;
    case Qt::RightButton:
        switch(item->getStatus())
        {
        case INITIAL:
            item->setStatus(FLAG);
            if(_cells[x][y] == -1)
            {
                remain_mines--;
                remain_INITIAL--;
            }
            left_mines--;
            if(isFirst)
            {
                isFirst = false;
                first_x = x;
                first_y = y;
                uTimer->start(1000);
            }
            break;
        case FLAG:
            item->setStatus(QUESTION);
            if(_cells[x][y] == -1)
            {
                remain_mines++;
            }
            left_mines++;
            remain_INITIAL++;
            break;
        case QUESTION:
            item->setStatus(FLAG);
            if(_cells[x][y] == -1)
            {
                remain_mines--;
                remain_INITIAL--;
            }
            left_mines--;
            break;
        default:
            break;
        }
        JudgeIsWin();
        break;
    default:
        break;
    }
}

bool FieldData::JudgeIsWin()
{
    if(remain_mines == 0||remain_mines == remain_INITIAL)
    {
        uTimer->stop();
        QMessageBox::information(_view,"Result","You win,new game will start");
        reset();
        return true;
    }
    return false;
}

void FieldData::initCells()
{
    _cells.clear();
    _cells = vector<vector<int>>(_width,vector<int>(_height,0));
}

void FieldData::initItems()
{
    _items.clear();
    _items = vector<vector<cellItem*>>(_width,vector<cellItem*>(_height,nullptr));
}

void FieldData::deployScene()
{
    if(_scene)
        delete _scene;
    _scene = new QGraphicsScene;
    for(int i = 0;i<_width;i++)
    {
        for(int j = 0;j<_height;j++)
        {
             creatAItem(i,j);
        }
    }
    _view->setScene(_scene);
}

void FieldData::deployMines()
{
    int mines = _mines;
    while(mines)
    {
        int x = rand()%_width,y = rand()%_height;
        if(_cells[x][y] != -1 && (x != first_x || y != first_y))
        {
            _cells[x][y] = -1;
            updateSurrounding(x,y);
            mines--;
        }
    }
}
