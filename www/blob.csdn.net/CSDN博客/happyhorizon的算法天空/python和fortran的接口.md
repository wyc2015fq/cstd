# python和fortran的接口 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:10:22[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：3108

由于历史缘故，很多成熟的计算代码都是用fortran写成的。在python中调用fortran代码，要用到f2py这个程序。现在该项目已经合并到numpy中了，先安装python再装好numpy，就可以使用f2py。
# 一个简单的例子：
foo.f90
```
subroutine hello (a)
  integer a
  write(*,*)'Hello from Fortran90!!!', a 
end subroutine hello
```
用终端编译：
```
f2py -m foo -c foo.f90
```
在python控制台中运行：
```
$ python
>>> import foo
>>> foo.hello(15)
 Hello from Fortran90!!!     15
```
# 明确定义接口的参数：
另外一个例子，非常明确地定义了接口的输入、输出参数：
```
!SUBROUTINE        
      SUBROUTINE ADDSUB(A,B,C,D)  
      IMPLICIT NONE  
      DOUBLE PRECISION A,B,C,D  
!f2py intent(in) :: A,B  
!f2py intent(out) :: C,D  
      C = A + B  
      D = A - B  
      print*, "ADDSUB From Fortran!"  
      print*, "ADD=",C  
      print*, "SUB=",D  
      RETURN  
      END
```
注意这两行的代码:
```
!f2py intent(in) :: A,B  
!f2py intent(out) :: C,D
```
对于Fortran只是注释,但对于f2py却很重要,相当于"签名".
注意签名的注释前面不能有空格!
当然也可以写成如下的形式:
```
Cf2py intent(in) :: A,B  
Cf2py intent(out) :: C,D
```
下面开始编译Fortran代码为python模块,打开CMD窗口,输入如下命令:
```
f2py -m testfortran -c testfortran.f90
```
会在当前目录下生成testfortran.pyd的文件. 下面就可以再python中使用这个模块了:
```
In [1]: import testfortran  
In [2]: print testfortran.__doc__  
This module 'testfortran' is auto-generated with f2py (version:2).  
Functions:  
  c,d = addsub(a,b)   
In [3]: x=testfortran.addsub(4,9)  
ADDSUB From Fortran!  
ADD= 13.000  
SUB= -5.000  
In [4]: x  
Out[4]: (13.0, -5.0)
```
# 接口参数是数组：
接口参数是数组的情况比较复杂。需要Python中必须确定数组的维数，并且NumPy定义的数组存储方式必须是Fortran的按列存储。
实现方式：
在Python中使用NumPy定义Fortran方式存储的二维数组，利用ndpointer定义数组类型和维数，用ctypes模块调用其data属性将数组首地址传入将二维数组的首地址和维数信息传入Fortran中进行计算并返回。
fortran代码：py2f90.f90
```
module py2f90
use,intrinsic::iso_c_binding
implicit none
contains
    subroutine transferMat2For(matrix,n1,n2)bind(c,name='array2py')
    implicit none
    integer(c_int),intent(in),value::n1,n2
    real(c_float),intent(out)::matrix(n1,n2)
    integer::i,j
    ! initialize matrix
    matrix = 0.0E0
    ! loop
    do i=1,n1
        do j=1,n2
            matrix(i,j) = real(i,4)*1.E1+real(j,4)*2.E0
            write(*,"('Row:',i4,1x,'Col:',i4,1x,'Value:',1x,F5.2)")i,j,matrix(i,j)
        enddo
    enddo
    return
    end subroutine
end module
```
test_mat.f90:
```
program test
use py2f90
implicit none
real(kind=4)::aa(4,5)
call transferMat2For(aa,4,5)
end program
```
python调用代码：test_mat.py
```
#! /usr/bin/env python
#coding=utf-8
import numpy as np
from numpy.ctypeslib import load_library,ndpointer
from ctypes import c_int
# shape of 2d array
n1,n2 = 2, 5
# create an empty 2d array
data = np.empty(shape=(n1,n2),dtype='f4',order='f')
flib = load_library("test_mat","./")
flib.argtypes = [ndpointer(dtype='f4',ndim=2),c_int,c_int]
flib.array2py(data.ctypes.data,n1,n2)
print("*"*80)
print(data)
```
终端编译指令：
```
f2py -m test_mat -c test_mat.f90 py2f90.f90
```
在终端输入：`python test_mat.py`，得到：
![image.png](https://upload-images.jianshu.io/upload_images/4685306-a280d57d7f9bcfa5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
参考：
[f2py::演示在python中如何使用Fortran代码](http://blog.csdn.net/rumswell/article/details/7377339)
[Python调用C/Fortran混合的动态链接库-下篇](http://www.cnblogs.com/pasuka/p/4012508.html)
[原来Numpy的array可以很方便地和ctypes结合起来使用](http://blog.sina.com.cn/s/blog_4513dde60100no4d.html)
