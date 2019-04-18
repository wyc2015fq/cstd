# Linux curses库使用 - 深之JohnChen的专栏 - CSDN博客

2007年09月11日 13:29:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：25562


                 相信您在网路上一定用过如 tin,elm 等工具, 这些软体有项共同的特色, 即他们能利用上下左右等方向键来控制游标的位置. 除此之外, 这些程式的画面也较为美观. 对Programming 有兴趣的朋友一定对此感到好奇, 也许他能在 PC上用Turbo C轻易地写出类似的程式, 然而, 但当他将相同的程式一字不变地移到工作站上来编译时, 却出现一堆抓也抓不完的错误. 其实, 原因很简单, 他使用的函式库可能在 UNIX 上是没有定义的. 有些在 Turbo-C 上被广泛使用的一些函式, 可能在 UNIX 上是不被定义的. 为了因应网路上各式各样的终端机形态 (terminal), UNIX 上特别发展出一套函式库, 专门用来处理 UNIX 上游标移动及萤幕的显示. 这就是本篇文章要为您介绍的 - curses.h 函式库. 利用这个函式库, 您也可以写出像 elm 般利用方向键来移动光棒位置的程式.

1. curses 的历史与版本:

    cureses 最早是由柏克莱大学的 Bill Joy 及 Ken Arnold 所发展出来的. 当时发展此一函式库主要原因是为了提高程式对不同终端机的相容性而设计的. 因此, 利用 curses 发展出来的程式将和您所使用的终端机无关. 也就是说, 您不必担心您的程式因为换了一部终端机而无法使用. 这对程式设计师而言, 尤其是网路上程式的撰写, 是件相当重要的一件事. curses之所以能对上百种以上的终端机工作, 是因为它将所有终端机的资料, 存放在一个叫 termcap 的资料库, ( 而在第二版的 System V 系统中, 新版的 curses 以 terminfo 取代原来的 termcap). 

    有了这些记录, 程式就能够知道遇到哪一种终端机时, 须送什麽字元才能移动游标的位置, 送什麽字元才能清除整个萤幕清除. (* 注一) 

另外, 本文的介绍 以 System V 的 curses 版本为主. 

2. 如何在您的程式使用 curses:

在您的 C 程式的档头将 <cureses.h>include 进来.当您引进 curses.h 这个函式库後, 系统会自动将 <stdio.h>和 <unctl.h>一并 include 进来. 

另外, 在 System V 版本中, <terminfo.h>这个函式库也将一并include进来. 

#include <curses.h>

main()

{

: :

: :

}

3. 如何编译(compile):

当您编辑好您的程式, 在 UNIX 提示符号下键入: 

gcc [file.c] -lcurses

或 

gcc [file.c] -lcurses -ltermlib(*注二) 

4. 如何开始我的第一个 curses 程式:

在开始使用 curses 的一切命令之前, 您必须先利用 initscr()这个函式来开启 curses 模式. 

相对的, 在结束 curses 模式前 ( 通常在您结束程式前 ) 也必须以endwin()来关闭 curses 模式.

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <curses.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)main()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    initscr();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    : :
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    : : 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    : :
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    endwin();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

这是一般 curses 程式标准的模式. 

此外, 您可以就您程式所须, 而做不同的设定. 当然, 您可以不做设定,而只是呼叫 initscr(). 您可以自己写一个函式来存放所有您所须要的设定. 平常使用时, 只要呼叫这个函式即可启动 curses 并完成一切设定. 下面的例子, 即是笔者将平常较常用的一些设定放在一个叫 initial()的函式内.

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void initial()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    initscr();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    cbreak();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    nonl();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    noecho();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    intrflush(stdscr,FALSE);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    keypad(stdscr,TRUE);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    refresh();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

 各函式分别介绍如下:
- initscr(): initscr() 是一般 curses 程式必须先呼叫的函数, 一但这个函数被呼叫之後, 系统将根据终端机的形态并启动 curses 模式. 
- endwin(): curses 通常以呼叫 endwin() 来结束程式. endwin() 可用来关闭curses 模式, 或是暂时的跳离 curses 模式. 如果您在程式中须要call shell ( 如呼叫 system() 函式 ) 或是需要做 system call, 就必须先以 endwin() 暂时跳离 curses 模式. 最後再以wrefresh() doupdate() 来重返 curses 模式. 
- cbreak() and nocbreak(): 当 cbreak 模式被开启後, 除了 DELETE 或 CTRL 等仍被视为特殊控制字元外一切输入的字元将立刻被一一读取.当处於 nocbreak 模式时, 从键盘输入的字元将被储存在 buffer 里直到输入 RETURN或 NEWLINE.在较旧版的 curses 须呼叫 crmode(),nocrmode() 来取代 cbreak(),nocbreak()  
- nl() and nonl(): 用来决定当输入资料时, 按下 RETURN 键是否被对应为 NEWLINE 字元 ( 如 /n ). 而输出资料时, NEWLINE 字元是否被对应为 RETURN 和 LINDFEED系统预设是开启的. 
- echo() and noecho(): 此函式用来控制从键盘输入字元时是否将字元显示在终端机上.系统预设是开启的. 
- intrflush(win,bf): 呼叫 intrflush 时须传入两个值, win 为一 WINDOW 型态指标, 通常传入标准输出入萤幕 stdscr. bf 为 TRUE 或 FALSE. 当 bf 为 true 时, 当输入中断字元 ( 如 break) 时, 中断的反应将较为快速.但可能会造成萤幕的错乱.  
- keypad(win,bf): 呼叫 keypad 时须传入两个值, win 为一 WINDOW 型态指标, 通常传入标准输出入萤幕 stdscr. bf 为 TRUE 或 FALSE. 当开启 keypad 後, 可以使用键盘上的一些特殊字元, 如上下左右>等方向键, curses 会将这些特殊字元转换成 curses.h 内定义的一些特殊键. 这些定义的特殊键通常以 KEY_ 开头. 
- refresh(): refresh() 为 curses 最常呼叫的一个函式. curses 为了使萤幕输出入达最佳化, 当您呼叫萤幕输出函式企图改变萤幕上的画面时, curses 并不会立刻对萤幕做改变, 而是等到refresh() 呼叫後, 才将刚才所做的变动一次完成. 其馀的资料将维持不变. 以尽可能送最少的字元至萤幕上. 减少萤幕重绘的时间.如果是 initscr() 後第一次呼叫 refresh(), curses 将做清除萤幕的工作.  

5. 游标的控制:
- move(y,x): 将游标移动至 x,y 的位置. 
- getyx(win,y,x): 得到目前游标的位置. (请注意! 是 y,x 而不是&y,&x ) 

6. 有关清除萤幕的函式:

clear() and erase(): 将整个萤幕清除. (请注意配合refresh() 使用)

7. 如何在萤幕上显示字元:
- echochar(ch): 显示某个字元. 
- addch(ch): 显示某个字元. 
- mvaddch(y,x,ch): 在(x,y) 上显示某个字元. 相当於呼叫move(y,x);addch(ch); 
- addstr(str): 显示一串字串. 
- mvaddstr(y,x,str): 在(x,y) 上显示一串字串. 相当於呼叫move(y,x);addstr(str); 
- printw(format,str): 类似 printf() , 以一定的格式输出至萤幕. 
- mvprintw(y,x,format,str): 在(x,y) 位置上做 printw 的工作. 相当於呼叫move(y,x);printw(format,str); 

