
# Python C-C++混合编程 - 机器学习的小学生 - CSDN博客


2017年04月27日 20:41:28[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1348


Python 2.7.13，VS2015
VS环境配置：
**1. Release 环境配置：**
包含目录：
```python
D:
```
```python
\Miniconda
```
```python
2
```
```python
\include
```
附加依赖项：
```python
D:
```
```python
\Miniconda
```
```python
2
```
```python
\libs
```
```python
\python
```
```python
27.lib
```
并且确定python27.dll的路径在环境变量中。
**2.Debug 环境配置**
修改附加依赖项为：
```python
D:
```
```python
\Miniconda
```
```python
2
```
```python
\libs
```
```python
\python
```
```python
27_d.lib
```
并且确定python27_d.dll的路径在环境变量中。
我使用的是miniconda管理python,所以上面的配置路径是这样的。因为安装的 python自由release版本的静态库和动态库，因此需要手动编译python源代码，生成自己需要版本的debug版本的lib和dll。
## 1. C/C++ 调用python
**main.c**
```python
#include "stdafx.h"
```
```python
#include <Python.h>
```
```python
#include<iostream>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
func_copy_from_python(
```
```python
int
```
```python
a)
{
```
```python
int
```
```python
res;
    PyObject * module, *func;
    PyObject *args, *value;
```
```python
// 导入module
```
```python
module = PyImport_Import(PyString_FromString(
```
```python
"modulename"
```
```python
));
```
```python
// 获取module中的函数
```
```python
func = PyObject_GetAttrString(module,
```
```python
"power_func"
```
```python
);
```
```python
// 建立参数
```
```python
args = PyTuple_New(
```
```python
1
```
```python
);
    PyTuple_SetItem(args,
```
```python
0
```
```python
, PyInt_FromLong(a));
```
```python
// 调用python module中的函数
```
```python
value = PyObject_CallObject(func, args);
```
```python
// 转换为C类型数据
```
```python
res = PyInt_AsLong(value);
```
```python
return
```
```python
res;
}
```
```python
int
```
```python
main(
```
```python
int
```
```python
argc,
```
```python
char
```
```python
** argv)
{
    Py_SetProgramName(argv[
```
```python
0
```
```python
]);
    Py_Initialize();
```
```python
// do something
```
```python
int
```
```python
a =
```
```python
10
```
```python
;
```
```python
cout
```
```python
<<
```
```python
"result: "
```
```python
<< func_copy_from_python(a) << endl;
```
```python
//PyRun_SimpleString("print 'Hello Python!'\n");
```
```python
Py_Finalize();
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
**modulename.py**
```python
#-*-coding:utf-8-*-
```
```python
'''
Created on 2017年4月27日
@author: Administrator
'''
```
```python
def
```
```python
power_func
```
```python
(a)
```
```python
:
```
```python
return
```
```python
a*a
```
```python
#print power_func(10)
```
## 2.Python 调用C/C++
```python
#include<iostream>
```
```python
#include<Python.h>
```
```python
using namespace std;
```
```python
//函数的具体实现，由C/C++完成
```
```python
int
```
```python
power_func(
```
```python
int
```
```python
a)
{
```
```python
return
```
```python
a*a;
}
```
```python
/*
  包裹函数:_power_func,它负责将Python参数转换为C参数(利用PyArg_ParseTuple),
  通过调用函数 power_func 完成具体的任务，并将返回值，转化后返回给Python环境
*/
```
```python
static
```
```python
PyObject * _power_func(PyObject *
```
```python
self
```
```python
, PyObject *args)
{
```
```python
int
```
```python
_a;
```
```python
// 中间变量
```
```python
int
```
```python
res;
```
```python
if
```
```python
(!PyArg_ParseTuple(args,
```
```python
"i"
```
```python
, &_a))
```
```python
//将python参数转换为C参数
```
```python
return
```
```python
NULL
```
```python
;
    res = power_func(_a);
```
```python
return
```
```python
PyLong_FromLong(res);
}
```
```python
/*
  导出表：moduleMethod. 它负责告诉Python这个模块中，有哪些函数可以被Python调用。
  导出表的名字可以随便起，每一项有4个参数：
                                            第一个参数是提供给Python环境的函数名字。
                                            第二个参数_power_func,即包裹函数。
                                            第三个参数的含义是参数变长。
                                            第四个参数是一个说明性的字符串。
    导出表总是以{NULL,NULL,0,NULL}结束。
*/
```
```python
static
```
```python
PyMethodDef moduleMethod[] = {
    {
```
```python
"power_func"
```
```python
,
        _power_func,
    METH_VARARGS,
```
```python
""
```
```python
},
    {
```
```python
NULL
```
```python
,
```
```python
NULL
```
```python
,
```
```python
0
```
```python
,
```
```python
NULL
```
```python
}
};
```
```python
/*
  导出函数：initmodulename ,这个的名字不是任意取，是你的module名称添加前缀init。
  导出函数中，将模块名称与导出表进行连接。
*/
```
```python
PyMODINIT_FUNC initmymodule(
```
```python
void
```
```python
)
{
    (
```
```python
void
```
```python
)Py_InitModule(
```
```python
"mymodule"
```
```python
, moduleMethod);
}
```

