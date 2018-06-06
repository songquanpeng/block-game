#include "widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Crash it!");
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowOpacity(0.8);
    resize(AREA_ROW*SIZE+INFO_ROW*SIZE,AREA_COL*SIZE);
    InitGame();
}

Widget::~Widget()
{

}

/*
0 : 空方块      白色
1 : 玩家控制方块 黑色
3 : 可消方块    绿色
2 : 固化方块    红色
4 : 地图边界方块 灰色
5 : 渐固化方块   蓝色
*/

void Widget::InitGame()
{
    //初始化随机数种子
    srand(static_cast<unsigned int>(time(0)));
    score = 0;
    stepNumber = 0;
    consumeTime = 0;
    speed_ms=1400;
    refresh_ms=30;
    create_ms=900;
    //初始化地图
    for(int i=0;i<AREA_ROW;++i)
        for(int j=0;j<AREA_COL;++j)
        {
            //qDebug()<<j;
            if(j==0||j==AREA_ROW-1)
            {
                board[i][j]=4;
            }
            else if(i==0||i==AREA_COL-1)
            {
                board[i][j]=4;
            }
            else
            {
                board[i][j]=0;
            }
        }
    //初始化玩家方块
    x = getRandomInt();
    y = getRandomInt();
    board[x][y] = 1;
    //board[4][5] = 2;
    StartGame();
}

void Widget::StartGame()
{

    game_timer=startTimer(speed_ms); //开启游戏timer
    paint_timer=startTimer(refresh_ms); //开启界面刷新timer
    creat_timer=startTimer(create_ms);//开启方块产生timer
    timer=startTimer(1000);//打开计时器
}

void Widget::BlockMove(Direction dir)
{
    bool ok=false;
    bool addScore=false;//注意,在这里一定要初始化!!!否则存在score错误的情况
    switch (dir) {
    case UP:
        if((board[x-1][y]==0)||(board[x-1][y]==3)||(board[x-1][y]==5))
        {
            ok=true;
            if(board[x-1][y]!=0)
            {
                addScore=true;
            }
        }
        else
        {
            ok=false;
        }
        if(ok)
        {
            board[x--][y] = 0;
            board[x][y] = 1;
            ++stepNumber;
            if(addScore)
            {
                ++score;
                addScore=false;
            }
        }
        break;
    case DOWN:
        if((board[x+1][y]==0)||(board[x+1][y]==3)||(board[x+1][y]==5))
        {
            ok=true;
            if(board[x+1][y]!=0)
            {
                addScore=true;
            }

        }
        else
        {
            ok=false;
        }
        if(ok)
        {
            board[x++][y] = 0;
            board[x][y] = 1;
            ++stepNumber;
            if(addScore)
            {
                ++score;
                addScore=false;
            }

        }

        break;
    case LEFT:
        if((board[x][y-1]==0)||(board[x][y-1]==3)||(board[x][y-1]==5))
        {
            ok=true;
            if(board[x][y-1]!=0)
            {
                addScore=true;
            }

        }
        else
        {
            ok=false;
        }
        if(ok)
        {
            board[x][y--] = 0;
            board[x][y] = 1;
            ++stepNumber;
            if(addScore)
            {
                ++score;
                addScore=false;
            }

        }

        break;
    case RIGHT:
        if((board[x][y+1]==0)||(board[x][y+1]==3)||(board[x][y+1]==5))
        {
            ok=true;
            if(board[x][y+1]!=0)
            {
                addScore=true;
            }

        }
        else
        {
            ok=false;
        }
        if(ok)
        {
            board[x][y++] = 0;
            board[x][y] = 1;
            ++stepNumber;
            if(addScore)
            {
                ++score;
                addScore=false;
            }

        }

        break;
    case SPACE:
        break;
    default:
        break;
    }
}

