# ubuntu下caffe安装python3接口 - Snoopy_Dream - CSDN博客





2018年07月28日 10:54:20[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：625








正常遇到的问题和python2安装那些个，直接看下面链接。

[https://blog.csdn.net/g11d111/article/details/78141202](https://blog.csdn.net/g11d111/article/details/78141202)

```python
try:
    xrange          # Python 2
except NameError:
    xrange = range  # Python 3
```

1.修改makefile.config中python的路径

![](https://img-blog.csdn.net/20180728104820606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.

```python
sudo pip3 install numpy
sudo apt-get install python3-skimage
```

3.python文件夹下面 sudo pip3 install -r requirements.txt

4.

```
cd /usr/lib/x86_64-linux-gnu
sudo ln -s libboost_python-py35.so libboost_python3.so
```

5.make -j12&&make py

6.如果是faster-rcnn的时候，lib文件夹下面 make的时候注意：修改makefile文件

```
all:
    python3 setup.py build_ext --inplace #python setup.py build_ext --inplace
    rm -rf build
```

7.lib文件夹下面的setup.py中第53行修改（python3与2的区别）

![](https://img-blog.csdn.net/20180728111820500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

8.修改.bashrc以及生效

9.下面一部实在不行，就尝试一下。

```
cd /usr/local/lib
sudo ldconfig
```





