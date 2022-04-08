#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QLCDNumber>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static QLCDNumber* timeNumber;
    static QLCDNumber* mineNumber;

private:
    Ui::MainWindow *ui;

    void mousePressEvent(QMouseEvent* event);


private slots:

    void on_actionQuit_triggered();
    void on_actionNew_game_triggered();
    void on_actionConfig_triggered();
    void on_actionAbout_triggered();

};
#endif // MAINWINDOW_H
