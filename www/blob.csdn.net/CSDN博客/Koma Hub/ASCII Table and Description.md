# ASCII Table and Description - Koma Hub - CSDN博客
2019年02月11日 20:41:53[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：51
个人分类：[基础知识](https://blog.csdn.net/Rong_Toa/article/category/7587699)
ASCII stands for American Standard Code for Information Interchange. Computers can only understand numbers, so an ASCII code is the numerical representation of a character such as 'a' or '@' or an action of some sort. ASCII was developed a long time ago and now the non-printing characters are rarely used for their original purpose. Below is the ASCII character table and this includes descriptions of the first 32 non-printing characters. ASCII was actually designed for use with teletypes and so the descriptions are somewhat obscure. If someone says they want your CV however in ASCII format, all this means is they want 'plain' text with no formatting such as tabs, bold or underscoring - the raw format that any computer can understand. This is usually so they can easily import the file into their own applications without issues. Notepad.exe creates ASCII text, or in MS Word you can save a file as 'text only'.
[http://www.asciitable.com/](http://www.asciitable.com/)
![](https://img-blog.csdnimg.cn/20190211203954151.gif)
![](https://img-blog.csdnimg.cn/20190211204008290.gif)
```cpp
/*
 * 练习2.7：下述字面值表示何种含义？它们各自的数据类型是什么？
 *
 * (a) "Who goes with F\145rgus?\012"
 * (b) 3.14e1L		(c) 1024f		(d) 3.14L 
 */
/*
 * (a) 表示字符串字面量："Who goes with Fergus?(换行)"
 * (b) long double
 * (c) float
 * (d) long double
 *
 * ASCII码参考：http://www.asciitable.com/
 */
#include <iostream>
int main()
{
	std::cout << "Who goes with F\145rgus?\012";
	return 0;
}
```
结果：
`Who goes with Fergus?`
