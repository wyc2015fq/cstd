# 大数据学习笔记(五) -- linux中vi编辑器的快捷键以及正则表达式 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月23日 17:50:45[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：35


**Table of Contents**

[文本操作技巧](#%E6%96%87%E6%9C%AC%E6%93%8D%E4%BD%9C%E6%8A%80%E5%B7%A7)

[打开文件关闭文件](#%E6%89%93%E5%BC%80%E6%96%87%E4%BB%B6%E5%85%B3%E9%97%AD%E6%96%87%E4%BB%B6)

[从编辑模式到输入模式的常用操作](#%E4%BB%8E%E7%BC%96%E8%BE%91%E6%A8%A1%E5%BC%8F%E5%88%B0%E8%BE%93%E5%85%A5%E6%A8%A1%E5%BC%8F%E7%9A%84%E5%B8%B8%E7%94%A8%E6%93%8D%E4%BD%9C%C2%A0)

[在编辑模式下移动光标的快捷键](#%E5%9C%A8%E7%BC%96%E8%BE%91%E6%A8%A1%E5%BC%8F%E4%B8%8B%E7%A7%BB%E5%8A%A8%E5%85%89%E6%A0%87%E7%9A%84%E5%BF%AB%E6%8D%B7%E9%94%AE%C2%A0)

[在编辑模式下，进行复制粘贴撤销操作](#%E5%9C%A8%E7%BC%96%E8%BE%91%E6%A8%A1%E5%BC%8F%E4%B8%8B%EF%BC%8C%E8%BF%9B%E8%A1%8C%E5%A4%8D%E5%88%B6%E7%B2%98%E8%B4%B4%E6%92%A4%E9%94%80%E6%93%8D%E4%BD%9C%C2%A0)

[末行模式](#%E6%9C%AB%E8%A1%8C%E6%A8%A1%E5%BC%8F%C2%A0)

[正则表达式（grep）](#grep)

# 文本操作技巧

## **打开文件关闭文件**

![](https://img-blog.csdn.net/20180923160657307?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```bash
[root@node001 ~]# vi +22 profile     # 打开且光标移动至22行
[root@node001 ~]# vi + profile       # 打开且光标移动至最后一行
[root@node001 ~]# vi +/after profile   # 打开且光标移动第一次出现单词after的位置
```

## **从编辑模式到输入模式的常用操作 **

![](https://img-blog.csdn.net/2018092316133952?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## **在编辑模式下移动光标的快捷键 **

![](https://img-blog.csdn.net/2018092316395888?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## **在编辑模式下，进行复制粘贴撤销操作 **

![](https://img-blog.csdn.net/20180923165458702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解释：
- 复制粘贴 是 yw 或 yy ，然后 p或 P ，如果是剪切的话，就是先 dd 或dw 再 p 或P 。
- 上面的命令是可以跟数字结合的，如3dd ，删除3行内容，从光标所在行开始。3dw删除3个单词，从光标所在字符开始。
- 也可使用 dG 命令，代表从当前行开始将下面的内容全部删除。

## 末行模式 

![](https://img-blog.csdn.net/20180923172453822?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

额外用法：
-  输入冒号进入 末行 模式， 输入   num1，num2d 可以删除 num1行到num2行的内容。
-  输入冒号进入 末行 模式， 输入   num1，num2y 可以复制 num1行到num2行的内容。
-  输入冒号进入 末行 模式， 输入   num1，&-2d 可以删除 num1行到倒数第二行的内容。

![](https://img-blog.csdn.net/20180923174023691?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 用法示例：

![](https://img-blog.csdn.net/20180923174700841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们将 第2行开始到第20行之间的所有 etc 替换成 abc ，注意当前的光标在第16行，那么我们可以使用如下命令

```bash
2,.+4s/etc/abc/g
```

效果如下

![](https://img-blog.csdn.net/20180923174733266?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# **正则表达式（grep）**

我们可以通过grep命令 来匹配某文本中的某行或某些行是否 与 我们给出的 正则表达式相匹配，然后显示出所匹配的行内容

正则表达式中的操作符分为两种，一种是基本操作符，一种是扩展的，下面有两张图黑色字体的为基本操作符，红色字体的为扩展操作符。使用时扩展操作符需要在 操作符前面加上 \  进行转义，或者在 grep 后面加上 -E 参数

** 正则表达式常用的操作符**

![](https://img-blog.csdn.net/201809232011048?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/201809231942046?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 下面是几个示例

```bash
[root@node001 ~]# cat grep 
ojasoidjsaoidj1w3213ddhelloas
d12e12js21s212
dwqdoi32132dasd
hello/sadq213213hi dsadsadsahi dsamk
hello dcxzcjuhicenruqehello sdadshi
czxczxc1321kjbckhqweqwk
1dasfs hello das
```

找出grep文件中包含数字的行

```bash
[root@node001 ~]# grep "[0-9]" grep 
ojasoidjsaoidj1w3213ddhelloas
d12e12js21s212
dwqdoi32132dasd
hello/sadq213213hi dsadsadsahi dsamk
czxczxc1321kjbckhqweqwk
1dasfs hello das
```

找出grep文件中包含字符串hello的行

```bash
[root@node001 ~]# grep "hello" grep 
ojasoidjsaoidj1w3213ddhelloas
hello/sadq213213hi dsadsadsahi dsamk
hello dcxzcjuhicenruqehello sdadshi
1dasfs hello das
```

找出grep文件中包含 hello 单词的行

```bash
[root@node001 ~]# grep "\<hello\>" grep 
hello/sadq213213hi dsadsadsahi dsamk
hello dcxzcjuhicenruqehello sdadshi
1dasfs hello das
```

找出grep文件中包含连续两个 "dsa" 字符的行 

```bash
[root@node001 ~]# grep "\(dsa\)\{2\}" grep 
hello/sadq213213hi dsadsadsahi dsamk
[root@node001 ~]# grep -E "(dsa){2}" grep 
hello/sadq213213hi dsadsadsahi dsamk
```

找出grep文件中 包含 "hello" 隔若干字符  "hi"  的行

```bash
[root@node001 ~]# grep -E "(hello).*(hi)" grep 
hello/sadq213213hi dsadsadsahi dsamk
hello dcxzcjuhicenruqehello sdadshi
```

找出grep文件中 包含 "hello" 隔若干字符 "hi" 隔若干字符 "hello" 隔若干字符 "hi" 接若干字符的 行

```bash
[root@node001 ~]# grep -E "(hello).*(hi).*\1.*\2" grep 
hello dcxzcjuhicenruqehello sdadshi
```

