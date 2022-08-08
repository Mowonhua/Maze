#include "mazehard.h"
#include "ui_mazehard.h"

#define BEGIN 30
#define END 930
#define ROWGAP 30
#define ROWNUM ((END-BEGIN)/ROWGAP)
#define LINENUM (ROWNUM+1)
#define GRIDNUM (ROWNUM*ROWNUM)

MazeHard::MazeHard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MazeHard)
    , sec(0)
    , msec(0)
    , success(true)
    , running(true)
{
    ui->setupUi(this);

    setFocus();
    ui->Play->setFocusPolicy(Qt::NoFocus);

    pixmap = new QPixmap(QWidget::size());
    paint = new QPainter;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));

    initial();
    //初始化角色和终点
    ui->man->move(BEGIN,BEGIN);
    ui->end->move(END-ROWGAP,END-ROWGAP);
    mazegenerator();
}

MazeHard::~MazeHard()
{
    delete ui;
}

int MazeHard::find(int x)//找代表元
{
    if(header[x]==x)
        return x;
    else
        return find(header[x]);
}

void MazeHard::join(int x,int y)//合并集合
{
    int fx=find(x),fy=find(y);
    if(fx!=fy)
        header[fy]=fx;
}

int selectneibourh(int random)
{
    int rand1=rand()%4+1;//1-4分别表示上下左右
    re:
    switch(rand1)
    {
    case 1:
        if(0<=random&&random<=ROWNUM-1)
        {
            rand1=rand()%3+2;
            goto re;
        }
        else
            return random-ROWNUM;
    case 2:
        if(GRIDNUM-ROWNUM<=random&&random<=GRIDNUM-1)
        {
            rand1=rand()%3==0?1:rand()%2+3;
            goto re;
        }
        else
            return random+ROWNUM;
    case 3:
        if(random%ROWNUM==0)
        {
            rand1=rand()%3==0?4:rand()%2+1;
            goto re;
        }
        else
            return random-1;
    case 4:
        if(random%ROWNUM==ROWNUM-1)
        {
            rand1=rand()%3+1;
            goto re;
        }
        else
            return random+1;

    }
    return random;//avoid warning
}

void MazeHard::carveway(int x,int y)//在x,y间打开一条路
{
    linking[x][y]=1;//1代表连通
    linking[y][x]=1;

    //画笔初始化，用白色覆盖制造路径
    QPen pen;
    pen.setColor(QColor(Qt::white));
    pen.setWidth(2);
    paint->begin(pixmap);
    paint->setPen(pen);

    int x1=x%ROWNUM,y1=x/ROWNUM;
    int x2=y%ROWNUM,y2=y/ROWNUM;
    int m=x1<x2||y1<y2?x:y;
    int mx=m%ROWNUM,my=m/ROWNUM;

    //竖线
    int Ux=BEGIN+(mx+1)*ROWGAP;
    int Uy=BEGIN+(my)*ROWGAP+2;
    int Dx=BEGIN+(mx+1)*ROWGAP;
    int Dy=BEGIN+(my+1)*ROWGAP-2;
    //横线
    int Lx=BEGIN+(mx)*ROWGAP+2;
    int Ly=BEGIN+(my+1)*ROWGAP;
    int Rx=BEGIN+(mx+1)*ROWGAP-2;
    int Ry=BEGIN+(my+1)*ROWGAP;

   if(y-x==1||x-y==1)
        paint->drawLine(Ux,Uy,Dx,Dy);
    else
       paint->drawLine(Lx,Ly,Rx,Ry);
    paint->end();
}



void MazeHard::initial()//初始化UI
{
    QLineF line;
    int x1=BEGIN,y1=0,x2=END,y2=0;

    pixmap->fill(Qt::white);//设置白色背景
    paint->begin(pixmap);
    QPen pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(2);
    paint->setPen(pen);

    //画黑色网格图
    for(int i=0;i<LINENUM;i++)
    {
        y1+=ROWGAP;
        y2=y1;

        line.setLine(x1,y1,x2,y2);
        paint->drawLine(line);//画横线

        line.setLine(y1,x1,y2,x2);
        paint->drawLine(line);//画竖线
    }
    paint->end();



}

void MazeHard::mazegenerator()//生成迷宫
{
    if(!header.empty())
        header.clear();
    if(!linking.empty())
        linking.clear();

    header.resize(GRIDNUM);
    linking.resize(GRIDNUM);

    for(int i=0;i<GRIDNUM;i++)
    {
        header[i]=i;//开始每个元素的代表元都是它本身
        linking[i].resize(GRIDNUM);
        linking[i].fill(-1);//-1表示不连通
    }

    //用并查集算法保证迷宫有活路
    srand(time(0));
    while(find(0)!=find(GRIDNUM-1))//当起点终点无路径时
    {
        int random=rand()%GRIDNUM;//0-99
        int neibour=selectneibourh(random);
        if(find(random)!=find(neibour))
        {
            join(random,neibour);
            carveway(random,neibour);
        }
    }
}

void MazeHard::processOneThing()
{
    ui->mSec->setText(QString("%1").arg(++msec));//1ms计一次
    if(msec==1000)
    {
        msec=0;
        ui->Sec->setText(QString("%1").arg(++sec));//1s计一次
    }
    update();//实时更新画面
}

