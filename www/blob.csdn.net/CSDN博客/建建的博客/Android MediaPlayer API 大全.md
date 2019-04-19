# Android MediaPlayer API 大全 - 建建的博客 - CSDN博客
2017年06月02日 09:44:39[纪建](https://me.csdn.net/u013898698)阅读数：250
个人分类：[Android](https://blog.csdn.net/u013898698/article/category/6941721)
## Android MediaPlayer API 大全
1）当一个MediaPlayer对象被刚刚用new操作符创建或是调用了reset()方法后，它就处于Idle状态。当调用了release()方法后，它就处于End状态。这两种状态之间是MediaPlayer对象的生命周期。
1.1) 在一个新构建的MediaPlayer对象和一个调用了reset()方法的MediaPlayer对象之间有一个微小的但是十分重要的差别。在处于Idle状态时，调用getCurrentPosition(), getDuration(), getVideoHeight(),getVideoWidth(), setAudioStreamType(int), setLooping(boolean), setVolume(float, float),pause(), start(), stop(), seekTo(int),
 prepare() 或者 prepareAsync() 方法都是编程错误。当一个MediaPlayer对象刚被构建的时候，内部的播放引擎和对象的状态都没有改变，在这个时候调用以上的那些方法，框架将无法回调客户端程序注册的OnErrorListener.onError()方法；但若这个MediaPlayer对象调用了reset()方法之后，再调用以上的那些方法，内部的播放引擎就会回调客户端程序注册的OnErrorListener.onError()方法了，并将错误的状态传入。
1.2) 我们建议，一旦一个MediaPlayer对象不再被使用，应立即调用release()方法来释放在内部的播放引擎中与这个MediaPlayer对象关联的资源。资源可能包括如硬件加速组件的单态组件，若没有调用release()方法可能会导致之后的MediaPlayer对象实例无法使用这种单态硬件资源，从而退回到软件实现或运行失败。一旦MediaPlayer对象进入了End状态，它不能再被使用，也没有办法再迁移到其它状态。
1.3) 此外，使用new操作符创建的MediaPlayer对象处于Idle状态，而那些通过重载的create()便利方法创建的MediaPlayer对象却不是处于Idle状态。事实上，如果成功调用了重载的create()方法，那么这些对象已经是Prepare状态了。
2) 在一般情况下，由于种种原因一些播放控制操作可能会失败，如不支持的音频/视频格式，缺少隔行扫描的音频/视频，分辨率太高，流超时等原因，等等。因此，错误报告和恢复在这种情况下是非常重要的。有时，由于编程错误，在处于无效状态的情况下调用了一个播放控制操作可能发生。在所有这些错误条件下，内部的播放引擎会调用一个由客户端程序员提供的OnErrorListener.onError()方法。客户端程序员可以通过调用MediaPlayer.setOnErrorListener（[Android](http://lib.csdn.net/base/android).media.MediaPlayer.OnErrorListener）方法来注册OnErrorListener.
2.1) 一旦发生错误，MediaPlayer对象会进入到Error状态。
2.2) 为了重用一个处于Error状态的MediaPlayer对象，可以调用reset()方法来把这个对象恢复成Idle状态。
2.3) 注册一个OnErrorListener来获知内部播放引擎发生的错误是好的编程习惯。
2.4) 在不合法的状态下调用一些方法，如prepare()，prepareAsync()和setDataSource()方法会抛出IllegalStateException异常。
3) 调用setDataSource(FileDescriptor)方法，或setDataSource(String)方法，或setDataSource(Context，Uri)方法，或setDataSource(FileDescriptor，long，long)方法会使处于Idle状态的对象迁移到Initialized状态。
3.1) 若当此MediaPlayer处于其它的状态下，调用setDataSource()方法，会抛出IllegalStateException异常。
3.2) 好的编程习惯是不要疏忽了调用setDataSource()方法的时候可能会抛出的IllegalArgumentException异常和IOException异常。
4) 在开始播放之前，MediaPlayer对象必须要进入Prepared状态。
4.1) 有两种方法（同步和异步）可以使MediaPlayer对象进入Prepared状态：要么调用prepare()方法（同步），此方法返回就表示该MediaPlayer对象已经进入了Prepared状态；要么调用prepareAsync()方法（异步），此方法会使此MediaPlayer对象进入Preparing状态并返回，而内部的播放引擎会继续未完成的准备工作。当同步版本返回时或异步版本的准备工作完全完成时就会调用客户端程序员提供的OnPreparedListener.onPrepared()监听方法。可以调用MediaPlayer.setOnPreparedListener(android.media.MediaPlayer.OnPreparedListener)方法来注册OnPreparedListener.
4.2) Preparing是一个中间状态，在此状态下调用任何具备边影响的方法的结果都是未知的！
4.3) 在不合适的状态下调用prepare()和prepareAsync()方法会抛出IllegalStateException异常。当MediaPlayer对象处于Prepared状态的时候，可以调整音频/视频的属性，如音量，播放时是否一直亮屏，循环播放等。
5) 要开始播放，必须调用start()方法。当此方法成功返回时，MediaPlayer的对象处于Started状态。isPlaying()方法可以被调用来[测试](http://lib.csdn.net/base/softwaretest)某个MediaPlayer对象是否在Started状态。
5.1) 当处于Started状态时，内部播放引擎会调用客户端程序员提供的OnBufferingUpdateListener.onBufferingUpdate()回调方法，此回调方法允许应用程序追踪流播放的缓冲的状态。
5.2) 对一个已经处于Started 状态的MediaPlayer对象调用start()方法没有影响。
6) 播放可以被暂停，停止，以及调整当前播放位置。当调用pause()方法并返回时，会使MediaPlayer对象进入Paused状态。注意Started与Paused状态的相互转换在内部的播放引擎中是异步的。所以可能需要一点时间在isPlaying()方法中更新状态，若在播放流内容，这段时间可能会有几秒钟。
6.1) 调用start()方法会让一个处于Paused状态的MediaPlayer对象从之前暂停的地方恢复播放。当调用start()方法返回的时候，MediaPlayer对象的状态会又变成Started状态。
6.2) 对一个已经处于Paused状态的MediaPlayer对象pause()方法没有影响。
7) 调用stop()方法会停止播放，并且还会让一个处于Started，Paused，Prepared或PlaybackCompleted状态的MediaPlayer进入Stopped状态。
7.1) 对一个已经处于Stopped状态的MediaPlayer对象stop()方法没有影响。
8) 调用seekTo()方法可以调整播放的位置。
8.1) seekTo(int)方法是异步执行的，所以它可以马上返回，但是实际的定位播放操作可能需要一段时间才能完成，尤其是播放流形式的音频/视频。当实际的定位播放操作完成之后，内部的播放引擎会调用客户端程序员提供的OnSeekComplete.onSeekComplete()回调方法。可以通过setOnSeekCompleteListener(OnSeekCompleteListener)方法注册。
8.2) 注意，seekTo(int)方法也可以在其它状态下调用，比如Prepared，Paused和PlaybackCompleted状态。此外，目前的播放位置，实际可以调用getCurrentPosition()方法得到，它可以帮助如音乐播放器的应用程序不断更新播放进度
9) 当播放到流的末尾，播放就完成了。
9.1) 如果调用了setLooping(boolean)方法开启了循环模式，那么这个MediaPlayer对象会重新进入Started状态。
9.2) 若没有开启循环模式，那么内部的播放引擎会调用客户端程序员提供的OnCompletion.onCompletion()回调方法。可以通过调用MediaPlayer.setOnCompletionListener(OnCompletionListener)方法来设置。内部的播放引擎一旦调用了OnCompletion.onCompletion()回调方法，说明这个MediaPlayer对象进入了PlaybackCompleted状态。
9.3) 当处于PlaybackCompleted状态的时候，可以再调用start()方法来让这个MediaPlayer对象再进入Started状态。
###### #
这张状态转换图清晰的描述了MediaPlayer的各个状态，也列举了主要的方法的调用时序，每种方法只能在一些特定的状态下使用，如果使用时MediaPlayer的状态不正确则会引发IllegalStateException异常。
Idle 状态：当使用new()方法创建一个MediaPlayer对象或者调用了其reset()方法时，该MediaPlayer对象处于idle状态。这两种方法的一个重要差别就是：如果在这个状态下调用了getDuration()等方法（相当于调用时机不正确），通过reset()方法进入idle状态的话会触发OnErrorListener.onError()，并且MediaPlayer会进入Error状态；如果是新创建的MediaPlayer对象，则并不会触发onError(),也不会进入Error状态。
End 状态：通过release()方法可以进入End状态，只要MediaPlayer对象不再被使用，就应当尽快将其通过release()方法释放掉，以释放相关的软硬件组件资源，这其中有些资源是只有一份的（相当于临界资源）。如果MediaPlayer对象进入了End状态，则不会在进入任何其他状态了。
Initialized 状态：这个状态比较简单，MediaPlayer调用setDataSource()方法就进入Initialized状态，表示此时要播放的文件已经设置好了。
Prepared 状态：初始化完成之后还需要通过调用prepare()或prepareAsync()方法，这两个方法一个是同步的一个是异步的，只有进入Prepared状态，才表明MediaPlayer到目前为止都没有错误，可以进行文件播放。
Preparing 状态：这个状态比较好理解，主要是和prepareAsync()配合，如果异步准备完成，会触发OnPreparedListener.onPrepared()，进而进入Prepared状态。
Started 状态：显然，MediaPlayer一旦准备好，就可以调用start()方法，这样MediaPlayer就处于Started状态，这表明MediaPlayer正在播放文件过程中。可以使用isPlaying()测试MediaPlayer是否处于了Started状态。如果播放完毕，而又设置了循环播放，则MediaPlayer仍然会处于Started状态，类似的，如果在该状态下MediaPlayer调用了seekTo()或者start()方法均可以让MediaPlayer停留在Started状态。
Paused 状态：Started状态下MediaPlayer调用pause()方法可以暂停MediaPlayer，从而进入Paused状态，MediaPlayer暂停后再次调用start()则可以继续MediaPlayer的播放，转到Started状态，暂停状态时可以调用seekTo()方法，这是不会改变状态的。
Stop 状态：Started或者Paused状态下均可调用stop()停止MediaPlayer，而处于Stop状态的MediaPlayer要想重新播放，需要通过prepareAsync()和prepare()回到先前的Prepared状态重新开始才可以。
PlaybackCompleted状态：文件正常播放完毕，而又没有设置循环播放的话就进入该状态，并会触发OnCompletionListener的onCompletion()方法。此时可以调用start()方法重新从头播放文件，也可以stop()停止MediaPlayer，或者也可以seekTo()来重新定位播放位置。
Error状态：如果由于某种原因MediaPlayer出现了错误，会触发OnErrorListener.onError()事件，此时MediaPlayer即进入Error状态，及时捕捉并妥善处理这些错误是很重要的，可以帮助我们及时释放相关的软硬件资源，也可以改善用户体验。通过setOnErrorListener(android.media.MediaPlayer.OnErrorListener)可以设置该监听器。如果MediaPlayer进入了Error状态，可以通过调用reset()来恢复，使得MediaPlayer重新返回到Idle状态。
补充一点开发时遇见的问题：
在停止播放的是代码应这样写才可以在下次播放时重新开始播放
```
private void stop(){
  if(mediaplayer!=null){//mediaplayer 是MediaPlayer的 instance
   mediaplayer.stop();
      try{
    mediaplayer.prepare();//stop后下次重新播放要首先进入prepared状态
    mediaplayer.seekTo(0);//须将播放时间设置到0；这样才能在下次播放是重新开始，否则会继续上次播放
   }catch(IOException e){
    e.printStackTrace();
   }
  }
```
```
```
