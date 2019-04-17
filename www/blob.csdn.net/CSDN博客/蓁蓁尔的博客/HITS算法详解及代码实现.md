# HITS算法详解及代码实现 - 蓁蓁尔的博客 - CSDN博客





2016年03月29日 16:43:42[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：3445








详解见：[http://blog.csdn.net/hguisu/article/details/8013489](http://blog.csdn.net/hguisu/article/details/8013489)

**Hub页面（枢纽页面）和Authority页面（权威页面）**是HITS算法最基本的两个定义。所谓“Authority”页面，是指与某个领域或者某个话题相关的高质量网页，比如搜索引擎领域，Google和百度首页即该领域的高质量网页，比如视频领域，优酷和土豆首页即该领域的高质量网页。所谓“Hub”页面，指的是包含了很多指向高质量“Authority”页面链接的网页，比如hao123首页可以认为是一个典型的高质量“Hub”网页。

HITS算法步骤：

（1）将查询q提交给基于关键字查询的检索系统，从返回结果中取前n个网页(如n=200)作为**根集**，记为root。

（2）在根集root的基础上，HITS算法对网页集合进行**扩充集**合base，扩充原则是：凡是与根集内网页有直接链接指向关系的网页都被扩充到集合base，无论是指向还是被指向都被扩充进入扩展网页集合base。

（3）计算扩展集base中所有页面的Hub值（枢纽度）和Authority值（权威度）。初始化都为1.

（4）每次迭代计算Hub权值和Authority权值，直到稳定：

      网页 a (i)在此轮迭代中的Authority权值即为所有指向网页 i 页面 j 的Hub权值之和：a (i) = Σ h (j) ;

      网页 a (i)的Hub分值即为所指向 i 的页面 j 的Authority权值之和：h (i) = Σ a (j) 。

      对a (i)、h (i)进行规范化处理:

           将所有网页的中心度都除以最高权威度以将其标准化：a (i) = a (i)/|max_a(i)| ；

           将所有网页的权威度都除以最高中心度以将其标准化：h (i) = h (i)/ |max_h(i)| ：

代码：[http://www.lxway.net/15224186.html](http://www.lxway.net/15224186.html) Java代码，亲测可用。



