# Python进阶_wxpy学习：异常chu'li - 去向前方的博客 - CSDN博客





2018年07月14日 15:04:49[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：1246








# 目录：


- [目录：](#目录)
- [前言](#前言)
- [异常处理](#异常处理)- [异常的抛出和捕捉](#异常的抛出和捕捉)
- [已知错误码](#已知错误码)
- [注意](#注意)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的是和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)

聊天对象可以参考前文：[聊天对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80973533)

消息处理可以参考前文：[消息处理](https://blog.csdn.net/lvsehaiyang1993/article/details/80980411)

微信监控可以参考前文：[微信监控](https://blog.csdn.net/lvsehaiyang1993/article/details/81002467)

探索调试可以参考前文：[探索调试](https://blog.csdn.net/lvsehaiyang1993/article/details/81009728)

实用组件可以参考前文：[实用组件](https://blog.csdn.net/lvsehaiyang1993/article/details/81026695)
# 异常处理

## 异常的抛出和捕捉

每当使用 wxpy 向微信发出请求 (例如发送消息、加好友、建群等操作)，wxpy 都会在收到服务端响应后进行检查。

若响应中的错误码不为 0，程序将抛出 ResponseError 异常。

```
class wxpy.ResponseError(err_code, err_msg)[源代码]

    当 BaseResponse 的返回值不为 0 时抛出的异常

    err_code

        错误码 (int)

    err_msg

        错误消息 (文本)，但可能为空
```

捕捉异常:

```python
try:
    # 尝试向某个群员发送消息
    group.members[3].send('Hello')
except ResponseError as e:
    # 若群员还不是好友，将抛出 ResponseError 错误
    print(e.err_code, e.err_msg) # 查看错误号和错误消息
```

## 已知错误码

通常来说，每个错误码表示一种类型的错误。

但因微信未公开 (也没有义务公开) 这套错误码体系的具体说明，我们只能根据经验猜测部分错误码的定义。

以下为一些常见的已知错误码。欢迎提交 PR 进行完善。 

“` 

1205
通常因为操作频率过高。需要控制频率，避免再次引起该错误。

## 注意

Web 微信对 加好友、建群 这两种操作的频率限制尤其严格！

对于微信而言，为了机器人避免打扰其他用户，以及控制服务器的负载压力，需要对各种不同的操作进行频率限制。

通常每种操作可有多层频率限制，而每层频率限制分为两个参数:

```
周期、次数，分布表示: 在 x 周期内，只能发送 y 个请求。
```

举个例子：

```
对于 发送消息 操作，可能会是这样 (数值为虚构):

    层   限制周期    限制次数
    1   2 分钟    120
    2   10 分钟   300
    3   1 小时    1000
    4   24 小时   2000
    可能会有用户在 1 分钟内狂发 100 条消息。
    但这样的频率不可能维持一整天，所以一天内 3000 条是足够的。

通过以上方式，微信可实现较为合理的限制。
```

1204

通常因为操作对象不为好友关系。例如尝试向一位不为好友的群员发送消息时，会引起这个错误。 

1100, 1101, 1102

通常表示机器人已经掉线，需要重新登录。

请重新初始化 Bot 对象，并重新注册消息。

```
因为重新登录后，聊天对象的 user_name 可能已经变化，所以原先的消息注册也会因此失效。
```





