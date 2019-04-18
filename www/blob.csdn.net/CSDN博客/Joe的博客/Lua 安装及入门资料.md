# Lua 安装及入门资料 - Joe的博客 - CSDN博客





2017年03月04日 19:32:58[Joe-Han](https://me.csdn.net/u010089444)阅读数：1080标签：[lua																[安装																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Torch](https://blog.csdn.net/u010089444/article/category/6765936)





**入门资料：**
- [Learn Lua in 15 Minutes](http://tylerneylon.com/a/learn-lua/)
- [Lua 教程](http://www.runoob.com/lua/lua-tutorial.html)

## **Lua安装**

Linux 系统上安装 Lua 非常简单，只需要下载源码包并在终端解压编译即可，本文使用了5.3.4版本进行安装：

```
curl -R -O http://www.lua.org/ftp/lua-5.3.4.tar.gz
tar zxf lua-5.3.4.tar.gz
cd lua-5.3.4
make linux test
make install
```

## **遇到的问题**

![这里写图片描述](https://img-blog.csdn.net/20170304192636935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**原因：**缺少libreadline-dev，在命令窗口输入如下语句：
`sudo apt-get install libreadline-dev`
## **测试**

```
lua
print("Hello World!")
```

![这里写图片描述](https://img-blog.csdn.net/20170304193132515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=安装&t=blog)](https://so.csdn.net/so/search/s.do?q=lua&t=blog)




