# useUnicode=true&characterEncoding=UTF-8  和 request.setCharacterEncoding - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月02日 15:54:12[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：202


# [转载链接](http://blog.csdn.net/qq_28484355/article/details/53305838)

request.setCharacterEncoding  是 设置  对 从客户端获取的 数据的 解码格式，如jsp页面是UTF-8, servlet的编码是GBK，此时就需要设置 一下 request.setCharacterEncoding=“UTF-8” ，来告诉 servlet用 UTF-8 来编码格式来解码jsp传过来的数据。



