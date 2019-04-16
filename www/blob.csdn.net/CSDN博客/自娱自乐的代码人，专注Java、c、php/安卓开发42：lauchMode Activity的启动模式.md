# 安卓开发42：lauchMode Activity的启动模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月24日 14:46:41[initphp](https://me.csdn.net/initphp)阅读数：1485
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)








  最近在做东西的时候，发现有时会出现两个相同的Activity，本来从Act_A跳转到Act_B，那么按下返回键的时候正常情况下会返回到Act_A，但按下返回键之后居然还是Act_B，这问题一开始还让我纳闷了半天，不过了解了关于Activity的LaunchMode之后就豁然开朗了，在此留个笔记。
        在Android 2.3中Activity的启动模式有4种，分别是：Standard、SingleTop、SingleTask、SingleInstance；设置系统的启动模式可以在AndroidManifest.xml中的<Activity>标签中添加如下属性：

        android:launchMode="standard"

        android:launchMode=”singleTop”

        android:lauchMode=”singleTask”

        android:lauchMode=”singleInstance”

其中，standard是默认属性，singleTop表示栈顶只有一个Activity实例，而singleTask表示当前栈内只有一个实例，而singleInstance就是我们常说的单例模式，通过这种模式启动的Activity会存在与一个单独的Task中，而该Task中只有singleInstance的Activity实例。下面就来详细说一下几种启动模式：

        在此之前我们先看一下Android中关于Task的概念吧：

        Task可以认为是一个栈，可放入多个Activity。比如启动一个应用，那么Android就创建了一个Task，然后启动这个应用的入口Activity，就是intent-filter中配置为main和launch的那个。这个Activity是根（Root）Activity位于栈底，可能会在它的界面调用其他Activity，这些Activity如果按照上面那三个模式，也会在这个栈（Task）中，只是实例化的策略不同而已。在栈中我们知道，应该遵循栈的规则“Last
 In,First Out”也就是后进先出。如下图1：

