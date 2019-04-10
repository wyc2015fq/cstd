#  			[C语言实现贪吃蛇之结构链表篇](https://www.cnblogs.com/evilkant/p/5727575.html) 		



　　之前的两篇博客将运用的C语言知识限定在了一般的数组上，但如果已经完整地了解过C语言的话，运用结构和链表会让程序的结构更明了，逻辑更清晰。这篇博客就将介绍如何用结构和链表改善之前的程序。

　　首先，我们为蛇的节点定义一个结构：

```
typedef struct node{
    COORD cor;
    struct node *next;
}node;
```

COORD结构我在上一篇已经介绍过，这里就直接借用了。

```
COORD food = { 3,5 };
node *head;
```

  food也相应地由COORD来定义，并定义头指针。接下来就来改动之前的代码吧。

主函数还是维持原样，然后从主函数里出现的函数一个个改起。

init_game函数做如下改动：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void init_game(void)
{
    head = (node *)malloc(sizeof(node));　　//初始化蛇头
    head->cor.X = 3;
    head->cor.Y = 8;
    head->next = NULL;
    int i, j;
    for (i = 0; i<17; i++) {
        for (j = 0; j<17; j++) {
            if (i == 0 || i == 16 || j == 0 || j == 16)
                putchar('#');
            else if (i == 5&&j == 3)
                putchar('*');
            else if (i == food.Y&&j == food.X)
                putchar('!');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

函数里的3和8是蛇头的初始坐标，这样写会导致程序的可读性下降，修改起来也不方便，可以在函数之前做相应的宏定义改善。

下一个需要改动的函数是move_snake函数。很容易发现我们需要做很多次比较坐标的事，我们干脆把它写成函数：

```
int cor_cmp(COORD pt1, COORD pt2)
{
    return (pt1.X == pt2.X&&pt1.Y == pt2.Y);
}
```

gotoxy函数和generate_food函数也要做一些改动：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void gotoxy(COORD pt)
{
    HANDLE hout;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hout, pt);
}

COORD generate_food(void)
{
    COORD food_;
    node *p=NULL;　　　　　　//把定义的指针初始为NULL是一个好习惯
    int in_snake = 0;
    srand((unsigned int)time(NULL));
    do {
        food_.X = rand() % 16;
        food_.Y = rand() % 16;
        for (p = head; p != NULL; p = p->next)
            if (cor_cmp(food_,p->cor))
                in_snake = 1;
    } while (food_.X == 0 || food_.X == 16 || food_.Y == 0 || food_.Y == 16 || in_snake);
    return food_;
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

好了，有了这些改进过的  函数，我们就可以写出move_snake函数了。这时我们还需要考虑的一个问题是我们该如何利用链表存储蛇的坐标。链表的一个优势在于它是动态的，它的单元数不像数组一样一开始就是被写死的。我们自然而然地想到当蛇吃到食物时，就向链表尾部追加一个单元。至于如何更新蛇的坐标，有两个办法，第一种是像之前数组的方法一样，从蛇头开始向后传递坐标，其代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void move_snake(int dir)
{
    node *p = NULL;
    COORD last=head->cor, current;
    int grow = 0;
    switch (dir) {
    case UP:
        head->cor.Y--;
        break; 
    case DOWN:
        head->cor.Y++;
        break;
    case LEFT:
        head->cor.X--;
        break;
    case RIGHT:
        head->cor.X++;
        break;
    }
    if (cor_cmp(head->cor,food)) {
        grow = 1;
        food = generate_food();
    }
    for (p = head->next; p != NULL; p = p->next) {
        current = p->cor;
        p->cor = last;
        last = current;
    }
    gotoxy(head->cor);
    putchar('*');
    if (grow) {
        for (p = head; p->next != NULL; p = p->next)　　//找到蛇尾
            ;
        p->next = (node *)malloc(sizeof(node));　　　　//追加节点
        p->next->cor = last;
        p->next->next = NULL;
        len++;
        gotoxy(food);
        putchar('!');
    }
    else {
        gotoxy(last);
        putchar(' ');
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

因为和数组更新的方法几乎一致，我就不展开解释了。

另一种更新坐标的方法是每次移动时申请一块新的内存空间存储移动后的蛇头坐标并让head指针指向它，如果蛇不长节的话就释放蛇尾的内存空间，通俗来说就是添头去尾。代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void move_snake(int dir)
{
    node *p,*t = NULL;
    node *newh = NULL;
    newh = (node *)malloc(sizeof(node));　　　　//为新头开辟内存空间
    newh->cor = head->cor;　　　　　　　　　　　　//把旧头的坐标赋给新头
    newh->next = head;　　　　　　　　　　　　　　//把新头的next指针指向旧头
    head = newh;　　　　　　　　　　　　　　　　　　//让头指针指向新开辟的内存空间
    int grow = 0;
    switch (dir) {
    case UP:
        head->cor.Y--;
        break; 
    case DOWN:
        head->cor.Y++;
        break;
    case LEFT:
        head->cor.X--;
        break;
    case RIGHT:
        head->cor.X++;
        break;
    }
    if (cor_cmp(head->cor,food)) {
        grow = 1;
        food = generate_food();
    }
    gotoxy(head->cor);
    putchar('*');
    if (grow) {　　　　　　　　　　//如果长节的话不再对链表进行操作
        len++;
        gotoxy(food);
        putchar('!');
    }
    else {
        for (p = head; p->next->next != NULL; p = p->next)
            ;　　　　　　　　　　　　　　//找到倒数第二节
        t = p->next;　　　　　　　　　　//记录下最后一节的地址
        p->next = NULL;　　　　　　　　//将倒数第二节的next指针赋值为0
        gotoxy(t->cor);
        putchar(' ');
        free(t);　　　　　　　　　　　　//释放蛇尾的内存空间
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

以上就是如何用链表完成坐标更新的两种方式，最后在游戏结束后不要忘了释放申请的内存空间。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void free_node(node *h)
{
    node *p,*q;
    for (p = h; p != NULL; p = q) {
        q = p->next;
        free(p);
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

在printf("GAME OVER")之后，加上free_node(head)就完成这项收尾工作了。

哦，差点忘了给出isalive函数的修改版，贴一下代码吧：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
int isalive(void)
{
    int self_eat = 0;
    node *p = NULL;
    for (p = head->next; p != NULL; p = p->next)
        if (cor_cmp(p->cor, head->cor))
            self_eat = 1;
    return (head->cor.X == 0 || head->cor.X == 16 || head->cor.Y == 0 || head->cor.Y >= 16 || self_eat) ? 0 : 1;
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

到现在，贪吃蛇基本功能的实现对我们来说应该是易如反掌了。在之后的博客我会再编写一些额外的有趣的功能。欢迎大家一起学习探讨。





标签: [C](https://www.cnblogs.com/evilkant/tag/C/)