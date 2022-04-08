#include "configdialog.h"
#include "ui_configdialog.h"
#include<QIntValidator>
#include<QDebug>

configDialog::configDialog(int width,int height,int mines,int isSpecial,QWidget *parent) :
    QDialog(parent),
    _width(width),_height(height),_mines(mines),
    _special(isSpecial),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);

    ui->easyButton->setAutoExclusive(false);
    ui->mediumButton->setAutoExclusive(false);
    ui->hardButton->setAutoExclusive(false);

    switch (isSpecial)
    {
    case 0:
        ui->easyButton->setChecked(true);
        closeAllEdit();
        break;
    case 1:
        ui->mediumButton->setChecked(true);
        closeAllEdit();
        break;
    case 2:
        ui->hardButton->setChecked(true);
        closeAllEdit();
        break;
    default:
        break;
    }
    setEditText(_width,_height,_mines);

}

configDialog::~configDialog()
{
    delete ui;
}

void configDialog::accept()
{
    _width = ui->widthEdit->text().toInt();
    _height = ui->heightEdit->text().toInt();
    _mines = ui->minesEdit->text().toInt();
    QDialog::accept();
}

void configDialog::setEasy()
{
    setEditText(10,10,20);
    _special = 0;
    closeAllEdit();
}

void configDialog::setMedium()
{
    setEditText(30,16,98);
    _special = 1;
    closeAllEdit();
}

void configDialog::setHard()
{
    setEditText(54,27,297);
    _special = 2;
    closeAllEdit();
}

void configDialog::closeAllEdit()
{

    ui->widthEdit->setReadOnly(true);
    ui->heightEdit->setReadOnly(true);
    ui->minesEdit->setReadOnly(true);

}

void configDialog::setEditText(int width, int height, int mines)
{
    ui->widthEdit->setText(QString::number(width));
    ui->heightEdit->setText(QString::number(height));
    ui->minesEdit->setText(QString::number(mines));

}


void configDialog::openAllEdit()
{
    ui->widthEdit->setReadOnly(false);
    ui->heightEdit->setReadOnly(false);
    ui->minesEdit->setReadOnly(false);
    _special = -1;
}


void configDialog::on_easyButton_clicked(bool checked)
{
    if(checked)
    {
        ui->mediumButton->setChecked(false);
        ui->hardButton->setChecked(false);
        setEasy();
    }
    else
        openAllEdit();
}

void configDialog::on_mediumButton_clicked(bool checked)
{
    if(checked)
    {
        ui->easyButton->setChecked(false);
        ui->hardButton->setChecked(false);
        setMedium();
    }
    else
        openAllEdit();
}

void configDialog::on_hardButton_clicked(bool checked)
{
    if(checked)
    {
        ui->easyButton->setChecked(false);
        ui->mediumButton->setChecked(false);
        setHard();
    }
    else
        openAllEdit();
}
