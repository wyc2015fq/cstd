# 导Excel-未将对象引用设置到对象的实例 - Jun5203 - CSDN博客
2019年03月10日 21:29:02[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：34
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
在导出Excel表格的过程中，出现了以下问题，但是秉着发现问题解决问题的思想，我站在巨人的肩膀上成功的game over它。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310212201600.jpg)
断点调试后，报错的位置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310212350317.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
庆幸的是有同僚出出现过这种错误，也使我的问题得到了解决。
解决方法如下：
```
excel.Cells[i + 2, j + 1] = dgv[j, i].Value;
```
