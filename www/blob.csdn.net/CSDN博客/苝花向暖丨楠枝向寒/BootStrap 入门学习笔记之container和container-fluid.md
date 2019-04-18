# BootStrap 入门学习笔记之container和container-fluid - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月08日 19:56:52[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：208


BootStrap重写了 css的一些标签，比如p标签，默认字体大小 等等。而且预先写好了一些 CSS样式，我们可以给 div加指定的标签，来用人家预先写好了的样式，进行布局 ，装饰等等。

class=“container”

例子1：

![](https://img-blog.csdn.net/20180408193521775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408193607624?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

例子2：

![](https://img-blog.csdn.net/20180408193740328?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408193806634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过上述两个例子 可以看出、container 默认居中，如果没有设置宽度、默认在浏览器左右两边留出固定的距离。如果设置了宽度，则div变为 设置的宽度、同时保证居中、然后 左右边距动态改变。  如果没有设置高度、那么div高度则是文字内容高度，设置了就是所设置的高度。

container 和 container-fluid 对比

![](https://img-blog.csdn.net/20180408195152678?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408195301408?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

更改屏幕显示百分比

![](https://img-blog.csdn.net/20180408195437457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看出 container 距浏览器两边的宽度是固定的、而container—fluid 是屏幕占比100%

再附上一张编译之后的 bootstrap 代码，证明我的观点

![](https://img-blog.csdn.net/20180408195503848?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


