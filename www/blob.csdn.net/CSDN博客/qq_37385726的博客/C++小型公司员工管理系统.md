# C++小型公司员工管理系统 - qq_37385726的博客 - CSDN博客





2017年05月01日 13:03:34[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：754










小型公司员工管理系统



（为了避免繁琐的初始化，所有虚基类的构造函数均不带参数，，采用键盘录入的方式赋值）



子对象：教育背景类==CEducation

（毕业学校，最高学历）



员工类==Staff

（属性：姓名，工作编号，基本工资，奖金，当月出勤记录；教育背景（子对象））

（行为：录入和显示信息，根据考勤等因素计算月薪（不同类别的员工计算的方法各不相同）【所以可以把这个声明为虚函数，当然也可以静态联编，使用作用域限定符就好了】） 



销售人员行政人员经理




销售经理







```cpp
#ifndef _WOSHILEIA_H_
#define _WOSHILEIA_H_
#include
using namespace std;
#include
#include
class CEducation  //定义教育背景类
{
public:
	CEducation();  //空的构造函数
	void Seteducation(void);  //输入毕业学校和最高学历
	void Geteducation(void); //输出毕业学校和最高学历
protected:
	char *gra_school;	//毕业学校
	char highest_edu;  //最高学历
};
class Staff  //定义职员类
{
public:
	Staff();  //空的构造函数
	void SetStaff(void);  //输入职员信息
	void show(void);      //显示职员信息
protected:
	char name[20];  //职员姓名
	long number;  //工作编号
	int base_salary;  //基础工资
	int award; //奖金
	double attendance; //当月出勤记录(0~1之间)
	CEducation education;  //教育背景子对象
};
class Salemen:virtual public Staff  //定义销售人员类
{
public:
	Salemen();  //空的构造函数
	void SetPerAmount(void);//输入个人销售额
	void SetPerRate(void); //输入销售人员提成率
	void GetSalary(void);//计算当月工资
protected:
	int PerAmount;//个人销售额
	double PerRate;//提成比例
};
class Excutive:public Staff  //定义行政人员类
{
public:
	Excutive();  //空的构造函数
	void GetSalary(void); //计算当月工资
};
class Manager :virtual public Staff //定义经理类
{
public:
	Manager(); //定义空的构造函数
	void SetComAmount(void); //输入公司销售额
	void SetComRate(void); //输入经理提成比例
	void GetSalary(void); //计算当月工资
protected:
	int Amount;//销售额
	double Rate;//提成比例
};
class Sale_Manager:public Salemen,public Manager //定义销售经理类
{
public:
	Sale_Manager(); //定义空的构造函数
	void GetSalary(void); //计算当月工资
};
#endif
```



```cpp
#include"woshileia.h"
CEducation::CEducation(){};//CEducation类的空的构造函数
void CEducation::Seteducation(void)//输入毕业学校和最高学历
{
	char *temp_stu = new char[50];
	cout << "请输入毕业学校:";
	cin >> temp_stu;
	cout <> highest_edu;
	highest_edu = toupper(highest_edu);
	gra_school = new char[strlen(temp_stu) + 1];
	strcpy(gra_school, temp_stu);
	delete[]temp_stu;
}
void CEducation::Geteducation(void)//输出毕业学校和最高学历
{
	cout << "毕业学校：" << gra_school << endl;
	switch (highest_edu)
	{
	case 'H':
		cout << "专科" << endl;
		break;
	case 'B':
		cout << "本科" << endl;
		break;
	case 'M':
		cout << "硕研" << endl;
		break;
	case 'D':
		cout << "博研" << endl; 
		break;
	}
}
Staff::Staff(){};//Staff类的空的构造函数
void Staff::SetStaff(void) //输入职员信息
{
	cout << "请输入姓名：";
	cin >> name;
	cout << "请输入工作编号:";
	cin >> number;
	cout << "请输入基础工资:";
	cin >> base_salary;
	cout << "请输入奖金:";
	cin >> award;
	cout<< "请输入当月出勤记录：";
	cin >> attendance;
}
void Staff::show(void)
{
	cout <> PerAmount;
}
void Salemen::SetPerRate(void)
{
	cout << "请输入销售人员的提成比例:";
	cin >> PerRate;
}
void Salemen::GetSalary(void)
{
	cout <<"当月工资：" << base_salary + award + PerAmount*PerRate - (1 - attendance) * 200;
}
Excutive::Excutive(void){};//Excutive类的空的构造函数
void Excutive::GetSalary(void)
{
	cout << "当月工资：" << base_salary + award - (1 - attendance) * 200;
}
Manager::Manager(void){};//Manager类的空构造函数
void Manager::SetComAmount(void)
{
	cout <<  "请输入公司销售额：";
	cin >> Amount;
}
void Manager::SetComRate(void)
{
	cout <<"请输入经理的提成比例：";
	cin >> Rate;
}
void Manager::GetSalary(void)
{
	cout << "当月工资:" << base_salary + award + Amount*Rate - (1-attendance) * 200;
}
Sale_Manager::Sale_Manager(void){};//Sale_Manager类的空构造函数
void Sale_Manager::GetSalary(void)
{
	cout << "当月工资:" << base_salary + award + PerAmount*PerRate + Amount*Rate - (1 - attendance) * 200;
}
```



```cpp
#include"woshileia.h"
#include
int main(void)
{
	char flag = 'y';
	int choose;
	while (toupper(flag) == 'Y')
	{
		cout << "请选择人员类别（1.销售人员，2.行政人员，3.经理，4.销售经理）：";
		cin >> choose;
		switch (choose)
		{
		case 1:
		{
				  Salemen person;
				  person.SetStaff();
				  person.SetPerAmount();
				  person.SetPerRate();
				  cout << "================================================" << endl;
				  person.show();
				  person.GetSalary();
				  break;
		}
		case 2:
		{
				  Excutive person;
				  person.SetStaff();
				  cout << "================================================" << endl;
				  person.show();
				  person.GetSalary();
				  break;
		}
		case 3:
		{
				  Manager person;
				  person.SetStaff();
				  person.SetComAmount();
				  person.SetComRate();
				  cout << "================================================" << endl;
				  person.show();
				  person.GetSalary();
				  break;
		}
		case 4:
		{
				  Sale_Manager person;
				  person.SetStaff();
				  person.SetPerAmount();
				  person.SetPerRate();
				  person.SetComAmount();
				  person.SetComRate();
			      cout << "================================================" << endl;
				  person.show();
				  person.GetSalary();
				  break;
		}
		}
		cout << endl << "================================================";
		cout << endl << "是否继续输入信息（“是”[y],“否”[n]）:";
		cin >> flag;
	}
	system("pause");
	return 0;
}
```







