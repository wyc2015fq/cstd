# awk按照多个分隔符进行分割 - 数据之美的博客 - CSDN博客
2017年06月01日 16:20:12[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：925
我们知道awk可以进行类似于cut之类的操作，如一个文件data如下
zhc-123|zhang
hongchangfirst-99|zhang
hongchang-100|zhang
如果我们
awk -F ‘-’ ‘{print $1;}’ data
会打印出
zhc
hongchangfirst
hongchang
但是如果我想根据多个分隔符进行分割呢？一种办法是两次awk，但是我们可以一次告诉awk我们所有的分隔符，如-和|这两个，如
awk -F ‘[-|]’ ‘{print $3;}’ data
会打印出
zhang
zhang
zhang
就这么简单，还有一个问题，如果我们想用[]作为分隔符怎么办？有办法，这样就行：
awk -F ‘[][]’ ‘{print $3;}’ data
这里在多说一个技巧，如果你想把两个文件按照对应的每行放在同一行上，可以用paste，比如：
data1文件为
1
2
3
data2的文件为
zhang
zhc
hongchangfirst
那么你想要得到
1 zhang
2 zhc
3 hongchangfirst
那么你这样就也可以，
paste data1 data2
默认使用tab键作为份分隔符，还可以自定义分隔符，如用=号：
paste -d‘=’ data1 data2
原文:[](http://blog.csdn.net/hongchangfirst/article/details/10070989)[http://blog.csdn.net/hongchangfirst/article/details/25071937](http://blog.csdn.net/hongchangfirst/article/details/25071937)
作者:hongchangfirst
hongchangfirst的主页:[http://blog.csdn.net/hongchangfirst](http://blog.csdn.net/hongchangfirst)
