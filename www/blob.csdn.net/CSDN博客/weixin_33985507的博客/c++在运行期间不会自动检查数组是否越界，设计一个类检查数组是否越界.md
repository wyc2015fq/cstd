# c++在运行期间不会自动检查数组是否越界，设计一个类检查数组是否越界 - weixin_33985507的博客 - CSDN博客
2010年10月27日 00:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9

```cpp
/*
c++在运行期间不会自动检查数组是否越界，设计一个类检查数组是否越界。 
*/
#include <iostream>
#include <string>
using namespace std;
class check
{
public:
	check(char*s)
	{
		str=new char[strlen(s)+1];
		strcpy(str,s);
		len=strlen(s);
	}
	char operator[](int n)
	{
		if(n>len-1)
		{
			cout<<"数组下标越界"<<endl;
			return ' ';
		}
		else
		{
			cout<<"数组下标没有越界"<<endl;
			return *(str+n);
		}
	}
	void Print(){cout<<str<<endl;}
private:
	char *str;
	int len;
};
void main()
{
	check array("GoodMorning");//类的构造函数
	array.Print();
	cout<<"Location 0:"<<array[0]<<endl;//判断下标为0是否越界
	cout<<"Location 20:"<<array[20]<<endl;//判断下标为20是否越界
}
```
