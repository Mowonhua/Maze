#ifndef MAZEMEDIUM_H
#define MAZEMEDIUM_H

#include <QWidget>
#include<QPainter>
#include<QTimer>
#include<QVector>
#include<QKeyEvent>

namespace Ui {
class MazeMedium;
}

class MazeMedium : public QWidget
{
    Q_OBJECT

public:
    explicit MazeMedium(QWidget *parent = nullptr);
    ~MazeMedium();

    void initial();
    void mazegenerator();
    int find(int);
    void join(int,int);
    void carveway(int,int);

protected:

    void keyPressEvent(QKeyEvent *) override;
    void paintEvent(QPaintEvent *) override;

private slots:

    void processOneThing();

    void on_Play_clicked();

    void on_Back_clicked();

private:
    Ui::MazeMedium *ui;

    QPixmap *pixmap;
    QPainter *paint;
    QTimer *timer;

    QVector<int> header;//代表元
    QVector<QVector<int>> linking;//二维向量记录两个格是否连通

    bool success,running;
    int msec,sec;
};

#endif // MAZEMEDIUM_H
