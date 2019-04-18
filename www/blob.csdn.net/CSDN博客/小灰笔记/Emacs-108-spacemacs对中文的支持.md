# Emacs-108-spacemacs对中文的支持 - 小灰笔记 - CSDN博客





2018年03月07日 21:28:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：502








       漫无目的地看文档，看到了一个比较有意思的或许也能解答我曾经困惑的标题——给spacemacs增加对中文的支持。

       不同的操作系统有不同的处理方法，这里小结Mac的使用方法。首先安装小企鹅的模拟器：


brew install fcitx-remote-for-osx--with-input-method=baidu-pinyin


       接下来，安装chinese-layer，这个只需要修改配置文件并且重启。修改如下；


![](https://img-blog.csdn.net/20180307212719847)



       修改点在52行，重启之后可以看到部分插件的安装编译。

       接下来，修改如下配置后重启：




(setq-defaultdotspacemacs-configuration-layers '((chinese :variables

chinese-enable-fcitx t)))


其他的诸如字典等设置，我觉得倒是没有太大的必要。





