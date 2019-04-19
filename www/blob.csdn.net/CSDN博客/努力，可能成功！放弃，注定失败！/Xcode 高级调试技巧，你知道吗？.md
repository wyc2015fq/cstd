# Xcode 高级调试技巧，你知道吗？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年04月02日 16:34:06[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：117

在使用正确的情形下，IDE 内置的调试工具能够帮我们节省大量时间。例如，与原始手动控制台打印相比，最简单设置断点进行单步调试的效率则要高出一个量级。而且这还仅仅只是调试武器库里的冰山一角，Xcode 有一整套方便的工具，旨在加快调试工作流程。
断点处自动执行任务
先来回顾一下断点调试时的常见场景：代码执行到了断点处暂停后，我们一般都会在控制台输入 *po myVariable* 等 LLDB 命令来获取上下文信息，然后再继续代码执行操作。
显然如果上述手动敲命令行的事情需要多次执行的话，那么调试过程也就变得非常乏味了，理想的情形应该是将其自动化，步骤如下：
选中断点右键进行编辑
点击 *Add Action*
选中 *Debugger Command*
输入你想执行的 LLDB 命令
勾选 *Automatically continue after evaluating actions* ，如果你希望执行相关命令后，继续运行程序。
![](https://upload-images.jianshu.io/upload_images/10344541-a4fac48141707c7f..png!web?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
这样当每次断点被触发后，我们设置的自定义命令就会被执行。整个过程无需手动敲 LLDB 命令，节约不少的开发时间。
条件断点
有时候我们可能希望设置的断点只会在特定情形下才会触发。一个常见的例子就是：在某循环语句代码中，我们希望某断点在特定情形下被触发。这样我们就能避免大量无效触发，而且将自己从持续的 *continue* 点击下解脱出来。
![](https://upload-images.jianshu.io/upload_images/10344541-9b3b48a7b9dd3d61..png!web?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
上面的例子中断点只会在 *counter > 20* 时才会被触发。当然你还可以使用更为复杂的 BOOL 语句，甚至是执行函数调用。
断点忽略
与条件断点相反，有时候我们可能需要在特定的时候跳过断点。例如，某个功能函数在生命周期内会被多次调用，但是我们对第一次调用不感兴趣。因此我们可以如下进行设置，使其在首次调用时直接跳过触发断点的行为：
![](https://upload-images.jianshu.io/upload_images/10344541-140887eb430ec9b3..png!web?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
异常断点
上述几个情形下，我们都会明确在某行代码设置断点，但是有时候我们需求的断点则无法提前预设。例如，当程序在运行时发生异常并且未被捕获，此时程序会出现 Crash 。虽然 Xcode 会在控制台的打印出具体的调用栈等信息，并且大多数时候这些信息对于错误定位都非常有用，但是它毕竟没有直接将我们带到特定的上下文。
为了解决这个问题，我们就需要针对特定事件设置断点而非特定代码。你可以根据自己感兴趣的事件类型进行具体设置：
![](https://upload-images.jianshu.io/upload_images/10344541-f06b190893930f37..png!web?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
符号断点
异常断点严格意义上来说是符号断点的一个具体类型，我们还可以定义更多自定义类型的通用断点。例如，我们可以对 *viewDidLoad* 方法的调用设置一个通用断点，在每次调用时进行触发：
![](https://upload-images.jianshu.io/upload_images/10344541-3075d351ef5890df..png!web?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
通过这种方式，我们就无需在每个调用处手动设置断点。
变量跟踪
上述的那些断点都是针对程序中的代码执行。但是应用却不是只包含代码，其中还有非常重要的另一个对象：数据。所以除了代码执行需要设置断点之外，某些数据的变化过程也有可能是我们所关心事件。
为了实现数据变化过程的跟踪，首先我们需要在变量可见域内设置断点。然后我们那在控制台右键设置变量观察，最后你可以取消该辅助断点。
![](https://upload-images.jianshu.io/upload_images/10344541-e9ffeffad95d4f35..png!web?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
当然，我们也可以在控制台通过 LLDB 命令来完成设置：
watchpointsetvariableself.counter
完成设置后，变量在整个生命周期内发生的任何变化都会触发断点。
# 。
作者：程晓媛闯天涯
我是晓媛，喜欢记得关注我哦！
作者：程晓媛闯天涯
链接：https://www.jianshu.com/p/6e80a0adcbdf
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
