# kibana设置视图展示 - sxf_123456的博客 - CSDN博客
2018年08月22日 21:13:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：914
1、在Discover 查看索引是否存在
![](https://img-blog.csdn.net/20180822211312630?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
若不存在，点击Management，再点击Index Patterns进入，点击“+”按钮添加索引：
![](https://img-blog.csdn.net/20180822211312718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180822211312726?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、点击Visualize 进入视图，点击“+”，选择相应的视图类型
![](https://img-blog.csdn.net/20180822211312785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3、上边操作之后进入下边页面，选择相应索引
![](https://img-blog.csdn.net/20180822211312820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4、按照查询语句配置下边部分
![](https://img-blog.csdn.net/20180822211312789?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意，Y轴是数量还是唯一值，若为唯一值，选择Y-Axis下边的Aggression中的unin count
