
# C++实践参考：警察与厨师 - 迂者-贺利坚的专栏 - CSDN博客

2016年04月27日 16:37:40[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：3303


【项目-警察和厨师】
（1）根据下面的类图，定义各个类：
![这里写图片描述](https://img-blog.csdn.net/20160427160652698)[ ](https://img-blog.csdn.net/20160427160652698)
要求：
各个成员函数，只要输出相关的信息即可，暂不深究其业务功能
请为各个类增加构造函数
在实现中，可以增加需要的其他函数
自行编制main函数，完成初步的测试
[参考解答1]
```python
#include <iostream>
```
```python
using
```
```python
namespace std;
class Person
{
```
```python
public
```
```python
:
```
```python
Person
```
```python
(
```
```python
int
```
```python
,
```
```python
string
```
```python
);
```
```python
void
```
```python
action();
```
```python
string
```
```python
getName()
    {
```
```python
return
```
```python
name;
    }
```
```python
private
```
```python
:
```
```python
int
```
```python
age;
```
```python
string
```
```python
name;
};
Person::Person(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n):age(a), name(n) {}
```
```python
void
```
```python
Person::action()
{
    cout<<name<<
```
```python
" do some action"
```
```python
<<endl;
}
class Police:
```
```python
public
```
```python
Person
{
```
```python
public
```
```python
:
```
```python
Police
```
```python
(
```
```python
int
```
```python
,
```
```python
string
```
```python
,
```
```python
int
```
```python
);
```
```python
void
```
```python
arrest(Person);
```
```python
private
```
```python
:
```
```python
int
```
```python
level;
```
```python
//级别
```
```python
};
Police::Police(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n,
```
```python
int
```
```python
l):Person(a,n),level(l) {}
```
```python
void
```
```python
Police::arrest(Person p)
{
    cout<<
```
```python
" Police "
```
```python
<<getName()<<
```
```python
" arrest "
```
```python
<<p.getName()<<endl;
}
class Cook:
```
```python
public
```
```python
Person
{
```
```python
public
```
```python
:
```
```python
Cook
```
```python
(
```
```python
int
```
```python
,
```
```python
string
```
```python
,
```
```python
double
```
```python
);
```
```python
void
```
```python
getCake(
```
```python
int
```
```python
);
```
```python
private
```
```python
:
```
```python
double
```
```python
salary;
```
```python
//薪水
```
```python
};
Cook::Cook(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n,
```
```python
double
```
```python
s):Person(a,n),salary(s) {}
```
```python
void
```
```python
Cook::getCake(
```
```python
int
```
```python
n)
{
    cout<<
```
```python
" Cook "
```
```python
<<getName()<<
```
```python
" gave me "
```
```python
<<n<<
```
```python
" cakes."
```
```python
<<endl;
}
```
```python
int
```
```python
main()
{
    Person tom(
```
```python
120
```
```python
,
```
```python
"Tom"
```
```python
);
    Police jack(
```
```python
30
```
```python
,
```
```python
"Jack"
```
```python
,
```
```python
2
```
```python
);
    Cook john(
```
```python
24
```
```python
,
```
```python
"John"
```
```python
,
```
```python
5000
```
```python
);
    jack.arrest(tom);
    john.getCake(
```
```python
4
```
```python
);
```
```python
return
```
```python
0
```
```python
;
}
```
（2）下面的类图，为Polic类和Cook类增加了对象成员，请扩充代码，完成上述各项要求
![这里写图片描述](https://img-blog.csdn.net/20160427160837625)[ ](https://img-blog.csdn.net/20160427160837625)
[参考解答1]
[

](https://img-blog.csdn.net/20160427160837625)
```python
#include <iostream>
```
```python
using
```
```python
namespace std;
class Person
{
```
```python
public
```
```python
:
```
```python
Person
```
```python
(
```
```python
int
```
```python
,
```
```python
string
```
```python
);
```
```python
void
```
```python
action();
```
```python
string
```
```python
getName()
    {
```
```python
return
```
```python
name;
    }
```
```python
private
```
```python
:
```
```python
int
```
```python
age;
```
```python
string
```
```python
name;
};
Person::Person(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n):age(a), name(n) {}
```
```python
void
```
```python
Person::action()
{
    cout<<name<<
```
```python
" do some action"
```
```python
<<endl;
}
class Police:
```
```python
public
```
```python
Person
{
```
```python
public
```
```python
:
```
```python
Police
```
```python
(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n,
```
```python
int
```
```python
l,
```
```python
int
```
```python
la,
```
```python
string
```
```python
ln);
```
```python
void
```
```python
arrest(Person);
```
```python
void
```
```python
show();
```
```python
private
```
```python
:
```
```python
int
```
```python
level;
```
```python
//级别
```
```python
Person leader;
```
```python
//领导
```
```python
};
Police::Police(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n,
```
```python
int
```
```python
l,
```
```python
int
```
```python
la,
```
```python
string
```
```python
ln):Person(a,n),level(l),leader(la,ln) {}
```
```python
void
```
```python
Police::arrest(Person p)
{
    cout<<
```
```python
"Police "
```
```python
<<getName()<<
```
```python
" arrest "
```
```python
<<p.getName()<<endl;
}
```
```python
void
```
```python
Police::show()
{
    cout<<
```
```python
"Police "
```
```python
<<getName()<<
```
```python
", leader is "
```
```python
<<leader.getName()<<endl;
}
class Cook:
```
```python
public
```
```python
Person
{
```
```python
public
```
```python
:
```
```python
Cook
```
```python
(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n,
```
```python
double
```
```python
s,
```
```python
int
```
```python
pa,
```
```python
string
```
```python
pn,
```
```python
int
```
```python
pl,
```
```python
int
```
```python
pla,
```
```python
string
```
```python
pln);
```
```python
void
```
```python
getCake(
```
```python
int
```
```python
);
```
```python
void
```
```python
show();
```
```python
private
```
```python
:
```
```python
double
```
```python
salary;
```
```python
//薪水
```
```python
Police protector;
```
```python
//厨师小店的片区警察
```
```python
};
Cook::Cook(
```
```python
int
```
```python
a,
```
```python
string
```
```python
n,
```
```python
double
```
```python
s,
```
```python
int
```
```python
pa,
```
```python
string
```
```python
pn,
```
```python
int
```
```python
pl,
```
```python
int
```
```python
pla,
```
```python
string
```
```python
pln):
    Person(a,n),salary(s),protector(pa,pn,pl,pla,pln) {}
```
```python
void
```
```python
Cook::getCake(
```
```python
int
```
```python
n)
{
    cout<<
```
```python
"Cook "
```
```python
<<getName()<<
```
```python
" gave me "
```
```python
<<n<<
```
```python
" cakes."
```
```python
<<endl;
}
```
```python
void
```
```python
Cook::show()
{
    cout<<
```
```python
"Cook "
```
```python
<<getName()<<
```
```python
" is protected by Police "
```
```python
<<protector.getName()<<endl;
}
```
```python
int
```
```python
main()
{
    Person tom(
```
```python
120
```
```python
,
```
```python
"Tom"
```
```python
);
    Police jack(
```
```python
30
```
```python
,
```
```python
"Jack"
```
```python
,
```
```python
2
```
```python
,
```
```python
43
```
```python
,
```
```python
"Jerry"
```
```python
);
    Cook john(
```
```python
24
```
```python
,
```
```python
"John"
```
```python
,
```
```python
5000
```
```python
,
```
```python
30
```
```python
,
```
```python
"Jack"
```
```python
,
```
```python
2
```
```python
,
```
```python
43
```
```python
,
```
```python
"Jerry"
```
```python
);
    jack.show();
    john.show();
```
```python
return
```
```python
0
```
```python
;
}
```
[
](https://img-blog.csdn.net/20160427160837625)
> 评价：

> - 这些代码是完成是题目的要求，但是，并不好。

> - 每个构造函数带上一长串的参数，难写，难看，这本身就是质量问题。

> - 这种写法，也根本未体现对象的“封装”——都是一串散乱的基本类型数据在工作。

> - 我们希望看到jack警察的上司就是一个人，john厨师的保卫者，就是一个警察。

> - 需要做的是，利用对象作为构造函数的参数，使结构清晰。

> - 当然，这时需要增加相关的复制构造函数了。

> [参考解答2]

> \#include <iostream>
> using
> namespace std;
class Person
{
> public
> :
> Person
> (
> int
> ,
> string
> );
> void
> action();
> string
> getName()
    {
> return
> name;
    }
> private
> :
> int
> age;
> string
> name;
};
Person::Person(
> int
> a,
> string
> n):age(a), name(n) {}
> void
> Person::action()
{
    cout<<name<<
> " do some action"
> <<endl;
}
class Police:
> public
> Person
{
> public
> :
> Police
> (
> int
> a,
> string
> n,
> int
> l, Person);
> void
> arrest(Person);
> void
> show();
> private
> :
> int
> level;
> //级别
> Person leader;
> //领导
> };
Police::Police(
> int
> a,
> string
> n,
> int
> l, Person p):Person(a,n),level(l),leader(p) {}
> void
> Police::arrest(Person p)
{
    cout<<
> "Police "
> <<getName()<<
> " arrest "
> <<p.getName()<<endl;
}
> void
> Police::show()
{
    cout<<
> "Police "
> <<getName()<<
> ", leader is "
> <<leader.getName()<<endl;
}
class Cook:
> public
> Person
{
> public
> :
> Cook
> (
> int
> a,
> string
> n,
> double
> s,Police p);
> void
> getCake(
> int
> );
> void
> show();
> private
> :
> double
> salary;
> //薪水
> Police protector;
> //厨师小店的片区警察
> };
Cook::Cook(
> int
> a,
> string
> n,
> double
> s,Police p):
    Person(a,n),salary(s),protector(p) {}
> void
> Cook::getCake(
> int
> n)
{
    cout<<
> "Cook "
> <<getName()<<
> " gave me "
> <<n<<
> " cakes."
> <<endl;
}
> void
> Cook::show()
{
    cout<<
> "Cook "
> <<getName()<<
> " is protected by Police "
> <<protector.getName()<<endl;
}
> int
> main()
{
    Person jerry(
> 43
> ,
> "Jerry"
> );
    Police jack(
> 30
> ,
> "Jack"
> ,
> 2
> ,jerry);
    Cook john(
> 24
> ,
> "John"
> ,
> 5000
> ,jack);
    jack.show();
    john.show();
> return
> 0
> ;
}
> 评论：

> 这样做，是不是在逻辑上很清楚了？

> Person、Police类中该定义复制构造函数，在这里没有写，用其默认复制构造函数了。相关类中没有定义指针型成员，不必要深复制，所以，可以使用默认复制构造函数。


