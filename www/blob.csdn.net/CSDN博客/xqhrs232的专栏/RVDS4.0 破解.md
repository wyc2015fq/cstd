# RVDS4.0 破解 - xqhrs232的专栏 - CSDN博客
2010年05月14日 10:32:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：12124
转载时请以超链接形式标明文章原始出处和作者信息及[本声明](http://bangzhuzhongxin.blogbus.com/logs/11205960.html)
[http://amazingxiu.blogbus.com/logs/62781676.html](http://amazingxiu.blogbus.com/logs/62781676.html)
    这几天闲来无事，在看如何安装RVDS4.0，也就是RealView Development Suite 4.0 Professional。
自己一个人捣腾了很久才慢慢知道如何安装了，其中参考了网上不少的强帖：
gliethttp.cublog.cn 的帖子：[RVDS-RealView Development Suite 4.0 Professional软件](http://blog.chinaunix.net/u1/38994/showart.php?id=2038478)
另外还有MCU论坛，ARM专栏论坛中的帖子：[RealView Development Suite 4.0 Professional （for windows,linux）及其破解下载(我的crack就是从这里下的，网上别处的好像都被删除了，嘿嘿，谢谢了)。](http://bbs.mcu123.net/bbs/thread-12555-1-1.html)
虽然万事具备，安装文件有了，破解的文件也有了(貌似这样对待外人的软件也不太妥~~~，不过拿来用啦)。安装的时候却是有一些需要注意的事项的。
    我安装RVDS的时候，装了4遍，才算把它的“脾气”弄清楚。下面就把我自己的一点理解说说。
还是结合破解里面的“readme”吧。
1. 下载特殊文件压缩包，将特殊文件解压缩在RVDS4.0的安装目录下，如F:ARM
1. 修改MAC地址：
  1) 开始 -> 运行 -> CMD -> ipconfig/all，Physical Address一项为MAC地址
  2) 用记事本打开rvds.dat，找到HOSTID=00023F0468D4，共有19处，后面的数字替换成MAC地址，保存
2. 安装RVDS4.0
  1) 安装至弹出 ARM License Wizard 窗口时，选择 Install License
   2）将压缩包crack中特殊文件解压缩在RVDS4.0的安装目录下，如F:ARM(一定是放在ARM下的目录，尽量不要再附加一个目录，如F:/ARM/crack等。)
  3) 浏览选择刚才修改的rvds.dat，Add添加
  4) 完成安装
4. 运行 crack.bat (就是在F:/ARM目录下)，直至完成。运行到ecc时会暂时无反应，请耐心等待。完成后会自动关闭的。
PS：如果某一次没有安装成功的话，先将软件卸载，然后推荐使用“Windows清理助手”将注册表里面遗留的上一次安装信息等全部清空一遍。再安装，这样就不会遗留上一次的错误信息，以至于后面的破解安装一直都是错误的。
