# R语言入门--画图（一）--ggplot2 - 默槑 - 博客园







# [R语言入门--画图（一）--ggplot2](https://www.cnblogs.com/modaidai/p/7275677.html)





先写一些需要用到的知识点，比如包、函数

dplyr 很好用的包 经常与ggplot2连用

mutate：用于对数据框的列进行重新处理，或者用处理的结果添加新列









数据清洗：

1、na.omit()               #去除数组当中的空值

　　newdata<-na.omit(olddata)









ggplot：

1、在ggplot的里面添加直线：

　　geom_hline()添加水平线

　　geom_viline()添加垂直线        #geom_hline(yintercept=1.3,type=4)+geom_vline(xintercept=-1.1,type=4)      #type=4是虚线

　　增加斜线：

　　geom_abline(slope=1,intercept=0,color='red',size=2)         #增加一个y=1*x+0的直线

　　slope设置斜率，intercept设置截距，color设置线条颜色，size设置线条粗细



2、ggplot    - -  >labs

　　title：设置主标题

　　subtitle：设置子标题（在主标题下面）

　　caption：设置副标题（在右下角）

　　color：设置颜色图例的标题



　　设置图片的标题（title）, 子标题（subtitle）, 引用（caption）

　　代码示例：

```
ggplot(mtcars, aes(mpg, wt, colour = cyl)) + 
   geom_point() + 
   labs(
　　title = "This is title", 
　　subtitle = "This is subtitle", 
　　caption = "This is caption",
　　xlab = "New x lab", 
　　ylab = "New y lab",
　　colour = "New Cyl"
　　　　)
```



　　默认情况下，图例的标题对应的是数据框中的列名，labs可以对图例的标题重新设置

　　除了labs 函数外，还有其他的可以设置图片标题，x轴和y轴标签的函数

　　 xlab : 修改x轴标签

　　yalb : 修改y轴标签

　　ggtilte : 设置图片标题，子标题

　　代码示例：

```
ggplot(mtcars, aes(mpg, wt, colour = cyl)) + 
  geom_point() + 
  xlab(label = "New x lab") +
  ylab(label = "New y lab") + 
  ggtitle(label = "This is title", subtitle = "This is subtitle")
```














