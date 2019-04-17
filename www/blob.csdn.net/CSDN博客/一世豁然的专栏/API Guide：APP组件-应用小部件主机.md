# API Guide：APP组件-应用小部件主机 - 一世豁然的专栏 - CSDN博客





2017年04月22日 10:31:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：376








大多数Android设备上提供的Android主屏幕允许用户嵌入应用程序小部件以快速访问内容。如果您正在建立一个主配件或类似的应用程序，那么你还可以允许用户通过实施AppWidgetHost来嵌入应用程序小部件。这不是大多数应用程序需要做的事情，但如果您正在创建自己的主机，请务必了解主机隐含地同意的合同义务。




本文档重点介绍实现自定义AppWidgetHost所涉及的职责。 有关如何实现AppWidgetHost的示例，请参阅Android主屏幕启动器的源代码。





以下是实现自定义AppWidgetHost所涉及的关键类和概念的概述：


1、应用小部件主机：AppWidgetHost提供与AppWidget服务的应用程序（如主屏幕）的交互，其希望在其UI中嵌入应用程序小部件。 AppWidgetHost必须具有在主机自己的包中唯一的ID。 该ID在主机的所有用途上保持不变。 ID通常是您在应用程序中分配的硬编码值。




2、应用小部件ID：每个应用程序小部件实例在绑定时分配一个唯一的ID（请参阅bindAppWidgetIdIfAllowed（），在“绑定”应用程序小部件中有更详细的讨论）。这唯一ID由主机使用allocateAppWidgetId（）获取。该ID在窗口小部件的整个生命周期中是持久的，也就是直到从主机中删除。任何特定于主机的状态（例如小部件的大小和位置）应由主机包持续存储，并与应用程序小部件ID相关联。




3、应用程序小部件主机视图：AppWidgetHostView可以被认为是在需要显示窗口小部件时被包装的框架。 每当小部件被主机infated时，应用程序小部件将分配给AppWidgetHostView。




4、选项包：AppWidgetHost使用选项包来向AppWidgetProvider传递关于如何显示窗口小部件的信息（例如，大小范围，以及窗口小部件是在屏幕上还是主屏幕上）。 此信息允许AppWidgetProvider根据显示的方式和位置来定制窗口小部件的内容和外观。 您可以使用updateAppWidgetOptions（）和updateAppWidgetSize（）来修改应用程序小部件的包。 这两种方法都会触发对AppWidgetProvider的回调。







一、绑定应用程序小部件

当用户将应用程序小部件添加到主机时，会发生称为绑定的进程。 绑定是指将特定应用程序小部件ID与特定主机和特定AppWidgetProvider相关联。 有不同的方法来实现这一点，具体取决于您的应用程序正在运行的Android版本。





一）、在Android 4.0及更低版本上绑定应用程序小部件

在运行Android 4.0及更低版本的设备上，用户通过一个允许用户选择窗口小部件的系统活动来添加应用程序小部件。 这隐含地执行权限检查，也就是通过添加应用程序小部件，用户隐式地授予您的应用程序的权限，以向应用程序添加应用程序小部件。 这是一个例子，说明了这种方法，取自原始的Launcher。 在这个片段中，一个事件处理程序使用请求代码REQUEST_PICK_APPWIDGET调用startActivityForResult（）来响应用户操作：




```java
private static final int REQUEST_CREATE_APPWIDGET = 5;
private static final int REQUEST_PICK_APPWIDGET = 9;
...
public void onClick(DialogInterface dialog, int which) {
    switch (which) {
    ...
        case AddAdapter.ITEM_APPWIDGET: {
            ...
            int appWidgetId =
                    Launcher.this.mAppWidgetHost.allocateAppWidgetId();
            Intent pickIntent =
                    new Intent(AppWidgetManager.ACTION_APPWIDGET_PICK);
            pickIntent.putExtra
                    (AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId);
            ...
            startActivityForResult(pickIntent, REQUEST_PICK_APPWIDGET);
            break;
    }
    ...
}
```


