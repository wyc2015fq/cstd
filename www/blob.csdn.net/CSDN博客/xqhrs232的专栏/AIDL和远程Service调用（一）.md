# AIDL和远程Service调用（一） - xqhrs232的专栏 - CSDN博客
2012年02月29日 17:15:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：607标签：[service																[import																[android																[interface																[button																[encoding](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andy_android/article/details/6929008](http://blog.csdn.net/andy_android/article/details/6929008)
本讲内容：AIDL和远程Service调用
本讲源代码：App_elfPlayer
本讲的内容，理解起来很难，也许你看了很多资料也看不明白，但是用起来缺简单的要命。所以我们干脆拿一个音乐播放器中进度条的实例来说明一下 AIDL和Remote Service的价值和使用方法，你把这个例子跑一边，体会一下就OK了。下面的例子是我正在准备的项目实例中的一部分。
首先说明一下我们面临的问题，如果看不懂下面的描述请看前面的课程：
第一、我们知道在AndroId中如果需要进行音乐播放，最方面的方法就是使用自带的MediaPlayer对象，如果我们在Activity中控 制MediaPlayer对象进行播放，那么一旦你打开了另外一个程序譬如浏览器，那么歌声就会立刻停止，这当然不是我们需要的结果。 我们需要的是在做其他事情的同时能够在后台听歌，于是我们就需要把对MediaPlayer对象的操作放在后台Service中去。
第二、我们已经把对MediaPlayer的操作转移到Service中去了，按照我们以前的做法，我们在Activity中发送一个Intent 对象给Service对象，在Intent中传送播放啊、暂停啊一类的信息给Service，这样Service就知道该怎么做了。这一切看起来很美好， 可是现在出了一个新问题，那就是我想在Activity中显示一个进度条，这个进度条要跟着Service中的MediaPlayer中的歌曲进度同步向 前走，而且如果我点击进度条中的某一个位置，还想让歌曲跳转到新的时间点继续播放，这个，该怎么实现？
第三、我们需要在Activity中操作Service中的MediaPlayer对象，就好像这个对象是自己的一样。我们可以采用**Android接口定义语言**AIDL（Android Interface Definition Language）技术：
1、把Service中针对MediaPlayer的操作封装成一个接口(.aidl文件) 
2、在Service中建个子类实现这接口的存根(stub)对象 
3、并在onBind()方法中返回这个存根对象。 
4、 在Activity中使用绑定服务的方式连接Service，但是不用Intent来传递信息，而是在ServiceConnection的 onServiceConnected方法里，获得Service中Stub对象的客户端使用代理。我们通过操作Activity中的代理就可以达到操作 Service中的MediaPlayer对象的目的。这样我们就可以想用本地对象一样操作Service中的对象了，那么进度条一类的需求自然也就迎刃 而解。
下面的例子，并不是专门为本讲准备的，所以有些无关代码，而且没加注释，请见谅（本例完整讲解会放在项目实训中，正在准备）：
1、新建一个项目 App_elfPlayer ，启动Activity是个启动画面:CoverActivity
2、AndroidManifest.xml 的内容如下：
|`01`|`<?``xml``version``=``"1.0"``encoding``=``"utf-8"``?>`|
|`02`|`<``manifest``package``=``"app.android.elfplayer"``xmlns:android``=``"http://schemas.android.com/apk/res/android"``android:versioncode``=``"1"``android:versionname``=``"1.0"``>`|
|`03`|`    ``<``uses``-sdk``=``""``android:minsdkversion``=``"7"``>`|
|`04`|`    ``<``uses``-permission``=``""``android:name``=``"android.permission.WRITE_EXTERNAL_STORAGE"``></``uses``>`|
|`05`||
|`06`|`    ``<``application``android:label``=``"@string/app_name"``android:icon``=``"@drawable/icon"``>`|
|`07`|`        ``<``activity``android:name``=``".CoverActivity"``>`|
|`08`|`            ``<``intent``-filter``=``""``>`|
|`09`|`                ``<``action``android:name``=``"android.intent.action.MAIN"``>`|
|`10`|`                ``<``category``android:name``=``"android.intent.category.LAUNCHER"``>`|
|`11`|`            ``</``category``></``action``></``intent``>`|
|`12`|`        ``</``activity``>`|
|`13`|`        ``<``activity``android:name``=``".PlayerActivity"``>`|
|`14`|`        ``</``activity``>`|
|`15`|`        ``<``service``android:name``=``".MusicService"``android:enabled``=``"true"``>`|
|`16`|`        ``</``service``>`|
|`17`|`    ``</``application``>`|
|`18`||
|`19`|`</``uses``></``manifest``>`|
我们注意到有2个Activity，1个Service，还有读写外部存储的权限声明
3、CoverActivity.java的代码如下：这是个全屏的启动画面，2秒后会跳转到PlayerActivity
|`01`|`package``app.android.elfplayer;`|
|`02`||
|`03`|`import``android.app.Activity;`|
|`04`|`import``android.content.Intent;`|
|`05`|`import``android.os.Bundle;`|
|`06`|`import``android.os.Handler;`|
|`07`|`import``android.view.Window;`|
|`08`|`import``android.view.WindowManager;`|
|`09`||
|`10`|`public``class``CoverActivity ``extends````Activity {```|
|`11`|`    ``/** Called when the activity is first created. */`|
|`12`|`    ``@Override`|
|`13`|`    ``public``void``onCreate(Bundle savedInstanceState) {`|
|`14`|`        ``super``.onCreate(savedInstanceState);`|
|`15`|`        ``getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);`|
|`16`|`        ``requestWindowFeature(Window.FEATURE_NO_TITLE);`|
|`17`|`        ``setContentView(R.layout.cover);`|
|`18`||
|`19`|`        ``new``Handler().postDelayed(``new``Runnable(){`|
|`20`||
|`21`|`             ``@Override`|
|`22`|`             ``public``void``run() {`|
|`23`|`                 ````Intent mainIntent =````new``Intent(CoverActivity.``this``,PlayerActivity.``class``);`|
|`24`|`                 ``CoverActivity.``this``.startActivity(mainIntent);`|
|`25`|`                 ``CoverActivity.``this``.finish();`|
|`26`|`             ``}`|
|`27`||
|`28`|`            ````},````2000``);`|
|`29`||
|`30`|`    ``}`|
|`31`|`}`|
4、PlayerActivity.java的代码如下：
|`001`|`package``app.android.elfplayer;`|
|`002`||
|`003`|`import``android.app.Activity;`|
|`004`|`import``android.content.ComponentName;`|
|`005`|`import``android.content.Context;`|
|`006`|`import``android.content.Intent;`|
|`007`|`import``android.content.ServiceConnection;`|
|`008`|`import``android.os.Bundle;`|
|`009`|`import``android.os.Handler;`|
|`010`|`import``android.os.IBinder;`|
|`011`|`import``android.os.Message;`|
|`012`|`import``android.os.RemoteException;`|
|`013`|`import``android.util.Log;`|
|`014`|`import``android.view.View;`|
|`015`|`import``android.widget.ImageButton;`|
|`016`|`import``android.widget.SeekBar;`|
|`017`|`import``android.widget.SeekBar.OnSeekBarChangeListener;`|
|`018`||
|`019`|`public``class``PlayerActivity ``extends````Activity {```|
|`020`||
|`021`|`    ``public``static``final````int````PLAY = ``1``;`|
|`022`|`    ``public``static``final````int````PAUSE = ``2``;`|
|`023`||
|`024`|`    ``ImageButton imageButtonFavorite;`|
|`025`|`    ``ImageButton imageButtonNext;`|
|`026`|`    ``ImageButton imageButtonPlay;`|
|`027`|`    ``ImageButton imageButtonPre;`|
|`028`|`    ``ImageButton imageButtonRepeat;`|
|`029`|`    ``SeekBar musicSeekBar;`|
|`030`||
|`031`|`    ``IServicePlayer iPlayer;`|
|`032`|`    ``boolean``isPlaying = ``false``;`|
|`033`|`    ``boolean``isLoop = ``false````;```|
|`034`||
|`035`|`    ``@Override`|
|`036`|`    ``public``void``onCreate(Bundle savedInstanceState) {`|
|`037`|`        ``super``.onCreate(savedInstanceState);`|
|`038`|`        ``setContentView(R.layout.player);`|
|`039`||
|`040`|`        ``imageButtonFavorite = (ImageButton) findViewById(R.id.imageButtonFavorite);`|
|`041`|`        ``imageButtonNext = (ImageButton) findViewById(R.id.imageButtonNext);`|
|`042`|`        ``imageButtonPlay = (ImageButton) findViewById(R.id.imageButtonPlay);`|
|`043`|`        ``imageButtonPre = (ImageButton) findViewById(R.id.imageButtonPre);`|
|`044`|`        ``imageButtonRepeat = (ImageButton) findViewById(R.id.imageButtonRepeat);`|
|`045`|`        ``musicSeekBar = (SeekBar) findViewById(R.id.musicSeekBar);`|
|`046`||
|`047`|`        ``bindService(``new``Intent(PlayerActivity.``this``, MusicService.``class``), conn, Context.BIND_AUTO_CREATE);`|
|`048`|`        ``startService(``new``Intent(PlayerActivity.``this``, MusicService.``class``));`|
|`049`||
|`050`|`        ``imageButtonPlay.setOnClickListener(``new``View.OnClickListener() {`|
|`051`||
|`052`|`            ``@Override`|
|`053`|`            ``public``void``onClick(View v) {`|
|`054`|`                ``Log.i(``"yao"````,````"imageButtonPlay -> onClick"``);`|
|`055`||
|`056`|`                ``if``(!isPlaying) {`|
|`057`|`                    ``try``{`|
|`058`|`                        ``iPlayer.play();`|
|`059`|`                    ````}````catch``(RemoteException e) {`|
|`060`|`                        ``e.printStackTrace();`|
|`061`|`                    ``}`|
|`062`|`                    ``imageButtonPlay.setBackgroundResource(R.drawable.pause_button);`|
|`063`|`                    ````isPlaying =````true``;`|
|`064`||
|`065`|`                ````}````else``{`|
|`066`|`                    ``try``{`|
|`067`|`                        ``iPlayer.pause();`|
|`068`|`                    ````}````catch``(RemoteException e) {`|
|`069`|`                        ``e.printStackTrace();`|
|`070`|`                    ``}`|
|`071`|`                    ``imageButtonPlay.setBackgroundResource(R.drawable.play_button);`|
|`072`|`                    ````isPlaying =````false``;`|
|`073`|`                ``}`|
|`074`|`            ``}`|
|`075`|`        ``});`|
|`076`||
|`077`|`        ``musicSeekBar.setOnSeekBarChangeListener(``new``OnSeekBarChangeListener() {`|
|`078`||
|`079`|`            ``@Override`|
|`080`|`            ``public``void````onProgressChanged(SeekBar seekBar,````int``progress, ``boolean``fromUser) {`|
|`081`|`            ``}`|
|`082`||
|`083`|`            ``@Override`|
|`084`|`            ``public``void``onStartTrackingTouch(SeekBar seekBar) {`|
|`085`|`            ``}`|
|`086`||
|`087`|`            ``@Override`|
|`088`|`            ``public``void``onStopTrackingTouch(SeekBar seekBar) {`|
|`089`|`                ``if``(iPlayer != ``null``) {`|
|`090`|`                    ``try``{`|
|`091`|`                        ``iPlayer.seekTo(seekBar.getProgress());`|
|`092`|`                    ````}````catch``(RemoteException e) {`|
|`093`|`                        ``e.printStackTrace();`|
|`094`|`                    ``}`|
|`095`|`                ``}`|
|`096`|`            ``}`|
|`097`|`        ``});`|
|`098`||
|`099`|`        ``handler.post(updateThread);`|
|`100`|`    ``}`|
|`101`||
|`102`|`    ``private``ServiceConnection conn = ``new``ServiceConnection() {`|
|`103`|`        ``public``void``onServiceConnected(ComponentName className, IBinder service) {`|
|`104`|`            ``Log.i(``"yao"````,````"ServiceConnection -> onServiceConnected"``);`|
|`105`|`            ``iPlayer = IServicePlayer.Stub.asInterface(service);`|
|`106`|`        ``}`|
|`107`||
|`108`|`        ``public``void``onServiceDisconnected(ComponentName className) {`|
|`109`|`        ``};`|
|`110`|`    ``};`|
|`111`||
|`112`|`    ````Handler handler =````new``Handler() {`|
|`113`|`        ``@Override`|
|`114`|`        ``public``void``handleMessage(Message msg) {`|
|`115`|`        ``};`|
|`116`|`    ``};`|
|`117`||
|`118`|`    ``private``Runnable updateThread = ``new``Runnable() {`|
|`119`|`        ``@Override`|
|`120`|`        ``public``void``run() {`|
|`121`|`            ``if``(iPlayer != ``null``) {`|
|`122`|`                ``try``{`|
|`123`|`                    ``musicSeekBar.setMax(iPlayer.getDuration());`|
|`124`|`                    ``musicSeekBar.setProgress(iPlayer.getCurrentPosition());`|
|`125`|`                ````}````catch``(RemoteException e) {`|
|`126`|`                    ``e.printStackTrace();`|
|`127`|`                ``}`|
|`128`|`            ``}`|
|`129`|`            ``handler.post(updateThread);`|
|`130`|`        ``}`|
|`131`|`    ``};`|
|`132`||
|`133`|`}`|
5、其中用到的IServicePlayer.aidl,放在和Java文件相同的包中，内容如下：
|`01`|`package``app.android.elfplayer;`|
|`02`|`interface``IServicePlayer{`|
|`03`|`    ``void``play();`|
|`04`|`    ``void``pause();`|
|`05`|`    ``void``stop();`|
|`06`|`    ``int``getDuration();`|
|`07`|`    ``int``getCurrentPosition();`|
|`08`|`    ``void``seekTo(``int````current);```|
|`09`|`    ``boolean``setLoop(``boolean````loop);```|
|`10`|`}`|
一旦你写好了这个IServicePlayer.aidl文件，ADT会自动帮你在gen目录下生成IServicePlayer.java文件
6、MusicService.java的内容如下：
|`01`|`package``app.android.elfplayer;`|
|`02`||
|`03`|`import``android.app.Service;`|
|`04`|`import``android.content.Intent;`|
|`05`|`import``android.media.MediaPlayer;`|
|`06`|`import``android.os.IBinder;`|
|`07`|`import``android.os.RemoteException;`|
|`08`|`import``android.util.Log;`|
|`09`||
|`10`|`public``class``MusicService ``extends````Service {```|
|`11`||
|`12`|`    ````String tag =````"yao"``;`|
|`13`||
|`14`|`    ``public``static``MediaPlayer mPlayer;`|
|`15`||
|`16`|`    ``public``boolean``isPause = ``false``;`|
|`17`||
|`18`|`    ````IServicePlayer.Stub stub =````new``IServicePlayer.Stub() {`|
|`19`||
|`20`|`        ``@Override`|
|`21`|`        ``public``void``play() ``throws``RemoteException {`|
|`22`|`            ``mPlayer.start();`|
|`23`|`        ``}`|
|`24`||
|`25`|`        ``@Override`|
|`26`|`        ``public``void``pause() ``throws``RemoteException {`|
|`27`|`            ``mPlayer.pause();`|
|`28`|`        ``}`|
|`29`||
|`30`|`        ``@Override`|
|`31`|`        ``public``void``stop() ``throws``RemoteException {`|
|`32`|`            ``mPlayer.stop();`|
|`33`|`        ``}`|
|`34`||
|`35`|`        ``@Override`|
|`36`|`        ``public``int``getDuration() ``throws``RemoteException {`|
|`37`|`            ``return``mPlayer.getDuration();`|
|`38`|`        ``}`|
|`39`||
|`40`|`        ``@Override`|
|`41`|`        ``public``int``getCurrentPosition() ``throws``RemoteException {`|
|`42`|`            ``return``mPlayer.getCurrentPosition();`|
|`43`|`        ``}`|
|`44`||
|`45`|`        ``@Override`|
|`46`|`        ``public``void``seekTo(``int``current) ``throws````RemoteException {```|
|`47`|`            ``mPlayer.seekTo(current);`|
|`48`|`        ``}`|
|`49`||
|`50`|`        ``@Override`|
|`51`|`        ``public``boolean``setLoop(``boolean``loop) ``throws````RemoteException {```|
|`52`|`            ``return``false``;`|
|`53`|`        ``}`|
|`54`||
|`55`|`    ``};`|
|`56`||
|`57`|`    ``@Override`|
|`58`|`    ``public``void``onCreate() {`|
|`59`|`        ````Log.i(tag,````"MusicService onCreate()"``);`|
|`60`|`        ``mPlayer = MediaPlayer.create(getApplicationContext(), ElfPlayerUtil.getFileinSD(``"wind.mp3"``));`|
|`61`|`    ``}`|
|`62`||
|`63`|`    ``@Override`|
|`64`|`    ``public``IBinder onBind(Intent intent) {`|
|`65`|`        ``return``stub;`|
|`66`|`    ``}`|
|`67`||
|`68`|`}`|
7、其它代码和资源可以参见本讲附带的源代码，编译并运行程序，查看结果：
![image](http://www.imiyoo.com/uploads/allimg/110802/1QG25V0-0.png)
![image](http://www.imiyoo.com/uploads/allimg/110802/1QG22314-1.png)
![image](http://www.imiyoo.com/uploads/allimg/110802/1QG23407-2.png)
最后总结一下，AIDL提供了一种非常简单的方式，让我们可以把一个进程内的对象或方法暴露给另一个程序使用，就好象另一个程序也拥有这些功能一样。
最后感谢一首歌这个网站，本讲的图片素材采用的是他们的UI元素，好了，本讲就到这里。
