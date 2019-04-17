# ubuntu上创建自己的arduino库、使关键字设置高亮、安装arduino - V__KING__的专栏 - CSDN博客





2013年10月10日 16:35:40[v__king__](https://me.csdn.net/V__KING__)阅读数：2462








一、ubuntu 12.04 上安装arduino（前提是已经安装好了ubuntu）  -->  网站：playground.arduino.cc//Linux/Ubuntu

命令：sudo apt-get install arduino arduino-core







二、创建自己的库（c++的.cpp和.h文件就不说了，网上很多）   -->网站：arduino.cc/en/Hacking/LibraryTutorial

1. 在你的librarys目录下建立一个叫Morse的目录(目录名随便取)




2. 将Morse.cpp和Morse.h 复制到 Morse目录 下




3.重新开启Arduino IDE，在Skatch->Import Library就可以看到你的Morse库了。




三、将程序库中关键字高亮

    新程序里关于Morse函数库的内容无法被IDE识别和高亮。很不幸的是，目前Arduino的IDE无法自动识别自定义函数库里的内容


    在Morse的文件夹里，创建一个叫keywords.txt文件，（参照标准库中的txt文件）内容如下：

```cpp
Morse   KEYWORD1
dash    KEYWORD2
dot     KEYWORD2
```


    每一行均由关键字的名字、TAB键（非空格）、关键字种类顺序组成。类名是KEYWORD1，将被高亮成橘黄色；函数名是KEYWORD2，将被高亮成棕色。重启Arduino IDE后这些设置将生效。




