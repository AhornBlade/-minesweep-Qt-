#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include<QRadioButton>

namespace Ui {
class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT

public:
    explicit configDialog(int weight,int height,int mines,int isSpecial = -1,QWidget *parent = nullptr);
    ~configDialog();

    int _width;
    int _height;
    int _mines;

    int _special;

public slots:
    virtual void accept() override;

    void setEasy();
    void setMedium();
    void setHard();

private slots:
    void on_easyButton_clicked(bool checked);

    void on_mediumButton_clicked(bool checked);

    void on_hardButton_clicked(bool checked);

private:
    Ui::configDialog *ui;

    void closeAllEdit();
    void openAllEdit();

    void setEditText(int width,int height,int mines);

};

#endif // CONFIGDIALOG_H
