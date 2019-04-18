# 【C语言】C语言中格式化字符的具体用法（C语言中%的那些事） - csdn_baotai的博客 - CSDN博客

2018年05月12日 20:39:18[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：1365


## 格式字符格式字符用以指定输出项的数据类型和输出格式。

## 格式字符控制：

> 

说明：的sprintf（格式化后的字符，“％（长度）格式符”，格式化前的字符）;

1，d格式符用来输出十进制数有以下几种用法。：

（1），％d按整数的实际长度输出。

（2），％MD，米为指定的输出字段的宽度。如果数据位数小于米，则左端补以空格，若大于米，则按实际位数输出。（3），％LD，输出长整型数据。例：long a = 135790; 的printf（“％LD”，A）;如果用％d输出就会发生错误，因为整型数据的范围是-32768到32767。对于长型数据应当用％ld的格式输出，对长整型数据也可以指定字段宽度，如：％8LD。

2，O格式符，以八进制数形式输出整数。由于内存单元中的各位的值（0或1）按八进制形式输出，因此输出的数值不带符号，即将符号位也一起作为八进制的一部分输出。

3，X格式符，以十六进制数形式输出整数。同样不会出现负的十六进制数。同样可以用％LX输出长整型数，也可以指定输出字段宽度。

4，U格式符，用来输出无符号型数据，即无符号数，以十进制形式输出。一个有符号整数（int型）也可以用％U格式输出，反之，一个无符号型的数据也可以用％ d格式输出。按相互赋值的规则处理.Unsigned型数据也可以用％×格式输出。

5，C格式符，用来输出一个字符。一个字符型数据也可以用整数形式输出。

6，S格式符，用来输出一个字符串。

（1），％S，的的printf（“％S”，“HELLO”）;

（2），％MS输出字符串占米列，如字符串本身大于米，则突破米的限制，字符串全部输出，如果小于米，则左补空格（3），％ - 。毫秒如果字符串小于米，则在米范围内，字符串向左靠，右补空格。

（4），％m.ns，输出占米列，但只取字符串中左端Ñ个字符。这Ñ个字符输出在米列的右侧，左补空格。

（5），％ - m.ns，其中M，N含义同上，N个字符输出在米范围的左侧，右补空格如果N> M，则自动取值，即保证Ñ个字符正常输出。

7，F格式符，用来输出实数（包括单，双精度），以小数形式输出。

（1），％F，不指定字段宽度，有系统自动指定，使整数部分全部如数输出，并输出6位小数。应当注意，并非全部数字都是有效数字。单精度实数的有效位数一般是7位。

（2），％m.nf指定输出的数据占米列，其中有Ñ位小数。如果数值长度小于米，则左补空格。

（3），％ - m.nf与％m.nf基本相同，只是输出的数值向左端靠，右端补空格。

8，电子格式符，以指数形式输出。

9，克格式符，用来输出实数，它根据数值的大小，自动选择˚F格式或ë格式（选择输出时占用宽度较小的一种），并且不输出无意义的零。

************************************************** *******

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

> 

|**码**|**格式**|
|----|----|
|％C|字符|
|％d|带符号整数|
|％一世|带符号整数|
|％E|科学计数法，使用小写的“e”|
|％E|科学计数法，使用大写“E”|
|％F|浮点数|
|％G|使用％E或％F中较短的一个|
|％G|使用％E或％F中较短的一个|
|Ø％|八进制|
|％S|一串字符|
|％u|无符号整数|
|％X|无符号十六进制数，用小写字母|
|％X|无符号十六进制数，用大写字母|
|％p|一个指针|
|％N|参数应该英文的一个指向一个整数的指针			指向的的英文字符数放置的位置|
|%%|一个'％'符号|

一个位于一个％和格式化命令间的整数担当着一个最小字段宽度说明符，并且加上足够多的空格或0使输出足够长。如果你想填充0，在最小字段宽度说明符前放置0。你可以使用一个精度修饰符，它可以根据使用的格式代码而有不同的含义。
- 用％E，％E和％F，精度修饰符让你指定想要的小数位数。例如，
	

`    ％12.6f`
将会至少显示12位数字，并带有6位小数的浮点数。

- 用％克和％G，精度修饰符决定显示的有效数的位数最大值。
- 用％S，修饰符精度简单的表示一个**最大**的最大长度，以补充句点前的最小字段长度。

所有的printf（）的输出都是右对齐的，除非你在％符号后放置了负号。例如：    ％-12.4f

将会显示12位字符，4位小数位的浮点数并且左对齐。你可以修改带字母**l**和**h**％d，％i，％o，％u和％x等类型说明符指定长型和短型数据类型（例如％hd表示一个短整数）。％e，％f和％g类型说明符，可以在它们前面放置**l**指出跟随的是一个double。％g，％f和％e类型说明符可以置于字符'＃'前保证出现小数点，即使没有小数位。带％×类型说明符的'＃'字符的使用，表示显示十六进制数时应该带加'0x'前缀。带％氧气类型说明符的'＃'字符的使用，表示显示八进制数时应该带一个'0'前缀。

****************************格式化符在sscanf的的函数中的强大用法*********** * *****************

1.常见用法。

　　char buf [512];

　　的sscanf（“123456”，“％s”时，BUF）; //此处BUF是数组名，它的意思是将123456以％S的形式存入BUF中！

　　printf（“％s \ n”，buf）;

　　结果为：123456

　　2.取指定长度的字符串。如在下例中，取最大长度为4字节的字符串。

　　的sscanf（“123456”，“％4S”，BUF）;

　　printf（“％s \ n”，buf）;

　　结果为：1234

　　3.取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。

　　sscanf（“123456 abcdedf”，“％[^]”，buf）;

　　printf（“％s \ n”，buf）;

　　结果为：123456

　　4.仅限包含指定字符集的字符串。如在下例中，仅包含1到9和小写字母的字符串。

　　的sscanf（“123456abcdedfBCDEF”，“％[1-9A-Z]”，BUF）;

　　printf（“％s \ n”，buf）;

　　结果为：123456abcdedf

　　当输入：

　　的sscanf（“123456abcdedfBCDEF”，“％[1-9A-Z]”，BUF）;

　　的printf（“％S \ n”个，BUF）;

　　结果为：123456

　　5.取到指定字符集为止的字符串。如在下例中，取遇大写字母为止的字符串。

　　sscanf（“123456abcdedfBCDEF”，“％[^ AZ]”，buf）;

　　printf（“％s \ n”，buf）;

　　结果为：123456abcdedf

　　6，给定一个字符串iios / 12DDWDFF @ 122，获取/和@之间的字符串，先将“iios /”过滤掉，再将非'@'的一串内容送到buf中

　　sscanf（“iios / 12DDWDFF @ 122”，“％* [^ /] /％[^ @]”，buf）;

　　printf（“％s \ n”，buf）;

　　结果为：12DDWDFF

　　7，给定一个字符串“你好，世界”，仅保留世界（注意：“”之后有一空格，％S遇空格停止，加*则是忽略第一个读到的字符串）。

　　sscanf（“hello，world”，“％* s％s”，buf）;

　　printf（“％s \ n”，buf）;

　　结果为：世界

　　％* S表示第一个匹配到的％S被过滤掉，即你好被过滤了

　　如果没有空格则结果为NULL。

　　sscanf的的功能很类似于正则表达式，但却没有正则表达式强大，所以如果对比较复杂的字符串处理，建议使用正则表达式。

　　// ------------------------------------------------ -------

　　用它来分隔类似这样的字符串2006：03：18：

　　int a，b，c;

　　/ * sscanf（“2006：03：18”，“％d：％d：％d”，a，b，c）; * / / *错误方法，要在变量a，b，c前加上取地址符，由huanmie_09修改* /

　　的sscanf（“2006：03：18”，“％d：％d：％d”，＆一个，及B，C）;

　　以及2006：03：18 - 2006：04：18：

　　char sztime1 [16] =“”，sztime2 [16] =“”;

　　sscanf（“2006：03：18 - 2006：04：18”，“％s - ％s”，sztime1，sztime2）;

　　但是后来，我需要处理2006：03：18-2006：04：18

　　仅仅是取消了' - '两边的空格，却打破了％S对字符串的界定。

　　我需要重新设计一个函数来处理这样的情况？这并不复杂，但是，为了使所有的代码都有统一的风格，我需要改动很多地方，把已有的sscanf的的替换成我自己的分割函数。我以为我肯定需要这样做，并伴随着对的的sscanf的强烈不满而入睡;一觉醒来，发现其实不必。

　　如果读取的字符串，不是以空格来分隔的话，就可以使用％[]。

　　％[]类似于一个正则表达式。[AZ]表示读取AZ的所有字符，[^ AZ]表示读取除AZ以外的所有字符。

　　所以那个问题也就迎刃而解了：

　　sscanf（“2006：03：18 - 2006：04：18”，“％[0-9，：] - ％[0-9，：]”，sztime1，sztime2）;

　　在softmse（杰克）的问题贴中，去掉我（去掉我）et / Expert / topic / 4843 / 4843294.xml？temp = .4321558中，给出了一个很cool的sscanf用例，而后通过学习，发现sscanf的真棒，现做一总结。

搜集一些特殊用法：

％[]的用法：％[]表示要读入一个字符集合，如果[后面第一个字符是“^”，则表示反意思。

                     []内部字符串可以是1或更多字符组成。空字符集（％[]）是违反规定的，可

                     导致不可预知的结果。％[^]也是违反规定的。

％[AZ]读取在AZ之间的字符串，如果不在此之前则停止，如

              char s [] =“你好，我的朋友”; //注意：，逗号在不AZ之间

              sscanf的（S，“％[AZ]”，字符串）; // string = hello

％[^ az]读取不在az之间的字符串，如果碰到az之间的字符则停止，如

              char s [] =“HELLOkitty”; //注意：，逗号在不是AZ之间

              sscanf（s，“％[^ az]”，string）; // string = HELLO

％* [^ =]前面带*号表示不保存变量。跳过符合条件的字符串。

              char s [] =“notepad = 1.0.0.1001”;

       char szfilename [32] =“”;

       int i = sscanf（s，“％* [^ =]”，szfilename）;

// szfilename = NULL，因为没保存

int i = sscanf（s，“％* [^ =] =％s”，szfilename）;

// szfilename = 1.0.0.1001

％40C读取40个字符

％[^ =]读取字符串直到'='，'^'后面可以带更多字符，如：

              char s [] =“notepad = 1.0.0.1001”;

              char szfilename [32] =“”;

             int i = sscanf（s，“％[^ =]”，szfilename）;

           // szfilename =记事本

       如果参数格式是：％[^ =：]，那么也可以从notepad：1.0.0.1001读取记事本


**注意：当碰到不满足条件的字符后，sscanf的的就会停止执行，不再扫描之后的字符。**

********************** strtok的字符串分割函数也是个实战性较强的函数，但是已被strsep函数取代********* * ***************

` #include <string.h>`` char * strtok（char * str1，const char * str2）;`
函数返回字符串*STR1*中紧接“标记”的部分的指针，字符串*str2的*是作为标记的分隔符。*STR1*  指向作为标记的分隔符。之后所以的调用*STR1*  都应为NULL。

例如：
`    char str [] =“now＃是所有＃好男人来到他们国家的援助之时”;``    char delims [] =“＃”;``    char * result = NULL;``    result = strtok（str，delims）;``    while（result！= NULL）{``        printf（“结果是\”％s \“\ n”，result）;``         result = strtok（NULL，delims）;``    }`
以上代码的运行结果是：
`    结果是“现在”``    结果是“是所有人的时间”``    结果是“好男人来了”``    结果是“援助他们的国家”`
 -------------------------------------------------- -------------------------------

char * strname = NULL;

    则strName =（字符*）malloc的（的sizeof（char）的）;

 memset的（即strName，0，的sizeof（char）的）;

 的strcpy（则strName，“通|太阳|的java |谷歌”）;

    char delims [] =“|”;

    char * ret = NULL;

    RET =（字符*）malloc的（的sizeof（char）的）;

 memset的的（保留，0，的的sizeof（char）的的）;

 RET = strtok的（即strName，delims）;

 while（ret！= NULL）

 {

  printf（“％s \ n”，ret）;

   RET =（字符*）malloc的（的sizeof（char）的）;

  memset的的（保留，0，的的sizeof（char）的的）;

  RET = strtok的（NULL，delims）; 

 }

-------------------------------------------------- -----------------------------------

#   strsep

baikeViewInfo = {ID：“2466295”，编辑：“真”，标题：“strsep”，expIndex：“0”};

　　原型：char * strsep（char ** stringp，const char * delim）;　　功能：分解字符串为一组字符串从stringp指向的位置起向后扫描，遇到DELIM指向位置的字符后，将此字符替换为NULL，返回stringp指向的地址。

 ********************* strncat函数字符串截取接接*********************** * *********

# strncat函数函数

baikeViewInfo = {ID：“1028544”，编辑：“真”，标题：“strncat函数”，expIndex：“0”};

　　原型：extern char * strncat（char * dest，char * src，int n）;　　用法：#include < [string.h](http://baike.baidu.com/view/1728868.htm) >　　功能：把SRC所指字符串的前字符串添加到DEST结尾处（覆盖DEST结尾处的'\ 0'）并添加'\ 0'。　　说明：SRC和DEST所指内存区域不可以重叠且DEST中必须有足够的空间来容纳SRC的字符串。　　返回指向DEST的指针。　　举例：　　// strncat.c　　#include <syslib.h>　　#include <string.h>　　主要（）　　{　　char d [20] =“金色全球”;　　char * s =“查看WinIDE库”;　　clrscr（）;　　strncat函数函数（d，S，5）;　　的printf（“％S”，d）;　　的的getchar（）;　　返回0;　　}

以上代码的运行结果是：

Golden Global View注意查看前面有个空格字符。

*********************** strncasecmp ************************** *

# strncasecmp

baikeViewInfo = {ID：“1589829”，编辑：“真”，标题：“strncasecmp”，expIndex：“0”};

　　相关[函数](http://baike.baidu.com/view/15061.htm)：BCMP，memcmp，STRCMP，与strcoll，STRNCMP　　表头文件：#include <string.h>　　函数定义：int strncasecmp（const char * s1，const char * s2，size_t n）　　函数说明：strncasecmp（）用来比较参数S1和S2字符串前Ñ个字符，比较时会自动忽略大小写的差异　　返回值：若参数s1和s2字符串相同则返回0 s1若大于s2则返回大于0的值s1若小于s2则返回小于0的值　　#include <string.h>

　　char * a =“acddfefekr”;

　　char * b =“AbCddfefekr”;

　　的printf（“％d \ n”个，strncasecmp（A，B，2））;

　　以上代码的运行结果是：1

****************************** strcasecmp ******************* ******

# strcasecmp

（忽略大小写比较字符串）

　　相关函数BCMP，memcmp，STRCMP，与strcoll，STRNCMP

　　表头文件#include < [string.h](http://baike.baidu.com/view/1728868.htm) >

　　定义函数int strcasecmp（const char * s1，const char * s2）;

　　函数说明strcasecmp（）用来比较参数S1和S2字符串，比较时会自动忽略大小写的差异。

　　此原文这里有错误：返回值若参数S1和S2字符串相同则返回0.s1长度大于S2长度则返回大于0的值，S1长度若小于S2长度则返回小于0的值。

　　strcasecmp是比较两个字符串，但比较的不是它们的长度，而是字符的编码大小 - 但字母不区分大小写，如果S1小于S2，返回小于0的一个整数;如果S1等于S2，返回值是0;如果S1> S2，返回一个大于0的整数。

　　#include <string.h>

　　主要（）

　　{

　 　　char * a =“aBcddfefekr”;

 　　char * b =“AbCddfefekr”;

 　　的printf（“％d \ n”个，strcasecmp（A，B））;

　　}

　　执行aBcDeF = AbCdEf

　　以上代码的运行结果是：0

**　　注意与strncasecmp（）的区别，不要弄混了。**

　　该函数是二进制安全的，且对大小写不敏感。

*************************函数strncpy *********************** * *******

　　strncpy：字符复制原型：char * strncpy（char * dest，char * src，  [size_t](http://baike.baidu.com/view/3236587.htm)  n）;

　　功能：将字符串的SRC中最多Ñ个字符复制到字符数组DEST中（它并不像的strcpy的一样遇到NULL才停止复制，而是等凑够Ñ个字符才开始复制），返回指向DEST的指针。

　　说明：

　　如果n> dest串长度，dest栈空间溢出产生崩溃异常。

　　否则：

　　1）SRC串长度<= DEST串长度，（这里的串长度包含串尾NULL字符）

　　如果n =（0，src串长度），src的前n个字符复制到dest中。但是由于没有NULL字符，所以直接访问dest串会发生栈溢出的异常情况。

　　如果n = src串长度，与strcpy一致。

　　如果n = dest串长度，[0，src串长度]处存放于desk字串，（src串长度，dest串长度]处存放NULL。

　　2）SRC串长度> DEST串长度

　　如果N = DEST串长度，则DEST串没有NULL [字符](http://baike.baidu.com/view/47.htm)，导致会输出[会有乱码](http://baike.baidu.com/view/47.htm)。如果不考虑SRC串复制完整性，可以将DEST最后一个字符置为NULL。

　　综上，**一般情况下，使用函数strncpy（）函数时，建议将Ñ置为DEST串长度**（除非你将多个SRC串都复制到目标寄存器数组，并且从DEST尾部反向操作），复制完毕后，为保险起见，将DEST串最后一字符置NULL，避免发生在第2）种情况下的输出乱码问题。当然喽，无论是的strcpy的还是函数strncpy（）函数，保证SRC串长度<目的地串长度才是最重要的。

*******************************的snprintf ***************** * *************

# 的的snprintf

baikeViewInfo = {ID：“1966670”，编辑：“真”，标题：“的snprintf”，expIndex：“0”};

　　int snprintf（char * str，  [size_t](http://baike.baidu.com/view/3236587.htm)  size，const char * format，...）;

　　将可变个[参数](http://baike.baidu.com/view/327406.htm)（...）按照格式格式化成字符串，然后将其复制到海峡中

　　（1）如果格式化后的字符串长度<size，则将此字符串全部复制到str中，并给它后添加一个字符串结束符（'\ 0'）;

　　（2）如果格式化后的字符串长度> = size，则将其中的（size-1）个字符复制到str中，并给它后添加一个字符串结束符（'\ 0'）

　　函数返回值：若成功则返回欲写入的字符串长度，若出错则返回负值.--------------------------- -----------------

    int snprintf（char * restrict buf，size_t n，const char * restrict format，...）;

　　函数说明：最多从源串中拷贝N-1个字符到目标串中，然后再在后面加一个0所以如果目标串的大小为Ñ

      的话，将不会溢出。

　　函数返回值：若成功则返回欲写入的字符串长度，若出错则返回负值。

　　结果1（推荐的用法）

　　#include <stdio.h>

　　#include <stdlib.h>

　　int main（）

　　{

     　　char str [10] = {0，};

     　　的snprintf（STR，的sizeof（STR），“0123456789012345678”）;

     　　printf（“str =％s \ n”，str）;

     　　返回0;

　　}

　　#include <stdio.h> // snprintf（）

　　#include < [string.h](http://baike.baidu.com/view/1728868.htm) > // strlen（）

　　int main（）

　　{

　　char toname [30];

　　char * name =“yangtaiping”;

　　printf（“strlen（name）=％d \ n”，strlen（name））;

　　的snprintf（toname，的sizeof（toname），“姓名：％S”，姓名）;

　　printf（“toname =％s \ n”，toname）;

　　printf（“strlen（name）=％d \ n”，strlen（name））;

　　}

**的strcpy（）的sprintf（）的strcat（）存在安全隐患，其对应的安全版本为：函数strncpy（）的snprintf（）strncat函数（）。**

　　snprintf（s，100，“％。* S”，3，“abcd”）; s的值为abc％。* s表示有两项，第一项指定了长度，第二项则是％s的内容，所以取前三位词条图册更多图册

　　类似的函数还有：

　　#include <stdio.h>

　　int printf（const char * format，...）; //格式化输出到标准输出设备

　　int fprintf（FILE * stream，const char * format，...）; //格式化输出到文件流

　　int sprintf（char * str，const char * format，...）; //格式化的输出字符串输出到STR指向的地址空间

- **void ** reverse_string（**char ** * string）  
- {  
- **if **（* string ==  '\ 0' ）  
-         printf（“％c” ，* string）;  
- **其他**
-     {  
-         reverse_string（++字符串）;  //输出下一个字符
-         printf（“％c” ，*（ - string））;  //因为字符串已经执行完++，所以若要输出当前字符得-
- 
-     }  
- }  
- **int ** main（）  
- {  
- **char ** s [] = “abcdefgh” ;  
-     reverse_string1（一个或多个）;  
- **返回** 0;  
- }
- **void ** reverse_string（**char ** * string）  
- {  
- **if **（* string ==  '\ 0' ）  
-         printf（“％c” ，* string）;  
- **其他**
-     {  
-         reverse_string（++字符串）;  //输出下一个字符
-         printf（“％c” ，*（ - string））;  //因为字符串已经执行完++，所以若要输出当前字符得-
- 
-     }  
- }  
- **int ** main（）  
- {  
- **char ** s [] = “abcdefgh” ;  
-     reverse_string1（一个或多个）;  
- **返回** 0;  
- }

