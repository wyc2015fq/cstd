# Cython基础 - littlely_ll的博客 - CSDN博客





2018年12月23日 14:45:38[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：230








## cdef

cdef是告诉编译器这条语句是C类型的语句，例如：

```
#pysum.pyx

cdef int Csum(int a, int b)：
    return a + b
```

它和真正的C语言定义的函数有一些区别，例如c用`{}`包裹函数体，而cdef用`:`定义。

python中不能直接调用cdef定义的函数(直接调用cdef定义的函数会出现ImportError:cannot import name ‘Csum’)，但可以通过python定义函数的形式进行调用它：

```
#pysum.pyx

def pysum(int a, int b):
    return Csum(a,b)
```

> - cdef,cpdef和def的区别：cdef定义的函数其实是C语言函数的原型，其只能被Python的函数进行调用，而不能被Python直接调用；cpdef除了能实现cdef的功能，其也能被python直接调用，但是变量的类型必须在C语言和Python中都存在才可以(尽量避免使用cpdef)；def定义的函数可以直接被Python可调用，def定义的函数内也可调用cdef定义的函数。


## python内部类型

在cython中可以使用python内部类型来声明变量：

```
cdef list parts, country=['中国','America']
cdef dict d
cdef str name
cdef set coll
cdef object strs=['中国','America']
```

## 类型强转

cython类型强转和c语言相似，只是把圆括号转换为尖括号：

```
cdef int *ptr_i = <int*>v
```

如果我们不确定要转换的类型或者想要cython在转换前检查类型，应该使用检查转换操作符：

```
def cast_to_list(a):
    cdef list cast_list = <list?>a #多加了一个问号
    print(type(a))
    print(type(cast_list))
```

## struct

c语言中定义结构体为：

```
struct mystruct{
    int a;
    float b;
};
```

而在cython中，使用cdef来进行结构的声明：

```
cdef struct mystruct:
    int a
    float b

#or
ctypedef struct mystruct:
    int a
    float b
```

不管是用cdef还是ctypedef定义结构，都可以使用这个结构体来声明其他变量：

```
cdef mystruct zz
```

初始化结构体有三种方法：

```
#1.作为参数传进
cdef mystruct m = mystruct(20,11.1)
cdef mystruct m = mystruct(a=20, b=11.1)
#2.作为元素传进
cdef mystruct zz
zz.a = 20
zz.b = 11.1
#3.作为字典传进
cdef mystruct zz = {'a':20,'b':11.1}
```

其实，在Python中使用`type(zz)`查看zz的类型就是一个字典。

cython中使用点号取结构体中的元素，而不论这个结构体是不是指针：

```
cdef st_t *p_st = make_struct()
cdef int a_doubled = p_st.a + p_st.a
```

只要在C中使用箭头操作，在cython中都要使用点操作符。

cython中不支持嵌套和匿名的内部struct或union。

## ctypedef

C中的typedef给出一个类型的别名，例如在c中定义一个结构体：

```
struct foobar{
    int x;
    char* y;
};
typedef struct foobar foobar_t;
```

在cython中可以这样定义：

```
cdef struct foobar:
    int x
    char* y
ctypedef foobar foobar_t
```

在已经定义好的结构体后，对该结构体起别名或作为参数传递函数中都不再需要写`struct`，例如：

```
void myfunc(struct foobar* x) #wrong
void myfunc(foobar* x) #right
```

定义指针的别名或指针函数都一样：

```
ctypedef int* int_ptr

ctypedef void (*func)(int)
```

**注意：`ctypedef`声明必须出现在文件域，并且不能用于在函数内或其他局部作用域声明类型名称**。

## 字符串处理

Python3的字符串处理：

```
type("a".encode("uft-8"))
## <class 'bytes'>
type("a".decode("UTF-8"))
#Traceback (most recent call last): File "<stdin>", line 1, in <module> AttributeError: 'str' object has no attribute 'decode'
```

即python3的字符串就是unicode类型，

python3要传递一个字符串到C，则需要为bytes类型，需要使用编码器(`.encode("UTF-8")`)编码为bytes类型。

> - 字符串要传到C中需要使用`.encode()`转为bytes类型（python2 string就是bytes）
- 从C中要接受字符串时需要用`.decode()`把bytes类型转化为string类型（Python3的string类型就是unicode）


## Cython .pxd文件

要连接C代码，最好使用.pxd文件声明所有的C/C++接口，这表示为python的外部声明，.pxd文件内容可以使用cimport直接导入到.pyx文件中。

## Cython wrapping C++

cython支持C++：

> - C++对象可以使用`new`和`del`关键字进行动态分配
- C++对象能够使用栈分配内存
- C++类能使用关键字`cppclass`进行声明
- 支持模板类和函数
- 支持重载函数
- 支持C++操作符重载


### 封装C++文件一般步骤

> - 在`setup.py`里设定语言为C++语言
- 创建一个或多个`.pxd`文件，其内有`cdef extern from`块和C++命名空间（如果有的话）。在这些块中：1. 声明类（cdef cppclass）2. 声明公共名称（变量，方法， 构造器等）


### namespace

在.pxd文件中应用C++，使用namespace:

```
cdef extern from "header.h" namespace "mynamespace":
    ...
```

### class

**步骤1**

假设有使用C++计算一个积分， 其`integrate.h`为：

```
#ifndef INTEGRATE_H
#define INTEGRATE_H

namespace c_integrate{
	class Integ{
	public:
		Integ();
		~Integ();
        double integrate(double ub, double lb, double(*func)(double x), int n);
		      		
};
double fun(double x); 
}

#endif //INTEGRATE_H
```

其`integrate.cpp`内容为：

```
#include <iostream>
#include "integrate.h"

double c_integrate::Integ::integrate(double ub, double lb, double(*func)(double x), int n){
	double dx = (ub - lb) / n;
	double s = 0;
	double i;
			
	for (i = lb; i <= ub; i+=dx){
		s += func(i)*dx;
	}
	return s;
}

c_integrate::Integ::Integ(){};
c_integrate::Integ::~Integ(){};

double c_integrate::fun(double x){
    return x*x;
}
```

**步骤2**

现在需要在cython中声明一些属性和方法，把这些声明放在`integrate.pxd`文件里，它基本上就是`integrate.h`的复制粘贴，但要符合cython的语法规则：

```
cdef extern from "integrate.h" namespace "c_integrate":
    cdef cppclass Integ:
        Integ() except +
        double integrate(double ub, double lb, double(*func)(double x), int n)

    double fun(double x)
```

在pxd文件中声明时注意：

> - 如果包含命名空间的话命名空间，如：`cdef extern from "integrate.h" namespace "c_integrate":`
- 定义类的话使用关键词`cppclass`
- 构造器可以加上`except+`, 这样如果C++初始化时出错，则cython会捕捉到并返回Python的异常，如果没有`except+`，C++出错时cython无法处理
- 方法或属性去掉分号


**步骤3**

然后创建.pyx文件，命名为`itgr.pyx`，当然也可以命名为`integrate.pyx`，但是要注意[命名冲突问题](http://docs.cython.org/en/latest/src/userguide/external_C_code.html#resolve-conflicts)。

```
#-*-coding: utf-8-*-
#distutils: language = c++

from integrate cimport Integ
from integrate cimport fun
	
cdef class PyInteg():
    cdef Integ c_integ
	
    def __cinit__(self):
        self.c_integ = Integ()
		
    def pyintegrate(self, double ub, double lb, int n):
        return self.c_integ.integrate(ub, lb, fun, n)
		
    def f(self, double x):
        return x*x
```

**注意：**

> - 第二行`#distutils = language = c++`表明`.pyx`文件是使用C++编译的
- Cython要初始化`cdef`定义的C++类时需要一个默认的无参构造器，只要有默认的构造器，它可声明一个栈分配的对象，例如`cdef Integ c_integ`。如果没有显式声明构造器，声明栈分配的对象会出错。


如果没有声明默认的构造器怎么办？可以使用如下方法：

```
#-*-coding: utf-8-*-
#distutils: language = c++

from integrate cimport Integ
from integrate cimport fun

cdef class PyInteg():
    cdef Integ* c_integ
	
    def __cinit__(self):
        self.c_integ = new Integ()

    def pyintegrate(self, double ub, double lb, int n):
        return self.c_integ.integrate(ub, lb, fun, n)

    def __dealloc__(self):
         del self.c_integ
```

注意：

> - 没有显式声明构造器则使用`cdef Integ* c_integ`进行堆分配对象（当然，即使显式声明构造器也可以这样声明）
- `__cinit__`和`__dealloc__`:保证Python实例的创建和删除只被调用一次。在`__cinit__`中对Integ实例化，在`__dealloc__`中进行删除
- 需要注意关键字`del`只能删除在堆上建立的对象，如果对象时建立在栈上，使用`del`会出现错误:`itgr.pyx:13:6: Deletion of non-heap C++ object`。


**步骤4**

设置`setup.py`：

```
#-*-coding: utf-8 -*-

from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext_modules = [Extension("itgr", sources=["itgr.pyx","integrate.cpp"],
    language="c++")]

setup(
    name="itgr",
    ext_modules = cythonize(ext_modules)
)
```

最后运行`python setup.py build_ext --inplace`，待代码生成完成后直接可以调用`itgr`包了。

[Cython基础 ppt](https://github.com/Alxe1/CythonStudy)



