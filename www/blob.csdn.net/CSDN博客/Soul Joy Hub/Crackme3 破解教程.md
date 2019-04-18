# Crackme3 破解教程 - Soul Joy Hub - CSDN博客

2016年06月13日 09:55:41[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1286


Crackme3 破解教程 


**1、先用PEiD对 Crackme3进行 壳测试 **

![](http://images.cnitblog.com/blog/709693/201501/191655053753834.png)

点击File右边的按钮，选中Crackme3 

![](http://images.cnitblog.com/blog/709693/201501/191655125318554.png)

结果如下图所示： 



![](http://images.cnitblog.com/blog/709693/201501/191655142977828.png)

即 无壳。 

- 
**试运行软件 **

![](http://images.cnitblog.com/blog/709693/201501/191655147193657.png)

点击 Register now！ 

结果如下： 

![](http://images.cnitblog.com/blog/709693/201501/191655151883500.png)

- 
用Ollydbg开始破解 

打开 

![](http://images.cnitblog.com/blog/709693/201501/191655264062693.png)

点击File->Open，找到Crackme3 打开 

![](http://images.cnitblog.com/blog/709693/201501/191655280313223.png)

右击->Search for->All referenced text strings 

![](http://images.cnitblog.com/blog/709693/201501/191655310946226.png)

右击->Search for text 

输入关键词 "Wrong" 

![](http://images.cnitblog.com/blog/709693/201501/191655327196756.png)

点OK 

![](http://images.cnitblog.com/blog/709693/201501/191655395314203.png)

双击： 

![](http://images.cnitblog.com/blog/709693/201501/191655419692849.png)

观察： 

![](http://images.cnitblog.com/blog/709693/201501/191655424538462.png)

![](http://images.cnitblog.com/blog/709693/201501/191655428133506.png)

在00440F2C处按F2 设置断点，按F9 运行 

![](http://images.cnitblog.com/blog/709693/201501/191655433448835.png)

点击 Register now ！，进入Olldbg，按F7进入调试 

![](http://images.cnitblog.com/blog/709693/201501/191655442354507.png)

经过分析，用户名和注册码就是：Registered User 和 GFX-745-IER-954 

但 00440F2C 语句会进行核对，所以要将这句delete 

在这句上，按backspace 

![](http://images.cnitblog.com/blog/709693/201501/191655447508363.png)

- 
保存修改 

右击->Copt to executable 

弹出窗口，右击->Save file 

![](http://images.cnitblog.com/blog/709693/201501/191655454223437.png)

按"是（Y）" 

选择地址，保存 

![](http://images.cnitblog.com/blog/709693/201501/191655477814123.png)

打开保存后的软件，输入：Registered User 和 GFX-745-IER-954 

![](http://images.cnitblog.com/blog/709693/201501/191655492661611.png)

再按Registered now ！ 

![](http://images.cnitblog.com/blog/709693/201501/191655504388068.png)


破解成功！

