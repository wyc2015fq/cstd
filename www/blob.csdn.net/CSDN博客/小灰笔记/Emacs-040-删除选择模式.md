# Emacs-040-删除选择模式 - 小灰笔记 - CSDN博客





2018年01月13日 18:35:39[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：298








Emacs-040-删除选择模式

            或许，标题中的选择改为选中会更好些，理解能够更到位。

            先描述一下功能修改的前提，默认情况下Emacs的表现。加入有如下文本Buffer，我选中了其中的一个词语。

            通常，我们使用其他编辑器留下来的思维是只要我按下一个按键，text文本会被替代。但是在Emacs默认的行为中，按下一个按键的效果如下：

![](https://img-blog.csdn.net/20180113183518383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            实现的效果不是替换，而是追加。如果需要进行替换功能的开启，那么就需要开启删除选中的模式。具体实现需要在配置文件中增加如下配置：

(delete-selection-mode t)

            配置完使其生效之后，编辑效果已经跟我们期待的一致。



