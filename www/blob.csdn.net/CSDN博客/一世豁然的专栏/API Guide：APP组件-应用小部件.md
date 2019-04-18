# API Guide：APP组件-应用小部件 - 一世豁然的专栏 - CSDN博客





2017年04月21日 09:37:16[一世豁然](https://me.csdn.net/Explorer_day)阅读数：497








注意：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档




应用程序小部件是微型应用程序视图，可以嵌入其他应用程序（如主屏幕）并接收定期更新。 这些视图在用户界面中称为小部件，您可以使用App Widget提供器发布。 能够容纳其他App Widget的应用程序组件称为App Widget主机。 下面的屏幕截图显示了音乐应用程序小部件。


![](https://img-blog.csdn.net/20170420091923249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





本文档介绍如何使用App Widget提供器发布应用小部件。 有关创建自己的App Widget Host来托管应用程序小部件的讨论，请参阅应用App Widget Host。





小部件设计：有关如何设计应用程序小部件的信息，请阅读Widget设计指南。







一、基础

要创建App Widget，您需要以下知识：


AppWidgetProviderInfo对象：描述App Widget的元数据，例如App Widget的布局，更新频率和AppWidgetProvider类。 这应该用XML来定义。





AppWidgetProvider类实现：定义基于广播事件的基本方法，允许您以编程方式定义App Widget的接口。 通过它，当App Widget更新，启用，禁用和删除时，您将收到广播。





视图布局：以XML定义的App Widget的初始布局。




此外，您可以实现App Widget配置活动。 这是一个可选的活动，当用户添加您的App Widget并允许他或她在创建时修改App Widget设置时启动。





以下部分介绍如何设置每个组件。








二、在清单中声明一个应用程序小部件

首先，在应用程序的AndroidManifest.xml文件中声明AppWidgetProvider类。




```
<receiver android:name="ExampleAppWidgetProvider" >
    <intent-filter>
        <action android:name="android.appwidget.action.APPWIDGET_UPDATE" />
    </intent-filter>
    <meta-data android:name="android.appwidget.provider"
               android:resource="@xml/example_appwidget_info" />
</receiver>
```


<receiver>元素需要android：name属性，它指定App Widget使用的AppWidgetProvider。






<intent-filter>元素必须包含具有android：name属性的<action>元素。 该属性指定AppWidgetProvider接受ACTION_APPWIDGET_UPDATE广播。 这是您必须明确声明的唯一广播。 AppWidgetManager会根据需要自动将所有其他App Widget广播发送到AppWidgetProvider。





<meta-data>元素指定AppWidgetProviderInfo资源，需要以下属性：


android:name：指定元数据名称。 使用android.appwidget.provider将数据标识为AppWidgetProviderInfo描述符。

android:resource:指定AppWidgetProviderInfo资源位置。







三、添加AppWidgetProviderInfo元数据

AppWidgetProviderInfo定义了App Widget的基本属性，例如其最小布局尺寸，其初始布局资源，更新App Widget的频率以及（可选）在创建时启动的配置Activity。 使用单个<appwidget-provider>元素在XML资源中定义AppWidgetProviderInfo对象并将其保存在项目的res / xml /文件夹中。





例如：



```
<appwidget-provider xmlns:android="http://schemas.android.com/apk/res/android"
    android:minWidth="40dp"
    android:minHeight="40dp"
    android:updatePeriodMillis="86400000"
    android:previewImage="@drawable/preview"
    android:initialLayout="@layout/example_appwidget"
    android:configure="com.example.android.ExampleAppWidgetConfigure"
    android:resizeMode="horizontal|vertical"
    android:widgetCategory="home_screen">
</appwidget-provider>
```


以下是<appwidget-provider>属性的摘要：



1、minWidth和minHeight属性的值指定App Widget默认消耗的最小空间量。默认的主屏幕基于具有定义的高度和宽度的单元格网格将“应用程序小部件”在其窗口中放置。如果应用程序小部件的最小宽度或高度的值与单元格的尺寸不匹配，则应用程序窗口小部件的尺寸将舍入到最近的单元格大小。




有关调整App Widget大小的更多信息，请参阅App Widget设计指南。





注意：要使您的应用程序小部件可以在设备之间移动，您的应用程序小部件的最小大小不能大于4 x 4个单元格。




2、minResizeWidth和minResizeHeight属性指定App Widget的绝对最小大小。这些值应该指定在应用程序小部件将难以辨认或者不可用时尺寸的大小。使用这些属性允许用户将窗口小部件的大小调整为可能小于由minWidth和minHeight属性定义的默认窗口小部件的大小。 在Android 3.1中引入。




有关调整App Widget大小的更多信息，请参阅App Widget设计指南。





3、updatePeriodMillis属性定义AppWidget框架如何通过调用onUpdate（）回调方法从AppWidgetProvider请求更新的频率。 实际的更新不能保证按照这个值准确地发生，我们建议尽可能不频繁地进行更新 - 也许不超过一个小时来节省电池。 您还可以允许用户在配置中调整频率 - 有些人可能希望股票代码每15分钟更新一次，或者每天更新一次。




注意：如果设备在更新时间（由updatePeriodMillis定义）时处于睡眠状态，则设备将唤醒以执行更新。 如果您每小时不更新多次，这可能不会导致电池寿命的重大问题。 但是，如果您需要更频繁地更新和/或您不需要在设备睡眠时进行更新，则可以根据不会唤醒设备的警报来执行更新。 为此，请使用AlarmManager设置一个使用AppWidgetProvider收到的Intent的警报。 将报警类型设置为ELAPSED_REALTIME或RTC，只有在设备唤醒时才会发出报警。 然后将updatePeriodMillis设置为零（“0”）。





4、initialLayout属性指向定义App Widget布局的布局资源。




5、configure属性定义当用户添加App Widget时要启动的Activity，以便他或她配置App Widget属性。 这是可选的（请参阅下面的“创建应用程序小部件配置活动”）。




6、previewImage属性指定了在配置应用程序小部件后，用户在选择应用程序小部件时会看到的内容。 如果没有提供，用户会看到您的应用程序的启动器图标。 该字段对应于AndroidManifest.xml文件中的<receiver>元素中的android：previewImage属性。 有关使用previewImage的更多讨论，请参阅设置预览图像。 在Android 3.0中引入。




7、autoAdvanceViewId属性指定应该由窗口小部件的主机自动进阶的应用程序小部件子视图的视图ID。 在Android 3.0中引入。




8、resizeMode属性指定可以调整小部件大小的规则。 您可以使用此属性使主屏幕小部件在水平，垂直或两个轴上重新调整大小。 用户触摸一个小部件以显示其调整大小的句柄，然后拖动水平和/或垂直句柄来更改布局网格上的大小。 resizeMode属性的值包括“horizontal”，“vertical”和“none”。 要将窗口小部件声明为可水平和垂直调整大小，请提供“horizontal | vertical”值。 在Android 3.1中引入。




9、minResizeHeight属性指定小部件可以调整大小的最小高度（以dps为单位）。 如果该字段大于minHeight或垂直调整大小未启用，则此字段无效（请参阅resizeMode）。 在Android 4.0中引入。




10、minResizeWidth属性指定小部件可以调整大小的最小宽度（以dps为单位）。 如果该字段大于minWidth或者未启用水平调整大小（参见resizeMode），则此字段无效。 在Android 4.0中引入。




11、widgetCategory属性声明您的App Widget是否可以在主屏幕（home_screen），锁定屏幕（键盘保护）或两者上显示。 只有低于5.0的Android版本支持锁屏小部件。 对于Android 5.0及更高版本，只有home_screen有效。




有关<appwidget-provider>元素接受的属性的更多信息，请参阅AppWidgetProviderInfo类。








四、创建应用程序小部件布局

您必须使用XML定义应用程序小部件的初始布局，并将其保存在项目的res / layout /目录中。 您可以使用下面列出的View对象来设计App Widget，但在开始设计App Widget之前，请阅读并了解“App Widget设计指南”。





如果您熟悉Layouts，则创建App Widget布局很简单。 但是，您必须注意，App Widget布局基于RemoteViews，它不支持每种布局或查看窗口小部件。





RemoteViews对象（以及App Widget）可以支持以下布局类：



- `[FrameLayout](https://developer.android.google.cn/reference/android/widget/FrameLayout.html)`
- `[LinearLayout](https://developer.android.google.cn/reference/android/widget/LinearLayout.html)`
- `[RelativeLayout](https://developer.android.google.cn/reference/android/widget/RelativeLayout.html)`
- `[GridLayout](https://developer.android.google.cn/reference/android/widget/GridLayout.html)`




和以下小部件类：



- `[AnalogClock](https://developer.android.google.cn/reference/android/widget/AnalogClock.html)`
- `[Button](https://developer.android.google.cn/reference/android/widget/Button.html)`
- `[Chronometer](https://developer.android.google.cn/reference/android/widget/Chronometer.html)`
- `[ImageButton](https://developer.android.google.cn/reference/android/widget/ImageButton.html)`
- `[ImageView](https://developer.android.google.cn/reference/android/widget/ImageView.html)`
- `[ProgressBar](https://developer.android.google.cn/reference/android/widget/ProgressBar.html)`
- `[TextView](https://developer.android.google.cn/reference/android/widget/TextView.html)`
- `[ViewFlipper](https://developer.android.google.cn/reference/android/widget/ViewFlipper.html)`
- `[ListView](https://developer.android.google.cn/reference/android/widget/ListView.html)`
- `[GridView](https://developer.android.google.cn/reference/android/widget/GridView.html)`
- `[StackView](https://developer.android.google.cn/reference/android/widget/StackView.html)`
- `[AdapterViewFlipper](https://developer.android.google.cn/reference/android/widget/AdapterViewFlipper.html)`




不支持这些类的子类。





RemoteViews还支持ViewStub，这是一个不可见的零大小视图，您可以在运行时随意地inflate布局资源。





一）、向应用程序小部件添加边距


窗口小部件通常不应延伸到屏幕边缘，并且不应与视觉上与其他窗口小部件齐平，因此您应该在窗口小部件框架周围的所有侧面添加边距。





从Android 4.0开始，应用程序小部件将自动在小部件框架和应用程序小部件的边框之间进行填充，以便与用户主屏幕上的其他小部件和图标更好地对齐。 要利用此强烈推荐的行为，请将应用程序的targetSdkVersion设置为14或更高。





编写一个单一具有应用于早期版本平台的自定义页边距的布局，并且Android 4.0及更高版本没有额外的边距很容易：


1、将应用程序的targetSdkVersion设置为14或更高。

2、创建一个下面的布局，为其边距引用维度资源：



```
<FrameLayout
  android:layout_width="match_parent"
  android:layout_height="match_parent"
  android:padding="@dimen/widget_margin">

  <LinearLayout
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="horizontal"
    android:background="@drawable/my_widget_background">
    …
  </LinearLayout>

</FrameLayout>
```


3、创建二维资源，一个在res / values /中提供Android 4.0之前的自定义边距，一个在res / values-v14 /中为Android 4.0小部件提供额外的填充：



res / values / dimensional.xml：



`<dimen name="widget_margin">8dp</dimen>`res/values-v14/dimens.xml:
`<dimen name="widget_margin">0dp</dimen>`

默认情况下，另外一个选择是为您的九个补丁背景资源简单地创建额外的边距，并为API级别14或更高版本提供不同的没有边距的九个补丁。









五、使用AppWidgetProvider类

AppWidgetProvider类将BroadcastReceiver扩展为一个便捷类来处理App Widget广播。 AppWidgetProvider仅接收与App Widget相关的事件广播，例如，当App Widget被更新，删除，启用和禁用时。 当这些广播事件发生时，AppWidgetProvider会收到以下方法调用：


[onUpdate()](https://developer.android.google.cn/reference/android/appwidget/AppWidgetProvider.html#onUpdate(android.content.Context,%20android.appwidget.AppWidgetManager,%20int%5B%5D))


这被称为以AppWidgetProviderInfo中的updatePeriodMillis属性定义的间隔更新App Widget（请参阅上面的添加AppWidgetProviderInfo元数据）。当用户添加App Widget时也会调用此方法，因此它应该执行必要的设置，如定义视图的事件处理程序，并在必要时启动临时服务。但是，如果您已经声明了一个配置活动，则当用户添加App Widget则不会被调用，但是在后续更新才会被调用。在配置完成后，执行第一次更新的配置活动是有责任的。 （请参阅下面的创建应用程序小部件配置活动。）





[onAppWidgetOptionsChanged()](https://developer.android.google.cn/reference/android/appwidget/AppWidgetProvider.html#onAppWidgetOptionsChanged(android.content.Context,%20android.appwidget.AppWidgetManager,%20int,%20android.os.Bundle))


当小部件首次放置并且任何时候该小部件被调整大小时，此函数被调用。 您可以使用此回调来根据窗口小部件的大小范围显示或隐藏内容。 您可以通过调用getAppWidgetOptions（）获取大小范围，该方法返回包含以下内容的Bundle：


OPTION_APPWIDGET_MIN_WIDTH - 包含小部件实例当前宽度（以dp为单位）的下限。


OPTION_APPWIDGET_MIN_HEIGHT - 包含小部件实例的当前高度（以dp为单位）的下限。


OPTION_APPWIDGET_MAX_WIDTH - 包含widget实例当前宽度（以dp为单位）的上限。


OPTION_APPWIDGET_MAX_HEIGHT - 包含小部件实例的当前宽度（以dp为单位）的上限。





这个回调是在API Level 16（Android 4.1）中引入的。 如果您实现此回调，请确保您的应用程序不依赖于它，因为它不会在旧设备上调用。





[onDeleted(Context,
 int[])](https://developer.android.google.cn/reference/android/appwidget/AppWidgetProvider.html#onDeleted(android.content.Context,%20int%5B%5D))


每当应用程序小部件从App Widget主机中删除时，都会被调用。





[onEnabled(Context)](https://developer.android.google.cn/reference/android/appwidget/AppWidgetProvider.html#onEnabled(android.content.Context))


当第一次创建App Widget的实例时，此函数将被调用。例如，如果用户添加了您的App Widget的两个实例，这仅仅是第一次调用。 如果您需要打开一个新的数据库或执行只需要对所有App Widget实例发生一次的其他设置，那么这是一个很好的做法。





[onDisabled(Context)](https://developer.android.google.cn/reference/android/appwidget/AppWidgetProvider.html#onDisabled(android.content.Context))


当您的应用程序小部件的最后一个实例从App Widget主机中删除时，此操作将被调用。 您应该清理onEnabled（Context）中完成的任何工作，例如删除临时数据库。





[onReceive(Context,
 Intent)](https://developer.android.google.cn/reference/android/appwidget/AppWidgetProvider.html#onReceive(android.content.Context,%20android.content.Intent))


在每个广播和上述每个方法被调用时，此方法被调用。您通常不需要实现此方法，因为默认的AppWidgetProvider实现过滤所有App Widget广播，并根据需要调用上述方法。





最重要的AppWidgetProvider回调是onUpdate（），因为每个App Widget都添加到主机时调用（除非您使用配置活动）。 如果您的App Widget接受任何用户交互活动，那么您需要在此回调中注册事件处理程序。 如果您的App Widget不创建临时文件或数据库，或执行需要清理的其他工作，则onUpdate（）可能是您需要定义的唯一回调方法。 例如，如果您想要一个按钮的应用程序小部件，该按钮在单击时启动活动，则可以使用以下AppWidgetProvider的实现：




```java
public class ExampleAppWidgetProvider extends AppWidgetProvider {

    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds) {
        final int N = appWidgetIds.length;

        // Perform this loop procedure for each App Widget that belongs to this provider
        for (int i=0; i<N; i++) {
            int appWidgetId = appWidgetIds[i];

            // Create an Intent to launch ExampleActivity
            Intent intent = new Intent(context, ExampleActivity.class);
            PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);

            // Get the layout for the App Widget and attach an on-click listener
            // to the button
            RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.appwidget_provider_layout);
            views.setOnClickPendingIntent(R.id.button, pendingIntent);

            // Tell the AppWidgetManager to perform an update on the current app widget
            appWidgetManager.updateAppWidget(appWidgetId, views);
        }
    }
}
```


此AppWidgetProvider仅定义onUpdate（）方法，以定义启动Activity的PendingIntent，并使用setOnClickPendingIntent（int，PendingIntent）将其附加到App Widget的按钮。 请注意，它包含循环，遍历appWidgetIds中的每个条目，这是一个ID数组，用于标识此提供程序创建的每个App Widget。 这样，如果用户创建了多个应用程序小部件实例，则它们都将同时更新。 但是，只有一个updatePeriodMillis计划将针对App
 Widget的所有实例进行管理。 例如，如果更新计划被定义为每两个小时，并且第一个应用程序小部件的第二个实例在第一个之后一小时添加，则它们将在由第一个和第二个更新定义的时期更新 期间将被忽略（他们将每两个小时更新一次，而不是每小时更新一次）。






注意：由于AppWidgetProvider是BroadcastReceiver的扩展，因此您的进程不能保证在回调方法返回后继续运行（有关广播生命周期的信息，请参阅BroadcastReceiver）。 如果您的App Widget设置过程可能需要几秒钟（可能在执行Web请求时），并且要求您的进程继续，请考虑在onUpdate（）方法中启动服务。 从服务内部，您可以对App Widget执行自己的更新，而不用担心AppWidgetProvider由于应用程序无响应（ANR）错误而关闭。 有关运行服务的应用程序小部件的示例，请参阅维基词典示例的AppWidgetProvider。





另请参阅ExampleAppWidgetProvider.java示例类。





一）、接收应用程序小部件广播意图

AppWidgetProvider只是一个便捷类。 如果您想直接收到App Widget广播，您可以实现自己的BroadcastReceiver或覆盖onReceive（Context，Intent）回调。 您需要关心的意图如下：



- `[ACTION_APPWIDGET_UPDATE](https://developer.android.google.cn/reference/android/appwidget/AppWidgetManager.html#ACTION_APPWIDGET_UPDATE)`
- `[ACTION_APPWIDGET_DELETED](https://developer.android.google.cn/reference/android/appwidget/AppWidgetManager.html#ACTION_APPWIDGET_DELETED)`
- `[ACTION_APPWIDGET_ENABLED](https://developer.android.google.cn/reference/android/appwidget/AppWidgetManager.html#ACTION_APPWIDGET_ENABLED)`
- `[ACTION_APPWIDGET_DISABLED](https://developer.android.google.cn/reference/android/appwidget/AppWidgetManager.html#ACTION_APPWIDGET_DISABLED)`
- `[ACTION_APPWIDGET_OPTIONS_CHANGED](https://developer.android.google.cn/reference/android/appwidget/AppWidgetManager.html#ACTION_APPWIDGET_OPTIONS_CHANGED)`









六、创建应用程序小部件配置活动

如果您希望用户在添加新的App Widget时配置设置，则可以创建App Widget配置活动。 此活动将由App Widget主机自动启动，并允许用户在创建时配置App Widget的可用设置，例如App Widget的颜色，大小，更新周期或其他功能设置。





应将配置活动声明为Android清单文件中的常规活动。 然而，它将由App Widget主机使用ACTION_APPWIDGET_CONFIGURE操作启动，因此Activity需要接受此Intent。 例如：




```
<activity android:name=".ExampleAppWidgetConfigure">
    <intent-filter>
        <action android:name="android.appwidget.action.APPWIDGET_CONFIGURE"/>
    </intent-filter>
</activity>
```


此外，活动必须在AppWidgetProviderInfo XML文件中声明，具有android：configure属性（请参阅上面的添加AppWidgetProviderInfo元数据）。 例如，配置Activity可以这样声明：





```
<appwidget-provider xmlns:android="http://schemas.android.com/apk/res/android"
    ...
    android:configure="com.example.android.ExampleAppWidgetConfigure"
    ... >
</appwidget-provider>
```


请注意，使用完全限定的命名空间声明“活动”，因为它将从您的包范围之外引用。






这就是开始使用配置活动所需要的。 现在你需要的是实际的活动。 但是，当您实现“活动”时，有两件重要的事情要记住：


1、App Widget主机调用配置Activity而且配置Activity应始终返回结果。结果应包括通过启动活动的Intent来传送的App Widget ID。（保存在Intent extras中作为EXTRA_APPWIDGET_ID）。

2、当创建App Widget时，不会调用onUpdate（）方法（启动配置活动时，系统不会发送ACTION_APPWIDGET_UPDATE广播）。 App 小部件首次创建时，从AppWidgetManager请求更新的配置活动是有责任的。 然而，onUpdate（）将被调用用于后续更新 - 它只会在第一次被跳过。




有关如何从配置返回结果并更新App Widget的示例，请参阅以下部分中的代码片段。





一）、从配置活动更新App Widget

当应用程序小部件使用配置活动时，配置完成后，更新应用程序小部件是活动的责任。 您可以直接从AppWidgetManager请求更新。





以下是正确更新应用程序小部件并关闭配置的过程的摘要步骤：


1、首先，从启动Activity的Intent中获取App Widget ID：



```java
Intent intent = getIntent();
Bundle extras = intent.getExtras();
if (extras != null) {
    mAppWidgetId = extras.getInt(
            AppWidgetManager.EXTRA_APPWIDGET_ID,
            AppWidgetManager.INVALID_APPWIDGET_ID);
}
```


2、执行您的App Widget配置。






3、配置完成后，通过调用getInstance（Context）获取AppWidgetManager的一个实例：



```java
AppWidgetManager appWidgetManager = AppWidgetManager.getInstance(context);
```


4、通过调用updateAppWidget（int，RemoteViews）通过RemoteViews布局更新App Widget：





```java
RemoteViews views = new RemoteViews(context.getPackageName(),
R.layout.example_appwidget);
appWidgetManager.updateAppWidget(mAppWidgetId, views);
```


5、最后，创建返回Intent，使用Activity结果进行设置，并完成Activity：





```java
Intent resultValue = new Intent();
resultValue.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, mAppWidgetId);
setResult(RESULT_OK, resultValue);
finish();
```


提示：当首次打开配置活动时，将活动结果与EXTRA_APPWIDGET_ID一起设置为RESULT_CANCELED，如上述步骤5所示。 这样，如果用户在到达结束之前退出活动，则会通知App Widget主机配置已取消，并且将不会添加App Widget。






有关示例，请参阅ApiDemos中的ExampleAppWidgetConfigure.java示例类。








七、设置预览图像

Android 3.0引入了previewImage字段，它指定了应用程序小部件的预览。 该预览将从窗口小部件选择器向用户显示。 如果未提供此字段，则会使用应用程序小部件的图标进行预览。





这是您如何在XML中指定此设置：




```
<appwidget-provider xmlns:android="http://schemas.android.com/apk/res/android"
  ...
  android:previewImage="@drawable/preview">
</appwidget-provider>
```


为了帮助您创建应用程序小部件的预览图像（在previewImage字段中指定），Android模拟器包括一个名为“Widget Preview”的应用程序。 要创建预览图像，请启动此应用程序，为应用程序选择应用程序小部件，并设置您希望显示预览图像的方式，然后将其保存并存储在应用程序的可绘制资源中。









八、使用集合应用程序小部件

Android 3.0引入集合了应用程序小部件。 这些类型的应用程序小部件使用RemoteViewsService来显示由远程数据支持的集合，例如来自内容提供商。 RemoteViewsService提供的数据是使用以下视图类型之一在应用程序小部件中显示的，我们将其称为“集合视图”：


[ListView](https://developer.android.google.cn/reference/android/widget/ListView.html)


在垂直滚动列表中显示项目的视图。 有关示例，请参阅Gmail应用程序小部件。





[GridView](https://developer.android.google.cn/reference/android/widget/GridView.html)


在二维滚动网格中显示项目的视图。 有关示例，请参阅书签应用程序小部件。





[StackView](https://developer.android.google.cn/reference/android/widget/StackView.html)


堆叠的卡片视图（类似于rolodex），用户可以向上/向下轻弹前卡，分别查看上一张/下一张卡片。 例子包括YouTube和图书应用程序小部件。





[AdapterViewFlipper](https://developer.android.google.cn/reference/android/widget/AdapterViewFlipper.html)


一个适配器支持的简单ViewAnimator，可在两个或多个视图之间活动。 一次只显示一个。





如上所述，这些集合视图显示由远程数据支持的集合。 这意味着他们使用适配器将他们的用户界面绑定到他们的数据。 适配器将一组数据中的各个项目绑定到单独的View对象中。 由于这些集合视图是由适配器支持的，因此Android框架必须包含额外的体系结构，以支持其在应用程序小部件中的使用。 在应用程序小部件的上下文中，适配器由RemoteViewsFactory替代，它只是适配器接口周围的薄包装。 当对集合中的特定项目请求时，RemoteViewsFactory会将该集合的项目作为RemoteViews对象创建并返回。
 为了在应用程序小部件中包含集合视图，必须实现RemoteViewsService和RemoteViewsFactory。





RemoteViewsService是允许远程适配器请求RemoteViews对象的服务。 RemoteViewsFactory是集合视图（如ListView，GridView等）和该视图的底层数据之间的适配器接口。 从StackView Widget示例中，以下是用于实现此服务和界面的样板代码示例：




```java
public class StackWidgetService extends RemoteViewsService {
    @Override
    public RemoteViewsFactory onGetViewFactory(Intent intent) {
        return new StackRemoteViewsFactory(this.getApplicationContext(), intent);
    }
}

class StackRemoteViewsFactory implements RemoteViewsService.RemoteViewsFactory {

//... include adapter-like methods here. See the StackView Widget sample.

}
```





一）、示例应用程序

本节中的代码摘录摘自StackView Widget示例：


![](https://img-blog.csdn.net/20170421085219315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





该示例由10个视图的堆栈组成，显示值为“0！到“9！” 示例应用程序小部件具有以下主要行为：


1、用户可以在应用程序小部件中垂直移动顶视图以显示下一个或上一个视图。 这是一个内置的StackView行为。




2、没有任何用户交互，应用程序小部件将按顺序自动前进其视图，如幻灯片放映。 这是由于在res / xml / stackwidgetinfo.xml文件中设置了android：autoAdvanceViewId =“@ id / stack_view”。 此设置适用于视图ID，在这种情况下，该视图是堆叠视图的视图ID。




3、如果用户触摸顶视图，则应用程序小部件将显示Toast消息“触摸视图n”，其中n是触摸视图的索引（位置）。 有关如何实现的更多讨论，请参阅将行为添加到单个项目。




二）使用集合实现应用程序小部件

要使用集合实现应用程序小部件，请遵循用于实现任何应用程序小部件的基本步骤。 以下部分描述了使用集合实现应用程序小部件时需要执行的其他步骤。





1、集合应用程序小部件的清单

除了在清单中声明应用程序小部件中列出的要求之外，为了使应用程序小部件集合绑定到您的RemoteViewsService成为可能，您必须在清单文件中声明具有BIND_REMOTEVIEWS权限的服务。 这可以防止其他应用程序自由访问您的应用程序小部件的数据。 例如，当创建使用RemoteViewsService来填充集合视图的App Widget时，清单条目可能如下所示：




```
<service android:name="MyWidgetService"
...
android:permission="android.permission.BIND_REMOTEVIEWS" />
```


android：name =“MyWidgetService”是指RemoteViewsService的子类。






2、应用程序小部件集合的布局

您的应用程序小部件布局XML文件的主要要求是它包含集合视图之一：ListView，GridView，StackView或AdapterViewFlipper。 以下是StackView Widget示例的widget_layout.xml：




```
<?xml version="1.0" encoding="utf-8"?>

<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent">
    <StackView xmlns:android="http://schemas.android.com/apk/res/android"
        android:id="@+id/stack_view"
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:gravity="center"
        android:loopViews="true" />
    <TextView xmlns:android="http://schemas.android.com/apk/res/android"
        android:id="@+id/empty_view"
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:gravity="center"
        android:background="@drawable/widget_item_background"
        android:textColor="#ffffff"
        android:textStyle="bold"
        android:text="@string/empty_view_text"
        android:textSize="20sp" />
</FrameLayout>
```


请注意，空视图必须是集合视图的兄弟节点，空视图表示空状态。






除了整个应用程序小部件的布局文件之外，还必须创建另一个布局文件，该文件定义集合中每个项目的布局（例如，图书集合中每本书的布局）。 例如，StackView Widget示例只有一个布局文件widget_item.xml，因为所有项目都使用相同的布局。 但是WeatherListWidget示例有两个布局文件：dark_widget_item.xml和light_widget_item.xml。





3、应用程序小部件集合的AppWidgetProvider类

与常规的应用程序小部件一样，AppWidgetProvider子类中的大部分代码通常都在onUpdate（）中。 在使用集合创建应用程序小部件时，onUpdate（）实现的主要区别是必须调用setRemoteAdapter（）。 这告诉集合视图在哪里获取其数据。 RemoteViewsService然后可以返回RemoteViewsFactory的实现，并且窗口小部件可以提供适当的数据。 当您调用此方法时，必须传递指向您的RemoteViewsService实现的意图以及指定要更新的应用程序小部件的应用程序小部件ID。





例如，下面是StackView Widget示例如何实现onUpdate（）回调方法来将RemoteViewsService设置为应用程序小部件集合的远程适配器：




```java
public void onUpdate(Context context, AppWidgetManager appWidgetManager,
int[] appWidgetIds) {
    // update each of the app widgets with the remote adapter
    for (int i = 0; i < appWidgetIds.length; ++i) {

        // Set up the intent that starts the StackViewService, which will
        // provide the views for this collection.
        Intent intent = new Intent(context, StackWidgetService.class);
        // Add the app widget ID to the intent extras.
        intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetIds[i]);
        intent.setData(Uri.parse(intent.toUri(Intent.URI_INTENT_SCHEME)));
        // Instantiate the RemoteViews object for the app widget layout.
        RemoteViews rv = new RemoteViews(context.getPackageName(), R.layout.widget_layout);
        // Set up the RemoteViews object to use a RemoteViews adapter.
        // This adapter connects
        // to a RemoteViewsService  through the specified intent.
        // This is how you populate the data.
        rv.setRemoteAdapter(appWidgetIds[i], R.id.stack_view, intent);

        // The empty view is displayed when the collection has no items.
        // It should be in the same layout used to instantiate the RemoteViews
        // object above.
        rv.setEmptyView(R.id.stack_view, R.id.empty_view);

        //
        // Do additional processing specific to this app widget...
        //

        appWidgetManager.updateAppWidget(appWidgetIds[i], rv);
    }
    super.onUpdate(context, appWidgetManager, appWidgetIds);
}
```


4、RemoteViewsService类



如上所述，您的RemoteViewsService子类提供用于填充远程集合视图的RemoteViewsFactory。





具体来说，您需要执行以下步骤：


1）、RemoteViewsService子类。 RemoteViewsService是通过远程适配器可以请求RemoteViews的服务。

2）、在RemoteViewsService子类中，包含一个实现RemoteViewsFactory接口的类。 RemoteViewsFactory是远程集合视图（如ListView，GridView等）和该视图的底层数据之间的适配器的接口。 您的实现负责为数据集中的每个项制作一个RemoteViews对象。 该接口是适配器周围的薄包装。




RemoteViewsService实现的主要内容是它的RemoteViewsFactory，如下所述。





5、RemoteViewsFactory界面

实现RemoteViewsFactory接口的自定义类向应用程序小部件提供其集合中项目的数据。 为此，它将您的应用程序小部件项目XML布局文件与数据源相结合。 数据源可以是从数据库到简单数组的任何数据。 在StackView Widget示例中，数据源是一组WidgetItems。 RemoteViewsFactory用作将数据粘贴到远程集合视图的适配器。





您需要为RemoteViewsFactory子类实现的两个最重要的方法是onCreate（）和getViewAt（）。





系统在首次创建工厂时调用onCreate（）。 这是您为数据源设置任何连接和/或游标的位置。 例如，StackView Widget示例使用onCreate（）初始化WidgetItem对象的数组。 当您的应用程序小部件处于活动状态时，系统将使用其在数组中的索引位置来访问这些对象，并显示它们包含的文本。





以下是StackView Widget示例的RemoteViewsFactory实现的摘录，该实现显示了onCreate（）方法的部分内容：



```java
class StackRemoteViewsFactory implements
RemoteViewsService.RemoteViewsFactory {
    private static final int mCount = 10;
    private List<WidgetItem> mWidgetItems = new ArrayList<WidgetItem>();
    private Context mContext;
    private int mAppWidgetId;

    public StackRemoteViewsFactory(Context context, Intent intent) {
        mContext = context;
        mAppWidgetId = intent.getIntExtra(AppWidgetManager.EXTRA_APPWIDGET_ID,
                AppWidgetManager.INVALID_APPWIDGET_ID);
    }

    public void onCreate() {
        // In onCreate() you setup any connections / cursors to your data source. Heavy lifting,
        // for example downloading or creating content etc, should be deferred to onDataSetChanged()
        // or getViewAt(). Taking more than 20 seconds in this call will result in an ANR.
        for (int i = 0; i < mCount; i++) {
            mWidgetItems.add(new WidgetItem(i + "!"));
        }
        ...
    }
...
```


RemoteViewsFactory方法getViewAt（）返回与数据集中指定位置的数据相对应的RemoteViews对象。 以下是StackView Widget示例的RemoteViewsFactory实现的摘录：





```java
public RemoteViews getViewAt(int position) {

    // Construct a remote views item based on the app widget item XML file,
    // and set the text based on the position.
    RemoteViews rv = new RemoteViews(mContext.getPackageName(), R.layout.widget_item);
    rv.setTextViewText(R.id.widget_item, mWidgetItems.get(position).text);

    ...
    // Return the remote views object.
    return rv;
}
```





6、将行为添加到单个项目

以上部分介绍如何将数据绑定到应用程序小部件集合。 但是，如果您想为集合视图中的各个项添加动态行为呢？





如使用AppWidgetProvider类中所述，您通常使用setOnClickPendingIntent（）来设置对象的点击行为，例如使按钮启动“活动”。 但是，对于个人收藏项目中的子视图不允许使用此方法（为了说明，您可以使用setOnClickPendingIntent（）在启动应用程序的Gmail应用程序小部件中设置全局按钮，例如但不在单个列表项中 ）。 而是将点击行为添加到集合中的各个项目，您可以使用setOnClickFillInIntent（）。 这需要设置收集视图的待处理意图模板，然后通过RemoteViewsFactory为集合中的每个项目设置填充意图。





本节使用StackView Widget示例来描述如何向单个项添加行为。 在StackView Widget示例中，如果用户触摸顶视图，则应用程序小部件将显示Toast消息“触摸视图n”，其中n是触摸视图的索引（位置）。 这是它的工作原理：


1）、StackWidgetProvider（一个AppWidgetProvider子类）创建一个挂起的intent，它有一个名为TOAST_ACTION的自定义操作。

2）、当用户触摸视图时，意图被触发，并且广播TOAST_ACTION。

3）、此广播由StackWidgetProvider的onReceive（）方法拦截，应用程序小部件显示触摸视图的Toast消息。 收集项目的数据由RemoteViewsFactory通过RemoteViewsService提供。




注意：StackView Widget示例使用广播，但通常，应用程序小部件将简单地在像这样的场景中启动一个活动。





7、设置待处理的意图模板

StackWidgetProvider（AppWidgetProvider子类）设置一个挂起的意图。 收藏的个人项目无法设置自己的待处理意图。 相反，集合作为一个整体设置了一个挂起的意图模板，并且各个项目设置了填充意图，以逐个项目的形式创建独特的行为。





该类也接收当用户触摸视图时发送的广播。 它在其onReceive（）方法中处理此事件。 如果意图的操作是TOAST_ACTION，则应用程序小部件将显示当前视图的Toast消息。




```java
public class StackWidgetProvider extends AppWidgetProvider {
    public static final String TOAST_ACTION = "com.example.android.stackwidget.TOAST_ACTION";
    public static final String EXTRA_ITEM = "com.example.android.stackwidget.EXTRA_ITEM";

    ...

    // Called when the BroadcastReceiver receives an Intent broadcast.
    // Checks to see whether the intent's action is TOAST_ACTION. If it is, the app widget
    // displays a Toast message for the current item.
    @Override
    public void onReceive(Context context, Intent intent) {
        AppWidgetManager mgr = AppWidgetManager.getInstance(context);
        if (intent.getAction().equals(TOAST_ACTION)) {
            int appWidgetId = intent.getIntExtra(AppWidgetManager.EXTRA_APPWIDGET_ID,
                AppWidgetManager.INVALID_APPWIDGET_ID);
            int viewIndex = intent.getIntExtra(EXTRA_ITEM, 0);
            Toast.makeText(context, "Touched view " + viewIndex, Toast.LENGTH_SHORT).show();
        }
        super.onReceive(context, intent);
    }

    @Override
    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds) {
        // update each of the app widgets with the remote adapter
        for (int i = 0; i < appWidgetIds.length; ++i) {

            // Sets up the intent that points to the StackViewService that will
            // provide the views for this collection.
            Intent intent = new Intent(context, StackWidgetService.class);
            intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetIds[i]);
            // When intents are compared, the extras are ignored, so we need to embed the extras
            // into the data so that the extras will not be ignored.
            intent.setData(Uri.parse(intent.toUri(Intent.URI_INTENT_SCHEME)));
            RemoteViews rv = new RemoteViews(context.getPackageName(), R.layout.widget_layout);
            rv.setRemoteAdapter(appWidgetIds[i], R.id.stack_view, intent);

            // The empty view is displayed when the collection has no items. It should be a sibling
            // of the collection view.
            rv.setEmptyView(R.id.stack_view, R.id.empty_view);

            // This section makes it possible for items to have individualized behavior.
            // It does this by setting up a pending intent template. Individuals items of a collection
            // cannot set up their own pending intents. Instead, the collection as a whole sets
            // up a pending intent template, and the individual items set a fillInIntent
            // to create unique behavior on an item-by-item basis.
            Intent toastIntent = new Intent(context, StackWidgetProvider.class);
            // Set the action for the intent.
            // When the user touches a particular view, it will have the effect of
            // broadcasting TOAST_ACTION.
            toastIntent.setAction(StackWidgetProvider.TOAST_ACTION);
            toastIntent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetIds[i]);
            intent.setData(Uri.parse(intent.toUri(Intent.URI_INTENT_SCHEME)));
            PendingIntent toastPendingIntent = PendingIntent.getBroadcast(context, 0, toastIntent,
                PendingIntent.FLAG_UPDATE_CURRENT);
            rv.setPendingIntentTemplate(R.id.stack_view, toastPendingIntent);

            appWidgetManager.updateAppWidget(appWidgetIds[i], rv);
        }
    super.onUpdate(context, appWidgetManager, appWidgetIds);
    }
}
```





8、设置填写意图

您的RemoteViewsFactory必须为集合中的每个项目设置一个填写意图。 这使得可以区分给定项目的单个点击动作。 然后将填写意图与PendingIntent模板组合，以确定在单击项目时将执行的最终意图。




```java
public class StackWidgetService extends RemoteViewsService {
    @Override
    public RemoteViewsFactory onGetViewFactory(Intent intent) {
        return new StackRemoteViewsFactory(this.getApplicationContext(), intent);
    }
}

class StackRemoteViewsFactory implements RemoteViewsService.RemoteViewsFactory {
    private static final int mCount = 10;
    private List<WidgetItem> mWidgetItems = new ArrayList<WidgetItem>();
    private Context mContext;
    private int mAppWidgetId;

    public StackRemoteViewsFactory(Context context, Intent intent) {
        mContext = context;
        mAppWidgetId = intent.getIntExtra(AppWidgetManager.EXTRA_APPWIDGET_ID,
                AppWidgetManager.INVALID_APPWIDGET_ID);
    }

    // Initialize the data set.
        public void onCreate() {
            // In onCreate() you set up any connections / cursors to your data source. Heavy lifting,
            // for example downloading or creating content etc, should be deferred to onDataSetChanged()
            // or getViewAt(). Taking more than 20 seconds in this call will result in an ANR.
            for (int i = 0; i < mCount; i++) {
                mWidgetItems.add(new WidgetItem(i + "!"));
            }
           ...
        }
        ...

        // Given the position (index) of a WidgetItem in the array, use the item's text value in
        // combination with the app widget item XML file to construct a RemoteViews object.
        public RemoteViews getViewAt(int position) {
            // position will always range from 0 to getCount() - 1.

            // Construct a RemoteViews item based on the app widget item XML file, and set the
            // text based on the position.
            RemoteViews rv = new RemoteViews(mContext.getPackageName(), R.layout.widget_item);
            rv.setTextViewText(R.id.widget_item, mWidgetItems.get(position).text);

            // Next, set a fill-intent, which will be used to fill in the pending intent template
            // that is set on the collection view in StackWidgetProvider.
            Bundle extras = new Bundle();
            extras.putInt(StackWidgetProvider.EXTRA_ITEM, position);
            Intent fillInIntent = new Intent();
            fillInIntent.putExtras(extras);
            // Make it possible to distinguish the individual on-click
            // action of a given item
            rv.setOnClickFillInIntent(R.id.widget_item, fillInIntent);

            ...

            // Return the RemoteViews object.
            return rv;
        }
    ...
    }
```





三）、保存新收集的数据

下图说明了在更新发生时使用集合的应用程序小部件中发生的流程。 它显示了应用程序小部件代码如何与RemoteViewsFactory进行交互，以及如何触发更新：


![](https://img-blog.csdn.net/20170421093443868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


使用集合的应用程序小部件的一个功能是能够向用户提供最新内容。 例如，考虑Android 3.0 Gmail应用程序小部件，为用户提供收件箱的快照。 为了实现这一点，您需要能够触发RemoteViewsFactory和集合视图来获取和显示新数据。 您通过AppWidgetManager调用notifyAppWidgetViewDataChanged（）来实现此目的。此调用导致您的RemoteViewsFactory的onDataSetChanged（）方法的回调，从而使您有机会获取任何新数据。 请注意，您可以在onDataSetChanged（）回调中同步执行处理密集型操作。
 您可以确保在从RemoteViewsFactory提取元数据或视图数据之前完成此调用。 此外，您可以在getViewAt（）方法内执行处理密集型操作。 如果此调用需要很长时间，则加载视图（由RemoteViewsFactory的getLoadingView（）方法指定）将显示在集合视图的相应位置，直到返回。






