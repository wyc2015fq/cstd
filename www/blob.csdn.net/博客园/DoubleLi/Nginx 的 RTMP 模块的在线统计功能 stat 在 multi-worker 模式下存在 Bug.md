# Nginx 的 RTMP 模块的在线统计功能 stat 在 multi-worker 模式下存在 Bug - DoubleLi - 博客园






 《[让你的 Nginx 的 RTMP 直播具有统计某频道在线观看用户数量的功能](http://blog.csdn.net/defonds/article/details/9065591)》一文介绍了 Nginx 的在线统计模块。
        我们的在线直播服务使用了 Nginx 的 Rtmp 模块(请参阅《[Nginx RTMP 模块 nginx-rtmp-module 指令详解](http://blog.csdn.net/defonds/article/details/9274479)》)。总体来讲，这个模块的功能稳定性和性能等方面都是很出色的。只要你的直播源头数据供给没问题，Nginx 的直播发布就 No Problem。
        对于闲置频道，就是没人观看的直播频道，为了节省系统资源，我们进行了定时清理。那么怎么判定一个频道是闲置的呢？一开始我们使用了《[让你的 Nginx 的 RTMP 直播具有统计某频道在线观看用户数量的功能](http://blog.csdn.net/defonds/article/details/9065591)》介绍的方法，Java 使用 Apache 的 http client 调用 Nginx 的 nclients，即 http://直播服务器IP/nclients?app=app应用名&name=频道名，所得结果为 0 的即为闲置频道。
        但在实际环境中，我们发现，这个在线统计功能并不靠谱。有时有人正在播放的频道，被统计为 0。这样造成统计错误，进而使不应该被清理的频道被清理掉，严重影响了直播效果的稳定性。
        一开始我们以为是自己没有配置好 Nginx，但重新检查并反复部署仍然如此。
        有次我们使用 http://直播服务器IP/stat 直接用页面跟踪直播频道的在线统计情况，发现统计结果正常的几率在 80% 以上。然而那错误的 20% 的统计结果足以让我们的清理程序犯下致命的错误了。我们不禁对 Nginx 的在线统计功能产生了怀疑，这个靠谱不靠谱？
        经过多方求证，其中包括 RTMP 模块的作者的权威确认，我们终于得到结论，这确实是 Nginx 的一个 Bug。 
        原来是 Nginx 的 stat 对 multi-worker 模式的并发支持不够好造成的。
        官方给出两种解决方法：让 Nginx 工作在 single-worker 模式，或者为每个 Nginx worker 配置单独的 stat file。
        第一种方法对于 Nginx 提供的性能肯定大有影响，尤其是对于多核服务器。第二种方法，官方却没有说明如何单独配置。
        结合实际情况，我们没有使用这两种方法，而是自己去管理维护每个直播频道的连接数量。当然这也不是最好的办法，还是希望 Nginx 能够尽快支持 multi-worker 模式下的 stat。
参考链接：[https://github.com/arut/nginx-rtmp-module/issues/159](https://github.com/arut/nginx-rtmp-module/issues/159)。









