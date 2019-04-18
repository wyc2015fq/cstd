# C++设计模式实现--组合(Composite)模式 - L_Andy的专栏 - CSDN博客

2014年06月17日 15:10:38[卡哥](https://me.csdn.net/L_Andy)阅读数：989


**一. 举例**

这个例子是书上的，假设有一个公司的组结结构如下：
![](https://img-my.csdn.net/uploads/201205/10/1336622103_2421.jpg)

它的结构很像一棵树，其中人力资源部和财务部是没有子结点的，具体公司才有子结点。

而且最关健的是，它的每一层结构很相似。

代码实现如下：

#include <iostream>

#include <list>

#include <string>

using namespace std;

//公司类，提供接口  

class Company      

{    

public:    
Company(string name)  
{  
m_name = name;  
}    

virtual ~Company()  
{}    

virtual void Add(Company *pCom)  
{}    

virtual void Display(int depth)  
{}  

protected:  
string m_name;    

};  

//具体公司  

class ConcreteCompany : public Company      

{    

public:    
ConcreteCompany(string name): Company(name)  
{}  

virtual ~ConcreteCompany()  
{}  

//增加子树或叶子  
void Add(Company *pCom)  
{  
m_listCompany.push_back(pCom);  
}  

//显示  
void Display(int depth)  
{  
for(int i = 0;i < depth; i++)  
{  
cout<<"-";    

}  

cout<< m_name << endl;  

list<Company *>::iterator iter = m_listCompany.begin();    

for(; iter != m_listCompany.end(); iter++) //显示下层结点    
{      
(*iter)->Display(depth + 2);  
}  
}  

private:    
list<Company *> m_listCompany;    

};    

//具体的部门，财务部    

class FinanceDepartment : public Company     

{  

public:    
FinanceDepartment(string name):Company(name)  
{}    

virtual ~FinanceDepartment()  
{}    

//只需显示，无限添加函数，因为已是叶结点    
virtual void Display(int depth)  
{    
for(int i = 0; i < depth; i++)    
cout<<"-";    

cout<< m_name << endl;  
}  

};    

//具体的部门，人力资源部    

class HRDepartment :public Company      

{    

public:    
HRDepartment(string name):Company(name)  
{}  

virtual ~HRDepartment()  
{}  

//只需显示，无限添加函数，因为已是叶结点    
virtual void Display(int depth)  
{  
for(int i = 0; i < depth; i++)    
{  
cout<<"-";  
}  
cout<< m_name << endl;    
}    

};  

//////////////////////////////////////////////////////////////////////////  

//测试代码  

int main()  

{  
Company *root = new ConcreteCompany("总公司");    
Company *leaf1=new FinanceDepartment("财务部");    
Company *leaf2=new HRDepartment("人力资源部");    
root->Add(leaf1);    
root->Add(leaf2);    

//华东分公司  
Company *mid1 = new ConcreteCompany("华东分公司");    
Company *leaf3=new FinanceDepartment("华东分公司财务部");    
Company *leaf4=new HRDepartment("华东分公司人力资源部");    
mid1->Add(leaf3);  
mid1->Add(leaf4);  
root->Add(mid1);  

//南京办事处  
Company *mid2=new ConcreteCompany("南京办事处");    
FinanceDepartment *leaf5=new FinanceDepartment("南京办事处财务部");    
HRDepartment *leaf6=new HRDepartment("南京办事处人力资源部");    
mid2->Add(leaf5);  
mid2->Add(leaf6);  
root->Add(mid2);  

//杭州办事处  
Company *mid3=new ConcreteCompany("杭州办事处");    
FinanceDepartment *leaf7=new FinanceDepartment("杭州办事处财务部");    
HRDepartment *leaf8=new HRDepartment("杭州办事处人力资源部");    
mid3->Add(leaf7);    
mid3->Add(leaf8);    
mid2->Add(mid3);  

root->Display(0);  

delete leaf1;  
delete leaf2;    
delete leaf3;  
delete leaf4;    
delete leaf5;  
delete leaf6;  
delete leaf7;  
delete leaf8;    
delete mid1;  
delete mid2;    
delete root;    
getchar();
return 0;  

}  

**二. 说明**

1. 上面公司的结构图其实就是整体与部分的关系，而且的话整体与部分可以一致对待，因为有很多相似之处嘛。

2. 这棵树有两种几能，要么是棵叶，要么是子棵。

其实这种模式就是组合模式。

**三. 组合模式**

定义：**将对象组合成树形结构以表示“部分-整体”的层次结构。组合使得用户对单个对象和组合对象的使用具有一致性**。

**要注意两点：**

1. “树形”，必须是一种层次结构，有可以向下延伸的分枝，也有不变的树叶。

2. "一致性"，也就是要具有很多相似性。

**结构图如下：**

![](https://img-my.csdn.net/uploads/201205/10/1336623070_2028.jpg)

component：主要是定义统一的接口，说白了也就是提取出相似性。

composite：定义分枝节点，也就是子树。

leaf：定义叶节点，叶节点是没有子节点的。

