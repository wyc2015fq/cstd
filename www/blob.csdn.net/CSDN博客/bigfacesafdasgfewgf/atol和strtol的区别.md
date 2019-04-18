# atol和strtol的区别 - bigfacesafdasgfewgf - CSDN博客





2015年01月31日 23:28:42[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：4085








**atol和strtol的区别**



    字符串中有两个重要的函数:atol和strtol，它们的功能都是字符数组，转数值。但是用法差异较大。我们下面来说一下这两个函数在具体使用的时候要注意哪些方面。




    首先，说atol。

    这个函数定义为：



```cpp
long atol(const char* s);
```


    就是输入一个字符数组（注意，不是string类型的字符串）的头元素的地址，然后我们就可以转换成数值。举个例子：






```cpp
#include<iostream>
#include<string>

using namespace std; 

int main()
{
	char a[] = "123"; 
	cout<<atoi(a)<<endl; 

	char b[] = "abc"; 
	cout<<atoi(b)<<endl; 

	char c[] = "23&"; 
	cout<<atoi(c)<<endl; 
	
	string str = "234"; 
	cout<<atoi(str.c_str())<<endl; 

	return 0; 
}
```


结果：

![](https://img-blog.csdn.net/20150131233950107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如果我们输入的是第一个字符就是非法的字符，那么返回的是0；

如果我们输入的是前面是有效的数值字符，那么返回前面的数值，后面非法的不返还。




那么如果我们需要用string类型过来操作呢？可以这样子：

我们使用str.c_str()函数。




    再说strtol函数。

函数定义为：

long strtol(const char*s, char** endptr, int base);





    作用就是将字符串转换成长整数，base为进制数。如果转换成功，*endptr指向s; 否则*endptr指向第一个非法字符。


    关于其中的base，规定如下：

![](https://img-blog.csdn.net/20150131234221321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    有下面几个例子：




![](https://img-blog.csdn.net/20150131234325063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150131234347043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





所以说，strtol函数中的输入base的作用还是挺大的。


















