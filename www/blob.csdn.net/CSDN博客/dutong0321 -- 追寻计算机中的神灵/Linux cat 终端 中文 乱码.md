# Linux cat 终端 中文 乱码 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2016年09月02日 12:22:11[dutong0321](https://me.csdn.net/dutong0321)阅读数：10354标签：[终端																[linux																[cat																[中文																[乱码](https://so.csdn.net/so/search/s.do?q=乱码&t=blog)](https://so.csdn.net/so/search/s.do?q=中文&t=blog)](https://so.csdn.net/so/search/s.do?q=cat&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)
个人分类：[我的系统](https://blog.csdn.net/dutong0321/article/category/5790533)
### 概述
#### OS：Raspbian
最近每天都在考虑笔试、面试以及找工作的事情，不仅CSDN博客又有开始荒废的倾向，就是以前的一些知识也开始逐步退忘，最近，赶紧还得复习复习。反正，大四真忙。 
上次，将vim的编码改成了UTF-8，为什么不用GBK，这是因为在以前发现有的设备并不支持GBK。我在编写彩虹雨的游戏(一个cocos2d-x的项目)当中就出现过，在WIN32平台运行正常，但是移植到安卓平台，所有的字都无法显示。无奈采用将文字保存到XML中，然后从XML中读取文字。最后，因为采用XML来保存文字，还增加了一个多语言支持。 
但是，随之而来的一个问题就是Linux里的cat命令无法正常使用了，因为cat只能读取GBK码，其他都是乱码。我从网络上找了很久资料，发现没有任何关于这面的资料，即使有也根本没用。刚刚，忽然想起来vim的显示不过也是字符编码的转换，那么我自己写一个简单的脚本不就OK了。于是，我就成功了。
### cat 可正常显示所有中文编码
1.首先要安装两个工具enca iconv 
树莓派上好像本来就有iconv 所以不用安装 其他的Linux发行版如果没有自行安装
```bash
sudo apt-get install enca
```
2.建立一个新的shell脚本
```bash
sudo vim /usr/bin/cat1
```
脚本内容:
```bash
#!/bin/bash
# 获取文件编码
var=$(enca $1)
code=${var#*; }
#生成临时文件
iconv -f $code -t GBK -o /tmp/tempcat $1
#读取临时文件
cat /tmp/tempcat
#删除临时文件
rm /tmp/tempcat
```
3.最后记得改变文件权限就可以了
```bash
sudo chmod 755 /usr/bin/cat1
```
以后 打开文件执行 cat1 filename即可 很方便 而且不用更改系统配置
