# max 宏定义取消：error C2589:  error C2059: 语法错误 : “::” - wishchinYang的专栏 - CSDN博客
2013年10月18日 16:39:19[wishchin](https://me.csdn.net/wishchin)阅读数：2469
原文链接：[http://blog.csdn.net/danelumax2/article/details/9172465](http://blog.csdn.net/danelumax2/article/details/9172465)有修改！
一：关于Pcl和WIndef的冲突：
1. 错误输出
    ./zlibrary/ui/src/win32/w32widgets/W32VBorderBox.cpp(114) : error C2589: “(”: “::”右边的非法标记
    ./zlibrary/ui/src/win32/w32widgets/W32VBorderBox.cpp(114) : error C2059: 语法错误 : “::”
2. 错误代码举例
**[c-sharp]**[view
 plain](http://blog.csdn.net/zhuangshn/article/details/5537499#)[copy](http://blog.csdn.net/zhuangshn/article/details/5537499#)
- size.Width = std::max(size.Width, elementSize.Width);  
3. 函数模板max   
**[c-sharp]**[view
 plain](http://blog.csdn.net/zhuangshn/article/details/5537499#)[copy](http://blog.csdn.net/zhuangshn/article/details/5537499#)
- template<class _Ty> inline  
- const _Ty& (__CLRCALL_OR_CDECL max)(const _Ty& _Left, const _Ty& _Right)  
-     {   // return larger of _Left and _Right
- return (_DEBUG_LT(_Left, _Right) ? _Right : _Left);  
-     }  
   注：模板就是实现代码重用机制的一种工具，它可以实现类型参数化，即把类型定义为参数， 从而实现了真正的代码可重用性。模版可以分为两类，一个是函数模版，另外一个是类模版。
4. 错误原因
    函数模板max与Visual C++中的全局的宏max冲突。
5. 解决办法
    第一种办法：设置项目属性，在预定义处理器中添加定义NOMINMAX来禁止使用Vsual C++的min/max宏定义。
                       项目属性   ——> C/C++ ——> 预处理器 ——> 预处理器定义 (此处添加预定义编译开关   NOMINMAX）
      注意！！！一定要加在第一个上面！！！坑爹的微软！！！
    第二种办法： 加上括号，与Vsual C++的min/max宏定义区分开
(可能是跟Windows中定义的宏想混淆了
可以用括号改变语句的顺序，强制制定为stl中的函数名
double min=（std::numeric_limits<double>::max）();)
**[c-sharp]**[view
 plain](http://blog.csdn.net/zhuangshn/article/details/5537499#)[copy](http://blog.csdn.net/zhuangshn/article/details/5537499#)
- size.Width = std::max(size.Width, elementSize.Width);  
 修改为：
**[c-sharp]**[view
 pl](http://blog.csdn.net/zhuangshn/article/details/5537499#)size.Width = (std::max)(size.Width, elementSize.Width); 
**二：**终极方案：对于使用了Windef文件的AtlImage.h 和GDIPlus.h
直接对里面的max() min()函数进行替换 ；
或者 在使用到 max() min()函数 的头文件定义后面添加：
```cpp
if nodef max
def max(a,b)  FUN..............
endDEF....
```
