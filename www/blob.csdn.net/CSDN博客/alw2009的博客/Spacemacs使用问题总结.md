# Spacemacs使用问题总结 - alw2009的博客 - CSDN博客
2019年01月04日 21:43:42[traveler_zero](https://me.csdn.net/alw2009)阅读数：71
**目录**
[一、为什么使用spacemacs之后， 鼠标右键拷贝的功能失效了？](#%E4%B8%80%E3%80%81%E4%B8%BA%E4%BB%80%E4%B9%88%E4%BD%BF%E7%94%A8spacemacs%E4%B9%8B%E5%90%8E%EF%BC%8C%20%E9%BC%A0%E6%A0%87%E5%8F%B3%E9%94%AE%E6%8B%B7%E8%B4%9D%E7%9A%84%E5%8A%9F%E8%83%BD%E5%A4%B1%E6%95%88%E4%BA%86%EF%BC%9F)
### 一、为什么使用spacemacs之后， 鼠标右键拷贝的功能失效了？
现象描述： 在Windows用putty链接Linux环境， 启动emacs。 在没使用spacemacs之前， 我可以在putty的会话窗口里用鼠标选中想要拷贝的文本， 然后点击右键拷贝到Window的剪贴板上。但是在使用了spacemacs之后， 这种拷贝方法就不生效了。
原因分析： putty支持xterm-style mouse reporting的功能， 能把终端捕捉到的信号发送给server端处理。 spacemacs默认开启xterm-mouse-mode的功能， 这个功能可以让emacs响应鼠标点击。所以你可以定义一些命令绑定鼠标事件, 也可以查询当前的鼠标按键绑定了什么命令。 C-h k 鼠标动作就能查看。当spacemacs处理了鼠标事件了之后， putty原来的复制功能就失效了。
解决方法： 有三种方法可以让我们可以继续用右键拷贝功能
1. 停掉spacemacs的xterm-mouse-mode, M-x xterm-mouse-mode.
2.在putty设置里停掉xterm-style mouse reporting的功能。 
![](https://img-blog.csdnimg.cn/20190104213650221.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
**3. 不用关闭xterm-mouse-mode和xterm-style mouse report, 只是在鼠标选中文本的时候同时按住shift键就可以了， 这时putty不会把鼠标事件发送给server端。（推荐使用这种方法）**
参考： 
[https://tartarus.org/~simon/putty-snapshots/htmldoc/Chapter4.html#config-features-mouse](https://tartarus.org/~simon/putty-snapshots/htmldoc/Chapter4.html#config-features-mouse)
[https://ftp.gnu.org/old-gnu/Manuals/emacs/html_node/emacs_212.html](https://ftp.gnu.org/old-gnu/Manuals/emacs/html_node/emacs_212.html)
