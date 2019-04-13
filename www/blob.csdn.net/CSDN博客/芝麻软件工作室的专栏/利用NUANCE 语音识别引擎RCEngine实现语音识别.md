
# 利用NUANCE 语音识别引擎RCEngine实现语音识别 -  芝麻软件工作室的专栏 - CSDN博客


2012年08月09日 10:44:49[seven-soft](https://me.csdn.net/softn)阅读数：808



# 1.   基础概念：
RCEngine ：
是一个封装了语音识别，语音文件操作，电话控制的类，它派生自 RCEngineInterface 抽象基类。所以要在程序中调用 Nuance 的语音识别功能你就必须实例化 RCEngine 。
NotifiableAdapter ：
RCEngine 的所有函数都是异步函数，它使用确认 --- 通知形式与用户程序通信，要获取到这些确认和通知消息你必须建立一个消息处理类，而这个类必须派生自 NotifiableAdapter ，并且在实例化 RCEngine 时把本类指针交给 RCEngine 。
Application object ：
通常 Nuance 把一个派生自 NotifiableAdapter 的且与一个 RCEngine 对象对应得实例化对象称为一个 Application object 。一个应用程序可以根据自己有多少个 license 来创建多个 Application object 。在 Nuance 的例子和我写的客户端中 RCAPP 就是一个 Application object 。
Dispatcher object :
他是个事件分发对象，他一直都在不断的作把识别事件分发给各个 RCEngine 的循环，当所有的 RCEngine 都摧毁时他便自动结束事件循环。
Triggerable ：
触发器，你若要让 Dispatcher 自动在某触发事件放生时调用你的处理函数，那么你的处理函数实现的类必须要派生于 Triggerable 。所以我的代码中 RCAPP 也派生于这个类。

# 2.   本客户端组成：
RCAPP ： 担当 Application object 和 Triggerable 角色。
AudioSampleFetcher ： 一个为 RCEngine 提供识别数据的辅助类。
CRCDispatcher ： 担当 Dispatcher object 角色。
# 3.   本程序中各个类之间的关系：
每个 RCAPP 包含一个 AudioSampleFetcher 为它在识别时提供数据。
系统中可根据 license 个数创建多个 RCAPP 。各个 RCAPP 的消息需要由 CRCDispatcher 分发。而 CRCDispatcher 是个消息分发的循环体，它只有在所有 RCAPP 都被删除时才退出循环。 CRCDispatcher 在系统中只可以有一个。
# 4.   识别客户端初始化流程：
一个客户端从创建到开始可以进行识别需要一系列对象创建过程，这个过程很重要，而且也比较复杂。下面我将一步步地把这个过程描述出来：
## 4.1. Dispatcher object 的创建：
这是所有操作的开始点。我们需要一些准备工作来创建这个 Dispatcher 对象：
### 4.1.1. 创建一个 NuanceConfig 对象：
对于单独语法包的客户端，我们可以用函数 NuanceConfigBuild （）来创建，只要向它提供语法包路径就可以了。对于多语法包的客户端我们要用 NuanceConfigBuildFromCommandLine （）函数来创建，创建时把所有参数作为一个存放字符串数组 (char**) 放到它的第二个参数里，它第一个参数是参数个数。这里必须要注意第二个参数的数据的格式和空格大小写等，任何一个错误都会引起创建失败。我的做法是用一个配置文件存储所有的参数，然后逐一读出，组装成一个字符串指针数组。下面是我的配置文件的部分：
packagedir=H:/GHT/ICA/CVP_M
packagedir=H:/GHT/ICA/CVP_C
audio.Provider=mem
client.Behaviors=calllog,timeout
。。。。
读出整理后要求的格式是：
-package
H:/GHT/ICA/CVP_M
–package
H:/GHT/ICA/CVP_M
audio.Provider=mem
client.Behaviors=calllog,timeout
。。。。
上面每一行作为字符串（ char* ）分别存储到以下的 punit 数组中：
typedef char* _tCmdLnUnit;
_tCmdLnUnit punit[256];
然后再调用 NuanceConfigBuildFromCommandLine （）
### 4.1.2.
### 创建一个 Dispatcher ：
这部分比较简单只要把上一步创建的 NuanceConfig 作为参数传入 Dispatcher 的构造函数即可。

## 4.2. 创建 Application object ：
### 4.2.1.
### 创建 RCAPP
### ：
本客户端的 RCAPP 对象就是 Application object 对象。它派生于 NotifiableAdapter 和 Triggerable ，并且它有一个 RCEngine * 的内部成员。
### 4.2.2.
### 创建 RCEngine ：
其构造函数为： RCEngine(NuanceConfig const* config, DispatcherInterface & dispatcher, Notifiable & notifiable, NuanceStatus & status) ；把刚才创建的 NuanceConfig 对象、 Dispatcher 对象、 RCAPP 对象的指针作为参数即可。
## 4.3. 启动 Dispatcher 的消息循环：
### 4.3.1.
### 创建一个线程：
Dispatcher 的消息循环需要独占一个线程。
### 4.3.2.
### 执行消息循环：
只要执行 Dispatcher 的 Dispatch （）函数即可。该函数不会退出，除非所有 RCEngine 都被删掉。当它退出时，就是这个 Dispatcher 应该被删除的时候了。
## 4.4. HandleInitializationCompleted 被调用：
当你的 Application object 的 HandleInitializationCompleted 被调用时且通告状态值是 NUANCE_OK 时，说明你的 RCEngine 已初始化成功了。但注意这里并不是表示你可以进行识别了，你还需要做以完一下工作：

### 4.4.1.
### 设置播音为外部：
因为我们使用的是自己的播放音平台，所以必须设定是外部播音。
4.4.2. 打开数据库
我们要用到动态语法所以必须要打开数据库。用函数 OpenDatabase （）打开数据库，这里要提供 odbc 数据源名称，用户帐号，数据库类型的信息。
4.4.3. 打开 calllog 通道
调用 OpenCalllogChannel （）函数打开 callog 通道让 Nuance 把 callog 放到 NuanceConfig 对象创建时参数指定的位置，否则 callog 会放到客户端程序同样路径下。
4.4.4. 创建 AudioSampleFetcher ：
这是个为识别提供数据的辅助类，在这里创建比较合适。
## 4.5. HandleNuanceDBOpened ：
通告状态值是 NUANCE_OK 时，说明你的数据库打开成功。这个时候，你的初始化成功完成了。你可以进入下一步，开始识别了。
## 4.6. 启动识别：
### 4.6.1. 设定识别阀值：
当的识别得分，低于这个值时，识别结果就会被拒绝。
### 4.6.2. 设定 NoSpeechTimeoutSecs 值：
当 RCEngine 启动超后在该值时间内没有人声输入，系统就会结束识别并返回 NoSpeechTimeout 信息。
### 4.6.3.
### 预定义输出结果格式：
当需要格式化的识别结果时，要在这里设定好结果的输出格式。比如我们输入的格式是： <&confidence>spelling:<spelling> 则输出的结果可以是： 69 spelling:chai4 shan1 shan1 . 表示识别分数 69, 识别出来的 slot （这里是 <spelling> ）对应的值是 chai4 　 shan1 　 shan1 。
### 4.6.4.
### 启动识别：
使用 RCEngine 的 RecognizeUtterance （）函数启动识别。这里要提供一个 Top grammar ，也就是静态语法中的 top grammar. 识别过程需要一定的时间等待结果，若你要在被过程中启动超时则可以通过设定 behavior.timeout.ExternalPromptDone 为 TRUE ，让之前设定的 NoSpeechTimeoutSecs 生效。
### 4.6.5.
### 启动录音：
由 AudioSampleFetcher 的 StartPlatformDependentRecording 函数调用外部录音平台录音。 AudioSampleFetcher 会创建触发器，该触发器会每 100 毫秒（该值可以在启动触发器时自己定义，推荐用默认的 100 毫秒）进行一次录音数据的输入。
### 4.6.6.
### 发现人音 HandleStartOfSpeech ：
正常的话，当在音频数据中发现人声时，你的 application object 的 HandleStartOfSpeech 被调用。这时你可以停掉外部的平台放音。
### 4.6.7.
### 发现语音结束点 HandleEndOfSpeech ：
当识别系统认为人声结束时，你的 application object 的 HandleEndOfSpeech 会被调用。
### 4.6.8.
### 发现语音结束点 HandleRecognitionStopped ：
当系统识别结束时这个 application object 的 HandleRecognitionStopped 会被调用。这时你要做的是用 AudioSampleFetcher 的 StopPlatformDependentRecording 停掉外部平台录音。用 RecResultGetType （）获取识别结束原因，除了成功外，其中还饱含识别失败的原因等，用 RecResultGetTextResult 获取识别的格式化结果。至此一次识别结束。
# 5.   识别客户端的关闭：
识别客户端关闭的具体步骤是：
## 5.1. 停止所有识别任务：
用 RCEngine 的 Abort （）函数终止一切操作。
## 5.2. 关闭动态语法数据库：
用 RCEngine 的 CloseDatabase （）关闭语法数据库。
## 5.3. 删除所有 Application object( 在它释构时删除它的 RCEngine 对象成员 ) ：
即删除本客户端的 RCAPP 对象。
## 5.4. Dispatcher 退出消息循环时删除 Dispatcher object 对象：
这里一定要等到 Dispatcher 自动退出循环才删掉这个对象，不然会引起错误。