当系统活动结束时，它会将用户选择的应用程序小部件的结果返回给您的活动。 在以下示例中，活动通过调用addAppWidget（）来添加应用程序小部件：



```java
public final class Launcher extends Activity
        implements View.OnClickListener, OnLongClickListener {
    ...
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        mWaitingForResult = false;

        if (resultCode == RESULT_OK && mAddItemCellInfo != null) {
            switch (requestCode) {
                ...
                case REQUEST_PICK_APPWIDGET:
                    addAppWidget(data);
                    break;
                case REQUEST_CREATE_APPWIDGET:
                    completeAddAppWidget(data, mAddItemCellInfo, !mDesktopLocked);
                    break;
                }
        }
        ...
    }
}
```


addAppWidget（）方法检查应用程序小部件是否需要在添加之前进行配置：



```java
void addAppWidget(Intent data) {
    int appWidgetId = data.getIntExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, -1);

    String customWidget = data.getStringExtra(EXTRA_CUSTOM_WIDGET);
    AppWidgetProviderInfo appWidget =
            mAppWidgetManager.getAppWidgetInfo(appWidgetId);

    if (appWidget.configure != null) {
        // Launch over to configure widget, if needed.
        Intent intent = new Intent(AppWidgetManager.ACTION_APPWIDGET_CONFIGURE);
        intent.setComponent(appWidget.configure);
        intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId);
        startActivityForResult(intent, REQUEST_CREATE_APPWIDGET);
    } else {
        // Otherwise, finish adding the widget.
    }
}
```


有关配置的更多讨论，请参阅创建应用程序小部件配置活动课程。




一旦应用程序小部件准备就绪，下一步是将其添加到工作区的实际工作中。 原始的Launcher会使用一个名为completeAddAppWidget（）的方法来执行此操作。





二）、在Android 4.1及更高版本上绑定应用程序小部件

Android 4.1为更精简的绑定过程添加了API。 这些API还使得主机可以提供用于绑定的自定义UI。 要使用此改进的过程，您的应用程序必须在其清单中声明BIND_APPWIDGET权限：



`<uses-permission android:name="android.permission.BIND_APPWIDGET" />`

但这只是第一步。 在运行时，用户必须明确授予您的应用程序权限，以允许它将应用程序小部件添加到主机。 要测试您的应用程序是否具有添加小部件的权限，请使用bindAppWidgetIdIfAllowed（）方法。 如果bindAppWidgetIdIfAllowed（）返回false，您的应用程序必须显示一个对话框，提示用户授予权限（“允许”或“始终允许”）覆盖所有未来的应用程序小部件添加。 此代码段提供了如何显示对话框的示例：



```java
Intent intent = new Intent(AppWidgetManager.ACTION_APPWIDGET_BIND);
intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, appWidgetId);
intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_PROVIDER, info.componentName);
// This is the options bundle discussed above
intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_OPTIONS, options);
startActivityForResult(intent, REQUEST_BIND_APPWIDGET);
```


主机还必须检查用户是否添加了需要配置的应用程序小部件。 有关此主题的更多讨论，请参阅创建应用程序小部件配置活动。







二、主机责任

开发人员可以使用AppWidgetProviderInfo元数据为窗口小部件指定多个配置设置。 下面更详细讨论的这些配置，其选项可以由主机从与widget提供程序相关联的AppWidgetProviderInfo对象中检索。





无论您的Android版本如何，所有主机都有以下职责：


1、添加窗口小部件时，必须如上所述分配窗口小部件ID。 您还必须确保当从主机中删除小部件时，您可以调用deleteAppWidgetId（）来释放widget ID。




2、添加窗口小部件时，请确保启动其配置活动（如果存在），如从配置活动更新应用程序小部件中所述。 这是许多应用程序小部件在正确显示之前的必要步骤。




3、每个应用程序小部件按照AppWidgetProviderInfo元数据（使用android：minWidth和android：minHeight）定义，以dps为单位指定最小宽度和高度。 确保小部件至少布置了定义中的的dps。 例如，许多主机在网格中对齐图标和小部件。 在这种情况下，默认主机应使用满足minWidth和minHeight约束的最小数量的单元格添加应用程序小部件。




