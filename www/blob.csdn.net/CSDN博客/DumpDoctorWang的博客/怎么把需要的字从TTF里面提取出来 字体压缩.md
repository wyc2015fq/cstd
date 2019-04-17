# 怎么把需要的字从TTF里面提取出来\字体压缩 - DumpDoctorWang的博客 - CSDN博客





2019年01月30日 16:09:42[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：401








# 一、使用font-spider

## 1.1、下载

官网：[http://font-spider.org](http://font-spider.org)

项目地址：[https://github.com/aui/font-spider](https://github.com/aui/font-spider)

用法打开官网就有，和font-converter相比，自动化程度比较高，但是需要先安装Node JS。
## 1.2、使用

官网有很详细的教程，也有中文，大家可自行查看。

# 二、使用font-converter

## 2.1、下载地址

项目地址：[https://github.com/Jijun/font-converter-gui](https://github.com/Jijun/font-converter-gui)

Linux下载：[https://github.com/Jijun/font-converter-gui/releases/download/v1.0/font-converter.linux.amd64.tgz](https://github.com/Jijun/font-converter-gui/releases/download/v1.0/font-converter.linux.amd64.tgz)

MacOS下载：[https://github.com/Jijun/font-converter-gui/releases/download/v1.0/font-converter-1.0.dmg](https://github.com/Jijun/font-converter-gui/releases/download/v1.0/font-converter-1.0.dmg)
如果你正在使用Windows，那就用第一个，这个我也不知道怎么编译。

## 2.2、使用

Linux版本的用法是首先解压，然后找到font-converter，并双击，进入如下界面
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130160102456.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

“TTF Directory”是你的TTF字体所在的目录，不需要指定具体的字体文件。“Contain Chars”是要保留在字体文件中的字，你要保留啥字，你就在里面输入你要保留的字。“Output Format”就选TTF。

然后点击“Start Converter”，就会开始压缩，在你的字体目录下就会多一个文件夹名字为“ttf”（或者“woff”、“eot”）的目录，里面就包含了压缩好的字体文件。





