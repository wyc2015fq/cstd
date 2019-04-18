# C++中的字符数组逐个赋值时报错的解决方法 - bigfacesafdasgfewgf - CSDN博客





2015年01月20日 10:03:55[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1322标签：[C++																[字符数组																[赋值																[结束标志位](https://so.csdn.net/so/search/s.do?q=结束标志位&t=blog)
个人分类：[C/C++](https://blog.csdn.net/puqutogether/article/category/2595123)





**C++中的字符数组逐个赋值时报错的解决方法**



    C++中定义一个字符数组的时候，如果初始化,则cout的时候不会出错，但是在逐个字符赋值的时候，很容易出错。






```cpp
#include<iostream>
#include<string>
using namespace std; 

int main()
{
	//char s[100] = "china"; 
	char s[100]; 
	s[0]='c'; 
	s[1]='n'; 
	s[2]='\0'; 
	cout<<s<<endl; 

	return 0; 
}
```




如果我们直接初始化：


![](https://img-blog.csdn.net/20150120100122911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





如果我们逐个字符的赋值：




![](https://img-blog.csdn.net/20150120100206341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








这是为什么呢？




原来是在逐个字符赋值的时候，一定要记住最后来个s[len-1]='\0'; 这个是字符串的结束标志！！




改正的输出如下：




![](https://img-blog.csdn.net/20150120100320878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=赋值&t=blog)](https://so.csdn.net/so/search/s.do?q=字符数组&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)




