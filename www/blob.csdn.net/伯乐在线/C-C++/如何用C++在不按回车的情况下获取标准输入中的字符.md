# 如何用C++在不按回车的情况下获取标准输入中的字符 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [alexxxx](http://www.jobbole.com/members/pcg941027) 翻译，[aahung](http://www.jobbole.com/members/aahung) 校稿。未经许可，禁止转载！
英文出处：[stackoverflow](http://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
### 题主问题
Adam：因为不是经常用到，所以我总是记不住如何实现这个功能。在 C/C++ 中，不用到下一行（输入回车）就从标准输入获得字符最好的方法是什么呢？
另外如果不用把字符敲入屏幕那肯定是最完美的，但我仅仅想不通过敲控制台（Enter）就获得键盘输入。
### 最佳答案，来自 [Johannes Schaub – litb](http://stackoverflow.com/users/34509/johannes-schaub-litb)：
只用纯C++的话是不太可能满足可移植性的，因为这很大程度上依赖于所使用的终端，其中终端连接了stdin（通常是行缓冲）。但是你可以使用某些库来完成这个功能：
1.Windows  编译器下可用 conio。使用 _getch() 函数让你不用等待回车输入就能得到字符。我不经常使用Windows来进行开发，但是我见过我同学仅仅包含头文件 conio.h 就可以使用了。你可以在维基百科找到 conio.h 。上面列出了 getch() ，在 Visual C++ 中声明为 deprecated（不赞成使用的）。
2.Linux 中可以使用 curses，在 Windows 中也可以使用兼容的 curses 实现。相比于 conio 它也有一个 getch() 函数。（可通过 man getch  来查看它的 manpage）。同样你也可以在维基百科中找到 Curses。
如果你打算让你的程序跨平台运行的话，我推荐你使用 curses。然而我相信有一些你能用来关掉行缓冲的函数（我相信那应该叫做“原始模式”，与“加工模式”，详细可以在 man stty 中查看）。Curses 可以在跨平台跨终端的环境中帮你处理好这个。
**回答中的一些具体代码：**
**来自[Kay](http://stackoverflow.com/users/416224/kay)：**
这段代码使用 kbhit() 检查键盘是否被按下，并用 getch() 获取输入的字符。

C++
```
#include <conio.h>
if (kbhit()!=0) {
    cout<<getch()<<endl;
}
```
**来自[Falcon Momot](http://stackoverflow.com/users/1507797/falcon-momot)：**
在 Linux（或者其他类 UNIX 系统）中可以这样实现：

C++
```
#include <unistd.h>
#include <termios.h>
char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
```
