
# [android] 百度地图开发 (一).申请AK显示地图及解决显示空白网格问题 - 杨秀璋的专栏 - CSDN博客

2014年12月22日 02:14:50[Eastmount](https://me.csdn.net/Eastmount)阅读数：9611所属专栏：[Android实例开发与学习](https://blog.csdn.net/column/details/eastmount-android.html)



**最近做android百度地图,但是使用baidumapapi_v2_3_1.jar和libBaiduMapSDK_v2_3_1.so显示百度地图时总是遇到问题——只显示网格而没有显示地图,网络连接和APIKey申请都是正确的,就是不知道为什么不能显示,网上也有很多人遇到这个问题,有的是SDK更新不兼容.而且网上很多百度地图都是使用V2.3.1版本,后来没有办法只有通过baidumapapi_v2_4_1.jar完成显示地图.**
**其中需要注意的是使用2.3.1时调用manager.init("APIKey",null)初始化地图,而2.4.1版本是通过在AndroidManifest.xml的meta-data中输入AK.**
**PS:这仅仅是一篇Android开发百度地图的基础文章,网上资料也很多,但该方法可行.参考郭神《Android第一行代码》中方法也是v2.3.1只能显示网格而没有地图.**

## 一.开发前申请APIKey
**参考资料：**
**官方开发SDK指南**http://developer.baidu.com/map/sdkandev-14.htm
**百度地图申请密钥**http://lbsyun.baidu.com/apiconsole/key
**首先,注册成为一名百度开发者.**
![](https://img-blog.csdn.net/20141221213812819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**提交注册成功后如下图所示.**
![](https://img-blog.csdn.net/20141221214004297)
**点击"去创建应用>>",如下图所示,这里显示你的应用列表.**
![](https://img-blog.csdn.net/20141221214106978)
**此时点击“创建应用”即可申请APIKey,选择类型以前我们见过的博客书籍通常是选择"for mobile"应用类型,这里已经没有该选项了,故我选择"for Android",同时输入安全码!**
![](https://img-blog.csdn.net/20141221220503694)
这里你可能疑惑"安全码"是什么?它是由"数字签名;包名"组成,其中数字签名是我们打包程序时keystore的SHA1指纹,通过点击Eclipse的"窗体(Window)->首选项(Preferences)->Android->Build"查看,同时也可参照上面官方文档链接通过cmd获取.而包名就是应用程序对应的包名.如下:
E5:BA:71:31:9D:A3:BF:92:8D:2E:8F:3A:6D:0A:93:5B:3D:36:59:08;com.example.baidumapshow
![](https://img-blog.csdn.net/20141221221250312)
点击"提交"后即可获取的AK：QwaNhFQ0ty2QmdYh3Nrr0gQx



![](https://img-blog.csdn.net/20141222001700099)




## 二.配置项目调用SDK
**下载Android SDK地址：**
**http://developer.baidu.com/map/sdkandev-download.htm**
**在创建好的应用程序BaiduMapShow项目中libs目录下复制baidumapapi_v2_4_1.jar文件,同时libs目录下新建armeabi目录,并拷贝libBaiduMapSDK_v2_4_1.so文件.如下图所示:**
![](https://img-blog.csdn.net/20141222003058359)
**其中该工程包名为com.eample.baidumapshow,libs目录用于存放第三方Jar包,armeabi目录存放NDK生成so文件调用其C/C++函数库.同时需要导入jar包,右键工程->"Properties(属性)"->"Java构建路径",如下图所示添加JAR.**
![](https://img-blog.csdn.net/20141222004006365)

## 三.源代码显示百度地图
**1.activity_main.xml布局文件**
**布局文件中通过加载百度地图控件MapView,它是由百度提供的自定义控件故需要加上完整包名,同时该控件需要接受点击事件clickable设置为true.**
```python
<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/container"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context="com.example.baidumapshow.MainActivity"
    tools:ignore="MergeRootFrame" >
    <com.baidu.mapapi.map.MapView
        android:id="@+id/map_view"
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:clickable="true" />
</FrameLayout>
```
**2.MainActivity.java文件**
```python
public class MainActivity extends Activity {
	
	//BMapManager 对象管理地图、定位、搜索功能
	private BMapManager mBMapManager;  
	//MapView 地图主控件 
	private MapView mapView = null;  
	//MapController 完成地图控制
	private MapController mMapController = null;   
	//MKMapViewListener 用于处理地图事件回调 
	MKMapViewListener mMapListener = null;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        /**
         * 创建对象BMapManager并初始化操作
         * V2.3.1中init(APIKey,null) V2.4.1在AndroidManifest中赋值AK
         * 注意 初始化操作在setContentView()前
         */
        mBMapManager = new BMapManager(getApplication());  
        mBMapManager.init(null); 
        setContentView(R.layout.activity_main);  
        
        mapView = (MapView) findViewById(R.id.map_view);  
        //获取地图控制器 取得MapController实例
        mMapController = mapView.getController();  
        //设置地图是否响应点击事件
        mMapController.enableClick(true);  
        //设置地图缩放级别 3-19级别 级别越高 信息越详细
        mMapController.setZoom(16);  
        //显示内置缩放控件
        mapView.setBuiltInZoomControls(true);  
          
        /**
         * 获取学校经纬度 设置地图中心点
         */
        GeoPoint point = new GeoPoint((int)(39.96703 * 1E6), (int)(116.323772 * 1E6));  
        mMapController.setCenter(point);  
        mapView.regMapViewListener(mBMapManager, new MKMapViewListener() {  
              
            /** 
             * 地图移动完成时会回调此接口方法 
             */  
            @Override  
            public void onMapMoveFinish() {  
            	Toast.makeText(MainActivity.this, "地图移动", Toast.LENGTH_SHORT).show();
            }  
              
            /** 
             * 地图加载完毕回调此接口方法 
             */  
            @Override  
            public void onMapLoadFinish() {  
            	Toast.makeText(MainActivity.this, "地图载入", Toast.LENGTH_SHORT).show();
            }  
              
            /** 
             *  地图完成带动画的操作（如: animationTo()）后，此回调被触发 
             */  
            @Override  
            public void onMapAnimationFinish() {  
                  
            }  
              
            /** 
             *  当调用过 mMapView.getCurrentMap()后，此回调会被触发 
             *  可在此保存截图至存储设备 
             */  
            @Override  
            public void onGetCurrentMap(Bitmap arg0) {  
                 
            }  
              
            /** 
             * 点击地图上被标记的点回调此方法 
             *  
             */  
            @Override  
            public void onClickMapPoi(MapPoi arg0) {  
                if (arg0 != null){  
                    Toast.makeText(MainActivity.this, arg0.strText, Toast.LENGTH_SHORT).show();
                }  
            }  
        });  
    }
    
    @Override
	protected void onResume() {
		mapView.onResume();
		if (mBMapManager != null) {
			mBMapManager.start();
		}
		super.onResume();
	}
    
    @Override
	protected void onDestroy() {
		mapView.destroy();
		if (mBMapManager != null) {
			mBMapManager.destroy();
			mBMapManager = null;
		}
		super.onDestroy();
	}
	@Override
	protected void onPause() {
		mapView.onPause();
		if (mBMapManager != null) {
			mBMapManager.stop();
		}
		super.onPause();
	}
}
```
3.AndroidManifest.xml中申明权限和APIKey
其中最重要的是添加APIKey,方法mBMapManager.init(null)调用.
<meta-data
android:name="com.baidu.lbsapi.API_KEY"
android:value="QwaNhFQ0ty2QmdYh3Nrr0gQx">
</meta-data>
```python
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.baidumapshow"
    android:versionCode="1"
    android:versionName="1.0" >
    <uses-sdk
        android:minSdkVersion="19"
        android:targetSdkVersion="19" />
    <!-- 获取网络状态  -->
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <!-- 访问网络 -->
    <uses-permission android:name="android.permission.INTERNET" />
    <!-- 获取WiFi状态 -->
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
    <!-- 允许程序写入外部存储,如SD卡上写文件 -->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.WRITE_SETTINGS" />
    <!-- 读取电话状态 -->
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.CALL_PHONE" />
    <!-- 获取精确位置 GPS芯片接收卫星的定位信息,定位精度达10米以内 -->
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
    <!-- 通过WiFi或移动基站的方式获取用户错略的经纬度信息 -->
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    <!-- 获取模拟定位信息 -->
    <uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_GPS" />
    
    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <meta-data   
    		android:name="com.baidu.lbsapi.API_KEY"  
    		android:value="QwaNhFQ0ty2QmdYh3Nrr0gQx">
        </meta-data> 
        <activity
            android:name="com.example.baidumapshow.MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```
**4.运行效果如下图所示**
![](https://img-blog.csdn.net/20141222020126390)![](https://img-blog.csdn.net/20141222020102216)
**最后希望文章对大家有所帮助,这是我帮同学做百度地图简单研究的一篇基础性文章,其中后面准备讲讲百度地图POI兴趣搜索和添加标注等功能吧!对于前面的问题我也不知道为什么总是显示网格而没有地图,但是通过V2_4_1能显示地图.**
**我猜测其原因是以前申请的APIKey调用方法可以实现显示地图,但是由于百度新更新的SDK方法不兼容,现在申请的APIKey需要使用新的方法,故上面的才能显示,个人意见,不知道是否正确啊!**
**(By:Eastmount 2014-12-22 夜2点****http://blog.csdn.net/eastmount/****)**
**参考资料:**
**1.郭霖大神《Android第一行代码》第11章基于百度地图的开发**
**2.xiaanming大神文章****http://blog.csdn.net/xiaanming/article/details/11171581**
**3.百度官方文档和文库****http://wenku.baidu.com/view/86bab0b3f524ccbff12184bd.html**


