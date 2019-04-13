
# c++-tcl编程总结 - Augusdi的专栏 - CSDN博客


2015年08月22日 20:42:22[Augusdi](https://me.csdn.net/Augusdi)阅读数：2521


﻿﻿
**一、           给Tcl脚本直接传递char*参数，Tcl脚本中的所有参数和结果都是char*类型**
**test.tcl**

```python
proc add {a b c} {
set d [expr $a + $b + $c]
return $d
}
```
**test.cpp**
```python
// gettclvalueofvariable.cpp
#include "stdafx.h"
#include "iostream"
#include "stdio.h"
#include "string"
#include "tcl.h"
#include "assert.h"
#include "tclDecls.h"
#pragma comment(lib,"tcl85.lib")
using namespace std;
#define STRCMD_SIZE 50
int _tmain(int argc, _TCHAR* argv[])
{
         Tcl_Main(argc,argv,Tcl_AppInit);
         return 0;
}
int Tcl_AppInit(Tcl_Interp *interp)
{
         if (Tcl_Init(interp)==(TCL_ERROR))
         {
                   return TCL_ERROR;
         }
         if(Tcl_EvalFile(interp,"test.tcl")==TCL_ERROR)
         {
                   return TCL_ERROR;
         }
         char *str = "10";
         char *str1 = "11";
         char *str12 = "12";
         char strcmd[STRCMD_SIZE] = {0};
         sprintf(strcmd,"add %s %s %s",str,str1,str12);
         Tcl_eval_r(interp,strcmd);
         Tcl_GetVar(interp,"d",TCL_GLOBAL_ONLY);
         cout<<interp->result<<endl;
         char *p = NULL;
         p = interp->result;
         assert(p != NULL);
         cout<<"the value of p is: "<<p<<endl;
         return 1;
}
```
**二、           由于tcl的参数是char*类型的，所以在本程序中直接传递的char*类型参数，但是在实际过程中，往往c++函数需要接收string类型的参数，并传递给tcl脚本，这就涉及到string转换成char*以及char*转换成string。这样从tcl中获得的值可以转换成string，并传递给其他c++模块。**
**三、           接收string类型的实参，转换成char*类型，传递给Tcl脚本；从Tcl脚本接收到char*类型的结**
```python
#include "stdafx.h"#include "iostream"#include "string"using namespace std;int _tmain(int argc, _TCHAR* argv[]){         char* p = "hello";         cout<<"the value of p is: "<<p<<endl;         char m[10] = {0};         string result;         memcpy(m,p,sizeof(p)+1);         result = string(m);         cout<<"the value of m is: "<<m<<endl;         cout<<"the value of result is: "<<result<<endl;         return 0;}
```
**果后，转换成string，传递给其他c++函数。Tcl脚本同上例。**
```python
// 0906.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "tcl.h"
#include "string"
#include "iostream"
#include "stdio.h"
#include "assert.h"
#pragma comment(lib,"tcl85.lib")
#define ARRAYSIZE 30
using namespace std;
string callTcl()
{      
         Tcl_Interp *interp = Tcl_CreateInterp();
         //change string to char*
         string a = "123";                          //change a to char*
         char* str1 = const_cast<char*>(a.c_str());
         cout<<"the value of str1 is "<<str1<<endl;
         string b = "13";                          //change b to char*
         char* str2 = const_cast<char*>(b.c_str());
         cout<<"the value of str2 is: "<<str2<<endl;
         string c = "26";                            //change c to char*
         char* str3 = const_cast<char*>(c.c_str());
         cout<<"the value of str3 is: "<<str3<<endl;
         //call tcl and trans the char* to string and return
         if (Tcl_EvalFile(interp,"test.tcl") == TCL_ERROR)
         {
                   //return TCL_ERROR;
                   cout<<"call tcl error"<<endl;
         }
         char strcmd[ARRAYSIZE] = {0} ;
         sprintf(strcmd, "add %s %s %s", str1,str2,str3);
         Tcl_eval_r(interp,strcmd);
         char *p = NULL;
         Tcl_GetVar(interp,"d",TCL_GLOBAL_ONLY);
         cout<<"the value of interp->result is: "<<interp->result<<endl;
         p = interp->result;
         assert(p != NULL);
         cout<<"the value of p is: "<<p<<endl;
         //trans p to string type
         char result[60] = {0};
         memcpy(result,p,sizeof(p)+1);
         string sresult = string(result);
         return(sresult);
}
 
int _tmain(int argc, _TCHAR* argv[])
{
         Tcl_Main(argc,argv,Tcl_AppInit);
         return 0;
} 
int Tcl_AppInit(Tcl_Interp *interp)
{
         if (Tcl_Init(interp)==TCL_ERROR)
         {
                   return TCL_ERROR;
         }
         string a;
         a = callTcl();//call callTcl function
         callTcl();
         cout<<"the value of a is: "<<a<<endl;
         return TCL_OK;
}
```
运行结果如下图：
**四、           取名字空间中的值**
test1.tcl

```python
package provide test1 1.0
namespace eval test1 {
         namespace export a
         variable n 5
         proc a {} {
                   variable n
                   set n 6
                   return $n
                   #puts $n
         }
}
```
test2.tcl

```python
package provide test2 1.0
package require test1 1.0
namespace import test1::a
namespace eval test2 {
set b [a]
puts $b
}
```
用active tcl 可以运行test2.tcl，返回b的值为6，要想取得名字空间变量的值用set b test1::n即可。
**五、           在调用tcl脚本并取值时没必要遵循**
int _tmain(int argc,_TCHAR* argv[] )
{
Tcl_Main(argc,argv,Tcl_AppInit);
return 0;
}
Int Tcl_AppInit(Tcl_Interp *interp)
{
call .tcl and get variable value
}层次结构
只要在一般的函数里遵循初始化interp→调用tcl脚本→取值即可，下面是实例代码：
test.tcl代码如下：
puts "hello"
proc test {} {
set a 4
return $a
}

C++代码如下：
\#include "stdafx.h"
\#include "iostream"
\#include "stdio.h"
\#include "string"
\#include "tcl.h"
\#include "assert.h"
\#include "tclDecls.h"
\#include "assert.h"
\#pragma comment(lib,"tcl85.lib")

using namespace std;
\#pragma comment (lib,"tcl85.lib")
int calltcl();
int _tmain(int argc, _TCHAR* argv[])
{
calltcl();
return 0;
}
int calltcl()
{
Tcl_Interp *interp = Tcl_CreateInterp();
if (Tcl_Init(interp)==TCL_ERROR)
{
return TCL_ERROR;
}
if (Tcl_EvalFile(interp,"test.tcl")==TCL_ERROR)
{
return TCL_ERROR;
}
char strcmd[30] = {0};
sprintf(strcmd,"test");//get the value of a variable in namespace
//sprintf(strcmd,"add %s %s %s",str,str1,str12);//get the value of a variable in proc
Tcl_eval_r(interp,strcmd);
Tcl_GetVar(interp,"a",TCL_GLOBAL_ONLY);
cout<<interp->result<<endl;
char *p = NULL;
p = interp->result;
assert(p != NULL);
cout<<"the value of p is: "<<p<<endl;
}
运行结果截图：
【注】本部分中的.tcl脚本均和.exe在同一目录下，如果.tcl在某一个文件夹下，比如，test.tcl在testlib文件夹中，则：
if (Tcl_EvalFile(interp,"test.tcl")==TCL_ERROR)改为if (Tcl_EvalFile(interp,"./testlib/test.tcl")==TCL_ERROR)即可
【Tcl同一个名字空间中两个不同过程实现变量共享】
Tcl代码：
namespace eval test {
global b
global testResult
global c
\#----------------
proc test1 {} {
global b
global testResult
set b 30123
set testResult 400
return $testResult
}
proc print {} {
global b
global c
set c $b
return $b
}
}
\#test::test1
\#test::print
C++代码：
\#include "stdafx.h"
\#include "iostream"
\#include "stdio.h"
\#include "string"
\#include "tcl.h"
\#include "assert.h"
\#include "tclDecls.h"
\#include "assert.h"
using namespace std;
\#pragma comment (lib,"tcl85.lib")
int calltcl();
int _tmain(int argc, _TCHAR* argv[])
{
calltcl();
return 0;
}
int calltcl()
{
Tcl_Interp *interp = Tcl_CreateInterp();//interp负o责e获?取¨?b的Ì?值¦Ì
if (Tcl_Init(interp)==TCL_ERROR)
{
return TCL_ERROR;
}
if (Tcl_EvalFile(interp,"tes01t.tcl")==TCL_ERROR)
{
return TCL_ERROR;
}
Tcl_Interp *interp1 = Tcl_CreateInterp();
if (Tcl_Init(interp1)==TCL_ERROR)
{
return TCL_ERROR;
}

if (Tcl_EvalFile(interp1,"tes01t.tcl")==TCL_ERROR)
{
return TCL_ERROR;
}

char strcmd[30] = {0};
char strcmd1[30] = {0};
sprintf(strcmd1,"test::test1");
sprintf(strcmd,"test::print");//get the value of b variable in namespace

Tcl_eval_r(interp1,strcmd1);//获?取¨?testResult值¦Ì
Tcl_eval_r(interp,strcmd1);//获?取¨?b的Ì?值¦Ì，ê?先¨¨执¡ä行D一°?次ä?test1过y程¨¬
Tcl_eval_r(interp,strcmd);
Tcl_GetVar(interp1,"testResult",TCL_GLOBAL_ONLY);
Tcl_GetVar(interp,"b",TCL_GLOBAL_ONLY);

char *p = NULL;
p = interp->result;
assert(p != NULL);
string value(p);
cout<<"the value of b is: "<<p<<endl;
char *p1 = NULL;
p1 = interp1->result;
assert(p1 != NULL);
string value1(p1);
cout<<"the value of testResult is: "<<p1<<endl;
}
返回值：

【注】以下代码用同一个解释器一样获取相同结果
\#include "stdafx.h"
\#include "iostream"
\#include "stdio.h"
\#include "string"
\#include "tcl.h"
\#include "assert.h"
\#include "tclDecls.h"
\#include "assert.h"
using namespace std;
\#pragma comment (lib,"tcl85.lib")
int calltcl();
int _tmain(int argc, _TCHAR* argv[])
{
calltcl();
return 0;
}
int calltcl(
{
Tcl_Interp *interp = Tcl_CreateInterp();//interp负o责e获?取¨?b的Ì?值¦Ì
if (Tcl_Init(interp)==TCL_ERROR)
{
return TCL_ERROR;
}
if (Tcl_EvalFile(interp,"tes01t.tcl")==TCL_ERROR)
{
return TCL_ERROR;
}
char strcmd[30] = {0};
sprintf(strcmd,"test::test1");
Tcl_eval_r(interp,strcmd);
Tcl_GetVar(interp,"testResult",TCL_GLOBAL_ONLY);

char *p = NULL;
p = interp->result;
assert(p != NULL);
string value(p);
cout<<"the value of testResult is: "<<p<<endl;

sprintf(strcmd,"test::print");
Tcl_eval_r(interp,strcmd);
Tcl_GetVar(interp,"b",TCL_GLOBAL_ONLY);
char *pb = NULL;
pb = interp->result;
assert(pb != NULL);
string bvalue(pb);
cout<<"the b value is:"<<endl<<bvalue<<endl;
}


