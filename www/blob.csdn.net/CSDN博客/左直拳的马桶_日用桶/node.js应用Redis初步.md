# node.js应用Redis初步 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月12日 12:31:37[左直拳](https://me.csdn.net/leftfist)阅读数：30504

node.js下使用Redis，首先：
1、有一台安装了Redis的服务器，当然，安装在本机也行
2、本机，也就是客户端，要装node.js
3、项目要安装nodejs_redis模块
注意第 3 点，不是在本机安装就行了，而是说，要在项目中安装（引用）。
方法是，DOS窗口，在项目目录下，输入
npm install redis
这样就将nodejs_redis下载一份，放到当前目录下了。看看，多了一个文件夹：node_modules\redis
编写以下代码，保存到当前目录下\hello.js
```java
var redis = require("redis"),//召唤redis
/*
	连接redis数据库，createClient(port,host,options);
	如果REDIS在本机，端口又是默认，直接写createClient()即可
	redis.createClient() = redis.createClient(6379, '127.0.0.1', {})
*/
client = redis.createClient(6379,'192.168.159.128',{});
//如果需要验证，还要进行验证
//client.auth(password, callback);
// if you'd like to select database 3, instead of 0 (default), call
// client.select(3, function() { /* ... */ });
//错误监听？
client.on("error", function (err) {
    console.log("Error " + err);
});
client.set("string key", "string val", redis.print);//set "string key" "string val"
/*
	redis.print，回调函数，将redis的返回值显示出来。上一句执行结果，将返回“OK”	
*/
client.hset("hash key", "hashtest 1", "some value", redis.print);
client.hset(["hash key", "hashtest 2", "some other value"], redis.print);
//遍历哈希表"hash key"
client.hkeys("hash key", function (err, replies) {
    console.log(replies.length + " replies:");
    replies.forEach(function (reply, i) {
        console.log("    " + i + ": " + reply);
    });
client.hget("hash key","hashtest 1",redis.print);    
/*两种都可以断掉与redis的连接，
end()很粗暴，不管3721，一下子退出来了，上面那句获取哈希表"hash key"的某个元素值的表达式将没有结果返回
而quit()则是先将语句处理完毕再干净地退出，斯文得很
*/
//client.end();
client.quit();
});
```
运行：
DOS窗口，当前项目目录下，输入
node hello.js
参考资料：
[https://github.com/mranney/node_redis](https://github.com/mranney/node_redis)
英文的，但没办法，这里最权威，最全面，其他地方查了查，感觉都是扯淡。
