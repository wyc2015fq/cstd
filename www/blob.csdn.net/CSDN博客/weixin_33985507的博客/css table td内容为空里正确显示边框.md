# css table td内容为空里正确显示边框 - weixin_33985507的博客 - CSDN博客
2012年03月10日 10:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
通常情况下，td内无内容时没有边线（但也有时有，还未深究）
解决方法：
    给table加个border-collapse:collapse;
    给td加个empty-cells:show;
