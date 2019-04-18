# 科学网—谁发起了对GitHub的攻击？ - 蒋迅的博文




# 谁发起了对GitHub的攻击？                           

已有 5943 次阅读2015-4-6 06:28|个人分类:[科技](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&classid=136794&view=me)|系统分类:[海外观察](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=420554&catid=2)|关键词:GitHub|[github](misc.php?mod=tag&id=127150)




从3月26日开始，[GitHub遭持续80多小时的大规模DDoS攻击](http://tech.sina.com.cn/s/2015-03-31/074110022397.shtml)。[劫持JS数据包](http://www.solidot.org/story?sid=43553)，采用的方法是[Man-on-the-side攻击](http://www.solidot.org/story?sid=43530)。原以为这个事件就算过去了。但这些日子里，当我访问科学网博客的时候，仍然时常出现每隔2妙就跳出“alert("WARNING: malicious javascript detected on this domain")”窗口的情况。其实这个事件断断续续地[仍在继续](http://www.solidot.org/story?sid=43590)：百度联盟广告js仍被劫持，国外ip访问浏览器均提示危险。网上已经有大量文章在谈这件事。大家都好奇，谁是“[DDoS攻击GitHub的幕后组织](http://www.solidot.org/story?sid=43569)”：

为了防止数据在网络中无限循环，名为[存活时间（Time to live，TTL）](http://en.wikipedia.org/wiki/Time_to_live)或跳数限制（hop limit）的机制限定了数据包的寿命。当TTL=0，数据包将被丢弃。大多数系统发送IP包时都是从TTL=64开始，如果该数据包抵达你时TTL=46，那么你和发送者之间经过了18跳（64-18=46）。在对GitHub发动大规模DDoS攻击时，攻击者劫持了百度的JS文件。如图所示，百度服务器所发送数据包的TTL=64，第一次抵达用户浏览器时TTL=42（不同位置这一数字会有所不同），经过了22跳，用户发回的请求包的TTL值也是64，但接下来的响应包的TTL值非常怪异，显然有个中间人设备注入了伪造包。如何识别这个中间人设备的IP？你可以借助Traceroute工具。利用该工具发送TTL=1，2，3....的请求包，因为TTL的值很小，在到达目的地前跳数就会变为零被沿路的路由器抛弃，而此时路由器会使用其IP地址发回时间超时的报文，如果某两跳之间发现了伪造包，那么注入设备应该就藏在其中。一位研究人员用定制Traceroute工具测试发现，注入设备位于第11跳和第12跳之间，通过查询第12跳设备的IP地址，作者发现它[位于中国联通骨干网](http://blog.erratasec.com/2015/04/pin-pointing-chinas-attack-against.html)，因此得出了某政府与此有关的结论。 




转载本文请联系原作者获取授权，同时请注明本文来自蒋迅科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-420554-880168.html](http://blog.sciencenet.cn/blog-420554-880168.html)

上一篇：[【数学都知道】2015年4月2日](blog-420554-879145.html)
下一篇：[[转载]毛泽东时代的“告密者”自白](blog-420554-881308.html)


