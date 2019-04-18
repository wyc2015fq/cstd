# jedis使用方法以及场景 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月01日 18:02:36[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：58


1、下载jedis：     [点击打开链接](https://redis.io/clients#java)         下面是我用的版本

![](https://img-blog.csdn.net/20180401174906977)

打开：点击我选中的那个exe文件

![](https://img-blog.csdn.net/20180401175402677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

打开以后这样： 里面有个port 这个是端口号，一会要使用。

![](https://img-blog.csdn.net/20180401175439824?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、导包 ：  一定要和 自己下的 版本 一致

<dependency>
    <groupId>redis.clients</groupId>
    <artifactId>jedis</artifactId>
    <version>${jedis.version}</version>

</dependency>

fastjson： 将对象转换为 字符流，变成Json，所有语言通用

<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.41</version>
</dependency>

3、使用：

![](https://img-blog.csdn.net/20180401180107263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用场景：jedis 就是将 不经常修改的数据放入内存当中，这样浏览器显示的时候就不用从硬盘里面拿了，提高加载速度。

