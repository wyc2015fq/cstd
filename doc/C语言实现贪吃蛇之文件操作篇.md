#  			[C语言实现贪吃蛇之文件操作篇](https://www.cnblogs.com/evilkant/p/5731119.html) 		



　　对于C语言的初学者来说，可能会觉得文件操作可能是一个比较陌生的领域，但实际上无论编写怎样的程序文件操作都是必需的。在操作系统中，所有的外围设备（包括键盘和显示器）都被看作是文件系统中的文件，因此所有的输入/输出都要通过读文件/写文件完成。启动一个C语言程序时，操作环境负责打开标准输入，标准输出，标准错误3个文件，并将这3个文件的指针提供给该程序。而我们熟知的getchar函数putchar函数其实就是通过这些文件指针实现的。这样说大家应该对文件操作的重要性有了一个初步的了解，接下来我们就运用文件操作的相关知识，为贪吃蛇游戏加上存档读档功能。

　　如何存储画面的信息呢，在贪吃蛇游戏中，我们只需要存储蛇的坐标食物的坐标以及蛇的前进方向。我们可以利用标准库里的fopen,fwrite,fread三个函数完成这一要求。关于这三个函数的用法，读者均可自行查阅。需要注意的是，在VS开发环境下，必须使用fopen_s才行。

我们先来编写存储游戏的函数save()，首先是一些变量的定义。

```
COORD snake[50];
FILE *mem;
errno_t err;
char dir;　　　//因为在save函数里要用到，把原来定义在主函数里的dir移出来
```

save函数：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void save()
{
    node *p=NULL;
    if ((err = fopen_s(&mem, "memory.txt", "w+")) == NULL) {
        fwrite(&len, sizeof(len), 1, mem);
        fwrite(&dir, sizeof(dir), 1, mem);
        fwrite(&food, sizeof(food), 1, mem);
        for (p = head; p != NULL; p = p->next)
            fwrite(&(p->cor), sizeof(p->cor), 1, mem);
        fclose(mem);
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

load函数的文件操作部分只是把fwrite变成了fread，另外还要创立len长度的链表并一一赋值，最后打印出蛇和食物就好了。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void load()
{
    node *p, *q;
    int i,j;
    if ((err = fopen_s(&mem, "memory.txt", "r")) == NULL) {　//读数据
        fread(&len, sizeof(len), 1, mem);
        fread(&dir, sizeof(dir), 1, mem);
        fread(&food, sizeof(food), 1, mem);
        fread(snake, sizeof(COORD), len, mem);
        fclose(mem);
    }
    p = head = (node *)malloc(sizeof(node));　　//创立len长的链表
    p->cor = snake[0];
    for (i = 1; i < len; i++) {　　　　//这里的p,q和数组篇的last,current起的作用一样
        q = (node *)malloc(sizeof(node));　　
        q->cor = snake[i];
        p->next = q;
        p = q;
    }
    p->next = NULL;
    for (i = 0; i < 17; i++) {　　//打印边界
        for (j = 0; j < 17; j++)
            if (i == 0 || i == 16 || j == 0 || j == 16)
                putchar('#');
            else
                putchar(' ');
        putchar('\n');
    }
    for (p = head; p->next != NULL; p = p->next) {　　//打印蛇身
        gotoxy(p->cor);
        putchar('*');
    }
    gotoxy(food);　　//打印食物
    putchar('!');
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

有了save和load函数，接下来我们需要考虑的是什么时候需要存储，显然只有当游戏中途退出时需要存储。那么我们加上ESC键退出的功能。

```
#define ESC 27
```

稍微改变下get_dir函数。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
int get_dir(int old_dir)
{
    int new_dir = old_dir;
    if (_kbhit()) {
        if (_getch() == ESC){
            save();
            free_node(head);
            system("cls");
            exit(0);
        };
        new_dir = _getch();
        if (len > 1 && (abs(new_dir - old_dir) == 2 || abs(new_dir - old_dir) == 8))
            new_dir = old_dir;
    }
    return new_dir;
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

注意这里ESC应该放在第一个getch()处检测。ESC不同于方向键，第一次便返回键值。

既然加入了存档与读档功能，那么就必须得有个新的游戏与继续游戏的选择界面。我们在主函数的循环开始前加入choose函数实现这个界面。

先给新的游戏和继续游戏两个选项选个位置：

```
COORD s_option = { 5,5 },c_option = { 5,7 };
```

函数代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void choose(void)
{
    int key, option = 1;
    gotoxy(s_option);
    printf("->新的游戏");
    gotoxy(c_option);
    printf("  继续游戏");
    while ((key = _getch()) != ENTER) {
        if (key == UP&&option > 1)
            option--;
        if (key == DOWN&&option < 2)
            option++;
        if (option == 1) {
            gotoxy(s_option);
            printf("->");
            gotoxy(c_option);
            printf("  ");
        }
        else {
            gotoxy(c_option);
            printf("->");
            gotoxy(s_option);
            printf("  ");
        }
    }
    system("cls");
    if (option == 1)
        init_game();
    else
        load();
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

option的值n代表第n个选项。这样写之后运行发现光标会一直在闪烁，我们干脆把光标给隐藏了，稍微修改一下gotoxy函数：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void gotoxy(COORD pt)
{
    HANDLE hout;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hout, pt);
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
    SetConsoleCursorInfo(hout, &cursor_info);
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

后面两行代码起隐藏光标的作用。若有疑惑，均可查阅MSDN。

在主函数中去掉init_game函数，在循环前加上choose函数，我们就完成了存档与读档的功能。





标签: [C](https://www.cnblogs.com/evilkant/tag/C/)