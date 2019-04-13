
# Glide源码分析2 -- request创建与发送过程 - 谢杨易的博客 - CSDN博客

2016年08月07日 16:12:27[谢杨易](https://me.csdn.net/u013510838)阅读数：1843



## 1. 概述和核心类
使用Glide的一般形式为
Glide.with(context).load(url). placeholder(R.drawable.test).into(imageView)。
使用起来相当简单，大家有没有想过内部流程是怎么实现的呢？我是一个喜欢研究源码的人，抱着打破砂锅问到底的想法窥探了下Glide源码，发现这个流程主要与Glide的requests生成与发送过程有关。与它相关的核心类如下
1） Glide：提供了很多static方法，是我们使用的入口，其中的with方法比较关键，之后我们再详细分析
2）RequestManager：进行requests随Activity或Fragment生命周期管理，这部分可参见[ Glide源码分析 – 绑定Activity生命周期](http://blog.csdn.net/u013510838/article/details/52143097)。这个强大功能是Glide内部实现的，完全不需要用户参与。
3）GenericRequestBuilder: 使用builder模式创建Request，其中的into()方法比较关键，之后详细介绍。它的UML类图如下
![这里写图片描述](http://i1.piimg.com/567571/1b8131292bb8e27c.png)[ ](http://i1.piimg.com/567571/1b8131292bb8e27c.png)
针对不同格式的图片，Glide分别做了它们的request builder的实现。其他模块只需要依赖GenericRequestBuilder，满足依赖抽象的原则，后期支持别的格式会很方便，可扩展性强。
[
](http://i1.piimg.com/567571/1b8131292bb8e27c.png)4）GenericRequest：实现了Request接口，包含request状态和request操作API，如begin(), cancel(), clear(), pause()。 同时它实现了ResourceCallback接口，在获取图片数据exception时回调
onException(), 在获取数据成功时回调onResourceReady()
5）RequestTracker：requests的集合。包含两个集合，一个为requests， 包含所有的request。另一个为pendingRequests，包含等待的requests。
6）Target：图片的目标实体，相当于MVC中的View。只分析了关键的类，它的UML类图如下
![这里写图片描述](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)[ ](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
这里比较常用的是ImageViewTarget，它封装了ImageView和几个比较重要的listener。如onLoadStarted(), onLoadFailed(), onResourceReady()
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
## 2 Request生成与发送流程
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)以Glide.with(context).load(url). placeholder(R.drawable.test).into(imageView)为例分析
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)1）Glide.with(context)，详细过程在另一篇文章中有讲到，参看[ Glide源码分析 – 绑定Activity生命周期](http://blog.csdn.net/u013510838/article/details/52143097)
[

](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
```python
public
```
```python
static
```
```python
RequestManager
```
```python
with
```
```python
(Activity activity) {
        RequestManagerRetriever retriever = RequestManagerRetriever.
```
```python
get
```
```python
();
```
```python
// 获取RequestManager
```
```python
return
```
```python
retriever.
```
```python
get
```
```python
(activity);
    }
```
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)retriever.get()是这部分的关键点，获取了RequestManager对象。RequestManager主要作用为实现request和Activity生命周期相关联。在本文讲述的Glide这部分内容中关系不大。
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)2）load(String url)，创建DrawableTypeRequest并利用它的load方法设置相关参数
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
```python
public
```
```python
class
```
```python
RequestManager
```
```python
implements
```
```python
LifecycleListener
```
```python
{
```
```python
public
```
```python
DrawableTypeRequest<String> load(String string) {
```
```python
return
```
```python
(DrawableTypeRequest<String>) fromString().load(string);
    }
```
```python
public
```
```python
DrawableTypeRequest<String> fromString() {
```
```python
return
```
```python
loadGeneric(String.class);
    }
```
```python
private
```
```python
<T> DrawableTypeRequest<T> loadGeneric(
```
```python
Class
```
```python
<
```
```python
T
```
```python
>
```
```python
modelClass
```
```python
) {
```
```python
// 省略一段代码
```
```python
return
```
```python
optionsApplier.apply(
```
```python
// 创建DrawableTypeRequest，它是GenericRequestBuilder的子类
```
```python
new
```
```python
DrawableTypeRequest<T>(modelClass, streamModelLoader, fileDescriptorModelLoader, context,
                        glide, requestTracker, lifecycle, optionsApplier));
    }
}
```
```python
public
```
```python
class
```
```python
GenericRequestBuilder
```
```python
<
```
```python
ModelType
```
```python
,
```
```python
DataType
```
```python
,
```
```python
ResourceType
```
```python
,
```
```python
TranscodeType
```
```python
>
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
public
```
```python
GenericRequestBuilder<ModelType, DataType, ResourceType, TranscodeType> load(ModelType model) {
```
```python
// 设置相关参数，仅此而已
```
```python
this.model = model;
        isModelSet =
```
```python
true
```
```python
;
```
```python
return
```
```python
this;
    }
}
```
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)这部分关键点是new DrawableTypeRequest()，DrawableTypeRequest是GenericRequestBuilder的子类，故load()调用后builder就创建好了。之后就可以通过builder模式来配置参数了。
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)3) placeholder(R.drawable.test), 仅仅是设置GenericRequestBuilder中相关变量而已，典型的builder模式。其他方法如centerCrop()等都是这样实现的
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
```python
public
```
```python
class
```
```python
GenericRequestBuilder
```
```python
<
```
```python
ModelType
```
```python
,
```
```python
DataType
```
```python
,
```
```python
ResourceType
```
```python
,
```
```python
TranscodeType
```
```python
>
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
public
```
```python
GenericRequestBuilder<ModelType, DataType, ResourceType, TranscodeType> placeholder(
```
```python
int
```
```python
resourceId) {
```
```python
this
```
```python
.placeholderId = resourceId;
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
}
```
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)几乎所有的参数设置都是使用上面的类似方法
[
](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)4）into(imageview)，这是最关键的一个方法。分为两个阶段来分析
a) 创建Request对象并将builder中设入的参数，设置到Request数据类中
```python
// 先来看看Glide对View的封装。
```
```python
// Glide中对View对象做了封装，添加了listener等回调方法。凡是看到Target的地方都可以理解为封装后的View
```
```python
// Target的UML类层次关系图可以回头看看核心类讲解部分
```
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
ViewTarget
```
```python
<
```
```python
T
```
```python
extends
```
```python
View
```
```python
,
```
```python
Z
```
```python
>
```
```python
extends
```
```python
BaseTarget
```
```python
<
```
```python
Z
```
```python
> {
```
```python
public
```
```python
ViewTarget(T view) {
```
```python
if
```
```python
(view ==
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
"View must not be null!"
```
```python
);
        }
```
```python
// 将ImageView设置进去
```
```python
this
```
```python
.view = view;
        sizeDeterminer =
```
```python
new
```
```python
SizeDeterminer(view);
    }
}
```
```python
public
```
```python
class
```
```python
GenericRequestBuilder
```
```python
<
```
```python
ModelType
```
```python
,
```
```python
DataType
```
```python
,
```
```python
ResourceType
```
```python
,
```
```python
TranscodeType
```
```python
>
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
// 看到我们经常调用的方法了吧，哈哈~
```
```python
public
```
```python
Target<TranscodeType> into(ImageView view) {
```
```python
// 主线程判断，对View修改只能在主线程中，你懂得~
```
```python
Util.assertMainThread();
```
```python
if
```
```python
(view ==
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
IllegalArgumentException(
```
```python
"You must pass in a non null View"
```
```python
);
        }
```
```python
// 此处省略几行代码
```
```python
// 先封装ImageView为ImageViewTarget,再调用into(target)方法
```
```python
// 还记得前面说到过Glide对View做了封装吧，你就把target当做View好了
```
```python
return
```
```python
into(glide.buildImageViewTarget(view, transcodeClass));
    }