![](http://hi.csdn.net/attachment/201112/17/0_132408751549eO.gif)

图1





        1.Standard模式

        在默认情况下，也就是我们新建一个工程，工程中的主Activity没有做任何设置，但此时系统会认为这是默认的启动格式。

        在这种模式下，系统会在当前的task中创建一个Activity的实例，该Activity可以被实例化多次并属于不同的task，一个task可以拥有多个Activity的实例。这是最基本的LaunchMode因此在此不多说。

        2.SingleTop模式

        在SingleTop模式下，如果一个Activity的实例已存在于当前栈的栈顶，同时在此时收到了启动该Activity的Intent消息，这时该Activity不会再去创建一个实例了，而是直接引用当前栈顶的该Activity。

        这个意思很好理解，比如：我们启动一个程序，该程序有两个Activity，首先启动Activity_A，然后通过Activity_A跳转到Activity_B,此时如果在Activity_A中有一个定时器，隔10秒后再启动Activity_B，那么此时在SingleTop模式下Activity_B中的onCreate方法是不会再次执行的。这也就避免了Activity_B的重复创建，但是此时Activity_B中的onNewIntent()覆写方法会被执行。如下图2：

![](http://hi.csdn.net/attachment/201112/17/0_1324088666x8Ns.gif)


图2

        这里首先进入根Activity也就是LunchModeDemo然后通过按钮跳转到TestA，TestA是SingleTop模式，通过打印出的log信息我们可以知道，TestA的确没有重新创建，而是执行了其中onNewIntent()方法，我们将TestA修改为Standard模式看下，如图3：

![](http://hi.csdn.net/attachment/201112/17/0_1324089139Rqvn.gif)


图3

       可以很明显的看到，TestA的onCreate()方法执行了两次，也就是说通过SingleTop模式，可以防止当前Activity被重复启动。

       3.SingleTask模式

        从命名上我们就可以知道其大致的作用，也就是说在SingleTask模式下，如果没有Activity的实例，那么系统会创建一个新的task同时实例化该Activity为Root Activity。但是如果该Activity的实例已经存在与另外的task中，那么系统不会再创建新的task和Activity的实例，而是直接去引用已经存在的Activity。在同一时间内，系统只会存在一个该Activity的实例。


如果一个Activity是SingleTask模式，经过多次调用回到该Activity时，它会位于当前栈的栈顶，而之前位于栈顶的Activity将会被Destroy掉(A ->  B_singleTask -> C -> D -> A -> B_singleTask这时在B_Acticity所在的task中，只剩下A和B，同时B在栈顶。其余的经过调用产生的Activity和task都会被系统回收掉)。如图4：

![](http://hi.csdn.net/attachment/201112/17/0_13240937035Tlv.gif)

图4

        log信息如下：

![](http://hi.csdn.net/attachment/201112/17/0_1324094289CV6a.gif)

         从log信息中我们可以看到结果和描述的一致。


        在SDK中有如下例子如图5：

![](http://hi.csdn.net/attachment/201112/17/0_1324097895o2z0.gif)

图5

以下是我自己写的一个类似的例子，用于验证：

       Activity_A是SingleTask类型，我们通过Activity_A启动Activity_B，然后在一个新的task中启动Activity_C,最后通过Activity_C启动Activity_D;我们假设A和B在task1中，C和D在task2中。现在通过Activity_D调用Activity_B，在成功执行后，会跳转到Activity_B的界面中，这时会回到task1中。如图6：

![](http://hi.csdn.net/attachment/201112/17/0_1324090460c7xG.gif)

图6




       执行后输出log信息如下：

![](http://hi.csdn.net/attachment/201112/17/0_1324098419NC2C.gif)

结果和SDK途中描述的一致。但是在SDK中还有一个具体的例子，原文如下：

“As another example, the Android Browser application declares that the web browser activity should always open in its own task—by specifying the singleTask launch mode in the <activity> element. This means that if
 your application issues an intent to open the Android Browser, its activity is not placed in the same task as your application. Instead, either a new task starts for the Browser or, if the Browser already has a task running in the background, that task is
 brought forward to handle the new intent.”

翻译如下：

“另外一个例子，Android浏览器声明它的浏览Activity应该一直处于它自己的任务中，因为它在<activity>标签中使用了特殊的SingleTask属性。这也就意味着，如果你的应用程序发送了一个intent去打开Android浏览器，那么浏览器的Activity不会和你的应用程序放在同一个任务里。取而代之是为该浏览器创建一个新的task，但如果该浏览器已经处于一个任务中并在后台运行，那么这个任务将会根据传递的intent而切换到前台。”

       以下是分析验证步骤：

       (1).我们使用Android自带的浏览器浏览网页，如下图7：

![](http://hi.csdn.net/attachment/201112/17/0_1324099051g8dJ.gif)

图7


(2).运行我们自己的程序，当我们自己的程序启动之后，该浏览器程序就会在后台执行，程序界面如下图8:

![](http://hi.csdn.net/attachment/201112/17/0_13240994215IJ2.gif)


图8

(3).在程序界面上只有一个Button，打开系统默认浏览器并跳转到csdn网页，这时我们点击Button后如图9:

![](http://hi.csdn.net/attachment/201112/17/0_1324099758RZFm.gif)


图9

(4).这时我们会打开浏览器并跳转到相应的界面，但我之前已经打开过浏览器，并在浏览网页了，根据SingleTask 的特点，我们可以知道，这个主界面并不会重新创建，而是继续引用原来的，那么这个是否是我们之前打开的那个浏览器呢？如图10:

![](http://hi.csdn.net/attachment/201112/17/0_13240998424wtx.gif)


图10

        通过浏览器自带的Menu中的“窗口“可以看到，我们之前浏览的网页在这里，也就是说这个程序并没有重新创建，而是在同一个程序中开启了一个新的Activity，而这个Activity会打开我们指定的网址，这就是SingleTask模式的特点。SingleTask在浏览器应用中比较常见，因为当不同的程序调用浏览器的时候，会启动浏览器的跳转页面，这个页面就是浏览器的主页面，每个打开的网页就是一个webView。


        SingleTask总结：

        从目前来看，SingleTask用在浏览器这种应用上非常合适，别的应用还真不清楚。当别的应用调用具有SingleTask的Activity时，系统只生成了一个，而前面生成的页面内容，全都保存起来了，这个不就是浏览器的功能吗！

       4.SingleInstance模式

        声明该属性的Activity将会在一个独立的Task中，该Task有且仅有它一个Activity的实例，任何由它启动的Activity都将会运行在另外的Task中。

        SingleInstance模式和SingleTask模式很相似，但是又有区别，比如SingleTask并不一定会新建一个Task，而SingleInstance一定会新建一个Task，同时在新建的Task中有且仅有一个SingleInstance类型的RootActivity。SingleTask可以和多个Activity共用一个Task。以下是SingleInstance属性的Activity调用流程图，如图11：

![](http://hi.csdn.net/attachment/201112/17/0_1324101865RlD4.gif)


图11

       通过lod信息我们可以的得到如下验证：

![](http://hi.csdn.net/attachment/201112/17/0_1324102290lb9u.gif)

       从图11中，我们可以知道，LunchModeDemo 在启动TestE时，新建了一个Task20，但是在TestE启动TestF时，却将TestF放到了Task19中，这时候TestF继续启动TestD，TestD也在Task19中。当我们退出的时候，首先返回TestF，然后再返回LunchModeDemo，注意这里返回的不是TestE，在LunchModeDemo返回之后我们看到了TestE，最后TestE返回之后我们看到了主界面。如果仔细的朋友一定发现了，按照常理来说，TestF回退之后应该会到TestE中，但结果却回到了LunchModeDemo中。Android中的返回是根据栈来返回的，因为LunchModeDemo和TestD、TestF在一个栈里，自然按照"后进先出"的规则出栈。

       下图12是另一种验证方法：

![](http://hi.csdn.net/attachment/201112/17/0_1324102563GcuE.gif)


图12


        以下是代码的log信息：

![](http://hi.csdn.net/attachment/201112/17/0_1324103887Ei2I.gif)

        从图12中我们可以知道，用户最终看到的是TestE的界面，启动流程几乎和图11一致，但在TestD这一步程序再次通过StartActivity(intent)启动了TestE，但是TestE并没有创建新的Activity。但是这次的退出流程就和上次不一样了，上次最终是由LunchModeDemo退出的，而这次是由TestE最终退出。

       SingleInstance总结：

       Activity的SingleInstance属性将使得该Activity只会创建一个，而且会独自拥有一个属于自己的Task。但是在返回的时候，也就是在Activity出栈的时候，Activity的返回顺序是根据栈来返回的。当前栈顶的Activity返回之后，位于当前栈顶下第一个Activity会展示出来，然后继续返回，当该栈的RootActivity返回之后，会继续调转到创建该RootActivity的栈，并重复前面的动作。




总结：

       因为在项目遇到了某些问题所有回过头来仔细看了看关于Activity的一些LaunchMode，希望这些小总结对大家都有所帮助。

       文中所到的源码都在Ubuntu 10.04环境下通过Eclipse编译通过，有兴趣的朋友可以自行下载源码，无需资源分。



