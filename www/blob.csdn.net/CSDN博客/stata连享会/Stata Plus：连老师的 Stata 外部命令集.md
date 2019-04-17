# Stata Plus：连老师的 Stata 外部命令集 - stata连享会 - CSDN博客





2018年12月05日 14:38:11[arlionn](https://me.csdn.net/arlionn)阅读数：185
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)













### [「原项目主页-更新中」](https://gitee.com/arlionn/StataPlus/blob/master/README.md)



#### 介绍
- 内容：存放了自 2003 一年以来我下载的所有外部命令。
- 更新时间：`2018/9/18 10:51`
- 下载地址 1：百度云盘  [https://pan.baidu.com/s/1-nvAG8ZDihWcKakViug_QQ](https://pan.baidu.com/s/1-nvAG8ZDihWcKakViug_QQ)
- 下载地址 2：百度网盘： [https://pan.baidu.com/s/1ZT3HQjkYMB83ihCnbeTZ1Q](https://pan.baidu.com/s/1ZT3HQjkYMB83ihCnbeTZ1Q)
- 命令清单：[- 点击查看 -](https://gitee.com/arlionn/StataPlus/blob/master/lian_plus_tree.txt)

#### 使用方法

下载 **plus.rar** 后，与你的 plus 文件夹合并或直接覆盖你的 plus 文件夹。
- **方法1：** 下载 [「plus.rar」](https://pan.baidu.com/s/1-nvAG8ZDihWcKakViug_QQ) 到本地，解压后，放置于 **D:\stata15\ado** 文件夹下即可。若有自建的 **plus** 文件夹，可以将二者合并，或者直接覆盖 (我的外部命令应该更全面一些)。
- **方法2：** 若想同时保留你自己的 **plus** 和我提供的 **plus** 文件夹，则可以将我的重命名为 **plus2**，然后在 **[profile.do](http://profile.do)** （存放于 `D:\stata15` 目录下）添加如下语句：`adopath + "D:\stata15\ado\plus2"`(绝对路径)，或者 `adopath + "c(sysdir_stata)\ado\plus2"` (相对路径)。重启 Stata 后即可保证 **plus2** 中的命令生效。

输入 `sysdir` 可以查看你的 plus 文件夹存放于何处。我的文件路径如下：

```
. sysdir
   STATA:  D:\stata15\
    BASE:  D:\stata15\ado\base\
    SITE:  D:\stata15\ado\site\
    PLUS:  D:\stata15/ado\plus\
PERSONAL:  D:\stata15/ado\personal\
OLDPLACE:  c:\ado\
```

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-f70a8626f8e16880.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



