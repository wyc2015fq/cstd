# 微信token server服务 - 三少GG - CSDN博客
2016年10月15日 16:25:41[三少GG](https://me.csdn.net/scut1135)阅读数：1739
解决方案：
## **微信鉴权等作为单独的server，对外提供服务. 用grpc通信**
golang库： https://github.com/chanxuehong/wechat.v2/blob/master/mp/core/access_token_server.go   重写AccessTokenServer
方案：https://cnodejs.org/topic/57b330b6670139eb7bf6fd4c
1.redis有2个功能：第一是用作token的缓存，第二是作为锁媒介2.利用事件队列缓存请求过程：1.redis作为缓存，缓存住token2.每次请求判断token是否过期2.1 如果token未过期，则直接使用2.2 如果token过期，则利用redis的setnx（只能设置不存在的key）命令获取锁（redis单线程，一次处理1个命令，如果一下子来了N多请求，也只能一条一条处理），设置成功的请求去做获取token的操作，其他的请求~~缓存到事件队列~~（设一个timeout后去redis取,
 如果还是没取到, 重复）
############################################
1.以下办法有点复杂：
https://github.com/diamont1001/wechat-jssdk-server/blob/master/docs/%E8%AE%BE%E8%AE%A1%E6%96%87%E6%A1%A3/token%E5%92%8Cticket%E6%9B%B4%E6%96%B0%E9%80%BB%E8%BE%91.md
2.  redis锁
http://xiaorui.cc/2016/03/23/golang%E4%BD%BF%E7%94%A8redigo%E5%AE%9E%E7%8E%B0redis%E7%9A%84%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81/
