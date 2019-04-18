# 在线实时大数据平台Storm版本兼容的问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月13日 17:23:50[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3487
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









部署了storm1.0.1最新版，但原来生产的程序是storm0.8.2版本并在该版本环境中运行，直接将程序放到1.0.1环境中storm jar运行失败。




重构程序，引入storm-core-1.0.1.jar，替换storm-0.8.2.jar，代码中主要import中包头org.apache替换backtype，可能是1.0版本后被apache网罗了，所以包头改为ora.apache，下面具体的类都不变，函数也暂时没有发现有大变化。




特别要注意的是：打包工程时，不能带jar，包括storm-core-1.0.1.jar（会提示default.yaml文件冲突），所以第三方jar需要先放到storm1.0.1安装目录下的lib或extlib或extlib-daemon，三者有什么区别暂时还不知道，但试验过，只要jar放到任何一个都有效。



