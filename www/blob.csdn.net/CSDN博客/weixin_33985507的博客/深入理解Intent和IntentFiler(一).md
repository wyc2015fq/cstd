# 深入理解Intent和IntentFiler(一) - weixin_33985507的博客 - CSDN博客
2016年07月23日 18:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
http://blog.csdn.net/u012637501/article/details/41080891
为了比较深刻的理解并灵活使用Intent，我计划将这部分的学习分为两步：一是深入理解Intent基本概念以及其类；二是，通过实例来阐述如何灵活使用Intent来启动一个组件以及实现组件之间的传递数据。
**一、什么是Intent,有什么作用?**
    Android的应用程序包括四大组件：Activity、contentProvider、Service、BroadcastReceiver,为了方便不同组件之间的交流通信，应用程序就采用了一种统一的方式启动组件及传递数据，即使用Intent。            
    Intent封装了Android应用程序需要启动某个组件的"意图"，Intent类的对象是组件间的通信载体，一个Intent对象就是一组信息，其包含接收Intent组件所关心的信息（如action 和 Data）和Android 系统关心的信息(如Category等)。也就是说，发送"意图"的组件通过Intent对象所包含的内容，来启动指定的(即Component属性)或通过筛选(即Action&Category属性)的某(些)组件，然后实施相应的动作（即Action属性）并传递相应的数据(即Data属性)以便完成相应的动作。
