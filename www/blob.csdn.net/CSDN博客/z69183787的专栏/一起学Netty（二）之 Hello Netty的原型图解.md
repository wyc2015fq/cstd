# 一起学Netty（二）之 Hello Netty的原型图解 - z69183787的专栏 - CSDN博客
2016年10月31日 10:22:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1601
上一个小节，写的一个入门的Hello Netty的案例，它的模型其实很简单，我们画个简单的图理解一下
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51306999#)[copy](http://blog.csdn.net/linuu/article/details/51306999#)
- package com.lyncc.netty.concept;  
- 
- /**
-  * 
-  * @author Bazingalyncc
-  * 描述：
-  * 时间  2016年4月29日
-  */
- publicclass HelloWorldConcept {  
- 
- /**
-      * 
-      * 
-      * 
-      * 
-      *                   ________________________                                 __________________________
-      *                  |                        |                               |                          |    
-      *                  |   <-----Inbound-----   |                               |   ---inbound------- >    |   ________
-      *                  |   _____        ______  |                               |    _______      ____     |  |        |
-      *      _______     |  |     |       |    |  |                               |    |     |     |    |    |  |        |  
-      *     |       |    |  |  ②  |       |  ③ |  |      ___________________      |    |  ⑤  |     |  ⑥ |    |  |        |
-      *     |       |    |  |_____|       |____|  |     |                   |     |    |_____|     |____|    |  |        |     
-      *     |client |----|-------______-----------|-----|      network      |-----|--------------------------|--| server |
-      *     |       |    |       |     |          |     |___________________|     |          ______          |  |        |
-      *     |       |    |       |  ①  |          |                               |          |     |         |  |        |         
-      *     |       |    |       |_____|          |                               |          |  ④  |         |  |________|
-      *     |       |    |                        |                               |          |_____|         |
-      *     |_______|    |   -----Outbound--->    |                               |    <-----outbound----    | 
-      *                  |___ChannelPipeline______|                               |______ChannelPipeline_____| 
-      *                                                                               
-      *  ①：StringEncoder继承于MessageToMessageEncoder，而MessageToMessageEncoder又继承于ChannelOutboundHandlerAdapter
-      *  ②：HelloWorldClientHandler.java
-      *  ③：StringDecoder继承于MessageToMessageDecoder，而MessageToMessageDecoder又继承于ChannelInboundHandlerAdapter
-      *  ④：StringEncoder 编码器
-      *  ⑤：StringDecoder 解码器
-      *  ⑥：HelloWorldServerHandler.java
-      *  
-      * 
-      * 
-      */
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
（如果不清晰，还请放大浏览器，应该可以帮助理解netty的基本模型）
