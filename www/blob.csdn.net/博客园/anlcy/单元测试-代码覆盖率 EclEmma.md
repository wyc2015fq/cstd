
# 单元测试-代码覆盖率 EclEmma - anlcy - 博客园






# [单元测试-代码覆盖率 EclEmma](https://www.cnblogs.com/camilla/p/7126621.html)
1. EclEmma的介绍
EclEmma是一个开源的软件测试工具，可以在编码过程中查看代码调用情况、也可以检测单覆盖率。
2. Eclipse下EclEmma安装
> 1. 选择Help->Eclipse Marketplace->搜索EclEmma，Install；

> 2. 重启eclipse发现工具栏上出现Coverage图标，说明安装成功；
![2](https://images0.cnblogs.com/blog/382323/201405/091608542761772.jpg)
3. EclEmm
#### 3.1 EclEmma查看代码调用情况
> 1. 新建一个项目，然后添加一个类，然后简单书写一些代码；

> 2. 右键项目->选择Coverage As->Java Application，可以得到如下结果：
![3](https://images0.cnblogs.com/blog/382323/201405/091643245261711.jpg)
> 3. 从运行结果可以看到，有多种颜色，其中

> 绿色表示代码被执行到

> 黄色表示代码部分执行到

> 红色表示代码没有被执行到

#### 3.2 EclEmma检测覆盖率
> 1. 选择Window->Show View->Other->Java->Coverage可以看到代码执行的覆盖率；
![33](https://images0.cnblogs.com/blog/382323/201405/091643252298326.jpg)
> 2. 其中可以看到每一个类中代码被执行的百分比，见2，也可以看到整个项目代码被执行的百分比，见1；

> 3. 其中检测覆盖率可以用到单元测试中，查看单元测试覆盖率。






