# 087_emacs设置空白显示 - 小灰笔记 - CSDN博客





2019年04月12日 21:50:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：10








         本来是想设置一个多余空白显示的，但是找到这个功能之后发现其实内置的这个功能是兼具空白显示功能的。

         这个功能说起来我之前应该是需求不大，但是使用了一阵子spacemacs之后，对其中默认开始的高亮显示多余空白的功能给绑住了。但是，臃肿的spacemacs感觉在Windows上还是不如自己的配置方便。于是，就想在我自己的配置中增加这么一个功能。

![](https://img-blog.csdnimg.cn/20190412214940375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         其实，emacs内置了这个功能。使用whitespace-mode可以实现这个功能的开关切换，如果指定了参数的话可以开启或者关闭。但是后来发现这个功能无法全局有效。再次搜索，锁定了以下内容。

![](https://img-blog.csdnimg.cn/20190412214940745.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         增加配置：

(global-whitespace-mode t)

         生效之后，全局显示OK。效果如下：

![](https://img-blog.csdnimg.cn/20190412214940787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190412214940794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个效果，比不上spacemacs的简洁，但是应该也是足够我的需求应用了。



