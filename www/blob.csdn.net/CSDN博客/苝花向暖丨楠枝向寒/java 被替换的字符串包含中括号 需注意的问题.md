# java 被替换的字符串包含中括号 需注意的问题 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月30日 00:01:58[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：1143


![](https://img-blog.csdn.net/20180729235544949?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如上图所示，我想将 "[small]" 这部分 替换为字母b，但是确将中括号里面的 “small” 每一个字母都替换成了b ，此时我想到了 java中的 中括号是特殊字符，正则表达式当中会用到。然后 我就想到了 用 "\" 来进行转义。

然后就如下图所示直接编译期报错。

![](https://img-blog.csdn.net/20180729235834775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解决方式如下图所示

![](https://img-blog.csdn.net/20180729235922493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后我又想到，当我们用到磁盘路径的时候， "\\" 和 "/" 的意义是一样的，然后我就改成这样

![](https://img-blog.csdn.net/20180730000035901?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

发现并没卵用，看来"\\" 和 "/" 只有在表示文件物理路径的时候才具有相同的意义。

