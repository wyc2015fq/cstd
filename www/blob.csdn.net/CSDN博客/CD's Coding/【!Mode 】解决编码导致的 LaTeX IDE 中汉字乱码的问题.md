# 【!Mode::】解决编码导致的 LaTeX IDE 中汉字乱码的问题 - CD's Coding - CSDN博客





2019年03月20日 11:45:54[糖果天王](https://me.csdn.net/okcd00)阅读数：25








## 0x00 前言

最近想试着用LaTeX写写作业，也是为了方便未来写论文的时候可以不那么手忙脚乱，

从IDE开始就陷入了选择困难，最终选定使用 CTex 和 TeXStudio 这两者，

CTeX 一直没有更新过了，是官网的那个 [CTeX_2.9.2.164_Full.exe (1.31G)](http://www.ctex.org/CTeXDownload)，

TeXStudio 则用的是 [texstudio-2.12.14-win-qt5.exe (74.2M)](http://texstudio.sourceforge.net/)
而说到为什么我装了 CTeX 还要装 TeXStudio，就开始进入正题了——

因为我当时手头有个模板 CTeX 打开跑不起来！我就装了一个 TeXStudio 就跑起来了……

事后才发现是编码问题：- CTeX 默认采用和 Windows 系统一致的 GB2312 编码
- TeXStudio 默认采用最广为使用的 UTF-8 编码
- 用 Notepad++ 打开模板之后发现，原来模板是 UTF-8 编码的

## 0x01 代码层面解决方案

> 
其实 CTeX 也是可以处理 UTF-8 编码的，但是你得告诉它你让他读的这个文件是 UTF-8 的才行。


我们可以在 .tex 文件的开头加一行，用来告知 CTeX 这个文件是用 UTF-8 编码的，

这个操作是不是很眼熟？Python 里面也有 `# coding: utf-8` 这样的操作，

TeX 里的其实差不多，也是需要在代码首行写上 `% !Mode:: "TeX:UTF-8"` ，形似 ：
```
% !Mode:: "TeX:UTF-8"
\documentclass[11pt]{article}
\usepackage[a4paper]{geometry}
\geometry{left=2.0cm,right=2.0cm,top=2.5cm,bottom=2.5cm}
```

这样一来，就可以在 CTeX 里正确的读取执行使用 UTF-8 编码的文件啦！

## 0x02 IDE层面解决方案

> 
既然 “默认” 按照某种编码方式来读入，那么我们修改掉这个 “默认” 不就好啦。


这里我主要还是想要向比较主流（主流的意思是组里的学长们都在用）的 CTeX 看齐，

所以就倒腾一下 TeXStudio，让它在 GB2312 和 UTF-8 编码时都可以和 CTeX 对齐。

（其实就是我喜欢 TeXStudio 的右侧预览功能，原因也简单 —— 就是好看！）
那么 TeXStudio 的默认编码修改其实也简单：

Option -> Configure TeXstudio -> Editor -> Default Font Encoding

选项 -> TeXstudio 配置 -> 编辑器 -> 默认编码
![Configure TeXstudio](https://img-blog.csdnimg.cn/20190320111215123.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)
## 0x03 碎碎念

刚上手 LaTeX，感觉是真的有点复杂，对于按照自己的理解写的东西也不知道对不对，

如果有哪里不对或者是需要补充说明的地方请各位不吝赐教，非常感谢~

## 0xFF IDE界面

> 
为了给还没有开始 LaTeX 学习的小伙伴做个简单的介绍，

把自己手头有的 TeXStudio 和 CTeX(WinEdt) 的界面给大家展示下好啦

顺带一提：右侧的PDF生成预览，都是这俩IDE自带的，装了就有。
![TeXStudio](https://img-blog.csdnimg.cn/20190320105128826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)
![CTeX](https://img-blog.csdnimg.cn/20190320105426787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)










