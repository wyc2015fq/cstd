# 从vc6.0转到vs中出现的错误解析!(有感于sift和surf移植） - 三少GG - CSDN博客
2010年12月09日 17:18:00[三少GG](https://me.csdn.net/scut1135)阅读数：1701

**从vc6.0转到vs2005 2008出现的错误解析**
遇到问题咯，转的别人的文章~~
最近开发平台由VC6.0升级至VS2005，需要将原有的项目迁移，特将碰到的问题归纳如下**：**
１消息映射
VS2005对消息的检查更为严格，以前在VC6下完全正常运行的消息映射在VS2005下编译不通过
a. ON_MESSAGE(message,OnMyMessage);
OnMyMessage返回值必须为LRESULT，其形式为**：**afx_msg LRESULT OnMyMessage(WPARAM, LPARAM)；如果不符合，则有错误提示**：**
**error****C2440:** “**static_cast**”**:****无法从**“void (__thiscall CPppView**:****:*** )(WPARAM,LPARAM)”转换为“LRESULT (__thiscall CWnd**:****:*** )(WPARAM,LPARAM)”
       在匹配目标类型的范围内没有具有该名称的函数
**error****C2440:** “**static_cast**”**:****无法从**“void (__thiscall CPppView**:****:*** )(void)”转换为“LRESULT (__thiscall CWnd**:****:*** )(WPARAM,LPARAM)”
       在匹配目标类型的范围内没有具有该名称的函数
b. ON_COMMAND_EX(id,OnMyMessage2);
在VS2005中，OnMyMessage返回值必须为BOOL，且含有一个 **UINT** 参数指出了命令ID，其形式为**：**afx_msg BOOL OnMyMessage(**UINT**)；如果不符合，
则有错误提示，如在VS6中，OnMyMessage2的定义为afx_msg BOOL OnViewZoomBar()时亦可正常编译通过，但在VS2005下，有错误提示**：**
**error****C2440:** “**static_cast**”**:****无法从**“BOOL (__thiscall CMainFrame**:****:*** )(void)”转换为“BOOL (__thiscall CCmdTarget**:****:*** )(**UINT**)”
       在匹配目标类型的范围内没有具有该名称的函数
**error****C2440:** “**static_cast**”**:****无法从**“BOOL (__thiscall CMainFrame**:****:*** )(void)”转换为“BOOL (__thiscall CCmdTarget**:****:*** )(**UINT**)”
       在匹配目标类型的范围内没有具有该名称的函数
2　字符处理
　在c中广泛使用的strcpy,strcat,strstr等等推荐使用更为安全的strcpy_s,strcat_s,strstr_s等来代替
3　数学函数检查
　VS2005中，数学函数的参数检查更为严格，如pow(2, 45)会引起一个错误提示如下**：**
**error** C2668**:** “pow”**:** 对重载函数的调用不明确
       d**:**/program files/microsoft visual studio 8/vc/include/math.h(575)**:** 可能是“long double pow(long double,int)”
       d**:**/program files/microsoft visual studio 8/vc/include/math.h(527)**:** 或“float pow(float,int)”
       d**:**/program files/microsoft visual studio 8/vc/include/math.h(489)**:** 或“double pow(double,int)”
       试图匹配参数列表“(int, int)”时
正确的使用为pow(2.0, 45)
4　更加符合C++标准
如在VS6中，在FOR循环中的循环变量的定义的作用域延伸到循环体外，VS2005则修正了这样的bug。
VC6**:**
for(int i=0;i<100;i++)f2();  
for(i = 1;i<10;i++)f1(); //i已经定义
而有VS2005中，第二句的i必须重新定义。
===================================================================================
2.  error C2668: “pow”: 对重载函数的调用不明确
MATH.H里定义了6个重载函数：
inline double __cdecl pow(double _X, int _Y)
  {return (_Pow_int(_X, _Y)); }
inline double __cdecl pow(int _X, int _Y)
  {return (_Pow_int(_X, _Y)); }
inline float __cdecl pow(float _X, float _Y)
  {return (powf(_X, _Y)); }
inline float __cdecl pow(float _X, int _Y)
  {return (_Pow_int(_X, _Y)); }
inline long double __cdecl pow(long double _X, long double _Y)
  {return (powl(_X, _Y)); }
inline long double __cdecl pow(long double _X, int _Y)
  {return (_Pow_int(_X, _Y)); }
如pow(2, 45)会引起一个错误提示如下，正确的使用为pow(2.0, 45)等 
==========================================================================
3. error C2668: “sqrt” : 对重载函数的调用不明确
如果
int IsSquareNum(int num)
{
    if((int)sqrt(num)==(int)sqrt(num))
        return 1;
    else
        return 0;
}
则提示上面的错误
改为:
int IsSquareNum(int num)
{
    if((int)sqrt((double)num)==(int)sqrt((double)num))
        return 1;
    else
        return 0;
}
再者，
void main()
{
Demo demo; //定义Demo的对象
int Value_1;
double Value_2;
Value_1=demo.GetValue();
Value_2=sqrt(Value_1); //开平方根
cout<<Value_2<<endl;
system("pause");
}
调试时出现以下错误：
1>./demo.cpp(30) : error C2668: “sqrt”: 对重载函数的调用不明确
1>        C:/Program Files/Microsoft Visual Studio 9.0/VC/include/math.h(581): 可能是“long double sqrt(long double)”
1>        C:/Program Files/Microsoft Visual Studio 9.0/VC/include/math.h(533): 或       “float sqrt(float)”
1>        C:/Program Files/Microsoft Visual Studio 9.0/VC/include/math.h(128): 或       “double sqrt(double)”
参考以上提示，将 int Value_1;修改为 double Value_1;
这样就符合“double sqrt(double)”的规则了。
运行正确，运行结果为3.
再如，
对于这个问题在VC6上没有问题，在VS.net中应该改为
把k=(double)sqrt((int)a);
改成k=(double)sqrt((double)a);
就能运行
可能原因解释：  重载函数二义性，不知道用哪个转换将int转换之后更好，这三个函数都是标准转换，所以一样好。你可以用转换把int类型参数static_cast<T>(x)，T可以是float就行，因为它应该足够表示int了。很多VC6下可以运行的程序，到了VC2008下，都不会直接通过，因为VC2008更标准了，VC6应该说有点老了。
