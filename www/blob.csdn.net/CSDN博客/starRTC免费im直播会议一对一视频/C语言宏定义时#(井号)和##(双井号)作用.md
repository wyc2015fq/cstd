# C语言宏定义时#(井号)和##(双井号)作用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月17日 14:11:02[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：766
#的功能是将其后面的宏参数进行字符串化操作（Stringfication），简单说就是在对它所引用的**宏变量** 通过替换后在其左右各加上一个双引号。
#define example(instr) printf("the input string is:\t%s\n",#instr)
#define example1(instr) #instr
当使用该宏定义时：
example(abc); 在编译时将会展开成：printf("the input string is:\t%s\n","abc");
string str=example1(abc); 将会展成：string str="abc";
而##被称为连接符（concatenator），用来将两个Token连接为一个Token。
