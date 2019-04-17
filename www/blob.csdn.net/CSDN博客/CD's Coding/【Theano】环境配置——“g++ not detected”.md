# 【Theano】环境配置——“g++ not detected” - CD's Coding - CSDN博客





2017年02月07日 00:24:03[糖果天王](https://me.csdn.net/okcd00)阅读数：11011








## 0x00 前言

不管想要做些什么，配置环境总是最让人头疼的部分…… 

看了半天的Theano，终于打算跑跑程序瞧瞧了…… 

谁知道新的一轮配置才刚刚开始…… 

提示：懒得看我那么多废话的可以直接点[这里](http://blog.csdn.net/okcd00/article/details/54828747#t6)跳到配置方法
## 0x01 WARNING (theano.configdefaults)

```
# 尝试运行
$ python example.py

# 报错文本
WARNING (theano.configdefaults): g++ not available, if using conda: `conda install m2w64-toolchain`
WARNING (theano.configdefaults): g++ not detected ! Theano will be unable to execute optimized C-implementations (for both CPU and GPU) and will default to Python implementations. Performance will be severely degraded. To remove this warning, set Theano flags cxx to an empty string.
```

```
# 尝试 Step1 (那装个mingw试试？)
$ conda install mingw libpython

# 反馈文本
Fetching package metadata .........
Solving package specifications: ..........

Package plan for installation in environment C:\Program Files\Anaconda2:

The following packages will be downloaded:

    package                    |            build
    ---------------------------|-----------------
    conda-env-2.6.0            |                0          498 B
    mingw-4.7                  |                1        56.1 MB
    libpython-2.0              |           py27_0          30 KB
    requests-2.12.4            |           py27_0         755 KB
    pyopenssl-16.2.0           |           py27_0          68 KB
    conda-4.3.8                |           py27_0         522 KB
    ------------------------------------------------------------
                                           Total:        57.4 MB

The following NEW packages will be INSTALLED:

    conda-env: 2.6.0-0       (copy)
    libpython: 2.0-py27_0    (copy)
    mingw:     4.7-1         (copy)

The following packages will be UPDATED:

    conda:     4.2.9-py27_0  --> 4.3.8-py27_0  (copy)
    pyopenssl: 16.0.0-py27_0 --> 16.2.0-py27_0 (copy)
    requests:  2.11.1-py27_0 --> 2.12.4-py27_0 (copy)

# 报错文本
CondaIOError: IO error: Missing write permissions in: C:\Program Files\Anaconda2
#
# You don't appear to have the necessary permissions to install packages
# into the install area 'C:\Program Files\Anaconda2'.
# However you can clone this environment into your home directory and
# then make changes to it.
# This may be done using the command:
#
# $ conda create -n my_root --clone=C:\Program Files\Anaconda2
```

## 0x02 CondaIOError: IO error

```
# 尝试 Step2（那按照提示试试？）
$ conda create -n my_root --clone=C:\Program Files\Anaconda2

# 报错文本
TooManyArgumentsError: Too many arguments: did not expect any arguments for --clone. Got 1 argument (Files\Anaconda2) and expected 0.

# 尝试 Step3（是不是应该框起来？）
$ conda create -n my_root --clone="C:\Program Files\Anaconda2"
```

```
# 反馈文本
Source:      C:\Program Files\Anaconda2
Destination: C:\Users\Work\.conda\envs\my_root
The following packages cannot be cloned out of the root environment:
 - conda-4.2.9-py27_0
 - conda-build-2.0.2-py27_0
Packages: 181
Files: 22
WARNING conda.lock:touch(53): Failed to create lock, do not run conda in parallel processes [errno 13]
An unexpected error has occurred.
Please consider posting the following information to the
conda GitHub issue tracker at:

    https://github.com/conda/conda/issues

Current conda install:

               platform : win-64
          conda version : 4.2.9
       conda is private : False
      conda-env version : 4.2.9
    conda-build version : 2.0.2
         python version : 2.7.12.final.0
       requests version : 2.11.1
       root environment : C:\Program Files\Anaconda2  (read only)
    default environment : C:\Program Files\Anaconda2
       envs directories : C:\Users\Work\.conda\envs
                          C:\Program Files\Anaconda2\envs
          package cache : C:\Users\Work\.conda\envs\.pkgs
                          C:\Program Files\Anaconda2\pkgs
           channel URLs : https://repo.continuum.io/pkgs/free/win-64/
                          https://repo.continuum.io/pkgs/free/noarch/
                          https://repo.continuum.io/pkgs/pro/win-64/
                          https://repo.continuum.io/pkgs/pro/noarch/
                          https://repo.continuum.io/pkgs/msys2/win-64/
                          https://repo.continuum.io/pkgs/msys2/noarch/
            config file : None
           offline mode : False

` C:\Program Files\Anaconda2\Scripts\conda-script.py create -n my_root --clone=C:\Program Files\Anaconda2`


    Traceback (most recent call last):
      File "C:\Program Files\Anaconda2\lib\site-packages\conda\exceptions.py", line 473, in conda_exception_handler
        return_value = func(*args, **kwargs)
        ......
        makedirs(head, mode)
      File "C:\Program Files\Anaconda2\lib\os.py", line 157, in makedirs
        mkdir(name, mode)
    WindowsError: [Error 5] : u'C:\\Program Files\\Anaconda2\\pkgs\\menuinst-1.4.1-py27_0.tmp'
```

```
# 尝试 Step4 (哦路径有个空格，会不会是要这样加双引号？)
conda create -n my_root --clone=C:\"Program Files"\Anaconda2

# 报错文本
CondaValueError: Value error: prefix already exists: C:\Users\Work\.conda\envs\my_root

# 纠结的去碎觉了
```

## 0x03 Missing write permissions

所以说来说去还是因为没有权限对吧…… 

那我就把这文件夹的权限开放好了：

![属性更改](https://img-blog.csdn.net/20170204120119368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# 好的，正常了

$ conda install mingw libpython
Fetching package metadata .........
Solving package specifications: ..........

Package plan for installation in environment C:\Program Files\Anaconda2:

The following packages will be downloaded:

    package                    |            build
    ---------------------------|-----------------
    mingw-4.7                  |                1        56.1 MB
    libpython-2.0              |           py27_0          30 KB
    requests-2.12.4            |           py27_0         755 KB
    pyopenssl-16.2.0           |           py27_0          68 KB
    conda-4.3.9                |           py27_0         525 KB
    ------------------------------------------------------------
                                           Total:        57.4 MB

The following NEW packages will be INSTALLED:

    conda-env: 2.6.0-0
    libpython: 2.0-py27_0
    mingw:     4.7-1

The following packages will be UPDATED:

    conda:     4.2.9-py27_0  --> 4.3.9-py27_0
    pyopenssl: 16.0.0-py27_0 --> 16.2.0-py27_0
    requests:  2.11.1-py27_0 --> 2.12.4-py27_0

Proceed ([y]/n)?
```

```
# 感动QAQ
Fetching packages ...
mingw-4.7-1.ta 100% |###############################| Time: 0:06:54 141.86 kB/s
libpython-2.0- 100% |###############################| Time: 0:00:00 124.08 kB/s
requests-2.12. 100% |###############################| Time: 0:00:12  64.36 kB/s
pyopenssl-16.2 100% |###############################| Time: 0:00:00 161.22 kB/s
conda-4.3.9-py 100% |###############################| Time: 0:00:04 109.79 kB/s
Extracting packages ...
[      COMPLETE      ]|##################################################| 100%
Unlinking packages ...
[      COMPLETE      ]|##################################################| 100%
Linking packages ...
[      COMPLETE      ]|##################################################| 100%
```

既然知道怎么用conda装东西了，回到之前遇到的问题：
`WARNING (theano.configdefaults): g++ not available, if using conda: `conda install m2w64-toolchain``
```
# 太好了，终于成功啦QAQ
$ conda install m2w64-toolchain
Fetching package metadata ...........
Solving package specifications: .

Package plan for installation in environment C:\Program Files\Anaconda2:

The following NEW packages will be INSTALLED:

    m2w64-binutils:                 2.25.1-5
    m2w64-bzip2:                    1.0.6-6
    m2w64-crt-git:                  5.0.0.4636.2595836-2
    m2w64-gcc:                      5.3.0-6
    m2w64-gcc-ada:                  5.3.0-6
    m2w64-gcc-fortran:              5.3.0-6
    m2w64-gcc-libgfortran:          5.3.0-6
    m2w64-gcc-libs:                 5.3.0-7
    m2w64-gcc-libs-core:            5.3.0-7
    m2w64-gcc-objc:                 5.3.0-6
    m2w64-gmp:                      6.1.0-2
    m2w64-headers-git:              5.0.0.4636.c0ad18a-2
    m2w64-isl:                      0.16.1-2
    m2w64-libiconv:                 1.14-6
    m2w64-libmangle-git:            5.0.0.4509.2e5a9a2-2
    m2w64-libwinpthread-git:        5.0.0.4634.697f757-2
    m2w64-make:                     4.1.2351.a80a8b8-2
    m2w64-mpc:                      1.0.3-3
    m2w64-mpfr:                     3.1.4-4
    m2w64-pkg-config:               0.29.1-2
    m2w64-toolchain:                5.3.0-7
    m2w64-tools-git:                5.0.0.4592.90b8472-2
    m2w64-windows-default-manifest: 6.4-3
    m2w64-winpthreads-git:          5.0.0.4634.697f757-2
    m2w64-zlib:                     1.2.8-10
    msys2-conda-epoch:              20160418-1

The following packages will be UPDATED:

    anaconda:                       4.2.0-np111py27_0    --> custom-py27_0

Proceed ([y]/n)? y

msys2-conda-ep 100% |###############################| Time: 0:00:00 187.73 kB/s
m2w64-gmp-6.1. 100% |###############################| Time: 0:00:03 214.94 kB/s
m2w64-gmp-6.1. 100% |###############################| Time: 0:00:09  76.21 kB/s
m2w64-gmp-6.1. 100% |###############################| Time: 0:00:08  79.34 kB/s
m2w64-headers- 100% |###############################| Time: 0:01:21  72.44 kB/s
m2w64-isl-0.16 100% |###############################| Time: 0:00:09  72.56 kB/s
m2w64-libiconv 100% |###############################| Time: 0:00:33  45.89 kB/s
m2w64-libmangl 100% |###############################| Time: 0:00:00  53.69 kB/s
m2w64-libwinpt 100% |###############################| Time: 0:00:00  47.28 kB/s
m2w64-make-4.1 100% |###############################| Time: 0:00:02  48.44 kB/s
m2w64-windows- 100% |###############################| Time: 0:00:00 434.62 kB/s
anaconda-custo 100% |###############################| Time: 0:00:00  59.71 kB/s
m2w64-crt-git- 100% |###############################| Time: 0:00:28 122.83 kB/s
m2w64-gcc-libs 100% |###############################| Time: 0:00:01 112.73 kB/s
m2w64-mpfr-3.1 100% |###############################| Time: 0:00:03  92.55 kB/s
m2w64-pkg-conf 100% |###############################| Time: 0:00:06  71.01 kB/s
m2w64-gcc-libg 100% |###############################| Time: 0:00:07  43.77 kB/s
m2w64-mpc-1.0. 100% |###############################| Time: 0:00:01  41.31 kB/s
m2w64-winpthre 100% |###############################| Time: 0:00:00  47.91 kB/s
m2w64-gcc-libs 100% |###############################| Time: 0:00:07  73.92 kB/s
m2w64-bzip2-1. 100% |###############################| Time: 0:00:00 113.92 kB/s
m2w64-tools-gi 100% |###############################| Time: 0:00:05  58.54 kB/s
m2w64-zlib-1.2 100% |###############################| Time: 0:00:07  28.71 kB/s
m2w64-binutils 100% |###############################| Time: 0:07:49  99.05 kB/s
m2w64-gcc-5.3. 100% |###############################| Time: 0:11:21  63.30 kB/s
m2w64-gcc-5.3. 100% |###############################| Time: 0:11:25  62.89 kB/s
m2w64-gcc-ada- 100% |###############################| Time: 0:11:35  50.55 kB/s
m2w64-gcc-fort 100% |###############################| Time: 0:03:51  46.45 kB/s
m2w64-gcc-objc 100% |###############################| Time: 0:05:56  44.59 kB/s
m2w64-toolchai 100% |###############################| Time: 0:00:00   3.57 kB/s
```

## 0x04 ImportError: cannot import name gof

```python
# 开心的import试试看~

>>> import theano
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "C:\Python27\lib\site-packages\theano\__init__.py", line 66, in <module>
    from theano.compile import (
  File "C:\Python27\lib\site-packages\theano\compile\__init__.py", line 10, in <module>
    from theano.compile.function_module import *
  File "C:\Python27\lib\site-packages\theano\compile\function_module.py", line 18, in <module>
    from theano import config, gof
ImportError: cannot import name gof

# 。。。。。。
```

## 0x05 环境配置：从入门到放弃

小标题说明一切……  

所以我装了半天的mingw还是被认作是cygwin是么…… 

（绝望脸）反正我也只是为了练习Theano的语法，不纠结那么多用起来再说……
于是……投奔了实验室的ssh远程CentOS操作，虽然只要两三分钟不输入指令就会断线……但好歹能用啊……

## 0x06 后记

因为看到有人评论了这篇文章，才突然想起来，oh，这里有个坑忘了填，现在在这里补充一下。 

想了想不如就直接写个从零开始配置Theano的流程 (Thx for @caoyixuan) 

此处使用Anaconda平台来为我们的安装进行辅助：- Windows 
- Install `Anaconda`: download package from [https://anaconda.org/](https://anaconda.org/)
- cmd: >`conda install mingw libpython`
- add environment  variables:  
- D:\Anaconda2;
- D:\Anaconda2\Library\bin;
- D:\Anaconda2\MinGW\bin;
- D:\Anaconda2\MinGW\x86_64-w64-mingw32\lib;
- D:\Anaconda2\Scripts;

- cmd: >`pip install theano`
- you should get/create a `.theanorc.txt` in your user folder:  
- c:\Users\name.theanorc.txt
- If you have GPU and want to make use of it , follows the installation instruction in [http://deeplearning.net/software/theano/install.html](http://deeplearning.net/software/theano/install.html)

- Linux 
- Install `Anaconda`: download package from [https://anaconda.org/](https://anaconda.org/)
- make sure anacoda is your default python path
- cmd: >`pip install theano`
- you should get/create a `.theanor file` in your user folder: 
- ~/.theanorc


## 0xFF Theano Scripts

开工开工…… 开始看基础

```
Python 2.7.12 (v2.7.12:d33e0cf91556, Jun 27 2016, 15:24:40) [MSC v.1500 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.

>>> import theano
WARNING (theano.configdefaults): g++ not available, if using conda: `conda install m2w64-toolchain`
WARNING (theano.configdefaults): g++ not detected ! Theano will be unable to execute optimized C-implementations (for both CPU and GPU) and will default to Python implementations. Performance will be severely degraded. To remove this warning, set Theano flags cxx to an empty string.
>>> import theano.tensor as T
>>> from theano import function, pp, printing, scan, shared
>>> import numpy as np
>>>
>>>
>>> '''Define a function'''
'Define a function'
>>> # 1. theano function
... x, y, a, b = T.fscalars('x', 'y', 'a', 'b')
>>> a = x + y
>>> b = x - y
>>> f_add = theano.function(inputs=[x, y], outputs=a)
>>> f_add_minus = theano.function(inputs=[x, y], outputs=[a, b])
>>>
>>> x1 = 10.
>>> y1 = 1.
>>> a1 = f_add(x1, y1)
>>> a2, b2 = f_add_minus(x1, y1)
>>> print a1, a2, b2
11.0 11.0 9.0
>>>
>>> # 2. python function
... def f_add(x, y):
...     a = x + y
...     return a
...
>>> def f_add_minus(x, y):
...     a = x + y
...     b = x - y
...     return a, b
...
>>> x1 = 10.
>>> y1 = 1.
>>> a1 = f_add(x1, y1)
>>> a2, b2 = f_add_minus(x1, y1)
>>> print a1, a2, b2
11.0 11.0 9.0
>>>
>>>
>>>
>>> ''' Derivatives '''
' Derivatives '
>>> # Derivative
... x = T.scalar('x', dtype='float32')
>>> y = x ** 2
>>> gy = T.grad(y, x)
>>> f = theano.function([x], gy)
>>> print f(4)
8.0
>>>
>>> # partial derivative
... x = T.scalar('x', dtype='float32')
>>> a = T.scalar('a', dtype='float32')
>>> y = x * a
>>> gy = T.grad(y, [x, a])
>>> f = theano.function([x, a], gy)
>>> print f(4, 3)
[array(3.0, dtype=float32), array(4.0, dtype=float32)]
>>>
>>>
>>> ''' Debugging tricks '''
' Debugging tricks '
>>> x = T.scalar('x',dtype='float32')
>>> y = x ** 2
>>> gy = T.grad(y, x)
>>> f = function([x], gy)
>>>
>>> # using pp
... pp(gy)   # before optimization
'((fill((x ** TensorConstant{2}), TensorConstant{1.0}) * TensorConstant{2}) * (x ** (TensorConstant{2} - TensorConstant{1})))'
>>> pp(f.maker.fgraph.outputs[0])  # after optimization
'(TensorConstant{2.0} * x)'
>>>
>>> # using printing.debugprinting
... printing.debugprint(gy)  # before optimization
Elemwise{mul} [id A] ''
 |Elemwise{mul} [id B] ''
 | |Elemwise{second,no_inplace} [id C] ''
 | | |Elemwise{pow,no_inplace} [id D] ''
 | | | |x [id E]
 | | | |TensorConstant{2} [id F]
 | | |TensorConstant{1.0} [id G]
 | |TensorConstant{2} [id F]
 |Elemwise{pow} [id H] ''
   |x [id E]
   |Elemwise{sub} [id I] ''
     |TensorConstant{2} [id F]
     |InplaceDimShuffle{} [id J] ''
       |TensorConstant{1} [id K]
>>> printing.debugprint(f.maker.fgraph.outputs[0])  # after optimization
Elemwise{mul,no_inplace} [id A] ''
 |TensorConstant{2.0} [id B]
 |x [id C]
>>>
>>> # printing.Print
... # recall that functions defined before do not print internal variables
... x = T.scalar('x',dtype='float32')
>>> xp2 = x + 2
>>> xp2_printed = printing.Print('this is xp2:')(xp2)
>>> xp2m2 = xp2_printed * 2
>>> f = function([x], xp2m2)
>>> f(20)
this is xp2: __str__ = 22.0
array(44.0, dtype=float32)
>>>
>>> # try to make this work
... x = T.scalar('x',dtype='float32')
>>> y = x ** 2
>>> yprint = printing.Print('this is y:')(y)
>>> gy = T.grad(yprint, x)
>>> f = function([x], gy)
>>> # alternatively:
... # p = printing.Print('y')
... # yprint = p(y)
...
>>>
>>> ''' shared variable '''
' shared variable '
>>> state = shared(0.)
>>> inc = T.iscalar('inc')
>>> accumulator = function([inc], state, updates=[(state, state+inc)])
>>> print state.get_value()
0.0
>>> z = accumulator(10)
>>> print z
0.0
>>> print state.get_value()
10.0
>>>
>>>
>>> ''' Loop '''
' Loop '
>>> # python for loop
... import numpy
>>> A = numpy.array([1, 2], dtype='float32')
>>> k = 5
>>> result = [numpy.array([1,1])]
>>> def mul(a, b): return a*b
...
>>> for i in range(k):
...     result.append(mul(result[-1], A))
...
>>> print result[-1]
[  1.  32.]
>>>
>>> # theano scan
... k = T.scalar("k", dtype='int32')
>>> A = T.vector("A", dtype='float32')
>>> def mul(a, b): return a*b
...
>>> result, updates = theano.scan(
...                 fn=mul,
...                 outputs_info=T.ones_like(A),
...                 non_sequences=A,
...                 n_steps=k)
>>>
>>> power = theano.function(
...                 inputs=[A,k],
...                 outputs=result[-1],
...                 updates=updates)
>>>
>>> A_val = numpy.array([1, 2], dtype='float32')
>>> k_val = 5
>>> r = power(A_val, 5)
>>> print r
[  1.  32.]
>>>
>>>
>>> # calculate polynomial
... # that is, a0 * x^0 + a1 * x^1 + a2 * x^2 + ... + an * x^n
... coefficients = theano.tensor.vector("coefficients", dtype='float32')
>>> x = T.scalar("x", dtype='float32')
>>>
>>> max_coefficients_supported = 10000
>>>
>>> def cumulative_poly(coeff, power, prior_sum, x):
...     return prior_sum + coeff * (x ** power)
...
>>> # Generate the components of the polynomial
... zero = np.asarray(0., dtype='float32')
>>> full_range=theano.tensor.arange(max_coefficients_supported, dtype='float32')
>>> results, updates = theano.scan(fn=cumulative_poly,
...                                 outputs_info=T.as_tensor_variable(zero),
...                                 sequences=[coefficients, full_range],
...                                 non_sequences=x)
>>>
>>> polynomial = results[-1]
>>> calculate_polynomial = theano.function(inputs=[coefficients, x],
...                                      outputs=polynomial)
>>>
>>> test_coeff = np.asarray([1, 0, 2], dtype=np.float32)
>>> print(calculate_polynomial(test_coeff, 3))
19.0
```