```
```python
// into()方法的关键调用，传入使用View封装好的ViewTarget
```
```python
public
```
```python
<Y extends Target<TranscodeType>> Y into(Y target) {
```
```python
// 确保在主线程中，这样才能对View做修改
```
```python
Util.assertMainThread();
```
```python
if
```
```python
(target ==
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
IllegalArgumentException(
```
```python
"You must pass in a non null Target"
```
```python
);
        }
```
```python
if
```
```python
(!isModelSet) {
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
"You must first set a model (try #load())"
```
```python
);
        }
        Request previous = target.getRequest();
```
```python
// 此ViewTarget如果已经有request，则删除掉它，并从requestTracker集合中remove掉
```
```python
if
```
```python
(previous !=
```
```python
null
```
```python
) {
            previous.clear();
            requestTracker.removeRequest(previous);
            previous.recycle();
        }
```
```python
// 创建Request，典型的builder模式调用。buildRequest()最后调用到obtainRequest()
```
```python
Request request = buildRequest(target);
        target.setRequest(request);
```
```python
// target实现了lifecycleListener，故将它加入到lifecycle，这样在恰当的时机得到回调
```
```python
lifecycle.addListener(target);
```
```python
// 加入到requestTracker中，并run。它是数据获取的入口，后面重点分析
```
```python
requestTracker.runRequest(request);
```
```python
return
```
```python
target;
    }
