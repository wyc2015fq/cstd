
# OKHttp源码分析2 - Request的创建和发送 - 谢杨易的博客 - CSDN博客

2016年09月03日 18:19:05[谢杨易](https://me.csdn.net/u013510838)阅读数：1603



## 1 概述
我们先来看一个使用OKHttp的典型例子
```python
//builder模式创建一个Request
```
```python
Request request =
```
```python
new
```
```python
Request.Builder()
                .url(
```
```python
"https://baidu.com"
```
```python
)
                .build();
```
```python
//创建okHttpClient对象
```
```python
OkHttpClient mOkHttpClient =
```
```python
new
```
```python
OkHttpClient();
```
```python
//创建call
```
```python
Call call = mOkHttpClient.newCall(request);
```
```python
//异步方式加入队列中，并添加回调方法
```
```python
call.enqueue(
```
```python
new
```
```python
Callback() {
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onFailure
```
```python
(Request request, IOException e) {
            }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onResponse
```
```python
(
```
```python
final
```
```python
Response response)
```
```python
throws
```
```python
IOException {
            }
 });
```
OKHttp使用起来还是相当简单的，大家是否已经跃跃欲试想了解下它的底层实现呢。从使用中我们看到，它大致分为两步：Request的创建和Request的发送。现在就带大家一起来分析下这两个步骤。对Http协议不熟悉的同学，可以先看看我的这篇文章[Http协议简介](http://blog.csdn.net/u013510838/article/details/52431558)
## 2 Request创建流程分析
Request创建使用了十分典型的builder模式。
```python
Request.java
```
```python
public
```
```python
static
```
```python
class
```
```python
Builder {
```
```python
public
```
```python
Builder
```
```python
() {
```
```python
// 默认采用的GET方式
```
```python
this
```
```python
.method =
```
```python
"GET"
```
```python
;
```
```python
// request的首部也是采用builder模式创建
```
```python
this
```
```python
.headers =
```
```python
new
```
```python
Headers.Builder();
    }
```
```python
// url肯定是少不了的方法，不然可就要报Exception了哦~
```
```python
public
```
```python
Builder
```
```python
url
```
```python
(HttpUrl url) {
```
```python
if
```
```python
(url ==
```
```python
null
```
```python
)
```
```python
throw
```
```python
new
```
```python
IllegalArgumentException(
```
```python
"url == null"
```
```python
);
```
```python
this
```
```python
.url = url;
```
```python
return
```
```python
this
```
```python
;
    }
```
```python
public
```
```python
Request
```
```python
build
```
```python
() {
```
```python
// 看到了吧，少了谁都不能少了url
```
```python
if
```
```python
(url ==
```
```python
null
```
```python
)
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"url == null"
```
```python
);
```
```python
// builder模式最终创建出Request对象
```
```python
return
```
```python
new
```
```python
Request(
```
```python
this
```
```python
);
    }
```
```python
// Request类的构造方法
```
```python
private
```
```python
Request
```
```python
(Builder builder) {
```
```python
// 简单的赋值，对builder模式熟悉的同学肯定不会陌生了
```
```python
this
```
```python
.url = builder.url;
```
```python
this
```
```python
.method = builder.method;
```
```python
this
```
```python
.headers = builder.headers.build();
```
```python
this
```
```python
.body = builder.body;
```
```python
this
```
```python
.tag = builder.tag !=
```
```python
null
```
```python
? builder.tag :
```
```python
this
```
```python
;
  }
}
```
Request的创建是不是简单到小儿科了啊。为了扩展下大家视野，后面核心类讲解部分会详细说Request中的一些主要参数，这样咱们才可以掌握一些高级用法了。很期待，是不是？
## 3 Request发送流程分析
![Markdown](http://i4.buimg.com/573600/73b55169626244be.png)
Request对象代表了客户端HTTP请求的数据，它被封装在Call这个对象中。这个过程也很简单，看下面的代码分析。
```python
public
```
```python
class
```
```python
OkHttpClient
```
```python
implements
```
```python
Cloneable
```
```python
{
```
```python
// 包装Request对象
```
```python
public
```
```python
Call
```
```python
newCall
```
```python
(Request request) {
```
```python
return
```
```python
new
```
```python
Call(
```
```python
this
```
```python
, request);
  }
```
```python
protected
```
```python
Call
```
```python
(OkHttpClient client, Request originalRequest) {
```
```python
// 创建一个新的OKHttpClient对象，然后使用default的OKHttpClient赋值给它，不用关注这个方法，不是重点
```
```python
this
```
```python
.client = client.copyWithDefaults();
```
```python
this
```
```python
.originalRequest = originalRequest;
  }
}
```
下面就到了Request发送的主要过程了。使用Dispatcher调度器根据当前request总数目，立即执行request或先放入等待队列中。立即执行时采用了线程池方式利用子线程来执行。
```python
public
```
```python
class
```
```python
Call
```
```python
{
```
```python
// request发送的起始点，一看enqueue小编就猜到跟队列啥的有关系了
```
```python
public
```
```python
void
```
```python
enqueue
```
```python
(Callback responseCallback) {
    enqueue(responseCallback,
```
```python
false
```
```python
);
  }
```
```python
void
```
```python
enqueue(Callback responseCallback,
```
```python
boolean
```
```python
forWebSocket) {
```
```python
synchronized
```
```python
(
```
```python
this
```
```python
) {
```
```python
if
```
```python
(executed)
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"Already Executed"
```
```python
);
      executed =
```
```python
true
```
```python
;
    }
```
```python
// 调用了Dispatcher对象的enqueue，将回调方法封装在了AsyncCall里面，我们接下来重点分析它们
```
```python
client.getDispatcher().enqueue(
```
```python
new
```
```python
AsyncCall(responseCallback, forWebSocket));
  }
}
```
```python
// 调度器，用来管理多个Request的发送的
```
```python
public
```
```python
final
```
```python
class
```
```python
Dispatcher
```
```python
{
```
```python
// APP内当前存活总request数目最大值，setMaxRequests()方法可更改它
```
```python
private
```
```python
int
```
```python
maxRequests =
```
```python
64
```
```python
;
```
```python
// 发往单个host的request的最大值。不清楚host的同学可以先简单将host理解为http://www.baidu.com中的www.baidu.com，
```
```python
// 它最终会被DNS解析为服务器的IP地址
```
```python
// setMaxRequestsPerHost()可更改它
```
```python
private
```
```python
int
```
```python
maxRequestsPerHost =
```
```python
5
```
```python
;
```
```python
// 等待队列，合适时机时才开始run
```
```python
private
```
```python
final
```
```python
Deque<AsyncCall> readyCalls =
```
```python
new
```
```python
ArrayDeque<>();
```
```python
// 运行队列，里面的request可以得到立即执行。
```
```python
// 被cancel但没有finish的request也在这里面。所以要注意finish request哦，别占着茅坑不那啥哈~
```
```python
private
```
```python
final
```
```python
Deque<AsyncCall> runningCalls =
```
```python
new
```
```python
ArrayDeque<>();
```
```python
// 调度器来安排将request放入运行队列并立即run，还是放到等待队列
```
```python
synchronized
```
```python
void
```
```python
enqueue(AsyncCall call) {
```
```python
if
```
```python
(runningCalls.size() < maxRequests && runningCallsForHost(call) < maxRequestsPerHost) {
```
```python
// 太幸运了，request数目还有空余，可以立即执行了
```
```python
runningCalls.add(call);
```
```python
// 典型的线程池方式调用子线程来执行。
```
```python
getExecutorService().execute(call);
    }
```
```python
else
```
```python
{
```
```python
// Request实在太多了，抱歉只能放入等待队列了。生晚了就是后娘养的啊，泪崩~
```
```python
readyCalls.add(call);
    }
  }
}
```
线程池执行子线程的过程大家都懂，我们就不详细分析了哈。我们应该重点关注子线程执行的任务Runnable，也就是我们这儿的AsyncCall对象。AsyncCall没有提供run()方法，但它的父类NamedRunnable中提供了。我们一个个来看。
```python
public
```
```python
abstract
```
```python
class
```
```python
NamedRunnable
```
```python
implements
```
```python
Runnable
```
```python
{
```
```python
// run方法，好亲切哦~
```
```python
@Override
```
```python
public
```
```python
final
```
```python
void
```
```python
run() {
```
```python
// 省略无关代码
```
```python
try
```
```python
{
```
```python
// 入口在这儿，由实现类自己去实现。
```
```python
// 父类实现了共同的方法和步骤，而将差异化的方法放在子类中，这种设计思想大家应该用腻了吧~
```
```python
execute();
    }
```
```python
finally
```
```python
{
      Thread.currentThread().setName(oldName);
    }
  }
```
```python
protected
```
```python
abstract
```
```python
void
```
```python
execute();
}
```
```python
// 重点关注它如何实现的execute()方法
```
```python
final
```
```python
class
```
```python
AsyncCall
```
```python
extends
```
```python
NamedRunnable
```
```python
{
```
```python
@Override
```
```python
protected
```
```python
void
```
```python
execute() {
```
```python
boolean
```
```python
signalledCallback =
```
```python
false
```
```python
;
```
```python
try
```
```python
{
```
```python
// 发送的重点，接下来详细分析
```
```python
Response response = getResponseWithInterceptorChain(forWebSocket);
```
```python
if
```
```python
(canceled) {
          signalledCallback =
```
```python
true
```
```python
;
```
```python
// response失败时回调我们最开始传入的callback的onFailure(), 看到了回调的地方了吧~
```
```python
responseCallback.onFailure(originalRequest,
```
```python
new
```
```python
IOException(
```
```python
"Canceled"
```
```python
));
        }
```
```python
else
```
```python
{
          signalledCallback =
```
```python
true
```
```python
;
```
```python
// 成功则回调最开始传入的callback的onResponse()
```
```python
responseCallback.onResponse(response);
        }
      }
```
```python
catch
```
```python
(IOException e) {
```
```python
if
```
```python
(signalledCallback) {
```
```python
// 发生异常且callback已经被回调了，则不能再回调callback了，否则就多回调了一次。
```
```python
logger.log(Level.INFO,
```
```python
"Callback failure for "
```
```python
+ toLoggableString(), e);
        }
```
```python
else
```
```python
{
```
```python
// 发生异常但callback没有被回调，则我们回调callback的onFailure()
```
```python
Request request = engine ==
```
```python
null
```
```python
? originalRequest : engine.getRequest();
          responseCallback.onFailure(request, e);
        }
      }
```
```python
finally
```
```python
{
```
```python
// 不论如何，最终都得finish掉request
```
```python
client
```
```python
.getDispatcher().finished(
```
```python
this
```
```python
);
      }
    }
  }
}
```
可以看到，run(）方法利用getResponseWithInterceptorChain()发送request并获取response，然后根据结果来分别回调我们最开始传入的callback的onFailure()和onResponse()。知道了callback回调的地方，总算放心了！接下来重点分析getResponseWithInterceptorChain()
```python
public
```
```python
class
```
```python
Call
```
```python
{
```
```python
private
```
```python
Response getResponseWithInterceptorChain(
```
```python
boolean
```
```python
forWebSocket) throws IOException {
    Interceptor.Chain chain =
```
```python
new
```
```python
ApplicationInterceptorChain(
```
```python
0
```
```python
, originalRequest, forWebSocket);
```
```python
// 下面分析proceed
```
```python
return
```
```python
chain.proceed(originalRequest);
  }
}
```
```python
class
```
```python
ApplicationInterceptorChain
```
```python
implements
```
```python
Interceptor
```
```python
.
```
```python
Chain
```
```python
{
```
```python
@Override
```
```python
public
```
```python
Response proceed(Request request) throws IOException {
```
```python
// 先执行interceptor拦截器，拦截器会做一些预处理，不用细看
```
```python
if
```
```python
(
```
```python
index
```
```python
<
```
```python
client
```
```python
.interceptors().size()) {
        Interceptor.Chain chain =
```
```python
new
```
```python
ApplicationInterceptorChain(
```
```python
index
```
```python
+
```
```python
1
```
```python
, request, forWebSocket);
        Interceptor interceptor =
```
```python
client
```
```python
.interceptors().get(
```
```python
index
```
```python
);
        Response interceptedResponse = interceptor.intercept(chain);
```
```python
if
```
```python
(interceptedResponse ==
```
```python
null
```
```python
) {
```
```python
throw
```
```python
new
```
```python
NullPointerException(
```
```python
"application interceptor "
```
```python
+ interceptor
              +
```
```python
" returned null"
```
```python
);
        }
```
```python
return
```
```python
interceptedResponse;
      }
```
```python
// 再进行Http request和response
```
```python
return
```
```python
getResponse(request, forWebSocket);
    }
```
```python
// 这篇文章最核心的方法，没有之一！
```
```python
Response getResponse(Request request,
```
```python
boolean
```
```python
forWebSocket) throws IOExbodyception {
```
```python
// 获取request的body并根据它在header中填充Content-Type， Content-Length， Transfer-Encoding字段
```
```python
// 对Http规范不是很了解的童鞋可以先看我的另一篇文章 Http协议简介
```
```python
RequestBody body = request.body();
```
```python
if
```
```python
(body !=
```
```python
null
```
```python
) {
```
```python
// 省略一段代码。对header进行填充，不是我们关注的重点，知道有这件事就行了。
```
```python
}
```
```python
// 创建HttpEngine，它是底层核心类，下一篇文章详细分析
```
```python
engine =
```
```python
new
```
```python
HttpEngine(
```
```python
client
```
```python
, request,
```
```python
false
```
```python
,
```
```python
false
```
```python
, forWebSocket,
```
```python
null
```
```python
,
```
```python
null
```
```python
,
```
```python
null
```
```python
,
```
```python
null
```
```python
);
```
```python
int
```
```python
followUpCount =
```
```python
0
```
```python
;
```
```python
// 由于要处理request和它可能的后续request，故使用了while循环
```
```python
// 后续request举一个例子大家就明白了。
```
```python
// server上某个页面移到了另外一个地址后，如果client发送此页面请求，server会发送重定向（redirect）的response，其中包含了页面新地址
```
```python
// okHttp根据页面新地址，重新组建request，发送给server。
```
```python
// server这次就可以回复页面的response了。
```
```python
// followUp request由OKHttp自动完成，大大方便了大家。功能如此之强大，你还不快使用OKHttp？
```
```python
while
```
```python
(
```
```python
true
```
```python
) {
```
```python
if
```
```python
(canceled) {
```
```python
// 取消了request，则关闭Http连接。
```
```python
// Http是短连接，不使用keep-alive技术时，每个request和response来回之后都要关闭连接。之后再发送request则重新连接
```
```python
engine.releaseConnection();
```
```python
throw
```
```python
new
```
```python
IOException(
```
```python
"Canceled"
```
```python
);
      }
```
```python
try
```
```python
{
```
```python
// 发送client的request，关键方法，下一篇再分析
```
```python
engine.sendRequest();
```
```python
// 获取server的response，下一篇再分析
```
```python
engine.readResponse();
      }
```
```python
catch
```
```python
(RequestException e) {
```
```python
// 异常处理逻辑，不用看，代码省略
```
```python
}
```
```python
// 处理followUp request, 前面已经解释了何为followUp request
```
```python
Response response = engine.getResponse();
      Request followUp = engine.followUpRequest();
```
```python
// followUp request全都处理完，跳出while循环，并返回最终的response
```
```python
if
```
```python
(followUp ==
```
```python
null
```
```python
) {
```
```python
if
```
```python
(!forWebSocket) {
          engine.releaseConnection();
        }
```
```python
return
```
```python
response;
      }
```
```python
// followUp request不能太多，否则可能陷入无止境的循环中。例如有些server故意弄一系列的重定向，我们不能被它坑了！
```
```python
if
```
```python
(++followUpCount > MAX_FOLLOW_UPS) {
```
```python
throw
```
```python
new
```
```python
ProtocolException(
```
```python
"Too many follow-up requests: "
```
```python
+ followUpCount);
      }
```
```python
if
```
```python
(!engine.sameConnection(followUp.httpUrl())) {
        engine.releaseConnection();
      }
```
```python
// http短连接，本次request完成，则要关闭连接。下一个request再重新连接。是不是感觉效率弱爆了？keep-alive可以破此局
```
```python
Connection connection = engine.close();
```
```python
// 处理followUp request，也是利用HttpEngine，跟之前request处理流程一样
```
```python
request = followUp;
      engine =
```
```python
new
```
```python
HttpEngine(
```
```python
client
```
```python
, request,
```
```python
false
```
```python
,
```
```python
false
```
```python
, forWebSocket, connection,
```
```python
null
```
```python
,
```
```python
null
```
```python
,
          response);
    }
  }
}
```
总算分析完了request发送流程了，过程还是相当麻烦的。HttpEngine下篇文章再分析。小编此刻已经四肢瘫软，好想来个葛优躺。不过革命还未成功，OKHttp还得继续分析。
这里还得说一下，除了异步方式之外，还可以采用同步方式。使用例子如下。
```python
// builder模式创建一个Request
Request request = new Request
```
```python
.Builder
```
```python
()
```
```python
.url
```
```python
(
```
```python
"https://baidu.com"
```
```python
)
```
```python
.build
```
```python
()
```
```python
;
```
```python
//创建okHttpClient对象
OkHttpClient mOkHttpClient = new OkHttpClient()
```
```python
;
```
```python
//创建
```
```python
call
```
```python
Call
```
```python
call
```
```python
= mOkHttpClient
```
```python
.newCall
```
```python
(request)
```
```python
;
```
```python
//同步方式加入队列中
```
```python
call
```
```python
.execute
```
```python
()
```
```python
;
```
与异步方式唯一的不同之处在于使用的execute()，而非enqueue()方法。下面分析execute()方法
```python
public
```
```python
Response
```
```python
execute
```
```python
()
```
```python
throws
```
```python
IOException {
```
```python
synchronized
```
```python
(
```
```python
this
```
```python
) {
```
```python
if
```
```python
(executed)
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"Already Executed"
```
```python
);
      executed =
```
```python
true
```
```python
;
    }
```
```python
try
```
```python
{
```
```python
// 直接execute，仅仅是加入ArrayDeque中，没有使用线程池利用子线程来执行，故称为同步方式
```
```python
client.getDispatcher().executed(
```
```python
this
```
```python
);
```
```python
// 同样利用getResponseWithInterceptorChain()方法
```
```python
// 先调用拦截器，然后利用HttpEngine sendRequest()和readResponse()，以及处理followUp request
```
```python
Response result = getResponseWithInterceptorChain(
```
```python
false
```
```python
);
```
```python
if
```
```python
(result ==
```
```python
null
```
```python
)
```
```python
throw
```
```python
new
```
```python
IOException(
```
```python
"Canceled"
```
```python
);
```
```python
return
```
```python
result;
    }
```
```python
finally
```
```python
{
      client.getDispatcher().finished(
```
```python
this
```
```python
);
    }
  }
```
```python
private
```
```python
final
```
```python
Deque<Call> executedCalls =
```
```python
new
```
```python
ArrayDeque<>();
    executedCalls.add(call);
  }
}
```
## 4 核心类
看完了同步和异步两种方式的整体流程之后，大家是不是对使用OKHttp更加胸有成竹了呢。下面跟大家一块分析下request创建和发送过程中使用到的核心类。
1）request：数据类，代表了client发送的网络请求，它的主要字段如下
```python
private
```
```python
final
```
```python
HttpUrl url;
```
```python
// url应该不用说了吧
```
```python
private
```
```python
final
```
```python
String
```
```python
method
```
```python
;
```
```python
// 请求方法，有get，post，put，delete，options等
```
```python
private
```
```python
final
```
```python
Headers headers;
```
```python
// 首部，request和response都包含三部分，start line，headers，body
```
```python
private
```
```python
final
```
```python
RequestBody body;
```
```python
// 主体部分
```
```python
private
```
```python
final
```
```python
Object tag;
```
```python
// 与http request协议无关，OKHttp中作为Request的标签，cancel Request时经常用
```
```python
private
```
```python
volatile
```
```python
CacheControl cacheControl;
```
```python
// 控制cache的使用
```
2）Response：数据类，代表了server的回复。主要的字段如下
```python
private
```
```python
final
```
```python
Request request;
```
```python
private
```
```python
final
```
```python
Protocol protocol;
```
```python
// start line中的协议类型，如Http1.0， Http1.1
```
```python
private
```
```python
final
```
```python
int
```
```python
code;
```
```python
// start line中的返回码，如404，表示文件找不到
```
```python
private
```
```python
final
```
```python
String message;
```
```python
// 位于start line中，用来解释返回码的字符串
```
```python
private
```
```python
final
```
```python
Handshake handshake;
```
```python
// 握手，TCP连接时要三次握手
```
```python
private
```
```python
final
```
```python
Headers headers;
```
```python
// 首部
```
```python
private
```
```python
final
```
```python
ResponseBody body;
```
```python
// 主体
```
```python
private
```
```python
Response networkResponse;
```
```python
// server的response
```
```python
private
```
```python
Response cacheResponse;
```
```python
// cache的response
```
```python
private
```
```python
final
```
```python
Response priorResponse;
```
3）OkHttpClient：代表客户端，提供了很多client使用的方法。是我们APP中经常打交道的一个类。建议全局使用一个。如果要使用多个，建议采用clone方法创建。
4）Call：主要的控制类，包含enqueue()和execute()，同步和异步发送request的两个方法。内部类AsyncCall 实现了异步调用子线程中使用的Runnable。
5）Dispatcher：调度器，以队列方式来管理多个requests。包含readyCalls和runningCalls，一个是等待队列，一个是运行队列。类似于线程池的思想，防止过多requests一块运行。
## 5 总结
OKHttp整个框架还是十分复杂的，本篇文章主要分析了APP中使用OKHttp时的主要API调用流程。至于底层Http request和response，下一篇文章会重点分析。

