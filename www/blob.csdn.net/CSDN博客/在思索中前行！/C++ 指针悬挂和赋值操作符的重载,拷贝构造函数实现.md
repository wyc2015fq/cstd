# C++ 指针悬挂和赋值操作符的重载,拷贝构造函数实现 - 在思索中前行！ - CSDN博客





2015年03月30日 22:21:24[_Tham](https://me.csdn.net/txl16211)阅读数：1010








## 指针悬挂：

问题：使用new申请的内存内存空间无法访问，也无法释放。

原因：直接对指向new申请的存储空间的指针变量进行赋值修改

后果：失去了原来的地址，原来的空间无法访问也无法释放，造成内存泄漏

            还可能造成同一个内存释放两次

容易引起指针悬挂的方式：对象的初始化和对象间赋值

容易引起指针悬挂的条件：类中含有指针类型的成员时，使用默认的拷贝构造函数和赋值函数都会出现两个指针变量互相赋值，产生指针悬挂的问题。

解决方法：需要重新定义拷贝构造函数和超载赋值运算符

赋值操作符：

作用：两个已经存在的对象间相互赋值，产生两个完全相同的内存拷贝

举例：string a("hello");//调用构造函数

             string b("would");//调用构造函数

             string c=a;//调用拷贝构造函数--风格差，应使用string c(a)

             c=b;  //调用拷贝赋值函数

## 重载赋值运算符：

语法：



```cpp
X& X::operator=(const X & fm)  
    {  
        函数体  
    }
```
注意：




          1、第一个引用的作用（为什么使用返回函数引用）：

               原因：为了实现对象间的连续赋值。

               使用返回函数引用的好处：结果得到的是一个变量，它既可以当左值，也可当右值，且采用赋值时没有引入临时变量，直接从原结果拷贝。

          2、第二个引用的作用：防止调用拷贝构造函数，因为拷贝构造函数也可能引起指针悬挂

          3、const的作用：当参数使用引用时，可能会改变传入的参数，为了避免这样，就使用const

具体代码：

```cpp
class Point  
    {  
    private:  
        char * name;  
    public:  
        Point(char * className)  
        {  
            name = new char[strlen(className)+1];  
            strcpy(name, className);  
        }  
        Point(const Point& p)//深拷贝  
        {  
            name = new char[strlen(p.name)+1];  
            strcpy(name,p.name);  
        }  
        ~Point()  
        {  
            cout<<name<<endl;  
            delete []name;  
        }  
        Point& operator=(const Point&p);  
    };  
    系统自带的等号运算符：  
    Point& Point::operator=(const Point&p)  
    {  
        name=p.name;//造成指针悬挂  
    }  
    重载后的等号运算符：  
    Point& Point::operator=(const Point&p)  
    {        
        delete[]name;//释放原来的        
        name=new char[strlen(p.name)+1];        
        strcpy(name,p.name);          
        return *this;      
    }
```

## 拷贝构造函数

      为什么要引入拷贝构造函数？

  作用：创建一个对象的同时，使用一个已经存在的对象给另一个对象赋值

 具体来说：它将一个已经定义过对象的数据成员 逐一拷贝给 新对象，而产生两个完全相同的内存拷贝

做比较：拷贝构造函数：对象被创建 +  用一个已经存在的对象进行初始化

                       拷贝赋值函数：对象已经存在不用创建 + 用一个已经存在的对象进行初始化

举例：string a("hello");//调用构造函数

                   string b("would");//调用构造函数

                   string c=a;//调用拷贝构造函数--风格差，应使用string c(a)

                   c=b;//调用拷贝赋值函数

       什么时候使用拷贝构造函数？（系统自己调用）

在创建新对象的时候，希望将一个已经存在的对象拷贝给这个新对象，这时系统会自动调用拷贝构造函数

总结：1、拷贝构造函数的参数必须是引用，否则出错。

                   2、执行的语句类似 Coord p=p1; 则会调用拷贝构造函数                

有三种情况：

1）创建一个新类 +  并使用类的一个对象初始化该类的另一个对象

               Coord p2(p1);//用对象p1初始化对象p2

               Coord p3=p1;//用对象p1初始化对象p1

2）函数的形参是类的对象 +  参数使用值传递（参数为引用的时候不调用拷贝构造函数），传参时，会调用拷贝构造函数

```cpp
fun1(Coord p)  
{  
    函数体  
}  
调用语句：  
Coord p1；  
fun1(p1);  
//分析：调用拷贝构造函数 Coord p=p1;  
  
fun1(Coord& p)  
{  
    函数体  
}  
调用语句：  
Coord p1；  
fun1(p1);
```

        //分析：参数表中使用了引用,没有调用拷贝构造函数啊,执行 Coord& p=p1;

3）函数的返回值是对象，函数调用完毕，返回调用者时，会调用拷贝构造函数

```cpp
Coord fun1(Coord& fun)  
    {  
        return fun;  
    }  
      
    调用语句：  
    Coord  c;  
    Coord p=fun(c);
```

//分析：return fun调用两次拷贝构造函数; VC测试，但是VS2005只调用一次，应该是进行了优化




```cpp
Coord& fun1(Coord& fun1)  
{    
    return fun1;  
}  
  
调用语句：  
Coord  c;  
Coord p=fun1(c);
```






// 分析：Coord p=fun1(c)调用一次拷贝构造函数，因为最后使用返回函数引用，return时没有借助临时变量，直接是 Coord p=fun1;

出现这种现象的原因：

1、在使用return返回一个对象时，系统是先申请一个临时对象temp，执行Coord temp=fun1;（调用一次拷贝构造函数）

      之后在执行Coord p=temp；（第二次调用）

2、在使用返回函数引用时，系统不会申请临时对象，只是直接把fun1拷贝给p

       即直接执行Coord p=fun1，而没有引入temp，故少用一次拷贝构造函数

      注意：这时要注意一个常出现的错误，返回的值不能是一个临时变量，常常的解决办法是函数参数使用引用，之后在使用return返回即可

怎么使用拷贝构造函数？

语法：函数名与类名相同，参数为本对象的引用，无返回类型，只有一个

                 类名::类名(类名& 对象名)

                 {拷贝成员}

代码：

```cpp
class Point  
    {  
    public:  
        Point(int xx=0,int yy=0){X=xx; Y=yy;}  
        Point(Point&  p);  
    private:  
        int  X,Y;  
    };  
    Point::Point (Point& p)  
    {  
        X=p.X; //参数p可以直接引用私有变量  
        Y=p.Y;  
    }
```
注意：参数必须为本对象的引用 + 函数体内参数可以直接引用私有变量（老忘）
常见问题：

  1、为什么拷贝函数的参数必须是引用？

              简单点说，为了避免递归。

              具体来说，引用传递的时候不需要调用拷贝构造函数 而 值传递需要调用拷贝构造函数

              所以，在进入拷贝构造函数时，需要把对象传进来，这时使用值传递还要再调一次拷贝构造函数.....这时要无限传递下去

2、浅拷贝与深拷贝

             出现这个问题的原因：构造函数中需要为指针申请空间（简单点说，成员变量含有指针）

             深拷贝：在拷贝构造函数中，为指针显式申请空间（正确的方式）

             浅拷贝：在拷贝构造函数中，仅仅使用成员间的对应复制，两个对象的指针的都指向同一个空间，一个指针指向的空间释放，另一个指针为野指针，危害江湖。

             解决方法：

             如果构造函数需要为类显示申请空间（含指针，使用new），则要使用显式的拷贝构造函数

             如果类中成员都是非指针，则可以使用系统默认的拷贝构造函数。

浅拷贝代码

```cpp
#include <iostream>  
    #include <string>  
    using namespace std;  
    class Point  
    {  
    private:  
        char * name;  
    public:  
        Point(char * className)  
        {  
            name = new char[strlen(className)+1];  
            strcpy(name, className);  
        }  
        Point(Point& p)//浅拷贝,或者不写  
        {  
            name = p.name;//(系统默认拷贝函数执行的代码)  
        }  
        ~Point()  
        {  
            cout<<name<<endl;//测试关键语句  
            delete []name;  
        }  
    };  
      
    int main()  
    {  
        Point a("123");  
        Point b=a;  
        return 0;  
    }
```

结果：![](http://hi.csdn.net/attachment/201107/22/0_13113210069rUF.gif)



深拷贝：

```cpp
#include <iostream>  
    #include <string>  
    using namespace std;  
    class Point  
    {  
    private:  
        char * name;  
    public:  
        Point(char * className)  
        {  
            name = new char[strlen(className)+1];  
            strcpy(name, className);  
        }  
        Point(Point& p)//深拷贝  
        {  
            name = new char[strlen(p.name)+1];  
            strcpy(name,p.name);  
        }  
        ~Point()  
        {  
            cout<<name<<endl;  
            delete []name;  
        }  
    };  
      
    int main()  
    {  
        Point a("123");  
        Point b=a;  
        return 0;  
    }
```
结果：

![](http://hi.csdn.net/attachment/201107/22/0_1311320843RjsH.gif)






## 拷贝构造函数和重载赋值运算符的代码对比：



```cpp
Point::Point(const  Point& p)//拷贝构造函数  
    Point& Point::operator=(const Point&p)//重载赋值运算符
```



注意：

         1、参数都是一样的，都有const和引用，但是带他们的原因不同，具体见上面。

         2、拷贝构造函数无返回值，而重载赋值运算符使用 返回函数引用。