除了上述要求之外，特定平台版本还介绍了在主机上增加新职责的功能。 这些在以下部分中描述。





一）、Android 3.0

Android 3.0（API Level 11）引入了小部件指定autoAdvanceViewId（）的功能。 此视图ID应指向Advanceable的实例，例如StackView或AdapterViewFlipper。 这表示主机应该以主机认为适当的间隔在该视图上调用advance（）（考虑到是否有意义的推进小部件 - 例如，如果主机 在另一个页面上，或者如果屏幕被关闭，主机可能不想推进小部件）





二）、Android 3.1

Android 3.1（API Level 12）引入了调整小部件大小的功能。 小部件可以使用AppWidgetProviderInfo元数据中的android：resizeMode属性指定它可以调整大小，并指示它是否支持水平和/或垂直调整大小。 在Android 4.0（API Level 14）中引入，该小部件还可以指定一个android：minResizeWidth和/或android：minResizeHeight。





主机的责任是使窗口小部件可以按照窗口小部件的指定进行水平和/或垂直调整。 指定其可调整大小的小部件可以任意调整大小，但不应调整为小于android：minResizeWidth和android：minResizeHeight指定的值。 有关示例实现，请参阅Launcher2中的AppWidgetResizeFrame。





三）、Android 4.0

Android 4.0（API Level 15）引入了填充策略的更改，将责任置于主机管理填充。 从4.0开始，应用程序小部件不再包含自己的填充。 相反，系统会根据当前屏幕的特征为每个小部件添加填充。 这导致网格中的小部件更加统一和一致的呈现。 为了协助托管应用程序小部件的应用程序，该平台提供了getDefaultPaddingForWidget（）方法。 当计算要分配给窗口小部件的单元格数量时，应用程序可以调用此方法来获取系统定义的填充并对其进行计算。





四）、Android 4.1

Android 4.1（API级别16）添加了一个API，允许窗口小部件提供程序获取有关其窗口小部件实例被托管的环境的更详细信息。 具体来说，主机向窗口小部件提供者提示窗口小部件正在显示的大小。 主机负责提供此尺寸信息。





主机通过updateAppWidgetSize（）提供此信息。尺寸以dps指定为最小和最大宽度/高度。指定范围（与固定大小相反）的原因是因为小部件的宽度和高度可能会随方向而变化。您不希望主机必须更新其所有的窗口小部件，因为这可能导致系统严重缓慢的运行。这些值应该被更新在安装小部件时——任何时候小部件调整大小，以及任何时候启动器在给定引导中首次infate小部件（由于值不会在引导时保持不动）。





五）、Android 4.2

Android 4.2（API级别17）增加了在绑定时指定选项包的功能。 这是指定应用程序小部件选项（包括大小）的理想方式，因为它使AppWidgetProvider可以立即访问第一次更新时的选项数据。 这可以通过使用bindAppWidgetIdIfAllowed（）方法来实现。 有关此主题的更多讨论，请参阅绑定应用程序小部件。





Android 4.2还引入了锁屏小部件。 当在锁屏上托管小部件时，主机必须在应用程序小部件选项包中指定此信息（AppWidgetProvider可以使用此信息适当地对窗口小部件进行风格）。 要将窗口小部件指定为锁屏窗口小部件，请使用updateAppWidgetOptions（），并将值OPID_APPWIDGET_HOST_CATEGORY包含为值WIDGET_CATEGORY_KEYGUARD。 此选项默认为WIDGET_CATEGORY_HOME_SCREEN，因此不需要为主屏幕主机设置此选项





确保您的主机只添加适用于您的应用程序的应用程序小部件，例如，如果主机是主屏幕，请确保AppWidgetProviderInfo元数据中的android：widgetCategory属性包含标志WIDGET_CATEGORY_HOME_SCREEN。 同样，对于锁屏，请确保该字段包含标志WIDGET_CATEGORY_KEYGUARD。 有关此主题的更多讨论，请参阅在锁屏上启用应用程序小部件。




