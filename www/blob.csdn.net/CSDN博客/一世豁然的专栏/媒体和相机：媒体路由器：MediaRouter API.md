# 媒体和相机：媒体路由器：MediaRouter API - 一世豁然的专栏 - CSDN博客





2017年05月05日 21:03:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：969








为了在应用程序中使用MediaRouter框架，您必须获取MediaRouter对象的实例，并附加MediaRouter.Callback对象以侦听路由事件。 通过路由相关联的MediaRouteProvider，内容发送一个媒体路由（除了在一些特殊情况下，如蓝牙输出设备）。 图1提供了用于在设备之间路由内容的类的高级视图。

![](https://img-blog.csdn.net/20170505201934132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.应用程序使用的关键媒体路由器类概述。





注意：如果您希望应用程式支持Google Cast装置，您应该使用Cast SDK，并将应用程式建成Cast Cast。 遵循Cast文档中的说明，而不是直接使用MediaRouter框架。








一、媒体路由按钮

Android应用程序应使用媒体路由按钮来控制媒体路由。 MediaRouter框架为按钮提供了一个标准界面，可帮助用户在可用时识别和使用路由。 媒体路由按钮通常放置在应用程序操作栏的右侧，如图2所示。


![](https://img-blog.csdn.net/20170505202245223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图2.操作栏中的媒体路由按钮。





当用户按下媒体路由按钮时，可用媒体路由出现在列表中，如图3所示。


![](https://img-blog.csdn.net/20170505202349615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图3.按媒体路由按钮后显示的可用媒体路由列表。





按照以下步骤创建媒体路由按钮：


1、使用AppCompatActivity

2、定义媒体路由按钮菜单项

3、创建一个MediaRouteSelector

4、将媒体路由按钮添加到操作栏

5、在活动生命周期中创建和管理MediaRouter.Callback方法




本节介绍前四个步骤。 下一节介绍回调方法。





一）\使用AppCompatActivity

当您在活动中使用媒体路由器框架时，您应该从AppCompatActivity扩展活动并导入包android.support.v7.media。 您必须将v7-appcompat和v7-mediarouter支持库添加到应用程序开发项目中。 有关向项目添加支持库的更多信息，请参阅支持库设置。





注意：请务必使用媒体路由器框架的android.support.v7.media实现。 不要使用旧的android.media包。





二）、定义媒体路由按钮菜单项

创建一个定义媒体路由按钮菜单项的xml文件。 该项目的操作应该是MediaRouteActionProvider类。 这是一个示例文件：




```
// myMediaRouteButtonMenuItem.xml
<?xml version="1.0" encoding="utf-8"?>
<menu xmlns:android="http://schemas.android.com/apk/res/android"
      xmlns:app="http://schemas.android.com/apk/res-auto"
      >

    <item android:id="@+id/media_route_menu_item"
        android:title="@string/media_route_menu_title"
        app:actionProviderClass="android.support.v7.app.MediaRouteActionProvider"
        app:showAsAction="always"
    />
</menu>
```



三）、创建一个MediaRouteSelector

媒体路由按钮菜单中显示的路由由MediaRouteSelector确定。 从AppCompatActivity扩展您的活动，并在创建活动时构建选择器，从onCreate（）方法调用MediaRouteSelector.Builder，如以下代码示例所示。 请注意，选择器保存在类变量中，允许的路由类型通过添加MediaControlIntent对象来指定：




```java
public class MediaRouterPlaybackActivity extends AppCompatActivity {
    private MediaRouteSelector mSelector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Create a route selector for the type of routes your app supports.
        mSelector = new MediaRouteSelector.Builder()
                // These are the framework-supported intents
                .addControlCategory(MediaControlIntent.CATEGORY_REMOTE_PLAYBACK)
                .build();
    }
}
```


对于大多数应用程序，所需的唯一路由类型是CATEGORY_REMOTE_PLAYBACK。 此路由类型将运行您的应用的设备视为遥控器。 连接的接收机设备处理所有内容数据检索，解码和回放。 支持Google Cast的应用（如Chromecast）的工作原理。




一些制造商支持一种称为“辅助输出”的特殊路由选项。 使用此路由，您的媒体应用程序将视频或音乐直接检索，渲染和流式传输到所选远程接收设备上的屏幕和/或扬声器。 使用辅助输出将内容发送到支持无线的音乐系统或视频显示器。 要启用这些设备的发现和选择，您需要将CATEGORY_LIVE_AUDIO或CATEGORY_LIVE_VIDEO控件类别添加到MediaRouteSelector。 您还需要创建和处理您自己的演示文稿对话框。





四）、将媒体路由按钮添加到操作栏

媒体路由菜单和MediaRouteSelector定义后，您可以将媒体路由按钮添加到活动中。 覆盖每个活动的onCreateOptionsMenu（）方法以添加选项菜单。




```java
@Override
public boolean onCreateOptionsMenu(Menu menu) {
    super.onCreateOptionsMenu(menu);

    // Inflate the menu and configure the media router action provider.
    getMenuInflater().inflate(R.menu.sample_media_router_menu, menu);

    // Attach the MediaRouteSelector to the menu item
    MenuItem mediaRouteMenuItem = menu.findItem(R.id.media_route_menu_item);
    MediaRouteActionProvider mediaRouteActionProvider =
            (MediaRouteActionProvider)MenuItemCompat.getActionProvider(
            mediaRouteMenuItem);
    // Attach the MediaRouteSelector that you built in onCreate()
    mediaRouteActionProvider.setRouteSelector(mSelector);

    // Return true to show the menu.
    return true;
}
```


有关在应用程序中实现操作栏的更多信息，请参阅“操作栏”开发人员指南。




您还可以在任何视图中将媒体路由按钮添加为MediaRouteButton。 您必须使用setRouteSelector（）方法将MediaRouteSelector附加到该按钮。 有关将媒体路由按钮合并到应用程序中的指导，请参阅Google Cast设计清单。








二、MediaRouter回调

所有在同一设备上运行的应用程序共享一个MediaRouter实例及其路由（由应用程序的MediaRouteSelector针对每个应用程序进行过滤）。 每个活动都使用MediaRouter.Callback方法与MediaRouter进行通信。 当用户选择，更改或断开路由时，MediaRouter会调用回调方法。





回调中有几种方法可以覆盖以接收关于路由事件的信息。 至少，您的MediaRouter.Callback类的实现应该覆盖onRouteSelected（）和onRouteUnselected（）。





由于MediaRouter是一个共享资源，您的应用程序需要管理其MediaRouter回调以响应通常的活动生命周期回调：


1、当创建活动（onCreate（Bundle））时，将获取一个指向MediaRouter的指针，并在应用程序的生命周期中保持它。

2、当活动变得可见时（onStart（）），将回调附加到MediaRouter，并在隐藏（onStop（））时将其分离。




以下代码示例演示如何创建和保存回调对象，如何获取MediaRouter的实例以及如何管理回调。 注意在onStart（）中附加回调时使用CALLBACK_FLAG_REQUEST_DISCOVERY标志。 这允许您的MediaRouteSelector刷新媒体路由按钮的可用路由列表。




```java
public class MediaRouterPlaybackActivity extends AppCompatActivity {
    private MediaRouter mMediaRouter;
    private MediaRouteSelector mSelector;

    // Variables to hold the currently selected route and its playback client
    private MediaRoute mRoute;
    private RemotePlaybackClient mRemotePlaybackClient;

    // Define the Callback object and its methods, save the object in a class variable
    private final MediaRouter.Callback mMediaRouterCallback =
            new MediaRouter.Callback() {

        @Override
        public void onRouteSelected(MediaRouter router, RouteInfo route) {
            Log.d(TAG, "onRouteSelected: route=" + route);

            if (route.supportsControlCategory(
                MediaControlIntent.CATEGORY_REMOTE_PLAYBACK)){
                // Stop local playback (if necessary)
                // ...

                // Save the new route
                mRoute = route;

                // Attach a new playback client
                mRemotePlaybackClient = new RemotePlaybackClient(this, mRoute);

                // Start remote playback (if necessary)
                // ...
            }
        }

        @Override
        public void onRouteUnselected(MediaRouter router, RouteInfo route, int reason) {
            Log.d(TAG, "onRouteUnselected: route=" + route);

            if (route.supportsControlCategory(
                MediaControlIntent.CATEGORY_REMOTE_PLAYBACK)){

                // Changed route: tear down previous client
                if (mRoute != null && mRemotePlaybackClient != null) {
                    mRemotePlaybackClient.release();
                    mRemotePlaybackClient = null;
                }

                // Save the new route
                mRoute = route;

                if (reason != MediaRouter.UNSELECT_REASON_ROUTE_CHANGED) {
                    // Resume local playback  (if necessary)
                    // ...
                }
            }
        }
    }


    // Retain a pointer to the MediaRouter
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Get the media router service.
        mMediaRouter = MediaRouter.getInstance(this);
        ...
    }

    // Use this callback to run your MediaRouteSelector to generate the list of available media routes
    @Override
    public void onStart() {
        mMediaRouter.addCallback(mSelector, mMediaRouterCallback,
                MediaRouter.CALLBACK_FLAG_REQUEST_DISCOVERY);
        super.onStart();
    }

    // Remove the selector on stop to tell the media router that it no longer
    // needs to discover routes for your app.
    @Override
    public void onStop() {
        mMediaRouter.removeCallback(mMediaRouterCallback);
        super.onStop();
    }
    ...
}
```


媒体路由器框架还提供了一个MediaRouteDiscoveryFragment类，该类负责为活动添加和删除回调。




注意：如果您正在撰写音乐播放应用程序并希望应用程序在后台播放音乐，则必须构建一个服务进行播放，并从服务的生命周期回调中调用媒体路由器框架。








三、控制远程播放路由

当您选择远程播放路线时，您的应用程序将用作遥控器。 路由另一端的设备处理所有内容数据检索，解码和播放功能。 应用程序UI中的控件使用RemotePlaybackClient对象与接收器设备通信。





RemotePlaybackClient类提供了管理内容播放的其他方法。 以下是RemotePlaybackClient类的一些关键播放方法：


1、play（） - 播放由Uri指定的特定媒体文件。

2、pause（） - 暂停当前播放的媒体轨道。

3、resume（） - 暂停命令后继续播放当前曲目。

4、seek（） - 移动到当前曲目中的特定位置。

5、release（） - 从您的应用程序中断开连接到远程播放设备。




您可以使用这些方法将操作附加到您在应用程序中提供的播放控件。 大多数这些方法还允许您包括回调对象，以便您可以监视播放任务或控制请求的进度。





RemotePlaybackClient类还支持多个媒体项目的队列，以便播放和管理媒体队列。




