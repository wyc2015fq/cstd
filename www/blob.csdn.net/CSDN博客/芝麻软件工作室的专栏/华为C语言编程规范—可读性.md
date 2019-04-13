
# 华为C语言编程规范—可读性 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:37:31[seven-soft](https://me.csdn.net/softn)阅读数：641


**4-1：注意运算符的优先级，并用括号明确表达式的操作顺序，避免使用默认优先级。**
说明：防止阅读程序时产生误解，防止因默认的优先级与设计思想不符而导致程序出错。
示例：下列语句中的表达式
word = (high << 8) | low (1)
if ((a | b) && (a & c)) (2)
if ((a | b) < (c & d)) (3)
如果书写为
high << 8 | low
a | b && a & c
a | b < c & d
由于
high << 8 | low = ( high << 8) | low,
a | b && a & c = (a | b) && (a & c)，
(1)(2)不会出错，但语句不易理解；a | b < c & d = a | （b < c） & d，(3)造成了判断条件出错。
**4-2：避免使用不易理解的数字，用有意义的标识来替代。**
涉及物理状态或者含有物理意义的常量，不应直接使用数字，必须用有意义的枚举或宏来代替。示例：如下的程序可读性差。
if (Trunk[index].trunk_state == 0)
{
Trunk[index].trunk_state = 1;
... // program code
}
应改为如下形式。
\#define TRUNK_IDLE 0
\#define TRUNK_BUSY 1
if (Trunk[index].trunk_state == TRUNK_IDLE)
{
Trunk[index].trunk_state = TRUNK_BUSY;
... // program code
}
**4-3：源程序中关系较为紧密的代码应尽可能相邻。**
说明：便于程序阅读和查找。
示例：以下代码布局不太合理。
rect.length = 10;
char_poi = str;
rect.width = 5;
若按如下形式书写，可能更清晰一些。
rect.length = 10;
rect.width = 5; // 矩形的长与宽关系较密切，放在一起。
char_poi = str;
**4-4：不要使用难懂的技巧性很高的语句，除非很有必要时。**
说明：高技巧语句不等于高效率的程序，实际上程序的效率关键在于算法。
示例：如下表达式，考虑不周就可能出问题，也较难理解。
* stat_poi ++ += 1;
* ++ stat_poi += 1;
应分别改为如下。
*stat_poi += 1;
stat_poi++; // 此二语句功能相当于“ * stat_poi ++ += 1; ”
++ stat_poi;
*stat_poi += 1; // 此二语句功能相当于“ * ++ stat_poi += 1; ”

