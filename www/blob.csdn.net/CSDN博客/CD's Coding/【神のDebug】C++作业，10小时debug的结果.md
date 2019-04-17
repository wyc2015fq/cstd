# 【神のDebug】C++作业，10小时debug的结果 - CD's Coding - CSDN博客





2013年11月17日 23:02:54[糖果天王](https://me.csdn.net/okcd00)阅读数：858








Debug是程序员的天职，寻找Bug的过程痛苦难熬啊……但是Debug出来之后顿时觉得世界一片美好^_^

 E:\【资料】我的编程\DevC++\ProXYC Fin 的目录

2013/11/17  22:58    <DIR>          .

2013/11/17  22:58    <DIR>          ..

2013/11/17  20:58                78 diary.txt

2013/11/17  12:57             2,683 main.cpp

2013/11/17  18:11             1,735 Product XYC Ver4.0.dev

2013/11/17  18:09                34 Product.txt

2013/11/17  01:12             8,566 ProductList.cpp

2013/11/17  12:57               873 ProductList.h

2013/11/16  22:37               549 ProductNode.cpp

2013/11/16  23:15               684 ProductNode.h

               8 个文件         15,202 字节

               2 个目录 30,579,601,408 可用字节




***Main.cpp***



```cpp
#include "ProductList.h"
//Finished on Nov.17th 18:10
using namespace std;

void menu()
{
cout
<<" *┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*\n"
<<" *┃                      商品信息维护系统Ver4.0                          ┃*\n"
<<" *┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃*\n"
<<" *┃                    ◎从下面的功能中选择一个◎                        ┃*\n"
<<" *┃1.显示全部信息          5.进货（插入）        9.存盘                  ┃*\n" 
<<" *┃2.查询型号              6.提货（删除）        0.结束(存盘退出)        ┃*\n"
<<" *┃3.查询名称              7.修改（更改）        a.恢复(丢失存盘前更改)  ┃*\n"
<<" *┃4.查询品牌              8.退出（放弃存储）    Copyright CYX           ┃*\n"
<<" *┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*\n\n";
}
//CYX Stand For Chendian Yanran Xujiali
//主程序
int main()
{
ProductList pl;
cout
<<" *┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*\n"
<<" *┃                           商品信息维护系统                           ┃*\n"
<<" *┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃*\n"
<<" *┃                        从下面的功能中选择一个!                       ┃*\n"
<<" *┃                                                                      ┃*\n" 
<<" *┃                             1.开始营业                               ┃*\n"
<<" *┃                             2.退出系统                               ┃*\n"
<<" *┃                     <Tip>测试用账号密码：okcd00                      ┃*\n"
<<" *┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*\n\n";
cout<<"\n请选择：";
string choice;
while(1)
{
cin>>choice;
if(choice[0]=='2')exit(0);
else if(choice[0]!='1')cout<<"此序号不存在，请重新输入！\n";
else if(pl.JudgeCode())//如果密码正确
{
pl.ReadFile();//读入文件
while(1)
{
cout<<"请按回车继续...";
getchar();
getchar();
system("cls");//清屏
menu();//显示菜单
cout<<"请输入您需要的功能:（单字符 0-a）";
cin>>choice;//选择

switch(choice[0])
                 {
                  case '1':pl.PrintList();break;    //显示全部产品信息
                  case '2':pl.FindByNO();break;     //按型号查询
                  case '3':pl.FindByName();break;   //按名称查询 
                  case '4':pl.FindByBrand();break;  //按品牌查询
                  case '5':pl.Insert();break;       //进货(插入结点)
                  case '6':pl.Delete();break;       //提货(删除结点) 
                  case '7':pl.Modify();break;       //修改(修改结点) 
                  case '8':{cout<<"谢谢您的使用！\n";exit(0);} //放弃存储直接退出
                  case '9':pl.WriteFile();break;    //存盘
                  case '0':{pl.WriteFile();cout<<"谢谢您的使用！\n请按回车退出此程序";getchar();exit(0);}//存盘退出
                  case 'a':{pl.DataResume();break;}//数据恢复
                  default:cout<<"您的输入不正确，请重新输入！\n"; 
}
}
}
else cout<<"请选择：";
}
}
```

***ProductList.cpp***




```cpp
/////////////////////Note//////////////////////////////////
//最大的Bug： ProductNode* nex 引用函数必须是nex->func() //
//            话说ProductNode s就必须是s.func()          //
/////////////////////////////////////////////////////////// 


//**在单链表中有序插入结点**//
//8th Correction 2013.11.17 
#include "ProductList.h" 
//#define Cout(a) cout<<a->getNO()<<"\t\t"<<a->getName()<<"\t\t"<<a->getBrand()<<"\t\t"<<a->getPrice()<<"\t\t"<<a->getQuantity()<<endl;
//偷个懒用define结果花了更多时间Debug……今天一天就用来debug了…… 11.17 15:00 

//////////////////////////////////////////
	string tmpNO;//型号
	string tmpName;//名称
	string tmpBrand;//品牌
	int tmpPrice;//卖出价
	int tmpQuantity;//数量
//鉴于不知道如何重载数据类型为 Product::getNO() 的运算符，直接绕一个弯读入好了 
//////////////////////////////////////////

void ProductList::InitInsert(ProductNode* s) //插入初始化 
{
 ProductNode* f=first;
 ProductNode* nex=first->next;

while(nex&&nex->getPrice()<s->getPrice()){ f=nex; nex=nex->next;}//f结点始终为nex结点的前趋结点，退出循环时，s应插入f结点后
 s->next=f->next;
 f->next=s;
}

//**读入文件**//
//Readfile又闪退了……11.17 
//Solved on 11.17 要把product.txt放入工程，而且大小写不能错= = 
void ProductList::ReadFile()
{
 ifstream fin("product.txt");//输入文件流对象
 if(fin.fail())
 {
               cout<<" product.txt 文件读入错误!\n";
               cout<<"请按回车键退出...";
               getchar();
               exit(0);
 }
 string oneline;//temp-文件的一行
 ProductNode* r=first;
 while(getline(fin,oneline))//当文件没有结束,读一行
 {
   istringstream sin(oneline);//字符串流,库函数sstream.h中的一员 
   ////////Try Once But Don't Know if it can be used//////
   ProductNode *s=new ProductNode; //NEW 一个新的结点 
   sin>>tmpNO>>tmpName>>tmpBrand>>tmpPrice>>tmpQuantity;
   s->setNO(tmpNO);
   s->setName(tmpName);
   s->setBrand(tmpBrand);
   s->setPrice(tmpPrice);
   s->setQuantity(tmpQuantity);
   InitInsert(s);
 }///////////////////////////////////////////////////////////
}

//**输出链表数据**//
//每次都会自动卡在PList这里……11.17 14:17 是While死循环了吗…… 
//把Cout Define去掉之后正常了，但是又闪退了= =，疯掉了= = 
void ProductList::PrintList()const
{
 cout<<"产品信息如下：\n";
 cout<<"型号"<<"\t\t"<<"名称"<<"\t\t"<<"品牌"<<"\t\t"<<"单价"<<"\t\t"<<"数量"<<endl;
 ProductNode* nex=first->next;
 while(nex)
{
cout<<nex->getNO()<<"\t\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t\t"<<nex->getPrice()<<"\t\t"<<nex->getQuantity()<<endl;
nex=nex->next;
}
}

//**营业结束,写入文件**//
void ProductList::WriteFile()
{
ofstream fout("product.txt");//输出文件流对象 
ProductNode* nex=first->next;
while(nex)
{
fout<<nex->getNO()<<"\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t"<<nex->getPrice()<<"\t"<<nex->getQuantity()<<endl;
nex=nex->next;
}
ofstream fout2("diary.txt");//清空日志文件(DEBUG 用)，可注释掉 
cout<<"存盘成功！\n";
}

//析构函数
ProductList::~ProductList()
{
 ProductNode* nex=first; 
 ProductNode* q;
 while(nex) //释放单链表的每一个结点的存储空间
 {
  q=nex; //暂存被释放结点
  nex=nex->next; //工作指针nex指向被释放结点的下一个结点，使单链表不断开
  delete q; 
 }
}

//**根据型号查找**(结果不止一个,所以用void)//
void ProductList::FindByNO()
{
string NO;
bool flag=false;//假定没有此产品
cout<<"输入产品型号：";
cin>>NO;
ProductNode* nex;
for(nex=first->next;nex;nex=nex->next)
if(nex->getNO()==NO)
{
if(flag==false)//只输出一次标题
cout<<"查询结果如下：\n"<<"型号"<<"\t\t"<<"名称"<<"\t\t"<<"品牌"<<"\t\t"<<"单价"<<"\t\t"<<"数量"<<endl;
cout<<nex->getNO()<<"\t\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t\t"<<nex->getPrice()<<"\t\t"<<nex->getQuantity()<<endl;
flag=true;//存在产品
}
if(flag==false)cout<<"无此产品！";
}
//**根据名称查找**//
void ProductList::FindByName()
{
string Name;
bool flag=false;//假定没有
cout<<"输入产品名称：";
cin>>Name;
ProductNode* nex=first->next;
for(nex=first->next;nex;nex=nex->next)
if(nex->getName()==Name)
{
if(flag==false)
cout<<"查询结果如下：\n"<<"型号"<<"\t\t"<<"名称"<<"\t\t"<<"品牌"<<"\t\t"<<"单价"<<"\t\t"<<"数量"<<endl;
cout<<nex->getNO()<<"\t\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t\t"<<nex->getPrice()<<"\t\t"<<nex->getQuantity()<<endl;
flag=true;
}
if(flag==false)cout<<"无此产品！";
}
//**根据品牌查找**//
void ProductList::FindByBrand()
{
string Brand;
bool flag=false;//假定没有
cout<<"输入产品品牌：";
cin>>Brand;
ProductNode* nex=first->next;
for(nex=first->next;nex;nex=nex->next)
if(nex->getBrand()==Brand)
{
if(flag==false)
cout<<"查询结果如下：\n"<<"型号"<<"\t\t"<<"名称"<<"\t\t"<<"品牌"<<"\t\t"<<"单价"<<"\t\t"<<"数量"<<endl;
cout<<nex->getNO()<<"\t\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t\t"<<nex->getPrice()<<"\t\t"<<nex->getQuantity()<<endl;
flag=true;
}
if(flag==false)cout<<"无此产品！";
}
//**进货，插入节点**//
//2013.11.17 Insert Rewrite 4th
void ProductList::Insert()
{
PrintList();
string NO,Name,Brand;
cout<<"请输入产品信息插入（输入产品型号时输入z并按回车 停止插入并返回上一级）\n";
cout<<"产品型号：";
cin>>NO;
if(NO[0]=='z') exit(0); //debug By XJL_ZoeCUR 11.17 18:07 Wrong by return -> exit(0)
ProductNode *s=new ProductNode;
s->setNO(NO);
cout<<"产品名称：";
cin>>Name;
s->setName(Name);
cout<<"产品品牌：";
cin>>Brand;
s->setBrand(Brand);

ProductNode *nex=first->next;
while (nex&&!(nex->getNO()==s->getNO()&&nex->getName()==s->getName()&&nex->getBrand()==s->getBrand())) nex=nex->next;
////这里的nex是一个指针，但是指针的话不知道如何才可以调用指针所指的类的函数/////
//2013.11.17 不知道怎么就编译通过了…… 庆祝^_^ Lucky 

if(nex)//此类产品存在
{
cout<<"此类产品存在!输入进货数量\n"; 
cout<<"产品数量：";
int Quantity; 
cin>>Quantity;
s->setQuantity(Quantity);
if(s->getQuantity()<=0){cout<<"数据错误!\n";return;}
tmpQuantity=nex->getQuantity();
tmpQuantity+=s->getQuantity();
nex->setQuantity(tmpQuantity);
tmpPrice=nex->getPrice();
s->setPrice(tmpPrice);//便于修改日志文件(ForDebug)
}
else//此类产品不存在
{
cout<<"产品单价：";
cin>>tmpPrice;
s->setPrice(tmpPrice);
if(s->getPrice()<=0){cout<<"数据错误!\n";return;}
cout<<"产品数量：";
cin>>tmpQuantity;
s->setQuantity(tmpQuantity);
if(s->getQuantity()<=0){cout<<"数据错误!\n";return;}
InitInsert(s);
}
ofstream fout("diary.txt",ios::app);//向日志文件中添加记录
   tmpNO=s->getNO();
   tmpName=s->getName();
   tmpBrand=s->getBrand();
   tmpPrice=s->getPrice();
   tmpQuantity=s->getQuantity();
fout<<"进货"<<"\t"<<tmpNO<<"\t"<<tmpName<<"\t\t"<<tmpBrand<<"\t"<<tmpPrice<<"\t"<<tmpQuantity<<endl;
cout<<"修改成功\n";
PrintList();
}
//**提货，数量减少or删除结点**//
bool ProductList::Delete()
{
PrintList();
cout<<"输入卖出产品的信息！\n";
string NO,Name,Brand;
cout<<"输入型号：（输入z返回）";
cin>>NO;
if(NO[0]=='z')return false;
cout<<"产品名称：";
cin>>Name;
cout<<"产品品牌：";
cin>>Brand;
//InitInsert(&s);//Something wrong

 ProductNode* nex=first->next;
 ProductNode* f=first;

while (nex&&!(nex->getNO()==NO&&nex->getName()==Name&&nex->getBrand()==Brand)) //查找结点
{
f=nex;
nex=nex->next;
}
if (!nex)//产品不存在 
{
cout<<"此产品不存在!\n"; 
return false;
}
else//产品存在 
{
int Quantity;
int Price=nex->getPrice();//修改日志用，因为nex结点要被删除
cout<<"输入提货数量：";
cin>>Quantity;
while(Quantity>nex->getQuantity()){cout<<"输入的数量超出库存量,请重新输入！\n";cin>>Quantity;}
if(Quantity<nex->getQuantity())
{
 tmpQuantity=nex->getQuantity();
 tmpQuantity-=Quantity;
 nex->setQuantity(tmpQuantity);
}

else//数量相等,删除结点 
{
f->next=nex->next;
delete nex;
cout<<"此产品被删除!\n";
}
cout<<"修改成功\n";
PrintList();
ofstream fout("diary.txt",ios::app);//向日志文件中添加记录
fout<<"提货"<<"\t"<<NO<<"\t"<<Name<<"\t\t"<<Brand<<"\t"<<Price<<"\t"<<Quantity<<endl;
return true;
}
}
//**数据恢复（读取日志文件进行相应操作）**//
void ProductList::DataResume()
{
ifstream fin("diary.txt");
string Type;//进货or提货
string oneline;
while(getline(fin,oneline))//当文件没有结束,读一行
{
istringstream sin(oneline);//字符串流
ProductNode* s=new ProductNode; 
   sin>>Type>>tmpNO>>tmpName>>tmpBrand>>tmpPrice>>tmpQuantity;
   s->setNO(tmpNO);
   s->setName(tmpName);
   s->setBrand(tmpBrand);
   s->setPrice(tmpPrice);
   s->setQuantity(tmpQuantity);
   
if(Type=="进货")
{ 
ProductNode* nex=first->next; //工作指针nex初始化
while (nex&&!(nex->getNO()==s->getNO()&&nex->getName()==s->getName()&&nex->getBrand()==s->getBrand())) //查找第i个结点
nex=nex->next;
if(nex)//此类产品存在
{
tmpQuantity=nex->getQuantity();
tmpQuantity+=s->getQuantity();
nex->setQuantity(tmpQuantity);
}
else InitInsert(s);//此类产品不存在
}
if(Type=="提货")
{
ProductNode* nex=first->next; //工作指针nex初始化
ProductNode* f=first; //工作指针nex初始化
while (nex&&!(nex->getNO()==s->getNO()&&nex->getName()==s->getName()&&nex->getBrand()==s->getBrand())) //查找第i-1个结点
{
f=nex;
nex=nex->next;
}
if (nex)//产品存在 
{
if(s->getQuantity()<nex->getQuantity())
{
 tmpQuantity=nex->getQuantity();
 tmpQuantity-=s->getQuantity();
 nex->setQuantity(tmpQuantity);
}
else if(s->getQuantity()==nex->getQuantity())//数量相等,删除结点 
{
f->next=nex->next;
delete nex;
}
}
}
} 
cout<<"数据恢复成功\n";
PrintList();
}
//**修改产品信息**//
bool ProductList::Modify()
{
PrintList();
cout<<"输入要修改的产品信息!\n";
string NO,Name,Brand;
cout<<"产品型号：(输入'z'返回)";
cin>>NO;
if(NO[0]=='z')return false;
cout<<"产品名称：";
cin>>Name;
cout<<"产品品牌：";
cin>>Brand;
ProductNode* nex=first->next; 
ProductNode* f=first; 
while (nex&&!(nex->getNO()==NO&&nex->getName()==Name&&nex->getBrand()==Brand)) //查找结点
{
f=nex;
nex=nex->next;
}
if (!nex) //结点nex不存在
{
cout<<"此产品不存在!\n"; 
return false;
}
else //结点nex存在
{
ofstream fout("diary.txt",ios::app);//向日志文件中添加记录
fout<<"提货"<<"\t"<<nex->getNO()<<"\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t"<<nex->getPrice()<<"\t"<<nex->getQuantity()<<endl;
int Price,Quantity;
cout<<"此产品信息如下：\n";
cout<<"型号："<<nex->getNO()<<"\n名称："<<nex->getName()<<"\n品牌："<<nex->getBrand()<<"\n单价："<<nex->getPrice()<<"\n数量："<<nex->getQuantity()<<endl;
cout<<"输入新型号：（输入z型号不变）";
cin>>NO;
if(NO[0]!='z')nex->setNO(NO);//原来读入和输出又记反了T^T  2013.11.17 
cout<<"输入新名称：（输入z名称不变）";
cin>>Name;
if(Name[0]!='z')nex->setName(Name);
cout<<"输入新品牌：（输入z品牌不变）";
cin>>Brand;
if(Brand[0]!='z')nex->setBrand(Brand);
cout<<"输入新单价：（输入0保持原价不变）";
cin>>Price;
if(Price!=0)nex->setPrice(Price);
cout<<"输入新数量：（输入0保持原数量不变）";
cin>>Quantity;
if(Quantity!=0)nex->setQuantity(Quantity);
//向日志文件中添加记录
fout<<"进货"<<"\t"<<nex->getNO()<<"\t"<<nex->getName()<<"\t\t"<<nex->getBrand()<<"\t"<<nex->getPrice()<<"\t"<<nex->getQuantity()<<endl;
//按单价顺序调整记录顺序
if(!(Price==0||(Price>=f->getPrice()&&(nex->next==NULL||Price<=nex->next->getPrice()))))//需要调整
{//单价没有改变or 单价>前一个结点的单价（包括头结点）并且 单价<后一个结点的单价（包括后一个结点为空）
f->next=nex->next;//先删除nex
InitInsert(nex);//再按单价顺序插入nex
}
cout<<"修改成功！";
PrintList();
return true;
}
}
//判断密码//
bool ProductList::JudgeCode() const
{
bool power=true;
while(1)
{
char goon;
cout<<"输入用户名:";
string name;
cin>>name;
cout<<"输入6位密码:";
char code[7];
for(int i=0;i<7;i++)
{
code[i]=_getch();//#include<conio.h>
if(i<6)cout<<'*';
}
cout<<endl;
if(name=="okcd00"&&code[0]=='o'&&code[1]=='k'&&code[2]=='c'&&code[3]=='d'&&code[4]=='0'&&code[5]=='0') 
break;
else cout<<"用户名或密码错误,是否重新输入?(y/n):";
cin>>goon;
if(goon=='n') {power=0;break;};
}
return power;
}
```

***ProductList.h***




```cpp
//产品库存，使用链表存储 
#include "ProductNode.h"
using namespace std;

class ProductList
{
ProductNode* first;//头结点
void InitInsert(ProductNode* s); //私有成员函数，读文件创建链表 
public:
ProductList(){first=new ProductNode;first->next=NULL;}//建立只有头结点的空链表
void ReadFile(); //营业开始,读入文件
void WriteFile(); //营业结束,写入文件
void Insert(); //进货，插入结点
void FindByNO(); //根据型号查找
void FindByName(); //根据名称查找
void FindByBrand(); //根据品牌查找
bool Delete(); //提货，删除结点
bool Modify(); //修改信息
void PrintList()const;//遍历单链表，按序号依次输出各元素
void DataResume(); //读入问卷 
~ProductList(); //析构函数
bool JudgeCode()const;//判断密码

private:

};

/*
istream operator <<(想写重载……但是括号里应该些什么呢……)//输出函数返回值的重载 
{
        cout<<
}
*/
```

***ProductNode.cpp***




```cpp
#include "ProductNode.h"
#include <cstring>

//string &operator=(const string &s);

//set private 
void ProductNode::setNO(string s){NO=s;}
void ProductNode::setName(string s){Name=s;}
void ProductNode::setBrand(string s){Brand=s;};
void ProductNode::setPrice(int i){Price=i;}
void ProductNode::setQuantity(int q){Quantity=q;}

//get private
string ProductNode::getNO(){return  NO;}
string ProductNode::getName(){return  Name;}
string ProductNode::getBrand(){return  Brand;}
int ProductNode::getPrice(){return  Price;}
int ProductNode::getQuantity(){return  Quantity;}
```

***ProductNode.h***




```cpp
//产品结点，本来想用Struct来着的……老师不让……就改成类了 

#include<string>
#include<iostream>
#include<fstream>//For file writing
#include<cstdlib>
#include<sstream>//istringstream & ostringstream
#include<conio.h>//single input judging function

using namespace std;

class ProductNode
{
public:
	void setNO(string s);
	void setName(string s);
	void setBrand(string s);
	void setPrice(int i);
	void setQuantity(int q);
	string getNO();
	string getName();
	string getBrand();
	int getPrice();
	int getQuantity();
	ProductNode* next;
//	class ProductList;
private:
	string NO;//型号
	string Name;//名称
	string Brand;//品牌
	int Price;//卖出价
	int Quantity;//数量
	//各种……is private with this context 
};
```







