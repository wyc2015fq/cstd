# ubuntu14.04安装深度学习Theano框架及错误解决 - 站在巨人的肩膀上coding - CSDN博客





2018年05月03日 23:09:18[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：163








## 环境
- 操作系统：ubuntu14.04
- Python：2.7.6
- 需要联网

## 相关库简介
- ***BLAS***（Basic Linear Algebra Subprograms）是基础线性代数子程序库，里面拥有大量已经编写好的关于线性代数运算的程序;
- ***LAPACK*** （Linear Algebra PACKage）包含了求解科学与工程计算中最常见的数值线性代数问题，如求解线性方程组、线性最小二乘问题、特征值问题和奇异值问题等;
- ***ATLAS***是python下的一个线性代数库，是基于另外两个线性代数库BLAS和lapack的；
- ***NumPy***提供了一个在python中做科学计算的基础库，它重在数值计算，甚至可以说是用于多维数组处理的库；
- ***SciPy***是基于numpy，提供了一个在python中做科学计算的工具集，也就是说它是更上一个层次的库；
- ***Theano***则是基于NumPy以及SciPy的一个更高级的用于科学计算的库。

## 相关库的关系
- 要安装Theano，就需要先安装好numpy和scipy；
- 要安装numpy和scipy，就需要ATLAS；
- 要安装ATLAS，就需要安装BLAS和LAPACK;

## 相关库的安装顺序
- 安装顺序：*`BLAS → LAPACK → ATLAS → numpy → scipy → Theano`*

## 检查numpy和scipy是否通过测试
- 说明：如果你的numpy和scipy是通过`apt-get`安装的，那么它们的单元测试可能会通不过！！！（我在安装过程中没有通过测试，scipy出现了**Error**）
- 如果numpy或scipy不能通过测试，就需要卸载，然后重新按照本文介绍的顺序安装。

```bash
# 检查numpy是否通过测试
python -c "import numpy;numpy.test()"
```
- 
如果numpy通过测试，会出现如下图所示的结果。注意最后一行***errors=0 failures=0***。如果没有通过测试，需要卸载并重新安装。
![numpy测试结果](https://img-blog.csdn.net/20150726185255050)

- 
如果scipy通过测试，会出现如下图所示的结果。注意最后一行***errors=0 failures=0***。如果没有通过测试，需要卸载并重新安装。
![scipy通过测试](https://img-blog.csdn.net/20150726185208417)

- 
注意：scipy很可能会出现测试错误，比如我安装过程中出现下图所示的错误。 
![scipy测试错误](https://img-blog.csdn.net/20150726185356526)


## 卸载numpy和scipy

```bash
# 卸载numpy
sudo apt-get remove python-numpy

# 卸载scipy
sudo apt-get remove python-scipy
```

## 安装各种包

```bash
# 安装gfortran,后面编译过程中会用到
sudo apt-get install gfortran
# 安装blas,Ubuntu下对应的是libopenblas，其它操作系统可能需要安装其它版本的blas——这是个OS相关的。
sudo apt-get install libopenblas-dev
# 安装lapack，Ubuntu下对应的是liblapack-dev，和OS相关。
sudo apt-get install liblapack-dev
# 安装atlas，Ubuntu下对应的是libatlas-base-dev，和OS相关。
sudo apt-get install libatlas-base-dev
```

## 安装numpy和scipy
- 使用pip安装numpy和scipy
- 安装pip的命令：**`sudo apt-get install python-pip`**
- 注意：一定要在安装完lapack/blas之后，再安装numpy和scipy。否则，会出现错误***`no lapack/blas resources found`***
- 安装numpy和scipy的命令如下所示。

```bash
# 安装numpy
sudo pip install numpy
# 测试numpy
# 测试通过才能进行下一步～～
python -c "import numpy;numpy.test()"

# 安装scipy
sudo pip install scipy
# 测试scipy
# 测试通过才能进行下一步～～
python -c "import scipy;scipy.test()"
```

```bash

```

```bash
#注意这里可能出现找不到pytest，sudo pip install pytest
```

```bash

```

```bash

```

```bash

```

```bash

```

```bash

```

```bash

```

```bash

```

```bash

```

```bash

```

```bash

```

# ```bash```安装各其他库


```bash

```

apt-get install python-dev
apt-get install python-pip
apt-get install python-nose
apt-get install g++
apt-get install git 
# 安装与测试theano
pip install Theano
python -c "import theano;theano.test()"
如果报错，可能是权限的问题，用sudo 打开python 即可
注意：这里可能出现错误
ImportError: No module named parameterized
可使用sudo pip install parameterized 来安装，然后再重新执行 python -c "import theano;theano.test()"

```bash

```

```bash

```

# 其他错误

## ~~1、在实际运行时候还可能出现~~

## ~~ImportError: cannot import name downsample【无效的解决方式】~~```bash```

from theano.tensor.signal.pool import downsample

ImportError: cannot import name downsample

解决方案来自大牛的回复 [[http://blog.csdn.net/u012162613/article/details/43277187#comments](http://blog.csdn.net/u012162613/article/details/43277187#comments) ]： 在较早的版本中，可能会使用from theano.tensor.signal.downsample import max_pool_2d。根据运行提示，更新到 from theano.tensor.signal.pool import pool_2d 中了。

查看了一下theano下面的包，发现在signal下面没有了downsample了，只有pool和conv了。如果以后再用downsample的时候，很可能就是调用这两个包里面的东西，改一下就好了。感叹一下，计算机变的真快。

把    from theano.tensor.signal.pool import downsample注释掉,改为

```
from theano.tensor.signal.pool import pool_2d

然后，删除原来的pool.pyc文件，重新用修改后的pool.py文件生成
```
python -m py_compile file.py  
```bash

```
``
## ~~2、问题1更本质的解决办法【在使用SCNN时候会出现问题4，可以先这样解决后再重装theano】~~

## https://blog.csdn.net/lyrassongs/article/details/78158621


写于Theano宣布停止更新第五天。本来是想调一下ntm-one-shot的代码的，地址如下https://github.com/tristandeleu/ntm-one-shot。但是在走到venv/bin/py.test mann -vv这一步的时候测试命令报错：

from theano.tensor.signal import downsample

E   ImportError: cannot import name 'downsample'

这个错误想必很多使用Theano或者是Lasagne的朋友们都遇到过，主要是Theano版本更新带来的错误。个人觉得你强行去改Lasagne里的代码不如直接在venv里面给Theano降级。对应“from theano.tensor.signal import downsample”的版本号为0.8.2。

所以此时只需要使用

~~pip install theano==0.8.2~~

（1）pip install theano[默认安装最新版]

~~pip install lasagne==0.1~~

（2）安装lasagne，这里应该查看官方连接，按照官方链接安装方法安装：https://github.com/Lasagne/Lasagne

【推荐，一般都是lasagne版本问题】



pip install -r https://raw.githubusercontent.com/Lasagne/Lasagne/master/requirements.txt
pip install https://github.com/Lasagne/Lasagne/archive/master.zip
即可。ntm的原写于2016年，但是作者目前还在修改代码，这就导致了test过程可能通过不了。根据2016年Theano发布到0.8.2颁布，做对应修改即可。


# 3、问题1，参考链接https://stackoverflow.com/questions/39501152/importerror-no-module-named-downsample





The `downsample` module has been moved to `pool`, so try declaring it as:
`from theano.tensor.signal.pool import pool_2d`
After changing delete your theano cache with the command:
`theano-cache purge`







# 3、AttributeError: 'module' object has no attribute 'find_graphviz'

`pip install pydot==1.1.0`

`参考链接：https://stackoverflow.com/questions/38446771/importing-theano-attributeerror-module-object-has-no-attribute-find-graphvi`

```bash

```

```bash

```

# ```bash4、在运行SCNN时候会出现```

# ```bashTypeError:**init**() got an unexpected keyword argument 'flip_filters'```

~~解决办法：卸载掉上述人为安装的0.8版本，sudo pip uninstall theano~~

~~然后重新安装sudo pip install theano，版本为1.0.1版本~~

~~Successfully installed theano-1.0.1~~还是会出现上述问题

# ```bash```



