# Mac上安装xgboost - BlueSky - CSDN博客
2017年10月26日 19:53:00[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：642
> 
毕业了工作一段时间了，好久没有写东西了。如果说一下从学校到职场的感受，就用经典的两个字可以描述–”围城”。没有什么都是自己想的那么好，直白点就是选择工作的时候一定要调研清楚，真正弄明白工作的真正性质，不要靠自己YY，否则工作了才发现不是自己想的那样就有点坑了。然而，目前发现我已经入坑了，所以以后工作还是彻底调研清楚，防忽悠！ 
      ——–以上就当是简单吐槽下。
最近看了下机器学习相关的东东，发现xgboost比较实用，就先从安装说起。 
    参考官网的步骤： 
[Building on OSX](http://xgboost.readthedocs.io/en/latest/build.html#building-on-osx)
> 
git clone –recursive [https://github.com/dmlc/xgboost](https://github.com/dmlc/xgboost)
  cd xgboost 
   cp make/minimum.mk ./config.mk; make -j4
上面是直接安装，没有使用多线程设置，如果不用多线程可以想想下，会有多慢。。。 
多线程配置安装，正确步骤：
- 多线程开启方法 
因为Mac 的 OS X 系统的 C语言 编译器用的是 Clang，如下图，在 Terminal 输入 gcc -v或者 gcc –version 后，显示 C语言 的编译器： 
![这里写图片描述](https://img-blog.csdn.net/20171026195005147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQVNJQV9rb2Jl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们需要安装新版gcc，brew大法好（类似ubuntu下apt-get install） 
`brew install gcc`
    这一步会花费一点时间，要有耐心，安装完成后，可以 
`brew info gcc`
![这里写图片描述](https://img-blog.csdn.net/20171026195717573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQVNJQV9rb2Jl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到安装的版本是gcc 7.2.0，这个一会要用到。
```python
cp make/config.mk ./config.mk
        vi config.mk
```
然后编辑 config.mk文件（下面的7对应的是上面安装的版本），
```python
export CC = gcc-7
    export CXX = g++-7
```
![这里写图片描述](https://img-blog.csdn.net/20171026200226316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQVNJQV9rb2Jl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后运行
```python
make -j4
```
这样就编译完了xgboost，然后我们安装下openmpi
```python
brew install openmpi
```
如果是利用Python调用，那么还需要
```python
cd python-package
    sudo python3 setup.py install
```
即可安装完成。 
这样就可以在**Python**环境中***import xgboost***了。
