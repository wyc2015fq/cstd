# java学习笔记：eclipse的workspace和working set - 左直拳的马桶_日用桶 - CSDN博客
2014年07月22日 22:24:13[左直拳](https://me.csdn.net/leftfist)阅读数：6055
我将这二个东西搞混了。
看上去，workspace 类似于VS里的解决方案，而working set则像VS里的解决方案文件夹，是个逻辑概念。
VS里的解决方案是个文件，而eclipse里的workspace，则是一个真实存在的文件夹。
另外，VS里的解决方案文件夹，隶属于解决方案；但在eclipse里，workspace 和 working set 是两个不同的视图。
当workspace项目很多的时候，可以建立working set，将项目分门别类，好得很。如果是建立不同的workspace来区分，则切换workspace，要重启eclipse，代价太高了。
另外，java里的package（包），看上去就相当于C#里的命名空间。