8. 如何从键盘上读取字元:
- getch(): 从键盘读取一个字元. (注意! 传回的是整数值) 
- getstr(): 从键盘读取一串字元. 
- scanw(format,&arg1,&arg2...): 如同 scanf, 从键盘读取一串字元. 

例:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int ch;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)char string1[80]; /**//* 请注意! 不可宣告为 char *string1; */
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)char string2[80];
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)echo(); /**//* 开启 echo 模式, 使输入立刻显示在萤幕上 */
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ch=getch();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)string1=getstr();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)scanw("%s",string2);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)mvprintw(10,10,"String1=%s",string1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)mvprintw(11,10,"String2=%s",string2);

 9. 如何利用方向键:

    curses 将一些如方向键等特殊控制字元, 以 KEY_ 为开头定义在 curses.h这个档案里头.

    如 KEY_UP 即代表方向键的 " ↑ ". 但, 如果您想使用curses.h 所为您定义的这些特殊键的话, 您就必须将 keypad 设定为TRUE. 否则, 您就必须自己为所有的特殊键定义了. curses.h 为一些特殊键的定义如下:
- KEY_UP 0403 ↑ 
- KEY_DOWN 0402 ↓ 
- KEY_LEFT 0404 ← 
- KEY_RIGHT 0405 → 
- KEY_HOME 0406 Home key (upward+left arrow) 
- KEY_BACKSPACE 0407 backspace (unreliable) 
- KEY_F0 0410 Function keys. 
- KEY_F(n) (KEY_F0+(n)) formula for f . 
- KEY_NPAGE 0522 Next page 
- KEY_PPAGE 0523 Previous page 

以上仅列出笔者较常使用的一些控制键, 至於其他控制键的定义, 请自行参阅 man curses (* 注三).

一并为您列出其他常用的一些特殊字元:
- [TAB] /t 
- [ENTER] /r 
- [ESC] 27 
- [BACKSPACE] 127 

10. 如何改变萤幕显示字元的属性:

    为了使输出的萤幕画面更为生动美丽, 我们常须要在萤幕上做一些如反白, 闪烁等变化. curses 定义了一些特殊的属性, 透过这些定义, 我们也可以在 curses 程式控制萤幕的输出变化.
- attron(mod): 开启属性. 
- attroff(mod): 关闭属性. 

curses.h 里头定义了一些属性, 如: 
- A_UNDERLINE: 加底线. 
- A_REVERSE: 反白. 
- A_BLINK: 闪烁. 
- A_BOLD: 高亮度. 
- A_NORMAL: 标准模式(只能配合 attrset() 使用) 

    当使用 attron() 开启某一种特殊属性模式後, 接下来在萤幕的输出都会以该种属性出现. 直到您呼叫 attroff() 将此模式关闭. 请注意, 当您欲 attron() 开启另一种属性时, 请记得利用attroff()先关闭原来的属性, 或直接以attrset(A_NORMAL) 将所有特殊属性关闭.否则, curses 会将两种属性做重叠处理. 

例:

attrset(A_NORMAL); /* 先将属性设定为正常模式 */ 

attron(A_UNDERLINE); /* 加底线 */

mvaddstr(9,10,"加底线"); /* 加底线输出一串字元 */

attroff(A_UNDERLINE); /* 关闭加底线模式, 恢复正常模式 */

attron(A_REVERSE); /* 开启反白模式 */

mvaddstr(10,10,"反白"); /* 输出一串反白字元 */

attroff(A_REVERSE); /* 关闭反白模式, 恢复正常模式 */

attron(A_BLINK); /* 开启闪烁模式 */

mvaddstr(11,10,"闪烁"); /* 输出一串闪烁字元 */

attroff(A_BLINK); /* 关闭闪烁模式, 恢复正常模式 */

attron(A_BOLD); /* 开启高亮度模式 */

mvaddstr(12,10,"高亮度"); /* 输出一串高亮度字元 */

attroff(A_BOLD); /* 关闭高亮度模式, 恢复正常模式 */

11. 其他一些函数:
- beep(): 发出一声哔声. 
- box(win,ch1,ch2): 自动画方框.
    - ch1: 画方框时垂直方向所用字元. 
- ch2: 画方框时水平方向所用字元. 
- example: box(stdscr,'|','-'); 将以 | 及 - 围成一个方框. 

12. 应用完整例:

    下面所举的例子, 即完全利用刚刚所介绍的含式来完成.这个程式可将从键盘上读取的字元显示在萤幕上, 并且可以上下左右方向键来控制游标的位置, 当按下 [ESC] 後, 程式即结束. 您有没有发现, 这不就是一个简单全萤幕编辑器的雏形吗?

实例1:

#include <curses.h>
#include <stdio.h>
#define StartX 1
#define StartY 1
void initial();
main()
{
   int x = StartX;
   int y = StartY;
   int ch;
   initial();
   box(stdscr,'|','-');
   attron(A_REVERSE);
   mvaddstr(0,20,"curses program");
   attroff(A_REVERSE);
   move(x,y);
   do{
      ch = getch();
      switch(ch)
      {
        case KEY_UP:
           --y;
           break;
            case KEY_DOWN: 
                ++y; 
                break;
            case KEY_RIGHT: 
                ++x; 
                break;
            case KEY_LEFT: 
                --x; 
                break;
            case ' ': 
                ++y;
                x=0;
                break;
            case '/t': 
                x+=7;
                break;
            case KEY_BACKSPACE: 
                mvaddch(y,--x,' ');
                break;
            case 27: 
                endwin(); 
                exit(1); 
            default: 
                addch(ch); 
                x++;
               break;
      }
      move(y,x);
   }while(1);
}

void initial()
{
 initscr();
 cbreak();
 notnl();
 noecho();
 intrflush(stdscr,false);
 keypad(stdscr,true);
 refresh();
}

实例2:

#include <curses.h>

int main()
{
 initscr();

 box(stdscr, '|', '-');  /*draw a box*/
 move(LINES/2, COLS/2);              /*move the cursor to the center*/
 waddstr(stdscr, "Hello, world!");  
 refresh();
 getch();

 endwin();
 return 0;
}

curses库函数说明

