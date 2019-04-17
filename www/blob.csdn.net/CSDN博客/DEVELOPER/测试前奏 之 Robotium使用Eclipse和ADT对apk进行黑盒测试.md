# 测试前奏 之 Robotium使用Eclipse和ADT对apk进行黑盒测试 - DEVELOPER - CSDN博客





2018年09月29日 17:09:40[学术袁](https://me.csdn.net/u012827205)阅读数：254








# 测试的基本概念

曝光两个概念：黑盒测试、白盒测试
- *黑盒测试*
功能测试、数据驱动测试或基于规格说明的测试，是通过使用整个软件或某种软件功能来严格地测试, 而并没有通过检查程序的源代码或者很清楚地了解该软件的源代码程序具体是怎样设计的。（无源码基于apk的测试）- *白盒测试*
通过程序的源代码进行测试而不使用用户界面。这种类型的测试需要从代码句法发现内部代码在算法，溢出，路径，条件等等中的缺点或者错误，进而加以修正。（基于源码的测试）
# 测试准备

对安卓应用进行测试，测试工具和测试环境的配置、测试工具配置准备是必不可少的。

## 测试环境的搭建和工具的配置

1）**JDK** 环境搭建

2）安卓的**SDK**环境的搭建

3）**Eclips**e安装，配置**ADT**和**SDK**
至于jdk的环境搭建，我想于大家而言 so easy 喽。

而**sdk**的配置活儿，或许被非安卓移动开发人员遇到，倒是一个难点！

但是也不要捉急，就是下载**eclipse**嘛，而最方便的方式是**adt-bundle**的下载！
**链接：**[https://pan.baidu.com/s/1TgkFk6q7nqSaCzx0QjLVAQ](https://pan.baidu.com/s/1TgkFk6q7nqSaCzx0QjLVAQ)**密码：** uqsq

当然你也可以不使用这里的adt-bundle中的eclipse、adt和sdk，使用你之前安卓开发中安装配置过的也是ok的。。。。。但是，有可能你会遇到很多的坑，各式各样，像我一样。因为我就是这么倔强，这么过来的！

当前，先依照正确方式走下去，一下是我的正确便准配置（目前这个IDE在我的配置下使用是没有问题的）
![在这里插入图片描述](https://img-blog.csdn.net/20180929155758170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

那么有了这个adt-bundle，基本的配置方式就不再赘述，[可查看这里进行配置](https://blog.csdn.net/m0_37690319/article/details/77890997)

由于在配置adt和sdk过程有些地方需要注意，所以这里简单提个醒，防止入炕！
## ADT安装

多截取几张图，“清晰明了” 送给你
![在这里插入图片描述](https://img-blog.csdn.net/20180929164159265?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ADT的安装过程第6步、第5步，按照上面的勾选处理，避免浪费配置按章的宝贵时间。
## SDK安装

安装sdk时，于国内网络安装基本无效，因此需要"科学上网"才行，或者配置下国内的专门跨域代理。下面是我使用过的，觉得还行，可用的代理服务

HTTP Proxy Server  :  [mirrors.neusoft.edu.cn](http://mirrors.neusoft.edu.cn)

HTTP Proxy Port  　  :  80

由于对sdk的安装是非常耗时间的，所以建议晚上下载，或许第二天就下载完了呢，也说不定！
![在这里插入图片描述](https://img-blog.csdn.net/20180929161706638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**这里可能会有朋友遇到这种问题？？**`为什么我点击 SDK Manager.exe 并不能看到象上图一样展示那么多的Package Android API版本！！`
![在这里插入图片描述](https://img-blog.csdn.net/20180929162019265?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

原因是你和我，拥有一样的好奇心，勾选了不该勾选的选项，哈哈。取消红色框框箭头指向的复选框即可。
![在这里插入图片描述](https://img-blog.csdn.net/20180929162236267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里在配置代理的时候，这个`Use downlaod cache` 的选项是不能够勾选的，不然在你执行 SDK Manager.exe的时候，一直会使用已有的版本，而不去拉取并展示更多的API版本。
***多一嘴，当SDK安装完成之后，不要忘记添加到系统环境变量中进行配置哦。***
## 测试工具robotium.jar

对于测试工具嘛，这是使用的是Robotium进行的测试工作。所以呢，[点击这里进行Robotium和Junit4的jar包下载](https://download.csdn.net/download/u012827205/10695865)

当然也可以**抛弃**使用**re-signer**对apk进行重签名，我喜欢的操作方式则是命令行执行（不要问我为什么，我喜欢逼格高一点的）。简单飘一个我的执行命令

```
//重签名
jarsigner -keystore C:\Users\YJH\.android\debug.keystore -storepass android -keypass android D:\Android-Testing\robotium-solo-5.6.1\resign-tools\SampAndroid.apk androiddebugkey

//重新打包
zipalign 4 D:\Android-Testing\robotium-solo-5.6.1\resign-tools\SampAndroid.apk D:\Android-Testing\robotium-solo-5.6.1\resign-tools\SampAndroid-debug.apk

//安装
adb install D:\Android-Testing\robotium-solo-5.6.1\resign-tools\SampAndroid-debug.apk
```

![在这里插入图片描述](https://img-blog.csdn.net/20180929154008417?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

你是不是觉得，然后能愉快的写测试代码了？？

你错了，问题仍然会接连不断的出现。。

# 问题在线

## `<1>`Eclipse中创建Android Application Project 无法运行

右击项目，执行`Run As/Android Application`***错误回现***
![在这里插入图片描述](https://img-blog.csdn.net/20180929165313206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

错误日志是这样的
```
[2018-09-29 16:50:54 - Dex Loader] Failed to load D:\Android-Testing\sdk-test\android-sdk_r24.3.2-windows\android-sdk-windows\build-tools\28.0.3\lib\dx.jar
[2018-09-29 16:50:54 - SampAndroid] Unknown error: Unable to build: the file dx.jar was not loaded from the SDK folder!
```

**提示的错误**，就是在向我解释，ta不能够再我的sdk路径`..\build-tools\28.0.3\lib`下面找到 `dx.jar` ?!
**错误原因**，在project.properties文件中未指定buildtools版本，导致项目调用了buildtools的最高版本，但是我所使用的buildtools版本最高是25.0.0，固然是找不到的啦。
**解决方案**，在项目的**project.properties**文件中添加配置- project.properties 文件中配置
target=android-21sdk.buildtools=25.0.0
## `<2>`Eclipse中创建Android Test Project 无法运行

右击项目，执行`Run As/Android Junit Test`***错误回现***
![在这里插入图片描述](https://img-blog.csdn.net/20180929170721410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

错误日志是这样的
```
An internal error occurred during: "Launching SampTest".
org.eclipse.jdt.internal.junit.runner.IVisitsTestTrees.visitTreeEntry(Lorg/eclipse/jdt/internal/junit/runner/ITestIdentifier;ZI)V
```

这个问题目前我使用百度、google，在国内、国外网站浏览，依然不得解决！

经过我的各种尝试、反复的安装、删除。终究让我找到了解决方案，激战三天，I was the winner.
**错误原因**，由于使用的IDE版本问题导致。博客上文我就已经提早说过，我很倔强，没有使用adt-bundle的内容。使用的是我本机上早已经安装过的，用来做j2ee后台开发的eclipse。以及从安卓开发中的sdk环境和单独下载的adt包。所以让我一步步体验并验证了和解决了该问题。
**解决方案**，换IDE

我的解决方式，更换eclipse版本从
![在这里插入图片描述](https://img-blog.csdn.net/20180929172833514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

到eclipse版本
![在这里插入图片描述](https://img-blog.csdn.net/20180929172737196?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Robotium 黑盒测试

## `[1]`eclipse 编写Android Application Project

`new /project .../ Android /Android Application Project`
![在这里插入图片描述](https://img-blog.csdn.net/20180929173749337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在创建安卓项目的时候有两点需要注意，
![在这里插入图片描述](https://img-blog.csdn.net/2018092917410915?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里特别说明下第二处，对于`no.2`处的填写，要更多的依照自己已经安装下载的sdk和android api版本进行填写。否则可能会出现你意想不到的结果。

安卓的项目创建完之后，要配置引入Robotium.jar。通过Build Path 进行配置！
![在这里插入图片描述](https://img-blog.csdn.net/20180929174525642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在**Java Build Path** 目录下**Libraries目录**下，点击**Add External JARs…** 添加**Robotium.jar**

在**Java Build Path** 目录下**Libraries目录**下，点击**Add Library…** 添加**Junit4**

至此，安卓的一个普通项目创建完毕！

我是做安卓开发滴，所以写个安卓功能会很快哦！接下来简单实现一个功能，打开app进入主页，页面从上到下有三个view（TextView、Button、Button）。点击第一个Button，TextView显示一行文字；点击第二个Button，弹出Toast提示。

现画一个时序图，着重解释操作流程
![在这里插入图片描述](https://img-blog.csdn.net/2018093010220890?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		final TextView tvDisplay = (TextView) findViewById(R.id.tvDisplay);
		findViewById(R.id.btnDisplay).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				tvDisplay.setText("public class MainActivity extends Activity");
			}

		});
		
		findViewById(R.id.btnToast).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Toast.makeText(v.getContext(), "吃糖 吃糖", Toast.LENGTH_LONG).show();
			}
		});
	}
}
```

```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context="ztman.androidtest.MainActivity" >

    <TextView
        android:id="@+id/tvDisplay"
        android:layout_centerInParent="true"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/hello_world" />
    
    <Button 
        android:id="@+id/btnDisplay"
        android:layout_centerHorizontal="true"
        android:layout_below="@+id/tvDisplay"
        android:layout_marginTop="20dp"
        android:layout_width="100dp"
        android:layout_height="43dp"
        android:text="点击显示"
        android:textSize="15sp"
        android:gravity="center"
        />
    
    <Button 
        android:id="@+id/btnToast"
        android:layout_centerHorizontal="true"
        android:layout_below="@+id/btnDisplay"
        android:layout_marginTop="20dp"
        android:layout_width="150dp"
        android:layout_height="43dp"
        android:text="点击弹出Toast"
        android:textSize="15sp"
        android:gravity="center"
        />

</RelativeLayout>
```

## `[2]`eclipse 编写Android Text Project

创建**测试项目Android Text Project**，过程于创建被测试项目过程是相似的，只是在`new project`创建时选择`Android Text Project`

贴下测试代码

```
public class TestAPK extends ActivityInstrumentationTestCase2{

	private Solo solo;
	private static final String MainActivity="com.example.sampandroid.MainActivity";
	private static Class launchActivityClass;
	
	static {
		try {
			launchActivityClass=Class.forName(MainActivity);
		}catch (ClassNotFoundException e) {
			throw new RuntimeException(e);
		}
	}
	
	public TestAPK() throws ClassNotFoundException{
		super(launchActivityClass);
	}
	
	
	
	@Override
	protected void setUp() throws Exception{
		super.setUp();
		solo = new Solo(getInstrumentation(), getActivity());
		
	}
	
	public void testButton() {
		Activity act = solo.getCurrentActivity();
		int btnId = act.getResources().getIdentifier("btnDisplay", "id",act.getPackageName());
		solo.waitForView(btnId);
		solo.clickOnButton("点击显示");
		solo.clickOnButton(0);
		
//		TextView outMsg = (TextView)solo.getView(R.id.tvDisplay);
//		assertEquals("public class MainActivity extends Activity", outMsg.getText().toString());
	}
	
	
	
	@Override
	protected void tearDown() throws Exception{
		//销毁动作
		solo.finishOpenedActivities();
	}
	
}
```

## `[3]`eclipse 执行测试

### 1创建测试项目

创建测试项目的时候，下图中有两个地方需要注意：

1* no.1指的是黑盒测试选项

2* no.2指的是白盒基于源码的测试选项
![在这里插入图片描述](https://img-blog.csdn.net/20180930091513766?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2取出 debug-apk 包

被测试项目运行在手机上，长这个样子
![在这里插入图片描述](https://img-blog.csdn.net/20180930092553357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从已创建且执行过的被测试项目中取出apk，在下图的被测试项目目录中可以看到
![在这里插入图片描述](https://img-blog.csdn.net/20180930092647618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)3apk 重签名

对apk的重签名方式有两种，第一种使用的是一个jar工具叫做re-sign.jar，操作方便简单，但是我不太喜欢，而我更倾向于使用命令行执行操作，因为显得逼格很高（哈哈~）。下面的重签名方式使用的是命令行操作。

将编译打包的安卓apk放在一个目录中，使用解压缩软件打开
![在这里插入图片描述](https://img-blog.csdn.net/20180930093118453?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时，可以看到红色框框中``META-INF中保存的就是签名校验的信息。删除该目录，退出！

然后使用命令行来完成apk的重签名，利用java自带的一个签名工具—— jarsigner

通过执行jarsigner命令，你可以看到ta的使用引导
![在这里插入图片描述](https://img-blog.csdn.net/20180930093633290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们使用对apk的重签名使用的是debug签名，而该debug签名所使用的(我的电脑)keystore位置就在``C:\Users\YJH\.android\debug.keystore

执行 jarsigner 签名的命令``jarsigner -keystore [debug keystore 路径] -storepass android -keypass android [待重签名apk 路径] androiddebugkey


![在这里插入图片描述](https://img-blog.csdn.net/20180930094644124?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后再次执行 zipalign 命令，优化重签名之后的 apk包，让我们重签名之后的包运行的更有效率。

执行 zipalign 重签名之后的优化命令``zipalign 4 [重签名后的apk路径] [优化后的副本目录]


![在这里插入图片描述](https://img-blog.csdn.net/2018093009521222?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

重上面执行的命令，以及命令执行的过程，能够看到在执行命令中的一个参数"4"的作用！

到这里就对apk重签名且重打包执行完毕！接下来只需要拖拽到模拟器中就行了，或者使用adb命令安装。

但是，有时候你可能会遇到这种问题。

在执行过上面操作之后，通过adb进行命令的在线安装，但是安装失败并报错
``Failure [INSTALL_PARSE_FAILED_NO_CERTIFICATES]
报错原因， java 的jdk版本在1.8以上。java的签名工具在使用的时候，需要指定签名算法。
解决方案，  在进行重签名过程添加执行命令的红色凸显部分参数即jarsigner -keystore [debug keystore 路径] -storepass android -keypass android ``-sigalg MD5withRSA -digestalg SHA1 [待重签名apk 路径] androiddebugkey``finally 测试执行成功，效果欣赏

![在这里插入图片描述](https://img-blog.csdn.net/20180929173113328?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到手机中的apk被起，并自动执行测试！














