
# Kconfig中的“depends on”和“select” - 嵌入式Linux - CSDN博客

2019年02月21日 09:50:30[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：66个人分类：[Makefile																](https://blog.csdn.net/weiqifa0/article/category/8686212)



在`Kconfig`文件中：
`config A
    depends on B
    select C`它的含义是：`CONFIG_A`配置与否，取决于`CONFIG_B`是否配置。一旦`CONFIG_A`配置了，`CONFIG_C`也自动配置了。
参考资料：
[“select” vs “depends” in kernel Kconfig](https://unix.stackexchange.com/questions/117521/select-vs-depends-in-kernel-kconfig)。
![](https://img-blog.csdnimg.cn/20190221095138521.png)
所以去找在哪里开了SND_SOC_HDMI_CODEC的 select

