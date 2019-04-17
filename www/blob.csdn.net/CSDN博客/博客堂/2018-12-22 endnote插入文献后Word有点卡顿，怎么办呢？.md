# 2018-12-22 endnote插入文献后Word有点卡顿，怎么办呢？ - 博客堂 - CSDN博客





2018年12月22日 16:13:18[最小森林](https://me.csdn.net/u012052268)阅读数：703











### 文章目录
- [题目：插入文献后Word有点卡顿，怎么办呢？](#Word_5)
- [0 引言](#0__7)
- [1 无效方法 × ：](#1____12)
- [1.1 禁用Endnoteword加载项载 ×](#11_Endnoteword__16)
- [1.2 取消检查语法 ×](#12___22)
- [2 比较好的解决办法](#2__32)
- [2.1 原来卡的情形：](#21__40)
- [2.2 正确办法：把引用格式转为非正式](#22__48)
- [2.3 返回正常的引用格式](#23__67)
- [3 找到了官网 解决办法：2019-04-12更新](#3__20190412_84)




> 
参考：知乎徐小聪（[https://zhuanlan.zhihu.com/p/32788201）](https://zhuanlan.zhihu.com/p/32788201%EF%BC%89)


# 题目：插入文献后Word有点卡顿，怎么办呢？

## 0 引言

有木有小伙伴们在使用endnote x7 插入文献的时候，发现插入$^{[1]}$之后，Word立马变得好卡！！对于写论文是一件极其痛苦的事情，那今天就介绍一种较好的解决办法，亲测好用。

## 1 无效方法 × ：

首先介绍一些网络上常见但不太有效方法：

### 1.1 禁用Endnoteword加载项载 ×

第一种方法是在word加载项中 禁用Endnote自动加载。

但是事实证明，该方法作用不大。。。还会导致endnote使用出现不可预知的问题。

### 1.2 取消检查语法 ×

第二种方法是取消word检查语法错误。

> 
在word中更正拼写和语法时 → 键入时标记语法错误。


![image](https://img-blog.csdn.net/20150318160617727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmd3ZWlqaXFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

有点用，但这种方法属于头疼却医脚。

## 2 比较好的解决办法

较为合适的解决办法：
- 换一种“非正式”endnote引用格式，word就变得一点也不卡；
- 等到全文写好之后，在转换为正式endnote引用格式 “ 正文引用的$^{[1]}$”这种形式。

下面分布具体说明：

### 2.1 原来卡的情形：

下图中是正常从endnote中插入文献的形式。

文中会有引用**文章的序号**，参考文献会在**文章末尾**统一列出来，但这样子的话，电脑经常会**变得卡顿**。

![image](https://pic1.zhimg.com/80/v2-bc979781744cc583e8066711bffa5f14_hd.jpg)

### 2.2 正确办法：把引用格式转为非正式

正确办法第一步：把引用格式转为非正式

点击：

> 
convert citations and bibliography --> convert to unformed citations。


点击方式如下图：

![image](https://pic3.zhimg.com/80/v2-e3678375c46e34127a3c0a83b8f78f0a_hd.jpg)

点击之后，文中引文就会变成下图样式：

参考文献**暂时不**在Word中显示$^{[1]}$，而是会在后面**列上论文的作者**和时间。

![image](https://pic2.zhimg.com/80/v2-385bf3f2c5a705e8b59286f05d26d56d_hd.jpg)

这样做的好处：即按照endnote的逻辑插入了文献（方便复原），又解决了卡顿的问题（两台不同电脑亲测）。

### 2.3 返回正常的引用格式

正确办法第二步：把非正式格式转为正式引用格式，

论文还要发表怎么能用这种非正式的引用格式呢？？ ？别着急，这种格式虽然不太美观，但是保持了endnote的引用逻辑，只需要点击相应的恢复按钮，即可瞬间恢复正常的引用格式$^{[1]}$。方法如下：
- 等文章全部写好之后 点击**convert citations and bibliography–convert Word citations to endnote** 如下图：

![image](https://pic4.zhimg.com/80/v2-174c97b57c4f28b58f0f17096934b01f_hd.jpg)
- 再点击更新引用格式：**update citations and bibliography**，如下图：

![image](https://pic2.zhimg.com/80/v2-bc494448e30c57c00440f4ac8e9b2c2d_hd.jpg)

此时 ，论文中的引用文献又变成了正常的引用格式$^{[1]}$。（当然此时word又变得很卡 (T.T)…  ）。

但是总归是能体面的好好写文章了，，，下一步word保存为PDF准备发表吧。

## 3 找到了官网 解决办法：2019-04-12更新

找到了endnote官网 解决办法：[https://support.clarivate.com/Endnote/s/article/EndNote-Problematic-field-codes-causing-jumping-cursor-the-range-cannot-be-deleted-or-other-problems-while-formatting?language=en_US](https://support.clarivate.com/Endnote/s/article/EndNote-Problematic-field-codes-causing-jumping-cursor-the-range-cannot-be-deleted-or-other-problems-while-formatting?language=en_US)

导致问题的原因是，有像域代码之类的交互，与endnote冲突。解决方法把word文档清除一下，即将域代码删除。

步骤有以下六部：
- Make a backup of your document.
- In Word 2010/2013/2016 with Endnote X4 or later, go to the EndNote tab and choose the “Convert Citations and Bibliography > Convert to Unformatted Citations” command.。。。

This will remove the reference list and revert the citations to a temporary format.
- Press [Ctrl]+A on the keyboard to highlight everything.
- Press [Ctrl]+6 (above the “T” and “Y” key) to remove any additional hidden field codes.
- Press [Ctrl]+C to copy the highlighted text.
- Open a new document and press [Ctrl]+V to paste.

中文方法：
- 备份你的文档
- 将论文引用转为非正式格式。（参见本文第二章）
- Ctrl+A 全选
- Crtl+6 去除域代码 （这个是关键步骤）
- Ctrl+C 复制到新的文档中去。
- Ctrl+V 复制到新的文档中去。

实际操作起来 有个更加神奇方法：
- Ctrl+A 选择前面所有的文本
- ctrl+6 去除域代码
- 这个时候目录的超链接就不能用了，参考文献也变成“纯文本”了
- 这个时候按ctrl+z 撤销刚刚的操作，这个时候神奇的发现恢复了目录链接状态。 但是神器的是参考文献也不卡了，就好像什么也没有发生。。。

这其中具体发生了什么，还不清楚，等到以后再补充。



