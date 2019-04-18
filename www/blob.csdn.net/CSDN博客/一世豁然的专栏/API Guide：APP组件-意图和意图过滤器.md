# API Guide：APP组件-意图和意图过滤器 - 一世豁然的专栏 - CSDN博客





2017年04月18日 16:25:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1855








注意：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。





意图是一个消息对象，您可以使用它从其他应用程序组件请求操作。 虽然意图通过几种方式促进组件之间的通信，但有三种基本用途：

1、启动一个活动：

活动表示应用程序中的单个屏幕。 您可以通过将Intent传递给startActivity（）来启动活动的新实例。 意图描述启动的活动并携带任何必要的数据。





如果要在活动结束后收到结果，请调用startActivityForResult（）。在活动的onActivityResult() 回调函数中，你的活动作为一个分离的Intent对象来接收结果。有关更多信息，请参阅活动指南。





2、启动一个服务：

服务是在没有用户界面的情况下在后台执行操作的组件。服务是在没有用户界面的情况下在后台执行操作的组件。 您可以通过将Intent传递给startService（）来启动服务以执行一次性操作（例如下载文件）。 意图描述启动的服务并携带任何必要的数据。





如果服务是使用客户端 - 服务器接口设计的，则可以通过将Intent传递给bindService（）来绑定到另一个组件的服务。 有关更多信息，请参阅服务指南。





3、传送一个广播：

广播是任何应用可以接收的消息。 系统为系统事件提供各种广播，例如系统启动或设备开始充电时。 通过将Intent传递给sendBroadcast（），sendOrderedBroadcast（）或sendStickyBroadcast（）），您可以向其他应用程序发送广播。








一、意图类型

有两种类型的意图：


I、显式意图通过名称指定组件（完全限定类名称）来启动。 您通常会使用明确的意图来启动自己的应用程序中的组件，因为您知道要启动的活动或服务的类名称。 例如，响应于用户操作启动新活动或启动服务以在后台下载文件。

II、隐含意图不指定特定的组件，而是声明要执行的一般操作，这允许来自另一个应用程序的组件来处理它。 例如，如果要向用户显示地图上的位置，则可以使用隐含意图来请求另一个有能力的应用程序在地图上显示指定的位置。




当您创建明确的启动活动或服务的意图时，系统将立即启动在Intent对象中指定的应用程序组件。





当您创建隐含意图时，Android系统通过将intent的内容与设备上其他应用程序的清单文件中声明的intent过滤器进行比较来找到适当的组件。 如果意图匹配意图过滤器，则系统启动该组件并将其传递给Intent对象。 如果多个意图过滤器兼容，系统将显示一个对话框，以便用户可以选择要使用的应用程序。





意图过滤器是应用程序的清单文件中的表达式，用于指定组件想要接收的意图类型。 例如，通过为活动声明意图过滤器，您可以使其他应用程序以某种意图直接启动您的活动。 同样，如果您没有为活动声明任何意图过滤器，则只能以明确的意图启动它。





注意：为确保您的应用程序安全，始终在启动服务时使用明确的意图，并且不要为服务声明意图过滤器。 使用隐含的意图启动服务是一个安全隐患，因为您无法确定什么服务将响应意图，并且用户无法查看哪个服务启动。 从Android 5.0（API级别21）开始，如果您使用隐式意图调用bindService（），系统将抛出异常。





