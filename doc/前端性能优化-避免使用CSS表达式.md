# 前端性能优化-避免使用CSS表达式

 

# 简介

CSS表达式，如下面这个例子：

background-color:expression((new Date().getHours()%2?"#B8D4FF":"#F08A00"));

这个表示是为了实现背景颜色每2个小时变化一次；这种会导致性能下降。不过应该多数开发人员比较少使用CSS表达式。这里就一笔带过就好了。