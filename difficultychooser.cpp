#include "difficultychooser.h"
#include "ui_difficultychooser.h"

DifficultyChooser::DifficultyChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DifficultyChooser)
{
    ui->setupUi(this);
}

DifficultyChooser::~DifficultyChooser()
{
    delete ui;
}

void DifficultyChooser::on_Easy_clicked()
{
    easy.show();
}


void DifficultyChooser::on_Medium_clicked()
{
    medium.show();
}


void DifficultyChooser::on_Hard_clicked()
{
    hard.show();
}


void DifficultyChooser::on_Back_clicked()
{
    this->close();
}

