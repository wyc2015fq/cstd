# C++链表操作总结和常见链表操作 - fanyun的博客 - CSDN博客
2017年02月26日 21:13:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：5401
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
一、链表的定义
链表是一种动态数据结构，他的特点是用一组任意的存储单元（可以是连续的，也可以是不连续的）存放数据元素。链表中每一个元素成为“结点”，每一个结点都是由数据域和指针域组成的，每个结点中的指针域指向下一个结点。Head是“头指针”，表示链表的开始，用来指向第一个结点，而最后一个指针的指针域为NULL(空地址)，表示链表的结束。可以看出链表结构必须利用指针才能实现，即一个结点中必须包含一个指针变量，用来存放下一个结点的地址。实际上，链表中的每个结点可以用若干个数据和若干个指针。结点中只有一个指针的链表称为单链表，这是最简单的链表结构。再c++中实现一个单链表结构比较简单。
例如，可定义单链表结构的最简单形式如下：
struct Node
{
int Data;
Node*next;
};
这里用到了结构体类型。其中，*next是指针域，用来指向该结点的下一个结点；Data是一个整形变量，用来存放结点中的数据。当然，Data可以是任何数据类型，包括结构体类型或类类型。在此基础上，我们在定义一个链表类list，其中包含链表结点的插入，删除，输出等功能的成员函数。
下面简单实现下结构体链表，代码如下：
```cpp
#include <iostream>
using namespace std;
enum operation{create_List=1,print_List,insert_Node,delete_Node,delete_List,quit};//枚举类型，用于菜单选择结果
   
struct  node  //结点结构
{ int data ; 
  node * next; 
};  
operation  Menu(); //菜单函数
node * CreateList( );  //建立链表函数声明
void  PrintList( node *); //输出链表中结点信息函数声明
node * InsertNode(node *,node *); //在链表中插入结点函数声明
node * DeleteNode(node *,int); //在链表中删除结点函数声明 
node * deleteList(node *head); //删除整个链表
void Create();  //对应操作菜单--创建链表的操作
void Print( );  //对应操作菜单--遍历链表的操作
void Insert( ); //对应操作菜单--插入链表结点的操作
void Delete( ); //对应操作菜单--删除链表结点的操作
void DeleteAll(); //对应操作菜单--删除整个链表的操作
int n=0; //全局整型变量存放链表中结点个数
node * head=NULL ;  //全局指针变量存放链表头结点地址-头指针
int main()
{   
  operation menu_choice;  //存放菜单选择项
  do   //循环现实直到用户退出程序
  {
   menu_choice=Menu(); //菜单显示及用户选择
   switch(menu_choice) //用户选择功能匹配
   {
      case create_List: cout<<"1 创建链表"<<endl<<endl;
		                Create( );
					    break;
	  case  print_List: cout<<"2 遍历链表"<<endl<<endl;
                       Print();
					   break;
      case  insert_Node: cout<<"3 插入链表结点"<<endl<<endl;
                       Insert();
					    break;
	  case  delete_Node: cout<<"4 删除链表结点"<<endl<<endl;
                       Delete();
					    break;
      case  delete_List: cout<<"5 删除整个链表"<<endl<<endl;
                       DeleteAll();
					    break;
	  case quit : 
	  default:   cout<<"退出链表操作，结束程序"<<endl;
		        return 0;
   }   
   
  }while(menu_choice!=quit);  
  return 0;
}
/*创建链表*/
node * CreateList( )  //建立链表函数
{ node * s, * p ; // s指向新结点，p指向链表中最后的结点 
  s = new node ;   //动态建立第一个新结点
  cout<<"请输入一个整数值作为新结点的数据信息,输入0时建立链表结束"<<endl;
  cout<<"第"<<n+1<<"个结点"<<endl;
  cin >> s->data ;  //输入新结点数据
  head = NULL ;   //头指针初始值为NULL
  if( s->data==0)  //第一个结点数据就为0，建立一个空链表
  {
	  cout<<"您建立的空链表"<<endl;
      delete  s ;  //释放数据为0的结点
  }
  else //建立非空链表
  {
	  while ( s->data != 0 )  //通过判断新结点数据来进行循环
	  { if ( head == NULL )   head = s ; //头指针赋值
		 else  p->next = s ;  //将新结点插入已有链表的最后
		p = s ; // p指向链表中最后的结点 
		n=n+1;//结点个数增1
		s = new node ; //动态建立一个新结点
		cout<<"请输入一个整数值作为新结点的数据信息,输入0时建立链表结束"<<endl;
		cout<<"第"<<n+1<<"个结点"<<endl;
		cin >> s->data ; //输入新结点数据
	  }
	  p -> next = NULL ; //设置链表尾部为空
	  delete  s ;  //释放数据为0的结点
	  cout<<endl<<"链表建立完成...";
	  cout<<"建立的链表中共有"<<n<<"个节点"<<endl<<endl;
  }
  return ( head ) ; //返回头指针 
}
  
/*遍历链表*/
void  PrintList( node * head)  //输出链表中结点信息函数,链表遍历
{    node *p=head;
     int i=1;
	 cout<<endl<<"遍历链表..."<<endl;
    if (head!=NULL)  //如果链表非空，即链表中有结点
        do             //循环输出接点数据，直到移动到链表尾，即最后一个结点
		{     cout<<"第"<<i++<<"个结点数据为:"<<p->data<<endl; 
               p=p->next;				
        }while(p!=NULL) ;
	else
	{
	  cout<<"链表是空链表!"<<endl;
	}
	 cout<<"链表中共有"<<n<<"个节点"<<endl;
}
/*插入结点*/  
node * InsertNode(node *head,node * s)  //插入结点的函数，head为链表头指针，s指向要插入的新结点
{node *p,*q;                         
 p=head;   //使p指向链表中的第一个结点      
 if(head==NULL)      //原来的链表是空表
 { head=s;           //使head指向的新结点作为头结点
   s->next=NULL;     
 }          
 else  //原来的链表不是空表
 {while((s->data>p->data) && (p->next!=NULL)) //用循环定位要插入的结点位置p，使s插入到p之前的位置
	{q=p;  //q记录下当前的p，即q指向p的前一个结点
	 p=p->next;   //p后移一个结点
	}    
  if(s->data<=p->data)  //要插入的结点数据比最后一个结点数据小
  {	  if(head==p) //判断是否插入链表中的第一个结点之前
	  { head=s;  //插到原来第一个结点之前      
		 s->next=p;
	 } 	  
	  else //插到q指向的结点之后，p指向的结点之前
	  { q->next=s;
		s->next=p;         
	  }  	  
  } 
  else   //要插入的结点数据比最后一个结点数据还大
  {  p->next=s;    // 插到链表最后的结点之后，作为链表的尾结点
     s->next=NULL;
  } 
 } 
   n=n+1;          //结点数加１ 
   cout<<"成功完成一个新结点插入..."<<endl;
   return (head);
}
               
/*删除结点*/
node *DeleteNode(node *head,int delData)   //删除数据为delDate的结点的函数
{node *p,*q; 
 p=head;          //使p指向第一个结点
 if (head==NULL)     //是空表
 { cout<<"该链表是空链表，不能进行结点删除!"<<endl; 
   return(head);
 }
 //先找到要删除的结点                       
 while(delData!=p->data && p->next!=NULL) //p指向的不是所要找的结点且后面还有结点
 { q=p;  //q用来记录p前一个结点
   p=p->next;
 }         //p后移一个结点
  if(delData==p->data)      //找到了要删除的结点
  { if(p==head)  //如果要删除的是头结点
       head=p->next;   //若p指向的是首结点，把第二个结点地址赋予head
    else q->next=p->next;    //否则将下一结点地址赋给前一结点地址
    cout<<"成功删除数据为"<<delData<<"的结点"<<endl;
    n=n-1;
 }
 else 
	 cout<<"要删除的数据为"<<delData<<"的结点在链表中没有找到"<<endl;     //找不到该结点
 return(head);
}
/*删除整个链表*/
node * deleteList(node *head) //删除整个链表
{ 
  node *p,*s;
  p=head;
  if(head==NULL)
	  cout<<"链表本身就为空链表";
  else
  {
	  while(p->next!=NULL)
	  {
	     s=p;
		 p=p->next;
		 delete s;
		 n--;
	  }  
	  delete p;
	  n--;
      head=NULL;
  }
  cout<<"整个链表删除成功!"<<endl;
  return head;
}
/*菜单函数*/
operation  Menu()
{  int choice;
   cout<<endl<<endl;
   cout<<"链表操作菜单"<<endl;
   cout<<"1 创建链表"<<endl;
   cout<<"2 遍历链表"<<endl;
   cout<<"3 插入链表结点"<<endl;
   cout<<"4 删除链表结点"<<endl;
   cout<<"5 删除整个链表"<<endl;
   cout<<"6 退出"<<endl;
   cout<<endl<<endl<<"请输入功能序号";
   cin>>choice;
   return operation(choice);
}
/*对应操作菜单--创建链表的操作*/
void Create()
{
    if(head==NULL) //如果链表中已有结点，不允许重新建立
	{
	  head=CreateList( ); 
	}
	else
	{
	  cout<<"已创建过链表，不允许再次创建"<<endl;
	  cout<<"如果想重新创建，先删除原先链表"<<endl;	  
	}
}
/*对应操作菜单--遍历链表的操作*/
void Print( )
{
   PrintList(head);
}
/*对应操作菜单--插入链表结点的操作*/
void Insert( )
{  
  char IsGo;  //是否继续操作标志
  IsGo='y';
  cout<<endl<<"开始进行结点插入操作"<<endl;  
  node *stu;   
  while(IsGo=='y'||IsGo=='Y')  
  {   stu=new node;    //创建要插入的新结点
      cout<<endl<<"输入要插入的新结点数据:"; 
      cin>>stu->data;      //输入要插入的新结点数据 	 
	  head=InsertNode(head,stu);    //调用插入函数，返回链表头指针
	  cout<<"是否继续插入新结点? (继续插入请按y或Y,退出请按其它键)";
	  cin>>IsGo;    
  }
  cout<<endl<<"结点插入操作结束"<<endl;
}
/*对应操作菜单--删除链表结点的操作*/
void Delete( )
{
   char IsGo;  //是否继续操作标志
   int del_num;	//要删除的结点的数据
   IsGo='y';
   cout<<endl<<"开始进行结点插入操作"<<endl;  
   while(IsGo=='y'||IsGo=='Y')
   {
	   cout<<endl<<"输入要删除的节点的数据:";  //输入要插入的结点
	   cin>>del_num;  //输入要删除的结点的数据	
	   head=DeleteNode(head,del_num);    //删除后链表的头地址
       cout<<"是否继续删除结点? (继续插入请按y或Y,退出请按其它键)";
	   cin>>IsGo;    
   }
     cout<<endl<<"结点删除操作结束"<<endl;
}
/*对应操作菜单--删除整个链表的操作*/
void DeleteAll()
{
  head=deleteList(head);
}
```
单链表类的实现，代码如下：
```cpp
/////////////////////////////////////////////////////////////////
//日期：2014-11-03
//作者：Davis
//功能：单链表的定义与操作实现
/////////////////////////////////////////////////////////////////
#pragma once
template<class T>
struct Node
{
    T data;     //数据域，存放表元素
    Node *next; //指针域。指向下一个结点
};
template<class T>
class linkList
{
private:
    Node<T> *Head; //链表头指针
public:
    //构造函数、创建空链表
    linkList(void)
    {
      Head = new Node<T>;
      Head->next = NULL;
    }
    //析构函数、删除空链表
    ~linkList(void)
    {
      Node<T> *p;
      while(Head)
      {
        p = Head;//从头节点开始，依次释放结点
        Head = Head->next;
        delete p;
      }
      Head = NULL;//头结点指向空
    }
    //创建具有n个元素的线性链表
    void createList(int n)
    {                 //尾插法（正序）创建具有n个元素的线性表
      Node<T> *p,*s;  //设置工作指针，p指向尾结点
      p=Head;
      cout<<"请依次输入"<<n<<"个元素值"<<endl;
      for(int i=1;i<=n;i++)
      {
        s = new Node<T>;   //新建元素结点
        cin>>s->data;      //输入新建数据元素值
        s->next = p->next; //新结点链入表尾
        p->next = s;
        p = s;             //p是工作指针
      }
    }
    //在表中第i个位置插入元素
    void Insert(int i,T e)
    {
      int j = 0;
      Node<T> *p;
      p = Head;         //工作指针p指向头结点
      while(p && j<i-1) //查找第i-1个结点
      {
        p = p->next;
        j++;
      }
      if(!p || j>i-1)throw"位置异常";
      else
      {
        Node<T> *s;
        s = new Node<T>;  //创建新节点
        s->data = e;
        s->next = p->next;//结点s链到p结点之后
        p->next = s;
      }
    }
    T Delete(int i)//删除表中第i个元素
    {
          T x;
        Node<T> *p,*q;
        p = Head;  //从头结点开始查找
        int j = 0; //计数器初始化
        while(p->next && j<i-1)//p定位到删除结点的前驱
        {
          p = p->next;
          j++;
        }
        if(!p->next || j>i-1)throw"位置异常";//删除位置不合理
        else                                 //删除位置合理
        {
          q = p->next;      //暂存删除结点位置
          p->next = q->next;//从链表中摘除删除结点
          x = q->data;      //取删除数据元素的值
          delete q;         //释放删除点
          return x;         //返回删除元素的值
        }
     }
    //获取第i个元素的值
    T getElem(int i)
    {
      Node<T> *p;     //设置工作指针
      p = Head->next; //从首结点开始
      int j = 1;      //计数器初始化
      while(p && j<i) //定位到第i个元素结点
      {
          p = p->next;
          j++;
      }
      if(!p || j>i)
      {
          throw "位置异常";
      }
      else   //位置合理
      {
        return p->data;
      }
    }
    //在链表中查找值为e的元素
    int Locate(T e)
    {
      int j = 1;
      Node<T> *p;
      p = Head->next;
      while(p && p->data != e)
      {
        p = p->next;
        j++;
      }
      if(!p )//未找到，范围0
      {
          return 0;
      }
      else  //找到，返回位序
      {
          return j;
      }
    }
    //返回元素e的前驱
    T prior(T e)
    {
      Node<T> *p,*q;
      p = Head;
      q = p->next;
      while(q && q->data != e)
      {
        p = q;
        q = q->next;
      }
      if(p == Head)
      {
        throw "首元素，无前驱";
      }
      else if(!q)
      {
        throw "元素不存在";
      }
      else
      {
        return p->data;
      }
    }
    
    //测表空
    int Empty()
    {
      if(Head->next == NULL)
          return 1; //空表返回1
      else
          return 0; //非空表，返回0
    }
    //测表长
    int Length()
    {
      int len = 0; //计数器初始化
      Node<T> *p;  //设置头指针
      p=Head;      //指向头指针
      while(p->next)
      {
        len++;
        p = p->next;
      }
      return len;  //返回表长
    }
    //输出表元素
    void listDisplay()
    {
      Node<T> *p;
      p = Head->next;
      int i = 1;
      while(p)
      {
        cout<<i<<"\t";
        cout<<p->data<<endl;
        p = p->next;
        i++;
      }
    }
};
/////////////////////////////////////////////////////////////////
//功能：单链表类的测试
//////////////////////////////////////////////////////////////////
// linkList_Test.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "linkList.h"
#include "process.h"//exit()
#include<iostream>
using namespace std;
char pause;
typedef int T;
int _tmain(int argc, _TCHAR* argv[])
{
    int i;
    T e,prior_e;
    linkList<int>L;   //建立整形空链表
    system("cls");    //执行系统命令，清屏
    int choice;
    do
    {
      cout<<"1-创建链表\n";
      cout<<"2-在链表第i个位置插入元素\n";
      cout<<"3-删除链表中第i个位置的元素\n";
      cout<<"4-返回第i个元素的值\n";
      cout<<"5-元素定位\n";
      cout<<"6-按值求前驱\n";
      cout<<"7-测表空\n";
      cout<<"8-测表长\n";
      cout<<"9-显示链表\n";
      cout<<"10-退出\n";
      cout<<"Enter choice:";
      cin>>choice;
      switch(choice)
      {
      case 1://创建链表
             cout<<"请输入要创建的链表中元素的个数:";
             cin>>i;
             cout<<endl;
             L.createList(i);         
             break;
      case 2://元素插入
           cout<<"请输入插入的位置";
           cin>>i;
           cout<<endl;
           cout<<"请输入插入元素的值:";
           cin>>e;
           cout<<endl;
           try
           {
               L.Insert(i,e);
           }
           catch(char* err)
           {
             cout<<err<<endl;
           }
           break;
      case 3://元素删除
          cout<<"请输入删除位置";
          cin>>i;
          cout<<endl;
          try
          {
              e = L.Delete(i);
              cout<<"被删除元素为："<<e<<endl;
          }
          catch(char* err)
          {
             cout<<err<<endl;
          }
          cin.get(pause);
          system("pause");
          break;
      case 4://返回第i个元素值
          cout<<"请输入要查询的元素位置:";
          cin>>i;
          try
          {
              e = L.getElem(i);
              cout<<"第"<<i<<"个元素值为:"<<e<<endl;
          }
          catch(char* err)
          {
            cout<<err<<endl;
          }
          cin.get(pause);
          system("pause");
          break;
      case 5://按值进行元素查询
          cout<<"请输入要查询的元素值：";
          cin>>e;
          i = L.Locate(e);
          cout<<"查询元素"<<e<<"在链表中的位置为："<<i<<endl;
          cin.get(pause);
          system("pause");
          break;
      case  6://求元素前驱
          cout<<"请输入要求前驱元素的值：";
          cin>>e;
          try
          {
              prior_e = L.prior(e);
              cout<<"元素"<<e<<"的前驱值为:"<<prior_e<<endl;
          }
          catch(char *err)
          {
            cout<<err<<endl;
          }
          cin.get(pause);
          system("pause");
          break;
      case 7://测表空
          i = L.Empty();
          if(i)
          {
            cout<<"表空"<<endl;
          }
          else
          {
            cout<<"表不空"<<endl;
          }
          cin.get(pause);
          system("pause");
          break;
      case 8://测表长
          cout<<"链表长度为:"<<L.Length()<<endl;
          cin.get(pause);
          system("pause");
          break;
      case 9://遍历输出表
          L.listDisplay();
          cout<<endl;
          cin.get(pause);
          system("pause");
          break;
      case 10://退出
          break;
      default://非法选择
          cout<<"Invalid choice";
          break;
      }       
    }while(choice != 10);
    return 0;
}
```
