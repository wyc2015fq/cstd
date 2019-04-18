# Mathematica开发环境(上): 包管理器 - 知乎
# 

我第一次写Mathematica的Package大概是在两年前.

然后一年多前开始了一个项目...好吧到现在才7个star我就不放了...

一直想写个教程,但是Mathematica的包管理几乎没有

这是因为官方从6.0(10多年前)开始就信奉去包化策略

你可以看到Mathematica从来没有开头引入一堆包的设定...

(如果你看上古代码的话是有这种设定的)

但是近年来Py第二波大火让我意识到...这个政策真是蠢...

人家pip一下,我只能发邮件给客服看看什么时候能有这个功能...

好吧看不下去的不止我一个, 终于有大佬开始手撸包管理器了

[Building a convenient package distribution system](https://link.zhihu.com/?target=https%3A//mathematica.stackexchange.com/questions/160330/building-a-convenient-package-distribution-system)

我们来尝试一下:

```
PacletInstall[
    "PacKit", 
    "Site" ->"http://www.wolframcloud.com/objects/b3m2a1.paclets/PacletServer"
]
RebuildPacletData[]
```

安装成功后面板中会出现 PacKit 选项
![](https://pic3.zhimg.com/v2-3fd0ca52d117659e1f84e99cdfd11232_b.jpg)
接下来选择第一个 App Manager 面板

面板上右键生成笔记本(这是个bug)然后拖到最下面有个 New Apps 按钮.
![](https://pic4.zhimg.com/v2-b80bfea91924ce607570782fabda5753_b.jpg)
新建我们的软件包,然后依次点击如下蓝框,选第一个 Init 启用 git.
![](https://pic2.zhimg.com/v2-62020ec472779cbc3c5a4ea9382bff0d_b.jpg)
然后随便找个笔记本输入

`AuthenticationDialog["GitHub", "https://github.com"]`
![](https://pic2.zhimg.com/v2-a24f32141c01633c18b05791d43b7c59_b.jpg)
然后点击 Create GitHub Repo, 项目上传成功!

> 可能会有bug导致失败, 不过没关系不一定要用这个的git功能.

现在打开第二个面板: Paclet Server Manager

如图, 依次输入:

```
b3m2a1
PacletServer
http://www.wolframcloud.com/objects/b3m2a1.paclets
```

![](https://pic2.zhimg.com/v2-727a64c1eea65f86c0aef7df0adc1911_b.jpg)
接着切换 Main 页面, (重大bug, 有很大概率死机).
![](https://pic3.zhimg.com/v2-1e674914b74b387472d529243208c076_b.jpg)
然后安装你要的软件包, (轻微bug, 有时候点了没用,多点两下)
![](https://pic1.zhimg.com/v2-2abfd194266cb4327ec39c72a53be354_b.jpg)
以上所有操作可能伴随巨量报错, 不过不严重不至于奔溃.

