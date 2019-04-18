# Android 即时通讯开发小结（一） - weixin_33985507的博客 - CSDN博客
2018年06月11日 11:22:01[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> 《Android 即时通讯开发小结》基于IM Andriod 开发的各种常见问题，结合[网易云信](https://link.juejin.im?target=http%3A%2F%2Fwww.yunxin.163.com%2F%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)即时通讯技术的实践，对[IM](https://link.juejin.im?target=http%3A%2F%2Fwww.yunxin.163.com%2FIM%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)开发做一个全面的总结。
### 相关推荐阅读
[Android
即时通讯开发小结（二）](https://link.juejin.im?target=http%3A%2F%2Fnetease.im%2Fblog%2Fim6-0608%2F%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)
[移动IM开发指南1：如何进行技术选型](https://link.juejin.im?target=http%3A%2F%2Fnetease.im%2Fblog%2Fim9-0608%2F%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)
[移动IM开发指南2：心跳指令详解](https://link.juejin.im?target=http%3A%2F%2Fnetease.im%2Fblog%2Fim10-0608%2F%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)
[移动IM开发指南3：如何优化登录模块](https://link.juejin.im?target=http%3A%2F%2Fnetease.im%2Fblog%2Fim11-0608%2F%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)
### 客户端架构
作为一个 IM 软件，最重要的一个特性就是保证消息的达到率和实时性。达到率受服务器性能和设计协议影响，后面再谈。而实时性则主要取决于客户端进程是否长期存活，连接是否一致保持。
### 进程切分
在 Android 系统中，App 对于自己应用的生命周期是基本没有控制力，系统能在任意时候将你的进程杀死，且不会发出任何通知，也会在它认为合适的时候把你叫起来。进程前后台切换也同样不会给出任何通知。不过进程的生死控制也还是有一些规矩的，大体上来说就是进程占的资源越多（内存，CPU 时间等等），对于用户越不重要（前台进程->可视进程->服务进程->后台进程->空进程），越容易被干掉。因此，进程应当尽量小巧，且具有高的优先级。
如果一个应用本身就很小巧的话，一个进程就完全足够了，主线程负责 UI，另起一个后台线程跑一个服务。而如果应用比较庞大的话，将推送服务独立出来则是一个更好的选择。主进程负责用户交互和主要的业务逻辑，占用庞大的资源，当退到后台后，随时被杀死都无所谓。推送进程则仅仅负责与服务器交互，保持最小限度的业务逻辑处理。
网络连接和登录状态是绑在一起的，登录之后，同步数据也是必须的操作。因此，登录和同步数据都需要在推送进程中完成，除此之外，其他的业务都交给 UI 进程处理。推送进程收到自己不属于自己的协议时，就将数据扔给 UI 进程处理。
两个进程之间通信方式没有别的选择，只有 AIDL，难点在于接口的设计。IM 业务逻辑复杂，我们不可能为每一个调用实现一个 AIDL 接口，因此肯定会把接口调用打包成控制命令传递。而标识控制命令比较容易想到的方式，是采用类似于 Message 的 what，由我们为每一个控制命令分配一个命令号（或者再加一个子命令号），并指定对应的命令数据格式，接收端根据命令号再将数据反解出来处理。这种方式比较麻烦，且可维护性很差。更优雅的方式是使用远程过程调用，发送端申明业务的调用接口，并在远端实现这些接口，当发送端调用这些接口时，远端直接调用对应接口的实现。除了使用各种第三方框架外，Java 自身的 Proxy 也能实现这个功能。而从推送进程到 UI 进程还有一点不同，UI 进程随时可能会被干掉，AIDL 调用可能会返回失败，此种情况可选择 Intent 方式传递数据，并兼具唤起 UI 进程的功能。
### 保  活
保活分为三个方面，一是系统API提供了接口，应用自己就能做的，这是”合法“的，二是利用系统的缺陷，躲开系统的审查，这算是”非法“的，或者是”灰色“的，三就是多个 App 结盟，互相唤醒，这是耍流氓，谁的阵营庞大谁就赢。
第一种主要有系统闹钟，各种事件的 BroadcastReceiver，任务被移除的回调通知等。
第二种已知的就是在 4.4 及以前版本上，使用 native 进程，并将该进程从 davilk 父进程中脱离，挂接到 init 进程上，以此避开系统的查杀。然后在这个 native 进程中，定时唤起应用。为了让这个 native 进程更轻巧，可以使用 exec 的方式启动一个可执行文件，以除掉直接 fork 带入的 Zygote 进程环境。另外，这种方式也被用在监听自己应用被卸载时弹出调查窗口。
第三种方式现在各大互联网公司都在使用，方式很简单，互相调用指定的 Service，或发指定得广播即可。只要你起一个阿里系的 App，其他阿里系的 App 都会被跟着唤起。你启动一个装了友盟 SDK 的 App， 其他装了友盟 SDK 的
App，以及阿里系的 App 都会被跟着唤醒。
通常，第一种是必备，第二种和第三种则会结伴出现，流氓到底。
### 通信协议选择
消息的实时性的另一个保证是长连接。当然，你也可以用短连接轮询，但这个一般只在网页端短时聊天使用，在 Android 后台无限时轮询没有人能受的了。长连接类型可以选传统的 TCP，也可以使用
比较新的 WebSocket。 使用后一种的好处主要是服务器的，他们一套连接就可以服务好 App 端和 Web 端。
IM 的通信协议选择性很多，开源的有 XMPP，MQTT等，使用开源协议的优势在于上手快，资料多。而大部分主流 IM 则一般会设计私有的通信协议。使用私有协议，可以针对自己的业务逻辑，设计出更省流量，效率更高的协议，同时，还能有效保护自己的生态圈，就像 Android 手机装不了苹果系统，易信用户不能给微信用户发消息一样。
私有协议的协议内容和开源协议差不多，可以包含通用的协议头，然后加上负载包体。打包时，为了追求可读性，可以使用文本协议，为了追求省流量，则一般使用二进制协议。
在设计私有协议时，消息必达是一个需要侧重考量的地方。由于移动网络的复杂性，消息在客户端和服务器之间传递是有很大可能被传丢的。当客户端发送消息给服务器时，客户端并不能确保消息一定就会被服务器收到，需要服务器在收到消息后给客户端一个回馈，如果客户端没有收到回馈，就需要在一定超时后重新发送。这里存在一个问题就是有可能服务器已经收到了，但回馈的包被丢掉了，这时就会造成消息重复，为了去重，我们需要为相同的消息分配相同的 uuid，供接收方去重。同样，当服务器将消息转发给接收端时，服务器也不能保证接收端就一定能收到，需要接收端给服务器一个回执，告诉服务器这条消息我已经收到了，你就不要再给我发了。
Android
即时通讯开发小结（二）将会从“建立安全连接”、“心跳”、“断线重连”、“多媒体数据管理”、“图片”、“语音”等问题出发，结合[网易云信](https://link.juejin.im?target=http%3A%2F%2Fwww.yunxin.163.com%2F%3Ffrom%3Djuejin%26amp%3Butm_source%3Djuejin%26amp%3Butm_medium%3Darticle%26amp%3Butm_content%3Dim-tech-5)即时通讯技术的实践， 进行详细的介绍和总结。
