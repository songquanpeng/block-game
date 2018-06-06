#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>


const int SIZE=30; //单个方块单元的边长
const int AREA_ROW=12; //场景行数
const int AREA_COL=12; //场景列数
const int INFO_ROW=12;
//方向
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    //初始化游戏: 清空分数;清空方块
    void InitGame();
    void StartGame();
    void BlockMove(Direction);

    virtual void paintEvent(QPaintEvent *event); //场景刷新
    virtual void timerEvent(QTimerEvent *event); //定时器事件
    virtual void keyPressEvent(QKeyEvent *event); //键盘响应


private:
    int board[AREA_ROW][AREA_COL];//行列
    int getRandomInt();
    void CubeCuring();//方块固化
    void CreateCube();//产生方块

    //void draw();
    int speed_ms; //固化时间间隔
    int refresh_ms; //刷新时间间隔
    int create_ms;//产生方块时间间隔
    int game_timer; //方块固化计时器
    int paint_timer; //渲染刷新计时器
    int creat_timer;//方块产生计时器
    int timer;//计时器
    int x;
    int y;
    int score;
    int stepNumber;
    int consumeTime;
    bool IsOver;
};

#endif // WIDGET_H
