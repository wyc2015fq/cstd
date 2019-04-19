# Android MediaPlayer使用方法简单介绍---(setdatasource) - u013366022的专栏 - CSDN博客
2014年05月21日 10:12:40[slitaz](https://me.csdn.net/u013366022)阅读数：6795
1）如何获得MediaPlayer实例：
可以使用直接new的方式：
MediaPlayer mp = new MediaPlayer();
也可以使用create的方式，如：
MediaPlayer mp = MediaPlayer.create(this, R.raw.test);//这时就不用调用setDataSource了
2) 如何设置要播放的文件：
MediaPlayer要播放的文件主要包括3个来源：
a. 用户在应用中事先自带的resource资源
例如：MediaPlayer.create(this, R.raw.test);
b. 存储在SD卡或其他文件路径下的媒体文件
例如：mp.setDataSource("/sdcard/test.mp3");
c. 网络上的媒体文件
例如：mp.setDataSource("[http://www.citynorth.cn/music/confucius.mp3](http://www.citynorth.cn/music/confucius.mp3)");
MediaPlayer的setDataSource一共四个方法：
setDataSource (String path)
setDataSource (FileDescriptor fd)
setDataSource (Context context, Uri uri)
setDataSource (FileDescriptor fd, long offset, long length)
其中使用FileDescriptor时，需要将文件放到与res文件夹平级的**assets**文件夹里，然后使用：
AssetFileDescriptor fileDescriptor = getAssets().openFd("rain.mp3");
m_mediaPlayer.setDataSource(fileDescriptor.getFileDescriptor(),fileDescriptor.getStartOffset(), fileDescriptor.getLength());
来设置datasource
3）对播放器的主要控制方法：
Android通过控制播放器的状态的方式来控制媒体文件的播放，其中：
prepare()和prepareAsync() 提供了同步和异步两种方式设置播放器进入prepare状态，需要注意的是，如果MediaPlayer实例是由create方法创建的，那么第一次启动播放前不需要再调用prepare（）了，因为create方法里已经调用过了。
start()是真正启动文件播放的方法，
pause()和stop()比较简单，起到暂停和停止播放的作用，
seekTo()是定位方法，可以让播放器从指定的位置开始播放，需要注意的是该方法是个异步方法，也就是说该方法返回时并不意味着定位完成，尤其是播放的网络文件，真正定位完成时会触发OnSeekComplete.onSeekComplete()，如果需要是可以调用setOnSeekCompleteListener(OnSeekCompleteListener)设置监听器来处理的。
release()可以释放播放器占用的资源，一旦确定不再使用播放器时应当尽早调用它释放资源。
reset()可以使播放器从Error状态中恢复过来，重新会到Idle状态。
4）设置播放器的监听器：
MediaPlayer提供了一些设置不同监听器的方法来更好地对播放器的工作状态进行监听，以期及时处理各种情况，
如： setOnCompletionListener(MediaPlayer.OnCompletionListener listener)、
setOnErrorListener(MediaPlayer.OnErrorListener listener)等,设置播放器时需要考虑到播放器可能出现的情况设置好监听和处理逻辑，以保持播放器的健壮性。