--------------------------------------------------------  
INDDW*curscr：当前屏幕 
WINDOW *stdscr：标准屏幕 
int LINES：终端上的行数 
int COLS：终端上的列数 
bool TRUE：真标志，1 
bool FALSE：假标志，0 
int ERR：错误标志，－1 
int OK：OK标志，0 
--------------------------------------------- 
win：WINDOW * 
bf：bool 
ch：chty pe 
str：char * 
chstr：chty pe* 
fmt：char * 
否则：int  
--------------------------------------------- 
初始化 
WINDOW *initscr() 
SCREEN*newterm(char *type, FILE *outfd, FILE *infd) 
对用户访问的每个终端都应该调用newterm 
t y p e是终端的名称，包括在$ T E R M中(如ansi, xterm, vt100等等) 
SCREEN*set_term(SCREEN *new) 
用户可以切换当前终端。所有的函数都将在设置的当前终端上起作用。 
int endwin() 
退出程序之前，关闭所有打开的窗口。 
之后还可以调用refresh()， 
int isendwin() 
ifendwin()->;refresh()，TRUE，|FALSE。 
----------------------- 
WINDOW *newwin(nlines,ncols,begy,begx) 
begy和begx是窗口左上角的坐标。nlines是一个整数， 
存放着行的数目，而ncols,存放着列的数目。 
所有的参数设置为零，则打开的窗口的大小将与屏幕的大小相同。 
int delwin(win) 
它删除窗口win。如果存在子窗口，以前先要删除这些子窗口。这个函数将 
释放win所占据的所有资源。在调用endwin()之前用户应该删除所有的窗口 
int mvwin(win,by,bx) 
它将把窗口移到坐标(by,bx)处。如果把窗口移出屏幕边界的范围，则ERR 
WINDOW *subwin(origwin,nlines,ncols,begy,begx) 
它返回一个位于origwin窗口中间的子窗口。如果用户改变这两个窗口 
(origwin或者那个新窗口)中的一个，则这种改变将会同时反映到这两 
个窗口上。在下一次调用refresh()之前，先要调用touchwin(origwin) 
begx和begy是相对于屏幕的，而不是相对于origwin的。 
WINDOW *derwin(origwin,nlines,ncols,begy,begx) 
此函数与subwin(.)相同，只不过这里的begx和begy是相对于origwin的，而不是相对于屏幕的。 
mvderwin(win,y,x) 
此函数将把win移到父窗口内。(注意：此函数尚未实现)。 
WINDOW *dupwin(win) 
此函数复制窗口win。 
int overlay(win1,win2) 
将把win1中的所有文本拷贝到win2中，但是不拷贝空格。 
int overwrite(win1,win2) 
也是做文本拷贝工作的函数，但它拷贝空格。 
int copywin(win1,win2,sminrow,smincol,dminrow, 
dmincol,dmaxrow,dmaxcol,overlay) 
它所做的工作与overlay(.)和overwrite(.)相似，但是该函数还可以让程序员选择拷贝窗口的哪个区域。 
int addch(ch) 
int waddch(win,ch) 
int mvaddch(y,x,ch) 
int mvwaddch(win,y,x,ch) 
---------------- 
int addstr(str) 
int addnstr(str,n) 
int waddstr(win,str) 
int waddnstr(win,str,n) 
int mvaddstr(y,x,str) 
int mvaddnstr(y,x,str,n) 
int mvwaddstr(win,y,x,str) 
int mvwaddnstr(win,y,x,str,n) 
----------------------------- 
int addchstr(chstr) 
int addchnsrtr(chstr,n) 
int waddchstr(win,chstr) 
int waddchnstr(win,chstr,n) 
int mvaddchstr(y,x,chstr) 
int mvaddchnstr(y,x,chstr,n) 
int mvwaddchstr(win,y,x,chstr) 
int mvwaddchnstr(win,y,x,chstr,n) 
*****chstr是指向ch type数组的一个指针。***** 
--------------------------- 
int echochar (ch) 
int wechochar (win,ch) 
--------------------------- 
int printw(fmt,..) 
int wprintw(win.fmt,....) 
int mvprintw(y,x,fmt,...) 
int mvwprntw(win,y,x,fmt,..) 
--------------------------- 
int insch(c) 
int winsch(win,c) 
int mvinsch(y,x,c) 
int mvwinsch9win,y,x,c) 
这些函数把字符ch插入到光标的左边， 
光标后面的所有字符则向右移动一个位置。 
在这一行最右端的字符可能会丢失。 
--------------------------- 
int insert() 
int winsert(win) 
在当前行的上方插入一个空行(最底下的一行将被丢失)。 
--------------------------- 
int insdelin(n) 
int winsdelln(win,n) 
如果n为正数，则这些函数将在适当的窗口的当前光标上方插入n行 
(这样一来最底下的n行将丢失)；如果n为负数，则光标下面的n行将被删除， 
余下的行将上升，顶替它们的位置。这些函数将在当前光标的左边插入str 
(字符的个数不能超过一行的限度)。在光标右边的字符将右移，如果到达行 
尾，则字符将丢失，光标位置不变。y和x是指在插入str以前先要把光标移动 
到的坐标，n是要插入的字符的数目(如果n为0则插入整个字符串)。 
--------------------------- 
int insstr(str) 
int insnstr(str,n) 
int winsstr(win,str) 
int winsnstr(win,str,n) 
int mvinsstr9y,x,str) 
int mvinsnstr(y,x,str,n) 
int mvwinsstr(win,y,x,str) 
int mvwinsnstr(win,y,x,str,n) 
这些函数将在当前光标的左边插入str(字符的个数不能超过一行的限度)。 
在光标右边的字符将右移，如果到达行尾，则字符将丢失，光标位置不变。 
y和x是指在插入str以前先要把光标移动到的坐标，n是要插入的字符的数目 
(如果n为0则插入整个字符串)。 
--------------------------- 
int delch() 
int wdelch(win) 
int mvdelch(y,x) 
int mvwdelch(win,y,x) 
删除光标左边的字符，并把光标右边余下的字符向左移动一个位置。 
y和x是在进行删除操作以前要把光标移动到的坐标。 
int deleteln() 
int wdeleteln(win) 
删除光标下面的一行，并把下面所有的其他行都向上移动一个位置。 
此外，屏幕最底下的一行将被清除。 
----------------------------- 
方框和直线 

