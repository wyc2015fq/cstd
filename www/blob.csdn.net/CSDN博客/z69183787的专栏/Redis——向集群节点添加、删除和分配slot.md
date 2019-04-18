# Redis——向集群节点添加、删除和分配slot - z69183787的专栏 - CSDN博客
2018年08月15日 16:05:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：495
[https://blog.csdn.net/a491857321/article/details/52058693](https://blog.csdn.net/a491857321/article/details/52058693)
## 1、添加节点
`添加节点：redis-trib.rb add-node 192.168.72.100:7006 192.168.72.100:7005 `
前面的IP加端口号是要添加的redis节点，后面的IP和端口号是集群中的任意一个节点。
![](https://img-blog.csdn.net/20160728202742943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2、为节点分配slot
`为节点分配slot：redis-trib.rb reshard 192.168.72.100:7006`
![](https://img-blog.csdn.net/20160728203110339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
添加之后，分配之前节点slot的情况![](https://img-blog.csdn.net/20160728203458720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)分配之后节点的slot情况
![](https://img-blog.csdn.net/20160728203557128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3、添加从节点
`添加从节点：redis-trib.rb add-node 192.168.72.100:7007 192.168.72.100:7005`
和上面情况类似，添加之后用redis-cli 登陆新添加的节点，然后执行设置主节点
`设置主节点：cluster replicate 578d27842e8da87f89f14c73faf8f5bbe2f9ed85（对应master的nodeID）`
![](https://img-blog.csdn.net/20160728204118072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 4、删除集群节点
**先删除主节点的情况**
删除集群主节点
删除集群主节点之前要先将其上面的slot分配到其他主节点上
- 
`重新分配slot：redis-trib.rb reshard 192.168.72.100:7006`
- 
`删除主节点：redis-trib.rb del-node 192.168.72.100:7006 578d27842e8da87f89f14c73faf8f5bbe2f9ed85`
![](https://img-blog.csdn.net/20160728205300964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到删除之后原来主节点的从节点自动变为其他主节点的从节点了（可以试验一下，观察一下该从节点与移动的slot有什么关系）
删除从节点
`删除从节点：redis-trib.rb del-node 192.168.72.100:7007 4e3c459e26040f49b51dce8fdae5cb571b066ff0`
![](https://img-blog.csdn.net/20160728205935811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**先移除从节点的情况**
先删除从节点
`删除从节点：redis-trib.rbdel-node 192.168.72.100:7007 4e3c459e26040f49b51dce8fdae5cb571b066ff0`
![](https://img-blog.csdn.net/20160728210203321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
再删除主节点
`重新分配slot：redis-trib.rb reshard 192.168.72.100:7006`
![](https://img-blog.csdn.net/20160728210504106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后再删除主节点
`删除主节点：redis-trib.rb del-node 192.168.72.100:7006 578d27842e8da87f89f14c73faf8f5bbe2f9ed85`
![](https://img-blog.csdn.net/20160728210722703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
一般建议的如果要移除主节点，先将从节点移除，避免出错。
