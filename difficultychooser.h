#ifndef DIFFICULTYCHOOSER_H
#define DIFFICULTYCHOOSER_H

#include <QWidget>
#include"mazeeasy.h"
#include"mazemedium.h"
#include"mazehard.h"

namespace Ui {
class DifficultyChooser;
}

class DifficultyChooser : public QWidget
{
    Q_OBJECT

public:
    explicit DifficultyChooser(QWidget *parent = nullptr);
    ~DifficultyChooser();

private slots:
    void on_Easy_clicked();

    void on_Medium_clicked();

    void on_Hard_clicked();

    void on_Back_clicked();

private:
    Ui::DifficultyChooser *ui;

    MazeEasy easy;
    MazeMedium medium;
    MazeHard hard;
};

#endif // DIFFICULTYCHOOSER_H
