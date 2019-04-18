# C++对象数组——MOOC阚道宏 - 心纯净，行致远 - CSDN博客





2018年07月03日 13:37:48[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：70








定义了一个对象数组，如下：

```cpp
#include <iostream>
using namespace std;

class Square	//定义一个正方形类
{
	public:
		double a;	//保存边长的double型数据成员a 
		double Area(){	//求正方形面积的函数成员Area,内联函数 
			return (a*a);
		}
		Square(double x=0){	//带默认形参值的构造函数，内联函数 
			a=x;
		}
 }; 
 
 int main(){
 	//定义正方形类Square的对象数组obj，定义时初始化
	//数组obj有3个元素，每个元素都是一个正方形对象，边长分别是2/3/4 
 	Square obj[3]={Square(2),Square(3),Square(4)};
	 
	 //显示数组中各正方形对象的边长和面积
	 int n;
	 for(n=0;n<3;n++){
	 	cout<<obj[n].a<<endl;
	 	cout<<obj[n].Area()<<endl; 
	 } 
	 return 0;
 }
```




