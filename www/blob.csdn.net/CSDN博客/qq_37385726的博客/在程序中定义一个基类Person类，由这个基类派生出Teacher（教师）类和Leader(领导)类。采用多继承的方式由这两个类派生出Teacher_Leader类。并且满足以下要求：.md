# 在程序中定义一个基类Person类，由这个基类派生出Teacher（教师）类和Leader(领导)类。采用多继承的方式由这两个类派生出Teacher_Leader类。并且满足以下要求： - qq_37385726的博客 - CSDN博客





2017年04月30日 20:20:05[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：2870








a. 在公共基类中包含姓名name、年龄age、性别gender、地址address、电话phone等数据成员，以及用于信息输出的成员函数show。

b. Teacher类的数据成员中还有之职称job、工资salary，Leader类的数据成员还有职务duty,却这两个类都有输出自己信息的成员函数show。

c. 在类中声明成员函数，在类外实现

d. 使用虚基类来避免继承中的二义性。




```cpp
#include
using namespace std;
#include
#include
class Person
{
public:
	Person(char* name, int age, char *gender, char * address, long phone);
	void show(void);
protected:
	char *name;
	int age;
	char *gender;
	char *address;
	long phone;
};
Person::Person(char* name, int age, char *gender, char * address, long phone)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->age = age;
	this->gender = new char[strlen(gender) + 1];
	strcpy(this->gender, gender);
	this->address = new char[strlen(address) + 1];
	strcpy(this->address, address);
	this->phone = phone;
}
void Person::show(void)
{
	cout << "姓名  " << name << endl << "年龄  " << age << endl << "性别  " << gender << endl << "地址  " << address << endl;
	cout<< "电话  " << phone << endl;
}
class Teacher :virtual public Person
{
public:
	Teacher(char* name, int age, char *gender, char * address, long phone, char *job, int salary) :Person(name, age, gender, address, phone)
	{
		this->job = new char[strlen(job) + 1];
		strcpy(this->job, job);
		this->salary = salary;
	}
	void show(void);
private:
	char *job;
	int salary;
};
void Teacher::show(void)
{
	cout << "职称  " << job << endl << "工资  " << salary << endl;
}
class Leader :virtual public Person
{
public:
	Leader(char* name, int age, char *gender, char * address, long phone, char *duty) :Person(name, age, gender, address, phone)
	{
		this->duty = new char[strlen(duty) + 1];
		strcpy(this->duty, duty);
	}
	void show(void);
private:
	char *duty;
};
void Leader::show(void)
{
	cout << "职务  " << duty << endl;
}
class Teacher_Leader :public Teacher, public Leader
{
public:
	Teacher_Leader(char* name, int age, char *gender, char * address, long phone, char *job, int salary, char *duty) :
		Person(name, age, gender, address, phone),
		Teacher(name, age, gender, address, phone, job, salary), Leader(name, age, gender, address, phone, duty){};
	void show(Teacher_Leader p);
};
void Teacher_Leader::show(Teacher_Leader p)
{
	p.Person::show();
	p.Teacher::show();
	p.Leader::show();
}
int main(void)
{
	Teacher_Leader p("张三", 45, "男", "四川大学", 10086, "大学老师", 4500, "教英语");
	p.show(p);
	system("pause");
	return 0;
}
```