```
```python
// buildRequest最终调用到obtainRequest方法，创建Request的关键方法，
```
```python
// 将builder中的参数传入，这种方式传入参数太多，个人觉得不优雅，可以创建一个RequestParams数据类来存放参数
```
```python
private
```
```python
Request obtainRequest(Target<TranscodeType> target, float sizeMultiplier, Priority priority,
            RequestCoordinator requestCoordinator) {
```
```python
// 看到这么一大段是不是觉得很懵逼了，我也觉得这是Glide写的挺不优雅的一个地方
```
```python
return
```
```python
GenericRequest.obtain(
                loadProvider,
                model,
                signature,
                context,
                priority,
                target,
                sizeMultiplier,
                placeholderDrawable,
                placeholderId,
                errorPlaceholder,
                errorId,
                fallbackDrawable,
                fallbackResource,
                requestListener,
                requestCoordinator,
                glide.getEngine(),
                transformation,
                transcodeClass,
                isCacheable,
                animationFactory,
                overrideWidth,
                overrideHeight,
                diskCacheStrategy);
    }
}
```
```python
// GenericRequest类，builder中传入的参数最终大部分设到了Request数据类中
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
GenericRequest
```
```python
<
```
```python
A
```
```python
,
```
```python
T
```
```python
,
```
```python
Z
```
```python
,
```
```python
R
```
```python
> {
```
```python
// GenericRequest.obtain()，总算找到你了~。
```
```python
// 参数太多，就不列出来了，不然累成狗~
```
```python
public
```
```python
static
```
```python
<A, T, Z, R> GenericRequest<A, T, Z, R> obtain(...)
        GenericRequest<A, T, Z, R> request = (GenericRequest<A, T, Z, R>) REQUEST_POOL.poll();
```
```python
if
```
```python
(request ==
```
```python
null
```
```python
) {
```
```python
// 创建Request
```
```python
request =
```
```python
new
```
```python
GenericRequest<A, T, Z, R>();
        }
```
```python
// 利用入参初始化request中的参数，参数太多，就不一一写出来了
```
```python
request.init(...)；
```
```python
// 到这一步Request总算创建好了，百转千回，真心不容易呀~
```
```python
return
```
```python
request;
    }
}
```
b)调用 requestTracker中的runRequest()，发送request请求。创建好request只是into()方法的第一阶段，还需要将request请求发送出去。
```python
// 利用集合的方式来管理所有request，很常见的一种设计方式吧。关于RequestTracker类可以回过头看看核心类介绍部分
```
```python
// 不过小编觉得还可以尝试下对象池方式来管理request，这样不用重复的创建和回收
```
```python
// Glide中BitmapPool就是典型的对象池方式。
```
```python
// Android消息机制中的Message对象也是相当典型的对象池方式。设计模式上称为享元模式。扯远了，哈哈
```
```python
public
```
```python
class
```
```python
RequestTracker {
```
```python
public
```
```python
void
```
```python
runRequest
```
```python
(Request request) {
```
```python
// 添加到全部request所在集合中
```
```python
requests.add(request);
```
```python
if
```
```python
(!isPaused) {
```
```python
// 还记得核心类中讲Request时说过Request中有很多控制API吧
```
```python
// begin()方法是request发送的入口，下面重点介绍
```
```python
request.begin();
        }
```
```python
else
```
```python
{
```
```python
// 如果当前状态为paused，则不直接运行，而是添加到等待集合中。这一般发生在Activity或Fragment onStop()时
```
```python
pendingRequests.add(request);
        }
    }
}
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
GenericRequest {
```
```python
public
```
```python
void
```
```python
begin
```
```python
() {
```
```python
// 记录时间
```
```python
startTime = LogTime.getLogTime();
```
```python
if
```
```python
(model ==
```
```python
null
```
```python
) {
            onException(
```
```python
null
```
```python
);
```
```python
return
```
```python
;
        }
        status = Status.WAITING_FOR_SIZE;
```
```python
// 验证width和height是否合法
```
```python
if
```
```python
(Util.isValidDimensions(overrideWidth, overrideHeight)) {
```
```python
// 发送request的关键所在，当获取了width height之后就可以向底层发送request请求了。后面详细说
```
```python
onSizeReady(overrideWidth, overrideHeight);
        }
```
```python
else
```
```python
{
            target.getSize(
```
```python
this
```
```python
);
        }
```
```python
if
```
```python
(!isComplete() && !isFailed() && canNotifyStatusChanged()) {
```
```python
// 数据load开始前，回调onLoadStarted，一般此时viewTarget会设置placeHolder
```
```python
// 这是placeHolder占位图的实现原理，是不是顿时茅厕顿开了呀，哈哈~
```
```python
target.onLoadStarted(getPlaceholderDrawable());
        }
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
            logV(
```
```python
"finished run method in "
```
```python
+ LogTime.getElapsedMillis(startTime));
        }
    }
