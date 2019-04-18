# c++ string 转 char*  const char*  char[] - writeeee的专栏 - CSDN博客
2015年12月19日 23:13:12[writeeee](https://me.csdn.net/writeeee)阅读数：403
```cpp
```
```cpp
#include <STDIO.H>
#include <iostream>
#include <MATH.H>
#include <string>
int main()
{
	/////////////////////////////////////////////////////
	//string ======> char*  char[]
	/////////////////////////////////////////////////////
	std::string source="source str";
	//printf(source);//输出失败  must be a pointer（printf）
	//转化成const char* 用 source.c_str();  source.data(); 
	const char* desPtr1 =source.c_str();//=====ok
	const char* desPtr1_1 =source.data();//=====ok
	printf("%s\n",desPtr1_1);
	std::cout<<desPtr1_1<< std::endl;
	//转 char* 直接转是不行的，得用string的 copy() 或者 strcpy()  一下写了两种开辟内存的方式，分别用了两种复制函数
	//char* desStr1=source;//can't convert
	//char* desStr1=source.c_str();//cant't convert
	char* desPtr1_2 =new char[source.length()+1];
	memset(desPtr1_2,0,source.length()+1);
	strcpy(desPtr1_2,source.c_str());
	delete desPtr1_2;
	
	char* desPtr2=(char*)malloc((source.length()+1)*sizeof(char));
	memset(desPtr2,0,source.length()+1);//注意初始化
	source.copy(desPtr2,source.length());
	printf("%s\n",desPtr2);
	delete desPtr2;
	/////////////////////////////////////////////////////
	//char[] ======> string
	/////////////////////////////////////////////////////
	char charArray[32] = {"abc"};//注意初始化的方式
	char cArray1_2[32];
	std::string charDes1(charArray);
	printf("%s",charDes1.c_str());
	
	system("pause");
	return 0;
}
```
