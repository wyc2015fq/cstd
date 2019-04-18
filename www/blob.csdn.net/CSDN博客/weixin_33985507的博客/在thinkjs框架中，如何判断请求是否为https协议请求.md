# 在thinkjs框架中，如何判断请求是否为https协议请求 - weixin_33985507的博客 - CSDN博客
2017年02月09日 05:25:56[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 在thinkjs框架中，如何判断请求是否为https协议请求
首先，我们要明白一个点，在[Think-js 官方文档 2.2 - http模块](https://thinkjs.org/zh-cn/doc/2.2/api_http.html)中，官方阐述了
> 
http.req , http.res是系统原生的request,response对象！
从整篇api文档来看，http模块并没有实现类似express的 req.protocol，req.secure的内容。因此我们无法从请求上直接获取该请求是http还是https。
### 怎么办？
了解如何获取协议类型，我们需要先有一些内容需要
### https是什么？
HTTPS (also called [HTTP over TLS](https://tools.ietf.org/html/rfc2818), [HTTP over SSL](https://docs.adobe.com/docs/en/cq/5-6-1/deploying/config-ssl.html),and HTTP Secure]) is a protocol for secure communication over a computer network which is widely used on the Internet.
简而言之https就是建立在安全套接层(ssl)或安全传输层协议(tls)上的安全通信协议，被广泛应用于英特网。不过目前来说，我们真正广泛使用的是TLS协议
### 如果使用TLS，我们该怎么判断
当使用TLS的时候 net.Socket会收到一个值为byte 22的头数据，该数据表明了握手时使用了TLS协议
```
var net = require('net');
var baseAddress = 3000;
net.createServer(tcpConnection).listen(baseAddress);
function tcpConnection(conn) {
    conn.once('data', function (buf) {
        var address = (buf[0] === 22) ? console.log('https') : console.log('http');
        });
    });
}
```
显然这个不适合我们，因为我们目前需要的是在控制器里获取到protocol类型。
另一种判断方法
> 
如果我们愿意翻看koa或者express的源码，我们可以很轻易的获取到我们想要的知识
以下是express对protocol的处理
```
/**
 * Return the protocol string "http" or "https"
 * when requested with TLS. When the "trust proxy"
 * setting trusts the socket address, the
 * "X-Forwarded-Proto" header field will be trusted
 * and used if present.
 *
 * If you're running behind a reverse proxy that
 * supplies https for you this may be enabled.
 *
 * @return {String}
 * @public
 */
defineGetter(req, 'protocol', function protocol(){
  var proto = this.connection.encrypted
    ? 'https'
    : 'http';
  var trust = this.app.get('trust proxy fn');
  if (!trust(this.connection.remoteAddress, 0)) {
    return proto;
  }
  // Note: X-Forwarded-Proto is normally only ever a
  //       single value, but this is to be safe.
  proto = this.get('X-Forwarded-Proto') || proto;
  return proto.split(/\s*,\s*/)[0];
});
/**
 * Short-hand for:
 *
 *    req.protocol === 'https'
 *
 * @return {Boolean}
 * @public
 */
defineGetter(req, 'secure', function secure(){
  return this.protocol === 'https';
});
```
从代码中我们可以看出，node在使用TLS的时候，会在原生的connection对象上携带加密信息，如果非TLS，这个request.connection.encrypted将会是undefined。如果express设置了信任代理，会先检测设置的信任代理的地址是否与远端地址相符，或者头信息中携带'X-Forwarded-Proto'（可能是来自于Nginx或其他代理时加上的协议指示）,也会返回相应protocol类型。
在thinkjs中，没有类似express app.set('trust proxy')的方法，所以我们直接在控制器里这样写就行了
```
// controller xxx.js
'use strict';
import Base from './base.js';
function getProtocol(http) {
    let proto = http.req.connection.encrypted ? 'https' : 'http';
    proto = http. header('X-Forwarded-Proto') || proto;
    return proto.split(/\s*,\s*/)[0];
}
export default class extends Base {
  /**
   * index action
   * @return {Promise} []
   */
  * indexAction(){
    console.log('protocol:', getProtocol(this.http))
  }
}
```
整合获取protocol功能略。
