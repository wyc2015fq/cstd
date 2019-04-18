# C++数据分散管理、集中管理、类的程序实例 - 心纯净，行致远 - CSDN博客





2018年07月01日 22:18:14[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：103








                程序功能：计算圆形、长方形的C++程序 

方法：

1、采用数据分散管理策略，数据分散交给各个函数来管理；

2、采用数据集中管理策略，统一定义公共的变量存放数据；

3、引入类和对象

方法1程序实例：

```cpp
//计算圆形、长方形的C++程序 
//采用数据分散管理策略，数据分散交给各个函数来管理 
#include <iostream>
using namespace std;

double RArea(double r);
double RLen(double r);
double CArea(double a,double b);
double CLen(double a,double b);

int main(){
	double r,a,b;
	cout<<"请依次输入圆形半径，长方形长宽"; 
	cin>>r>>a>>b; 
	
	cout<<CArea(a,b)<<endl;
	cout<<CLen(a,b)<<endl;
	
	cout<<RLen(r)<<endl;
	cout<<RArea(r)<<endl; 
}

double RArea(double r){
	return (3.14*r*r); 
}
double RLen(double r){
	return (3.14*2*r);
}
double CArea(double a,double b){
	return (a*b);
}
double CLen(double a,double b){
	return (2*(a+b));
}
```

方法2实例：

```cpp
//计算圆形、长方形的C++程序 
//采用数据集中管理策略，数据写到外部变量 
#include <iostream>
using namespace std;

double r,a,b;

double CArea();
double CLen();
double RArea();
double RLen();


int main(){
	cout<<"请依次输入圆形半径，长方形长宽"; 
	cin>>r>>a>>b; 
	
	cout<<CArea()<<endl;
	cout<<CLen()<<endl;
	
	cout<<RLen()<<endl;
	cout<<RArea()<<endl; 
}

double RArea(){
	return (3.14*r*r); 
}
double RLen(){
	return (3.14*2*r);
}
double CArea(){
	return (a*b);
}
double CLen(){
	return (2*(a+b));
}
```

方法3实例：

```cpp
//计算圆形、长方形的C++程序 
//采用分类管理策略，使用面向对象程序设计方法 
#include <iostream>
using namespace std;

class Circle	//圆形类：声明
{ 
public:
		double r;
		double CArea();
		double CLen();
};
double Circle::CArea() {	//圆形类：实现 
	return (3.14*r*r);
}
double Circle::CLen(){
	return (2*3.14*r);
}
class Rectangle		//长方形类：声明
{
public:
		double a,b;
		double RArea();
		double RLen();
};
double Rectangle::RArea() {	//长方形类：实现 
	return (a*b);
}
double Rectangle::RLen(){
	return (2*(a+b));
}
int main(){
	Circle obj1;
	Rectangle obj2;
	
	//输入参数
	cin>>obj1.r>>obj2.a>>obj2.b;
	
	cout<<obj1.CArea()<<endl;
	cout<<obj1.CLen()<<endl;
	cout<<obj2.RArea()<<endl;
	cout<<obj2.RLen()<<endl;
	
	return 0;
}
```

方法4：

使用多文件结构和面向对象的程序设计方法。包含1.cpp（包含main函数）、2.cpp、2.h。

1.cpp

```cpp
//计算圆形、长方形的C++程序 
//采用分类管理策略，使用面向对象程序设计方法 
#include <iostream>
using namespace std;

#include "2.h"

int main(){
	//由圆形类和长方形类定义对应的对象 
	Circle obj1;
	Rectangle obj2;
	
	//输入参数
	cin>>obj1.r>>obj2.a>>obj2.b;
	
	cout<<obj1.CArea()<<endl;
	cout<<obj1.CLen()<<endl;
	cout<<obj2.RArea()<<endl;
	cout<<obj2.RLen()<<endl;
	
	return 0;
}
```

2.h

```cpp
class Circle	//圆形类：声明
{ 
public:
		double r;
		double CArea();
		double CLen();
};

class Rectangle		//长方形类：声明
{
public:
		double a,b;
		double RArea();
		double RLen();
};
```
2.cpp

```cpp
#include "2.h"

double Circle::CArea() {	//圆形类：实现 
	return (3.14*r*r);
}
double Circle::CLen(){
	return (2*3.14*r);
}

double Rectangle::RArea() {	//长方形类：实现 
	return (a*b);
}
double Rectangle::RLen(){
	return (2*(a+b));
}
```



