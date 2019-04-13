
# win10 系统编码 乱码 - 追求卓越,做到专业 - CSDN博客


2019年01月03日 16:50:48[Waldenz](https://me.csdn.net/enter89)阅读数：152


重装系统后，别人给发送的文件或者压缩包，有时候中文会是乱码，原因是操作系统编码问题。cmd中使用chcp查看当前系统编码，显示是65001。
解决方法：
控制面板->区域->管理->更改系统区域设置->取消勾选 “Beta版，使用Unicode UTF-8提供全球语言支持”。
![](https://img-blog.csdnimg.cn/20190103164643671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
重启系统后，重新查询当前系统编码，为936。 问题解决。
![](https://img-blog.csdnimg.cn/20190103164857134.png)


