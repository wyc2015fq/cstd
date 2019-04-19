# Tiled源码分析(四): 插件机制 - 逍遥剑客 - CSDN博客
2014年02月07日 23:26:13[xoyojank](https://me.csdn.net/xoyojank)阅读数：3139
Tiled作为一个通用的2D地图编辑器, 对于功能扩充做了一些考虑, 比例加载/输出第三方的文件格式. 不过分析下来, 能扩充的功能有限, 像UI面板之类的功能扩展就没有提供支持, 但是参考已经有的接口可以很方便地实现
# 插件原理
Qt本身就对插件机制提供了完善的支持, 对于应用程序, 插件一般是以DLL的方式存在, 在程序运行时动态加载. DLL中实现已经在主程序中定义好的抽象接口, 然后主程序这边只需要使用这些抽象接口去调用插件中实现的各种自定义功能了.
# 插件加载
通过QDirIterator用去遍历plugins目录下的文件, 使用QPluginLoader加载DLL, 并返回一个QObject对象
![](https://img-blog.csdn.net/20140207232702328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 插件管理
所有的插件在运行时都是以QObject的形式保存在PluginManager里, 并且都有一个对应的名字, 用查找索引
# 插件使用
因为插件中实现的QObject都实现了主程序中定义好的接口, 所以在访问相应接口功能时, 只需要进行动态类型转换, 就可以实现该对象有没有实现相应的接口了:
![](https://img-blog.csdn.net/20140207232336359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140207232330750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 总结
虽说自己开发的编辑器没什么必要采用插件形式扩充功能, 不过参考这种设计可以使功能模块之间的耦合度尽量降低, 也方便别人维护, 保证程序的健壮性
