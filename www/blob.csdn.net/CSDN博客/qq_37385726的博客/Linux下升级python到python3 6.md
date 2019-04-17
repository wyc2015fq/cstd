# Linux下升级python到python3.6 - qq_37385726的博客 - CSDN博客





2018年08月15日 14:31:06[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：206








```
# python -V    # 查看python 版本
# cd /home/centos/Downloads    # 进入存放目录
# wget https://www.python.org/ftp/python/3.6.1/Python-3.6.1.tar.xz    # 获取新版的 python
# tar xvf Python-3.6.1.tar.xz    # 解压文件
# cd ./Python-3.6.1     # 进入解压后的文件目录
# mkdir /usr/local/python3    # 创建编译安装目录
# ./configure --prefix=/usr/local/python3    # 配置编译安装目录
# make && make install    # 编译安装
zipimport.ZipImportError: can't decompress data; zlib not available
```

**解决 zipimport.ZipImportError: can't decompress data; zlib not available 的方法：**

1.下载源码： [http://www.zlib.net/](http://www.zlib.net/)

2.解压安装：

```
# cd /home/centos/Downloads/
# tar zxvf zlib-1.2.11.tar.gz 
# cd zlib-1.2.11
# ./configure
# make && make install
```

**重新安装 python3:**

```
# cd /home/centos/Downloads/Python-3.6.1
# ./configure -- prefix=/usr/local/python3
# make && make install
# mv /usr/bin/python /usr/bin/python_old    # 备份旧的python
# ln -s /usr/local/python3/bin/python3 /usr/bin/python    # 把新的 python3 链接到 python
# python
Python 3.6.1 (default, May 30 2017, 18:13:28) 
# yum search    # 安装完后导致 yum 不可用
File "/usr/bin/yum", line 30
    except KeyboardInterrupt, e:
                            ^
SyntaxError: invalid syntax
```

**解决 yum 不可用：**

```
# vi /usr/bin/yum
#!/usr/bin/python    改成：    #!/usr/bin/python_old
# yum search
```



> 
只读文件不可写请参见：[这里](https://www.cnblogs.com/pinganzi/p/6024261.html)




