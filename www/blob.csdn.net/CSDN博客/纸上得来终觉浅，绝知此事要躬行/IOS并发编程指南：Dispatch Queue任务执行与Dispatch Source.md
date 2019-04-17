# IOS并发编程指南：Dispatch Queue任务执行与Dispatch Source - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月04日 11:00:01[boonya](https://me.csdn.net/boonya)阅读数：1223








**导读：** 本文为读《Concurrency Programming Guide》笔记第三篇，在对OS X和iOS应用开发中实现任务异步执行的技术、注意事项、Operation与Dispatch Queues实践解析后，作者付宇轩（[@DevTalking](http://weibo.com/jacefu)）着重分享了让Dispatch Queue执行任务的那些事儿。当然，本着“Talk
 is cheap, show me the code”原则，除却讲解外，作者还分享了各个知识点具体操作实现的Swift代码片段。


## 系列阅读
- [iOS开发中设计并发任务技术与注意事项](http://geek.csdn.net/news/detail/60236)
- [iOS并发编程中Operation与Dispatch Queues实践](http://geek.csdn.net/news/detail/63001)
- [iOS并发编程指南：Dispatch Queue任务执行与Dispatch Source](http://geek.csdn.net/news/detail/69122)

## 通过Dispatch Queue执行任务

如果想让Dispatch Queue执行任务，首先就是得将任务放入队列中，我们可以异步的将任务加入队列，也可以同步的将任务加入队列，可以一个任务一个任务的加，也可以一组一组的加。这节我们就来看看将任务加入队列的那些事。

### 向队列添加任务

我们可以使用`dispatch_async`或者`dispatch_async_f`函数异步的向队列中添加任务，也就是说当我们添加完任务后该函数会立即返回，我们不需要等待任务执行完成，而且我们也不会知道队列到底何时开始执行任务。`dispatch_async`函数有两个参数，一个是目标队列，类型为`dispatch_queue_t`，另一个是闭包，类型为`dispatch_block_t`：

```
let serialQueue = dispatch_queue_create("com.example.MySerialQueue", nil)        
dispatch_async(serialQueue, {    
    print("Task in the queue...")
})
```

`dispatch_async_f`函数有三个参数，第一个是类型为`dispatch_queue_t`的目标队列，第二个是队列上下文指针，第三个是类型为`dispatch_function_t`的任务函数，队列上下文指针为该函数的唯一参数：

```
class AddTaskToQueue {
    func launch() {
        let serialQueue = dispatch_queue_create("com.example.MySerialQueue", nil)
        dispatch_async(serialQueue, {
            print("Task in the queue...")
        })
        dispatch_async_f(serialQueue, unsafeBitCast(0, UnsafeMutablePointer<Int>.self), taskFunction())
        sleep(3)  
    }
    func taskFunction() -> dispatch_function_t {
        return { context in 
            print("Do some work with context...")
        }
    }
}
let addTaskToQueue = AddTaskToQueue()
addTaskToQueue.launch()
```

除了这两个函数，我们还可以使用`dispatch_sync`和`dispatch_sync_f`函数同步的向队列中添加任务，并且我们要等待直到任务执行完成。这两个函数和上面的异步添加任务函数用法完全一致。

那么什么时候用异步什么时候用同步呢，大多数情况下我们都是在主线程中使用GCD分派任务，为了避免阻塞主线程，影响用户体验，所以通常情况下我们都使用异步添加任务的方式。当然为了避免任务与主线程中产生资源竞争的问题，有时候酌情也会使用同步添加任务的方式。

### Dispatch Queue的Completion Block

还记得`NSOperation`的`completionBlock`属性吗，这个回调函数在任务执行完成后调用，用于处理有些后续工作或者消息通知。在Dispatch Queue中并没有类似的属性，但是我们可以通过其他方式来实现。举一个很常见的应用场景，我们在主线程中分派一个下载图片的任务，让其在二级线程中执行，当图片下载完成后通知主线程，并由主线程将图片显示出来，我们看看简单的代码片段：

```
class DownloadImage {    
    func dispatchTaskInMainThread() {
 dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), downloadImage())       
    }
    func downloadImage() -> (() -> ()) { 
        return {
            print("Downloading image in \(NSThread.currentThread())")
            dispatch_async(dispatch_get_main_queue()) {
                print("Handle image and display in \(NSThread.currentThread())")
            }
        }
    }
}
```

我们来看看上面代码都做了些什么，首先在`dispatchTaskInMainThread`方法中，我们使用`dispatch_get_global_queue`函数获取到全局并发队列，然后将`downloadImage`下载图片的方法作为任务添加到该全局队列中。在`downloadImage`方法里，当图片下载完成后通过`dispatch_get_main_queue`函数获取到主队列，也就是在主线程中对图片进行处理，这样我们就达到了Completion
 Block的效果。

### 在队列中循环执行任务

在我们的日常开发中，经常会使用到for循环来处理一些任务，而且这些任务之间也并没有先后顺序的关联，每个任务相对比较独立。遇到这种情况，我们可以用`dispatch_apply`或`dispatch_apply_f`函数让任务在队列中循环执行，并且可以是并发执行，这样相比for循环的串行执行要更加效率：

```
// for循环
let arr = ["Swift", "Objective-C", "Java", "Delphi", "C++"]        
for element in arr {
    print("Handle element. the element is \(element)")
}

// dispatch_apply
dispatch_apply(arr.count, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)){ index in            
    print("Handle element. the element is \(arr[index]). Current thread is \(NSThread.currentThread())")          
}
```

从上面示例代码片段中可以看到，`dispatch_apply`函数有三个参数，第一个参数是循环次数，第二个参数是目标队列，第三个则是要执行的闭包任务，循环次数是该闭包的唯一参数。

### 暂停和重启队列

在Dispatch Queue执行任务时，如果我们想暂停队列，可以使用`dispatch_suspend`函数，重新让队列执行任务可以使用`dispatch_resume`。这里要注意的是暂停队列只是让队列暂时停止执行下一个任务，而不是中断当前正在执行的任务。

### Dispatch Group的使用

在实际开发中，为了提升性能我们或许会经常使用`dispatch_async`异步的将任务添加进队列去执行，但有些时候需要之前队列中的多个任务都执行完成之后，才能获取到正确的或者说想要的结果供后续逻辑代码使用，遇到这种情况，就可以使用Dispatch Group，将多个任务在队列中归为一个组，并可以使用`dispatch_group_wait`函数让之后的逻辑代码等待，直到该组的任务都执行完成后再执行。

```
var count = 0        
let concurrentQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
dispatch_async(concurrentQueue) {  
    print("Task1 in dispatchGroup...") 
    sleep(2)
    count += 1 
}

dispatch_async(concurrentQueue) {    
    print("Task2 in dispatchGroup...")
    sleep(3)
    count += 1 
}

dispatch_async(concurrentQueue) {
    print("Task3 in dispatchGroup...")
    sleep(1)
    count += 1
}

print("I expect the count is 3, and the factual count is \(count)")
```

上面的代码片段就是我刚才描述的场景，因为使用的是并发队列，也不好在每个任务里进行回调处理，所以我们永远不会得到正确的`count`。如果我们使用Dispatch Group事情就简单多了：

```
var count = 0
let concurrentQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
let dispatchGroup = dispatch_group_create()
dispatch_group_async(dispatchGroup, concurrentQueue) {
    print("Task1 in dispatchGroup...")
    sleep(2)
    count += 1
}

dispatch_group_async(dispatchGroup, concurrentQueue) {
    print("Task2 in dispatchGroup...")
    sleep(3)
    count += 1
}

dispatch_group_async(dispatchGroup, concurrentQueue) {
    print("Task3 in dispatchGroup...")
    sleep(1)
    count += 1
}

dispatch_group_wait(dispatchGroup, DISPATCH_TIME_FOREVER)
print("I expect the count is 3, and the factual count is \(count)")
```

上面的代码中，先使用`dispatch_group_create`函数创建Dispatch Group，然后使用`dispatch_group_async`函数将任务分进组里，然后再添加进队列中。该函数有三个参数，分别是Dispatch Group、Dispatch Queue和要执行任务的闭包。当添加完任务后使用`dispatch_group_wait`函数等待，直到指定组的任务全部完成，才会继续执行后面的打印语句，该函数有两个参数，第一个是目标组，第二个是等待时间`DISPATCH_TIME_NOW`或`DISPATCH_TIME_FOREVER`。

## Dispatch Source

前面的文章中介绍过Dispatch Source：

> 
Dispatch Source是GCD中的一个基本类型，从字面意思可称为调度源，它的作用是当有一些特定的较底层的系统事件发生时，调度源会捕捉到这些事件，然后可以做其他的逻辑处理，调度源有多种类型，分别监听对应类型的系统事件。我们来看看它都有哪些类型：

- Timer Dispatch Source：定时调度源。
- Signal Dispatch Source：监听UNIX信号调度源，比如监听代表挂起指令的SIGSTOP信号。
- Descriptor Dispatch Source：监听文件相关操作和Socket相关操作的调度源。
- Process Dispatch Source：监听进程相关状态的调度源。
- Mach port Dispatch Source：监听Mach相关事件的调度源。
- Custom Dispatch Source：监听自定义事件的调度源。

这一节就来看看如何使用Dispatch Source。

用通俗一点的话说就是用GCD的函数指定一个希望监听的系统事件类型，再指定一个捕获到事件后进行逻辑处理的闭包或者函数作为回调函数，然后再指定一个该回调函数执行的Dispatch Queue即可，当监听到指定的系统事件发生时会调用回调函数，将该回调函数作为一个任务放入指定的队列中执行。也就是说当监听到系统事件后就会触发一个任务，并自动将其加入队列执行，这里与之前手动添加任务的模式不同，一旦将Diaptach Source与Dispatch Queue关联后，只要监听到系统事件，Dispatch Source就会自动将任务（回调函数）添加到关联的队列中。

有些时候回调函数执行的时间较长，在这段时间内Dispatch Source又监听到多个系统事件，理论上就会形成事件积压，但好在Dispatch Source有很好的机制解决这个问题，当有多个事件积压时会根据事件类型，将它们进行关联和结合，形成一个新的事件。

### 监听事件类型

Dispatch Source一共可以监听六类事件，分为11个类型，我们来看看都是什么：
- `DISPATCH_SOURCE_TYPE_DATA_ADD`：属于自定义事件，可以通过`dispatch_source_get_data`函数获取事件变量数据，在我们自定义的方法中可以调用`dispatch_source_merge_data`函数向Dispatch Source设置数据，下文中会有详细的演示。
- `DISPATCH_SOURCE_TYPE_DATA_OR`：属于自定义事件，用法同上面的类型一样。
- `DISPATCH_SOURCE_TYPE_MACH_SEND`：Mach端口发送事件。
- `DISPATCH_SOURCE_TYPE_MACH_RECV`：Mach端口接收事件。
- `DISPATCH_SOURCE_TYPE_PROC`：与进程相关的事件。
- `DISPATCH_SOURCE_TYPE_READ`：读文件事件。
- `DISPATCH_SOURCE_TYPE_WRITE`：写文件事件。
- `DISPATCH_SOURCE_TYPE_VNODE`：文件属性更改事件。
- `DISPATCH_SOURCE_TYPE_SIGNAL`：接收信号事件。
- `DISPATCH_SOURCE_TYPE_TIMER`：定时器事件。
- `DISPATCH_SOURCE_TYPE_MEMORYPRESSURE`：内存压力事件。

### 创建Dispatch Source

我们可以使用`dispatch_source_create`函数创建Dispatch Source，该函数有四个参数：
- `type`：第一个参数用于标识Dispatch Source要监听的事件类型，共有11个类型。
- `handle`：第二个参数是取决于要监听的事件类型，比如如果是监听Mach端口相关的事件，那么该参数就是`mach_port_t`类型的Mach端口号，如果是监听事件变量数据类型的事件那么该参数就不需要，设置为0就可以了。
- `mask`：第三个参数同样取决于要监听的事件类型，比如如果是监听文件属性更改的事件，那么该参数就标识文件的哪个属性，比如`DISPATCH_VNODE_RENAME`。
- `queue`：第四个参数设置回调函数所在的队列。

```bash
let dispatchQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)        
let dispatchSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatchQueue)
```

上面的代码就是创建Dispatch Source的简单示例。

### 设置事件处理器

前文中提到过，当Dispatch Source监听到事件时会调用指定的回调函数或闭包，该回调函数或闭包就是Dispatch Source的事件处理器。我们可以使用`dispatch_source_set_event_handler`或`dispatch_source_set_event_handler_f`函数给创建好的Dispatch Source设置处理器，前者是设置闭包形式的处理器，后者是设置函数形式的处理器：

```
dispatch_source_set_event_handler(dispatchSource, {            
    print("Dispatch Source 事件处理器...")
})
// 根据闭包尾随的特性，还可以有下面的写法
dispatch_source_set_event_handler(dispatchSource) {      
    print("Dispatch Source 事件处理器...")      
}
```

从上面示例代码中可以看到，该函数有两个参数，第一个是设置目标Dispatch Source，第二个参数就是设置处理器了。

既然是事件处理器，那么肯定需要获取一些Dispatch Source的信息，GCD提供了三个在处理器中获取Dispatch Source相关信息的函数，比如`handle`、`mask`。而且针对不同类型的Dispatch Source，这三个函数返回数据的值和类型都会不一样，下面来看看这三个函数：
- `dispatch_source_get_handle`：这个函数用于获取在创建Dispatch Source时设置的第二个参数`handle`。

- 如果是读写文件的Dispatch Source，返回的就是描述符。
- 如果是信号类型的Dispatch Source，返回的是`int`类型的信号数。
- 如果是进程类型的Dispatch Source，返回的是`pid_t`类型的进程id。
- 如果是Mach端口类型的Dispatch Source，返回的是`mach_port_t`类型的Mach端口。

- `dispatch_source_get_data`：该函数用于获取Dispatch Source监听到事件的相关数据。 
- 如果是读文件类型的Dispatch Source，返回的是读到文件内容的字节数。
- 如果是写文件类型的Dispatch Source，返回的是文件是否可写的标识符，正数表示可写，负数表示不可写。
- 如果是监听文件属性更改类型的Dispatch Source，返回的是监听到的有更改的文件属性，用常量表示，比如`DISPATCH_VNODE_RENAME`等。
- 如果是进程类型的Dispatch Source，返回监听到的进程状态，用常量表示，比如`DISPATCH_PROC_EXIT`等。
- 如果是Mach端口类型的Dispatch Source，返回Mach端口的状态，用常量表示，比如`DISPATCH_MACH_SEND_DEAD`等。
- 如果是自定义事件类型的Dispatch Source，返回使用`dispatch_source_merge_data`函数设置的数据。

- `dispatch_source_get_mask`：该函数用于获取在创建Dispatch Source时设置的第三个参数`mask`。在进程类型，文件属性更改类型，Mach端口类型的Dispatch Source下该函数返回的结果与`dispatch_source_get_data`一样。

### 注册Cancellation Handler

Cancellation Handler就是当Dispatch Source被释放时用来处理一些后续事情，比如关闭文件描述符或者释放Mach端口等。我们可以使用`dispatch_source_set_cancel_handler`函数或者`dispatch_source_set_cancel_handler_f`函数给Dispatch Source注册Cancellation Handler：

```bash
dispatch_source_set_cancel_handler(dispatchSource) {            
    print("进行善后处理...")      
}
```

该函数有两个参数，第一个参数是目标Dispatch Source，第二个参数就是要进行善后处理的闭包或者函数。

### 更改Dispatch Source的目标队列

在上文中，我们说过可以使用`dispatch_source_create`函数创建Dispatch Source，并且在创建时会指定回调函数执行的队列，那么如果事后想更改队列，比如说想更改队列的优先级，这时我们可以使用`dispatch_set_target_queue`函数实现：

```bash
let dispatchQueueDefaultPriority = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)        
let dispatchSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatchQueueDefaultPriority)   
let dispatchQueueLowPriority = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0)     
dispatch_set_target_queue(dispatchSource, dispatchQueueLowPriority)
```

这里需要注意的是，如果在更改目标队列时，Dispatch Source已经监听到相关事件，并且回调函数已经在之前的队列中执行了，那么会一直在旧的队列中执行完成，不会转移到新的队列中去。

### 暂停恢复Dispatch Source

暂停和恢复Dispatch Source与Dispatch Queue一样，都适用`dispatch_suspend`和`dispatch_resume`函数。这里需要注意的是刚创建好的Dispatch Source是处于暂停状态的，所以使用时需要用`dispatch_resume`函数将其启动。

### 废除Dispatch Source

如果我们不再需要使用某个Dispatch Source时，可以使用`dispatch_source_cancel`函数废除，该函数只有一个参数，那就是目标Dispatch Source。

### Dispatch Source实践

说了这么多，这一节来看看Dispatch Source到底怎么用。

#### 用Dispatch Source监听定时器

Dispatch Source能监听的事件中有一个类型就是定时器，我们来看看如何实现：

```bash
class TestDispatchSource {  
    func launch() {  
        let dispatchQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
        let timer = createTimerDispatchSource(dispatch_time(DISPATCH_TIME_NOW, 0), interval: NSEC_PER_SEC * 5, leeway: 0, queue: dispatchQueue) {
            print("处理定时任务，该任务每5秒执行一次...")
        }
        dispatch_resume(timer)
        sleep(30)
    }
    func createTimerDispatchSource(startTime: dispatch_time_t, interval: UInt64, leeway: UInt64, queue: dispatch_queue_t, handler: dispatch_block_t) -> dispatch_source_t {
        let timerDispatchSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue)
        dispatch_source_set_timer(timerDispatchSource, startTime, interval, leeway)
        dispatch_source_set_event_handler(timerDispatchSource, handler)
        return timerDispatchSource
    }
}
```

上面的代码示例中一个新的函数`dispatch_source_set_timer`，该函数的作用就是给监听事件类型为`DISPATCH_SOURCE_TYPE_TIMER`的Dispatch Source设置相关属性，该函数有四个参数：
- `source`：该参数为目标Dispatch Source，类型为`dispatch_source_t`.
- `start`：该参数为定时器的起始时间，类型为`dispatch_time_t`。
- `interval`：该参数为定时器的间隔时间，类型为`UInt64`，间隔时间的单位是纳秒。
- `leeway`：该参数为间隔时间的精度，类型为`UInt64`，时间单位也是纳秒。

#### 用Dispatch Source监听自定义事件

Dispatch Source能监听的事件中有一个类型是自定义事件，下面我们来看看如何使用:

```bash
class TestDispatchSource {    
    func launch() {
        var totalProcess = 0
        let dispatchSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue())
        dispatch_source_set_event_handler(dispatchSource) {
            let process = dispatch_source_get_data(dispatchSource)
            totalProcess += Int(process)
            print("这里可以在主线程更新UI，显示进度条...进度为\(totalProcess)%")
        }
        dispatch_resume(dispatchSource)
        generateCustomEvent(dispatchSource)
    }
    func generateCustomEvent(dispatchSource: dispatch_source_t) {
        let queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)
        for index in 0...100 { 
            dispatch_sync(queue) {   
                print("模拟自定义事件...进度为\(index)%")  
                dispatch_source_merge_data(dispatchSource, 1)
                sleep(2)  
            }
        }
    }
}
```

我们来看看`generateCustomEvent(dispatchSource: dispatch_source_t)`方法，该方法的作用的是模拟自定义事件，首先创建一个全局并发队列，然后循环让其执行任务，在执行的任务里调用`dispatch_source_merge_data`函数，就可以触发监听类型为`DISPATCH_SOURCE_TYPE_DATA_ADD`或者`DISPATCH_SOURCE_TYPE_DATA_OR`的Dispatch
 Source。该函数有两个参数，第一个参数是目标Dispatch Source，第二个参数的类型是无符号长整型，用于向目标Dispatch Source中的对应变量追加指定的数。

我们再来看看如何监听自定义时间，首先创建类型为`DISPATCH_SOURCE_TYPE_DATA_ADD`的Dispatch Source，然后设置回调闭包，在闭包中使用`dispatch_source_get_data`获取追加的变量值，该函数只有一个参数，就是目标Dispatch Source，这里需要注意的是通过`dispatch_source_get_data`函数获取的变量值并不是累加值，而是每次调用`dispatch_source_merge_data`函数时设置的值，所以在上面的示例中用`totalProcess`变量累加每次获取到的值。

上面的示例可以用来模拟后台进行下载，根据下载的数据量使用`dispatch_source_merge_data`函数给目标Dispatch Source设置相应的变量值，然后在主线程中监听到Dispatch Source的自定义事件，通过`dispatch_source_get_data`函数获取到变量，用于更新显示进度条的UI。

> 
**作者简介：** 付宇轩（[@DevTalking](http://weibo.com/jacefu)），从事Java中间件开发、iOS开发。主要主持开发企业级ETL中间件、BPM中间件、企业级移动应用，个人博客地址：[http://www.devtalking.com](http://www.devtalking.com)。




