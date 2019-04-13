
# Glide源码分析4 -- 缓存，编解码和网络请求 - 谢杨易的博客 - CSDN博客

2016年08月07日 16:14:47[谢杨易](https://me.csdn.net/u013510838)阅读数：2659



## 1. 概述和核心类
在[Glide源码分析 – request创建与发送过程](http://blog.csdn.net/u013510838/article/details/52143086)一文中，我们谈到request最终通过GenericRequest的onSizeReady()方法进行，其中调用了engine.load()方法去实际获取数据。本文主要讲述engine.load()之后发生的那些事，让大家能够对底层数据获取有个更清晰的认识。从这点也可以看出Glide设计分层的精妙。主要涉及的核心类如下
1）GenericRequest：定义了很多对request的处理方法，我们比较关心的是request的发送，它的入口是begin()，会调用到onSizeReady()，最终调用到engine.load()，也就是数据获取部分的入口
2）Engine：封装了数据获取的很多关键方法，向request层提供这些API，比如load(), release(), clearDiskCache()等方法。可以认为是一个外观模式。
3）MemoryCache：内存缓存类，先从缓存中获取数据，如果没有才做后面的工作。这是第一级缓存。Glide采用了两级缓存模式。第二级缓存为DiskLruCache，为磁盘缓存。获取磁盘缓存比较耗时，需要在子线程中进行，故而在DecodeJob中得到调用。此处不会调用磁盘缓存。
4）EngineJob， EngineRunnable：EngineJob是一个控制类，作为EngineRunnable的管理者，提供start(), cancel()等很多操作runnable的方法。一般会以线程池的方式向子线程提交EngineRunnable任务。而EngineRunnable就是我们在子线程中需要执行的任务，也是特别关键的一个类。
5）DecodeJob，DataFetcher，ResourceDecoder，Transformation：DecodeJob流程为从缓存或网络或本地获取数据，然后转码为所需的格式，最后编码并保存到DiskLruCache中。这是数据获取阶段很关键的一个类。DataFetcher负责根据不同途径数据获取（如本地File，url，URI等），ResourceDecoder负责根据不同文件格式解码（如Bitmap，GIF等）Transformation负责编码为不同格式文件（如Bitmap，GIF等）。后面在类层次关系中会详细讲解这几个类的关系以及它们的子类。现在只需要知道这几个类的作用就可以了。
上面讲解了五个方面，大概十多个类。我们可以把整个过程分为两个阶段：任务提交阶段和任务执行阶段。为了更清晰的理清逻辑关系，可以看下面这张图。
任务提交阶段：
![这里写图片描述](http://i4.piimg.com/567571/17bfe74c32e06d45.png)
任务执行阶段
![这里写图片描述](http://i4.piimg.com/567571/e3f4199dad6f647b.png)
## 2. 任务提交阶段源码分析
1）GenericRequest的begin()方法是整个提交阶段的入口，它会调用engine来完成任务的提交，并回调一些listener。这些listener是我们经常使用的，从下面的源码中我们可以清晰的看见这些listener的调用时机。
```python
public
```
```python
void
```
```python
begin() {
        startTime
```
```python
=
```
```python
LogTime
```
```python
.
```
```python
getLogTime();
```
```python
if
```
```python
(model
```
```python
==
```
```python
null
```
```python
) {
```
```python
// loadModel为空时，会回调requestListener的onException()
```
```python
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
        status
```
```python
=
```
```python
Status
```
```python
.
```
```python
WAITING_FOR_SIZE;
```
```python
if
```
```python
(Util
```
```python
.
```
```python
isValidDimensions(overrideWidth, overrideHeight)) {
```
```python
// size验证通过后，会提交请求
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
            target
```
```python
.
```
```python
getSize(this);
        }
```
```python
if
```
```python
(
```
```python
!
```
```python
isComplete()
```
```python
&&
```
```python
!
```
```python
isFailed()
```
```python
&&
```
```python
canNotifyStatusChanged()) {
```
```python
// onLoadStarted回调时机，任务提交最开始的时候
```
```python
target
```
```python
.
```
```python
onLoadStarted(getPlaceholderDrawable());
        }
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
            logV(
```
```python
"finished run method in "
```
```python
+
```
```python
LogTime
```
```python
.
```
```python
getElapsedMillis(startTime));
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
onSizeReady(int width, int height) {
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
            logV(
```
```python
"Got onSizeReady in "
```
```python
+
```
```python
LogTime
```
```python
.
```
```python
getElapsedMillis(startTime));
        }
```
```python
if
```
```python
(status
```
```python
!=
```
```python
Status
```
```python
.
```
```python
WAITING_FOR_SIZE) {
```
```python
return
```
```python
;
        }
        status
```
```python
=
```
```python
Status
```
```python
.
```
```python
RUNNING;
```
```python
// 计算尺寸
```
```python
width
```
```python
=
```
```python
Math
```
```python
.
```
```python
round(sizeMultiplier
```
```python
*
```
```python
width);
        height
```
```python
=
```
```python
Math
```
```python
.
```
```python
round(sizeMultiplier
```
```python
*
```
```python
height);
        ModelLoader
```
```python
<
```
```python
A, T
```
```python
>
```
```python
modelLoader
```
```python
=
```
```python
loadProvider
```
```python
.
```
```python
getModelLoader();
```
```python
// 获取DataFetcher，我们可以自定义DataFetcher
```
```python
final DataFetcher
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
dataFetcher
```
```python
=
```
```python
modelLoader
```
```python
.
```
```python
getResourceFetcher(model, width, height);
```
```python
if
```
```python
(dataFetcher
```
```python
==
```
```python
null
```
```python
) {
```
```python
// 会回调requestListener的onException()
```
```python
onException(
```
```python
new
```
```python
Exception(
```
```python
"Failed to load model: \'"
```
```python
+
```
```python
model
```
```python
+
```
```python
"\'"
```
```python
));
```
```python
return
```
```python
;
        }
        ResourceTranscoder
```
```python
<
```
```python
Z, R
```
```python
>
```
```python
transcoder
```
```python
=
```
```python
loadProvider
```
```python
.
```
```python
getTranscoder();
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
            logV(
```
```python
"finished setup for calling load in "
```
```python
+
```
```python
LogTime
```
```python
.
```
```python
getElapsedMillis(startTime));
        }
```
```python
// 默认使用内存缓存
```
```python
loadedFromMemoryCache
```
```python
=
```
```python
true
```
```python
;
```
```python
// 进入Engine的入口，十分关键
```
```python
loadStatus
```
```python
=
```
```python
engine
```
```python
.
```
```python
load(signature, width, height, dataFetcher, loadProvider, transformation, transcoder,
                priority, isMemoryCacheable, diskCacheStrategy, this);
        loadedFromMemoryCache
```
```python
=
```
```python
resource
```
```python
!=
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
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
            logV(
```
```python
"finished onSizeReady in "
```
```python
+
```
```python
LogTime
```
```python
.
```
```python
getElapsedMillis(startTime));
        }
    }
```
2）engine.load(), 先尝试从内存缓存获取数据，再尝试从当前活跃Resources中获取数据，再看看这个任务是否当前已经提交过了。这些都没有的话，最后提交任务。它规范了整个任务提交的流程，可以看做是一个模板方法。
```python
public
```
```python
<
```
```python
T, Z, R
```
```python
>
```
```python
LoadStatus load(Key signature, int width, int height, DataFetcher
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
fetcher,
            DataLoadProvider
```
```python
<
```
```python
T, Z
```
```python
>
```
```python
loadProvider, Transformation
```
```python
<
```
```python
Z
```
```python
>
```
```python
transformation, ResourceTranscoder
```
```python
<
```
```python
Z, R
```
```python
>
```
```python
transcoder,
            Priority priority, boolean isMemoryCacheable, DiskCacheStrategy diskCacheStrategy, ResourceCallback cb) {
        Util
```
```python
.
```
```python
assertMainThread();
        long startTime
```
```python
=
```
```python
LogTime
```
```python
.
```
```python
getLogTime();
        final
```
```python
String
```
```python
id
```
```python
=
```
```python
fetcher
```
```python
.
```
```python
getId();
        EngineKey key
```
```python
=
```
```python
keyFactory
```
```python
.
```
```python
buildKey(id, signature, width, height, loadProvider
```
```python
.
```
```python
getCacheDecoder(),
                loadProvider
```
```python
.
```
```python
getSourceDecoder(), transformation, loadProvider
```
```python
.
```
```python
getEncoder(),
                transcoder, loadProvider
```
```python
.
```
```python
getSourceEncoder());
```
```python
// 先尝试从内存缓存获取数据
```
```python
EngineResource
```
```python
<?>
```
```python
cached
```
```python
=
```
```python
loadFromCache(key, isMemoryCacheable);
```
```python
if
```
```python
(cached
```
```python
!=
```
```python
null
```
```python
) {
```
```python
// 获取数据成功，会回调target的onResourceReady()
```
```python
cb
```
```python
.
```
```python
onResourceReady(cached);
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
                logWithTimeAndKey(
```
```python
"Loaded resource from cache"
```
```python
, startTime, key);
            }
```
```python
return
```
```python
null
```
```python
;
        }
```
```python
// 在尝试从活动Resources map中获取，它表示的是当前正在使用的Resources
```
```python
// 它也是在内存中，与内存缓存不同之处是clear缓存时不会clear它。
```
```python
EngineResource
```
```python
<?>
```
```python
active
```
```python
=
```
```python
loadFromActiveResources(key, isMemoryCacheable);
```
```python
if
```
```python
(active
```
```python
!=
```
```python
null
```
```python
) {
            cb
```
```python
.
```
```python
onResourceReady(active);
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
                logWithTimeAndKey(
```
```python
"Loaded resource from active resources"
```
```python
, startTime, key);
            }
```
```python
return
```
```python
null
```
```python
;
        }
```
```python
// 然后看看当前jobs中是否包含这个任务了，如果包含说明任务之前已经提交了，正在执行
```
```python
EngineJob current
```
```python
=
```
```python
jobs
```
```python
.
```
```python
get(key);
```
```python
if
```
```python
(current
```
```python
!=
```
```python
null
```
```python
) {
            current
```
```python
.
```
```python
addCallback(cb);
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
                logWithTimeAndKey(
```
```python
"Added to existing load"
```
```python
, startTime, key);
            }
```
```python
return
```
```python
new
```
```python
LoadStatus(cb, current);
        }
```
```python
// 如果这些都没尝试成功，最后就只能自己提交任务了
```
```python
EngineJob engineJob
```
```python
=
```
```python
engineJobFactory
```
```python
.
```
```python
build(key, isMemoryCacheable);
        DecodeJob
```
```python
<
```
```python
T, Z, R
```
```python
>
```
```python
decodeJob
```
```python
=
```
```python
new
```
```python
DecodeJob
```
```python
<
```
```python
T, Z, R
```
```python
>
```
```python
(key, width, height, fetcher, loadProvider, transformation,
                transcoder, diskCacheProvider, diskCacheStrategy, priority);
```
```python
// runnable是关键，它是任务执行阶段的入口
```
```python
EngineRunnable runnable
```
```python
=
```
```python
new
```
```python
EngineRunnable(engineJob, decodeJob, priority);
        jobs
```
```python
.
```
```python
put(key, engineJob);
        engineJob
```
```python
.
```
```python
addCallback(cb);
```
```python
// 开始提交job
```
```python
engineJob
```
```python
.
```
```python
start(runnable);
```
```python
if
```
```python
(
```
```python
Log
```
```python
.
```
```python
isLoggable(
```
```python
TAG
```
```python
,
```
```python
Log
```
```python
.
```
```python
VERBOSE)) {
            logWithTimeAndKey(
```
```python
"Started new load"
```
```python
, startTime, key);
        }
```
```python
return
```
```python
new
```
```python
LoadStatus(cb, engineJob);
    }
```
3）engineJob.start(runnable)，提交任务到线程池
```python
class
```
```python
EngineJob
```
```python
implements
```
```python
EngineRunnable
```
```python
.
```
```python
EngineRunnableManager
```
```python
{
```
```python
public
```
```python
void
```
```python
start(EngineRunnable engineRunnable) {
```
```python
this
```
```python
.engineRunnable = engineRunnable;
```
```python
// 提交任务，diskCacheService默认是一个AbstractExecutorService
```
```python
future = diskCacheService.submit(engineRunnable);
    }
}
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
AbstractExecutorService
```
```python
implements
```
```python
ExecutorService
```
```python
{
```
```python
public
```
```python
Future<?> submit(Runnable task) {
```
```python
if
```
```python
(task ==
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
NullPointerException();
        RunnableFuture<Void> ftask = newTaskFor(task,
```
```python
null
```
```python
);
```
```python
// 线程池中执行子线程的任务，子线程得到调用后任务就可以执行了
```
```python
execute(ftask);
```
```python
return
```
```python
ftask;
    }
}
```
## 3. 任务执行阶段源码分析
1）EngineRunnable的run()，它是任务执行的入口
```python
class
```
```python
EngineRunnable
```
```python
implements
```
```python
Runnable
```
```python
,
```
```python
Prioritized
```
```python
{
```
```python
public
```
```python
void run() {
```
```python
if
```
```python
(isCancelled) {
```
```python
return
```
```python
;
        }
```
```python
Exception
```
```python
exception
```
```python
=
```
```python
null
```
```python
;
        Resource
```
```python
<?
```
```python
> resource =
```
```python
null
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
// 使用DecodeJob来完成数据的获取，编解码等
```
```python
resource = decode();
        }
```
```python
catch
```
```python
(
```
```python
Exception
```
```python
e) {
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
                Log.v(TAG,
```
```python
"Exception decoding"
```
```python
, e);
            }
```
```python
exception
```
```python
= e;
        }
```
```python
if
```
```python
(isCancelled) {
```
```python
if
```
```python
(resource !=
```
```python
null
```
```python
) {
```
```python
// 取消则回收各种资源防止内存泄露，此处的子类一般采用对象池方式来回收，防止反复的创建和回收。如BitmapPool
```
```python
resource.recycle();
            }
```
```python
return
```
```python
;
        }
```
```python
if
```
```python
(resource ==
```
```python
null
```
```python
) {
```
```python
// 任务执行最终失败则回调onLoadFailed，可以从此处分析出target callback的最终回调时机
```
```python
onLoadFailed(
```
```python
exception
```
```python
);
        }
```
```python
else
```
```python
{
```
```python
// 任务执行最终成功则回调onLoadComplete
```
```python
onLoadComplete(resource);
        }
    }
```
```python
private
```
```python
Resource
```
```python
<?
```
```python
> decode() throws
```
```python
Exception
```
```python
{
```
```python
if
```
```python
(isDecodingFromCache()) {
```
```python
// 从DiskLruCache中获取数据并解码，比较简单，读者可自行分析
```
```python
return
```
```python
decodeFromCache();
        }
```
```python
else
```
```python
{
```
```python
// 从其他途径获取数据并解码，如网络，本地File，数据流等
```
```python
return
```
```python
decodeFromSource();
        }
    }
```
```python
private
```
```python
Resource
```
```python
<?
```
```python
> decodeFromSource() throws
```
```python
Exception
```
```python
{
```
```python
// 调用decodeJob来完成数据获取和编解码
```
```python
return
```
```python
decodeJob.decodeFromSource();
    }  
}
```
2）decodeJob.decodeFromSource()
```python
class DecodeJob<A, T, Z> {
```
```python
public
```
```python
Resource<Z>
```
```python
decodeFromSource
```
```python
()
```
```python
throws
```
```python
Exception {
```
```python
// 获取数据，解码
```
```python
Resource<T> decoded = decodeSource();
```
```python
// 编码并保存到DiskLruCache中
```
```python
return
```
```python
transformEncodeAndTranscode(decoded);
    }
```
```python
private
```
```python
Resource<T>
```
```python
decodeSource
```
```python
()
```
```python
throws
```
```python
Exception {
        Resource<T> decoded =
```
```python
null
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
long
```
```python
startTime = LogTime.getLogTime();
```
```python
// 利用不同的DataFetcher来获取数据，如网络，本地File，流文件等
```
```python
final
```
```python
A data = fetcher.loadData(priority);
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
                logWithTimeAndKey(
```
```python
"Fetched data"
```
```python
, startTime);
            }
```
```python
if
```
```python
(isCancelled) {
```
```python
return
```
```python
null
```
```python
;
            }
```
```python
// 解码为所需的格式
```
```python
decoded = decodeFromSourceData(data);
        }
```
```python
finally
```
```python
{
            fetcher.cleanup();
        }
```
```python
return
```
```python
decoded;
    }
```
```python
private
```
```python
Resource<Z>
```
```python
transformEncodeAndTranscode
```
```python
(Resource<T> decoded) {
```
```python
long
```
```python
startTime = LogTime.getLogTime();
```
```python
// 根据ImageView的scaleType等参数计算真正被ImageView使用的图片宽高，并保存真正宽高的图片。
```
```python
// 比如centerCrop并且图片超出被ImageView裁剪时，我们没必要保存原图的宽高，而应该是裁剪之后的宽高，这样节省存储空间。
```
```python
// 这也是Glide相对于Picasso的一个很大的优势
```
```python
Resource<T> transformed = transform(decoded);
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
            logWithTimeAndKey(
```
```python
"Transformed resource from source"
```
```python
, startTime);
        }
```
```python
// 写入到DiskLruCache中，下次就可以直接从它里面拿了
```
```python
writeTransformedToCache(transformed);
        startTime = LogTime.getLogTime();
```
```python
// 转码，将源图片转码为ImageView所需的图片格式
```
```python
Resource<Z> result = transcode(transformed);
```
```python
if
```
```python
(Log.isLoggable(TAG, Log.VERBOSE)) {
            logWithTimeAndKey(
```
```python
"Transcoded transformed from source"
```
```python
, startTime);
        }
```
```python
return
```
```python
result;
    }
}
```
3）DataFetcher的loadData(priority)， DataFetcher的子类很多，参见概述和核心类部分。此处我们分析下从url获取的情形，这是我们碰到最多的情形。从url获取的DataFetcher是HttpUrlFetcher，它采用了android原生的HttpURLConnection网络库，如下
```python
public
```
```python
class
```
```python
HttpUrlFetcher
```
```python
implements
```
```python
DataFetcher
```
```python
<
```
```python
InputStream
```
```python
> {
```
```python
// 采用HttpURLConnection作为网络库，
```
```python
// 我们可以自定义DataFetcher，从而使用其他网络库，如OkHttp
```
```python
private
```
```python
HttpURLConnection urlConnection;
```
```python
public
```
```python
InputStream loadData(Priority priority) throws Exception {
```
```python
return
```
```python
loadDataWithRedirects(glideUrl.toURL(),
```
```python
0
```
```python
/*redirects*/
```
```python
,
```
```python
null
```
```python
/*lastUrl*/
```
```python
, glideUrl.getHeaders());
    }
```
```python
private
```
```python
InputStream loadDataWithRedirects(URL url,
```
```python
int
```
```python
redirects, URL lastUrl, Map<String, String> headers)
            throws IOException {
```
```python
if
```
```python
(redirects >= MAXIMUM_REDIRECTS) {
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
"Too many (> "
```
```python
+ MAXIMUM_REDIRECTS +
```
```python
") redirects!"
```
```python
);
        }
```
```python
else
```
```python
{
```
```python
// Comparing the URLs using .equals performs additional network I/O and is generally broken.
```
```python
// See http://michaelscharf.blogspot.com/2006/11/javaneturlequals-and-hashcode-make.html.
```
```python
try
```
```python
{
```
```python
if
```
```python
(lastUrl !=
```
```python
null
```
```python
&& url.toURI().equals(lastUrl.toURI())) {
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
"In re-direct loop"
```
```python
);
                }
            }
```
```python
catch
```
```python
(URISyntaxException e) {
```
```python
// Do nothing, this is best effort.
```
```python
}
        }
```
```python
// 静态工厂模式创建HttpURLConnection对象
```
```python
urlConnection = connectionFactory.build(url);
```
```python
for
```
```python
(Map.Entry<String, String> headerEntry : headers.entrySet()) {
          urlConnection.addRequestProperty(headerEntry.getKey(), headerEntry.getValue());
        }
```
```python
// Do our best to avoid gzip since it's both inefficient for images and also makes it more
```
```python
// difficult for us to detect and prevent partial content rendering. See #440.
```
```python
if
```
```python
(TextUtils.isEmpty(urlConnection.getRequestProperty(ENCODING_HEADER))) {
            urlConnection.setRequestProperty(ENCODING_HEADER, DEFAULT_ENCODING);
        }
```
```python
// 设置HttpURLConnection的参数
```
```python
urlConnection.setConnectTimeout(
```
```python
2500
```
```python
);
        urlConnection.setReadTimeout(
```
```python
2500
```
```python
);
        urlConnection.setUseCaches(
```
```python
false
```
```python
);
        urlConnection.setDoInput(
```
```python
true
```
```python
);
```
```python
// Connect explicitly to avoid errors in decoders if connection fails.
```
```python
urlConnection.connect();
```
```python
if
```
```python
(isCancelled) {
```
```python
return
```
```python
null
```
```python
;
        }
```
```python
final
```
```python
int
```
```python
statusCode = urlConnection.getResponseCode();
```
```python
if
```
```python
(statusCode /
```
```python
100
```
```python
==
```
```python
2
```
```python
) {
```
```python
return
```
```python
getStreamForSuccessfulRequest(urlConnection);
        }
```
```python
else
```
```python
if
```
```python
(statusCode /
```
```python
100
```
```python
==
```
```python
3
```
```python
) {
            String redirectUrlString = urlConnection.getHeaderField(
```
```python
"Location"
```
```python
);
```
```python
if
```
```python
(TextUtils.isEmpty(redirectUrlString)) {
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
"Received empty or null redirect url"
```
```python
);
            }
            URL redirectUrl =
```
```python
new
```
```python
URL(url, redirectUrlString);
```
```python
return
```
```python
loadDataWithRedirects(redirectUrl, redirects +
```
```python
1
```
```python
, url, headers);
        }
```
```python
else
```
```python
{
```
```python
if
```
```python
(statusCode == -
```
```python
1
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
IOException(
```
```python
"Unable to retrieve response code from HttpUrlConnection."
```
```python
);
            }
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
"Request failed "
```
```python
+ statusCode +
```
```python
": "
```
```python
+ urlConnection.getResponseMessage());
        }
    }
}
```
4）ResourceDecoder的decode()，ResourceDecoder的子类也很多，同样参见概述和核心类部分。我们分析下FileDescriptorBitmapDecoder，它是decode bitmap的关键所在
```python
public
```
```python
class
```
```python
FileDescriptorBitmapDecoder
```
```python
implements
```
```python
ResourceDecoder
```
```python
<
```
```python
ParcelFileDescriptor
```
```python
,
```
```python
Bitmap
```
```python
> {
```
```python
public
```
```python
Resource<Bitmap> decode(ParcelFileDescriptor source,
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
height) throws IOException {
        Bitmap bitmap = bitmapDecoder.decode(source, bitmapPool, width, height, decodeFormat);
```
```python
// 对象池管理方式，从bitmapPool中获取一个BitmapResource对象
```
```python
return
```
```python
BitmapResource.obtain(bitmap, bitmapPool);
    }
```
```python
public
```
```python
Bitmap decode(ParcelFileDescriptor resource, BitmapPool bitmapPool,
```
```python
int
```
```python
outWidth,
```
```python
int
```
```python
outHeight,
            DecodeFormat decodeFormat)
            throws IOException {
```
```python
// 通过MediaMetadataRetriever来解码
```
```python
MediaMetadataRetriever mediaMetadataRetriever = factory.build();
        mediaMetadataRetriever.setDataSource(resource.getFileDescriptor());
        Bitmap result;
```
```python
if
```
```python
(frame >=
```
```python
0
```
```python
) {
          result = mediaMetadataRetriever.getFrameAtTime(frame);
        }
```
```python
else
```
```python
{
          result = mediaMetadataRetriever.getFrameAtTime();
        }
        mediaMetadataRetriever.release();
        resource.close();
```
```python
return
```
```python
result;
    }
}
```
5）Transformation的transform(), 根据ImageView的实际宽高来裁剪图片数据。这样可以减小保存到DiskLruCache中的数据大小。它的子类也很多，我们分析下BitmapTransformation。
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
BitmapTransformation
```
```python
implements
```
```python
Transformation
```
```python
<
```
```python
Bitmap
```
```python
> {
```
```python
public
```
```python
final
```
```python
Resource<Bitmap> transform(Resource<Bitmap> resource,
```
```python
int
```
```python
outWidth,
```
```python
int
```
```python
outHeight) {
```
```python
if
```
```python
(!Util.isValidDimensions(outWidth, outHeight)) {
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
"Cannot apply transformation on width: "
```
```python
+ outWidth +
```
```python
" or height: "
```
```python
+ outHeight +
```
```python
" less than or equal to zero and not Target.SIZE_ORIGINAL"
```
```python
);
        }
        Bitmap toTransform = resource.get();
```
```python
int
```
```python
targetWidth = outWidth == Target.SIZE_ORIGINAL ? toTransform.getWidth() : outWidth;
```
```python
int
```
```python
targetHeight = outHeight == Target.SIZE_ORIGINAL ? toTransform.getHeight() : outHeight;
```
```python
// 裁剪，根据CenterCrop和fitCenter有两种裁剪方式
```
```python
Bitmap transformed = transform(bitmapPool, toTransform, targetWidth, targetHeight);
```
```python
final
```
```python
Resource<Bitmap> result;
```
```python
if
```
```python
(toTransform.equals(transformed)) {
            result = resource;
        }
```
```python
else
```
```python
{
```
```python
// 从bitmapPool对象池中获取BitmapResource
```
```python
result = BitmapResource.obtain(transformed, bitmapPool);
        }
```
```python
return
```
```python
result;
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
CenterCrop
```
```python
extends
```
```python
BitmapTransformation
```
```python
{
```
```python
protected
```
```python
Bitmap transform(BitmapPool pool, Bitmap toTransform,
```
```python
int
```
```python
outWidth,
```
```python
int
```
```python
outHeight) {
```
```python
final
```
```python
Bitmap toReuse = pool.get(outWidth, outHeight, toTransform.getConfig() !=
```
```python
null
```
```python
? toTransform.getConfig() : Bitmap.Config.ARGB_8888);
```
```python
// 按centerCrop方式裁剪
```
```python
Bitmap transformed = TransformationUtils.centerCrop(toReuse, toTransform, outWidth, outHeight);
```
```python
if
```
```python
(toReuse !=
```
```python
null
```
```python
&& toReuse != transformed && !pool.put(toReuse)) {
            toReuse.recycle();
        }
```
```python
return
```
```python
transformed;
    }
}
```
6）ResourceTranscoder的transcode(), 编码。有兴趣的读者可以自行分析
## 4 总结
Glide的Engine部分分为数据获取任务提交阶段和任务执行阶段。提交阶段先从内存缓存和当前活动Resources中获取，然后再在线程池中新开子线程，之后就只需要等待子线程得到执行了。任务执行阶段先尝试从DiskLruCache中获取Resources，然后利用DataFetcher获取数据，利用ResourceDecoder解码，利用Transformation裁剪图片数据，利用ResourceTranscoder转码为ImageView所需格式，这样就获取到了最终所需的Resources。
这一篇也是Glide源码解析的最终篇，感谢大家能看完我写的这些。不正确的地方，还希望指出来。谢谢！

