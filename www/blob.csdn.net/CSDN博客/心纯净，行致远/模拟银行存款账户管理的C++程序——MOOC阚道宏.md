# 模拟银行存款账户管理的C++程序——MOOC阚道宏 - 心纯净，行致远 - CSDN博客





2018年07月02日 21:10:40[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：375








程序由甲乙两个程序员完成，采用面向对象的程序设计方法。甲程序员完成主函数的设计，也就是main.cpp。乙程序员设计类并留出接口。

main.cpp

```cpp
#include <iostream>
using namespace std;

#include <string.h>	//插入头文件string.h，声明系统函数strcpy()
#include "2.h"	//插入头文件2.h，声明银行账户类Account

int main(){
	//键盘输入账号，账户名和存款金额等开户信息，先定义3个临时变量
	cout<<"请输入开户信息（账号、账户名、存款金额）：";
	int x;char str[10];float y;
	cin>>x>>str>>y;
	
	//创建账户obj
	Account obj;
	obj.no=x;
    strcpy(obj.name,str);
	obj.money =y;
	
	//管理账户
	int choice; 
	while(true){
		cout<<"1-存款\n2-取款\n3-查询余额\n0-退出\n请选择：";
		cin>>choice;
		if(choice==0) break;	//退出
		if(choice==1) obj.Deposit() ;	//存款
		if(choice==2) obj.Withdraw() ;	//取款
		if(choice==3) obj.Show() ;	//查询余额 
	} 
	return 0; 
}
```

2.h

```cpp
class Account{	//类声明部分 
	public:
		int no;	//账号
		char name[10];	//账户名
		float money;	//存款金额
		void Deposit();	//存款
		void Withdraw();	//取款
		void Show();	//显示金额 
};
```

2.cpp

```cpp
//源程序文件：2.cpp
#include <iostream>
using namespace std;

#include "2.h"	//插入头文件account，声明银行账户类Acccount
//类实现部分

void Account::Show() {
	cout<<"账号"<<no<<"的账户余额为："<<money<<"元\n\n"; 
}

void Account::Deposit() //存款
{
	cout<<"请输入存款金额";
	float x;
	cin>>x;
	money+=x;
	Show();
}

void Account::Withdraw() {	//取款 
	cout<<"请输入取款金额";
	float x;
	cin>>x;
	money-=x;
	Show();
}
```






