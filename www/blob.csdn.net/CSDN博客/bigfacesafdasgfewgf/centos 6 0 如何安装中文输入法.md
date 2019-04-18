# centos 6.0 如何安装中文输入法 - bigfacesafdasgfewgf - CSDN博客





2015年04月27日 08:02:24[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：961








centos 6.0没有自带的中文语言支持，所以我们在安装好系统之后，最后再额外安装一次中文输入法。




首先，打开terminal中终端，输入：




su root

yum install "@Chinese Support"





然后就是一路的“y”，确认好successfully！

最后我们再重新注销启动一下。




配置：

打开system->preference->input method

![](https://img-blog.csdn.net/20150427075550397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





在弹出的窗口中选择use IBus(recommended)->input method preference




![](https://img-blog.csdn.net/20150427075706665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





最后就是input method->Pinyin->Chinese-Pinyin->Add




就完成了配置过程。

重新log out出来，进去之后就可以用ctrl+space调用拼音输入法了。






