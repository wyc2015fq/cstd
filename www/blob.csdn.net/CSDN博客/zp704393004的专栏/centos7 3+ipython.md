# centos7.3+ipython - zp704393004的专栏 - CSDN博客





2018年04月17日 20:45:23[原来未知](https://me.csdn.net/zp704393004)阅读数：84








在安装ipython时提醒没有pip，因此网上的教程安装，但是出现没有epel包，所以，再次记录一次

利用rpm -ivh http://mirrors.ustc.edu.cn/fedora/epel/6/x86_64/epel-release-6-8.noarch.rpm
命令安装epel，不过这个网址应该没有epel了，需要改为这个网址rpm -ivh http://mirrors.ustc.edu.cn/fedora/epel/6/x86_64/epel-release-6-8.noarch.rpm


安装后，可以安装pip

yum -y install epel-release

然后，利用



```bash
pip
```

```bash
install
```

```bash
ipython
```



安装ipython，不过可能还需要安转过一些其他包，如：





```bash
yum
```

```bash
install
```

```bash
readline-devel
```



```bash
yum
```

```bash
install
```

```bash
patch
```



```bash
pip
```

```bash
install
```

```bash
readline
```


需要将ipython的路径软链接添加到/usr/bin目录下
```
```bash
ln
```

```bash
-s
```

```bash
/usr/local/python2
```

```bash
.7.12
```

```bash
/bin/ipython
```

```bash
/usr/bin/ipython
```
```







