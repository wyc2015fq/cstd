# Android MediaPlayer类详解 - u013366022的专栏 - CSDN博客
2014年08月13日 14:46:42[slitaz](https://me.csdn.net/u013366022)阅读数：1434
MediaPlayer类可用于控制音频/视频文件或流的播放。关于如何使用这个类的方法还可以阅读SDK API中VideoView类的文档。
**1．状态图**
对播放音频/视频文件和流的控制是通过一个状态机来管理的。下图显示一个MediaPlayer对象被支持的播放控制操作驱动的生命周期和状态。椭圆代表MediaPlayer对象可能驻留的状态。弧线表示驱动MediaPlayer在各个状态之间迁移的播放控制操作。**这里有两种类型的弧线。由一个箭头开始的弧代表同步的方法调用，而以双箭头开头的代表的弧线代表异步方法调用**。
![](https://img-my.csdn.net/uploads/201204/19/1334825321_4339.gif)
通过这张图，我们可以知道一个**MediaPlayer**对象有以下的状态：
**1）当一个MediaPlayer对象被刚刚用new操作符创建或是调用了reset()方法后，它就处于Idle状态。当调用了release()方法后，它就处于End状态。这两种状态之间是MediaPlayer对象的生命周期。**
1.1) 在一个新构建的**MediaPlayer**对象和一个调用了**reset()**方法的**MediaPlayer**对象之间有一个微小的但是十分重要的差别。在处于Idle状态时，调用**getCurrentPosition()**,**getDuration()**, **getVideoHeight()**,**getVideoWidth()**,**setAudioStreamType(int)**, **setLooping(boolean)**, **setVolume(float, float)**,**pause()**,**start()**, **stop()**, **seekTo(int)**,**prepare()** 或者 **prepareAsync()** 方法都是编程错误。当一个**MediaPlayer**对象刚被构建的时候，内部的播放引擎和对象的状态都没有改变，在这个时候调用以上的那些方法，框架将无法回调客户端程序注册的**OnErrorListener.onError()**方法；但若这个MediaPlayer对象调用了**reset()**方法之后，再调用以上的那些方法，内部的播放引擎就会回调客户端程序注册的**OnErrorListener.onError()**方法了，并将错误的状态传入。
1.2) 我们建议，一旦一个**MediaPlayer**对象不再被使用，应立即调用**release()**方法来释放在内部的播放引擎中与这个**MediaPlayer**对象关联的资源。资源可能包括如硬件加速组件的单态组件，若没有调用**release()**方法可能会导致之后的**MediaPlayer**对象实例无法使用这种单态硬件资源，从而退回到软件实现或运行失败。一旦**MediaPlayer**对象进入了End状态，它不能再被使用，也没有办法再迁移到其它状态。
1.3) 此外，使用**new**操作符创建的**MediaPlayer**对象处于Idle状态，而那些通过重载的**create()**便利方法创建的**MediaPlayer**对象却不是处于Idle状态。事实上，如果成功调用了重载的**create()**方法，那么这些对象已经是Prepare状态了。 
2) **在一般情况下，由于种种原因一些播放控制操作可能会失败，如不支持的音频/视频格式，缺少隔行扫描的音频/视频，分辨率太高，流超时等原因，等等。因此，错误报告和恢复在这种情况下是非常重要的。有时，由于编程错误，在处于无效状态的情况下调用了一个播放控制操作可能发生。在所有这些错误条件下，内部的播放引擎会调用一个由客户端程序员提供的OnErrorListener.onError()方法。客户端程序员可以通过调用MediaPlayer.setOnErrorListener（android.media.MediaPlayer.OnErrorListener）方法来注册OnErrorListener.**
2.1) 一旦发生错误，**MediaPlayer**对象会进入到Error状态。
2.2) 为了重用一个处于Error状态的**MediaPlayer**对象，可以调用**reset()**方法来把这个对象恢复成Idle状态。
2.3) 注册一个**OnErrorListener**来获知内部播放引擎发生的错误是好的编程习惯。
2.4) 在不合法的状态下调用一些方法，如**prepare()，prepareAsync()**和**setDataSource()**方法会抛出**IllegalStateException**异常。 
3) **调用setDataSource(FileDescriptor)方法，或setDataSource(String)方法，或setDataSource(Context，Uri)方法，或setDataSource(FileDescriptor，long，long)方法会使处于Idle状态的对象迁移到Initialized状态。**
3.1) 若当此**MediaPlayer**处于其它的状态下，调用**setDataSource()**方法，会抛出**IllegalStateException**异常。
3.2) 好的编程习惯是不要疏忽了调用**setDataSource()**方法的时候可能会抛出的**IllegalArgumentException**异常和**IOException**异常。 
**4) 在开始播放之前，MediaPlayer对象必须要进入Prepared状态。**
4.1) 有两种方法（同步和异步）可以使**MediaPlayer**对象进入Prepared状态：要么调用**prepare()**方法（**同步**），此方法返回就表示该**MediaPlayer**对象已经进入了Prepared状态；要么调用**prepareAsync()**方法（**异步**），此方法会使此**MediaPlayer**对象进入Preparing状态并返回，而内部的播放引擎会继续未完成的准备工作。当同步版本返回时或异步版本的准备工作完全完成时就会调用客户端程序员提供的**OnPreparedListener.onPrepared()**监听方法。可以调用**MediaPlayer.setOnPreparedListener(android.media.MediaPlayer.OnPreparedListener)**方法来注册**OnPreparedListener**.
4.2) Preparing是一个中间状态，在此状态下调用任何具备边影响的方法的结果都是未知的！
4.3) 在不合适的状态下调用**prepare()**和**prepareAsync()**方法会抛出**IllegalStateException**异常。当**MediaPlayer**对象处于**Prepared**状态的时候，可以调整音频/视频的属性，如音量，播放时是否一直亮屏，循环播放等。 
**5) 要开始播放，必须调用start()方法。当此方法成功返回时，MediaPlayer的对象处于Started状态。isPlaying()方法可以被调用来测试某个MediaPlayer对象是否在Started状态。**
5.1) 当处于Started状态时，内部播放引擎会调用客户端程序员提供的**OnBufferingUpdateListener.onBufferingUpdate()**回调方法，此回调方法允许应用程序追踪流播放的缓冲的状态。
5.2) 对一个已经处于Started状态的**MediaPlayer**对象调用**start()**方法没有影响。
**6) 播放可以被暂停，停止，以及调整当前播放位置。当调用pause()方法并返回时，会使MediaPlayer对象进入Paused状态。注意Started与Paused状态的相互转换在内部的播放引擎中是异步的。所以可能需要一点时间在isPlaying()方法中更新状态，若在播放流内容，这段时间可能会有几秒钟。**
6.1) 调用**start()**方法会让一个处于Paused状态的**MediaPlayer**对象从之前暂停的地方恢复播放。当调用**start()**方法返回的时候，**MediaPlayer**对象的状态会又变成Started状态。
6.2) 对一个已经处于Paused状态的**MediaPlayer**对象**pause()**方法没有影响。
**7) 调用stop()方法会停止播放，并且还会让一个处于Started，Paused，Prepared或PlaybackCompleted状态的MediaPlayer进入Stopped状态。**
7.1) 对一个已经处于Stopped状态的**MediaPlayer**对象**stop()**方法没有影响。
**8) 调用seekTo()方法可以调整播放的位置。**
8.1) **seekTo(int)**方法是异步执行的，所以它可以马上返回，但是实际的定位播放操作可能需要一段时间才能完成，尤其是播放流形式的音频/视频。当实际的定位播放操作完成之后，内部的播放引擎会调用客户端程序员提供的**OnSeekComplete.onSeekComplete()**回调方法。可以通过**setOnSeekCompleteListener(OnSeekCompleteListener)**方法注册。
8.2) 注意，**seekTo(int)**方法也可以在其它状态下调用，比如Prepared，Paused和PlaybackCompleted状态。此外，目前的播放位置，实际可以调用**getCurrentPosition()**方法得到，它可以帮助如音乐播放器的应用程序不断更新播放进度
**9) 当播放到流的末尾，播放就完成了。**
9.1) 如果调用了**setLooping(boolean)**方法开启了循环模式，那么这个**MediaPlayer**对象会重新进入Started状态。
9.2) 若没有开启循环模式，那么内部的播放引擎会调用客户端程序员提供的**OnCompletion.onCompletion()**回调方法。可以通过调用**MediaPlayer.setOnCompletionListener(OnCompletionListener)**方法来设置。内部的播放引擎一旦调用了**OnCompletion.onCompletion()**回调方法，说明这个**MediaPlayer**对象进入了PlaybackCompleted状态。
9.3) 当处于PlaybackCompleted状态的时候，可以再调用**start()**方法来让这个**MediaPlayer**对象再进入Started状态。
