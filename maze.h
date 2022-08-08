#ifndef MAZE_H
#define MAZE_H

#include <QWidget>

#include"difficultychooser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Maze; }
QT_END_NAMESPACE

class Maze : public QWidget
{
    Q_OBJECT

public:
    Maze(QWidget *parent = nullptr);
    ~Maze();


protected:

    void paintEvent(QPaintEvent *) override;

private slots:

    void on_StartGame_clicked();

    void on_Back_clicked();

private:
    Ui::Maze *ui;
    DifficultyChooser chooser;

};
#endif // MAZE_H
