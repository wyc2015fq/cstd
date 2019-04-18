# Emacs-032-关闭当前Buffer - 小灰笔记 - CSDN博客





2018年01月07日 10:58:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1478








                
Emacs-032-关闭当前Buffer
关闭当前Buffer，其实叫做杀死当前Buffer可能会更直接一点。因为在Emacs的设计中，相应的函数使用了kill。
这个功能现在我使用的十分频繁，或许主要的原因还在于我学到的东西少，技能比较肤浅。其实，更多时候我使用这个功能仅仅是为了能够切换到上一个打开的Buffer中。或许，后期能够掌握管理一个工程中的多个文件并且方便切换的时候这个功能使用的也就不多了。
最初，我还曾经把这个功能跟删除光标位置到句尾的功能混了。删除当前光标位置到句尾的操作为C-k，而这个是C-x k。
进行操作的时候，能够看到如下提示：
![](https://img-blog.csdn.net/20180106203802493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果确认，直接回车即可。
            