int border(ls,rs,ts,bs,tl,tr,bl,br) 
int wborder(win,ls,rs,ts,bs,tl,tr,bl,br) 
int box(win,vert,hor) 
这些函数在窗口的边界(或者win的边界)画上方框。在下面的表格中， 
读者将可以看到字符，以及它们的默认值。当用零去调用box(.)时将 
会用到这些默认值。在下面的图中读者可以看到方框中字符的位置 
------------- 
int vline9ch,n) 
int wvline(win,ch,n) 
int hline(ch,n) 
int whline(win,ch,n) 
这些函数将从当前光标位置开始画一条水平线或者垂直线。 
ch是画线所使用的字符，n是要画的字符的个数，光标位置并不移动。 
-------------------------- 
void bkgdset(ch) 
void wbkgdset(win,ch) 
这两个函数设置窗口或者屏幕的背景字符和属性。ch的属性将和窗口中 
所有非空格的字符的属性进行OR操作。背景是窗口的一部分， 
将不会随着滚动、输入或输出而改变 
int bkgd(ch) 
int wbkgd(win,ch) 
--------------------------------- 
int getch() 
int wgetch() 
int mvgetch(y,x) 
int mvwgetch(win,y,x) 
getch()将从终端读取输入，读取的方式取决于是否设置了延迟模式。 
如果设置了延迟模式，则getch()将一直等待，直到用户按下一个键为止； 
如果没有设置延迟模式，则它将返回输入缓冲区中的数据，如果输入缓冲区 
为空，则它将返回ERR。mvgetch(.)和mvwgetch(.)首先把光标移动到位置 
(y,x)上。名称中有w字母的函数将从与窗口win相关的终端读取输入， 
getch()和mvgetch(.)则从屏幕相关的终端读取。如果使能了keypad(.) 
在用户按下某个功能键时，getch()将返回一个代码，该代码在.h头文件中被 
定义为KEY_*宏。如果用户按下Esc键(它可能会是某个组合功能键的第 
一个键)，则ncurses将启动一个否则就返回功能键的值。(如果需要的话， 
可以使用notimeout()来关闭第二个定时器 
int ungetch() 
这个函数将把字符ch送回输入缓冲区。 
--------------------- 
int getstr(str) 
int wgetstr(win,str) 
int mvgetstr(y,x,str) 
int mvwgetstr(win,y,x,str) 
int wgetnstr(win,str,n) 
这些函数的作用相当于对getch()进行一系列的调用，直到接收到一个新行。 
行中的字符存放在str中(所以，在调用getstr(.)之前，不要忘记给字符指针 
分配内存)。如果打开了回送，则字符串将被显示出来(使用noecho()可以关 
闭回送)而用户的删除字符以及其他特殊字符也会被解释出来 
----------------- 
chty peinch(0 
chty pewinch(win) 
chty pemvinch(y,x) 
chty pemvwinch(win,y,x) 
这些函数从屏幕或窗口返回一个字符，因为返回值的类型是chty pe， 
所以还包括了属性信息。这一信息可以使用常量A_*从字符中扩展得到。 
------------------- 
int scanw(fmt,...) 
int wscanw(win,fmt,...) 
int mvscanw(y,x,fmt,...) 
int mvwscanw(win,y,x,fmt,...) 
int vwscanw(win,fmt,va_list.) 
---------------------------- 
输出选项 
int idlok9win,bf) 
void fdcok(win.bf) 
这两个函数为窗口使能或者关闭终端的insert/delete特征(idlok(.)针对一 
行，而idcok(.)则针对字符)。(注：idcok(.)尚未实现) 
void immedok(win,bf) 
bf设置为TRUE，则对窗口win的每一次改变都将导致物理屏幕的一次刷新。 
这将使程序的性能降低，所以默认的值是FALSE。(注：此函数尚未实现) 
int clearok(win,bf) 
如果bf值为TRUE，则下一次调用wrefresh(win)时将会清除屏幕， 
并完全地把它重新画一遍(就像用户在编辑器vi中按下Ctrl+L一样)。 
int leaveok(win,bf) 
默认的行为是，ncurses让物理光标停留在上次刷新窗口时的同一个位置上。 
不使用光标的程序可以把leaveok(.)设置为TRUE，这样一般可以节省光标 
移动所需要的时间。此外，ncurses将试图使终端光标不可见。 
int nl() 
int nonl() 
这两个函数控制新行的平移。使用nl()可以打开平移，这样在回车时就会 
平移到新的一行，在输出时就会走行。而nonl()可以把平移关上。 
关上平移之后，ncurses做光标移动操作时速度就会快一些。 
------------------------------ 
输入选项 

int keypad(win,bf) 
bf为TRUE，函数在等待输入时会使能用户终端的键盘上的小键盘。 
ncurses将返回一个键代码，该代码在.h头文件中被定义为KEY_*宏， 
它是针对小键盘上的功能键和方向键的。对于PC键盘来说， 
这一点是非常有帮助的，因为这样用户就可以使能数字键和光标键。 
int meta(win.bf) 
bf为TRUE，从getch()返回的键代码将是完整的8位(最高位将不会被去掉) 
int cbreak() 
int nocbreak() 
int crmode() 
int nocrmode() 
cbreak()和nocbreak()将把终端的CBREAK模式打开或关闭。如果CBREAK打开则程序就可以立刻使用读取的输入信息。如果CBREAK关闭，则输入将被缓存 
起来，直到产生新的一行(注意：crmode()和nocrmode()只是为了提供向上兼容性，不要使用它们) 
int raw() 
int noraw() 
这两个函数将把RAW模式打开或关闭。RAW与CBREAK相同， 
它们的区别在于RAW模式不处理特殊字符。 
int echo() 
int noecho() 
如果把echo()设置为TRUE，则用户所敲的输入将会回送并显示出来， 
而noecho()则对此保持沉默。 
int halfdelay(t) 
此函数与cbreak()相似，但它要延迟t秒钟。 
int nodelay(win,bf) 
终端将被设置为非阻塞模式。如果没有任何输入则getch()将返回ERR， 
否则如果设置为FALSE，则getch()将等待，直到用户按下某个键为止。 
int timeout(t) 
int wtimeout(win,t) 
笔者提倡大家使用这两个函数，而不要使用halfdelay(t)和nodelay(win,bf)getch()的结果取决于t的值。如果t是正数，则读操作将被阻塞t毫秒； 
如果t为零,则不发生任何阻塞;如果t是负数,则程序将阻塞，直到有输入为止 
int notimeout(win,bf) 
如果bf为TRUE，则getch()将使用一个特殊的定时器(一秒钟长)。到时间以后 
再对以Esc等键打头的输入序列进行解释。 
int typeahead(fd) 
如果fd是-1，则不检查超前键击，否则ncurses将使用文件描述符fd来进行这些检查 
int int rflush(win,bf) 
当bf为TRUE时使能该函数。在终端上按下任意中断键(quit、break.)时， 
所有的输出将会刷新到tty驱动程序队列中。 

四级(中级) 
user4 
97 
6339214 
984818 
202193 
qisiwole 
0 
------------------------------ 
终端属性 
int baudrate()此函数返回终端的速度，以bps为单位。 
char erasechar()此函数返回当前删除的字符。 
char killchar()此函数返回当前杀死的字符。 
int has_ic() 
int has_il()如果终端具有插入/删除字符的能力，则has_rc()将返回TRUE如果终端具有插入/删除行的能力，则has_il()将返回TRUE， 
否则这两个函数将返回ERR。(注：尚未实现) 
char *longname()此函数所返回的指针允许用户访问当前终端的描述符。 
chty petermattrs()(注：此函数尚未实现) 
char *termname()这个函数从用户环境中返回TERM的内容。(尚未实现) 
-------------------------------- 
更新终端 
int refresh() 
int wrefresh(win) 
refersh()将把窗口映像拷贝到终端，而wrefresh(win)将把窗口映像 
拷贝到win，并使它看起来象原来的样子。 
int wnoutrefresh(win) 
int doupdate() 
wnoutrefresh(win)将会只拷贝到窗口win，这意味着在终端上将不进行任何输出，但是虚拟屏幕实际上看起来象程序员所希望的那样。doupdate()将输 
出到终端上。程序可以改变许多窗口，对每个窗口都调用一次 
wnoutrefresh(win)，然后再调用一次doupdate()来更新物理屏幕。 
int redrawln(win) 
int wredrawln(win,bline,blines) 
如果在往屏幕上输出新内容时需要清除一些行或者整个屏幕，可以使用这两 
个函数。(可能这些行已经被破坏了或者由于其他的原因。) 
int touchwin(win) 
int touchline(win,start,count) 
int wtouchln(win,y,n,changed) 
int untouchwln(win) 
这些函数通知ncurses整个win窗口已经被改动过了，或者从start直到 
start+count的这些行已经被改动过了。例如，如果用户有一些重叠的窗口 
(正如在example.c中一样)，对某个窗口的改动不会影响其他窗口的映像。 
wtouchln(.) 
将按掀从y开始的n行。如果change的值是TRUE，则这些行被按掀过了， 
否则就还未被按掀过(改变或未改变)。 
untouchwin(win)将把窗口win标记为自上次调用refresh()以来还未被按掀。 
int ls_linetouched(win,line) 
int ls_wint ouched(win) 
通过使用这两个函数，用户可以检查自从上次调用refresh()以来， 
第line行或者窗口win是否已被按掀过。 
------------------------------- 
视频属性与颜色 
ncurses定义了八种颜色，在带有彩色支持的终端上用户可以使用这些颜色。 
首先，调用start_color()初始化颜色数据结构， 
然后使用has_colors()检查终端权能。 
start_color()将初始化COLORS和COLOR_PAIR。 
前者是终端所支持的最多的颜色数目，而后者是用户可以定义的色彩对的最大数目。 
两个属性可以使用OR操作组合起来。“COLORPAIRS_1COLORS_1” 

int color_content(color,t,g,b) 

此函数获取color的颜色成份r,g和b。 
首先，函数CheckColor调用start_color()初始化颜色，如果当前终端有彩色的话， 
则函数has_colors()将返回TRUE。我们检查了这一点以后，调用init_pair(.) 
把前景色和背景色组合起来，再调用wattrset(.)为特定的窗口设置这些颜色对。 
此外，如果我们使用的是黑白终端，还可以单独使用wattrset(.)来设置属性。 
如果要在xterm中获取颜色，我认为最佳方法是使用ansi_xterm， 
以及来自MidnightCommander的terminfo项目。用户可以获取ansi_xterm 
和MidnightCommander的源代码(mc_x.x.tar.gz)，然后编译ansi_xterm， 
并对mc_x.x.tar.gz文档中的xterm.ti和vt100.ti使用tic命令。 
执行ansi_xterm，把它试验出来 

--------------------------------- 
光标和屏幕坐标 

int move(y,x) 
int wmove(win,y,x) 
move()将移动光标，而wmove(win)则从窗口win中移动光标。对 
输入/输出函数来说，还定义了其他的一些宏，在调用特定函数之前， 
这些宏可以移动光标。 

int curs_set(bf) 这个函数将把光标置为可见或者不可见，如果终端有这个功能 
void getyx(win,y,x) getyx(.)将返回当前光标位置。(注意：这是一个宏) 
void getparyx(win,y,x) 
如果win是个子窗口，getparyx(.)将把该窗口对应父窗口的坐标存储在y和x则y和x都将为－1。(注：此函数尚未实现) 

void getbegyx(win,y,x) 
void getmaxyx(win,y,x) 
int getmaxx(win) 
int getmaxy(win) 
这些函数把窗口win的开始坐标和大小坐标存放在y和x中。 
int getsyx(int y,int x) 
int setsyx(int y,int x) 
getsyx(.)把虚拟屏幕光标存放在y和x中，而setsyx(.)则设置这个坐标。 
如果y和x是－1, 用户调用getsyx(.)将会设置leaveok。 

------------------------------ 
滚动 

int tscrollok(win,bf) 
当光标在屏幕的右下角并且输入了一个字符(或者新的一行)时，如bf为TRUE则窗口win中的文本将上滚一行。如果bf为FALSE，则鼠标留在原来的位置上 
当滚动特征打开时，使用下面的函数可以滚动窗口中的内容。 
(注意：当用户在窗口的最 
后一行输入一个新行时，也应该发生相应的滚动操作， 
所以在使用scrollok(.)时要十分小心， 
否则可能会得到出乎意料的结果。) 
int scrol(win) 
此函数将使窗口向上滚动一行(数据结构中的行也向上滚动)。 
int acrl(n) 
int wscrl(win,n) 
这两个函数将使屏幕或者窗口win向上向下滚动，滚动方向取决于整数n的值 
如果n是正数，则窗口向上滚动n行，否则如果n是负数，则窗口向下滚动n行 
int setscrreg(t,b) 
int wsetscrreg(win,t,b) 
这两个函数设置一个软滚动区。 
------------------------------------- 
小键盘 

WINDOW *newpad(nlines,ncols) 
WINDOW *subpad(orlg,nlines,ncols,begy,begx) 
int prefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol) 
int pnoutrefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol) 
int pechochar (pad,ch) 
---------------------------- 
软标签 

