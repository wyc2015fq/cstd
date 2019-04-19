# SAE提供服务分析 - =朝晖= - 博客园
# [SAE提供服务分析](https://www.cnblogs.com/dhcn/p/7105476.html)
这个分析列表主要关注两个问题，服务能做什么，移植实现难度。
AppConfig： 这个东西主要面向SAE本身的一些配置选项，移植时放弃这个东西，所以就不谈难度了
Counter ：这个东西提供某个操作的计数服务，这个东西根据并发强度和分布式需要，实现难度各有不同
Cron ：这个东东就是定时任务。他这个东西就是[Linux](http://lib.csdn.net/base/linux)的Cron的包装版，所以无需多做实现
CDN ：这个东西就不多说了，等你的网站用到这个东西的时候，你估计已经对它耳熟能详了
DeferredJob ：离线延迟任务功能，这个用异步消息队列+独立服务基本都可以实现
FetchURL：这个功能说白了就是HTTP抓取，不用实现，很多东西都可以做这个事情，比如注明的httpclient。
Image ：这个东西据说现在以RPC API提供了,由于图像操作比较耗CPU，所以将这种操作独立化，防止影响正常HTTP服务。
KVDB ：NoSQL[数据库](http://lib.csdn.net/base/mysql)，这个功能有很多工具可以做。
Mail ：这个东西发邮件的话，库很多啊
Memcache ：这个本身就是一个独立工具
[MySQL](http://lib.csdn.net/base/mysql) ：不用说了吧，SAE，主要提供了主从和HA两种模式。
Rank ：暂时写到这里，下次再写。
RDC 
Socket 
Storage 
TaskQueue 
TmpFS 
XHProf 
Wrappers 
----------------------------------------------------------------
中文分词服务 
全文检索服务 
地理信息服务 
微盘开放接口 
苹果消息推送服务 