```
```python
public
```
```python
void
```
```python
onSizeReady
```
```python
(
```
```python
int
```
```python
width,
```
```python
int
```
```python
height) {
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
            logV(
```
```python
"Got onSizeReady in "
```
```python
+ LogTime.getElapsedMillis(startTime));
        }
```
```python
if
```
```python
(status != Status.WAITING_FOR_SIZE) {
```
```python
return
```
```python
;
        }
```
```python
// 状态切换
```
```python
status = Status.RUNNING;
```
```python
// 部分代码省略
```
```python
// 采用engine来load数据，此处是最终request发送所在。涉及到cache等很多内容，后面一篇文章再详细分析
```
```python
loadStatus = engine.load(signature, width, height, dataFetcher, loadProvider, transformation, transcoder,
                priority, isMemoryCacheable, diskCacheStrategy,
```
```python
this
```
```python
);
        loadedFromMemoryCache = resource !=
```
```python
null
```
```python
;
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
            logV(
```
```python
"finished onSizeReady in "
```
```python
+ LogTime.getElapsedMillis(startTime));
        }
    }
}
```
```python
// ImageViewTarget类，再回过头看看placeHolder是如何被设到ImageView里面的
```
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
ImageViewTarget {
```
```python
public
```
```python
void
```
```python
onLoadStarted
```
```python
(Drawable placeholder) {
```
```python
// onLoadStarted中设置placeHolder到ImageView中，也就是占位符设置
```
```python
view.setImageDrawable(placeholder);
    }
}
```
into()方法是比较关键也比较麻烦的一个方法。Glide将View封装为了ViewTarget。into()方法分为request创建和request发送两大部分。创建采用Builder模式生成GenericRequest对象，发送则调用GenericRequest的begin方法，最终回调到底层Engine部分的相关方法。可以看出，经过Glide的层层封装，复杂的逻辑变得相对简单了很多。
## 3 总结
常用的Glide的接口其实本质上分为两步，request的创建，和request的发送。创建过程采用builder模式生成Request对象。发送过程则调用engine.load(), 详细过程下篇文章再分析。其中还根据不同状态，回调了ViewTarget中的不同方法。整个过程还是很复杂的。
[
						](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
[
	](http://i2.piimg.com/567571/9a63ba2fda6bac9b.png)