int slk_init(int fmt) 
int slk_set(int labnum,char *label,int fmt) 
int slk_refresh() 
int slk_noutrefresh() 
char *slk_label(int labnum) 
int slk_clear() 
int slk_restore() 
int slk_touch() 
int slk_attron(chty peattr) 
int slk_attrset(chty peattr) 
int slk_attroff(chty peattr) 
这些函数是与attron(attr)、attrset(attr)和attroft(attr)相对应的， 
但它们尚未实现。 
------------------------------ 
int beep() 
int flash() 
char *unctrl(chty pec) 
char *keyname(int c) 
int filter() 
(注：以上函数尚未实现。) 
void use_env(bf) 
int putwin(WINDOW *win,FILE*filep) 
(注：以上函数尚未实现。) 
WINDOW *getwin(FILE*filep) 
(注：以上函数尚未实现。) 
int delay_output(int ms) 
int flushinp() 
-------------------------------------- 
低级访问 

int def_prog_mode() 
int def_shell)_mode() 
int reset_prog_mode() 
int reset_shell_mode() 
int resetty() 
int savetty() 
int ripoffline(int line,int (*init)(WINDOW *,int )) 
int napms(int ms) 
--------------------------------- 
屏幕转储 

int scr_dump(char *filename) 
(注：此函数尚未实现。) 
int scr_restore(char *filename) 
(注：此函数尚未实现。) 
int scr_init(char *filename) 
(注：此函数尚未实现。) 
int scr_set(char *filename) 
(注：此函数尚未实现。) 
----------------------------------- 
Termcap模拟 

int tgetent(char *bp,char *name) 
int tgetflag(char fd[2]) 
int tgetnum(char fd[2]) 
char *tgetstr(char fd[2],char **area) 
char tgoto(char *cap,int col,int row) 
int tputs(char *str,int offset,int (*putc)()) 
------------------------------------------- 
调试函数 

