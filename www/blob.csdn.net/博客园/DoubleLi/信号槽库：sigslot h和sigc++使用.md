# 信号槽库：sigslot.h和sigc++使用 - DoubleLi - 博客园






用qt的知道，qt有方便简单的信号槽机制，但需要专门的qt工具处理。 
如果想直接使信号槽就可以使用sigslot库，或者sigc++库，或者boost中的signals，这里介绍sigslot和sigc++库。

sigslot.h：只有一个头文件，使用简单方便。 
sigc++：包含文件多，但功能更强大。

# sigslot库

## 官方地址

[http://sigslot.sourceforge.net/](http://sigslot.sourceforge.net/)

## 在vs2013中使用
- 包含头文件
`#include "sigslot.h"`- 1
- 1
- 改动：

```
//在sigslot.h的420,将：
typedef sender_set::const_iterator const_iterator;
    //改为：
typedef typename sender_set::const_iterator const_iterator;
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4

## 基本使用：
- signal0~signal8：信号类：作为类成员

```
class mySg
    {
        sigc::signal0<>                 sg1;    // 无参数
        sigc::signal2<char*, double>    sg2;    // 2个参数
    }
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
- connection 
槽函数：作为类成员，类需要继承has_slots<>，且槽函数的返回值必须是void类型

```
class mySlot: public : has_slots<>
    {
    public:
        void on_func1(){}                       // 无参数，与信号对应
        void on_func2(char*, double)(){}        // 2个参数
    };
    mySg    sig;
    mySlot  slt;
    sig.sg1.conncent(&slt,&mySlot::on_func1);
    sig.sg2.conncent(&slt,&mySlot::on_func2);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- disconnection 
解除连接：可以使用disconnect()和disconnect_all()

```
sig.sg1.disconnect(&slt);
    sig.sg1.disconnect_all();
```
- 1
- 2
- 1
- 2
- emiting 
发送信号：可以直接使用()运算符，也可以调用signal的emit函数

```
sig.sg1.emit();
    sig.sg2("str",0.1);
```
- 1
- 2
- 1
- 2

## 实例：

```cpp
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "sigslot.h"
using namespace sigslot;

#define pause_wait()    {std::cout << "\nPlease press any key to continue.\n";_getch();}


// class
enum    ObjState{Normal = 0, Hidden = 1};
class ObjectBase : public has_slots<> 
{
public:
    ObjectBase(){ px = 0; py = 0; state = Normal; }
    ObjectBase(const int &x, const int &y):px(x), py(y){ state = Normal; }
    ~ObjectBase(){};

    void    onMoveBy(const int &x, const int &y){px += x; py += y;}
    void    onResize(const int &x, const int &y){px = x; py = y;}
    void    onSetState(ObjState s){this->state = s;}
    void    onPrintState()
    {
        if(state == Normal)
            std::cout << "Normal" << std::endl;
        else if(state == Hidden)
            std::cout << "Hidden" << std::endl;
        else
            std::cout << "Error: error state value" << std::endl;
    }

    const int&  getX(){return px;}      
    const int&  getY(){return py;}
    const char* getState()
    {
        if(state == Normal)
            return "Normal";
        else if(state == Hidden)
            return "Hidden";
        else
            return nullptr;
    }

private:
    ObjState    state;
    int     px;
    int     py;
};

class Control
{
public:
    signal2<const int&, const int&> moveBy;
    signal2<const int&, const int&> resize;
    signal1<ObjState>   setState;
    signal0<>       printState;;
};

int main(int argc, char* argv[])
{
    ObjectBase obj(10,10);
    Control ctrl;

#if(1)
    ctrl.moveBy.connect(&obj, &ObjectBase::onMoveBy);
    ctrl.resize.connect(&obj, &ObjectBase::onResize);
    ctrl.setState.connect(&obj, &ObjectBase::onSetState);
    ctrl.printState.connect(&obj, &ObjectBase::onPrintState);

    //ctrl.moveBy.disconnect(&obj);
    //ctrl.moveBy.disconnect_all();

    // start pro loop
PRO_Start:
    while(true)
    {
        system("cls");
        cout << "Object Information" << endl;
        cout << "\t Position_x: " << obj.getX() << endl;
        cout << "\t Position_y: " << obj.getY() << endl;
        cout << "\t State: " << obj.getState() << endl;
        cout << "\n";
        cout << "\t m: move object \n"
                "\t r: resize object \n"
                "\t s: set object state \n"
                "\t p: print object state \n"   
                << endl;
        cout << "Input:";

        char ch = getchar();
        switch(ch)
        {
            case 'm':       // move
                ctrl.moveBy(10,10);
                break;
            case 'M':
                ctrl.moveBy.emit(-20,-20);
                break;

            case 'R':
            case 'r':       // resize
                ctrl.resize.emit(0, 0);
                break;

            case 'P':
            case 'p':       // printState
                goto PRO_PrintState;
                break;

            case 's':       // setState
                ctrl.setState(Normal);
                break;
            case 'S':
                ctrl.setState.emit(Hidden);
                break;

            case 'Q':
            case 'q':       // exit
                goto PRO_Exit;
                break;
            default: 
                break;
        }
    }

PRO_PrintState:
    ctrl.printState.emit();
    pause_wait();
    goto PRO_Start;

#endif
    pause_wait();

PRO_Exit:
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139

# sigc++库

## 官方地址

[http://libsigc.sourceforge.net/](http://libsigc.sourceforge.net/)

## 编译

可以在[http://download.csdn.net/detail/yehuohan/9600368](http://download.csdn.net/detail/yehuohan/9600368)下载已经编好的库文件，2.8.0版 
注意1：在用高版本编译时，如2.8.0，可能会提示找不到msvc_recommended_pragmas.h，可以在vs的
`属性 -> C/C -> 高级 -> 强制包含文件`- 1
- 1

去掉msvc_recommended_pragmas.h，或者添加一个msvc_recommended_pragmas.h文件，在[http://download.csdn.net/detail/yehuohan/9600369](http://download.csdn.net/detail/yehuohan/9600369)的源代码时已经放入了该文件。 
注意2：在使用高版本(如2.9.7)编译时，若提示找不到slot.cc等文件，可以使用低版本中的放入相应位置即可。

## 在vs中使用

```
* 项目 -> VC++目录 -> 包含目录： 添加 sigc++所在的目录
* 添加下列代码：
```

```
#include <sigc++/sigc++.h>
        #ifdef _DEBUG
            #pragma comment(lib,"sigcd.lib")
        #else
            #pragma comment(lib,"sigc.lib")
        #endif
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6

## 基本使用：
- signal 
信号类：可以作为其它类成员，也可直接定义

```
//返回值: int，2个参数:char*, double，参数个数可以直接指定
    sigc::signal<int, char*, double>    sg1;
    sigc::signal2<int, char*, double>   sg2;
    class mySg
    {
        sigc::signal2<int, char*, double>   sg3;
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- connection 
连接槽函数，作为类成员时，要继承sigc::trackable； 
ptr_fun, ptr_fun0~7 : 连接全局函数； 
mem_fun, mem_fun0~7 : 连接类成员函数； 
ptr_fun,mem_fun与signal之间只要参数类型对应就可连接， 并非一定要数字对应（如：ptr_fun3并非一定要与signal3对应连接）

```
sigc::connection ccSg1 = sg1.connect(sigc::ptr_fun(&func1));
    class mySlot: public : sigc::trackable
    {
    public:
        int func2(char*, double)(){}
    } obj;
    sigc::connection ccSg2 = sg2.connect(sigc::mem_fun2(obj,&mySlot::func));
    class mySg: public : sigc::trackable
    {
        mySg(){auto ccIter_Sg3 = sg3.connect(sigc::mem_fun(*this,&mySg::on_func));}
        sigc::signal2<int, char*, double>   sg3;
        int on_func(char*, double)(){}
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- disconnection 
如果不需要解连接，可以不用定义sigc::connection

```
ccSg1.disconnect();
    ccSg2.disconnect();
```
- 1
- 2
- 1
- 2
- emiting 
// 发送信号：调用signal的emit函数

```
sg1.emit("str",0.1);
    sg2.emit("str",0.1);
```
- 1
- 2
- 1
- 2

## 实例：

```cpp
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <iostream>
using namespace std;

#include <sigc++/sigc++.h>
#ifdef _DEBUG
    #pragma comment(lib,"sigc280d.lib")
#else
    #pragma comment(lib,"sigc280.lib")
#endif // _DEBUG

#define pause_wait()    {std::cout << "\nPlease press any key to continue.\n";_getch();}


// class
enum    ObjState{Normal = 0, Hidden = 1};
class ObjectBase : public sigc::trackable
{
public:
    ObjectBase(){ px = 0; py = 0; state = Normal; setConnection(); }
    ObjectBase(const int &x, const int &y) :px(x), py(y){ state = Normal; setConnection(); }
    ~ObjectBase(){};

    sigc::signal<void,const char*>  putMsg;

    void    onMoveBy(const int &x, const int &y){px += x; py += y;}
    void    onResize(const int &x, const int &y){px = x; py = y;}
    void    onSetState(ObjState s){this->state = s;}
    void    onPrintState()
    {
        if(state == Normal)
            std::cout << "Normal" << std::endl;
        else if(state == Hidden)
            std::cout << "Hidden" << std::endl;
        else
            std::cout << "Error: error state value" << std::endl;
    }

    const int&  getX(){return px;}      
    const int&  getY(){return py;}
    const char* getState()
    {
        if(state == Normal)
            return "Normal";
        else if(state == Hidden)
            return "Hidden";
        else
            return nullptr;
    }

private:
    ObjState    state;
    int     px;
    int     py;

    void    printMsg(const char* str){std::cout << str << std::endl; }
    void    setConnection()
    {
        putMsg.connect(sigc::mem_fun(*this, &ObjectBase::printMsg));
    }
};

class Control
{
public:
    Control(){}

    sigc::signal<void, const int&, const int&>  moveBy; 
    sigc::signal2<void, const int&, const int&> resize;     // return void type, and 2 parameters
    sigc::signal<void, ObjState>    setState;

private:
};

// print word steped by space
int printSpaceStr(const char* str)
{
    int             cnt = 0;
    const char*     p = str;
    char            ch;
    std::cout << endl;
    do
    {
        ch = *p;
        cout << ch << ' ';
        cnt++;
    } while (*++p != '\0');
    std::cout << endl;

    return cnt;
}

int main(int argc, char* argv[])
{
    ObjectBase obj(0, 0);
    Control ctrl;

    sigc::signal1<int, const char*> printSS;
    sigc::connection ccPrintSS = printSS.connect(sigc::ptr_fun1(&printSpaceStr));
    //ccPrintSS.disconnect();

    sigc::signal0<void>     printState;
    printState.connect(sigc::mem_fun(obj, &ObjectBase::onPrintState));

    ctrl.moveBy.connect(sigc::mem_fun(obj, &ObjectBase::onMoveBy));
    ctrl.resize.connect(sigc::mem_fun(obj, &ObjectBase::onResize));
    auto ccIter_SetState = ctrl.setState.connect(sigc::mem_fun(obj, &ObjectBase::onSetState));
    //ccIter_SetState->disconnect();

    // start pro loop
PRO_Start:
    while(true)
    {
        system("cls");
        obj.putMsg.emit("Object Information");
        cout << "\t Position_x: " << obj.getX() << endl;
        cout << "\t Position_y: " << obj.getY() << endl;
        cout << "\t State: " << obj.getState() << endl;
        cout << "\n";
        cout << "\t m: move object \n"
                "\t r: resize object \n"
                "\t s: set object state \n"
                "\t p: print object state \n"   
                "\t a: print space word \n"
                << endl;
        cout << "Input:";

        char ch = getchar();
        switch(ch)
        {
            case 'm':       // move
                ctrl.moveBy.emit(10,10);
                break;
            case 'M':
                ctrl.moveBy.emit(-20,-20);
                break;

            case 'R':
            case 'r':       // resize
                ctrl.resize.emit(0, 0);
                break;

            case 'P':
            case 'p':       // printState
                goto PRO_PrintState;
                break;

            case 's':       // setState
                ctrl.setState.emit(Normal);
                break;
            case 'S':
                ctrl.setState.emit(Hidden);
                break;

            case 'A':
            case 'a':
                goto RPO_PrintSpaceStr;
                break;

            case 'Q':
            case 'q':       // exit
                goto PRO_Exit;
                break;
            default: 
                break;
        }
    }

PRO_PrintState:
    printState.emit();
    pause_wait();
    goto PRO_Start;

RPO_PrintSpaceStr:
    printSS.emit("Sigc_Demo");
    pause_wait();
    goto PRO_Start;

PRO_Exit:
    return 0;
}
```









