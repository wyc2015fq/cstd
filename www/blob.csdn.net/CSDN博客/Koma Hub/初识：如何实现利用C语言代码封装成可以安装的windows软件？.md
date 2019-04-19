# 初识：如何实现利用C语言代码封装成可以安装的windows软件？ - Koma Hub - CSDN博客
2018年03月30日 19:32:31[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：893
首先你需要安装一个软件[Inno Setup](http://www.jrsoftware.org/isinfo.php)，安装完成后启动软件截图如下：
![](https://img-blog.csdn.net/20180330191934754)
点击新建》点击下一步，直至：
![](https://img-blog.csdn.net/20180330192051062)
这里你可以输入你想要的名字，然后继续next，直至：
![](https://img-blog.csdn.net/20180330192222606)
红框中分别为你需要的可执行文件（采用源文件编译后的可执行文件，我这里是简单的helloworld）。然后继续下一步，期间会有个窗口可以填写license文件等（我直接跳过了），到了一个选择ico的界面，由于ico的CSDN里面不支持，所以我就不放了。
![](https://img-blog.csdn.net/20180330192658335)
然后一直下一步，就会在相应的文件夹里生成安装文件：
![](https://img-blog.csdn.net/20180330192820269)
下面双击setup.exe进行安装就好了：
![](https://img-blog.csdn.net/20180330193028081)
安装向导里面的ico就是你设定的ico。后续步骤即为无脑安装，安装后的目录即为你建立该软件时的目录（完全一样）。