void _init(trace() 
void _trace(char *,...) 
char *_traceattr(mode) 
void traceon() 
void traceoff() 

2002-08-31 09:29:00 

四级(中级) 
user4 
97 
6339224 
984818 
202193 
qisiwole 
0 
------------------------------------------- 

8.21.1布尔型权能 
变量权能名称初始化描述 
auto_left_marginbwbwcub1从最后一列换行到第0列 
auto_right_marginamam终端的边界自动对齐 
back_color_erasebceut屏幕以背景色清除 
can_changeccccc终端可以重新定义现有的颜色 
ceol_standout_glitch xhpxs标准输出不会被覆盖所清除(hp) 
col_addr_glitchxhpaYA对hpa/mhpa大写字符而言只作正向移动 
cpi_changes_rescpixYF改变字符间距将会影响解析度 
cr_cancels_micro_mode crxm YB使用cr关闭宏模式 
eat_newline_glitchxenlxn在80列之后将忽略新行(Concept) 
erase_overtrikeeoeo可以用空格来删除叠印 
generic_typegngn通用行类型(如dialup,switch) 
hard_copyhchc硬拷贝终端 
hard_cursorchtsHC光标很难看到 
has_meta_keykmkm有一个元键(shift，设置奇偶校验位) 
has_print _wheeldaisyYC打印机需要操作员来改变字符集 
has_status_linehshs有一个额外的“状态行” 
hue_lightness_saturation hlshl终端只使用HLS颜色表示法(Tektronix) 
insert_null_glitchinin插入模式，能识别空行 
lpi_changes_reslpixYG改变行距将影响解析度 
memory_abovedada显示可以保留在屏幕上方 
memory_belowdbdb显示可以保留在屏幕下方 
move_insert_modemirmi在插入模式下可以安全地移动 
move_standout_mode msgrms在标准输出模式下可以安全地移动 
needs_xon_xoffnxonnx不能填充，需要nxon/xoff 
no_esc_ctl_cxsbxbBeehive信号(F1=Escape,F2=CtrlC) 
non_rev_rmcupnrrmcNRsmcup不能反转rmcup 
no_pad_char npcNP填充字符不存在 
non_dest_scroll_region ndscr ND滚动区不可摧毁 
over_strikeosos终端可以叠印 
prtr_silentmc5i5i打印机不向屏幕回送 
row_addr_glitchxvpaYDvhp/mvpa大写字母只能作正向移动 
semi_auto_right_margin samYE打印在最后一列将导致cr 
status_line_esc_okeslokes在状态行上可以使用Esc键 
dest_tabs_magic_smso xtxt制表符不可用(Teleray1061) 
tilde_glitchhzhzHazel_tine；不能打印’s 
transparent_underline ulul下划线字符叠印 
xon_coffxonxo终端使用xon/xoff握手机制 
8.21.2数值型权能 
变量权能名称初始值描述 
bit_image_entwining bitwinYo在SYSV中未作描写 
buffer_capacitybufszYa在打印前缓存的字节的数目 
columnscolsco在一行中列的数目 
dot_vert_spacingspinvYb在水平方向上点与点的距离，以每英寸多少点为单位 
dot_horz_spacingspinhYc在垂直方向上针之间的距离，以每英寸多少针为单位 
init_tabsitit每#个空格算一个制表符的位置 
label_heightlhlh每个标签多少行 
label_widthlwlw每个标签多少列 
lineslinesli屏幕或页面上行的数目 
lines_of_memorylmlm如果>;lines则表示内存中的行数，0意味着可变 
magic_cookie_glitch xmcsgsmso或rmso所剩下的空白字符的数目 
max_colorscolorsCo在屏幕上颜色的最大数目 
max_micro_address maddrYd在micro_._address中的最大值 
max_micro_jumpmjumpYe在parm_._micro中的最大值 
max_pairspairspa在屏幕上颜色对的最大数目 
micro_col_sizemcsYf在宏模式中字符间距的大小 
micro_line_sizemlsYg在宏模式中行距的大小 
no_color_videoncvNC不能使用彩色的视频属性 
number_of_pinsnpinsYh在打印头中针的数目 
num_labelsnlabNl屏幕上标签的数量 
output_res_char orcYi水平解析度，以每行单元数为单位 
output_res_lineorlYj垂直解析度，以每行单元数为单位 
output_res_horz_inch orhiYk水平解度，以每英寸单元数为单位 
output_res_vert_inch orviYl垂直解析度，以每英寸单元数为单位 
padding_baud_rate pbpb在需要cr/nl填充时最低的波特率 
virtual_terminalvtvt虚拟终端号(Unix系统) 
width_status_linewslws状态行的第n列 

(下面的数值型权能是在SYSVterm结构中定义的，但在man帮助中还没有提供对它们的 
描述。我们的解释来自term结构的头文件。) 
变量权能名称初始值描述 

bit_image_typebitypeYp位映像设备的类型 
buttonsbtnsBT鼠标按键的数目 
max_attributesmama终端能够处理的最多的组合属性 
maximum_windows wnumMW可定义窗口的最大数目 
print _ratecpsYm打印速率，以每秒字符数为单位 
wide_char _sizewidcsYn在双宽度模式中字符间距的大小 

8.21.3字符串型权能 
变量权能名称初始值描述 

acs_char sacscac图形字符集对一def=vt100 
alt_scancode_escscesaS8扫描码模拟的另一种换码(默认值是VT100) 
back_tab cbtbt向后tab(p) 
bellbelbl声音信号(响铃)(p) 
bit_image_repeatbirepXy把位映像单元重复#1#2次(使用tparm) 
bit_image_newlinebinelZz移动到位映像的下一行(使用tparm) 
bit_image_carriage_return bicr Yv移动到同一行的开头(使用tparm) 
carriage_returncrcr回车(p*) 
change_char _pitch cpiZA改变为每英寸#个字符 
change_line_pitchlpiZB改变为每英寸#行 
change_res_horzchrZC改变水平解度 
change_res_vertcvrZD改变垂直解析度 
change_scroll_region csrcs把滚动区改变为从#1行到#2行(VT100)(PG) 
char _paddingrmprP与ip相似，但它用在插入模式中 
char _set_namescsnmZy字符集名称的列表 
clear_all_tabstbcct清除所有的制表符停止(p) 
clear_marginsmgcMC清除所有的页边 
clear_screenclearcl清除屏幕和home光标(p*) 
clr_bolel1cb清除到行首 
clr_eolelce清除到行尾(P) 
clr_eosedcd清除到显示的末尾(p*) 
code_set_initcsinci多个代码集合的初始化序列 
color_namescolornmYw#1号颜色的名称 
column_addresshpach设置光标列(PG) 
command_char acter cmdchCC在原型中终端可以设置的cmd字符 
cursor_addresscupcm屏幕光标移动到#1行#2列(PG) 
cursor_downcud1do下移一行 
cursor_homehomehoHome光标(如果没有环的话) 
cursor_invisiblecivisvi使光标不可见 
cursor_leftcub1le把光标向左移一个空格 
cursor_mem_address mrcupCM内存相对的光标寻址 
cursor_normalcnormve使光标以最普通的外形显示(undovs/vi) 
cursor_rightcuf1nd不具有破坏性的空白(光标向右移) 
cursor_to_llllll最后一行，第一列(如果没有环的话) 
cursor_upcuu1upUpline(光标向上移) 

cursor_visiblecvvisvs使光标可见 
define_bit_image_region defbiYx定义方形的位映像区(使用tparm) 
define_char defcZE定义字符集中的某个字符 
delete_char acterdch1dc删除字符(p*) 
delete_linedl1dl删除行(p*) 
device_typedevtdv显示语言/代码集支持 
dis_status_linedslds关闭状态行 
display_pc_char dispcS1显示PC字符 
down_half_linehdhd向下移动半行(向前换1/2行) 
ena_acsenacseA使能另一个字符集合 
end_bit_image_region endbi Yy结束位映像区(使用tparm) 
enter_alt_char set_mode smacs as开始另一个字符集(p) 
enter_am_modesmamSA打开自动对齐特征 
enter_blink_modeblink mb打开字符闪烁效果 
enter_bold_modeboldmd打开粗体(特别亮)模式 
enter_ca_modesmcupti启动使用环的程序的字符串 
enter_delete_modesmdcdm删除模式(输入) 
enter_dim_modedimmh打开半亮模式 
enter_doublewide_mode swidm ZF使能双倍宽度模式 
enter_draft_qualitysdrfqZG设置草图效果的打印方式 
enter_insert_modesmirim插入模式(输入) 
enter_italics_modesitmZH使能斜体字模式 
enter_leftward_mode slmZI使能向左回车移动 
enter_micro_modesmicmZJ使能宏移动功能 
enter_near_letter_quality snlqZK设置NLQ打印 
enter_normal_quality snrmqZL设置一般质量的打印方式 
enter_pc_char set_mode smpch S2输入PC字符显示模式 
enter_protected_mode protmp打开保护模式 
enter_reverse_mode revmr打开反转视频模式 
enter_scancode_mode smscS4输入PC扫描码 
enter_secure_mode invismk打开空白模式(字符不可见) 

2002-08-31 09:29:00 

四级(中级) 
user4 
97 
6339230 
984818 
202193 
qisiwole 
0 
enter_shadow_mode sshmZM使能阴影打印模式 
enter_standout_mode smso so开始标准输出模式 
enter_subscript_mode ssubm ZN使能下标打印 
enter_superscript_mode ssupm ZO使能上标打印 
enter_underline_mode smul us开始下划线模式 
enter_upward_mode sum ZP使能向上回车移动 
enter_xon_modesmxonSX打开xon/xoff握手机制 
erase_char sechec删除#1个字符(PG) 
exit_alt_char set_mode rmacs ae终止可选的字符集(P) 
exit_am_modermamRA关闭自动对齐方式 
exit_attribute_mode sgr0me关闭所有属性 
exit_ca_modermcupte终止使用环的程序的字符串 
exit_delete_modermdced终止删除模式 
exit_doublewide_mode rwidm ZQ关闭双倍宽度打印方式 
exit_insert_modermirei结束插入模式 
exit_italics_moderitmZR关闭斜体打印模式 

exit_leftward_moderlmZS使能右向(普通的)回车移动 
exit_micro_modermicmZT关闭宏移动能力 
exit_pc_char set_mode rmpch S3关闭PC字符显示 
exit_scancode_mode rmsc S5关闭PC扫描码模式 
exit_shadow_mode rshmZU关闭阴影打印模式 
exit_standout_mode rmsose结束标准输出模式 
exit_subscript_mode rsubmZV关掉下标打印方式 
exit_superscript_mode rsupm ZW关掉上标打印方式 
exit_underline_mode rmulue结束下划线模式 
exit_upward_moderumZX打开向下(普通的)回车移动 
exit_xon_modermxonRX关掉xon/xoff握手机制 
flash_screenflashvb可视响铃(不能移动光标) 
form_feedffff硬拷贝终端页面的换页(p*) 
from_status_linefslfs从状态行返回 
init_1stringis1i1终端初始化字符串 
init_2stringis2i2终端初始化字符串 
init_3stringis3i3终端初始化字符串 
init_fileifif所包含的文件名称 
init_progiprogiP初始化程序的路径名 
initialize_colorinitcIc初始化颜色的定义 
initialize_pairinitpIp初始化颜色对 
insert_char acterich1ic插入字符(P) 
insert_lineil1al加入一个新的空白行(p*) 
insert_paddingipip在插入的字符之后再插入填充字符(p*) 
key_a1ka1K1小键盘左上方的键 
key_a3ka3K3小键盘右上方的键 
key_b2kb2K2小键盘中央的键 
key_backspacekbskb由回退键所发送 
key_begkbeg1开始键 
key_btabkcbtkB向右一tab键 
key_c1kc1K4小键盘左下角的键 
key_c3kc3K5小键盘右下角的键 
key_cancelkcan2取消键 
key_catabktbcka由clear_all_tabs键发送 
key_clearkclrkC由清除屏幕或者删除键发送 
key_closekclo3关闭键 
key_commandkcmd4命令键 
key_copykcpy5拷贝键 
key_createkcrt6创建键 
key_ctabkctabkt由clear_tab键发送 
key_dckdch1kD由删除字符键发送 
key_dlkdl1kL由删除行键发送 
key_downkcud1kd由终端向下光标键发送 
key_eickrmirkM在插入模式中由rmir或smir发送 
key_endkend7结束键 
key_enterkent8输入/发送键 
key_eolkelkE由clear_to_end_of_line键发送 

key_eoskedkS由clear_to_end_of_screen键发送 
key_exitkext9退出键 
key_findkfnd0查找键 
key_helpkhlp%1帮助键 
key_homekhomekh由home键发送 
key_ickich1kl由inschar /enterinsmode键发送 
key_ilkil1kA由插入行发送 
key_leftkcub1kl由终端向左键发送 
key_llkllkH由home_down键发送 
key_markkmrk%2标记键 
key_messagekmsg%3消息键 
key_movekmov%4移动键 
key_nextknxt%5下一个键 
key_npageknpkN由下页键发送 
key_openkopn%6打开键 
key_optionskopt%7选项键 
key_ppagekppkP由前页键发送 
key_previouskprv%8前一键 
key_print kprt%9打印键 
key_redokrdo%0redo键 
key_refrencekref&1引用键 
key_refreshkrfr&2刷新键 
key_replacekrpl&3替换键 
key_restartkrst&4重启键 
key_resumekres&5恢复键 
key_rightkcuf1kr由终端向右键发送 
key_saveksav&6保存键 
key_sbegkBEG&9按下开始键的同时按下shift键 
key_scancelkCAN&0按下取消键的同时按下shift键 
key_scommandkCMD*1按下命令键的同时按下shift键 
key_scopykCPY*2按下拷贝键的同时按下shift键 
key_screatekCRT*3按下创建键的同时按下shift键 
key_sdckDC*4按下删除字符键的同时按下shift键 
key_sdlkDL*5按下删除行键的同时按下shift键 
key_selectkslt*6选择键 
key_sendkEND*7按下结束键的同时按下shift键 
key_seolkEOL*8按下行尾键的同时按下shift键 
key_sexitkEXT*9按下退出键的同时按下shift键 
key_sfkindkF由前滚/下滚键发送 
key_sfindkFND*0按下查找键的同时按下shift键 
key_shelpkHLP#1按下帮助键的同时按下shift键 
key_shomekHOM#2按下Home键的同时按下shift键 
key_sickIC#3按下插入字符键的同时按下shift键 
key_sleftkLFT#4按下向左键的同时按下shift键 
key_smessagekMSG%a按下消息键的同时按下shift键 
key_smovekMOV%b按下移动键的同时按下shift键 
key_snextkNXT%c按下向后键的同时按下shift键 

key_soptionskOPT%d按下选项键的同时按下shift键 
key_spreviouskPRV%e按下向前键的同时按下shift键 
key_sprint kPRT%f按下打印键的同时按下shift键 
key_srkrikR由后滚/下滚键发送 
key_sredokRDO%g按下redo键的同时按下shift键 
key_sreplacekRPL%h按下替换键的同时按下shift键 
key_srightkRIT%l按下向右键的同时按下shift键 
key_srsumekRES%j按下恢复键的同时按下shift键 
key_ssavekSAV!1按下保存键的同时按下shift键 
key_ssuspendkSPD!2按下中断键的同时按下shift键 
key_sundokUND!3按下取消键的同时按下shift键 
key_stabkhtskT由set_tab键发送 
key_suspendkspd&7中断键 
key_undokund&8取消键 
key_upkcuulku由终端的向上键发送 
keypad_localrmkxke不处于“小键盘发送”方式之中 
keypad_xmitsmkxks把终端置为“小键盘发送”方式 
lab_f0lf0l0如果不是f0的话，则为功能键f0的标签 

2002-08-31 09:30:00 

一级(初级) 
user1 
100 
6339448 
984818 
200455 
linuxman2002 
0 
up 

2002-08-31 09:46:00 

四级(中级) 
user4 
97 
6339495 
984818 
202193 
qisiwole 
0 
lab_f1lf1l1如果不是f1的话，则为功能键f1 

的标签 
lab_f2lf2l2如果不是f2的话，则为功能键f2 

的标签 
lab_f3lf3l3如果不是f3的话，则为功能键f3 

的标签 
lab_f4lf4l4如果不是f4的话，则为功能键f4 

的标签 
lab_f5lf5l5如果不是f5的话，则为功能键f5 

的标签 
lab_f6lf6l6如果不是f5的话，则为功能键f6 

的标签 
lab_f7lf7l7如果不是f7的话，则为功能键f7 

的标签 
lab_f8lf8l8如果不是f8的话，则为功能键f8 

的标签 
lab_f9lf9l9如果不是f9的话，则为功能键f9 

的标签 
lab_f10lf10la如果不是f10的话，则为功能键 

f10的标签 
label_onsmlnLO打开软标签 
label_offrmlnLF关闭软标签 
meta_offrmmmo关闭“元模式” 
meta_onsmmmm打开“元模式”(8位) 
micro_column_address mhpa ZY近似宏调整的列—地址， 
micro_downmcud1ZZ近似宏调整的光标—向下 
micro_leftmcutb1Za近似宏调整的光标—向左 
micro_rightmcuf1Zb近似宏调整的光标—向右 
micro_row_address mvpaZc近似宏调整的行—地址 
micro_upmcuu1Zd近似宏调整的光标—向上 
newlinenelnw新行(行为近似于cr后跟lf) 
order_of_pinsporderZe匹配软件以及打印头中的针 
orig_colorsococ重置所有的颜色对 
orig_pairopop把默认的颜色对设置为原始的那 

个 
pad_char padpc填充字符(非空) 
parm_dchdchDC删除#1字符(PG*) 
parm_delete_linedlDL删除#1行(PG*) 
parm_down_cursor cudDO把光标向下移#1行(PG*) 
parm_down_micro mcudZf近似宏调用的cub 

parm_ichichIC插入#1个空白符号(PG*) 
parm_indexindnSF向上滚动#1行(PG) 
parm_insert_lineilAL加入#1个新的空白行(PG*) 
parm_left_cursorcubLE把光标向左移#1个空格(PG) 
parm_left_micromcubZg近似宏调整中的cub 
parm_right_cursor cufRl把光标向右移#1个空格(PG*) 
parm_right_micromcufZh近似宏调整中的cuf 
parm_rindexrinSR回滚#1行(PG) 
parm_up_cursorcuuUP把光标上移#1行(PG*) 
parm_up_micromcuuZi近似宏调整中的cuu 
pkey_keypfkeypk把功能键#1定义为字符#2的类型 
pkey_localpflocpl把功能键#1定义为执行字符串#2 
pkey_xmitpfxpx把功能键#1定义为发送字符串#2 
pkey_plabpfxlxl把功能键#1定义为发送#2，并显 

示#3 
plab_normplnpn编程标签#1，以显示字符串#2 
print _screenmc0ps打印屏幕内容 
prtr_nonmc5ppO打开打印机，打印#1个字节 
prtr_offmc4pf关闭打印机 
prtr_onmc5po打开打印机 
repeat_char reprp把字符#1重复#2次(PG*) 
req_for_inputrfiRF输入请求 
reset_1stringrs1r1把终端完全置为sane方式 
reset_2stringrs2r2把终端完全置为sane方式 
reset_3stringrs3r3把终端完全置为sane方式 
reset_filerfrf包含重置字符串的文件名称 
restore_cursorrcrc把光标置为上一个屏幕上的位置 
row_addressvpacv垂直绝对位置(设置行)(PG) 
save_cursorscsc保存光标位置(P) 
scancode_escapescescS7为了扫描码模拟按下Esc键 
scroll_forwardindsf把文本向上滚动(P) 
scroll_reverserisr把文本向下滚动(P) 
select_char _setscsZj选择字符集 
set0_des_seqs0dss0切换到代码集0(EUC集0，ASCII) 
set1_des_seqs1dss1切换到代码集1 
set2_des_seqs2dss2切换到代码集2 
set3_des_seqs3dss3切换到代码集3 
set_a_backgroundsetabAB使用ANSI设置背景颜色 
set_a_foregroundsetafAF使用ANSI设置前景颜色 
set_attributessgrsa定义视频属性(PG9) 
set_backgroundsetbSb设置当前背景颜色 
set_bottom_marginsmgbZk设置当前行的底部边界 
set_bottom_margin_parm smgbp ZI从bottomset_color_band的#1行 

或#2行设置底行 
setcolorYz改变#1号色带颜色 
set_color_pairscpsp设置当前颜色对 
set_foregroundsetfSf设置当前前景色 
set_left_marginsmglML设置当前行的左边界 
set_left_margin_parm smglpZm在#1行(#2行)设置左(右)边界 

set_lr_marginsmglrML设置左右边界 
set_page_lengthslinesYZ把页的长度设置为#1行(使用 

tparm) 
set_right_marginsmgrMR把右边界设置为当前列 
set_right_margin_parm smgrp Zn把右边界设置为#1列 
set_tabhtsst在当前列的所有行设置制表符 
set_tb_marginsmgtbMT设置上下边界 
set_top_marginsmgtZo把上边界设置为当前行 
set_top_margin_parm smgtp Zp把上边界设置为#1行 
set_WINDOW windwi当前窗口是从#1行到#2行，从#3 

