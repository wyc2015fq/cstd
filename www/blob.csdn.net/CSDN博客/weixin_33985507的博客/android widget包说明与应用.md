# android widget包说明与应用 - weixin_33985507的博客 - CSDN博客
2015年03月18日 15:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
widget包是存放自定义组件
开发自定义组件；
其实Android API开发指南中的App Widgets章节 已经说得很清楚了，下面只是对自己的理解进行一次梳理。 
-- 
AppWidget 就是HomeScreen上显示的小部件，提供直观的交互操作。通过在HomeScreen中长按，在弹出的对话框中选择Widget部件来进行创建，长按部件后并拖动到垃圾箱里进行删除。同一个Widget部件可以同时创建多个。 
AppWidget的实现主要涉及到以下类： 
AppWidgetProvider 
RemoteViews 
AppWidgetManager 
1. 
首先需要提供一个定义了Widget界面布局的XML文件(位于res/layout/..)，需要注意的是使用的组件必须是RemoteViews所支持的，目前原生API中支持的组件如下： 
FrameLayout 
LinearLayout 
RelativeLayout 
AnalogClock 
Button 
Chronmeter 
ImageButton 
ImageView 
ProgressBar 
TextView 
*如果使用了除此之外的组件，则在Widget创建时会导致android.view.InflateExceptionn异常。 
PS:这就导致有一些功能或样式无法实现，如很基本的list或文本编辑框都是无法直接实现的。如果想自定义Widget中的View的话只能通过修改framework来提供相应组件的支持。 
2. 
然后需要提供一个xml文件来定义Widget的基本属性，放置到res/xml/..目录下。 
如果使用的是Eclipse可按如下操作： 
1) 在res/目录下创建xml/目录 
2)创建xml文件（名字可任意），选择类型为AppWidgetProvider 
3)在弹出的便捷界面进行参数设置 
主要设置的参数如下: 
minWidth: 定义Wdiget组件的宽度 
minHeight: 定义Wdiget组件的高度 
updatePeriodMillis: 更新的时间周期 
initialLayout: Widget的布局文件 
configure: 如果需要在启动前先启动一个Activity进行设置，在这里给出Activity的完整类名(后面会说到，与一般Activity的实现有些许差别) 
*Widget大小的计算![](http://quding0308.iteye.com/images/smiles/icon_sad.gif)单元格数*74)-2，API上说是为了防止像素计算时的整数舍入导致错所以-2...不是很明白 
一个完整的样例： 
Xml代码  收藏代码 
    <?xml version="1.0" encoding="utf-8"?>  
    <appwidget-provider xmlns:android="http://schemas.android.com/apk/res/android"  
          android:minWidth="80dp"  
          android:minHeight="32dp"  
          android:updatePeriodMillis="86400000"  
          android:initialLayout="@layout/widget_provider"  
          android:configure="com.demo.widget.MyWidgetConfiguration" >  
    </appwidget-provider>  
3. 
xml都定义好后，接下来就是创建一个继承自AppWidgetProvider的子类，AppWidgetProvider实际上就是一个BroadcastReceiver，里面提供了以下函数： 
onReceive(Context, Intent) 
onUpdate(Context , AppWidgetManager, int[] appWidgetIds) 
onEnabled(Context) 
onDeleted(Context, int[] appWidgetIds) 
onDisabled(Context) 
可通过重写以上函数来监听Widget状态的变化并进行相应的处理。 
以上函数具体调用情况归纳如下： 
[启动 - 无confiure Activity] 
onReceive 
onEnabled —— 第一个widget被显示 
onReceive 
onUpdate —— 刷新界面 
[启动 - 带confiuration Activity] 
onReceive 
onUpdate 
[拖动] 
<无状态变化> 
[周期更新] 
onReceive 
onUpdate 
[删除] 
onReceive 
onDeleted —— widget被删除 
onReceive 
onDisabled —— 最后一个widget被移除 
[启动时位置不够] 
onReceive 
onEnabled 
onReceive 
onUpdate 
onReceive 
onDeleted 
onReceive 
onDisabled 
*每次状态的变化会触发onReceive，一般该函数是不需要重写的。 
简单了解AppWidgetProvider之后，我们来看具体实现。 
这里创建一个MyAppWidgetProvider继承AppWidgetProvider: 
Java代码  收藏代码 
    public class MyWidgetProvider extends AppWidgetProvider {  
          static final String TAG = "widget";  
          /** 
           * 更新 
           */  
          public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds){  
                Log.i(TAG, "onUpdate");  
          }  
          /** 
           * 第一个Widget组件启动时触发 
           */  
          public void onEnabled(Context context){  
                Log.i(TAG, "onEnabled");  
          }  
          /** 
           * 最后一个Widget组件关闭时触发 
           */  
          public void onDisabled(Context context){  
                Log.i(TAG, "onDisabled");  
          }  
          /** 
           * 任一Widget组件被删除时触发 
           */  
          public void onDeleted(Context context, int[] appWidgetIds){  
                Log.i(TAG, "onDeleted");  
          }  
          /** 
           * 以上函数触发前会先触发该函数，一般不需要重写 
           */  
          public void onReceive(Context context, Intent intent){  
                Log.i(TAG, "onReceive");  
                super.onReceive(context, intent);  
          }  
    }  
