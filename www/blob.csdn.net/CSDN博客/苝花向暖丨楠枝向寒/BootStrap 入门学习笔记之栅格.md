# BootStrap 入门学习笔记之栅格 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月08日 20:47:52[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：43


我对于栅格的理解就是css中的 浮动

例1：


![](https://img-blog.csdn.net/20180408200357705?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面代码写了 2遍

![](https://img-blog.csdn.net/20180408200525281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解释：  上面的 2行col-md-1 写在 container 的div 外面。红色是 container的边，下面两行写在了container里面。

说明问题： 

1、col-指的是横向将父层平分为 12 份 ，md-指的是 对于 小屏显示器（笔记本电脑） ，1 代表 12份中的1份的宽度。

2、上述例子我一共写了16个col-md-1，由于一行默认是12份、所以后4份被挤到下一行了，这就类似于div的浮动。

下面是 col-md-3  每个div占 3份的宽度

![](https://img-blog.csdn.net/20180408202253251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408202349996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

-----------------------------------------------------------------------------------------------------------------------------

问题： div 的 class=“row” 其中row的作用是什么呢？ 我把 row 改成了 a （随便改成什么都行，只要人家不存在这个class）

效果对比

![](https://img-blog.csdn.net/20180408203143500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面的 class=“row”，下面的class=“a” 

说明： 可以看出 row 的作用就是与父div 没有 外边距 。而自己 随便命名的class 就会有 外边距

-------------------------------------------------------------------------------------------------------------------------------

还是 关于 row的 作用 测试 

例1  class="row"

![](https://img-blog.csdn.net/20180408203655697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408203721258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

例2 上面的 div   class="a"

![](https://img-blog.csdn.net/20180408203822860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018040820384663?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

例3 下面的 div   class="a"

![](https://img-blog.csdn.net/20180408204022759?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408204051790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

说明： 

1、如果  class =row  该 栅格会 独占一行（col-md-6和4 被挤到了下面一行），并且会与 它上面的 类名 为 row的 div 自动空出 一部分距离 。 

2、栅格的宽度是 根据 占比 进行控制的、而高度则是文字内容占的高度控制的。

------------------------------------------------------------------------------------------------------------------------------

移动栅格 col-md-offset-1 向右移动一份 距离

上面是 移动前、下面是移动后

![](https://img-blog.csdn.net/20180408204657456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408204714145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

---------------------------------------------------------------------------------------------------------------------------------

push 和 pull

![](https://img-blog.csdn.net/20180408221209816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408221241322?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

说一下 push 和 offset 的区别 。 效果上看 没有区别。 

实际：**offset是直接在左侧补列达到偏移的效果，****push是通过位置移动达到移动的效果**

![](https://img-blog.csdn.net/20180408221434713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180408221444190?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