列到#4列 
start_bit_imagesbimZq开始打印位映像图形 
start_char _set_def scsdZr开始定义字符集 
stop_bit_imagerbimZs结束打印位映像图形 
stop_char _set_def rcsdZt结束定义字符集 
subscript_char acters subcs Zu下标字符的列表 
superscript_char acters supcs Zv 上标字符的列表 
tabhtta跳转到下面8个空格硬件的制表 

符位置 
these_cause_crdocrZw这些字符导致CR 
to_status_linetslts跳到状态行，第1列 
underline_char ucuc给某字符划下划线，并移过它 
up_half_linehuhu上移半行(反转1/2行) 
xoff_char actercoffcXFXON字符 
xon_char acterxoncXNXOFF字符 

(下面的字符串权能是在SYSVr终端结构中定义的，但在man帮助信息中还 

未作描述，对 
它们的解释是从终端结构头文件中得到的。) 

label_formatflnLf?? 
set_clocksclkSC设置时钟 
display_clockdclkDK显示时钟 
remove_clockrmclkRC删除时钟 
create_WINDOW cwinCW把窗口#1定义为从#2行,#3列到#4 

行，#5列 
goto_WINDOW wingoWG跳到窗口#1 
hanguphupHU挂起电话 
dial_phonedialDI拨电话号码#1 
quick_dialqdialQD拨电话号码#1，但不做进度检查 
tonetoneTO选择接触声调拨叫 
pulsepulsePU选择脉冲拨叫 
flash_hookhookfh闪光切换分支 
fixed_pausepausePA暂停2～3秒 
wait_tonewaitWA等待拨叫声音 
user0u0u0用户字符串#0 
user1u1u1用户字符串#1 
user2u2u2用户字符串#2 
user3u3u3用户字符串#3 
user4u4u4用户字符串#4 
user5u5u5用户字符串#5 
user6u6u6用户字符串#6 
user7u7u7用户字符串#7 
user8u8u8用户字符串#8 
user9u9u9用户字符串#9 
get_mousegetmGmsurses 应获得按钮事件 
key_mousekmousKm?? 
mouse_infominfoMi鼠标状态信息 
pc_term_optionspctrmS6PC终端选项 
req_mouse_posreqmpRQ请求鼠标位置报告 
zero_motionzeromZx后继字符没有移动

参考函数文件:
http://dl2.csdn.net/down4/20070911/11132900783.h

