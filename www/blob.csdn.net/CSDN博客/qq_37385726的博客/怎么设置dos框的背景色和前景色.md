# 怎么设置dos框的背景色和前景色~ - qq_37385726的博客 - CSDN博客





2017年02月03日 23:59:37[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：342











## 调用color函数法



- 

1



加入#include <stdlib.h>头文件，没有此头文件则无法调用本文使用的函数。



![如何更改c语言窗口的背景颜色与字体颜色](http://g.hiphotos.baidu.com/exp/w=500/sign=f54ebede7cf0f736d8fe4c013a54b382/b999a9014c086e0643bfb7ea06087bf40ad1cb78.jpg)


- 

2



在你所需要改变窗口颜色的位置调用函数 system（"color xx"） xx分别指的是背景颜色和文字（前景）颜色。x为一位16进制数，即1-f都可以使用。可以随意组合。



![如何更改c语言窗口的背景颜色与字体颜色](http://a.hiphotos.baidu.com/exp/w=500/sign=12d1fe8769224f4a5799731339f69044/e850352ac65c1038f3694663b6119313b07e8978.jpg)


- 

3



1-f分别代表的颜色如下：


 0 = 黑色      8 = 灰色   1 = 蓝色      9 = 淡蓝色   2 = 绿色      A = 淡绿色   3 = 湖蓝色    B = 淡浅绿色   4 = 红色      C = 淡红色   5 = 紫色      D = 淡紫色   6 = 黄色      E = 淡黄色   7 = 白色      F = 亮白色



![如何更改c语言窗口的背景颜色与字体颜色](http://b.hiphotos.baidu.com/exp/w=500/sign=1baad8a908d79123e0e094749d355917/fcfaaf51f3deb48f5d7e297ff41f3a292df57878.jpg)


- 

4



将背景设为白色，文字设为红色后的结果就是这样啦。





![如何更改c语言窗口的背景颜色与字体颜色](http://d.hiphotos.baidu.com/exp/w=500/sign=844c77867b3e6709be0045ff0bc69fb8/34fae6cd7b899e512c79b1b546a7d933c9950d9f.jpg)

![如何更改c语言窗口的背景颜色与字体颜色](http://a.hiphotos.baidu.com/exp/w=500/sign=ed2b605555da81cb4ee683cd6267d0a4/42a98226cffc1e17cac704904e90f603738de913.jpg)


END





## 2.更改属性法



- 

1



在打开用c语言制作的程序后，右击标题栏。



![如何更改c语言窗口的背景颜色与字体颜色](http://b.hiphotos.baidu.com/exp/w=500/sign=6e6fddc8212dd42a5f0901ab333a5b2f/2fdda3cc7cd98d10b8ac8490253fb80e7bec9013.jpg)


- 

2



选择“属性”



![如何更改c语言窗口的背景颜色与字体颜色](http://g.hiphotos.baidu.com/exp/w=500/sign=35a223c00bb30f24359aec03f894d192/8718367adab44aed60583645b71c8701a18bfb79.jpg)


- 

3



选择“颜色”选框



![如何更改c语言窗口的背景颜色与字体颜色](http://d.hiphotos.baidu.com/exp/w=500/sign=db7deed2fff2b211e42e854efa816511/e61190ef76c6a7ef055e2087f9faaf51f3de6613.jpg)


- 

4



这样就可以自由地调换颜色啦，左上方选择更改哪里的颜色，右上方可以通过填写rgb值来自由设置颜色。而中间一栏是预设好的颜色，可以直接使用。下方是预览图。



![如何更改c语言窗口的背景颜色与字体颜色](http://h.hiphotos.baidu.com/exp/w=500/sign=edf24f42532c11dfded1bf2353266255/500fd9f9d72a6059aa0669bb2c34349b033bba79.jpg)


- 

5



选择“确定”后，会有一个选择，请酌情自行选择。



![如何更改c语言窗口的背景颜色与字体颜色](http://a.hiphotos.baidu.com/exp/w=500/sign=a29ff77c2a738bd4c421b231918a876c/b3fb43166d224f4a588ffbb20df790529822d179.jpg)

![如何更改c语言窗口的背景颜色与字体颜色](http://d.hiphotos.baidu.com/exp/w=500/sign=089b091e885494ee87220f191df4e0e1/d0c8a786c9177f3e9c4c74ac74cf3bc79f3d5613.jpg)


- 

6



将背景设为白色，文字设为红色后的结果就是这样啦。



![如何更改c语言窗口的背景颜色与字体颜色](http://d.hiphotos.baidu.com/exp/w=500/sign=bd581542f91f4134e037057e151e95c1/80cb39dbb6fd5266cb50b4b5af18972bd4073613.jpg)


END





## 注意事项



- 
1.本文使用的是xp操作系统，但在其他系统中同样适用。

- 
2.还可以使用API法更改颜色，但较麻烦，有兴趣的可以去百度，本文就不赘述了。





- 







