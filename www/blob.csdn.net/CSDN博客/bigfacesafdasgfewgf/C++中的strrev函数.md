# C++中的strrev函数 - bigfacesafdasgfewgf - CSDN博客





2015年01月20日 10:10:59[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：5066标签：[C++																[strrev																[字符串																[字符数组																[字符串反转](https://so.csdn.net/so/search/s.do?q=字符串反转&t=blog)
个人分类：[C/C++](https://blog.csdn.net/puqutogether/article/category/2595123)





**C++中的strrev函数**



    C++中有函数strrev，功能是对字符串实现反转，但是要记住，strrev函数只对字符数组有效，对string类型是无效的。




    具体见下面代码，其中我还自己实现了一下：






```cpp
#include<iostream>
#include<string>
using namespace std; 

int main()
{
	char s[100] = "china"; 

	cout<<s<<endl; 
	strrev(s); 
	cout<<s<<endl; 

	/* strrev不能对string类型使用 
	string str="china"; 
	cout<<str<<endl; 
	strrev(str.c_str());
	cout<<str<<endl; 
	*/

	char str[100]="china"; 
	cout<<str<<endl; 
	int head=0, tail=strlen(str)-1; 
	for(; head<tail; head++, tail--)
	{
		swap(str[head], str[tail]); 
	}
	cout<<str<<endl; 

	return 0; 
}
```



![](https://img-blog.csdn.net/20150120101101063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=字符数组&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=strrev&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)




