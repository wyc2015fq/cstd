# c++ 多线程中 创建EXCEl应用程序失败解决办法 - writeeee的专栏 - CSDN博客
2015年09月08日 18:39:57[writeeee](https://me.csdn.net/writeeee)阅读数：1116
                
多线程时用CreateDispatch函数创建组件对象是会出错
解决方法是在线程开始位置加上下面的代码：
```cpp
AFX_MANAGE_STATE(AfxGetStaticModuleState())；
CoInitialize(NULL);
AfxEnableControlContainer();
```
