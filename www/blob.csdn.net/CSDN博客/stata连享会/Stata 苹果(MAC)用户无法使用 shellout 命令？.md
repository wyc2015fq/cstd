# Stata: 苹果(MAC)用户无法使用 shellout 命令？ - stata连享会 - CSDN博客





2019年01月12日 10:04:31[arlionn](https://me.csdn.net/arlionn)阅读数：286
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)








- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
A: 当然可以！


> - **方法1：** 使用 `!open` 命令替代 Windows 系统下的 `shellout` 命令；好处是，无需下载任何新命令。
- **方法2：** 使用暨南大学 [程振兴](https://www.czxa.top/) 写的 `copen` 命令。该命令可以同时在 Windows 和 Mac 系统下运行，完美替代此前的 `shellout` 命令。但对于 Windows 老用户而言 (以后也不准备使用 Mac)，没有必要使用该命令。


### copen：另一个 shellout （both Windows and Mac）
- **优点：** 同时适用于 Mac 和 Windows 的打开工具
- **Source:** 转自 [程振兴博客](https://www.czxa.top/)  → [copen：适用于Mac和Windows的打开工具](https://www.czxa.top/posts/58447/)

#### 文件下载

[copen.ado](https://www.czxa.top/asset/copen.ado)
[copen.sthlp](https://www.czxa.top/asset/copen.sthlp)

**使用方法：** 保存到 Stata 安装目录下的 `..\plus\c` 文件夹中即可。(Note: 可以使用 `sysdir` 查看你的文件路径)。

#### 高级设定 (未测试，风险自担)

**苹果用户** 使用 `shellout` 功能的低成本解决方案：
- 用 `copen` 的代码替换 `shellout` 的代码，覆盖掉 `shellout.ado` 即可。好处是不用更改我讲义中的代码。


否则，你就只能将我讲义中的所有 `shellout` 都替换成 `!open`  了。





### 附：适用于 Mac 和 Windows 的 打开工具 (程振兴)

这个命令是从 `outreg2` 包中的 `shellout` 命令修改而来。`shellout` 命令只能用于Windows 系统，因此为了方便自己使用，我对其进行了稍微的改写使之也适用与 Mac 系统。

运行 `ssc install outreg2` 命令即可安装 `shellout` 命令，打开 `shellout.ado` 即可看到它的源代码：

```
program define shellout
version 7.0

syntax [anything] [using/] [,cd]

* does the shelling
if "`c(os)'"=="Windows" | "$S_MACH"=="PC" {
        if "`using'"~="" {
                winexec cmd /c start ""  "`using'"
        }
        else {
                if "`cd'"~="cd" {
                        cap winexec `anything'
                        if _rc==193 {
                                winexec cmd /c start ""  "`anything'"
                        }
                        if _rc==601 {
                                noi di in yel "Cannot find `anything'. Make sure typed the name correctly."
                        }
                }
                else {
                        winexec cmd /c cd `c(pwd)'\ &  `anything'
                }
        }
}
else {
        * invisible to Stata 7
        local Version7 ""
        cap local Version7 `c(stata_version)'
        
        if "`Version7'"=="" {
                * stata 7
        }
        else {
                * non-PC systems
                di "{opt shellout} probably will not work with `c(os)'"
                shell `using'
        }
}
end
```

很容易发现，在这个程序里面进行了操作系统的判断，针对于非Windows系统的电脑会显示运行else部分，即无法使用，所以我们只需要改造一下else部分即可。Mac的shell命令——open有类似的功能，所以就直接放进去就好了，即：

```
*! 打开指定文件、网址、文件夹
*! 程振兴 2018年7月13日
cap prog drop copen
prog define copen
version 7.0
syntax [anything] [using/] [,cd]
if "`c(os)'"=="Windows" | "$S_MACH"=="PC" {
 if "`using'"~="" {
 winexec cmd /c start ""  "`using'"
 }
 else {
 if "`cd'"~="cd" {
 cap winexec `anything'
 if _rc==193 {
 winexec cmd /c start ""  "`anything'"
 }
 if _rc==601 {
 noi di in yel "找不到`anything'."
 }
 }
 else {
 winexec cmd /c cd `c(pwd)'\ &  `anything'
 }
 }
}
else {
 !open "`anything'"
}
end
```

为了区别原来的shellout命令，我将这个命令为`copen`。

> 
**文件下载：**


[copen.ado](https://www.czxa.top/asset/copen.ado)
[copen.sthlp](https://www.czxa.top/asset/copen.sthlp)





> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-b7c8baabd25a455e.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