void MazeHard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(QPoint(0, 0), *pixmap);
    if(sec<120&&ui->man->pos()==ui->end->pos())//60内抵达终点则赢
    {
        ui->Outcome->setText("Win!");
        timer->stop();
        success=true;
        running=false;
    }
    else if(sec>=120&&ui->man->pos()!=ui->end->pos())//60秒内未达终点则输
    {
        ui->Outcome->setText("Fail!");
        timer->stop();
        msec=0;
        success=false;
        running=false;
    }

}
int sca=0,offset=0;
bool MazeHard::repaint(int k)
{


    if(k==Qt::Key_Down&&ui->man->pos().y()>=350&&ui->man->pos().y()<END-ROWGAP)
    {
        sca-=30;offset++;
    }
    else if(k==Qt::Key_Up&&ui->man->pos().y()<130)
    {
        sca+=30;offset--;
    }
    else
        return false;


    QLineF line;
    int x1=BEGIN,y1=0,x2=END,y2=0;

    pixmap->fill(Qt::white);//设置白色背景
    paint->begin(pixmap);
    paint->translate(0,sca);

    QPen pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(2);
    paint->setPen(pen);

    //画黑色网格图
    for(int i=0;i<LINENUM;i++)
    {
        y1+=ROWGAP;
        y2=y1;

        line.setLine(x1,y1,x2,y2);
        paint->drawLine(line);//画横线

        line.setLine(y1,x1,y2,x2);
        paint->drawLine(line);//画竖线
    }
    for(int i=0;i<GRIDNUM;i++)
    {
        for(int j=0;j<GRIDNUM;j++)
        {
            if(linking[i][j]==1)
            {

                QPen pen1;
                pen1.setColor(QColor(Qt::white));
                pen1.setWidth(2);
                paint->setPen(pen1);

                int x1=i%ROWNUM,y1=i/ROWNUM;
                int x2=j%ROWNUM,y2=j/ROWNUM;
                int m=x1<x2||y1<y2?i:j;
                int mx=m%ROWNUM,my=m/ROWNUM;

                //竖线
                int Ux=BEGIN+(mx+1)*ROWGAP;
                int Uy=BEGIN+(my)*ROWGAP+2;
                int Dx=BEGIN+(mx+1)*ROWGAP;
                int Dy=BEGIN+(my+1)*ROWGAP-2;
                //横线
                int Lx=BEGIN+(mx)*ROWGAP+2;
                int Ly=BEGIN+(my+1)*ROWGAP;
                int Rx=BEGIN+(mx+1)*ROWGAP-2;
                int Ry=BEGIN+(my+1)*ROWGAP;

               if(j-i==1||i-j==1)
                    paint->drawLine(Ux,Uy,Dx,Dy);
                else
                   paint->drawLine(Lx,Ly,Rx,Ry);


            }
        }
    }
    paint->end();
    return true;
}

void MazeHard::on_Play_clicked()
{
    QPixmap *clearPix = new QPixmap(size());
    clearPix->fill (Qt::white);
    pixmap = clearPix;
    update();
    timer->stop();
    msec=0;sec=0;
    offset=0;sca=0;
    success=true;
    running=true;

    ui->Sec->setText("0");
    ui->mSec->setText("0");
    ui->Outcome->setText(" ");
    ui->man->move(BEGIN,BEGIN);
    initial();
    //初始化角色和终点
    ui->man->move(BEGIN,BEGIN);
    ui->end->move(END-ROWGAP,END-ROWGAP);
    mazegenerator();
}


void MazeHard::keyPressEvent(QKeyEvent *event)
{
    if(!success)//失败可重来
    {
        msec=0;sec=0;
        success=true;
        running=true;
        ui->man->move(BEGIN,BEGIN);
        return;
    }
    else if(success&&!running)
        return;
    else if(running)
    {
        timer->start();
        QPoint m=ui->man->pos();
        QPoint temp=m;
        switch(event->key())
        {
        case Qt::Key_Left:
            if(m.x()>BEGIN)
                m.setX(m.x()-ROWGAP);
            break;
        case Qt::Key_Right:
            if(m.x()<END-ROWGAP)
                m.setX(m.x()+ROWGAP);
            break;
        case Qt::Key_Up:
            if(m.y()>BEGIN-offset*ROWGAP)
                m.setY(m.y()-ROWGAP);
            break;
        case Qt::Key_Down:
            if(m.y()<END-ROWGAP*(offset+1))
                m.setY(m.y()+ROWGAP);
            break;
        }
        int mpos=ROWNUM*((m.y())/ROWGAP-1)+(m.x())/ROWGAP-1+offset*ROWNUM;
        int tempos=ROWNUM*((temp.y())/ROWGAP-1)+(temp.x())/ROWGAP-1+offset*ROWNUM;
        if(linking[tempos][mpos]==1)//有路则执行变化
        {
            if(repaint(event->key()))
                ui->end->move(END-ROWGAP,END-ROWGAP*(offset+1));
            else
                ui->man->move(m);
        }
        else//无路则保持原样
            ui->man->move(temp);
    }
}

void MazeHard::on_Back_clicked()
{
    timer->stop();
    this->close();
}

