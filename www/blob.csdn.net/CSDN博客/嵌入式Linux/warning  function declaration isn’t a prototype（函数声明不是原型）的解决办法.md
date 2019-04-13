
# warning: function declaration isn’t a prototype（函数声明不是原型）的解决办法 - 嵌入式Linux - CSDN博客

2017年07月19日 11:28:31[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：337


linux驱动中定义一个无参的函数
int probe_num()
{
....
}
警告：函数声明不是一个原型 [-Wstrict-prototypes]
应对方法：
改成
int probe_num(void)
{
....
}
警告消失

http://blog.csdn.net/dumgeewang/article/details/7410477