**二、Intent是如何实现组件间相互调用？**
**1.Intent实现**
![](https://img-blog.csdn.net/20141113160334234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjYzNzUwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    上图请求一个Activity组件的简单的实现流程图，算是用的最多的Intent解析实例。
   首先，发出"意图"的组件，通过调用Context.startActivity(intent)开始启动组件：发出"意图"的组件传入已经构好的Intent对象(为一组信息，它决定了是否能够成功的启动另一个组件）；
   然后，调用"动作"实际的执行着Instrumentation对象，它是整个应用激活的Activity管理者，集中负责应用内所有Activity的运行。它有一个隐藏的方法execStartActivity方法，就是负责根据Intent启动Activity。该方法去掉一些细节，它做得最重要的事情，就是将此调用，通过RPC的方式，传递到ActivityManagerService。
   最后，ActivityManagerService会将这些关键信息递交给另一个服务PackageManagerService，此服务掌握整个软件包及其各组件的信息，它会将传递过来的Intent，与已知的所有Intent Filters进行匹配（如果带有Component信息，就不用比了）。如果找到了，就把相关Component的信息通知回AcitivityManagerService，在这里，会完成启动Activity这个很多细节的事情。  
**2.Intent匹配**
    到底发出"意图"的组件是如何找到所需的组件呢？在这里，Intent Filters就开始起作用了，Intent Filters定义在AndroidMainFest.xml文件中，每一个Activity都会有一个<Intent Filters/>元素，它包含了<action/>、<data/>等子元素。当我们的intent对象没有包含Component信息时，这种"意图"被称之为隐形"意图"。也就是说，"意图"没有指明具体要启动哪个组件以及完成什么样的动作。这时我们就需要通过Intent Filters中的子元素进行信息匹配，从而确定当前包含Intent Filters属性的Activity是不是我们要启动的那个(些)组件。即发送"意图"组件配置好intent对象，被启动组件实现Intent Filters属性，最后，发送组件会根据被启动组件AndroidMainFest.xml中的<Intent Filters/>信息来确定它是不是目标组件。
![](https://img-blog.csdn.net/20141113160328229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjYzNzUwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**三、Intent对象详解**
  Intent类的对象是组件间的通信载体，利用Intent对象我们可以很方便的实现不同组件之间的通信。一个Intent对象就是一组信息，这些信息都是通过其Component、Action、Category、Data、Extra和Flag这7种属性体现的。Intent代表了Android应用的启动"意图"，Android应用将会根据Intent来启动指定组件，至于到底启动哪个组件，则取决于Intent的属性。 
**1.Action属性**
    Action属性为一个普通的字符串，它代表了该Intent对象要完成一个什么样的"动作"。当我们为Intent对象指明了一个action时，被启动的组件就会依照这个动作的指示表现出相应的行为，比如查看、拨打、编辑等，需要注意的是一个组件(如Activity)只能有一个action。我们可以方便自己的应用程序组件之间的通信，自定义action的(字符串)创建新的动作；也可以直接使用Intent类中的静态成员变量，比如ACTION_VIEW，ACTION_PICK，它们是Android中为action属性预定义的一批action变量。
在设置Intent对象Action属性时，有两种：
**[java]**[view plain](http://blog.csdn.net/u012637501/article/details/41080891)[copy](http://blog.csdn.net/u012637501/article/details/41080891)
[print](http://blog.csdn.net/u012637501/article/details/41080891)[?](http://blog.csdn.net/u012637501/article/details/41080891)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/517846/fork)
- (1)自定义字符串  
- public final String CUSTOME_ACTION="intent.action.CUSTOME_JIANG";//字符串可以任意  
- Intent intent=new Intent();  
- intent.setAction(ActionAttr.CUSTOME_ACTION); //注意：ActionAttr为我们创建的类，也可以使用this.CUSTOME_ACTION  
- (2)使用系统预定action常量  
- Intent intent=new Intent();  
- intent.setAction(Intent.ACTION_CALL);    //其中ACTION_CALL为Intent类的静态成员变量，可以类直接调用  
- //对应字符串"android.intent.action.CALL"  
**2.Data属性**
   Action属性为Intent对象描述了一个"动作"，那么Data属性就为Intent对象的Action属性提供了操作的数据。这里需要注意的是，Data属性只接受一个Uri对象，一个Uri对象通常通过如下形式的字符串来表示：
Uri字符串格式：scheme://host:port/path 举例： content://com.android.contacts/contacts/1或tel://18819463209
在设置Intent对象Data属性时可以这样：
**[java]**[view plain](http://blog.csdn.net/u012637501/article/details/41080891)[copy](http://blog.csdn.net/u012637501/article/details/41080891)
[print](http://blog.csdn.net/u012637501/article/details/41080891)[?](http://blog.csdn.net/u012637501/article/details/41080891)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/517846/fork)
- Intent intent=new Intent();  
- String data="content://com.android.contacts/contacts/1";  
- Uri uri=Uri.parse(data);//将字符串转换为Uri  
- intent.setData(uri);  
- 或者  
- Intent intent=new Intent();  
- intent.setData(Uri.parse("content://com.android.contacts/contacts/1"));  
 博主笔记1：action属性和data属性为Intent所传递信息的主要部分，action/data属性举例：
ACTION_VIEW content://contacts/people/1 -- 传递的信息： 显示 号码为1的人相关信息
ACTION_DIAL content://contacts/people/1 -- 传递的信息：给编号为1的人 打 电话
ACTION_VIEW tel:123 -- 传递的信息：将号码123 显示
ACTION_DIAL tel:123 --传递的信息：  拨打 号码123
ACTION_EDIT content://contacts/people/1 -- 传递的信息： 编辑编号为1的联系人
ACTION_VIEW content://contacts/people/ -- 传递的信息：列出显示所有联系人.如果希望在查看某个联系人，需要定义一个新的intent并且属性设置为{ ACTION_VIEW content://contacts/N } 传递到一个新的Activity。
总结：action属性、data属性是intent的主要属性。
**3.Catagory属性**
    通过Action，配合Data或Type属性可以准确的表达出一个完整的意图了。但为了使的"意图"更加精确，我们也给意图添加一些约束，这个约束由"意图"的Catagory属性实现。一个意图只能指定一个action属性，但是可以添加一个或多个Catagory属性。Category属性可以自定义字符串实现，但为了方便不同应用之间的通信还可以设置系统预定义的Category常量。调用方法addCategory 用来为Intent 添加一个Category，方法removeCategory 用来移除一个Category；getCategories方法返回已定义的Category。
在设置Intent对象Categoty属性时可以这样：
**[java]**[view plain](http://blog.csdn.net/u012637501/article/details/41080891)[copy](http://blog.csdn.net/u012637501/article/details/41080891)
[print](http://blog.csdn.net/u012637501/article/details/41080891)[?](http://blog.csdn.net/u012637501/article/details/41080891)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/517846/fork)
- (1)自定义字符串  
- public final String CUSTOME_CATEGORY="intent.action.CUSTOME_CATEGORY";//字符串可以任意  
- Intent intent=new Intent();  
- intent.addCategory(ActionAttr.CUSTOME_CATEGORY);   
- (2)使用系统预定action、category常量  
-     以下代码实现当点击某个按钮时，通过Intent对象实现返回HOME桌面。  
- Intent intent=new Intent();  
- intent.setAction(Intent.ACTION_MAIN);  
- intent.addCategory(Intent.CATEGORY_HOME);//返回Home桌面   
博主笔记2：一般来说Action属性和Category属性都是同时使用的。在Android中，所有应用主Activity（就是单独启动时候，第一个运行的那个Activity...），都需要能够接受一个Category为 CATEGORY_LAUNCHER，Action为ACTION_Main的意图。对于发出"意图"的组件，我们可以通过setAction()、addCategory()方法为"意图"添加action属性或者同时添加Action、Category属性;对于接收"意图"的组件，在AndroidManifest.xm文件中，我们可以这样设置：
<application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name=".ActionCateAttr"
            android:label="第一个Activity界面" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />            //默认Action
                <category android:name="android.intent.category.LAUNCHER" />    //默认Category
            </intent-filter>
        </activity>
        <activity
            android:name=".SecondaryActivity"
            android:label="第二个Activity界面" >
            <intent-filter>
                <action android:name="intent.action.JIANG_ACTION" />
                <category android:name="intent.action.JIANG_CATEGORY" />
                <category android:name="android.intent.category.DEFAULT" />    //默认Category
            </intent-filter>
        </activity>
注释：发出"意图"的Activity将Category为 CATEGORY_LAUNCHER，Action为ACTION_Main,接收"意图"的Activity须设置一个默认的Category属性CATEGORY_DEFAULT,这里不能将其设置为CATEGORY_LAUNCHER否者会报错。另外，如果我们使用了系统预定义的action常量，则需要在AndroidManifest.xm文件中添加相应的权限，这方面的内容我们将在第二部分讲到。
**4.Type属性**
   Type属性用于指定该Data所指定Uri对应的MIME类型，这种类型可以是任何自定义的MIME类型，只要符合abc/xyz格式的字符串即可。这里需要注意的是，Type属性和Data属性一般会出现相互覆盖的情况，如果希望Intent既有Data属性也有Type属性，必须通过setDataAndType()方法来实现。对于Type属性的理解，我记得有篇博文是这样作比方的：说了Data，就必须要提Type，很多时候，会有人误解，觉着Data和Type的差别，就犹如泡妞和泡马子之间的差别一样，微乎其微。但其实不然，Type信息，是用MIME来表示的，比如text/plain，这样的东西。说到这里，两者差别就很清晰了，Data就是门牌号，指明了具体的位置，具体问题具体分析，而type，则是强调物以类聚，解决一批量的问题。实际的例子是这样的，比如，从某个应用拨打一个电话，会发起的是action为ACTION_DIAL且data为tel:xxx这样的Intent，对应的人类语言就是拨打xxx的电话，很具象。而如果使用type，就宽泛了许多，比如浏览器收到一个未知的MIME类型的数据（比如一个视频...），就会放出这样的Intent，求系统的其他应用来帮助，表达成自然语言应该就是：查看pdf类文档，这样的。
博主笔记3：MIME类型？
    MIME(Multipurpose Internet Mail Extensions)多用途互联网邮件扩展类型就是设定某种扩展名的文件用一种应用程序来打开的方式类型，当该扩展名文件被访问的时候，浏览器会自动使用指定应用程序来打开。多用于指定一些客户端自定义的文件名，以及一些媒体文件打开方式。最早的HTTP协议中，并没有附加的数据类型信息，所有传送的数据都被客户程序解释为超文本标记语言HTML 文档，而为了支持多媒体数据类型，HTTP协议中就使用了附加在文档之前的MIME数据类型信息来标识数据类型。MIME意为多功能Internet邮件扩展，它设计的最初目的是为了在发送电子邮件时附加多媒体数据，让邮件客户程序能根据其类型进行处理。然而当它被HTTP协议支持之后，它的意义就更为显著了。它使得HTTP传输的不仅是普通的文本，而变得丰富多彩。每个MIME类型由两部分组成，前面是数据的大类别，例如声音audio、图象image等，后面定义具体的种类。
常见的MIME类型(通用型)：
超文本标记语言文本 .html text/html
xml文档 .xml text/xml
XHTML文档 .xhtml application/xhtml+xml
普通文本 .txt text/plain
RTF文本 .rtf application/rtf
PDF文档 .pdf application/pdf
Microsoft Word文件 .word application/msword
PNG图像 .png image/png
GIF图形 .gif image/gif
JPEG图形 .jpeg,.jpg image/jpeg
au声音文件 .au audio/basic
MIDI音乐文件 mid,.midi audio/midi,audio/x-midi
RealAudio音乐文件 .ra, .ram audio/x-pn-realaudio
MPEG文件 .mpg,.mpeg video/mpeg
AVI文件 .avi video/x-msvideo
GZIP文件 .gz application/x-gzip
TAR文件 .tar application/x-tar
任意的二进制数据 application/octet-stream
**5.Ertras属性**
  通过上面的这些项，识别问题，基本完美解决了，剩下一个重要的问题，就是传参。Extras是用来做这个事情的，它是一个Bundle 类的对象，有一组可序列化的key/value对组成。每一个Action都会有与之对应的key和value类型约定，发起Intent的时候，需要按照要求把Data不能表示的额外参数放入Extras中。
**6.Flags属性**
   能识别，有输入，整个Intent基本就完整了，但还有一些附件的指令，需要放在Flags中带过去。顾名思义，Flags是一个整形数，有一些列的标志 位构成，这些标志，是用来指明运行模式的。比如，你期望这个意图的执行者，和你运行在两个完全不同的任务中（或说进程也无妨吧...），就需要设置FLAG_ACTIVITY_NEW_TASK 的标志位。
**7.Component属性**
    通常来说，"意图"可分为显示intent和隐式intent。Intent Filters它是用来描述一个Activity或 Serveice等组件，我们通过在组件AndroidManifest.xml中<intent-ilters/>元素中添加<action/>等属性，以满足期望能够响应怎么样的Intent，这种没有指明要启动组件名方式就称之为隐式intent。当然，我们也可以使"意图"实现启动指定的组件，即称之为显示intent,主要通过Component属性来实现。Intent的Component属性需要接受一个ComponentName对象，这个对实现将要启动指定组件的类名、类所在的包名绑定在intent上。
**[java]**[view plain](http://blog.csdn.net/u012637501/article/details/41080891)[copy](http://blog.csdn.net/u012637501/article/details/41080891)
[print](http://blog.csdn.net/u012637501/article/details/41080891)[?](http://blog.csdn.net/u012637501/article/details/41080891)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/517846/fork)
- ComponentName comp=new ComponentName(ComponentAttr.this,SecondaryActivity.class);   
- Intent intent=new Intent();  
- intent.setComponent( comp);//设置intent的Component属性，指定"意图"要启动组件的包和类名  
- 注释：第一句用于创建一个ComponentName对象，来指定包名和类型-这就可以唯一地确定一个组件类。  
**四、Intent相关类**
**1.Activity类**
   这里我们只需学习使用Intent启动Activity组件将要用的的方法
|void|startActivity(Intent intent)作用：启动Activity,具体启动哪个Activity和怎么样启动由intent属性决定|
|----|----|
|void|startActivityForResult(Intent intent, int requestCode)作用：启动Activity，并返回一个结果。当被启动的Activity退出时，会调用 onActivityResult() 方法并向其传入一个 requestCode参数，这个 requestCode参数为非负数(>0)，作用是标志是哪个Activity组件发出的"意图",需要注意的是如果 requestCode小于0时，这个方法的只能用于启动一个Activity而不能返回值了。另外，Intent的action属性设为能够返回一个结果，如果设置为 `[Intent.ACTION_MAIN](http://www.cnblogs.com/)` or `[Intent.ACTION_VIEW](http://www.cnblogs.com/)，也是不能获取结果的。`|
待写：另外还有如何启动Service、BroadcastReceiver组件的方法，这以后学到了再说吧。
**2.Intent类**
(1)构造函数
||[Intent](http://developer.android.com/reference/android/content/Intent.html#Intent(android.content.Intent))()：创建一个空的构造函数      [Intent](http://developer.android.com/reference/android/content/Intent.html#Intent(android.content.Intent))([Intent](http://developer.android.com/reference/android/content/Intent.html) o)     ：拷贝构造函数|
|----|----|
||[Intent](http://developer.android.com/reference/android/content/Intent.html#Intent(java.lang.String))([String](http://developer.android.com/reference/java/lang/String.html) action)  ：创建一个具有acion属性的意图对象|
||[Intent](http://developer.android.com/reference/android/content/Intent.html#Intent(java.lang.String,%20android.net.Uri))([String](http://developer.android.com/reference/java/lang/String.html) action, [Uri](http://developer.android.com/reference/android/net/Uri.html) uri)：创建一个带action属性的意图，接受一个Uri对象|
||[Intent](http://developer.android.com/reference/android/content/Intent.html#Intent(android.content.Context,%20java.lang.Class)([Context](http://developer.android.com/reference/android/content/Context.html) packageContext, [Class](http://developer.android.com/reference/java/lang/Class.html)<?> cls)：创建一个已经指定组件的意图|
||[Intent](http://developer.android.com/reference/android/content/Intent.html#Intent(java.lang.String,%20android.net.Uri,%20android.content.Context,%20java.lang.Class)([String](http://developer.android.com/reference/java/lang/String.html) action, [Uri](http://developer.android.com/reference/android/net/Uri.html) uri, [Context](http://developer.android.com/reference/android/content/Context.html) packageContext, [Class](http://developer.android.com/reference/java/lang/Class.html)<?> cls)为一个指定的组件创建一个带action和data属性的意图|
