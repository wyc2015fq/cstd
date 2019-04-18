# Netty 之 netty的比较规范的C/S端的写法 - z69183787的专栏 - CSDN博客
2016年09月26日 11:01:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1172
个人分类：[RPC-Netty](https://blog.csdn.net/z69183787/article/category/6412734)

看了RPC框架Jupiter的源码和RocketMQ的Netty部分的代码，最后还是总结一下，把与Netty相关的代码都截取出来，写出一个比较规范的DEMO
这个DEMO还是比较简单的，但麻雀虽小五脏俱全啊，有心跳，有重连，有ack的，可以与大家分享一下~
https://github.com/BazingaLyn/netty-study/tree/master/src/main/java/com/lyncc/netty/production
一起看看吧，希望对大家有帮助，一起学习
