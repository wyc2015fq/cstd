# iTerm2 配色方案（oh-my-zsh& Powerline） - lijunweiyhn的博客 - CSDN博客





2018年11月12日 10:54:34[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：148








转载自：[https://blog.csdn.net/anningzhu/article/details/77774492](https://blog.csdn.net/anningzhu/article/details/77774492)

# iTerm2 配色方案（oh-my-zsh& Powerline）

> 
公司新配的Macbook Pro第一件事就是装iTerm2，

这篇文章将记录我的整个安装过程！

希望大家都一次安装成功！

请认真观看！


看看配置完是长这样滴！别着急，带你一起配......

![iterm2.png](http://upload-images.jianshu.io/upload_images/7165111-4eff070a5ee1a9c8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

iterm2.png
- 先去官网下载[iTerm2](http://www.iterm2.com/)，打开后是全黑色的背景。

![Screen Shot 2017-08-01 at 5.45.28 PM.png](http://upload-images.jianshu.io/upload_images/7165111-8242565b5f070824.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Screen Shot 2017-08-01 at 5.45.28 PM.png
- [solarized](http://ethanschoonover.com/solarized)可以说是目前网络上最流行的配色解决方案,我个人也是觉得比较这种颜色搭配。
- 第一种方法：打开iTerm2的偏好设定，`Profiles / Colors`，直接选择Solarized Dark。
- 另一种方法：最下面的Load Presets ... / Import... 直接加载iterm2-colors-solarized/Solarized Dark.itermcolors配色方案就可以了，效果一样。

![Screen Shot 2017-08-01 at 5.56.57 PM.png](http://upload-images.jianshu.io/upload_images/7165111-a73bebb06275fc16.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Screen Shot 2017-08-01 at 5.56.57 PM.png
- 
再次打开效果如下

![Screen Shot 2017-08-01 at 6.00.52 PM.png](http://upload-images.jianshu.io/upload_images/7165111-47ec93f6598428c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Screen Shot 2017-08-01 at 6.00.52 PM.png


安装[oh-my-zsh](http://ohmyz.sh/)

1.直接在命令行敲

```
sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
```
- 当oh-my-zsh就安装完成后，就可以简单配置[主题颜色](https://github.com/robbyrussell/oh-my-zsh/wiki/Themes)。
- 继续命令行敲入
` vi ~/.zshrc`

	打开后找到`ZSH_THEME="robbyrussell"`修改为`ZSH_THEME="agnoster"`

3.改为主题之后，重启iterms2后，效果如下：

![QQ20170802-171322.png](http://upload-images.jianshu.io/upload_images/7165111-da6f51c28224c687.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

QQ20170802-171322.png

哈哈，挺漂亮了，但是有一个问题就是，怎么有问号呀，这不是乱码。此时。还差最后一步！！！

Powerline字体下载安装

继续在命令行敲下面的两行命令
- 
`# clone 先克隆`

- 
`git clone https://github.com/powerline/fonts.git`

- 
`# install 后安装`

- 
`cd fonts`

- 
`./install.sh`


安装完字体库之后，把iTerm 2的设置里的Profile中的Text 选项卡中里的、 Font和Non-ASCII Font的字体都设置成 Powerline的字体，我这里设置的字体是12pt Meslo LG S DZ Regular for Powerline

![QQ20170802-173718.png](http://upload-images.jianshu.io/upload_images/7165111-0546fcf6666c3c54.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

QQ20170802-173718.png

修改完成后！重新启动iterm2，终于啊哈哈哈哈......

![](http://upload-images.jianshu.io/upload_images/7165111-4eff070a5ee1a9c8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
其他（git 中用到的终端命令）：


1.创建文件 ：`touch a.html`

2.创建并编辑文件 ：`vim a.html`

3.创建并添加内容 ： `echo nihao >>a.html`

4.查看文件内容 ： `cat a.html`

5.查看隐藏文件 ： `ls-ah`



转自：http://www.jianshu.com/p/33deff6b8a63



