# 《Linux程序设计》——使用curses函数库管理基于文本的屏幕 - 一世豁然的专栏 - CSDN博客





2014年09月24日 16:34:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1576








一、curses函数库介绍


 1、用curses函数库进行编译

  1）、curses是一个函数库，所以在使用时必须在程序中包含对应的头文件、函数声明和宏定义。但对是使用curses函数库的程序进行编译时，必须在程序中包含头文件


             curses.h，并在编译命名中使用-lcurses选项来链接cueses函数库。


   2）、可以使用命令**ls  -l  /usr/include/*curses.h**来查看自己curses的头文件。


   3）、可用命令**ls  -l  /usr/lib/lib*curses***来检查库文件。


   4）、如果发现头文件ncurses.h和curses.h都只是链接文件，而且系统中存在一个ncurses库文件则可以使用**gcc  program.c  -o  program  -lcurses**来编译命令。

   5）、如果curses配置并未使用ncurses库函数，则必须在程序中明确包含ncurses.h而不是curses.h来强制使用ncurses函数库，同时需要执行如下的编译命令：**gcc  -I/usr**

**             /include/ncurses  program.c  -o  program  -lncurses**。




 2、curses术语和概念

  1）、curses例程工作在屏幕、窗口、子窗口之上。屏幕就是你正在写的设备（设备通常是终端屏幕，也可能是xterm屏幕）。屏幕占据了设备上全部的可用显示面积。当然，如

            果设备是X视窗的一个终端窗口，则屏幕就是该终端窗口内所有可用的字符位置。

  2）、curses函数库用两个数据结构stdscr和curses来映射终端屏幕。

  3）、curses程序中输出字符的过程：

         I、使用curses函数刷新逻辑屏幕。

        II、要求curses用refresh函数来刷新物理屏幕。

  4）、curses函数使用的坐标都是y值（行号）在前、x值（列号）在后。每个位置不仅包含该屏幕位置处的字符u，还包含它的属性。







二、屏幕

 1、所有的curses程序必须以initscr函数开始，以endwin函数结束。

** #include<curses.h>**

**     WINDOW   *initscr(void); **

**     int  endwin(void);**

     I、initscr函数在一个程序中只能调用一次。如果成功，将返回一个指向stdscr结构的指针，如果失败，就输出一条诊断错误信息并使程序退出。   

    II、endwin函数在成功时返回ok，失败时返回ERR。可以先调用函数退出curses，然后通过调用clearok（stdscr，1）和refresh函数继续curses操作。




 2、输出到屏幕

**     #include<curses.h>**

**     int  addch(const  chtype  char_to_add);**

**     int  addchstr(chtype  *const  string_to_add);**

**     int  printw(char  *format,. . .);**

**     int  refresh(void);**

**     int  box(WINDOW  *win_ptr,  chatype  vertical_char,  chatype  horizontal_char);**

**     int  insch(chtype  char_to_insert);**

**     int  insertln(void);  **

**     int  delch(void); **

**     int  deleteln(void); **

**     int  beep(void);**

**     int  flash(void); **

     I、curses有自己的字符类型chtype，他可能比标准的char类型包含更多的二进制位。在ncurses的标准Linux版本中，chatype实际是unsigned  long类型的一个类型定义。

     II、add系列函数在光标的当前位置添加指定的字符或字符串。

     III、printw函数采用与printf函数相同的方法对字符串进行格式化，然后将其添加到光标的当前位置。

     IV、refresh函数的作用是刷新屋里屏幕，成功时返回OK，发生错误时返回ERR。

     V、box函数用来围绕一个窗口绘制方框。

     VI、insch函数插入一个字符，将已有字符向右移，但此操作对行尾的影响并未定义，具体取决于你所使用的终端。

     VII、insertln函数的作用是插入一个空白行，将现有行依次向下移一行。

     VIII、两个delete函数的作用上述两个insert函数正好相反。

     IX、beep函数可以让程序发出声音。

     X、flash函数的作用是使屏幕闪烁，但如果无法产生闪烁效果，它将会尝试在终端上发出声音。




 3、从屏幕读取

**    #include<curses.h>**

**    chtype  inch(void);**

**    int  instr(char  *string);**

**    int  innstr(char  *string,  int  number_of_characters);**

    I、以上函数可以从屏幕上读取字符。

    II、inch函数返回光标当前位置的字符以其属性字符信息。注意inch函数返回的并不是一个字符，而是一个chtype类型的变量。

    III、instr和innstr函数则将返回内容写到字符数组中。




 4、清除屏幕

** #include<curses.h>**

**     int  erase(void);**

**     int  clear(void);**

**     int  clrtobot(void);**

**     int  clrtoeol(void);**

     I、erase函数在每个屏幕位置写上空白字符。

     II、clear函数的功能类似erase函数，它也是用于清屏，但其还通过在内部调用一个底层函数clearok来强制重现屏幕原文，clearok函数会强制执行清屏操作，并在下一次调

           用refresh函数时重现原文。

     III、clrtobot函数清楚当前光标位置直到屏幕结尾的所有内容。

     IV、clrtoeol函数清楚当前光标位置直到光标所处行行尾的所有内容。




 5、移动光标

**  #include<curses.h>**

**     int  move(int  new_y,  int  new_x);**

**     int  leaveok(WINDOW  *window_ptr,  bool  leave_flag);**

     I、move函数用来将逻辑光标的位置移到指定的地点。

     II、leaveok函数设置了一个标识，该标志用于控制在屏幕刷新后curses将物理光标放置的位置。




 6、字符属性

** #include<curses.h>**

**    int  attron(chtype  attribute);**

**    int  attroff(chtype  attribute);**

**    int  attrset(chtype  attribute);**

**    int  standout(void);**

**    int  standend(void);**

    I、attrset函数设置curses属性。

    II、attron和attroff函数在不影响其他属性的前提下启用或关闭指定的属性。

    III、standout和standend函数提供了一种更加通用的强调或“突出”模式。在大多数终端上，它通常被映射为反白显示。

    IV、属性：A-NORMAL（标准的显示模式），A_BLINK（闪烁），A_BOLD（加粗），A_DIM（半透明），A_REVERSE（反显），A_STANDOUT（高

            亮），A_UNDERLINE（下划线）。







三、键盘

 1、键盘模式

** #include<curses.h>**

**     int  echo(void);**

**     int  noecho(void);**

**     int  cbreak(void);**

**     int  nobreak(void);**

**     int  raw(void);**

**     int  noraw(void);**

     I、两个echo函数用于开启或关闭输入字符的回显功能

     II、可以通过调用cbreak函数来启动或关闭cbreak模式。在这种情况下，字符一经输入就立即传送给程序，而不像cooked模式那样首先缓存字符，直到用户按下回车键后才

           将用户输入的字符传递给程序。

     III、raw函数调用的作用是关闭特殊字符的处理，所有执行该函数调用后，再想通过输入特殊字符序列来产生信号或进行流程就不可能了。noraw函数调用同时恢复cooked模 


            式和特殊字符处理功能。




 2、键盘输入

**  #include<curses.h>**

**    int  getch(void);**

**    int  getstr(char  *string);**

**    int  getnstr(char  *string, int  number_of_characters);**

**    int  scanw(char  *format, . . .);**







四、窗口

 1、WINDOW结构

  1）、窗口的创建和销毁

** #include<curses.h>**

**         WINDOW  *newwin(int  num_of_lines,  int  num_of_cols,  int  start_y,  int  start_x);**

**         int  delwin(WINDOW  *window_to_delet**e);

        I、newwin函数的作用是创建一个新窗口，该窗口从屏幕位置（start_y,  start_x）开始，行数和列数分别由参数num_of_lines,和int  num_of_cols指定。它返回一个指向新

             窗口的指针，如果创建失败则返回null。

        II、delwin函数的作用是删除一个先前通过newwin函数创建的窗口。




 2、通用函数

**#include<curses.h>**

**     int  addch(const  chtype  char);**

**     int  waddch(WINDOW  *window_pointer,  const  chtype  char);**

**     int  mvaddch(int  y, int  x,const  chtype  char);**

**     int  mvwaddch(WINDOW  *window_pointer, int  y, int  x, const  chtype  char);**

**     int  printw(char  *format, . . .);**

**     int  wprintw(WINDOW  *window_pointer,  char  *format, . . .);**

**     int  mvprintw(int  x, int  y, cha  *format,. . .);**

**     int  mvwprintw(WINDOW  *window_pointer, int  y,int  x,char  *format);**

     I、如果给函数增加了w，则必须在该函数的参数列表最前面增加一个WINDOW指针参数。

     II、如果给函数增加mv，则必须该函数的参数列表最前面增加两个参数，分别是纵坐标y和横坐标x。

     III、如果给函数增加mvw，则必须该函数的参数列表最前面增加三个参数，分别是WINDOW指针、纵坐标y和横坐标x。




 3、移动和创建窗口

**#include<curses.h>**

**    int  mvwin(WINDOW  *window_to_move,  int  new_y, int  new_x);**

**    int  wrefresh(WINDOW  *window_ptr);**

**    int  wclear(WINDOW  *window_ptr);**

**    int  werase(WINDOW  *window_ptr);**

**    int  touchwin(WINDOW  *window_ptr);**

**    int  scrollok(WINDOW  *window_ptr, bool  scroll_flag);**

**    int  scroll(WINDOW  *window_ptr);**

    I、mvwin函数的作用是在屏幕上移动一个窗口。

    II、touchwin函数非常特殊，它的作用是通知curses函数库及其指针参数指向的窗口内容已发生改变。

    III、两个scroll函数控制窗口的卷屏。如果传递给scrollok函数的是布尔值true则允许窗口卷屏。




 4、优化屏幕刷新

**#include<curses.h>**

**    int  wnoutrefresh(WINOW  *window_ptr);**

**    int  doupdate(void);**

    I、wnoutrefresh函数用于决定把那些字符发送到屏幕上，但它并不真正的发送这些字符，真正将更新发送到终端的工作由doupdate函数来完成。




 5、子窗口的创建和删除

** #include<curses.h>**

**    WINDOW  *subwin(WINDOW  *parent,  int  num_of_lines,  int  num_of_cols,  int  start_y,  int  start_x);**

**    int  dewin(WINDOW  *window_to_delete);**

    I、子窗口与新窗口的区别：子窗口没有独立的屏幕字符存储空间，它们与其父窗口（在创建子窗口时指定）共享同一字符存储空间。

    II、子窗口最主要的用途是提供了一种能够简洁额方式来卷动另一窗口的部分内容。







六、keypad模式

 1、curses函数库提供了一个精巧的用于管理功能键的功能。对每个终端来说，他们每个功能键所对应的转义序列都被保存，通常是保存在一个terminfo结构中，而头文件

       curses.h通过一组KEY_为前缀的定义来管理逻辑键。




 2、curses在启动时会关闭转义序列与逻辑键之间的转换功能，该功能需要调用keypad函数来启用。该函数调用成功时，返回OK，否则就返回ERR。

**#include<curses.h>**

**     int  keypad(WINDOW  *window_ptr,  bool  key_on);**

     I、将keypad参数设置为true，然后调用keypad函数来启用keypad模式。在该模式中，curses将接管按键转义序列的处理工作，都键盘操作不仅仅能够返回用户按下的键，  



         还将返回与逻辑按键对应的KEY_定义。

     II、keypad模式的限制

       A、识别escape转义序列的过程是与时间有关。

       B、为了让cursees能够区分“单独按下Escape键”和“一个以Escape字符开头的键盘转义序列”，必须等待一小段时间。

       C、curses不能处理二义性的Escape转义序列。







七、彩色显示

  1、颜色组合：必须同时定义一个字符的前景色和背景色。




  2、在使用curses函数库的颜色功能之前，必须检查当前终端是否支持彩色显示功能，然后对curses的颜色列成进行初始化。

**#include<curses.h>**

**      bool  has_colors(void);**

**      int  start_color(void);**

      I、如果终端支持颜色显示，has_colors函数将返回true，之后，需要调用start_color函数，如果该函数成功初始化了颜色的显示功能，将返回ok。

      II、一旦以上函数被成功调用，COLOR_PAIRS将被设置为终端所能支持的颜色组合数目最大值，一般为64。变量COLORS定义可用颜色数目的最大值，一般只有8种。





 3、再把颜色作为属性使用之前，必须首先调用init_pair函数对准备使用的颜色组合进行初始化。对颜色属性的访问是通过COLOR_PAIR函数完成的。


** include<curses.h>**

**    int  init_pair(short  pair_number,   short  foreground,  short  background);**

**    int  COLOR_PAIR(int  pair_number);**

**    int  pair_content(short  pair_number,   short  *foreground,  short  *background);**




 4、重新定义颜色

**    #include<curses.h>**

**    int  init_color(short  color_number,  short  red,  short  green,   shoer  blue);**

    I、这个函数将一个已有的颜色以新的亮度值重新定义，亮度值的范围从0到1000。








八、pad


 1、curses提供了数据结构来解决“控制尺寸大于正常窗口的的逻辑屏幕”这一问题。




  2、pad的创建


     #include<curses.h>

     WINDOW  *newpa(int  number_of_lines,  int  number_of_columns);

     I、此函数的返回值是一个指向WINDOW结构的指针。




 3、pad的刷新

**#include<curses.h>**

**    int  prefresh(WINDOW  *pad_ptr,  int pad_row,  int  pad_column,  int  screen_row_min,  int  screen_col_min,  int  sreen_row_max,  int  screen_col_max);**

    I、此函数的作用是将pad从坐标（pad_row，pad_column）开始的区域写到屏幕上指定的显示区域，该显示区域的范围从坐标（screen_row_min，screen_col_min）到

       （sreen_row_max，screen_col_max）。

    II、curses还提供了pnoutrefresh，它与wnoutrefresh一样，都是为了更有效的更新屏幕。




