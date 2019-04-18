# Python快速定位工作目录 - wishchinYang的专栏 - CSDN博客
2014年02月13日 16:17:44[wishchin](https://me.csdn.net/wishchin)阅读数：1619
原文链接：[http://www.cnblogs.com/wdong/archive/2010/08/19/1802951.html](http://www.cnblogs.com/wdong/archive/2010/08/19/1802951.html)
       常年奋斗在编码一线的同学，应该都深有体会，工作久了，很多项目文件、技术资料，设计文档，安装包等等一堆一堆的工作目录分散在各个磁盘中，需要用到的时候，频繁的在各个目录中切换，每次都得用资源浏览器打开，还得点多次鼠标才能找到，或者有些如Apache或者tomcat等服务又需要用命令行启动，如果文件目录层次比较深的话，每次重复下列动作：
```cpp
开发运行->cmd->c:\\Documents and Settings\\Administrator>D:
      D:\>cd mycode
      D:\>MyCode>cd python
      D:\>MyCode>Python>Practise>...
```
    反反复复的重复动作真的是太烦了，自己动手丰衣足食，还是用代码自己来写一段快速定位或切换工作目录的小工具，像工具软件一样点一下就直接打开，岂不是省去敲打那些无聊的cd命令，ok，因为已经凌晨了，突然来了兴致即兴发挥，实在搞不完，这一篇写把总体思路及几个关键的核心代码实现掉，下一篇完成所有功能
    总体思路：
    1.软件中可自定义常用工作目录或文件的配置项
    2.选择某一工作目录项可打开命令行，路径直接指向到指定的文件夹处
    3.选择某一工作目录项可打开资源浏览器，路径直接指向到指定的文件夹处
    4.选择某一文件项可直接打开执行
    5.使用python + Qt实现图形化操作界面(基于托盘图标)的工具软件
    此篇实现前四个核心功能需求
    1.使用python ConfigParser实现可配置项
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码：
```
```python
class ConfigItem(object):
 2     def __init__(self,category,caption,path,icon,command, explorer, execute):
 3         self.category = category
 4         self.caption = caption
 5         self.path = path
 6         self.icon = icon
 7         self.command = command
 8         self.explorer = explorer
 9         self.execute = execute
10         
11 def readConfigItems(file,sect=None,key=None):
12     if not os.path.isfile(file):
13         raise Exception("FileNotFound")
14     cf = ConfigParser.ConfigParser()
15     cf.read(file)
16     sections = cf.items("sections")
17     
18     if sect is None and key is None:
19         l = []
20         for name,key in sections:
21             i = ConfigItem(
22                 category= cf.get(key,"category"),
23                 caption=cf.get(key,"caption"),
24                 path=cf.get(key,"path"),
25                 icon=cf.get(key,"icon"),
26                 command=cf.getboolean(key,"command"),
27                 explorer=cf.getboolean(key,"explorer"),
28                 execute=cf.getboolean(key,"execute")
29             )
30             l.append(i)
31         return l
32     return cf.get(sect,key)
33 
34 def addConfigItem(file,item):
35     config = ConfigParser.RawConfigParser()
36     if not os.path.isfile(file):
37         raise Exception("FileNotFound")
38     config = ConfigParser.ConfigParser()
39     config.read(file)
40     sections = config.items("sections")
41     new_section = "section" + str(len(sections)+1)
42     config.set("sections",new_section,new_section)
43     config.add_section(new_section)
44     config.set(new_section, 'category', item.category)
45     config.set(new_section, 'caption', item.caption)
46     config.set(new_section, 'path', item.path)
47     config.set(new_section, 'icon', item.icon)
48     config.set(new_section, 'command',item.command)
49     config.set(new_section, 'explorer', item.explorer)
50     config.set(new_section, 'execute', item.execute)
51     # Writing our configuration file to 'example.cfg'
52     with open(file, 'wb') as configfile:
53         config.write(configfile)
54 
55    
56 if __name__ == "__main__":
57     #os.execvp(file)
58     #openCommand(r"D:\MyCode\Python\Pratices")
59     #openExplorer(r"D:\MyCode\Python\Pratices")
60     item = ConfigItem(
61                 category= "3",
62                 caption="test",
63                 path=1,
64                 icon=1,
65                 command=True,
66                 explorer=True,
67                 execute=True
68             )
69     addConfigItem("d:\\conf.conf", item)
70     print readConfigItems("d:\\conf.conf")
```
```
  配置文件格式：
```
```python
[sections]
section2 = section2
section1 = section1
[section2]
category = 1
execute = False
explorer = True
caption = 工作目录
command = True
path = 1
icon = 1
[section1]
category = 工作目录
execute = True
explorer = True
caption = Python练习代码
command = True
path = 1
icon = 1
```
```
2.使用命令行、资源浏览器打开指定文件夹目录或直接执行可执行文件 
```
```python
# !/usr/bin/python
 2 #-*-coding:utf-8-*-
 3 """
 4  author: "wdong"
 5  date: @2010-8-18
 6  summary:quick start your command file or explorer file or exec file
 7 """
 8 
 9 import os, time, datetime,sys
10 import ConfigParser
11 
12 def openCommand(dest):
13     """
14     Specify the folder to opened by command line
15     """
16     cmd = "cmd /k cd \"" + dest + "\""
17     import subprocess
18     subprocess.Popen(cmd, shell=True)
19 
20 def openExplorer(dest):
21     """
22     Specify the folder to opened by Explorer
23     """
24     cmd = "explorer \"" + dest + "\""
25     import subprocess
26     subprocess.Popen(cmd, shell=False)
27 
28 def execFile(file):
29     """
30     execute the file by system register file type
31     """
32     if os.path.isfile(path):
33         try:
34             os.system(file)
35         except:
36             os.system("file:///" + str(file).replace("\\\\", "/"))
37     else:
38         openExplorer(file)
```
```
    至此，核心功能调试完毕，明天接着做图形工具的实现，各位同行朋友，如果仔细读完，其实接下来的实现已经很简单了，有兴趣也可以自己做个界面玩玩，同时也希望高手们多多指点
[我的Python工具-快速定位工作目录（二）](http://www.cnblogs.com/wdong/archive/2010/08/20/1804112.html)
