# C++异常处理（一） - iStack - Koma Hub - CSDN博客
2018年06月10日 14:31:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：250
```cpp
#include<iostream>
#include<string>
#include<vector>
using namespace std;
/**
1 要求一个pop()操作但栈却是空的
2 要求一个push()操作但栈却是满的
*/
/*定义一个异常类*/
class popOnEmpty { 
public:
    string info = string("Pop on empty!");
};
/*定义一个异常类*/
class pushOnFull { 
public:
    string info = string("Push on full!");
    /*这个异常类可以接收来自throw的参数value*/
    pushOnFull( int i ) : _value( i ) { }
    int value() { return _value; }
private:
    int _value;
};
/*编写一个堆栈类*/
class iStack {
public:
    /*构造函数*/
    iStack( int capacity )
            /*初始化参数列表*/
            : _stack( capacity ), _top( 0 ) { }
    /*出栈*/
    void pop( int &top_value );
    /*压入栈*/
    void push( int value );
    /*栈满*/
    bool full();
    /*栈空*/
    bool empty();
    /*显示栈数据*/
    void display();
    /*栈的大小*/
    int size();
private:
    /*栈顶*/
    int _top;
    /*栈空间数组*/
    vector< int > _stack;
};
/*出栈*/
void iStack::pop( int &top_value )
{
    if ( empty() )
        throw popOnEmpty();
    top_value = _stack[ --_top ];
    cout << "iStack::pop(): " << top_value << endl;
}
/*入栈*/
void iStack::push( int value )
{
    cout << "iStack::push( " << value << " ) \n";
    if ( full() )
        throw pushOnFull(value);
    _stack[ _top++ ] = value;
}
/*栈为空，*/
bool iStack::empty()
{
    return _top == 0 ? true:false;
}
/*栈为满*/
bool iStack::full()
{
    return _stack.size() == _top? true : false;
} 
/*显示一个栈*/
void iStack::display()
{
    for(auto i = _stack.begin(); i != _stack.end(); i++)
        cout<<*i<<"-";
    cout<<"\n";
}
/*主函数*/
int main(int argc, char *argv[])
{
    iStack stack(20);
    /*一个try 块引入一个局部域在try
      块内声明的变量不能在try 块外被引用*/
    try {
        for ( int ix = 1; ix < 501; ++ix )
        {
            if ( ix % 3 == 0 )
                stack.push( ix );
            if ( ix % 4 == 0 )
                stack.display();
            if ( ix % 10 == 0 ) {
                int dummy;
                stack.pop( dummy );
                stack.display();
            }
        }
    }
    /*
    1.如果这些catch 子句不包含返回语句那么程序的执行将继续到哪儿呢在catch 子句完
        成它的工作之后程序的执行将在catch 子句列表的最后子句之后继续进行.
    2.catch 子句被检查的顺序与它们在try 块之后出现的顺序相同
    */
    catch(...){//catch所有异常
        cout<<"catch(...)"<<endl;
        try{
            throw;
        }
        catch ( pushOnFull &e) { 
            cout<<e.info<<e.value()<<endl; 
            try{
                //被重新抛出的异常就是原来的异常对象
                throw;
            }catch ( pushOnFull &e) { cout<<"Twice: "<<e.info<<endl; }
        }
        //这里的e用throw异常的拷贝进行初始化
        //为了防止不必要地拷贝大型类对象class 类型的参数应该被声明为引用
        catch ( popOnEmpty &e) { cout<<e.info<<endl; }
    }
    
    return 1;
}
/**
 *  栈展开stack unwinding:
 *  在查找用来处理被抛出异常的catch 子句时因为异常而退出复合语句和函数定义这
 *   个过程被称作栈展开stack unwinding
 * 本程序中，栈展开: throw -> pop() -> main()
 */
```
编译运行：
```cpp
$ g++ Demo.cpp
$ ./a.exe
iStack::push( 3 )
3-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 6 )
3-6-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 9 )
iStack::pop(): 9
3-6-9-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 12 )
3-6-12-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 15 )
3-6-12-15-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 18 )
3-6-12-15-18-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::pop(): 18
3-6-12-15-18-0-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 21 )
iStack::push( 24 )
3-6-12-15-21-24-0-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 27 )
3-6-12-15-21-24-27-0-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 30 )
iStack::pop(): 30
3-6-12-15-21-24-27-30-0-0-0-0-0-0-0-0-0-0-0-0-
3-6-12-15-21-24-27-30-0-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 33 )
iStack::push( 36 )
3-6-12-15-21-24-27-33-36-0-0-0-0-0-0-0-0-0-0-0-
iStack::push( 39 )
3-6-12-15-21-24-27-33-36-39-0-0-0-0-0-0-0-0-0-0-
iStack::pop(): 39
3-6-12-15-21-24-27-33-36-39-0-0-0-0-0-0-0-0-0-0-
iStack::push( 42 )
3-6-12-15-21-24-27-33-36-42-0-0-0-0-0-0-0-0-0-0-
iStack::push( 45 )
iStack::push( 48 )
3-6-12-15-21-24-27-33-36-42-45-48-0-0-0-0-0-0-0-0-
iStack::pop(): 48
3-6-12-15-21-24-27-33-36-42-45-48-0-0-0-0-0-0-0-0-
iStack::push( 51 )
3-6-12-15-21-24-27-33-36-42-45-51-0-0-0-0-0-0-0-0-
iStack::push( 54 )
3-6-12-15-21-24-27-33-36-42-45-51-54-0-0-0-0-0-0-0-
iStack::push( 57 )
iStack::push( 60 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-60-0-0-0-0-0-
iStack::pop(): 60
3-6-12-15-21-24-27-33-36-42-45-51-54-57-60-0-0-0-0-0-
iStack::push( 63 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-0-0-0-0-0-
iStack::push( 66 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-0-0-0-0-
iStack::push( 69 )
iStack::pop(): 69
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-69-0-0-0-
iStack::push( 72 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-72-0-0-0-
iStack::push( 75 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-72-75-0-0-
iStack::push( 78 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-72-75-78-0-
iStack::pop(): 78
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-72-75-78-0-
iStack::push( 81 )
iStack::push( 84 )
3-6-12-15-21-24-27-33-36-42-45-51-54-57-63-66-72-75-81-84-
iStack::push( 87 )
catch(...)
Push on full!87
Twice: Push on full!
```
另外，对于异常规范，C++11已经弃用动态异常规范，例如：
```cpp
/*编写一个堆栈类*/
class iStack {
public:
    /*构造函数*/
    iStack( int capacity )
            /*初始化参数列表*/
            : _stack( capacity ), _top( 0 ) { }
    /*出栈*/
    void pop( int &top_value ) throw (popOnEmpty);
    /*压入栈*/
    void push( int value ) throw (pushOnFull);
    /*栈满*/
    bool full();
    /*栈空*/
    bool empty();
    /*显示栈数据*/
    void display();
    /*栈的大小*/
    int size();
private:
    /*栈顶*/
    int _top;
    /*栈空间数组*/
    vector< int > _stack;
};
/*出栈*/
//动态异常规范已弃用
void iStack::pop( int &top_value ) throw (popOnEmpty)
{
    if ( empty() )
        throw popOnEmpty();
    top_value = _stack[ --_top ];
    cout << "iStack::pop(): " << top_value << endl;
}
/*入栈*/
void iStack::push( int value )throw (pushOnFull)
{
    cout << "iStack::push( " << value << " ) \n";
    if ( full() )
        throw pushOnFull(value);
    _stack[ _top++ ] = value;
}
```
在编译过程中提示：
```
$ g++ Demo.cpp
Demo.cpp:35:32: 警告：dynamic exception specifications are deprecated in C++11  -Wdeprecated]
     void pop( int &top_value ) throw (popOnEmpty);     动态异常规范已弃用
                                ^~~~~
Demo.cpp:37:28: 警告：dynamic exception specifications are deprecated in C++11  -Wdeprecated]
     void push( int value ) throw (pushOnFull);
                            ^~~~~
Demo.cpp:54:36: 警告：dynamic exception specifications are deprecated in C++11  -Wdeprecated]
 void iStack::pop( int &top_value ) throw (popOnEmpty)
                                    ^~~~~
Demo.cpp:62:31: 警告：dynamic exception specifications are deprecated in C++11  -Wdeprecated]
 void iStack::push( int value )throw (pushOnFull)
```
