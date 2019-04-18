# 寻找最小的k个数 - bigfacesafdasgfewgf - CSDN博客





2014年12月03日 11:09:35[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：768标签：[最小k个数																[快速选择																[堆																[时间复杂度](https://so.csdn.net/so/search/s.do?q=时间复杂度&t=blog)
个人分类：[Algorithm Probelm](https://blog.csdn.net/puqutogether/article/category/2812137)





**寻找最小的k个数**




# 1. 题目描述




    在一个长度为n的数组中，寻找最小的k个数。（最大的k个数解法类似）




# **2. 排序法**



    想法比较简单，先对n个数排序，再输入前面k个数，即可。

    这种方法的时间复杂度比较大。假设我们使用快排，需要O(nlogn), 然后输出k个数需要O(k)，一共要O(nlogn)~




   代码略。




# 3. 块移动法

    这种方法比较好，算法简单，易于实现。

    先把数组中的前k个数作为候选最小块；再让这个块往后移，每移动一次就比较当前数和块中的最大数，如果比最大数小，替换掉，重新选择一个块中的最大数。




    如下图所示：




![](https://img-blog.csdn.net/20141203112250132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    这种方法的时间复杂度为O(k)+O(n-k)k=O(nk).




   还有一种改进的方法，就是把k个数放在一个最大堆heap中，然后每次移动更新的时候，就是一个更新堆的过程，如果当前数比堆顶元素（k个数中的最大数）小，那么我们就更新这个堆。而更新堆只需要O(logk)。所以这种改进方法的时间复杂度为O(nlogk)。




# 4. 快速选择方法




    这种方法的比较难懂，不过时间复杂度为O(n).




    参考链接：

    https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/02.01.md](https://so.csdn.net/so/search/s.do?q=堆&t=blog)](https://so.csdn.net/so/search/s.do?q=快速选择&t=blog)](https://so.csdn.net/so/search/s.do?q=最小k个数&t=blog)




