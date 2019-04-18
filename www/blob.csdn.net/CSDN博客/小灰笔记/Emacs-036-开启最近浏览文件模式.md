# Emacs-036-开启最近浏览文件模式 - 小灰笔记 - CSDN博客





2018年01月13日 18:24:13[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：205








                
Emacs-036-开启最近浏览文件模式
很多软件都有类似的功能，一般是File菜单下能够显示几次最近浏览的文件的快捷访问方式。这个功能还是挺有用的，能够快速切入到最近的工作中去。
Emacs中也支持最近浏览文件便捷访问的方式，配置方式只需要在配置文件中增加如下配置：

(recentf-mode t)

(setq recentf-max-menu-items 10)

其中，第一个是开启了这个模式。而第二个则是指定了最近访问过的文件的最大个数。配置之后，Emacs的File菜单下拉中会增加一个选项内容。具体功能使用的情况如下：

![](https://img-blog.csdn.net/20180113182335211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


从上面看，其实这个功能还有几个其他的附属功能可以使用。随着配置功能接触的不断增加，我越来越觉得Emacs还是挺好玩的了。
            