void Widget::CubeCuring()
{
    for(int i=0;i<AREA_ROW;++i)
        for(int j=0;j<AREA_COL;++j)
        {
            if(board[i][j]==3)
            {
                board[i][j]=5;
            }
            else if(board[i][j]==5)
            {
                board[i][j]=2;
            }
        }
    if((board[x-1][y]==2||board[x-1][y]==4)&&
        (board[x+1][y]==2||board[x+1][y]==4)&&
        (board[x][y-1]==2||board[x][y-1]==4)&&
        (board[x][y+1]==2||board[x][y+1]==4))
    {
        IsOver=true;
        InitGame();
    }
}

void Widget::CreateCube()
{
    int temp1,temp2;
    while(1)
    {
        temp1=getRandomInt();
        temp2=getRandomInt();
        if(board[temp1][temp2]==0)
        {
            board[temp1][temp2]=3;
            break;
        }
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //绘制边框
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    painter.drawRect(0,0,AREA_COL*SIZE ,AREA_ROW*SIZE );
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    int rate;
//    speed_ms=5000;
//    refresh_ms=30;
//    create_ms=1000;
    //create_ms=1000*(10/consumeTime);
//    create_ms-=(5*consumeTime);
//    if(consumeTime>10)
//        create_ms = 500;
//    if(consumeTime>20)
//        create_ms = 250;
//    if(consumeTime>30)
//        create_ms = 100;
//    game_timer=startTimer(create_ms);
    rate = 200*score/(stepNumber+1);
    painter.drawText(QRect(AREA_COL*SIZE,0,INFO_ROW*SIZE,SIZE*AREA_ROW/2),Qt::AlignCenter,
                     "SCORE: "+QString::number(score)+"\nSTEP: "+QString::number(stepNumber)+"\nCONSUMING TIME: "+QString::number(consumeTime));
    painter.drawText(QRect(AREA_COL*SIZE,SIZE*AREA_ROW/2,INFO_ROW*SIZE,SIZE*AREA_ROW/2),Qt::AlignCenter,"Rate: "+QString::number(rate)+"\nPRESS SPACE TO \nEXIT");


    //"\nRate: "+QString::number(rate)
    //绘制方块
    for(int i=0;i<AREA_ROW;++i)
        for(int j=0;j<AREA_COL;++j)
        {
            if(board[i][j]==2)//固化方块
            {
                painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
                painter.drawRect(j*SIZE,i*SIZE,SIZE,SIZE);    // 参数 x,y,width,height
            }
            else if(board[i][j]==4)//边界方块
            {
                painter.setBrush(QBrush(Qt::gray,Qt::SolidPattern));
                painter.drawRect(j*SIZE,i*SIZE,SIZE,SIZE);    // 参数 x,y,width,height
            }
            else if(board[i][j]==3)//可消方块
            {
                painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
                painter.drawRect(j*SIZE,i*SIZE,SIZE,SIZE);    // 参数 x,y,width,height
            }
            else if(board[i][j]==5)//渐固化方块
            {
                painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
                painter.drawRect(j*SIZE,i*SIZE,SIZE,SIZE);    // 参数 x,y,width,height
            }
            else if(board[i][j]==1)//玩家方块
            {
                painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
                painter.drawRect(j*SIZE,i*SIZE,SIZE,SIZE);    // 参数 x,y,width,height
            }
        }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        BlockMove(UP);
        break;
    case Qt::Key_Down:
        BlockMove(DOWN);
        break;
    case Qt::Key_Left:
        BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove(RIGHT);
        break;
    case Qt::Key_Space:
        QWidget::close();
        break;
    default:
        break;
    }
}

void Widget::timerEvent(QTimerEvent *event)
{

    //刷新画面
    if(event->timerId()==paint_timer)
        update();
    //方块固化
    if(event->timerId()==game_timer)
        CubeCuring();
    //产生方块
    if(event->timerId()==creat_timer)
        CreateCube();
    if(event->timerId()==timer)
        ++consumeTime;
}


inline int Widget::getRandomInt()
{
    return rand()% 10 +1;
}
