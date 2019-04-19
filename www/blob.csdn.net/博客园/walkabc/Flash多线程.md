# Flash多线程 - walkabc - 博客园
# [Flash多线程](https://www.cnblogs.com/hitfire/articles/3175840.html)
Flash平台从11.4版本就开始支持多线程编程，只不过这个使用起来有点坑爹。
每个 worker 都是基于独立的 swf 应用程序创建的并且作为独立的 swf 应用程序执行。要创建 Worker 实例，您需要获取 SWF 文件的字节以作为 ByteArray 实例，并将该实例传递给此方法。下面就详细的介绍一下三种方式使用Flash的多线程技术。
针对此目的，有三种常用方式可用来访问 swf 的字节：
```
// Embed the SWF file
    [Embed(source="../swfs/BgWorker.swf", mimeType="application/octet-stream")]
    private static var BgWorker_ByteClass:Class;
    
    private function createWorker():void
    {
      // create the background worker
      var workerBytes:ByteArray = new BgWorker_ByteClass();
      var bgWorker:Worker = WorkerDomain.current.createWorker(workerBytes);
      
      // listen for worker state changes to know when the worker is running
      bgWorker.addEventListener(Event.WORKER_STATE, workerStateHandler);
      
      // set up communication between workers using 
      // setSharedProperty(), createMessageChannel(), etc.
      // ... (not shown)
      
      bgWorker.start();
    }
```
使用 URLLoader 加载外部 SWF 文件：
```
// load the SWF file
    var workerLoader:URLLoader = new URLLoader();
    workerLoader.dataFormat = URLLoaderDataFormat.BINARY;
    workerLoader.addEventListener(Event.COMPLETE, loadComplete);
    workerLoader.load(new URLRequest("BgWorker.swf"));
    
    private function loadComplete(event:Event):void
    {
      // create the background worker
      var workerBytes:ByteArray = event.target.data as ByteArray;
      var bgWorker:Worker = WorkerDomain.current.createWorker(workerBytes);
      
      // listen for worker state changes to know when the worker is running
      bgWorker.addEventListener(Event.WORKER_STATE, workerStateHandler);
      
      // set up communication between workers using 
      // setSharedProperty(), createMessageChannel(), etc.
      // ... (not shown)
      
      bgWorker.start();
    }
```
将单个 swf 同时用作原始 worker 和后台 worker：
```
// The primordial worker's main class constructor
    public function PrimordialWorkerClass()
    {
      init();
    }
    
    private function init():void
    {
      var swfBytes:ByteArray = this.loaderInfo.bytes;
      
      // Check to see if this is the primordial worker
      if (Worker.current.isPrimordial)    
      {
        // create a background worker
        var bgWorker:Worker = WorkerDomain.current.createWorker(swfBytes);
        
        // listen for worker state changes to know when the worker is running
        bgWorker.addEventListener(Event.WORKER_STATE, workerStateHandler);
        
        // set up communication between workers using 
        // setSharedProperty(), createMessageChannel(), etc.
        // ... (not shown)
        
        bgWorker.start();
      }
      else // entry point for the background worker
      {
        // set up communication between workers using getSharedProperty()
        // ... (not shown)
        
        // start the background work
      }
```
使用 `createWorker()` 创建一个 Worker 对象不会开始执行该 worker。要开始 worker 的代码执行，请调用 Worker 对象的 `start()` 方法。
Worker 非常有用，因为它们降低了帧速率由于主渲染线程被其他代码阻止而下降的几率。不过，worker 需要使用额外的系统内存和 CPU，这可能会影响总体应用程序性能。因为每个 worker 都使用自己的运行时虚拟机实例，甚至一个微小的 worker 的开销也可能很大。当使用 worker 时，请在您的所有目标平台上测试您的代码以确保对系统的需求不是太大。在典型情况下，Adobe 建议您不要使用超过一个或两个后台 worker。

