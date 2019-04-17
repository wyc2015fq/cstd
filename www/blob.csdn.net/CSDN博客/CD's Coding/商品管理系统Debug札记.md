# 商品管理系统Debug札记 - CD's Coding - CSDN博客





2013年11月17日 15:06:01[糖果天王](https://me.csdn.net/okcd00)阅读数：733标签：[管理																[debug																[指针																[类																[bug](https://so.csdn.net/so/search/s.do?q=bug&t=blog)
个人分类：[作业](https://blog.csdn.net/okcd00/article/category/1727537)







```cpp
//Debug的都快疯了……要不要那么难Debug，记录一些心得以及BUG以备忘
```

当使用指针指向另一个类的时候，若想使用那个类里的函数，需要 s->Func()的形式：



```cpp
ProductNode *s=new ProductNode; //NEW 一个新的结点 
   sin>>tmpNO>>tmpName>>tmpBrand>>tmpPrice>>tmpQuantity;
   s->setNO(tmpNO);
   s->setName(tmpName);
   s->setBrand(tmpBrand);
   s->setPrice(tmpPrice);
   s->setQuantity(tmpQuantity);
   InitInsert(s);
```


一个常用的登陆密码验证函数：





```cpp
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
```](https://so.csdn.net/so/search/s.do?q=类&t=blog)](https://so.csdn.net/so/search/s.do?q=指针&t=blog)](https://so.csdn.net/so/search/s.do?q=debug&t=blog)](https://so.csdn.net/so/search/s.do?q=管理&t=blog)




