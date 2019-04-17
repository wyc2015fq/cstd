# Java项目使用javadoc.exe生成JavaDoc文档 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年11月23日 15:32:28[boonya](https://me.csdn.net/boonya)阅读数：2027








# 1.从Eclipse导出JavaDoc

在Eclipse里项目右键 export 选 java>javadoc。


![](https://img-blog.csdn.net/20161123152715019)

下一步，选择Eclipse所使用的JDK版本下的javadoc.exe可执行文件。


![](https://img-blog.csdn.net/20161123152723128)

下一步，按照默认选项执行就行：


![](https://img-blog.csdn.net/20161123152730519)

下一步设置文档字符编码格式：


![](https://img-blog.csdn.net/20161123152739019)

注意JDK版本与程序编写的版本保持一致！


# 2.解决乱码

javadoc.exe默认执行是按照GBK方式执行的。在Eclipse里项目右键 export 选 java>javadoc，在向导的最后一页的Extra JavaDoc Options 里填上参数即可。比如项目采用的是UTF－8的编码就填：-encoding UTF-8 -charset UTF-8    
输入完成回车(Enter)执行。




![](https://img-blog.csdn.net/20161123152739019)




