
# PyCharm中import时无法识别自己写的模块 - 追求卓越,做到专业 - CSDN博客


2018年04月16日 14:39:18[Waldenz](https://me.csdn.net/enter89)阅读数：4414


1. 问题描述：
同目录下，当多个文件之间有相互依赖的关系的时候，import无法识别自己写的模块，PyCharm中提示No Module.
![](https://img-blog.csdn.net/20180416143001512?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2. 解决步骤：
(1). 打开File--> Setting—> 打开 Console下的Python Console，把选项（Add source roots to PYTHONPAT）点击勾选上
![](https://img-blog.csdn.net/20180416143453535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
(2). 右键点击自己的工作空间文件夹，找到Mark Directory as 选择Source Root，问题解决！
![](https://img-blog.csdn.net/20180416143803584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


