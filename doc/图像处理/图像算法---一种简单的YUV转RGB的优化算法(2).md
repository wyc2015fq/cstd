# 图像算法---一种简单的YUV转RGB的优化算法(2)

2016年09月30日 18:41:58 [Trent1985](https://me.csdn.net/Trent1985) 阅读数：1632



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/52712850



在前面一篇博客http://blog.csdn.net/trent1985/article/details/52053397 中，我们介绍了一种简单的基于完全基于移位运算的YUV转RGB算法优化方法，

今天，我们介绍基于查表的优化方法：

首先，我们看下最常用的YUV与RGB相互转换的算法公式，如下所示：

注意，RGB取值范围均为0-255：

1，RGB转YUV



```cpp
Y = 0.299R + 0.587G + 0.114B
U = -0.147R - 0.289G + 0.436B
V = 0.615R - 0.515G - 0.100B
```

2，YUV转RGB



```cpp
R = Y + 1.14V
G = Y - 0.39U - 0.58V
B = Y + 2.03U
```

对于1和2，等式两边同时乘以1024：

RGB转YUV：



```cpp
1024Y = 306R + 601G + 117B
1024U = -151R -296G + 446B
1024V = 630R -527G -102B
```

YUV转RGB：



```cpp
1024R = 1024Y + 1167V
1024G = 1024Y - 399U - 594V
1024B = 1024Y + 2078U
```

对于上述公式，移位变形：

RGB转YUV：



```cpp
Y = (306R + 601G + 117B) >> 10
U = (-151R -296G + 446B) >> 10
V =(630R - 527G - 102B) >> 10
```

YUV转RGB：



```cpp
R = (1024Y + 1167V) >> 10
G = (1024Y - 399U - 594V) >> 10
B = (1024Y + 2078U) >> 10
```

做到这一步，我们不在展开移位运算，而是进行查表运算：

eg：我们计算Y = (306R + 601G + 117B) >> 10为例，来说明如何定义查表：

对于R的范围为0-255，那么306R就可以构建一个表MAP_R[256]

MAP_R的计算如下：



```cpp
for(int i = 0; i < 256; i++)
{
    MAP_R[i] = 306*i;
}
```

这个表我们可以计算好直接放在这里，MAP_R[256] = {0,306,612......};

同理，我们构建MAP_G[256],MAP_B[256]，全部提前计算好放着，这样，Y的计算变换如下：



```cpp
Y = (MAP_R[R] + MAP_G[G]+MAP_B[B]) >> 10
```

到此处，我们可以发现，Y的计算只有3次查表3次加法和一次移位运算，这个速度是很快的，同理我们可以把其他的转换公式全部写成这类查表计算，

这样，我们的查表算法，大家应该就明白了吧，哈哈，这一篇就到此为止了！

最后放两张一样的测试图：

转换效果图

![img](https://img-blog.csdn.net/20160930183952269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

原图

![img](https://img-blog.csdn.net/20160930184008136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)