#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fielddata.h"
#include<QMessageBox>
#include"configdialog.h"
#include<QDebug>

QLCDNumber* MainWindow::mineNumber = nullptr;
QLCDNumber* MainWindow::timeNumber = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //qDebug()<<"节点一";

    timeNumber = new QLCDNumber;
    mineNumber = new QLCDNumber;

    Field;

    this->setCentralWidget(Field->getView());

    ui->statusbar->addWidget(timeNumber);
    ui->statusbar->addWidget(mineNumber);

    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int pos_x = event->localPos().x();
    int pos_y = event->localPos().y() - 35;
    cellItem* item = (cellItem*)Field->getView()->itemAt(pos_x,pos_y);
    if(item == nullptr)
        return;
    //qDebug()<< Field->getView()->itemAt(event->localPos().x(),event->localPos().y());

    Field->visitCell(item->pos().x()/cellItem::cell_w,
                     item->pos().y()/cellItem::cell_h,event->button());
    mineNumber->display(QString::number(Field->left_mines));
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionNew_game_triggered()
{
    Field->reset();
}

void MainWindow::on_actionConfig_triggered()
{
    configDialog dialog(Field->getWidth(),Field->getHeight(),
                        Field->getMines(),Field->getSpecial());
    if(dialog.exec() == QDialog::Accepted)
    {
        if(dialog._width > 54 && dialog._height > 27)
        {
            QMessageBox::information(this,"Warning","It's too wide and high");
        }
        else if(dialog._width > 54)
        {
            QMessageBox::information(this,"Warning","It's too wide");
        }
        else if(dialog._height > 27)
        {
            QMessageBox::information(this,"Warning","It's too high");
        }
        else if(dialog._mines < dialog._width * dialog._height * 0.9)
        {
            Field->costumizeWHMS(dialog._width,dialog._height,dialog._mines,dialog._special);
            Field->reset();
        }
        else
        {
            QMessageBox::information(this,"Warning","Mines is too much");
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"about","AhornBlade version 2.3");
}

