# Activity生命周期 与 重建 - u013366022的专栏 - CSDN博客
2014年06月03日 09:37:40[slitaz](https://me.csdn.net/u013366022)阅读数：544

每一个Android应用程序在运行时，对于底层的Linux Kernel而言都是一个单独的进程，但是对于Android系统而言，因为局限于手机画面的大小与使用的考虑，不能把每一个运行中的应用程序窗口都显示出来。
　　所以通常手机系统的界面一次仅显示一个应用程序窗口，Android使用了**Activity**的概念来表示界面。
　　运行中的应用程序分为五大类，分别是：
　　　　前景模式：foreground process
　　　　可见模式：visible process
　　　　背景模式：background process
　　　　空白模式：empty process
　　　　服务模式：service process
　　除了最后一个，貌似service process是Service的事情了。其他都与Activity相关。
　　Android系统会判断应用程序Activity是属于哪一个类，给予不同的Activity生命周期。      
　　Activity的生命周期也是它所在进程的生命周期。
**Activity生命周期的运行如图**：
![](http://pic002.cnblogs.com/images/2012/325852/2012120122450787.png)
## Activity生命周期进程类型
　　在Android系统中，**即使不关掉程序，当应用程序处于某种进程类时，也有可能被系统kill掉**。
　　Android系统通过运行机制，依照哪些画面或消息对使用者最重要以及当前内存使用状况，而做出是否kill Activity的决定。
**foreground process（前景模式）**是当前显示于手机屏幕上的应用程序画面，被定义为前景模式的进程，其中由onCreate()、onStart() 、onResume() 函数调用的Activity都会变成foreground process正在运行的Activity。
**visible process（可见模式）：**visible process最常发生的情况是当应用程序弹出对话框要与用户交互时，原应用程序就会变成透明（不可见）的，而对话窗口就会变成前景。
　　当对话窗口退出后，原应用程序马上就又变回原前景模式了。
　　在Activity窗口画面变为透明时，就会由onPause()函数掌控进入暂停状态。
　　当前景进程退出时，该Activity就会再度被拉到前景模式，由onResume() 函数唤醒。
**background process**是在Activity窗口画面被其他Activity完全盖掉，窗口画面已经完全看不见时，则会进入onStop()停止状态。
　　这种情况通常发生在两个不同的应用程序开启时，后开启的应用程序会覆盖掉原应用程序。
　　此时对background process Activity的处理**有两种选择**：一是直接被onDestroy()退出，该程序将完全关闭，无法再使用任何返回键回到该程序；另一个处理方式是当其他Activity需要内存时，这个background process会先被清除掉，释放出内存。
　　如果使用者再度浏览刚刚被清除掉的background process，则Android系统会自动再运行onCreate()重新启动该Activity，所以当系统需要内存时，就会暂时将背景进程清除，让它变成**empty process（空白模式**） ， 所以空白进程最重要的目的就是暂时释放出内存，直到使用者再度唤醒该empty
 process Activity时，才会将空白进程变成前景进程。
　　（Service相关）**service process（服务模式进程）**是由startService()所产生的，虽然服务进程有点类似背景进程在背景状态运行，但是它的运行等级和前景进程几乎一样高。
　　服务模式进程是持续运行的，虽然使用者看不到任何运行画面，Android系统不会自动关闭此类的服务进程，除非使用者自行关闭。这部分内容可在Service详解里面再讨论。
## 金字塔型的生命周期
![](http://pic002.cnblogs.com/images/2012/325852/2012120122484245.png)
**Activity生命周期的每一个阶段都表示为金字塔上的一个台阶，当系统创建一个新的activity时，每一个回调函数都把activity的状态网上挪一步。**
**金子塔的最顶层就是activity运行在前景模式下，用户可与之交互。**
　　当用户离开activity时，系统调用另一些回调函数，将activity的状态从金字塔中一步一步移下来。有些情况下，activity只移动一部分，并没有完全到底，这些情况下仍然可以移动回顶部。
　　注意这些状态中只有三个状态是静态（static）的，意味着activity只有在这三个状态下能停留一段时间：
**Resumed**：foreground，用户可交互running state
**Paused**：部分被遮挡，不能接收用户输入也不能执行代码，另一个半透明或者小的activity正挡在前面。
**Stopped**：activity完全被遮挡，不能被用户看到，activity被认为在background，当Stopped的时候，activity实例的状态信息被保留，但是不能执行任何代码。
　　其他状态都是转换状态，系统会很快调用其他相应的回调函数离开这些状态。比如系统调用onCreate()之后，会很快调用onStart()，之后是 onResume()。
## 回调函数
　　覆写这些回调函数时，首先要记得一定要调用基类的回调函数，即最开始一行永远是super.onXXX();
　　onPause()和onResume()中的动作应该互逆，比如说onPause()中释放了相机，那么onResume()中就要重新初始化相机。
　　Stopped状态下，UI对用户完全不可见，此时用户焦点在另一个activity或者另一个程序上。
　　onStop()中需要释放资源，因为有时候系统会kill掉Stopped状态的进程，如果有资源没有被释放，会造成内存泄露。
　　onStop()中还应该包括一些关闭操作，比如向数据库写信息。
　　当从Stopped状态回到前景时，首先需要调用onRestart()，这个函数做一些恢复工作，恢复停止但是并没有被销毁的activity；然后系统会接着调用onStart()，因为每次activity变为可见时都要调用onStart()。
　　可以把onStart()和onStop()看成一对，因为在一开始启动时和重新启动时都需要做一些初始化工作。
　　onDestroy()一般都是在onPause()和onStop()之后调用，但有一个例外的情况：如果在onCreate()中调用finish()方法，系统将会立即调用onDestroy()而不用经过生命周期中的其他阶段。
## Activity生命周期测试程序
注释部分来自于《Professional Android 4 Application Development》：
```
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
public class LifeOfActivity extends Activity
{
    private static final String TAG = "ActivityLife";
    // Called at the start of the full lifetime.
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_life_of);
        Log.e(TAG, "start onCreate~~~");
        // Initialize Activity and inflate the UI.
    }
    // Called after onCreate has finished, use to restore UI state.
    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState)
    {
        super.onRestoreInstanceState(savedInstanceState);
        Log.e(TAG, "start onRestoreInstanceState~~~");
        // Restore UI state from the savedInstanceState.
        // This Bundle has also been passed to onCreate.
        // Will only be called if the Activity has been killed by the system
        // since it was last visible.
    }
    // Called at the start of the visible lifetime.
    @Override
    protected void onStart()
    {
        super.onStart();
        Log.e(TAG, "start onStart~~~");
        // Apply any required UI change now that the Activity is visible.
    }
    // Called before subsequent visible lifetimes for an Activity process.
    @Override
    protected void onRestart()
    {
        super.onRestart();
        Log.e(TAG, "start onRestart~~~");
        // Load changes knowing that the Activity has already been visible
        // within this process.
    }
    // Called at the start of the active lifetime
    @Override
    protected void onResume()
    {
        super.onResume();
        Log.e(TAG, "start onResume~~~");
        // Resume any paused UI updates, threads, or processes required
        // by the Activity but suspended when it was inactive.
    }
    // Called to save UI state changes at the end of the active lifecycle.
    @Override
    protected void onSaveInstanceState(Bundle outState)
    {
        super.onSaveInstanceState(outState);
        Log.e(TAG, "start onSaveInstanceState~~~");
        // Save UI state changes to the outState(savedInstanceState).
        // This Bundle will be passed to onCreate and onRestoreInstanceState if
        // the process is killed and restarted by the run time.
    }
    // Called at the end of the active lifetime.
    @Override
    protected void onPause()
    {
        super.onPause();
        Log.e(TAG, "start onPause~~~");
        // Suspend UI updates, threads, or CPU intensive processes that don't
        // need to be updated when the Activity isn't the active foreground
        // Activity.
    }
    // Called at the end of the visible lifetime.
    @Override
    protected void onStop()
    {
        super.onStop();
        Log.e(TAG, "start onStop~~~");
        // Suspend remaining UI updates, threads, or processing that aren't
        // required when the Activity isn't visible.
        // Persist all edits or state changes as after this call the process if
        // likely to be killed.
    }
    // Sometimes called at the end of the full lifetime.
    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        Log.e(TAG, "start onDestroy~~~");
        // Clean up any resources including ending threads, closing databases
        // connections etc.
    }
}
```
## 重新创建Activity
　　如果activity是自己销毁的，实例就永远消失了，但是如果系统因为资源限制销毁了activity，虽然这个实例已经不在了，但是当用户返回到它时，系统会利用这个activity被销毁时存储的数据，重新创建一个实例。
## Activity的销毁和重建
　　Activity的销毁分为两种:
**　　第一种**是正常的销毁，比如用户按下Back按钮或者是activity自己调用了finish()方法；
**　　另一种**是由于activity处于stopped状态，并且它长期未被使用，或者前台的activity需要更多的资源，这些情况下系统就会关闭后台的进程，以恢复一些内存。
　　需要注意的是这其中有一种情况就是屏幕旋转的问题，当用户旋转手机屏幕，每一次都会导致activity的销毁和重新建立。
**在第二种情况下，尽管实际的activity实例已经被销毁，但是系统仍然记得它的存在，当用户返回到它的时候，系统会创建出一个新的实例来代替它，这里需要利用旧实例被销毁时候存下来的数据。**这些数据被称为“instance state”，是一个存在Bundle对象中的键值对集合。
缺省状态下，系统会把每一个View对象保存起来（比如EditText对象中的文本，ListView中的滚动条位置等），即如果activity实例被销毁和重建，那么不需要你编码，layout状态会恢复到前次状态。
　　但是如果你的activity需要恢复更多的信息，比如成员变量信息，则需要自己动手写了。
　　如果要存储额外的数据，必须覆写回调函数onSaveInstanceState().
　　系统会在用户离开activity的时候调用这个函数，并且传递给它一个Bundle object，如果系统稍后需要重建这个activity实例，它会传递同一个Bundle object到onRestoreInstanceState() 和 onCreate() 方法中去。
![](http://pic002.cnblogs.com/images/2012/325852/2012121720385489.png)
当系统停止activity时，它会调用onSaveInstanceState()（过程1），如果activity被销毁了，但是需要创建同样的实例，系统会把过程1中的状态数据传给onCreate()和onRestoreInstanceState()（图中标出的2和3）。
## 存储Activity状态
　　当系统停止activity时，系统会调用onSaveInstanceState()，状态信息会以**键值对**的形式存储下来。
　　默认的实现中存储了activity的view系列的状态，比如文本和滚动条位置等。
　　要存储额外的信息，必须自己实现onSaveInstanceState()，并且给Bundle object加上键值对。
　　比如：
```
static final String STATE_SCORE = "playerScore";
static final String STATE_LEVEL = "playerLevel";
...
@Override
public void onSaveInstanceState(Bundle savedInstanceState) {
    // Save the user's current game state
    savedInstanceState.putInt(STATE_SCORE, mCurrentScore);
    savedInstanceState.putInt(STATE_LEVEL, mCurrentLevel);
    
    // Always call the superclass so it can save the view hierarchy state
    super.onSaveInstanceState(savedInstanceState);
}
```
**要记得调用基类的实现，以实现默认的实现。**
## 恢复Activity状态
　　当activity重建时，需要根据Bundle中的状态信息数据恢复activity。
onCreate() 和onRestoreInstanceState()回调函数都会接收到这个Bundle。
　　因为每次创建新的activity实例的或重建一个实例的时候都会调用onCreate()方法，所以必须先检查是否Bundle是null，如果是null，则表明是要创建一个全新的对象，而不是重建一个上次被销毁的对象。
　　比如onCreate()方法可以这么写：
```
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState); // Always call the superclass first
   
    // Check whether we're recreating a previously destroyed instance
    if (savedInstanceState != null) {
        // Restore value of members from saved state
        mCurrentScore = savedInstanceState.getInt(STATE_SCORE);
        mCurrentLevel = savedInstanceState.getInt(STATE_LEVEL);
    } else {
        // Probably initialize members with default values for a new instance
    }
    ...
}
```
除了在onCreate()中恢复状态外，也可以选择在onRestoreInstanceState()中实现，这个函数在onStart()之后调用。
**只有在有数据要恢复的时候系统会调用onRestoreInstanceState()，所以不必检查Bundle是否为null。**
```
public void onRestoreInstanceState(Bundle savedInstanceState) {
    // Always call the superclass so it can restore the view hierarchy
    super.onRestoreInstanceState(savedInstanceState);
   
    // Restore state members from saved instance
    mCurrentScore = savedInstanceState.getInt(STATE_SCORE);
    mCurrentLevel = savedInstanceState.getInt(STATE_LEVEL);
}
```
此处也要注意，不要忘记调用基类实现。