![](https://img-blog.csdn.net/20170418140652716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


隐含意图通过系统传递以启动另一个活动的示例：


1、活动A创建具有动作描述的Intent，并将其传递给startActivity（）

2、Android系统搜索所有应用程序的意图过滤器匹配意图。

3、系统通过调用其onCreate（）方法并传递Intent来启动匹配活动（活动B）。







二、建立意图

Intent对象携带Android系统用于确定要启动的组件（例如应该接收到意图的确切组件名称或组件类别）的信息，以及收件人组件为正确执行操作而使用的信息（例如 采取的行动和数据采取行动）。





意图中包含的主要信息如下：


1、组件名（Component name）

要启动的组件的名称。





这是可选的，但它是使意图明确的关键信息，这意味着意图应仅传递给由组件名称定义的应用程序组件。 没有组件名称，意图是隐式的，系统根据其他意图信息（如下面描述的操作，数据和类别）决定哪个组件应该接收到意图。 因此，如果您需要在应用程序中启动特定组件，则应指定组件名称。





注意：启动服务时，应始终指定组件名称。 否则，您不能确定什么服务将对意图做出响应，并且用户无法看到哪个服务启动。





Intent的此字段是ComponentName对象，您可以使用目标组件的完全限定类名（包括应用程序包的名称）指定。 例如，com.example.ExampleActivity。 您可以使用setComponent（），setClass（），setClassName（）或Intent构造函数设置组件名称。





2、动作（Action）

指定要执行的通用操作（如查看或选择）的字符串。





在广播意图的情况下，这是发生和正在报告的动作。 这个行动在很大程度上决定了意图的其余部分是如何构成的 - 特别是数据和附加内容。





您可以指定自己应用程序中的意图（或由其他应用程序用于调用应用程序中的组件）使用的操作，但是您通常应该使用由Intent类或其他框架类定义的操作常量。 以下是开始活动的常见操作：


ACTION_VIEW


当您有一些活动可以向用户显示的信息（例如要在图库应用程序中查看的照片）或要在地图应用中查看的地址时，以startActivity（）的意图使用此操作。





ACTION_SEND


也称为“共享”意图，当您有一些用户可以通过其他应用程序（如电子邮件应用程序或社交共享应用程序）共享的数据时，您应该使用startActivity（）的意图。





有关定义通用操作的更多常量，请参阅Intent类引用。 其他操作也在Android框架的其他地方进行了定义，例如在系统“设置”应用程序中打开特定屏幕的“设置”操作。





您可以使用setAction（）或Intent构造函数为intent创建动作。





如果您定义自己的操作，请确保将应用程序的包名称作为前缀。 例如：




```java
static final String ACTION_TIMETRAVEL = "com.example.action.TIMETRAVEL";
```


3、数据（Data）



引用要处理的数据的URI（Uri对象）和/或该数据的MIME类型。 提供的数据类型通常由意图的动作决定。 例如，如果操作是ACTION_EDIT，则数据应包含要编辑的文档的URI。





创建意图时，除了URI之外，还要指定数据类型（其MIME类型）。 例如，尽管URI格式可能相似，但是能够显示图像的活动可能无法播放音频文件。 因此，指定您的数据的MIME类型有助于Android系统找到最好的组件来接收您的意图。 但是，有时可以从URI推断MIME类型，特别是当数据是内容：URI时，表示数据位于设备上并由ContentProvider控制，这使得数据MIME类型对系统可见。





仅仅要设置数据URI，请调用setData（）。 要设置MIME类型，请调用setType（）。 如果需要，您可以使用setDataAndType（）显式设置。





注意：如果要同时设置URI和MIME类型，请不要调用setData（）和setType（），因为它们都会将其他值的值都取消。 始终使用setDataAndType（）来设置URI和MIME类型。





4、类别（Category）

包含有关应该处理意图的组件种类的附加信息的字符串。 任何数量的类别描述都可以放在意图中，但大多数意图不需要类别。 以下是一些常见的类别：


CATEGORY_BROWSABLE


目标活动允许自己由Web浏览器启动，以显示由链接（例如图像或电子邮件）引用的数据。





CATEGORY_LAUNCHER


该活动是任务的初始活动，并列在系统的应用程序启动器中。





有关类别的完整列表，请参阅Intent类描述。





您可以使用addCategory（）指定类别。





上面列出的这些属性（组件名称，操作，数据和类别）表示意图的定义特征。 通过阅读这些属性，Android系统能够解析应该开始哪个应用程序组件。





但是，意图可以携带不影响应用程序组件解决方式的其他信息。 意图也可以提供：


1、额外（Extras）

键值对，带有完成请求的操作所需的附加信息。 正如一些操作使用特定类型的数据URI一样，一些操作也使用特定的附加功能。





您可以使用各种putExtra（）方法添加额外的数据，每个都接受两个参数：键名称和值。 您还可以使用所有额外的数据创建一个Bundle对象，然后使用putExtras（）将Bundle插入到Intent中。





例如，当创建用ACTION_SEND发送电子邮件的意图时，可以使用EXTRA_EMAIL键指定“到”收件人，并使用EXTRA_SUBJECT键指定“主题”。





Intent类为标准化数据类型指定了许多EXTRA_ *常量。 如果您需要声明自己的额外键（对于您的应用程序收到的意图），请确保将应用程序的程序包名称作为前缀。 例如：




```java
static final String EXTRA_GIGAWATTS = "com.example.EXTRA_GIGAWATTS";
```


2、标志（Flags）



在Intent类中定义的标志用作意图的元数据。 标志可以指示Android系统如何启动活动（例如，活动应该属于哪个任务）以及在启动后如何对待它（例如，它是否属于最近活动的列表）。





有关更多信息，请参阅setFlags（）方法。





一）、明确意图示例

明确的意图是您用于启动特定应用程序组件的意图，例如应用程序中的特定活动或服务。 要创建明确的意图，定义Intent对象的组件名称 - 所有其他intent属性都是可选的。





例如，如果您在应用程序中构建名为DownloadService的服务，旨在从Web下载文件，则可以使用以下代码启动它：




```java
// Executed in an Activity, so 'this' is the Context
// The fileUrl is a string URL, such as "http://www.example.com/image.png"
Intent downloadIntent = new Intent(this, DownloadService.class);
downloadIntent.setData(Uri.parse(fileUrl));
startService(downloadIntent);
```


Intent（Context，Class）构造函数为应用程序上下文和组件提供了一个Class对象。 因此，此意图明确地启动了应用程序中的DownloadService类。






有关构建和启动服务的更多信息，请参阅“服务指南”。





二）、隐含意图示例

隐式意图指定可以调用设备上能够执行该操作的任何应用程序的操作。 当您的应用程序无法执行操作时，使用隐含意图非常有用，但其他应用程序可能可以，并且你希望用户选择要使用的应用程序。





例如，如果您有要使用户与其他人共享的内容，请使用ACTION_SEND操作创建意图，并添加指定要共享的内容的附加内容。 当您以此意图调用startActivity（）时，用户可以选择一个应用程序来共享内容。





注意：用户可能没有任何处理您发送到startActivity（）的隐含意图的应用程序。 如果发生这种情况，呼叫将失败，您的应用程序将崩溃。 要验证活动是否接收到意图，请在Intent对象上调用resolveActivity（）。 如果结果为非空值，那么至少有一个应用程序可以处理该意图，并且可以安全地调用startActivity（）。 如果结果为空，则不应使用该意图，如果可能，您应该禁用发出意图的功能。







```java
// Create the text message with a string
Intent sendIntent = new Intent();
sendIntent.setAction(Intent.ACTION_SEND);
sendIntent.putExtra(Intent.EXTRA_TEXT, textMessage);
sendIntent.setType("text/plain");

// Verify that the intent will resolve to an activity
if (sendIntent.resolveActivity(getPackageManager()) != null) {
    startActivity(sendIntent);
}
```


注意：在这种情况下，不使用URI，但声明了意图的数据类型以指定附加内容。






当调用startActivity（）时，系统将检查所有已安装的应用程序，以确定哪些应用程序可以处理这种意图（具有ACTION_SEND操作的意图，并带有“text / plain”数据）。 如果只有一个应用程序可以处理它，该应用程序立即打开并被赋予意图。 如果多个活动接受意图，系统将显示一个对话框，以便用户可以选择要使用的应用程序。




三）、强制应用程序选择器

当有多个应用程序响应您的隐含意图时，用户可以选择要使用的应用程序，并使该应用程序成为该操作的默认选择。 当执行一个操作时，用户可能希望使用相同的应用程序，例如当打开网页时（用户通常只喜欢一个Web浏览器）。





但是，如果多个应用程序可以响应意图，并且用户可能希望每次都使用其他应用程序，则应显式显示选择器对话框。 选择器对话框要求用户每次选择要用于该操作的应用程序（用户无法为该操作选择默认应用程序）。 例如，当您的应用程序使用ACTION_SEND操作执行“共享”时，用户可能希望根据当前情况使用其他应用程序进行共享，因此您应该始终使用选择器对话框，如图所示。


![](https://img-blog.csdn.net/20170418151032412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





要显示选择器，使用createChooser（）创建一个Intent并将其传递给startActivity（）。 例如：




```java
Intent sendIntent = new Intent(Intent.ACTION_SEND);
...

// Always use string resources for UI text.
// This says something like "Share this photo with"
String title = getResources().getString(R.string.chooser_title);
// Create intent to show the chooser dialog
Intent chooser = Intent.createChooser(sendIntent, title);

// Verify the original intent will resolve to at least one activity
if (sendIntent.resolveActivity(getPackageManager()) != null) {
    startActivity(chooser);
}
```


这将显示一个对话框，其中包含响应传递给createChooser（）方法的意图的应用程序列表，并使用提供的文本作为对话框标题。






三、接收隐含意图

要宣传您的应用程序可以接收的隐含意图，请使用清单文件中的<intent-filter>元素为每个应用程序组件声明一个或多个意图过滤器。 每个意图过滤器根据意图的操作，数据和类别来指定它接受的意图的类型。 只有当意图可以通过您的一个意图过滤器时，系统才会向您的应用程序组件传递隐含的意图。





注意：无论任何意图过滤组件声明，显式意图始终都会传递给其目标。





应用程序组件应为其可以执行的每个唯一作业声明单独的过滤器。 例如，图片库应用程序中的一个活动可能有两个过滤器：一个用于查看图像的过滤器，另一个用于编辑图像的过滤器。 当活动开始时，它检查意图，并根据意图中的信息决定如何行为（例如显示编辑器控件）。





每个意图过滤器由应用程序的清单文件中的一个<intent-filter>元素定义，嵌套在相应的应用程序组件（例如<activity>元素）中。 在<intent-filter>内，您可以使用以下三个元素中的一个或多个来指定要接受的意图类型：


<action>

在name属性中声明接受的意图操作。 该值必须是操作的文字字符串值，而不是类常量。


<data>

声明接受的数据类型，使用指定数据URI（方案，主机，端口，路径等）和MIME类型的各个方面的一个或多个属性。


<category>

在name属性中声明接收的意图类别。 该值必须是操作的文字字符串值，而不是类常量。





注意：为了接收隐含意图，您必须在意图过滤器中包含CATEGORY_DEFAULT类别。 方法startActivity（）和startActivityForResult（）处理所有意图，就像它们宣布了CATEGORY_DEFAULT类别一样。 如果您没有在意图过滤器中声明此类别，则不会有隐式意图为您的活动进行解析。





例如，当数据类型为文本时，这是一个具有意图过滤器的活动声明，以接收ACTION_SEND意图：




```java
<activity android:name="ShareActivity">
    <intent-filter>
        <action android:name="android.intent.action.SEND"/>
        <category android:name="android.intent.category.DEFAULT"/>
        <data android:mimeType="text/plain"/>
    </intent-filter>
</activity>
```


创建一个包含多个<action>，<data>或<category>的实例的过滤器是可行的。 如果你这样做，你只需要确定该组件可以处理任何与这些过滤元件的所有组合。






当您想要处理多种意图时，但是仅仅是在操作，数据和类别类型的特定组合中，则需要创建多个意图过滤器。





通过将意图与三个元素中的每个元素进行比较，则可以针对过滤器测试隐含意图。要传递给组件，意图必须通过所有三个测试。 如果无法匹配其中的一个，Android系统将不会将意图传递给组件。 但是，由于组件可能具有多个意图过滤器，因此未通过组件过滤器之一的意图可能会使其在另一个过滤器上通过。 有关系统解决意图的更多信息，请参见以下有关意向解决的部分。





注意：为避免无意中运行不同的应用程序的服务，请始终使用明确的意图来启动您自己的服务，并且不要为服务声明意图过滤器。





注意：对于所有活动，您必须在清单文件中声明您的意图过滤器。 然而，可以通过调用registerReceiver（）动态注册广播接收器的过滤器。 然后，您可以使用unregisterReceiver（）取消注册接收器。 这样做允许您的应用程序在您的应用程序运行期间在指定的时间段内侦听特定的广播。





一）、过滤器例子

要更好地了解一些意图过滤器行为，请查看社交共享应用程序的清单文件中的以下代码段。




```
<activity android:name="MainActivity">
    <!-- This activity is the main entry, should appear in app launcher -->
    <intent-filter>
        <action android:name="android.intent.action.MAIN" />
        <category android:name="android.intent.category.LAUNCHER" />
    </intent-filter>
</activity>

<activity android:name="ShareActivity">
    <!-- This activity handles "SEND" actions with text data -->
    <intent-filter>
        <action android:name="android.intent.action.SEND"/>
        <category android:name="android.intent.category.DEFAULT"/>
        <data android:mimeType="text/plain"/>
    </intent-filter>
    <!-- This activity also handles "SEND" and "SEND_MULTIPLE" with media data -->
    <intent-filter>
        <action android:name="android.intent.action.SEND"/>
        <action android:name="android.intent.action.SEND_MULTIPLE"/>
        <category android:name="android.intent.category.DEFAULT"/>
        <data android:mimeType="application/vnd.google.panorama360+jpg"/>
        <data android:mimeType="image/*"/>
        <data android:mimeType="video/*"/>
    </intent-filter>
</activity>
```


第一个活动MainActivity是应用程序的主要入口点 - 用户最初使用启动器图标启动应用程序时打开的活动：



I、ACTION_MAIN操作指示这是主入口点，并不期望任何意图数据。

II、CATEGORY_LAUNCHER类别表示此活动的图标应放置在系统的应用启动器中。 如果<activity>元素未指定带有图标的图标，则系统将使用<application>元素中的图标。




这两个必须配对在一起，以便活动显示在应用程序启动器中。





第二个活动ShareActivity旨在促进文本和媒体内容的共享。 虽然用户可以通过从MainActivity导航到这个活动，但是他们也可以从另一个应用程序直接进入ShareActivity，该应用程序会发出与两个意图过滤器之一匹配的隐含意图。





注意：MIME类型，application / vnd.google.panorama360 + jpg是一种特殊的数据类型，可以指定全景照片，您可以使用Google全景API








四、使用等待意图

PendingIntent对象是Intent对象周围的包装器。 PendingIntent的主要目的是向外部应用程序授予使用所包含的Intent的权限，就像从应用程序自己的进程中执行的一样。





等待意图的主要用法包括：

I、声明当用户使用通知（Android系统的NotificationManager执行Intent）执行操作时执行的意图。

II、声明当用户使用您的App Widget执行操作（主屏幕应用程序执行Intent）时执行的意图。

III、声明将来在特定时间执行的意图（Android系统的AlarmManager执行Intent）。




因为每个Intent对象都被设计为由特定类型的应用程序组件（活动，服务或BroadcastReceiver）处理，所以PendingIntent也必须以相同的考虑来创建。 当使用等待的意图时，您的应用程序将不会通过诸如startActivity（）的调用来执行意图。 您必须通过调用相应的创建者方法创建PendingIntent来声明所需的组件类型：


I、启动Intent活动的PendingIntent.getActivity（）。

II、启动Intent服务的PendingIntent.getService（）。

III、启动Intent广播接收器的PendingIntent.getBroadcast（）。




除非您的应用程序正在接收来自其他应用程序的待处理内容，否则创建PendingIntent的上述方法是您可能需要的唯一PendingIntent方法。





每个方法都使用当前应用程序上下文，要包装的意图，以及一个或多个指定如何使用意图的标志（例如，该意图是否可以多次使用）。





有关使用待处理意图的更多信息，请参阅各个用例的文档，例如“通知和app widget API”指南。








五、意图决议

当系统收到启动活动的隐含意图时，它将通过将意图与意图过滤器进行比较，从而基于三个方面来搜索意图的最佳活动：


I、意图动作

II、意图数据（URI和数据类型）

III、意图类别




以下部分描述了如何在应用程序的清单文件中声明意图过滤器的方式将意图与适当的组件进行匹配。





一）、动作测试（action test）

要指定接受的意图操作，intent过滤器可以声明零个或多个<action>元素。 例如：



<intent-filter><actionandroid:name="android.intent.action.EDIT"/><actionandroid:name="android.intent.action.VIEW"/>
    ...
</intent-filter>



要通过此过滤器，Intent中指定的操作必须匹配过滤器中列出的其中一个操作。





如果过滤器没有列出任何操作，则没有任何意图匹配，因此所有意图都将失败。 但是，如果Intent没有指定动作，它将通过测试（只要过滤器至少包含一个动作）。





二）类别测试（category test）

要指定接受的意图类别，意图过滤器可以声明零个或多个<category>元素。 例如：




```
<intent-filter>
    <category android:name="android.intent.category.DEFAULT" />
    <category android:name="android.intent.category.BROWSABLE" />
    ...
</intent-filter>
```


为了通过类别测试，意图中的每个类别都必须匹配过滤器中的类别。意图过滤器可能定义比指定在意图中的更多类别，并且将仍然通过。因此，不考虑在过滤器中声明哪些类别，无任何类别的意图应始终通过此测试。






注意：Android会自动将CATEGORY_DEFAULT类别应用于传递给startActivity（）和startActivityForResult（）的所有隐式意图。 所以如果你希望你的活动接收到隐含意图，它必须在其意图过滤器中包含一个“android.intent.category.DEFAULT”类别（如前面的<intent-filter>示例所示）。





三）、数据测试<Data test>

要指定接受的意图数据，意图过滤器可以声明零个或多个<data>元素。 例如：




```
<intent-filter>
    <data android:mimeType="video/mpeg" android:scheme="http" ... />
    <data android:mimeType="audio/mpeg" android:scheme="http" ... />
    ...
</intent-filter>
```


每个<data>元素可以指定URI结构和数据类型（MIME媒体类型）。 URI的每个部分都有独立的属性 - 方案，主机，端口和路径 ：



<scheme>://<host>:<port>/<path>


例如：

content://com.example.project:200/folder/subfolder/etc





在这个URI中，方案是内容，主机是com.example.project，端口是200，路径是folder
 /子文件夹/ etc。





这些属性中的每一个在<data>元素中是可选的，但是有线性依赖关系：


I、如果未指定方案，则忽略主机。

II、如果未指定主机，则该端口将被忽略。

III、如果方案和主机都未指定，路径将被忽略。




当意图中的URI与过滤器中的URI规范进行比较时，它仅与过滤器中包含的URI的部分进行比较。
 例如：


I、如果一个过滤器只指定一个方案，则该方案的所有URI都与过滤器匹配。

II、如果一个过滤器指定一个方案和一个权限，但没有路径，那么具有相同方案和权限的所有URI都将通过过滤器，而不管其路径如何。

III、如果过滤器指定方案，权限和路径，则只有具有相同方案，权限和路径的URI才会传递过滤器。




注意：路径规范可以包含通配符星号（*），仅需要路径名的部分匹配。





数据测试将意图中的URI和MIME类型与过滤器中指定的URI和MIME类型进行比较。
 规定如下：


a、既不包含URI也不包含MIME类型的意图仅在过滤器未指定任何URI或MIME类型时传递测试。

b、包含URI但没有MIME类型（既不是显式的，也不能从URI推断）的意图仅在其URI与过滤器的URI格式匹配时传递测试，并且过滤器同样不指定MIME类型。

c、包含MIME类型而不是URI的意图只有在筛选器列出相同的MIME类型且不指定URI格式时，才会传递测试。

d、包含URI和MIME类型（从URI显式或可推断）的意图仅在该类型与过滤器中列出的类型匹配时才传递测试的MIME类型部分。 如果URI的URI与过滤器中的URI匹配，或者如果它具有内容：或file：URI，并且过滤器未指定URI，那么它会传递测试的URI部分。
 换句话说，假设一个组件支持content：和file：data，如果它的过滤器只列出一个MIME类型。




最后一条规则（d）反映了组件能够从文件或内容提供者获取本地数据的期望。 因此，他们的过滤器只能列出数据类型，并且不需要明确地命名内容：和file：schemes。 这是一个典型的例子。 例如，像下面这样的<data>元素告诉Android，该组件可以从内容提供器获取图像数据并显示它：




```
<intent-filter>
    <data android:mimeType="image/*" />
    ...
</intent-filter>
```


由于大多数可用数据由内容提供器分配，因此指定数据类型而不是URI的过滤器可能是最常见的。






另一个常见的配置是具有方案和数据类型的过滤器。 例如，像下面的<data>元素告诉Android组件可以从网络中检索视频数据，以执行该操作：




```
<intent-filter>
    <data android:scheme="http" android:type="video/*" />
    ...
</intent-filter>
```


四）、意图匹配



意图与意图过滤器匹配，不仅可以发现要激活的目标组件，还可以发现有关设备上的组件集的一些内容。 例如，家庭应用程序通过查找指定ACTION_MAIN操作和CATEGORY_LAUNCHER类别的意图过滤器的所有活动来填充应用启动器。





您的应用程序可以以类似的方式使用意图匹配。 PackageManager具有一组query ...（）方法，它们返回可以接受特定意图的所有组件，以及一系列相似的resolve ...（）方法，用于确定响应意图的最佳组件。 例如，queryIntentActivities（）返回可以执行作为参数传递的意图的所有活动的列表，而queryIntentServices（）返回类似的服务列表。
 这两种方法都不会激活组件; 他们只列出可以回应的。 有一种类似的方法，queryBroadcastReceivers（），用于广播接收器。




