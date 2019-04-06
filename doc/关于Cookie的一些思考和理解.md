# 关于Cookie的一些思考和理解

​             ![96](https://upload.jianshu.io/users/upload_avatars/5342565/1377a47f-6b10-4e6a-9ed7-9df0709dbafe.JPG?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96) 

​             [eason02](https://www.jianshu.com/u/9505096c45f4)                          

​                                                    0.3                                                 2018.06.04 14:21*               字数 1540             阅读 1075评论 0喜欢 5

关键词：`cookie` `安全性` `GDPR`

> 可能你最近在访问一些在欧洲有业务来往的网站的时候，你会发现会有Cookie的提示，没错，最近欧盟GDPR生效，前阵子一直忙于Cookie Privacy的事情，顺便总结一下Cookie的各个知识点

Cookie，是站点为了访客的一些信息来区分用户(Identifier)或者传递信息，特别是在big data的现在，更有利于站点site对用户行为和地区倾向进行大数据分析，也可以为推荐系统做前期的基础数据训练集。

#### Cookie存在哪里？

一般存在浏览器目录中的文本文件中

#### Cookie分为多少种？

1. 带失效时间`expires`的，在下次访问之前，如果失效时间到期，会自动清除对应`cookie`，`expires`和`maxage`的概念差不多，通常两个时间都是一样的，一个是时间，一个时间长度，前者是HTTP/1.0 protocol，后者是HTTP/1.1 protocol，为了向下兼容而已，所以，最好是两个参数都设置
2. 跟随`session`结束就自动清除的，这种`cookie`很常见，通常你会在`console`看到它的expires时间是1969年的或者所边当前时间还要早的时间，这种cookies会在会话结束的同时清除掉

#### Cookie的工作原理是什么？

前面我们说了Cookie一般存在浏览器目录中的文本文件中，并且会根据domain分开存放，比如，当你输入jianshu.com的时候，浏览器会向jianshu发送一个request，然后server根据request来返回response，把结果在显示器中显示，当你发送这个request的时候，浏览器会寻找当前浏览器目录中是否存在jianshu.com的相关cookie，如果有，浏览器会把Cookie文件中的数据放在request   header中一起向server发送，服务器收到Cookie数据，服务器会根据你的cookie信息做一些相应的处理，比如第一次访问的话，会为你创建一个新的session  id，否者来检测你是否需重新登录等等操作。

#### Cookie包含哪些字段？

主要包括server、expires、name、value、secure、httpOnly、path，其中服务器只想知道name和value字段，其余的字段有的为了存储，有的为了安全性

#### 如何查看Cookie？

比如在chrome下打开[jianshu.com](https://jianshu.com)，F12 -> Application -> Cookies -> <https://www.jianshu.com>
 



![img](https://upload-images.jianshu.io/upload_images/5342565-24141bac572d3c5f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

jianshu cookie

 也可以在Console里边输入document.cookies，但是只显示非HTTP的cookie name和value，没有其他信息



#### 如何操作cookie？

分为两种：第一种在前端对cookie进行操作，以chrome为例子，可以直接在console里边对cookie进行修改，例如我们将`default_font`改成`Simplified`(当然这只是举个例子，至于修改完重新发送请求到jianshu server之后会不会有作用，这个取决于jianshu server)，F12 -> Console，输入：

```
document.cookie = "default_font=Simplified;path=/"
```

注意：name、domain、path一定要对应正确，这里的cookie domain为[www.jianshu.com](https://www.jianshu.com)，所以不需要显式设置，结果如下：
 



![img](https://upload-images.jianshu.io/upload_images/5342565-3a2981dd513251f0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000)

修改后

 同样的也可以删除cookie，只需要将expires设置成比现在早的时间就可以了，顺便分享一个前端修改指定cookie过期时间为一年的脚本，下面是修改_ga的过期时间，可自行根据需求修改：



```
for(i in cookie_list){
    target = cookie_list[i];
    if(target.includes('_ga')){
        var date = new Date();
        var c_time = date.getTime();
        var age = 60*1000*60*24*365; // one year expiration
        date.setTime(c_time + age); 
        document.cookie = target+";expires="+date.toGMTString()+";path=/";
    }
}
```

第二种是后端服务器操作，可以直接在response上面进行操作，你可以直接抹掉某个cookie

```
response.clearCookie('default_font')
```

这个时候回来的response header就不会带default_font这个cookie了
 也可以修改已存在的cookie过期时间或者加入新的cookie，nodejs的application可以使用cookie.js

```
res.setHeader('Set-Cookie', cookie.serialize('my_cookie', 'my_value', {
httpOnly: false,
expires: "Mon, 11 Jun 2018 05:47:25 GMT"
maxAge: 60 * 60 * 24 * 7 // 1 week
}));
```

这个时候浏览器就会接受到一个包含cookie为my_cookie=my_value，并且过期时间为一个星期的response。

#### 为什么我设置了cookie没有生效？

首先可能有两个问题
 注意一下domain和path，必须完全对应可以设置，比如你要修改某一个cookie，修改的时候必须指定与其一样的domain和path才可以生效，否者不生效，另外，当前域不可以写其他域的cookie，子域名除外。

#### Cookie的安全性

既然cookie这么容易修改，毕竟cookie输入个人信息，容易获取，就容易被利用，那么我们自然而然会想到安全性问题，这里我能想到的有四种方式，在`后端服务器`对cookie进行适当的处理，可以提高cookie的安全性：

1. 在设置cookie的时候，加上以下参数，指明该cookie只用来HTTP请求使用，这个时候前端或者浏览器是拿不到这个cookie的，也就是使用document.cookie是不会出现的，默认为false：

```
res.setHeader('Set-Cookie', cookie.serialize('my_cookie', 'my_value', {
httpOnly: true,
expires: "Mon, 11 Jun 2018 05:47:25 GMT"
maxAge: 60 * 60 * 24 * 7 // 1 week
}));
```

1. 后端设置以下参数，使得cookie只在https protocol下传输，如果把https改成http，cookie无法跟随请求一起发送到server端，在发送cookie之前就已启用了加密传输协议https：

```
res.setHeader('Set-Cookie', cookie.serialize('my_cookie', 'my_value', {
httpOnly: true,
secure: true,
expires: "Mon, 11 Jun 2018 05:47:25 GMT"
maxAge: 60 * 60 * 24 * 7 // 1 week
}));
```

1. 设置expires的时间不要太长，如果必要，将其设置成跟随session的结束而结束，默认可以不指定expires时间，或者设置一个比当前时间要早的时间：

```
res.setHeader('Set-Cookie', cookie.serialize('my_cookie', 'my_value', {
httpOnly: true,
secure: true,
expires: "Thu, 1 Jan 1970 00:00:00 GMT"
}));
```

1. 对敏感的cookie信息进行加密处理，防止被有意利用，例如MD5加密

> 在此，给各位需要做GDPR的同学们一个建议，对cookie的控制，最好在server器端处理，避免在前端处理，因为他实际上是一个在线修改而已，那些你希望存在的cookie，实际上已经写入了访客浏览器目录中，只是在后续js处理中被你修改了而已，这是一种很搞笑的行为。

** 文章所有步骤都是经过实践检验并可行，若有问题，下方请评论。

# 

——END——
 作者 :  `Eason`，专注各种技术、平台、集成，不满现状，喜欢改改改
文章、技术合作
 Email : [eason.lau02@hotmail.com](mailto:eason.lau02@hotmail.com)