
# C语言程序设计实践（OJ）-用循环解决问题(II) - 迂者-贺利坚的专栏 - CSDN博客

2016年10月26日 20:16:53[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1251个人分类：[C语言教学																](https://blog.csdn.net/sxhelijian/article/category/6420119)



(1032)**统计字符个数**
**Description**
输入一行字符，分别统计出其中英文字母、数字、空格和其他字符的个数。
**Input**
一行字符
**Output**
统计值
**Sample Input**
aklsjflj123 sadf918u324 asdf91u32oasdf/.’;123
**Sample Output**
23 16 2 4
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52937849)
(1923)**ASCII码排序**
**Description**
输入三个字符后，按各字符的ASCII码从小到大的顺序输出这三个字符。
**Input**
输入数据有多组，每组占一行，有三个字符组成，之间无空格。
**Output**
对于每组输入数据，输出一行，字符中间用一个空格分开。
**Sample Input**
qwe
asd
zxc
**Sample Output**
e q w
a d s
c x z
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52937872)
(2506)**切面条**
**Description**
一根高筋拉面，中间切一刀，可以得到2根面条。
如果先对折1次，中间切一刀，可以得到3根面条。
如果连续对折2次，中间切一刀，可以得到5根面条。
那么，连续对折10次，中间切一刀，会得到多少面条呢？
**Input**
包含多组数据，首先输入T,表示有T组数据.每个数据一行，是对折的次数。
**Output**
每行一组数据，输出得到的面条数。
**Sample Input**
3
0
1
2
**Sample Output**
2
3
5
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52937879)
(1609)**等比数列**
**Description**
已知q与n，求等比数列之和：$1+q+q^2+q^3+q^4+…+q^n$
**Input**
输入数据含有不多于50对的数据，每对数据含有一个整数n(1≤n≤20)，一个小数q(0＜q＜2)。
***Output***
对于每组数据n和q，计算其等比数列的和，精确到小数点后3位，每个计算结果应占单独一行。
***Sample Input***
6 0.3 5 1.3
***Sample Output***
1.428
12.756
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52937958)
(2472)**大帆玩英雄联盟**
**Description**
最近大帆喜欢上了玩英雄联盟，而且最喜欢杀别的英雄。大帆玩英雄联盟有个特点，每杀一个英雄他就会十分的兴奋，随之他长长的脑袋就会颤抖一下。根据我的对大帆的研究，他长长的脑袋颤抖次数多了就会口吐白沫，根据多次统计，他平均每杀10个人脑袋就会颤抖一下，脑袋每颤抖10下就会吐一次白沫，他每次玩游戏就会准备一些吃的，他每吃一片面包就会杀一个人。
**Input**
输入大帆准备的面包数n。有多组测试数据。
**Output**
输出大帆脑袋颤抖的次数和口吐白沫的次数。
**Sample Input**
100
120
**Sample Output**
10 1
12 1
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52937971)
(1915)**第几天**
**Description**
给定一个日期，输出这个日期是该年的第几天。
**Input**
输入数据有多组，每组占一行，数据格式为YYYY-MM-DD组成，具体参见Sample Input,另外，可以向你确保所有的输入数据是合法的。
**Output**
对于每组输入数据，输出一行，表示该日期是该年的第几天。
**Sample Input**
2000-01-01
**Sample Output**
1
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52937994)
(2698)**大奖赛计分**
**Description**
在歌手大奖赛中，有9位评委为参赛的选手打分，分数为0~10分。选手最后得分为：去掉一个最高分和一个最低分后，取其余7个分数的平均值。请编写一个程序实现这个程序。
**Input**
9位评委给选手找的分，9个0-10之间的小数
**Output**
选手的最后得分，即9个成绩中去掉一个最高，去掉一个最低后的平均成绩。结果输出中，保留3位小数。
**Sample Input**
9.8 6.7 8.9 7.6 4.5 6.5 7.8 4.2 6.4
**Sample Output**
6.914
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52938012)

