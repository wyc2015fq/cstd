
# OKHttp源码分析1 - 框架 - 谢杨易的博客 - CSDN博客

2016年09月03日 18:16:57[谢杨易](https://me.csdn.net/u013510838)阅读数：3439



## 1 OKHttp介绍
网络库一直是Android APP开发中至关重要的库，关系到加载速度，流量消耗等诸多问题。Android系统提供了HttpClient和HttpURLConnection两个网络库。相比HttpClient，OKHttp性能更好，且API设计更加易用。相比HttpURLConnection，OKHttp功能更加强大。目前采用OKHttp方案的APP越来越多，搞懂它的底层源码有助于我们更好的优化APP中网络方案，更快定位bug。也能学到很多好的设计模式。所以是时候搞懂OKHttp的源码了！
## 2 OKHttp框架
OKHttp分为两层，上层主要提供API给用户调用，实现了request的创建和发送。底层依据Http协议为上层提供sendRequest(), readResponse()和followUpRequest()等方法。
OKHttp分层十分清晰，上层与Http协议关联不大，而底层则依据Http协议实现了三个关键方法。这种良好的设计模式有利于以后在框架中加入对除了Http之外的协议的支持。目前有些团队已经在研究基于TCP来实现网络库。
根据OKHttp的分层，后面分别通过两篇文章来详细讲述。
[OKHttp源码分析2 - Request的创建和发送](http://blog.csdn.net/u013510838/article/details/52424649)
[OKHttp源码分析3 - HttpEngine底层实现](http://blog.csdn.net/u013510838/article/details/52431516)
## 3 我的封装网络库的SDK
目前网络库，不论是Google自家的，还是第三方库，都非常多。无耻的在这里打个广告。我写了一个网络SDK，使用的例子如下。
```python
NetLoader
                .with(
```
```python
this
```
```python
)
                .url(
```
```python
"https://www.baidu.com/"
```
```python
)
                .listener(
```
```python
new
```
```python
NetRequestListener() {
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
onStarted
```
```python
() {
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
onSucceed
```
```python
(Object result) {
                        Toast.makeText(MainActivity.
```
```python
this
```
```python
, (String) result, Toast.LENGTH_SHORT).show();
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
onFailed
```
```python
(Exception e) {
                    }
                })
                .emptyView(
```
```python
new
```
```python
EmptyView(
```
```python
this
```
```python
) {
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
resetAsFetching
```
```python
() {
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
resetAsFailed
```
```python
() {
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
resetAsEmpty
```
```python
() {
                    }
                })
                .load();
```
它有如下优点：
1）提供了对不同网络库的统一封装。大家可以使用封装后的API来开发，更换底层网络库不需要更改app代码。
2）支持网络请求Request跟随Activity或Fragment生命周期。也就是Activity destroy时，Request自动finish；Activity stop时，Request暂停；Activity restart时，Request继续进行。（类似于Glide的处理方式）。这样用户完全不同担心Request 没有finish，导致网络资源没有释放的情况。
3）支持网络加载中，加载数据为空，加载失败等不同View画面展示。
4）用gradle来管理多个网络库的添加和更改，一行代码就可以搞定更换网络库这个相对比较麻烦的事情。
SDK分为APP层和SDK层。APP层中有使用sdk API的demo页面。大家需要下载两个git，并将它们放在同一个目录底下。目前SDK中除了加入了网络库封装外，还封装了图片库和Json解析库。我会将更多的常用库做好封装。使用这些封装好的库，你可以分分钟开发出一个高大上的APP。
双手奉上SDK的地址：
[https://github.com/xieyangyi/FrameApp.git](https://github.com/xieyangyi/FrameApp.git)
[https://github.com/xieyangyi/FrameSdk.git](https://github.com/xieyangyi/FrameSdk.git)

