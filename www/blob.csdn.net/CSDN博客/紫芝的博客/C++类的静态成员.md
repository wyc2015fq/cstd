# C++类的静态成员 - 紫芝的博客 - CSDN博客





2018年12月07日 18:23:54[紫芝](https://me.csdn.net/qq_40507857)阅读数：1421








**静态成员的提出是为了解决数据共享的问题，实现共享有多种方法，全局变量或对象具有局限性和不安全性**
- 静态数据成员
 可以实现多个对象之间的数据共享，并且使用静态数据成员还不会破坏隐藏的原则，保证了安全性 
- 静态成员函数
和静态数据成员一样，都属于类，而不属于某个对象，对静态成员的引用可以用类名限定的方法和静态数据成员一样，都属于类，而不属于某个对象，对静态成员的引用可以用类名限定的方法

```cpp
#include <iostream>
#include <string.h>
using namespace std;
class Student
{
public:
	Student(int Nu,char *Na,double Sc):Num(Nu)//学号唯一
	{
		int L=strlen(Na);
		Name=new char[L+1];
		strcpy(Name,Na);

		Score=Sc;

		Total_Score=Total_Score+Score;
	}
	~Student()
	{delete[] Name;}

	void Print()
	{
		cout<<"Num\t"<<Num<<endl;
		cout<<"Name\t"<<Name<<endl;
		cout<<"Score\t"<<Score<<endl;

	}
	static void Print_Total_Score()//静态函数，不依赖于对象
	{
			cout<<"Total_Score\t"<<Total_Score<<endl;
	}

private:
	const int Num;
	char *Name;
	double Score;
	static double Total_Score;
};

double Student::Total_Score=0;

int main()
{
	Student s1(10001,"Zhang San",90),
		    s2(10002,"Zhang San",80),
            s3(10003,"Zhang San",70);
	s1.Print();
	cout<<"----------------------------\n";
	s2.Print();
	cout<<"----------------------------\n";
	s2.Print();
	cout<<"----------------------------\n";
	Student::Print_Total_Score();
}
```



