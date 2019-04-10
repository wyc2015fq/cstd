#  			[C语言实现贪吃蛇之全靠数组篇](https://www.cnblogs.com/evilkant/p/5722135.html) 		



　　贪吃蛇游戏的设计思路很简单，相信有过一些编程经验的同学都不至于束手无策，可在我刚刚接触编程时，这个小小的贪吃蛇游戏可是让我费了不少脑筋，如今学习编程已经快一年了，前几天又看了一遍K&R，打算写几个贪吃蛇程序巩固一下知识。我打算写若干篇贪吃蛇的博客，从简单粗糙的开始，不断改良，希望能给初学C语言的同学一点借鉴。

　　话不多说，我们现在就开始吧，首先我们整理一下思路。首先我们要明确，既然贪吃蛇游戏理论上可以无限继续下去，那么游戏主体一定就是一个循环。蛇的移动就在这个循环中完成。如果是初学编程的话，可能会好奇如何让蛇移动起来，在这里我们是通过不断改变蛇的坐标，然后根据蛇的坐标不断刷新屏幕在视觉上形成蛇的移动效果。我们先来看看主函数：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 int main()
 2 {
 3     int dir=UP;　　　　　　　　　　　　　　//初始方向默认为向上，UP是个宏
 4     while (1){
 5         print_game();　　　　　　　　　　 //打印游戏
 6         dir=get_dir(dir);　　　　　　　　//获取方向
 7         move_snake(dir);　　　　　　　　 //移动蛇身
 8         if (!isalive())　　　　　　　　　//判断蛇的生命状态
 9             break;
10     }
11     printf("Game Over\n");
12 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

可见游戏的整体思路是比较简单的。有了框架我们再来具体实现，首先考虑使用的数据结构，正如博客名所示，这次我只用数组，不使用链表和结构体。那么我们用一个二维数组char  map[17][17]来表示地图，用一维数组unsigned char  snake[15]来表示蛇的坐标。这里之所以用char是因为本着能省则省的原则，我们这是一个17*17大小的小游戏，char足够了。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define UP 72　　　　　　　　　　　　　　　　　　　　　　　　　　//72,80,75,77是方向键对应的键值
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SNAKE 1
#define FOOD 2
#define BAR 3

char map[17][17] = { 0 };　　　　　　　　　　　　　　　　　　 //初始化地图
unsigned char snake[50] = { 77 };　　　　　　　　　　　　　　//初始化蛇头坐标
unsigned char food = 68;　　　　　　　　　　　　　　　　　　　　//初始化食物坐标
char len = 1;　　　　　　　　　　　　　　　　　　　　　　　　　　//蛇长
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

只用数组做贪吃蛇的话，在蛇坐标与食物坐标的存储上会比较麻烦，毕竟我们的游戏坐标是二维的，这里我采用位运算的方法，一个unsigned  char类型的变量八位，前四位记录x坐标，后四位记录y坐标，四位的话，最大值也就是15，所以我设置的是17*17的大小，（15+边界）*（15+边界）。要是觉得地图太小的话，用int或者long也是可以的。由于我们要经常做数字与x,y的转换，我们把它写成一个函数。

```
void tran(unsigned char num,unsigned char *x, unsigned char *y)
{
    *x = num >> 4;
    *y = (unsigned char)(num << 4) >> 4;　　　　　　//注意这里一定要做个强制类型转换
}
```

碰到被调用函数需要返回两个以上参数的时候，应该像上述函数一样，将要返回的值写成指针参数。

接下来，让我们把主函数里调用的函数一一填上。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void print_game(void)
{
    int i, j;
    for (i = 0; i<17; i++) {　　　　　　　　　　　　//根据地图上每点的情况绘制游戏
        for (j = 0; j<17; j++) {
            if (map[i][j] == 0)
                putchar(' ');
            else if (map[i][j] == SNAKE)
                putchar('*');
            else if (map[i][j] == BAR)
                putchar('#');
            else if(map[i][j]==FOOD)
                putchar('!');
        }
        putchar('\n');
    }
    Sleep(100);　　　　　　　　　　　　　　　　//休眠函数，将进程或线程挂起100ms，包含在window.h中
    system("cls");　　　　　　　　　　　　　　//清屏函数，包含在stdlib.h中
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

获取方向的函数注意考虑蛇身超过一节的蛇不能回头的问题，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
int get_dir(int old_dir)
{
    int new_dir = old_dir;
    if (_kbhit()) {　　　　　　　　//用kbhit()与getch()组合实现键盘响应
        _getch();
        new_dir = _getch();　　　　//getch()函数要使用两次
        if (len > 1 && (abs(new_dir - old_dir) == 2 || abs(new_dir - old_dir) == 8))
            new_dir = old_dir;
    }
    return new_dir;
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

getch()函数要用两次的原因是因为第一次返回的值指示该键扩展的字符，第二次调用才返回实际的键代码。abs为绝对值函数，之所以把绝对值函数与2，8比较是对方向的宏定义观察发现的规律。getch()和kbhit()前面的下划线是因为在vs环境下一些函数需要使用加下划线的版本，更安全。

接下来是移动蛇身的函数（游戏的大部分内容都在其中）：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void move_snake(int dir)
{
    int last = snake[0],current;　　　　//last与current用于之后蛇坐标的更新
    int i, j;
    int grow=0;　　　　　　　　　　　　　　//判断是否要长身体
    unsigned char x, y,fx,fy;　　　　　　//蛇坐标与食物坐标
    tran(food, &fx, &fy);
    tran(snake[0], &x, &y);
    switch (dir){　　　　　　　　　　　　//更新蛇头坐标
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }
    snake[0] = ((x ^ 0) << 4) ^ y;　　　　//将x,y换回一个数
    if (snake[0] == food) {
        grow = 1;
        food = generate_food();　　　　//产生新食物
    }
    for (i = 0; i<len; i++) {　　　　　　//蛇移动的关键，通过将蛇头原来的坐标赋给第二节，原来的第二节赋给第三节，依次下去，完成蛇坐标的更新
        if (i == 0)　　　　　　　　　　　　//如果只有头，跳过，因为前面已更新蛇头坐标
            continue;
        current = snake[i];　　　　　　　　//将当前操作的蛇节坐标存储到current里
        snake[i] = last;　　　　　　　　　　//完成当前操作蛇节坐标的更新
        last = current;　　//last记录的是上一次操作蛇节的坐标，这次操作已经结束，故把current赋给last
    }
    if (grow) {
        snake[len] = last;
        len++;
    }
    for (i = 0; i < 17; i++)　　　　　　　　//将边界与食物加到地图里去
        for (j = 0; j < 17; j++)
            if (i == 0 || i == 16 || j == 0 || j == 16)
                map[i][j] = BAR;
            else if (i == fy&&j == fx)
                map[i][j] = FOOD;
            else
                map[i][j] = 0;
    for (i = 0; i < len; i++) {　　　　　　//将蛇加到地图里去
        tran(snake[i], &x, &y);
        if (snake[i] > 0)
            map[y][x] = 1;
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

其中有个生产食物的函数，generate_food()，它利用随机数生成函数生成食物坐标，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
unsigned char generate_food(void)
{
    unsigned char food_,fx,fy;
    int in_snake=0,i;
    srand((unsigned int)time(NULL));
    do {
        food_ = rand() % 255;
        tran(food_, &fx, &fy);
        for (i = 0; i < len; i++)
            if (food_ == snake[i])
                in_snake = 1;
    } while (fx == 0 || fx == 16 || fy == 0 || fy == 16 || in_snake);
    return food_;
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

随机数生成函数，一般由srand()和rand()函数组成，前者以当前时间为参数提供种子供rand()函数生成更为随机的数。这里用do while语句做了一个筛选，最后产生在边界内且不再蛇身上的食物。

最好剩下一个判断蛇死活的函数，直接上代码吧：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
int isalive(void)
{    
    int self_eat = 0, i;
    unsigned char x, y;
    tran(snake[0], &x, &y);
    for (i = 1; i < len; i++)
        if (snake[0] == snake[i])
            self_eat = 1;
    return (x == 0 || x == 16 || y == 0 || y >= 16 || self_eat) ? 0 : 1;
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

判断了蛇是否撞到边界或者自食。

好了，有了这些函数，把他们组装起来就成了我们的简易贪吃蛇游戏。游戏画面如下：

![img](https://images2015.cnblogs.com/blog/940187/201607/940187-20160730233354419-571288574.png)

虽然相当得简陋，由于用的是全屏刷新的方法，闪屏也比较严重，但也算是麻雀虽小五脏俱全，该有的功能也都有了，代码也不过150行左右。在接下来的博客里我还会继续折腾这个游戏，直到它看起来比较顺眼为止。由于本人还是新手，这也是个新手向的教程，有什么问题欢迎指出。