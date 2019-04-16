# matplotlib作图布局 - littlely_ll的博客 - CSDN博客





2017年07月08日 21:19:24[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：301标签：[matplotlib																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=matplotlib&t=blog)
个人分类：[matplotlib																[python](https://blog.csdn.net/littlely_ll/article/category/6664856)](https://blog.csdn.net/littlely_ll/article/category/7015167)







```bash
plt.figure(0)
axes1 = plt.subplot2grid((3,3), (0,0), colspan=3)#(0,0)开始，占3列
axes2 = plt.subplot2grid((3,3), (1,0), colspan=2)#(1,0)开始，占2列
axes3 = plt.subplot2grid((3,3), (1,2))
axes4 = plt.subplot2grid((3,3), (2,0))
axes5 = plt.subplot2grid((3,3), (2,1),colspan=2)

all_axes = plt.gcf().axes
for ax in all_axes:
    for ticklabel in ax.get_xticklabels() + ax.get_yticklabels():
        ticklabel.set_fontsize(10)

plt.suptitle('Demo of subplot2grid')
plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20170708211527949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

`subplot2grid`参数有shape, loc以及可选的参数`colspan`，`rowspan`，它和`figure.add_subplot`一个重要的不同是loc位置是从0下标开始的





