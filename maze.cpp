#include "maze.h"
#include "./ui_maze.h"
#include<QDebug>



Maze::Maze(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Maze)
{
    ui->setupUi(this);
}

Maze::~Maze()
{
    delete ui;
}


void Maze::paintEvent(QPaintEvent *event)
{
    //QPixmap pixmap(QWidget::size());
   /* pixmap.load("./background.jpg");
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);*/
}


void Maze::on_StartGame_clicked()
{
    chooser.show();
}


void Maze::on_Back_clicked()
{
    this->close();
}

