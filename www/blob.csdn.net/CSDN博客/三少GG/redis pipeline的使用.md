# redis pipeline的使用 - 三少GG - CSDN博客
2014年05月27日 20:36:53[三少GG](https://me.csdn.net/scut1135)阅读数：3556
## redis pipeline的使用
(2013-05-28 20:02:43)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载▼
|标签：### [redis](http://search.sina.com.cn/?c=blog&q=redis&by=tag)### [pipeline](http://search.sina.com.cn/?c=blog&q=pipeline&by=tag)### [appendcommand](http://search.sina.com.cn/?c=blog&q=appendcommand&by=tag)### [c语言](http://search.sina.com.cn/?c=blog&q=c%D3%EF%D1%D4&by=tag)### [hiredis](http://search.sina.com.cn/?c=blog&q=hiredis&by=tag)### [it](http://search.sina.com.cn/?c=blog&q=it&by=tag)|分类：[Technology](http://blog.sina.com.cn/s/articlelist_2563713647_2_1.html)|
**1、pipeline原理**
redis是一个cs模式的tcp server，使用和HTTP类似的请求响应协议。通过ip+port的形式构建连接，发起请求命令。每个请求发出后client通常会阻塞并等待redis服务处理完毕返回响应。
原理清楚了，抱着提高效率的目的，我们自然会想一来一回、一来一回。类似
Req
Rsp
Req
Rsp
Req
Rsp
的形式，我们是否有改进提升的空间呢？明显是有的，特别是在连续多条命令不影响后续命令执行的情况下，我们可以采取多量少次的做法来减少网络交互与等待时间。也就是说可以改成类似下边的形式：
container
add Req
add Req
add Req
send
accept Rsp
这样一来，其实就可以看出至少节省了redis回传次数。如果add是在本地实现的话，那就直接从6次降到了2次了。（ps:这个我不是十分确定，有可能是在本地积累，有可能是在redis服务端积累）。O(∩_∩)O哈哈~，这就是redis pipeline（管道）的功用。批量执行命令，然后结果一次性返回。

**2、实践**
那到底使用之后性能提升如何呢？本次实践如下：
处理数据量（100000）（ps:和redis交互只是程序极小一部分，在此仅供对比使用）
HINCRBY逐条执行 程序耗时850+秒
利用pipeline 10000条执行一次 程序耗时600+秒
省时4分钟左右，占程序总时间大概30%左右
效果还是比较明显的
主体代码如下：
使用的客户端来自于Redis官方网站，是Redis推荐的基于C接口的客户端组件，见如下链接：https://github.com/antirez/hiredis
for(int i=0; i
{   
      if (REDIS_OK != redisAppendCommand(temp_xx, command))
      {   
             WriteLog("[WARNING]:Can't add command to reids!\n");
       }
}           
if (REDIS_OK != redisGetReply(temp_xx, (void**)&temp_yy))
       WriteLog("[WARNING]:pipeline process to redis failed\n");
其中temp_xx是redisContext，temp_yy是redisReply
**3、误区**
本次遇到问题具体原因我也没完全弄清楚，不过还是把它记录下来，当备忘吧。不知道是程序多线程原因还是本身pipeline与前后共用同一个redisContext会存在问题，我这次直接利用之前已存在的线程内全局redisContext时，会引发别处HGET返回结果内容出错。
用temp_xx和temp_yy（都是局部新建redisContext和redisReply）之后问题得到解决。我估计后者引发可能性更大些。
**4、总结**
（1）pipeline主要提升在与redis交互的网络延迟时间，对于多条命令不要求实时执行的情形而言，效果较为明显
（2）**使用时最好注意一下pipeline执行的redisContext和redisReply（如果遇到诡异问题，可以尝试一下使用局部新建、独占两者进行处理的方式）**
