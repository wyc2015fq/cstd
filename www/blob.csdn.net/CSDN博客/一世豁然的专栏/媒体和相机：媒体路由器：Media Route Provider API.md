# 媒体和相机：媒体路由器：Media Route Provider API - 一世豁然的专栏 - CSDN博客





2017年05月05日 21:37:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：825








Android媒体路由器框架允许制造商通过称为MediaRouteProvider的标准接口在其设备上启用播放。 路由提供商定义了用于在接收机设备上播放媒体的公共接口，使得可以从支持媒体路由的任何Android应用程序在设备上播放媒体。




本指南讨论如何为接收机设备创建媒体路由提供商，并使其可用于在Android上运行的其他媒体播放应用程序。








一、概述

Android媒体路由器框架使媒体应用程序开发人员和媒体播放设备制造商能够通过公共API和通用用户界面进行连接。 然后，实现MediaRouter接口的应用程序开发人员可以连接到框架，并向参与媒体路由器框架的设备播放内容。 媒体播放设备制造商可以通过发布MediaRouteProvider来参与该框架，该MediaRouteProvider允许其他应用程序连接到接收机设备上并播放媒体。 图1说明了应用程序如何通过媒体路由器框架连接到接收设备。


![](https://img-blog.csdn.net/20170505210749371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.媒体路由提供程序类如何提供从媒体应用程序到接收器设备的通信的概述。





为您的接收机设备构建媒体路由提供商时，提供商提供以下目的：


1、描述和发布接收机设备的功能，使其他应用程序可以发现并使用其播放功能。

2、包装接收机设备的编程接口及其通信传输机制，使设备与媒体路由器框架兼容。




一）、分配路由提供商

媒体路由提供商作为Android应用程序的一部分分发。 您的路由提供商可以通过扩展MediaRouteProviderService或将MediaRouteProvider的实现包含在您自己的服务中并为媒体路由提供者声明一个意图过滤器，使其可以应用于其他应用程序。 这些步骤允许其他应用程序发现和使用您的媒体路由。





注意：包含媒体路由提供商的应用程序还可以包括到路由提供商的MediaRouter接口，但这不是必需的。





二）、媒体路由器库

媒体路由器API在v7-mediarouter支持库中定义。 您必须将此库添加到应用程序开发项目中。 有关向项目添加支持库的更多信息，请参阅支持库设置。





注意：请务必使用媒体路由器框架的android.support.v7.media实现。 不要使用旧的android.media包。








二、创建提供者服务

媒体路由器框架必须能够发现并连接到媒体路由提供商，以允许其他应用程序使用您的路由。 为了做到这一点，媒体路由器框架寻找声明媒体路由提供者意图行动的应用程序。 当另一个应用程序想要连接到您的提供商时，框架必须能够调用并连接到它，因此您的提供者必须封装在服务中。





以下示例代码显示了媒体路由提供程序服务的声明和清单中的intent过滤器，允许它由媒体路由器框架发现和使用：




```
<service android:name=".provider.SampleMediaRouteProviderService"
    android:label="@string/sample_media_route_provider_service"
    android:process=":mrp">
    <intent-filter>
        <action android:name="android.media.MediaRouteProviderService" />
    </intent-filter>
</service>
```


此清单示例声明一个包裹实际媒体路由提供程序类的服务。 Android媒体路由器框架提供MediaRouteProviderService类，用作媒体路由提供商的服务包装。 以下示例代码演示如何使用此包装类：



```java
public class SampleMediaRouteProviderService extends MediaRouteProviderService {

    @Override
    public MediaRouteProvider onCreateMediaRouteProvider() {
        return new SampleMediaRouteProvider(this);
    }
}
```



三、指定路由能力

连接到媒体路由器框架的应用程序可以通过应用程序的清单声明发现您的媒体路由，但也需要了解您提供的媒体路由的功能。 媒体路线可以是不同类型，具有不同的功能，其他应用程序需要能够发现这些细节，以确定它们是否与您的路线兼容。





媒体路由器框架允许您通过IntentFilter对象，MediaRouteDescriptor对象和MediaRouteProviderDescriptor来定义和发布媒体路由的功能。 本节介绍如何使用这些类来发布其他应用程序的媒体路由的详细信息。





一）、路由类别

作为媒体路由提供商的编程描述的一部分，您必须指定您的提供商是否支持远程播放，辅助输出或两者兼容。 这些是由媒体路由器框架提供的路由类别：


1、`[CATEGORY_LIVE_AUDIO](https://developer.android.google.cn/reference/android/support/v7/media/MediaControlIntent.html#CATEGORY_LIVE_AUDIO)` -将音频输出到辅助输出设备，例如启用无线功能的音乐系统。

2、`[CATEGORY_LIVE_VIDEO](https://developer.android.google.cn/reference/android/support/v7/media/MediaControlIntent.html#CATEGORY_LIVE_VIDEO)` -将视频输出到辅助输出设备，如无线显示设备。

3、`[CATEGORY_REMOTE_PLAYBACK](https://developer.android.google.cn/reference/android/support/v7/media/MediaControlIntent.html#CATEGORY_REMOTE_PLAYBACK)` -在处理媒体检索，解码和播放在单独设备上视频或音频，例如Chromecast设备。




为了将这些设置包含在媒体路由的描述中，您将它们插入到IntentFilter对象中，该对象稍后将添加到MediaRouteDescriptor对象中：




```java
public final class SampleMediaRouteProvider extends MediaRouteProvider {
    private static final ArrayList<IntentFilter> CONTROL_FILTERS_BASIC;
    static {
        IntentFilter videoPlayback = new IntentFilter();
        videoPlayback.addCategory(MediaControlIntent.CATEGORY_REMOTE_PLAYBACK);
        CONTROL_FILTERS_BASIC = new ArrayList<IntentFilter>();
        CONTROL_FILTERS_BASIC.add(videoPlayback);
    }
}
```


如果指定CATEGORY_REMOTE_PLAYBACK意图，您还必须定义媒体路由提供商支持哪些媒体类型和播放控件。 下一节将介绍如何为设备指定这些设置。




二）、媒体类型和协议

远程播放设备的媒体路由提供商必须指定其支持的媒体类型和传输协议。 您可以使用IntentFilter类和该对象的addDataScheme（）和addDataType（）方法来指定这些设置。 以下代码段演示了如何使用http，https和实时流协议（RTSP）来定义支持远程视频播放的意图过滤器：




```java
public final class SampleMediaRouteProvider extends MediaRouteProvider {

    private static final ArrayList<IntentFilter> CONTROL_FILTERS_BASIC;

    static {
        IntentFilter videoPlayback = new IntentFilter();
        videoPlayback.addCategory(MediaControlIntent.CATEGORY_REMOTE_PLAYBACK);
        videoPlayback.addAction(MediaControlIntent.ACTION_PLAY);
        videoPlayback.addDataScheme("http");
        videoPlayback.addDataScheme("https");
        videoPlayback.addDataScheme("rtsp");
        addDataTypeUnchecked(videoPlayback, "video/*");
        CONTROL_FILTERS_BASIC = new ArrayList<IntentFilter>();
        CONTROL_FILTERS_BASIC.add(videoPlayback);
    }
    ...

    private static void addDataTypeUnchecked(IntentFilter filter, String type) {
        try {
            filter.addDataType(type);
        } catch (MalformedMimeTypeException ex) {
            throw new RuntimeException(ex);
        }
    }
}
```



三）、播放控制

提供远程播放的媒体路由提供商必须指定其支持的媒体控制类型。 这些是媒体路由可以提供的一般控制类型：


1、播放控制：如播放，暂停，倒带和快进。

2、排队功能：这允许发送应用程序从由接收机设备维护的播放列表中添加和删除项目。

3、会话功能：这防止通过使接收机设备向请求应用提供会话ID，然后用每个随后的播放控制请求来检查该ID，从而发送应用彼此干扰。




以下代码示例演示如何构建一个支持基本媒体路由播放控件的意图过滤器：




```java
public final class SampleMediaRouteProvider extends MediaRouteProvider {
    private static final ArrayList<IntentFilter> CONTROL_FILTERS_BASIC;
    static {
        ...
        IntentFilter playControls = new IntentFilter();
        playControls.addCategory(MediaControlIntent.CATEGORY_REMOTE_PLAYBACK);
        playControls.addAction(MediaControlIntent.ACTION_SEEK);
        playControls.addAction(MediaControlIntent.ACTION_GET_STATUS);
        playControls.addAction(MediaControlIntent.ACTION_PAUSE);
        playControls.addAction(MediaControlIntent.ACTION_RESUME);
        playControls.addAction(MediaControlIntent.ACTION_STOP);
        CONTROL_FILTERS_BASIC = new ArrayList<IntentFilter>();
        CONTROL_FILTERS_BASIC.add(videoPlayback);
        CONTROL_FILTERS_BASIC.add(playControls);
    }
    ...
}
```


有关可用播放控制意图的更多信息，请参阅MediaControlIntent类。




四）、MediaRouteProviderDescriptor

使用IntentFilter对象定义媒体路由的功能后，您可以创建一个描述符对象，以便发布到Android媒体路由器框架。 此描述符对象包含媒体路由功能的具体细节，以便其他应用程序可以确定如何与媒体路由进行交互。





以下示例代码演示如何将以前创建的意图过滤器添加到MediaRouteProviderDescriptor，并设置描述符以供媒体路由器框架使用：




```java
public SampleMediaRouteProvider(Context context) {
    super(context);
    publishRoutes();
}

private void publishRoutes() {
    Resources r = getContext().getResources();
    // Create a route descriptor using previously created IntentFilters
    MediaRouteDescriptor routeDescriptor = new MediaRouteDescriptor.Builder(
            VARIABLE_VOLUME_BASIC_ROUTE_ID,
            r.getString(R.string.variable_volume_basic_route_name))
            .setDescription(r.getString(R.string.sample_route_description))
            .addControlFilters(CONTROL_FILTERS_BASIC)
            .setPlaybackStream(AudioManager.STREAM_MUSIC)
            .setPlaybackType(MediaRouter.RouteInfo.PLAYBACK_TYPE_REMOTE)
            .setVolumeHandling(MediaRouter.RouteInfo.PLAYBACK_VOLUME_VARIABLE)
            .setVolumeMax(VOLUME_MAX)
            .setVolume(mVolume)
            .build();
    // Add the route descriptor to the provider descriptor
    MediaRouteProviderDescriptor providerDescriptor =
            new MediaRouteProviderDescriptor.Builder()
            .addRoute(routeDescriptor)
            .build();

    // Publish the descriptor to the framework
    setDescriptor(providerDescriptor);
}
```


有关可用描述符设置的更多信息，请参阅MediaRouter描述符和Media RouteProvider描述符的参考文档








四、控制路由

当应用程序连接到媒体路由提供商时，提供商通过其他应用程序发送到您的路由的媒体路由器框架接收播放命令。 要处理这些请求，您必须提供一个MediaRouteProvider.RouteController类的实现，该类可处理命令并处理与接收器设备的实际通信。





媒体路由器框架调用路由提供程序的onCreateRouteController（）方法来获取此类的实例，然后将请求路由到该实例。 这些是MediaRouteProvider.RouteController类的关键方法，您必须为媒体路由提供程序实现：


1、`[onSelect()](https://developer.android.google.cn/reference/android/support/v7/media/MediaRouteProvider.RouteController.html#onSelect())` -当应用程序选择要播放的路由时调用。
 您可以使用此方法进行媒体播放开始之前可能需要的任何准备工作。

2、`[onControlRequest()](https://developer.android.google.cn/reference/android/support/v7/media/MediaRouteProvider.RouteController.html#onControlRequest(android.content.Intent,%20android.support.v7.media.MediaRouter.ControlRequestCallback))` -向接收设备发送特定播放命令。

3、`[onSetVolume()](https://developer.android.google.cn/reference/android/support/v7/media/MediaRouteProvider.RouteController.html#onSetVolume(int))` -向接收设备发送请求以将播放音量设置为特定值。

4、[onUpdateVolume()](https://developer.android.google.cn/reference/android/support/v7/media/MediaRouteProvider.RouteController.html#onUpdateVolume(int))-向接收设备发送请求，将播放音量修改指定的量。

5、[onUnselect()](https://developer.android.google.cn/reference/android/support/v7/media/MediaRouteProvider.RouteController.html#onUnselect())-当应用程序取消选择路由时调用。

6、[onRelease()](https://developer.android.google.cn/reference/android/support/v7/media/MediaRouteProvider.RouteController.html#onRelease())-当框架不再需要路由时，调用它，从而释放其资源。




除音量更改外的所有播放控制请求均被引导至onControlRequest（）方法。 您执行此方法必须解析控制请求并对其进行适当的响应。 这是一种处理远程播放媒体路由命令的方法的示例实现：




```java
private final class SampleRouteController extends
        MediaRouteProvider.RouteController {
    ...

    @Override
    public boolean onControlRequest(Intent intent, ControlRequestCallback callback) {

        String action = intent.getAction();

        if (intent.hasCategory(MediaControlIntent.CATEGORY_REMOTE_PLAYBACK)) {
            boolean success = false;
            if (action.equals(MediaControlIntent.ACTION_PLAY)) {
                success = handlePlay(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_ENQUEUE)) {
                success = handleEnqueue(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_REMOVE)) {
                success = handleRemove(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_SEEK)) {
                success = handleSeek(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_GET_STATUS)) {
                success = handleGetStatus(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_PAUSE)) {
                success = handlePause(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_RESUME)) {
                success = handleResume(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_STOP)) {
                success = handleStop(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_START_SESSION)) {
                success = handleStartSession(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_GET_SESSION_STATUS)) {
                success = handleGetSessionStatus(intent, callback);
            } else if (action.equals(MediaControlIntent.ACTION_END_SESSION)) {
                success = handleEndSession(intent, callback);
            }

            Log.d(TAG, mSessionManager.toString());
            return success;
        }
        return false;
    }
    ...
}
```


重要的是要了解MediaRouteProvider.RouteController类旨在作为媒体播放设备的API的包装器。 该类中的方法的实现完全取决于接收设备提供的编程接口。






