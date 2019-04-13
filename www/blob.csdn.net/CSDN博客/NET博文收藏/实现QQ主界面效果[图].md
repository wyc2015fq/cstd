
# 实现QQ主界面效果[图] - .NET博文收藏 - CSDN博客


2008年06月10日 12:48:00[hejishan](https://me.csdn.net/hejishan)阅读数：931标签：[qq																](https://so.csdn.net/so/search/s.do?q=qq&t=blog)


**实现QQ主界面的滚动效果****[图]**
1. GC实现QQ的主界面截图
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xuweiqun/gc.jpg)
2.实现方法
最主要的计算各个组和头像的位置信息,有了位置信息,画图就很方便了.(画图请参考本专栏其他文章)
2.1 一些概念:
项 : 组和头像,如果组合并,那么这个组只有组1项,如果展开那么 组+ 组成员的个数
m_scrollIndex : 滚动了几个项(滚动时计算)
m_scrollY : 实际滚动的像数(通过m_scrollIndex计算得到,滚动时计算)
(obj,group,index): 通过OnMouseMove得到的位置信息
obj表示在组或者头像,group表示哪个组，index表示group中的哪个头像
当obj表示组时,index就不起作用
2.2 鼠标位置信息的表达
//32位int  表示OnMouseMove位置信息
//2006-09-21
// |--------|--------|----------------|
//  8位类型   8位组号   16位头像索引
typedef   int     TREEHIT;
\#define   MAKETREEHIT(obj,group,index)  ((obj << 24)+(group << 16)+index)
\#define   TREEHITOBJ(hit)               (hit >> 24)
\#define   TREEHITGROUP(hit)             ((hit & 0x00ff0000)>>16)
\#define   TREEHITINDEX(hit)             (hit & 0x0000ffff)
2.3 组和头像位置信息的计算
由于图象有大小的区分，并且组的高度和大小头像又不一样，所以滚动的时候要计算滚动
了几个项.这样可以保证，每次滚动多是按项来滚动的，而不是固定的像数.
计算第n个组的第m个头像的RECT,首先从第一组开始算起(高度从0开始),算到第n个组第m头像时,然后减去滚动的实际像数m_scrollY ,就是实际的RECT,如果不和可见区域相交,那么就不画图.



