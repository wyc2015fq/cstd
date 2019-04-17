# 【面经笔记】struct与class的区别 - xiaxzhou的博客 - CSDN博客





2017年07月15日 21:26:53[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：182








> 
[http://blog.csdn.net/yuliu0552/article/details/6717915](http://blog.csdn.net/yuliu0552/article/details/6717915)


C++中的struct对C中的struct进行了扩充，它已经不再只是一个包含不同数据类型的数据结构了，它已经获取了太多的功能。

struct能包含成员函数吗？ 能！

struct能继承吗？ 能！！

struct能实现多态吗？ 能！！！

有很多人应该已经知道这样一个事实，但总有些不知道的人，看到这些会感到很惊讶。是的，当我第一次注意到这个事实的时候，我也同样很吃惊。

既然这些它都能实现，那它和class还能有什么区别？

最本质的一个区别就是默认的访问控制，体现在两个方面：

> 
1）默认的继承访问权限。struct是public的，class是private的。

2）struct作为数据结构的实现体，它默认的数据访问控制是public的，而class作为对象的实现体，它默认的成员变量访问控制是private的。




