# 「自己开发直播」rtmp-nginx-module实现直播状态、观看人数控制 - DoubleLi - 博客园







这是自己搭建直播服务器、开发直播平台系列的文章，前面两篇文章分别为：
- [通过Nginx-rtmp-module搭建直播服务器并实现直播](http://www.ptbird.cn/nginx-rtmp-module-server.html)
- [实现nginx-rtmp-module多频道输入输出与权限控制](http://www.ptbird.cn/nginx-rtmp-multi-channel.html)

这篇文章是在上面多频道输入输出和权限控制的基础上进行的。

## 一、目标

### 1、实现直播状态的更改：

（直播状态的更改主要是为了显示，在前端刷新列表的时候，用于判断那些直播间在直播，哪些不在。）：
- 用户开始推流之后，操作数据库，更改直播状态，显示直播间已经开始直播。
- 用户推流结束或者关闭软件后，操作数据库，更改直播状态。

### 2、实现简单的人数统计（暂用）：

这里实现人数统计是基于数据库的（rtmp-nginx-module本身是有一个模块的，可以实现当前视频流观看人数的统计），这里只是简单的实现自增和自减数据库。
- 用户观看打开页面开始观看直播，数据库字段自增1
- 用户关闭页面后，连接断掉，数据库字段自减1

## 二、rtmp服务器的配置

在 [实现nginx-rtmp-module多频道输入输出与权限控制](http://www.ptbird.cn/nginx-rtmp-multi-channel.html) 这篇文章里，我们配置了 on_publish 这个参数。

主要实现多频道和权限控制，用户在正式推流到服务器之前首先去配置的url中验证一下权限（具体的可以看上篇文章）。

目前我们需要配置更多的参数：

```bash
application myapp{ 
         live on; # live on表示开启直播模式 
         on_publish http://192.168.124.125/on_publish.html;
         on_publish_done http://192.168.124.125/on_publish_done.html;
         on_play http://192.168.124.125/on_play.html;
         on_play_done http://192.168.124.125/on_play_done.html;
    }
```

可以看到，除了 on_publish ，我们还配置了 on_publish_done、on_play、on_play_done 三个参数，后面也是一个url。

这三个参数也是非常容易理解的，分别对应如下：
- on_publish_done —— 推流结束
- on_play —— 客户端播放
- on_play_done —— 客户端播放结束

对上面三个参数的具体解释和用法可以看rtmp的文档：
- [https://github.com/arut/nginx-rtmp-module/wiki/Directives#on_publish](https://github.com/arut/nginx-rtmp-module/wiki/Directives#on_publish)

![r.jpg](http://static.ptbird.cn/usr/uploads/2017/03/1856073095.jpg)

经过上面的配置，用户推送或者推送结束、或者用户播放和播放结束都能够触发URL，同时操作数据库即可。

## 三、程序部分注意问题

配置好了URL，就需要进行操作，但是会发现一个问题：
- 不知道用户观看的是哪个流、或者不知道推送结束的是哪个流

上面问题可以看 [实现nginx-rtmp-module多频道输入输出与权限控制](http://www.ptbird.cn/nginx-rtmp-multi-channel.html) 这篇文章里面接收的 name参数 ,上面的配置中所有的推送都会携带一个name参数，而这个name是不可变的，用于标识当前流名称。

用我写的简单举个例子：
- 下面是基于thinkphp5框架写的。
- url与函数不匹配是因为我做了route。

可以看到，每次的操作都会获得一个name的参数，当然，除了name之外每个不同的配置有不同的参数可以获取，具体可以去 [文档](https://github.com/arut/nginx-rtmp-module/wiki/Directives#on_publish) 了解一下

```php
/**
     * 用户直播结束的回调事件
     *   - 用户直播结束后，将status设置成0，标注该房间未在推流
     * @param Request $request
     */
    function onPublishDone(Request $request){
        // 获得stream name
        $name=trim($request->param('name'));
        // 关闭直播 设置status 为 0
        Db::name('room')->where(['guid'=>$name])->setField('status',0);
        return json('')->code(200)->header(['关闭直播']);
    }
    /**
     * 用户观看直播的回调事件
     */
    function onPlay(Request $request){
        // 获得视频流地址
        $name=trim($request->param('name'));
        // 有人加入观看，设置people +1
        Db::name('room')->where(['guid'=>$name])->setInc('people');
    }
    /**
     * 用户结束观看直播的回调事件
     * @param Request $request
     */
    function onPlayDone(Request $request){
        // 获得视频流地址
        $name=trim($request->param('name'));
        // 有人加入观看，设置people -1
        Db::name('room')->where(['guid'=>$name])->setDec('people');
    }
```
- 
- [文章已经结束啦](http://www.ptbird.cn/rtmp-nginx-module-status-and-people.html#)







from：[http://www.ptbird.cn/rtmp-nginx-module-status-and-people.html](http://www.ptbird.cn/rtmp-nginx-module-status-and-people.html)










