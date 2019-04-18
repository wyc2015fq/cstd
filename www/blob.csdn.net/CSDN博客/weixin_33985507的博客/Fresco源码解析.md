# Fresco源码解析 - weixin_33985507的博客 - CSDN博客
2018年11月18日 17:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
- View 对应DraweeView类（实际上是DraweeHolder），其负责展示数据，显示图片。
- Model对应DraweeHierarchy类，其负责持有数据，用一个层级组织和维护最终绘制和显示的图片。
- 对应DraweeController类，其负责控制数据的逻辑。
#### SimpleDraweeView
先看下简单的继承关系图
```
Object (java.lang)
    View (android.view)
        ImageView (android.widget)
            DraweeView (com.facebook.drawee.view)  根部DraweeView
                GenericDraweeView (com.facebook.drawee.view)  通用的DraweeView
                    SimpleDraweeView (com.facebook.drawee.view) 最简单的DraweeView
```
在这个类中主要做了下面几件事情
- 通过uri设置一张图片
- 在内部构建和设置控制器
来看下最常用的方法 `setImageURI`
```
public void setImageURI(Uri uri, @Nullable Object callerContext) {
    DraweeController controller =
        mControllerBuilder
            .setCallerContext(callerContext)
            .setUri(uri)
            .setOldController(getController())
            .build();
    setController(controller);
  }
```
追踪一下 `mControllerBuilder` 的赋值
```
private void init(Context context, @Nullable AttributeSet attrs) {
      ...
      mControllerBuilder = sDraweecontrollerbuildersupplier.get();
      ...
  }
```
在看下 `sDraweecontrollerbuildersupplier` 的赋值
```
public static void initialize(
      Supplier<? extends AbstractDraweeControllerBuilder> draweeControllerBuilderSupplier) {
    sDraweecontrollerbuildersupplier = draweeControllerBuilderSupplier;
  }
```
看下 `initialize` 是在哪里调用的
```
Fresco.java
  private static void initializeDrawee(Context context, @Nullable DraweeConfig draweeConfig) {
    ...
    sDraweeControllerBuilderSupplier =
        new PipelineDraweeControllerBuilderSupplier(context, draweeConfig);
    SimpleDraweeView.initialize(sDraweeControllerBuilderSupplier);
    ...
  }
public static void initialize(Context context, @Nullable ImagePipelineConfig imagePipelineConfig,  
    @Nullable DraweeConfig draweeConfig) {
    ...
    initializeDrawee(context, draweeConfig);
    ...
  }
```
可知`sDraweecontrollerbuildersupplier`的类型是：`PipelineDraweeControllerBuilderSupplier`, 再来看下 `PipelineDraweeControllerBuilderSupplier`里面的`get`方法
```
public PipelineDraweeControllerBuilder get() {
    return new PipelineDraweeControllerBuilder(
        mContext,
        mPipelineDraweeControllerFactory,
        mImagePipeline,
        mBoundControllerListeners);
  }
```
所以 可以得出下面的结论:
- `sDraweeControllerBuilderSupplier` 是 `PipelineDraweeControllerBuilderSupplier`
- `mControllerBuilder` 是 `PipelineDraweeControllerBuilder`
- `controller` 是 `PipelineDraweeController`
#### GenericDraweeView
在这里类里面主要做了下面几件事情
- 解析XML属性，根据属性值构建 `GenericDraweeHierarchy`
- 将`Hierarchy`设置给`DraweeView`体系
```
protected void inflateHierarchy(Context context, @Nullable AttributeSet attrs) {
    ...
    // 解析xml里面的属性值
    GenericDraweeHierarchyBuilder builder =
        GenericDraweeHierarchyInflater.inflateBuilder(context, attrs);
    setAspectRatio(builder.getDesiredAspectRatio());
    setHierarchy(builder.build());
    ...
  }
```
这里面包含了初始化的操作，有一点可以看到，`Hierarchy` 是在构造方法里面初始化的，所以在使用`SimpleDraweeView`的getHierarchy 方法是，可以不用判断是否为空
可以得出结论
- `Hierarchy` 是 `GenericDraweeHierarchy`
### DraweeView
在这里类里面主要做了下面几件事情
- 该控件用于展示从DraweeHierarchy获取的图像。
- 使用此控件之前，应该先为其设置DraweeHierarchy。推荐在创建的时候只做一次。
- 为了展示一张图片，DraweeController必须被设置。
- Note：虽然这个控件是ImageView的直接子类，但是不支持ImageView的众多方法。！
这个类里面有个关键的类 `DraweeHolder`, 可以说 `DraweeView` 类中所做的所有操作，都是直接或者间接和`DraweeHolder`有关系。`DraweeView` 从其体系中获取到的 `DraweeHierarchy` 和 `DraweeController` 的信息都被其转交到了 `DraweeHolder` 类中。
```
public void setHierarchy(DH hierarchy) {
    mDraweeHolder.setHierarchy(hierarchy);
    super.setImageDrawable(mDraweeHolder.getTopLevelDrawable());
  }
  public void setController(@Nullable DraweeController draweeController) {
    mDraweeHolder.setController(draweeController);
    super.setImageDrawable(mDraweeHolder.getTopLevelDrawable());
  }
```
再看一下这个类里面的两个重要方法
```
protected void onAttach() {
    doAttach();
  }
  protected void onDetach() {
    doDetach();
  }
```
继续跟踪 `doAttach` 方法
```
protected void doAttach() {
    mDraweeHolder.onAttach();
  }
```
```
DraweeHolder.java
  public void onAttach() {
    mIsHolderAttached = true;
    attachOrDetachController();
  }
  private void attachOrDetachController() {
    if (mIsHolderAttached && mIsVisible) {
      attachController();
    } else {
      detachController();
    }
  }
  private void attachController() {
    ...
    mController.onAttach();
    ...
  }
```
最终调用到了 mController.onAttach(); 方法，下面介绍 `DraweeController`
#### DraweeController
看下继承关系图
```
DraweeController (com.facebook.drawee.interfaces)
    AbstractDraweeController (com.facebook.drawee.controller)
        VolleyDraweeController (com.facebook.drawee.backends.volley)
        PipelineDraweeController (com.facebook.drawee.backends.pipeline)
```
看注释方法：
- DraweeController会被DraweeView所使用。
- DraweeView会转发事件给DraweeController，而DraweeController会基于事件控制DraweeHierarchy顶层视图的切换。
再看下它的一个子类 `AbstractDraweeController`
#### AbstractDraweeController
看注释方法：
- 实现了一些通用的功能，并不涉及具体是怎样抓取图片的。
- 所有的方法都应该在UI线程调用。
来看一下我们关心的方法 `onAttach()`
```
@Override
  public void onAttach() {
    ...
    mIsAttached = true;
    if (!mIsRequestSubmitted) {
      submitRequest();
    }
    ...
  }
  protected void submitRequest() {
    ...
    // 没有缓存的情况
    mSettableDraweeHierarchy.setProgress(0, true); // 设置加载进度为0
    ...
    mDataSource = getDataSource(); // 获取数据源
    final String id = mId;
    final boolean wasImmediate = mDataSource.hasResult();
    // 创建数据源观察者
    final DataSubscriber<T> dataSubscriber =
        new BaseDataSubscriber<T>() {
          @Override
          public void onNewResultImpl(DataSource<T> dataSource) {
            // isFinished must be obtained before image, otherwise we might set intermediate result
            // as final image.
            boolean isFinished = dataSource.isFinished();
            boolean hasMultipleResults = dataSource.hasMultipleResults();
            float progress = dataSource.getProgress();
            T image = dataSource.getResult();
            if (image != null) { // 有数据的情况
              // 调用 onNewResultInternal() 处理数据源获取的结果
              onNewResultInternal(
                  id, dataSource, image, progress, isFinished, wasImmediate, hasMultipleResults);
            } else if (isFinished) {
              onFailureInternal(id, dataSource, new NullPointerException(), /* isFinished */ true);
            }
          }
          @Override
          public void onFailureImpl(DataSource<T> dataSource) {
            onFailureInternal(id, dataSource, dataSource.getFailureCause(), /* isFinished */ true);
          }
          @Override
          public void onProgressUpdate(DataSource<T> dataSource) {
            boolean isFinished = dataSource.isFinished();
            float progress = dataSource.getProgress();
            onProgressUpdateInternal(id, dataSource, progress, isFinished);
          }
        };
    // 注册观察者
    mDataSource.subscribe(dataSubscriber, mUiThreadImmediateExecutor);
    ...
  }
```
再来看下 `onNewResultInternal()`
```
private void onNewResultInternal(String id, DataSource<T> dataSource,@Nullable T image,
      float progress, boolean isFinished, boolean wasImmediate, boolean deliverTempResult) {
    try {
      
      // ignore late callbacks (data source that returned the new result is not the one we expected)
      // 如果不是我们想要的数据，丢弃
      if (!isExpectedDataSource(id, dataSource)) {
        ...
        return;
      }
      ...
      // 创建Drawable
      Drawable drawable = createDrawable(image);
      ...
      try {
        // 设置当前显示的drawable
        if (isFinished) {
          ...
          mSettableDraweeHierarchy.setImage(drawable, 1f, wasImmediate);
          // 回调设置图像结束
          getControllerListener().onFinalImageSet(id, getImageInfo(image), getAnimatable());
        } else if (deliverTempResult) {
          mSettableDraweeHierarchy.setImage(drawable, 1f, wasImmediate);
          getControllerListener().onFinalImageSet(id, getImageInfo(image), getAnimatable());
        } else { // 还没有结束下载
          mSettableDraweeHierarchy.setImage(drawable, progress, wasImmediate);
          getControllerListener().onIntermediateImageSet(id, getImageInfo(image));
        }
      } finally {
        // 释放之前缓存的drawable对象和Image对象
        ...
      }
    } finally {
      ...
    }
  }
```
整体的调用过程 `onAttach()` --> `submitRequest()` --> `onNewResultInternal()`
�前面分析代码中，我们需要关注一下`getDataSource`方法，`PipelineDraweeController`就是我们真正用到的类
#### PipelineDraweeController
看注释方法：
- DraweeController是ImagePipeline和SettableDraweeHierarchy的桥梁。
- 实际显示的图像是数据源提供的，而数据源是在attach和detach时自动获取和关闭的。
前面分析可知 `submitRequest` 里面涉及到了`getDataSource()` 方法，下面看下源码：
```
@Override
  protected DataSource<CloseableReference<CloseableImage>> getDataSource() {
    ...
    DataSource<CloseableReference<CloseableImage>> result = mDataSourceSupplier.get();
    ...
    return result;
  }
```
`mDataSourceSupplier` 从哪里来的呢？继续在这个类里面找
```
private void init(Supplier<DataSource<CloseableReference<CloseableImage>>> dataSourceSupplier) {
    mDataSourceSupplier = dataSourceSupplier;
  }
  public void initialize( Supplier<DataSource<CloseableReference<CloseableImage>>> dataSourceSupplier, String id,
      CacheKey cacheKey, Object callerContext, @Nullable ImmutableList<DrawableFactory> customDrawableFactories,
      @Nullable ImageOriginListener imageOriginListener) {
    ...
    init(dataSourceSupplier);
    ...
  }
```
`initialize()` 方法是哪里调用的呢？所有的方法都是在 `Builder` 里面处理的
```
PipelineDraweeControllerBuilder.java
  protected PipelineDraweeController obtainController() {
    try {
      ...
      controller.initialize(
          obtainDataSourceSupplier(controller, controllerId),
          controllerId,
          getCacheKey(),
          getCallerContext(),
          mCustomDrawableFactories,
          mImageOriginListener);
      controller.initializePerformanceMonitoring(mImagePerfDataListener);
      return controller;
    } finally {
      ...
    }
  }
  // 父类 AbstractDraweeControllerBuilder.java
  protected AbstractDraweeController buildController() {
    ...
    AbstractDraweeController controller = obtainController();
    controller.setRetainImageOnFailure(getRetainImageOnFailure());
    controller.setContentDescription(getContentDescription());
    controller.setControllerViewportVisibilityListener(getControllerViewportVisibilityListener());
    ...
    return controller;
  }
  @Override
  public AbstractDraweeController build() {
    ...
    return buildController();
  }
```
离真相越来越近了,前面提到了 `PipelineDraweeControllerBuilder` 就是在分析 `SimpleDraweeView` 的时候，里面的变量 `mControllerBuilder`。
这里给 `mDataSourceSupplier` 赋值的流程大概出来了
`SimpleDraweeView.setImageURI()` --> `PipelineDraweeControllerBuilder.build()` --> `PipelineDraweeControllerBuilder.buildController()` --> `PipelineDraweeControllerBuilder.obtainController()` --> `PipelineDraweeController.initialize()` --> 赋值完成
在看一下 `mDataSourceSupplier` 取值方法 `obtainDataSourceSupplier(controller, controllerId)`
```
protected Supplier<DataSource<IMAGE>> obtainDataSourceSupplier(
      final DraweeController controller, final String controllerId) {
    ...
    // final image supplier;
    if (mImageRequest != null) {
      supplier = getDataSourceSupplierForRequest(controller, controllerId, mImageRequest);
    }
    // increasing-quality supplier; highest-quality supplier goes first
    if (supplier != null && mLowResImageRequest != null) {
      List<Supplier<DataSource<IMAGE>>> suppliers = new ArrayList<>(2);
      suppliers.add(supplier);
      suppliers.add(getDataSourceSupplierForRequest(controller, controllerId, mLowResImageRequest));
      supplier = IncreasingQualityDataSourceSupplier.create(suppliers, false);
    }
    ...
    return supplier;
  }
```
重点分析一下 `getDataSourceSupplierForRequest` 吧
```
protected Supplier<DataSource<IMAGE>> getDataSourceSupplierForRequest(
      final DraweeController controller, String controllerId, REQUEST imageRequest) {
    return getDataSourceSupplierForRequest(
        controller, controllerId, imageRequest, CacheLevel.FULL_FETCH);
  }
  protected Supplier<DataSource<IMAGE>> getDataSourceSupplierForRequest(
      final DraweeController controller,
      final String controllerId,
      final REQUEST imageRequest,
      final CacheLevel cacheLevel) {
    final Object callerContext = getCallerContext();
    return new Supplier<DataSource<IMAGE>>() {
      @Override
      public DataSource<IMAGE> get() {
        // 这里就是 getDataSource() 时候真正调用的方法  
        return getDataSourceForRequest(
            controller, controllerId, imageRequest, callerContext, cacheLevel);
      }
      @Override
      public String toString() {
        ...
      }
    };
  }
```
`mDataSourceSupplier` 初始化的操作分析告一段落，我们接着前面的 `getDataSource()` 方法来继续分析：
```
@Override
  protected DataSource<CloseableReference<CloseableImage>> getDataSource() {
    ...
    DataSource<CloseableReference<CloseableImage>> result = mDataSourceSupplier.get();
    ...
    return result;
  }
```
这里的 `mDataSourceSupplier.get()` 真正调用的地方就是上面的`get`方法了
```
getDataSourceForRequest(
            controller, controllerId, imageRequest, callerContext, cacheLevel);
```
#### 请求产生和发出请求
上面的代码分析到 `getDataSourceForRequest()` 方法，再来回顾一下流程 `onAttach()` --> `submitRequest()` --> `getDataSource()` -->`mDataSourceSupplier.get()` --> `getDataSourceForRequest()`
```
// PipelineDraweeControllerBuilder.java
  protected DataSource<CloseableReference<CloseableImage>> getDataSourceForRequest(
      DraweeController controller,
      String controllerId,
      ImageRequest imageRequest,
      Object callerContext,
      AbstractDraweeControllerBuilder.CacheLevel cacheLevel) {
    return mImagePipeline.fetchDecodedImage(
        imageRequest,
        callerContext,
        convertCacheLevelToRequestLevel(cacheLevel),
        getRequestListener(controller));
  }
```
这里涉及到了fresco里面的一个重要的类`ImagePipeline`来看下官方文档里面有对它的介绍:
```
Image pipeline 负责完成加载图像，变成Android设备可呈现的形式所要做的每个事情。
pipelineSequence
大致流程如下:
1.检查内存缓存，如有，返回
2.后台线程开始后续工作
3.检查是否在未解码内存缓存中。如有，解码，变换，返回，然后缓存到内存缓存中。
4.检查是否在磁盘缓存中，如果有，变换，返回。缓存到未解码缓存和内存缓存中。
5.从网络或者本地加载。加载完成后，解码，变换，返回。存到各个缓存中。
```
首先看下 `mImagePipeline` 是从哪里来的?
看下 `SimpleDraweeView` 中 `mControllerBuilder` ( `PipelineDraweeControllerBuilder` ) 获取的方法：
```
public PipelineDraweeControllerBuilder get() {
    return new PipelineDraweeControllerBuilder(
        mContext,
        mPipelineDraweeControllerFactory,
        mImagePipeline,
        mBoundControllerListeners);
  }
```
这里的 `mImagePipeline` 是从哪里来的，在构造方法中
```
imagePipelineFactory = ImagePipelineFactory.getInstance()
mImagePipeline = imagePipelineFactory.getImagePipeline();
ImagePipelineFactory.java
  public ImagePipeline getImagePipeline() {
    if (mImagePipeline == null) {
      mImagePipeline =
          new ImagePipeline(
              getProducerSequenceFactory(),
              mConfig.getRequestListeners(),
              mConfig.getIsPrefetchEnabledSupplier(),
              getBitmapMemoryCache(),
              getEncodedMemoryCache(),
              getMainBufferedDiskCache(),
              getSmallImageBufferedDiskCache(),
              mConfig.getCacheKeyFactory(),
              mThreadHandoffProducerQueue,
              Suppliers.of(false),
              mConfig.getExperiments().isLazyDataSource());
    }
    return mImagePipeline;
  }
```
可以看到 `mImagePipeline` 的类型是 `ImagePipeline`,再来看下 `mImagePipeline.fetchDecodedImage`:
```
public DataSource<CloseableReference<CloseableImage>> fetchDecodedImage(
      ImageRequest imageRequest,
      Object callerContext,
      ImageRequest.RequestLevel lowestPermittedRequestLevelOnSubmit,
      @Nullable RequestListener requestListener) {
    try {
      // 获取用于图片解码的生产者序列
      Producer<CloseableReference<CloseableImage>> producerSequence =
          mProducerSequenceFactory.getDecodedImageProducerSequence(imageRequest);
      // 提交请求
      return submitFetchRequest(
          producerSequence,
          imageRequest,
          lowestPermittedRequestLevelOnSubmit,
          callerContext,
          requestListener);
    } catch (Exception exception) {
      return DataSources.immediateFailedDataSource(exception);
    }
  }
```
重点关注一下 `getDecodedImageProducerSequence` 方法,首先看一下这个方法的返回值：里面有一个`mInputProducer`变量，这个变量又关联了另外一个变量，这里的（每一个函数）都以 （上一个函数）产生的Producer作为参数进行传递
```
producerSequence = {BitmapMemoryCacheGetProducer@4747} 
 mInputProducer = {ThreadHandoffProducer@4749} 
  mInputProducer = {BitmapMemoryCacheKeyMultiplexProducer@4751} 
   mInputProducer = {BitmapMemoryCacheProducer@4753} 
    mInputProducer = {DecodeProducer@4756} 
     mInputProducer = {ResizeAndRotateProducer@4761} 
      mInputProducer = {AddImageTransformMetaDataProducer@4767} 
       mInputProducer = {EncodedCacheKeyMultiplexProducer@4771} 
        mInputProducer = {EncodedMemoryCacheProducer@4773} 
         mInputProducer = {DiskCacheReadProducer@4776} 
          mInputProducer = {DiskCacheWriteProducer@4778} 
           mInputProducer = {NetworkFetchProducer@4780}
```
在看一下 `submitFetchRequest` 这个方法
```
private <T> DataSource<CloseableReference<T>> submitFetchRequest(
      Producer<CloseableReference<T>> producerSequence,
      ImageRequest imageRequest,
      ImageRequest.RequestLevel lowestPermittedRequestLevelOnSubmit,
      Object callerContext,
      @Nullable RequestListener requestListener) {
    try {
      return CloseableProducerToDataSourceAdapter.create(
          producerSequence, settableProducerContext, finalRequestListener);
    }
    ...
  }
```
继续跟踪，主要看一下`AbstractProducerToDataSourceAdapter`这个类
```
protected AbstractProducerToDataSourceAdapter(
      Producer<T> producer,
      SettableProducerContext settableProducerContext,
      RequestListener requestListener) {
    ...
    producer.produceResults(createConsumer(), settableProducerContext);
    ...
  }
```
`producer` 以 `BitmapMemoryCacheGetProducer` 为例，我们看下 `produceResults` 里面的代码实现
```
@Override
  public void produceResults(
      final Consumer<CloseableReference<CloseableImage>> consumer,
      final ProducerContext producerContext) {
      ...
      mInputProducer.produceResults(wrappedConsumer, producerContext);
      ...
  }
```
这个 Producer 又会调用 `mInputProducer.produceResults` 即 `ThreadHandoffProducer` 里面的 `produceResults` ，到这里，应该就大概明白这个sequence Producer的作用了，所谓sequence Producer，其实就是一层层的Producer不断的嵌套连接起来，完成同一个任务，而每一个Producer都相互独立，完成各自任务；同时，Producer间产生的结果，也会相互传递，互为表里。
