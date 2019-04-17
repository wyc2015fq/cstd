# 【HTTPServer】借助Python建立简易的HTTP服务 - CD's Coding - CSDN博客





2018年11月22日 11:48:20[糖果天王](https://me.csdn.net/okcd00)阅读数：120








## 0x00 前言

局域网内有个想传的文件，但是跨平台了怪麻烦的。

于是想着建立个http服务来传吧，但是又懒得下载东西，哦对了我有Python，那就足够了~

## 0x01 防火墙打开端口

> 
为了开启服务，首先需要打开一个对外端口以便其他机器访问进来，

Python中自带的HTTP Server默认占用的是端口`8000`，当然也可以自己选定特定端口。

如果直接使用命令无法访问的话，那应该就是被防火墙挡住了（下述介绍Linux的防火墙操作）。
**打开端口（python自带的HTTPServer端口是8000）**
`$ sudo firewall-cmd --permanent --add-port=8000/tcp`

**如果需要再把这个端口关上**
`$ sudo firewall-cmd --permanent --remove-port=8000/tcp`

**每次操作结束后都需要重启一下防火墙**
`$ sudo systemctl restart firewalld`

## 0x02 建立简易HTTP服务器

> 
Python 2 与 Python 3 的命令不同，需要注意


**Python 2**

```
# 采用默认端口（8000）建立HTTP服务
$ python -m SimpleHTTPServer
# 采用自定端口（8765）建立HTTP服务
$ python -m SimpleHTTPServer 8765
```

**Python 3**

```
# 采用默认端口（8000）建立HTTP服务
$ python -m http.server
# 采用自定端口（8765）建立HTTP服务
$ python -m http.server 8765
```

## 0x03 效果

> 
展示的均为直接在某个文件目录下开启的HTTP服务，

假如开启服务的文件夹下有 `index.html`，会默认将其作为首页，反之则是如图所示的文件目录


### 局域网内Linux服务器上开启的HTTP服务

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122114343920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

### 本机Windows机器上开启的HTTP服务

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122114454274.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)





