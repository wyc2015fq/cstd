# eclipse开发android手机定位 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年06月15日 14:47:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4378








一、eclipse开发环境搭建

1．JDK安装和部署

1）JDK下载

地址：[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)

本案例下载jdk5.0

2）JDK安装

默认安装即可；

3）JDK部署

【我的电脑】—右键—【属性】—【高级】-【环境变量】：

JAVA_HOME= C:/Program Files/Java/jdk1.5.0_02，说明是jdk安装路径

CLASSPATH=%JAVA_HOME%/lib

Path增加：%JAVA_HOME%/bin

2．Eclipse安装

地址：[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)

本案例下载eclipse3.6.2

解压缩即可；

打开eclipse.exe，进入【Window】—【Preferences】—【Java】检查Jre安装，入没有则配置路径。

二、android sdk和adt嵌入eclispse

1．ADT安装

1）下载地址：[http://www.android123.com.cn/android_kit.html](http://www.android123.com.cn/android_kit.html)

2）安装：启动Eclipse，选择【Help】 > 【install new software...】，打开界面上输入ADT解压地址，然后安装即可；

本案例安装ADT8.0.0

3．Android sdk安装

1）下载地址：[http://developer.android.com/sdk/index.html](http://developer.android.com/sdk/index.html)

2）解压即可；

3）安装：启动Eclipse，选择【Window】 > 【Preferences】>【Android】，直接设定"SDK Location"为SDK的安装解压目录；

4）配置：选择【Window】>【Android SDK and AVD Manager】，安装SDK版本和

部署dvd；

本案例安装sdk2.2，API8

至此eclipse开发android即可；

5）android sdk升级Google API

如果要开发手机定位，则需要通过【Window】>【Android SDK and AVD Manager】安装Google API，本案例安装与SDK同样的API8的Google API；

三、创建android工程

1．创建mamdemo工程，包命名为cn..map，命名GMapsActivity的Activity；

2．AndroidManifest.xml配置

<uses-library android:name="com.google.android.maps"/>--含库，application内加

<uses-permission android:name="android.permission.INTERNET" />--网络访问权限，application外加

<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION"/>--GPS访问权限，application外加

<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"/>--GPS访问权限，application外加

3．Main.xml配置

增加：<com.google.android.maps.MapView

android:id="@+id/MapView1"

android:layout_width="fill_parent"

android:layout_height="fill_parent"

android:apiKey="0L8E3nd9sIKt0X7nSW8yOqMsAx9ftQlWotNgUXw"

/>

其中android:apiKey需要申请：

1）debug.keystore路径：

【Window】 > 【Preferences】>【Android】>【Build】页：default debug keystore框内路径即是；

2）Java的Keytool工具路径：

%JAVA_HOME%/bin路径下有keytool.exe；

3）产生MD5

在cmd环境下，切换至debug.keystore路径，并执行命令：keytool -list -keystore debug.keystore，当提示你输入密码时，输入默认的密码android，这样就可以取得MD5值；

4）获取API key

[http://code.google.com/intl/zh-CN/android/maps-api-signup.html](http://code.google.com/intl/zh-CN/android/maps-api-signup.html)

输入MD5值获取APIkey，配置到main.xml中；

5．Activity开发

**package** cn.map;



**import** android.location.Criteria;

**import** android.location.Location;

**import** android.location.LocationListener;

**import** android.location.LocationManager;

**import** android.os.Bundle;

**import** android.provider.Settings;

**import** android.widget.Toast;



**import** com.google.android.maps.MapActivity;

**import** com.google.android.maps.MapView;

**import** java.util.List;



**import** android.content.Context;

**import** android.content.Intent;

**import** android.graphics.Canvas;

**import** android.graphics.Paint;

**import** android.graphics.Point;

**import** com.google.android.maps.GeoPoint;

**import** com.google.android.maps.MapController;

**import** com.google.android.maps.Overlay;





**public****class** GMapsActivity **extends** MapActivity {

**private** MapViewmMapView;

**private** MapController mMapController; 

**private** GeoPoint mGeoPoint;

**double**latitude,longitude;

/** Called when the activity is first created. */

@Override

**public****void** onCreate(Bundle savedInstanceState)

{

**super**.onCreate(savedInstanceState);

setContentView(R.layout.*main*);

mMapView = (MapView) findViewById(R.id.*MapView1*);

mMapView.setTraffic(**true**);//设置为交通模式

//mMapView.setSatellite(true); //设置为卫星模式

//mMapView.setStreetView(false);//设置为街景模式

mMapController = mMapView.getController(); //取得MapController对象(控制MapView)

mMapView.setEnabled(**true**);

mMapView.setClickable(**true**);

mMapView.setBuiltInZoomControls(**true**); //设置地图支持缩放

mGeoPoint = **new** GeoPoint((**int**) (23* 1000000), (**int**) (113* 1000000)); //设置起点为广州

mMapController.animateTo(mGeoPoint); //定位到指定坐标

mMapController.setZoom(12);//设置倍数(1-21) 



//添加Overlay，用于显示标注信息

MyLocationOverlay myLocationOverlay = **new** MyLocationOverlay();

List<Overlay> list = mMapView.getOverlays();

list.add(myLocationOverlay);



//通过GPS获取指定坐标

openGPSSettings();

getLocation();

}

**protected****boolean** isRouteDisplayed()

{

**return****false**;

}

**class** MyLocationOverlay **extends** Overlay

{

@Override

**public****boolean** draw(Canvas canvas, MapView mapView, **boolean** shadow, **long** when)

{

**super**.draw(canvas, mapView, shadow);

Paint paint = **new** Paint();

Point myScreenCoords = **new** Point();

// 将经纬度转换成实际屏幕坐标

mapView.getProjection().toPixels(mGeoPoint, myScreenCoords);

paint.setStrokeWidth(1);

paint.setARGB(255, 255, 0, 0);

paint.setStyle(Paint.Style.*STROKE*);

canvas.drawText("广州欢迎你", myScreenCoords.x, myScreenCoords.y, paint);

**return****true**;

}

}



**private****void** openGPSSettings() {

LocationManager alm = (LocationManager) **this**.getSystemService(Context.*LOCATION_SERVICE*); 

**if** (alm.isProviderEnabled(android.location.LocationManager.*GPS_PROVIDER*)) {

Toast.*makeText*(**this**, "GPS模块正常", Toast.*LENGTH_SHORT*).show();

**return**;

}

Toast.*makeText*(**this**, "请开启GPS！", Toast.*LENGTH_SHORT*).show();

Intent intent = **new** Intent(Settings.*ACTION_SECURITY_SETTINGS*);

startActivityForResult(intent,0); //此为设置完成后返回到获取界面

}



**private****final** LocationListener locationListener = **new** LocationListener() {

**public****void** onLocationChanged(Location location) { 

// 当坐标改变时触发此函数，如果Provider传进相同的坐标，它就不会被触发

// log it when the location changes

**if** (location != **null**) {

latitude = location.getLatitude();//维度

longitude= location.getLongitude();//经度

mGeoPoint = **new** GeoPoint((**int**)latitude, (**int**)longitude);

mMapController.animateTo(mGeoPoint); //定位到指定坐标

mMapController.setZoom(12); //设置倍数(1-21)

}

}



**public****void** onProviderDisabled(String provider) {

// **TODO** Auto-generated method stub

// Provider被disable时触发此函数，比如GPS被关闭

}

**public****void** onProviderEnabled(String provider) {

// **TODO** Auto-generated method stub

//Provider被enable时触发此函数，比如GPS被打开

}

**public****void** onStatusChanged(String provider, **int** status, Bundle extras) {

// **TODO** Auto-generated method stub

//Provider的转态在可用、暂时不可用和无服务三个状态直接切换时触发此函数

}

};



**private****void** getLocation(){

// 获取位置管理服务

LocationManager locationManager;

String serviceName = Context.*LOCATION_SERVICE*;

locationManager = (LocationManager) **this**.getSystemService(serviceName);

// 查找到服务信息

Criteria criteria = **new** Criteria();

criteria.setAccuracy(Criteria.*ACCURACY_FINE*); 

// 高精度

criteria.setAltitudeRequired(**false**);

criteria.setBearingRequired(**false**);

criteria.setCostAllowed(**true**);

criteria.setPowerRequirement(Criteria.*POWER_LOW*);

// 低功耗

String provider = locationManager.getBestProvider(criteria, **true**); 

// 获取GPS信息

Location location = locationManager.getLastKnownLocation(provider); 

// 通过GPS获取位置

**if** (location != **null**) {

latitude = location.getLatitude();//维度

longitude= location.getLongitude();//经度

mGeoPoint = **new** GeoPoint((**int**)latitude, (**int**)longitude);

mMapController.animateTo(mGeoPoint); //定位到指定坐标

mMapController.setZoom(12); //设置倍数(1-21) 

}

// 设置监听器，自动更新的最小时间为间隔N秒(1秒为1*1000)或最小位移变化超过N米

locationManager.requestLocationUpdates(provider, 100*1000, 500,locationListener);

}

}



四、打包android工程

【Export】—【Android】或【Android Tools】—【Export unsigned/ signed Application package】，按步骤即可生产apk文件。关键步骤是重新生成API key：

1．生成生产的keystore：

【Android Tools】—【Export signed Application package】，步骤二中选择“create new keystore”，本案例中：Location=D:/android/workspace/mapdemo.keystore；Password=mapdemo。步骤三中输入keystore各项信息，本案例中：alias=mymapdemo；Password=mapdemo；其他各项照填即可；最后步骤生成apk，这个apk文件还不能发布，需要在后面加上生产API KEY后重新发布。

2．获取MD5和API Key

1）MD5：cmd命令下，切换到D:/android/workspace目录，然后执行命令：keytool –list –keystore mapdemo.keystore，输入密码：mapdemo，产生MD5：DA:D3:46:4F:3D:D9:BD:4C:80:B5:F2:0C:03:3B:A1:16

2）[http://code.google.com/intl/zh-CN/android/maps-api-signup.html](http://code.google.com/intl/zh-CN/android/maps-api-signup.html)

输入MD5值获取APIkey：

android:apiKey="0L8E3nd9sIKvPOTtgAttDJpMlCynuALRiOzVz4g"，

修改main.xml中原debug.keystore生产的API key；

3）重新发布apk文件

【Android Tools】—【Export signed Application package】，步骤二中选择“Use Existing keystore”，本案例中：

Location=D:/android/workspace/mapdemo.keystore；Password=mapdemo；

步骤三中也是选择“Use existing keystore“，本案例中：alias=mymapdemo；Password=mapdemo；最后步骤生产apk文件，可发布。





说明：模拟器上演示地点地图通过，但手机移动GPS定位地图未测试！android2.1手机提示解析包出错，android2.2手机安装成功，和开发的skd2.2版本一致！



