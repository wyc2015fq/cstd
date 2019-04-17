# R语言︱R社区的简单解析（CRAN、CRAN Task  View） - 素质云笔记/Recorder... - CSDN博客





2016年04月20日 10:24:23[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4568
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)












笔者寄语：菜鸟笔者一直觉得r CRAN离我们大家很远，在网上也很难找到这个社区的全解析教程，菜鸟我早上看到一篇文章提到了这个，于是抱着学渣学习的心态去看看这个社团的磅礴、威武。




CRAN（The Comprehensive R Archive Network）：全面R档案网站




# 1、R社区——主界面：https://www.r-project.org/

主要功能：提供下载、最新版本的信息






# 2、R社区——CRAN Mirrors：https://cran.r-project.org/mirrors.html

主要功能：世界各地镜像地，挑一个离你近的。[R镜像是为了方便世界各地的使用者下载R软件及相关软件包，在各地设置的镜像。各地的镜像都是R网站的备份文件，完全一样。所以，选择离你最近的镜像，你下载R软件或R包的速度相对较快。](http://zhidao.baidu.com/link?url=vFwo_5RXJEBro5wk-rzEI1tV5Fk7Vk6KrMIXt5vLPC_f0z6VMlFIf8V0_IICk6dcfUxGkLWhAZUlVf_F64U3CNBEPFIrBZ-qIpt5tXWpBG3)




# 3、R社区——Contributed Packages：https://cran.r-project.org/web/packages/

主要功能：已有多少函数包（20160420有8280个）、[按时间排列](https://cran.r-project.org/web/packages/available_packages_by_date.html)、[按出版名称排列](https://cran.r-project.org/web/packages/available_packages_by_name.html)、安装包的方法、[手动安装包的方法](https://cran.r-project.org/manuals.html#R-admin)、[自己编包的一些规则](https://cran.r-project.org/manuals.html#R-exts)







# 4、R社区——CRAN Task Views：https://cran.r-project.org/web/views/

主要功能：包按功能分类，比如按计量经济学、贝叶斯、时间序列、临场试验、概率分布、空间、时空、生存等方面把包进行分类


——————————————————————————————————————————————————————————————






# 其他延伸




生物统计家园有一些关于包的中文解释：http://www.biostatistic.net/forum-144-1.html

解读:不太起眼的包的中文解读，还是能在一定程度上帮助你理解这个包



# [R中的“CRAN Task Views”功能](http://blog.csdn.net/mjk/article/details/6229737)：http://blog.csdn.net/mjk/article/details/6229737






```
如果要使用这个功能，需要先下载包ctv：
install.packages('ctv")
然后使用如下命令：
install.views("Econometrics")
这样就可以将计量经济学范畴的包都下载安装了。
```
































