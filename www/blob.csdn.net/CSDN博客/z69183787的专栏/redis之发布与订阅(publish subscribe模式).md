# redis之发布与订阅(publish/subscribe模式) - z69183787的专栏 - CSDN博客
2018年02月22日 11:55:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1189
[http://blog.csdn.net/u011499747/article/details/51232981](http://blog.csdn.net/u011499747/article/details/51232981)
发布与订阅（publish/subscribe）模式，简称为pub/sub模式，redis也实现了这种模式。
# redis提供的发布订阅命令
|命令|用例和描述|
|----|----|
|subscribe|subscribe channel [channel …] 订阅一个或多个频道|
|unsubscribe|unsubscribe [channel [channel …]] 退订频道，如果没有指定频道，则退订所有的频道|
|publish|publish channel message 给指定的频道发消息|
|psubscribe|psubscribe pattern [pattern …] 订阅给定模式相匹配的所有频道|
|punsubscribe|punsubscribe [pattern [pattern …]] 退订给定的模式，如果没有指定模式，则退订所有模式|
## 实例
一个客户端订阅test-channel 
![一个客户端订阅test-channel](https://img-blog.csdn.net/20160424122248797)
一个客户端向test-channel发消息 
![一个客户端向test-channel发消息](https://img-blog.csdn.net/20160424122300937)
# 缺点
所有pub/sub很有用，但是redis的实现有这么几个问题： 
1，如果一个客户端订阅了频道，但自己读取消息的速度却不够快的话，那么不断积压的消息会使redis输出缓冲区的体积变得越来越大，这可能使得redis本身的速度变慢，甚至直接崩溃。 
2，这和数据传输可靠性有关，如果在订阅方断线，那么他将会丢失所有在短线期间发布者发布的消息，这个让绝不多数人都很失望吧。
