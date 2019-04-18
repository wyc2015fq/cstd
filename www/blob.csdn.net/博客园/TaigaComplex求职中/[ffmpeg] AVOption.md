# [ffmpeg] AVOption - TaigaComplex求职中 - 博客园







# [[ffmpeg] AVOption](https://www.cnblogs.com/TaigaCon/p/10182169.html)





在ffmpeg中，常使用AVOption的API来进行参数设置。AVOption的API主要分为[设置参数](https://ffmpeg.org/doxygen/3.3/group__opt__set__funcs.html)以及[提取参数](https://ffmpeg.org/doxygen/3.3/group__opt__get__funcs.html)两种，无论是哪一种API都主要分为两大步骤：
- 寻找出参数所在的内存位置。
- 如果是设置参数，就先把参数转换为正确的数据类型，然后把参数写入内存位置；如果是提取参数，就先从内存位置提取出参数，然后把参数转换为输出所需的数据类型。

本文主要讨论第一步，即定位参数所在的内存位置（[av_opt_find2](https://ffmpeg.org/doxygen/3.3/group__avoptions.html#ga3e121fd0b44b34e31756f4fe848d29a7)）。



# 可被进行参数查找的结构

在调用av_opt_set这一类函数的时候，会传入一个结构体，如AVCodecContext，AVFormatContext，AVFilterContext，SwrContext，SwsContext等。这些是可以被用于参数查找的结构体，这些结构体都有一个特征：以AVClass作为结构体的头部。

AVClass主要作用有两个：提供用于参数查找所必须的材料；指示所在的结构体相关的log显示样式（如log level，log颜色等）。

AVClass中用于参数查找所必须的材料有两个：
|option|用于保存描述参数项的列表，即AVOption数组。|
|----|----|
|child_next|如果所查找的结构体中存在可被进行参数查找的子结构体，那么child_next函数应该返回指向该子结构体的指针，如此一来就可以通过子结构体进行参数查找。|



![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181226221215114-1567360607.png)





# 定位参数所在的内存地址

在定位参数内存地址的时候都会用到AVOption，其中在定位时用到的AVOption中比较重要的成员有：
|name|参数名称，用于与av_opt_set所输入的参数名称进行匹配，如果名称相同则表示成功找到该参数。|
|----|----|
|offset|参数所在位置与目标结构体头部的位置差。|

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181226221215786-728202521.png)

定位参数的内存地址可以分为两个步骤：
- 定位结构体时。去查看该结构体中指向的option列表，如果列表中有与输入的名称同名的参数，则表明匹配成功，那么该option列表所在的结构体就是我们所定位的结构体。如果在调用av_opt_set的时候指定了AV_OPT_SEARCH_CHILDREN，还会递归地去查找子结构体中的option列表。
- 在得到了目标结构体的地址后，加上option中的offset后就能得到目标参数的内存地址。