其中onUpdate顾名思义是对Widget进行更新的，前面定义的更新周期就是作用于该函数的。 
Widget的更新与Activity不同，必须借助于RemoteViews和AppWidgetMananger。具体实现如下： 
Java代码  收藏代码 
    public void onUpdate(Context context, AppWidgetMananger appWidgetManager, int[] appWidgetIds){  
        int N = appWidgetIds.length; // 可能启动了多个Widget，appWidgetIds记录了这些Widget的ID  
        for(int i=0; i<N; i++){  
            RemoteViews  views = new RemoteViews(getPackageName(), R.layout.widget_views);  
            appWidgetManager.updateAppWidget(appWidgetIds[i], views);  
        }  
    }  
其中需要注意的是，虽然RemoteViews参数都是一样的，但是对于每个Widget最好都新创建一个再进行传递，否则会导致一些错误。具体可参考AppWidget RemoteViews 内存溢出 。 
其他函数的可以根据需要实现。 
由于无法获取到RemoteViews创建的界面中的元素，对于Widget中组件的操作只能通过RemoteViews所提供的有限的函数进行，常用的有： 
setOnClickPendingIntent(int viewId, PendingIntent pendingIntent) 
setProgressBar(int viewId, int max, int progress, boolean indeterminate) 
setTextViewText(int viewId, CharSequence text) 
setViewVisibility(int viewId, int visibility) 
详细函数列表可参考API中的RemoteViews类 。 
4. 
最后，更新AndroidManifest.xml。 
AppWidgetProvider对应一个receiver属性: 
Xml代码  收藏代码 
    <receiver android:name="MyWidgetProvider">  
                <intent-filter>  
                    <action android:name="android.appwidget.action.APPWIDGET_UPDATE"></action>  
                </intent-filter>  
                <meta-data android:resource="@xml/widget_property" android:name="android.appwidget.provider"></meta-data>  
            </receiver>  
5. 
提供Configuration Activity 
Configuration Activity是一个在Widget启动前先启动的Activity，方便用户对Widget的属性进行设置。 
在res/xml/...下对应的"属性文件"中添加configure字段指定启动的Activity，并在AndroidManifest.xml中该Activity下提供一个action为android.appwidget.action.APPWIDGET_CONFIGURE 的IntenFilter。 
需要注意的是， 
如果设置了Configure属性，则必须在指定的Activity中进行如下处理： 
1.在onCreate中setContentView()函数前添加setResult(RESULT_CANCLE) ，这样如果在Activity初始化完成前按下了BACK按键，则Widget不会启动； 
2.在setContentView()函数之后（不一定要在onCreate中，在Activity退出前即可），添加如下设置以指定需要启动的Widget： 
Java代码  收藏代码 
    int mAppWidgetId = extras.getInt(AppWidgetManager.EXTRA_APPWIDGET_ID, AppWidgetManager.INVALID_APPWIDGET_ID);  
    Intent resultValue = new Intent();  
    resultValue.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, mAppWidgetId);  
    setResult(RESULT_OK, resultValue);  
否则会导致退出Activity后Widget不启动。 
>> Widget创建步骤汇总： 
1.定义Widget布局XML -> res/layout/... 
2.定义Widget属性文件(xml) -> res/xml/... 
3. 创建AppWidgetProider子类，实现onUpdate()等函数，在manifest中注册receiver，添加一个action为 android.appwidget.action.APPWIDGET_UPDATE 的IntentFilter，并添加如下<meta- data>标识： 
Xml代码  收藏代码 
    <meta-data android:resource="@xml/<属性文件xml>" android:name="android.appwidget.provider"></meta-data>  
4. 创建Coniguration Activity（注意处理好setResult），添加到属性文件中的Configure属性，在manifest中注册activity，添加一个 action为android.appwidget.action.APPWIDGET_CONFIGURE 的IntentFilter 
最后附上一个完整的例子， 
实现思路如下: 
1. 提供一个Configuration Activity，这里只简单显示一行文字； 
2.退出后Widget启动； 
3.点击Widget中的按钮会启动一个Activity
（转自：http://quding0308.iteye.com/blog/1164276）
