
# IntelliJ手动安装python与scala插件 - bitcarmanlee的博客 - CSDN博客


2017年04月25日 22:52:00[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：4731


IntelliJ强大之处在于各种各式的丰富插件，能满足日常开发中的绝大部分需求。由于天朝众所周知的原因，通过Intellij在线安装插件的形式，经常会折腾很久安装不成功。最让人恼火的是一个几十M甚至只有几M的插件，经常安装到一半或者一部分以后，因为网络连接的原因安装失败。本博主印象中每次新装环境时，装python插件用在线安装的方式都花费了相当长时间。于是痛下决心，研究了一下离线安装的方式，发现好简单，并且效率也大大提高了。
## 1.将需要的插件下载到本地
既然是手动离线安装，第一步自然是将需要的插件下载到本地。唯一需要注意的是注意插件的版本号与Intellij的版本号保持一致，以避免后续不必要的麻烦。例如我们想要安装python或者scala插件，在Preferences->Plugins里搜索python或者scala关键字，会显示出相应的插件。
![这里写图片描述](https://img-blog.csdn.net/20170425224313870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为我这里python插件已经安装好了，所以显示的是Uninstall按钮。如果是未安装，显示的将会是绿色的Install按钮。
如果天朝没有GFW的话，我们直接Install就完事了。但是，你懂的，事情没这么简单，如果你尝试Install十次之内安装成功，那恭喜你，网络超给力。如果不能，那先根据右边插件提供的地址，去官网先下载过来到本地，这样最大的好吃是，支持断点续传，可以保证下载成功。
## 2.从本地安装
下载完成以后，再打开Pluigin选项卡。
![这里写图片描述](https://img-blog.csdn.net/20170425224933020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意此时右下角有个Install plugin from disk。点击进去，找到我们刚刚下载成功的插件所在的位置，选中zip压缩包然后点击open，再重启Intellij，OK，大功告成！

