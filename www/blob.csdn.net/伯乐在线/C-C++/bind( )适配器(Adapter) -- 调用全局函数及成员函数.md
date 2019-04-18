# bind( )适配器(Adapter) -- 调用全局函数及成员函数 - 文章 - 伯乐在线
原文出处： [难免有错_，2016-05-08](http://blog.csdn.net/y396397735/article/details/51345677)
#### 调用全局函数
调用全局函数程序实例：


```
#include <iostream>
#include <algorithm>
#include <functional>
#include <locale>
#include <string>
using namespace std;
using namespace std::placeholders;
char my_toupper(char c)
{
    locale loc;
    return std::use_facet<std::ctype<char> >(loc).toupper(c);
}
int main()
{
    string s("Internationalization");
    string sub("Nation");
    string::iterator pos;
    pos = search
        (
        s.begin(), s.end(), 
        sub.begin(), sub.end(), 
        bind
        (
        equal_to<char>(), 
        bind(my_toupper, _1), 
        bind(my_toupper, _2)
        )
        );
    if (pos != s.end())
    {
        cout << "\"" << sub << "\" is part of\"" << s << "\""
            << endl;
    }
    system("pause");
}
```
运行结果：
“Nation” is part of”Internationalization”
请按任意键继续. . .
程序分析：
本例采用search()算法检验sub是否为s的一个子字符串，大小写不计。有了以下：


```
bind(equal_to<char>(),bind(my_toupper, _1),bind(my_toupper, _2))
```
便是建立一个function object 并相当于调用：


```
my_toupper(arg1) == my_toupper(arg2);
```
注意：
bind( )内部会复制被传入的实参。
若要改变这种行为，让function object使用一个引用(reference)指向被传入的实参，可利用ref()或cref()：
例如：


```
void incr(int& i)
{
    ++i;
}
int i =0;
bind(incr, i)();      //仅仅一个拷贝
bind(incr, ref(i))(); //引用传递
```
#### 调用成员函数
以下程序示范bind()如何被用来调用成员函数


```
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
using namespace std::placeholders;
class Person
{
private:
    string name_;
public:
    Person(const string& n)
        : name_(n)
    {}
    void print() const
    {
        cout << name_ << endl;
    }
    void print2(const string& prefix) const
    {
        cout << prefix << name_ << endl;
    }
};
int main()
{
    vector<Person> coll =
    {
        Person("csu"),
        Person("csru"),
        Person("csiu")
    };
    //每个person对象调用成员函数
    for_each(coll.begin(), coll.end(), bind(&Person::print, _1));
    cout << endl;
    for_each(coll.begin(), coll.end(), bind(&Person::print2, _1, "Person: "));
    cout << endl;
    bind(&Person::print2, _1, "This is : ")(Person("铁道学院"));
    system("pause");
}
```


```
/*
运行结果
csu
csru
csiu
Person: csu
Person: csru
Person: csiu
This is : 铁道学院
请按任意键继续. . .
*/
```
程序分析：
程序中的`bind(&Person::print, _1)`定义一个function object，其内针对传入的Person调用param1.print()，
也就是说，由于第一实参是个成员函数，下一个参数将定义“用以调用成员函数”的对象。
其他任何实参都会被传递给该成员函数。这意味着：


```
bind(&Person::print2, _1, "Person: ")
```
定义出一个function object，其内针对传入Person调用`param1.print2("Person：")`。
#### Lambda实现调用全局函数及类成员函数


```
//全局函数
#include <iostream>
#include <algorithm>
#include <locale>
#include <string>
using namespace std;
char my_toupper(char c)
{
    std::locale loc;
    return std::use_facet<std::ctype<char>>(loc).toupper(c);
}
int main()
{
    string s("Internationalizition");
    string sub("Nation");
    string::iterator pos;
    pos = search(s.begin(), s.end(),
        sub.begin(), sub.end(),
        [](char c1, char c2){   
        return my_toupper(c1) == my_toupper(c2);
        }
    );
    if (pos != s.end())
    {
        cout << sub << " is part of " << s << endl;
    }
    system("pause");
}
/*
Nation is part of Internationalizition
请按任意键继续. . .
*/
```


```
//类成员函数
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
using namespace std::placeholders;
class Person
{
private:
    string name_;
public:
    Person(const string& n)
        : name_(n)
    {}
    void print() const
    {
        cout << name_ << endl;
    }
    void print2(const string& str) const
    {
        cout << str << name_ << endl;
    }
};
int main()
{
    vector<Person> coll =
    {
        Person("csu"),
        Person("csru"),
        Person("csiu")
    };
    for_each(coll.begin(), coll.end(), 
        [](const Person& p){
        p.print();
    });
    cout << endl;
    for_each(coll.begin(), coll.end(),
        [](const Person& p){
        p.print2("Person: ");
        });
    system("pause");
}
/*
csu
csru
csiu
Person: csu
Person: csru
Person: csiu
请按任意键继续. . .
*/
```
