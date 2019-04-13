
# Android百度地图之位置定位和附近查找代码简单实现 (上) - 杨秀璋的专栏 - CSDN博客

2016年01月06日 03:18:30[Eastmount](https://me.csdn.net/Eastmount)阅读数：27804所属专栏：[Android实例开发与学习](https://blog.csdn.net/column/details/eastmount-android.html)



很长时间没有做Android相关知识了，闲暇之余再弄了弄最新的百度地图API，主要是进行百度地图附近餐馆查找功能来练练手，同时熟悉下最新的API教程。文章比较基础，也希望对你有所帮助~参考前文：
[[android] 百度地图开发 (一).申请AK显示地图及解决显示空白网格问题](http://blog.csdn.net/eastmount/article/details/42064123)
[[android] 百度地图开发 (二).定位城市位置和城市POI搜索](http://blog.csdn.net/eastmount/article/details/42115735)
[[android] 百度地图开发 (三).定位当前位置及getLastKnownLocation总为空问题](http://blog.csdn.net/eastmount/article/details/42534721)
官方网址：[http://developer.baidu.com/map/](http://developer.baidu.com/map/)
Demo下载地址：

## 一. 配置百度API环境
获取百度地图API Key前，遇到的第一个问题是“小米手机或豌豆荚不能识别USB插口”。解决方法是安装QuickShortcutMaker软件，输入USB启动计算机USB连接。
参考地址：[http://www.miui.com/thread-1733895-1-1.html](http://www.miui.com/thread-1733895-1-1.html)
![](https://img-blog.csdn.net/20160104131328197)![](https://img-blog.csdn.net/20160104131346594)
百度地图AndroidSDK介绍如下：
[http://developer.baidu.com/map/wiki/index.php?title=androidsdk](http://developer.baidu.com/map/wiki/index.php?title=androidsdk)
**第一步：创建百度地图API Key**
在使用之前，您需要先申请密钥，且密钥和应用证书和包名绑定。
申请应用新地址：[http://lbsyun.baidu.com/apiconsole/key](http://lbsyun.baidu.com/apiconsole/key)
百度地图Android SDK申请密钥Key方法如下，也可参照我以前写过的一篇文章：
[http://developer.baidu.com/map/index.php?title=androidsdk/guide/key](http://developer.baidu.com/map/index.php?title=androidsdk/guide/key)
[http://blog.csdn.net/eastmount/article/details/42064123](http://blog.csdn.net/eastmount/article/details/42064123)
![](https://img-blog.csdn.net/20160104141141968)
其中数字签名SHA1通过点击Eclipse的 "窗体(Window)->首选项(Preferences)->Android->Build" 查看，而包名是创建工程BaiduMapRes项目中的包。
![](https://img-blog.csdn.net/20141221221250312)


**第二步：下载Android SDK并配置**
Android SDK v3.1.0下载地址：
[http://developer.baidu.com/map/sdkandev-download.htm](http://developer.baidu.com/map/sdkandev-download.htm)
最新下载地址，可以选择自己喜欢的开发资源，它会提供相应的文件包：
[http://lbsyun.baidu.com/sdk/download](http://lbsyun.baidu.com/sdk/download)
![](https://img-blog.csdn.net/20160105013055164)
在工程里新建libs文件夹，将开发包里的baidumapapi_vX_X_X.jar拷贝到libs根目录下，将libBaiduMapSDK_vX_X_X.so拷贝到libs\armeabi目录下，拷贝完成后的工程目录如下图所示：
![](https://img-blog.csdn.net/20160105013130188)![](https://img-blog.csdn.net/20160105013152710)
同时需要导入jar包,右键工程->"Properties(属性)"->"Java构建路径",选择添加JAR，选定baidumapapi_vX_X_X.jar，确定后返回。

## 二. Hello BaiduMapAndroid SDK
第一个百度地图程序参考网址，简单进行叙述下：[Hello BaiduMapAndroid](http://developer.baidu.com/map/index.php?title=androidsdk/guide/hellobaidumap)
**第一步：****在AndroidManifest中添加开发密钥、所需权限等信息**
在application中添加开发密钥<application><meta-data/></application>
 并添加相关权限。
```python
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.baidumapres"
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
    <uses-permission android:name="com.android.launcher.permission.READ_SETTINGS" />
    <uses-permission android:name="android.permission.WAKE_LOCK"/> 
    <uses-permission android:name="android.permission.GET_TASKS" /> 
	
    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        
        <meta-data  
       		android:name="com.baidu.lbsapi.API_KEY"  
        	android:value="OYl0tcWvGrWtWucQN2rhdxG8" />  
        	
        <activity
            android:name="com.example.baidumapres.MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
    
</manifest>
```
**第二步：****在布局xml文件中添加地图控件**
布局文件activity_main.xml中通过加载百度地图控件MapView，它是由百度提供的自定义控件故需要加上完整包名，同时该控件需要接受点击事件clickable设置为true。
```python
<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/container"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context="com.example.baidumapres.MainActivity"
    tools:ignore="MergeRootFrame" >
    
    <com.baidu.mapapi.map.MapView  
        android:id="@+id/map_view"  
        android:layout_width="match_parent"  
        android:layout_height="match_parent"  
        android:clickable="true" />  
  
</FrameLayout>
```
**第三步：****MainActivity.java文件，****创建地图Activity，管理地图生命周期**
```python
package com.example.baidumapres;
import java.util.ArrayList;
import java.util.List;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.OverlayOptions;
import com.baidu.mapapi.map.PolygonOptions;
import com.baidu.mapapi.map.Stroke;
import com.baidu.mapapi.model.LatLng;
import android.app.Activity;
import android.os.Bundle;
public class MainActivity extends Activity {
	
	//地图
	private MapView mMapView = null;  
	private BaiduMap mBaiduMap;
	
    @Override  
    protected void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);   
        //在使用SDK各组件之前初始化context信息，传入ApplicationContext  
        //注意该方法要再setContentView方法之前实现  
        SDKInitializer.initialize(getApplicationContext());  
        setContentView(R.layout.activity_main);  
        
        //获取地图控件引用  
        mMapView = (MapView) findViewById(R.id.map_view);  
        
        /*
         * 设置地图类型 普通地图
         * 卫星地图 
         * mBaiduMap.setMapType(BaiduMap.MAP_TYPE_SATELLITE);   
         * 开启交通图   
         * mBaiduMap.setTrafficEnabled(true);
         */
        mBaiduMap = mMapView.getMap();  
        mBaiduMap.setMapType(BaiduMap.MAP_TYPE_NORMAL);  
        
        //几何图形覆盖物
        //定义多边形的五个顶点  
        LatLng pt1 = new LatLng(39.93923, 116.357428);  
        LatLng pt2 = new LatLng(39.91923, 116.327428);  
        LatLng pt3 = new LatLng(39.89923, 116.347428);  
        LatLng pt4 = new LatLng(39.89923, 116.367428);  
        LatLng pt5 = new LatLng(39.91923, 116.387428);  
        List<LatLng> pts = new ArrayList<LatLng>();  
        pts.add(pt1);  
        pts.add(pt2);  
        pts.add(pt3);  
        pts.add(pt4);  
        pts.add(pt5);  
        //构建用户绘制多边形的Option对象  
        OverlayOptions polygonOption = new PolygonOptions()  
            .points(pts)  
            .stroke(new Stroke(5, 0xAA00FF00))  
            .fillColor(0xAAFFFF00);  
        //在地图上添加多边形Option，用于显示  
        mBaiduMap.addOverlay(polygonOption);
        
    }  
    @Override  
    protected void onDestroy() {  
        super.onDestroy();  
        //在activity执行onDestroy时执行mMapView.onDestroy()，实现地图生命周期管理  
        mMapView.onDestroy();  
    }  
    @Override  
    protected void onResume() {  
        super.onResume();  
        //在activity执行onResume时执行mMapView. onResume ()，实现地图生命周期管理  
        mMapView.onResume();  
        }  
    @Override  
    protected void onPause() {  
        super.onPause();  
        //在activity执行onPause时执行mMapView. onPause ()，实现地图生命周期管理  
        mMapView.onPause();  
        }  
}
```
运行该代码简单的测试结果如下所示：包括卫星地图、交通地图、普通地图和五边形覆盖物，其他覆盖物参考[官方文档](http://developer.baidu.com/map/index.php?title=androidsdk/guide/basicmap)。
![](https://img-blog.csdn.net/20160105015832684)![](https://img-blog.csdn.net/20160105015847386)![](https://img-blog.csdn.net/20160105015857847)

## 三. 百度地图位置定位
基础地图功能介绍参考：[http://developer.baidu.com/map/index.php?title=androidsdk/guide/basicmap](http://developer.baidu.com/map/index.php?title=androidsdk/guide/basicmap)
类方法介绍：[http://wiki.lbsyun.baidu.com/cms/androidsdk/doc/v3_6_1/](http://wiki.lbsyun.baidu.com/cms/androidsdk/doc/v3_6_1/)
例如参考下图：[MapStatusUpdateFactory方法](http://developer.baidu.com/map/android_refer/com/baidu/mapapi/map/MapStatusUpdateFactory.html)
![](https://img-blog.csdn.net/20160106015806020)
其中百度地图定位主要参考我以前的代码和crazy1235（推荐大家学习）的文章：
[[android] 百度地图开发 (三).定位当前位置及getLastKnownLocation总为空问题](http://blog.csdn.net/eastmount/article/details/42534721)
[百度地图开发（二）之添加覆盖物 + 地理编码和反地理编码](http://blog.csdn.net/crazy1235/article/details/43377545)
[百度地图开发（三）之地图控制 + 定位 - crazy1235](http://blog.csdn.net/crazy1235/article/details/43898451)
需要注意的地方和核心步骤包括：
1.需要添加定位SDK包，在libs中添加locSDK_3.1.jar和liblocSDK3.so；
2.在AndroidManifest.xml文件中设置service：
```python
<application>
    <meta-data  
      	android:name="com.baidu.lbsapi.API_KEY"  
        android:value="OYl0tcWvGrWtWucQN2rhdxG8" />
    <activity>....</activity> 
    
    <service 
       	android:name="com.baidu.location.f" 
       	android:enabled="true" 
       	android:process=":remote" > 
    </service>
</application>
```
3.代码中通过BDLocationListener实现定位监听，具体代码如下：
```python
package com.example.baidumapres;
import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.CircleOptions;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationConfiguration;
import com.baidu.mapapi.map.MyLocationConfiguration.LocationMode;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.map.OverlayOptions;
import com.baidu.mapapi.map.Stroke;
import com.baidu.mapapi.model.LatLng;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
public class MainActivity extends Activity {
	
	//百度地图控件
	private MapView mMapView = null;  
	//百度地图对象
	private BaiduMap mBaiduMap;
	//按钮 添加覆盖物
	private Button addOverlayBtn;
	//是否显示覆盖物 1-显示 0-不显示
	private int isShowOverlay = 1;
	//按钮 定位当前位置
	private Button locCurplaceBtn;
	//是否首次定位
	private boolean isFirstLoc = true;
	//定位SDK的核心类
	private LocationClient mLocClient;
	//定位图层显示模式 (普通-跟随-罗盘)
	private LocationMode mCurrentMode;
	//定位图标描述
	private BitmapDescriptor mCurrentMarker = null;
	//当前位置经纬度
	private double latitude;
	private double longitude;
	//定位SDK监听函数
	public MyLocationListenner locListener = new MyLocationListenner();
	
    @Override  
    protected void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);   
        //在使用SDK各组件之前初始化context信息，传入ApplicationContext  
        //注意该方法要再setContentView方法之前实现  
        SDKInitializer.initialize(getApplicationContext());  
        setContentView(R.layout.activity_main);  
        
        //获取地图控件  
        mMapView = (MapView) findViewById(R.id.map_view);
        addOverlayBtn = (Button) findViewById(R.id.btn_add_overlay);
        locCurplaceBtn = (Button) findViewById(R.id.btn_cur_place);
        addOverlayBtn.setEnabled(false);
     
        //设置地图缩放级别16 类型普通地图
        mBaiduMap = mMapView.getMap();  
        MapStatusUpdate msu = MapStatusUpdateFactory.zoomTo(16.0f);  
        mBaiduMap.setMapStatus(msu);
        mBaiduMap.setMapType(BaiduMap.MAP_TYPE_NORMAL);  
        
        //开启定位图层
        mBaiduMap.setMyLocationEnabled(true);
        //定位初始化
        //注意: 实例化定位服务 LocationClient类必须在主线程中声明 并注册定位监听接口
        mLocClient = new LocationClient(this);
        mLocClient.registerLocationListener(locListener);
		LocationClientOption option = new LocationClientOption(); 
		option.setOpenGps(true);              //打开GPS
		option.setCoorType("bd09ll");        //设置坐标类型
		option.setScanSpan(5000);            //设置发起定位请求的间隔时间为5000ms 
		mLocClient.setLocOption(option);     //设置定位参数
		mLocClient.start();                  //调用此方法开始定位    
        
        //Button 添加覆盖物
        addOverlayBtn.setOnClickListener(new OnClickListener() {    
            @Override    
            public void onClick(View v) {
            	addCircleOverlay();
            }    
        });  
        //Button 定位当前位置
        locCurplaceBtn.setOnClickListener(new OnClickListener() {    
            @Override    
            public void onClick(View v) {
            	addMyLocation();
            }    
        });  
        
    }  
    
    /**
     * 定位SDK监听器 需添加locSDK jar和so文件
     */
    public class MyLocationListenner implements BDLocationListener {
   		@Override
   		public void onReceivePoi(BDLocation location) {
   		}
   		@Override
   		public void onReceiveLocation(BDLocation location) {		
   			//mapview 销毁后不在处理新接收的位置
   			if (location == null || mBaiduMap == null) {
   				return;
   			}
   			//MyLocationData.Builder定位数据建造器
   			MyLocationData locData = new MyLocationData.Builder()
   					.accuracy(location.getRadius())
   					.direction(100)
   					.latitude(location.getLatitude())
   					.longitude(location.getLongitude())
   					.build();
   			//设置定位数据
   			mBaiduMap.setMyLocationData(locData);
   			mCurrentMode = LocationMode.NORMAL;
   			//获取经纬度
   			latitude = location.getLatitude();
   			longitude = location.getLongitude();
   			//Toast.makeText(getApplicationContext(), String.valueOf(latitude), Toast.LENGTH_SHORT).show();
   			//第一次定位的时候，那地图中心点显示为定位到的位置
   			if (isFirstLoc) {
   				isFirstLoc = false;
   				//地理坐标基本数据结构
   				LatLng loc = new LatLng(location.getLatitude(),location.getLongitude());
   				//MapStatusUpdate描述地图将要发生的变化
   				//MapStatusUpdateFactory生成地图将要反生的变化
   				MapStatusUpdate msu = MapStatusUpdateFactory.newLatLng(loc);
   				mBaiduMap.animateMapStatus(msu);
   				Toast.makeText(getApplicationContext(), location.getAddrStr(), 
   						Toast.LENGTH_SHORT).show();
   			}
   		}
   	}
   
    /**
     * 定位并添加标注
     */
    private void addMyLocation() {
	    	 //更新
	       	mBaiduMap.setMyLocationConfigeration(new MyLocationConfiguration(
	       			mCurrentMode, true, mCurrentMarker));   
	       	mBaiduMap.clear();
	       	addOverlayBtn.setEnabled(true);
	       	//定义Maker坐标点  
	       	LatLng point = new LatLng(latitude, longitude);  
	       	//构建Marker图标  
	       	BitmapDescriptor bitmap = BitmapDescriptorFactory.fromResource(R.drawable.icon_marka);  
	       	//构建MarkerOption，用于在地图上添加Marker  
	       	OverlayOptions option = new MarkerOptions()  
	       		.position(point)  
	       	    .icon(bitmap);  
	       	//在地图上添加Marker，并显示  
	       	mBaiduMap.addOverlay(option);
       }
    
    /**
     * 添加覆盖物
     */
    private void addCircleOverlay() {
    	if(isShowOverlay == 1) {  //点击显示
    		mBaiduMap.clear();
    		isShowOverlay = 0;
            //DotOptions 圆点覆盖物
            LatLng pt = new LatLng(latitude, longitude); 
            CircleOptions circleOptions = new CircleOptions();
            //circleOptions.center(new LatLng(latitude, longitude));
            circleOptions.center(pt);                          //设置圆心坐标  
            circleOptions.fillColor(0xAAFFFF00);               //圆填充颜色
            circleOptions.radius(250);                         //设置半径 
            circleOptions.stroke(new Stroke(5, 0xAA00FF00));   // 设置边框
            mBaiduMap.addOverlay(circleOptions); 
    	} 
    	else {
    		mBaiduMap.clear();
    		isShowOverlay = 1;
    	}
    }
    
    @Override  
    protected void onDestroy() {  
    	mLocClient.stop();                       //退出时销毁定位
        mBaiduMap.setMyLocationEnabled(false);   //关闭定位图层
        mMapView.onDestroy();  
        mMapView = null;
        super.onDestroy();  
    }  
    
    @Override  
    protected void onResume() {  
        super.onResume();  
        mMapView.onResume();  
    }  
    
    @Override  
    protected void onPause() {  
        super.onPause();   
        mMapView.onPause();  
    }  
}
```
运行结果如下图所示"我的位置"和"添加覆盖物"：
![](https://img-blog.csdn.net/20160106022059299)![](https://img-blog.csdn.net/20160106022112459)

## 四. 百度地图POI附近餐馆查找
PS：这部分下一篇文章将详细介绍，主要是POI附件餐馆和路线规划。

## 五. 新年祝福
最后分享自己圣诞节大学面试和新年祝福的qq说说，与君卿共勉~
《你明明可以靠才华吃饭，却偏偏要去拼颜值》
很幸运!第一次坐飞机，是为了回家那边的大学去讲课和面试；第一次穿西装，白衬衣里套了三件衣服，其中一件毛衣，别冷着自己就行；第一份圣诞老人的礼物，居然是一份讲台前当老师的承诺。
来之前，一位朋友曾问过我：“你明明可以靠才华吃饭，却偏偏要去拼颜值教书吗？”我的回答是：“我有颜值吗？哈哈，其实教书也能展现我的才华与魅力的啊!”
的确，我也可以去到一个公司，扎扎实实写代码，一个月一万四左右；也有很多创业公司找我做搜索图谱、图像处理、python、selenium。但我更期待自己在讲台前和科研实验室的情景，画面太美，不敢想。
十年后，我不知道自己会不会像春哥的老师一样，也写一篇《别了，这狗日的科研》而放弃大学?但此时此刻还是非常乐观的，至少除了科研教学外，我还会写点代码，这就饿不着;还可以写点文章博客和讲课吧。很多时候，想是一回事，说是一回事，做又是另外一回事。一辈子，又能做多少自己喜欢的事情呢?对吧!
刚刚找工作的那段时间，我看到了一句话：千里马若有人赏识，不论对错，不为输赢。我那时也希望我为一家IT公司奉献很多年，赚个几十万。但后来，我才发现最欣赏我这匹“蠢马”的伯乐，应该在大学，或许就是芸芸众生中的学子一员，或许是那个面试的领导(虽然他一直想不明白一个搞软件的男生为什么要来贵州这边的大学)，再或许就是我自己的孤芳自赏吧!
《低俗小说》里面好像有这样一句话:Pride only hurts, it never helps. 有时候自尊心只会让你受伤，其他毫无用处。希望自己在大学能抹去那份傲娇，跟着心走，十年如一日的去坚持那份热情吧！毕竟短暂的激情是不值钱的，只有长久的激情才值钱。
哎!讲完课的当时，还是有一点低落，工资现在只有开发的一个零头了。痛，所以我这么多搞开发的小伙们，以后来找你们玩的时候，懂吧!哈哈，开个玩笑～这里的工作环境和方式我真心喜欢，而且又能教书！
最后用自己的博客签名结束这篇文章吧!“无知·乐观·低调·谦逊·生活“。时刻告诉自己：无知的我需要乐观的去求知，低调的底色是谦逊，而谦逊是源于对生活的通透，我们不止有工作、学习、编程，还要学会享受生活，人生何必走得这么匆忙，开心就好!fighting
新的一年自己最大的愿望就是希望成为一名大学老师，并且身体健康就好！
（By:Eastmount 2016-01-06 夜3点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)**）**



