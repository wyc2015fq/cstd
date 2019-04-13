
# 快速排序Python实现 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[快速排序Python实现](https://www.cnblogs.com/eniac1946/p/8748902.html)
|Posted on|2018-04-08 23:33|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8748902)|[收藏](#)
|本文用python语言介绍三种不同的快排实现。
|1. 一行代码实现的简洁版本
|[python]|[view plain](https://blog.csdn.net/razor87/article/details/71155518#)|[copy](https://blog.csdn.net/razor87/article/details/71155518#)

|quick_sort =|lambda array: array|if len(array) <=|1|else quick_sort([item|for item|in array[|1:]|if item <= array[|0]]) + [array[|0]] + quick_sort([item|for item|in array[|1:]|if item > array[|0]])

|2. 网上常见的快排实现
|[python]|[view plain](https://blog.csdn.net/razor87/article/details/71155518#)|[copy](https://blog.csdn.net/razor87/article/details/71155518#)

|def quick_sort(array, left, right):
|if left >= right:
|return
|low = left
|high = right
|key = array[low]
|while left < right:
|while left < right|and array[right] > key:
|right -=|1
|array[left] = array[right]
|while left < right|and array[left] <= key:
|left +=|1
|array[right] = array[left]
|array[right] = key
|quick_sort(array, low, left -|1)
|quick_sort(array, left +|1, high)
|由于快排是原地排序，因此不需要返回array。
|array如果是个列表的话，可以通过len(array)求得长度，但是后边递归调用的时候必须使用分片，而分片执行的原列表的复制操作，这样就达不到原地排序的目的了，所以还是要传上边界和下边界的。
|3.《算法导论》中的快排程序
|[python]|[view plain](https://blog.csdn.net/razor87/article/details/71155518#)|[copy](https://blog.csdn.net/razor87/article/details/71155518#)

|def quick_sort(array, l, r):
|if l < r:
|q = partition(array, l, r)
|quick_sort(array, l, q -|1)
|quick_sort(array, q +|1, r)
|def partition(array, l, r):
|x = array[r]
|i = l -|1
|for j|in range(l, r):
|if array[j] <= x:
|i +=|1
|array[i], array[j] = array[j], array[i]
|array[i +|1], array[r] = array[r], array[i+|1]
|return i +|1







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
