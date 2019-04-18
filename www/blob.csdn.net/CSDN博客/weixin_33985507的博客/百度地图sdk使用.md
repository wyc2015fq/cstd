# 百度地图sdk使用 - weixin_33985507的博客 - CSDN博客
2017年05月16日 17:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 准备
登陆百度地图开放平台，注册账号，添加应用。添加应用时需要包名和SHA1码，SHA1码需要在项目的工程目录下输入如下指令，用自己的keystore替换debug.keystore
`keytool -list -v -keystore debug.keystore`
结果如下：
![4103410-b66c968127c7c86e.png](https://upload-images.jianshu.io/upload_images/4103410-b66c968127c7c86e.png)
SHA1码.png
# 配置
- 下载相关的资源包[百度sdk下载](https://link.jianshu.com?t=http://lbsyun.baidu.com/sdk/download?selected=mapsdk_basicmap,mapsdk_searchfunction,mapsdk_lbscloudsearch,mapsdk_calculationtool,mapsdk_radar)
（注意多试几次，有些下载的不能用，有错误肯定在这儿！！！！）
- 
将下载得到的所有文件放到app/libs目录下，并且add dependence。
![4103410-6538c21873a37a4f.png](https://upload-images.jianshu.io/upload_images/4103410-6538c21873a37a4f.png)
添加百度jar包依赖.png
添加完成后在bulid.gradle中添加sourceSets
![4103410-a9b16ab883f69294.png](https://upload-images.jianshu.io/upload_images/4103410-a9b16ab883f69294.png)
添加sourceSets.png
完成后会在jniLibs中看到对应的文件
![4103410-4ebee08859e97f03.png](https://upload-images.jianshu.io/upload_images/4103410-4ebee08859e97f03.png)
jniLibs.png
- 在manifest中添加权限
```
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"/>
    <uses-permission android:name="android.permission.INTERNET"/>
    <uses-permission android:name="com.android.launcher.permission.READ_SETTINGS" />
    <uses-permission android:name="android.permission.WAKE_LOCK"/>
    <uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.GET_TASKS" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <uses-permission android:name="android.permission.WRITE_SETTINGS" />```
以及
```
```
<meta-data   
        android:name="com.baidu.lbsapi.API_KEY"
        android:value="开发者 key"/>```
```
- 使用前需要在application中的onCreate时添加`SDKInitializer.initialize(this);`
# 初始化中心点
```
/**
     * 初始化map中心点
     * @param latitude  中心点纬度
     * @param longitude 中心点经度
     */
    private void initMapCenter(double latitude, double longitude) {
        // 初始化搜索模块，注册事件监听
        LatLng cenpt = new LatLng(latitude, longitude);
        //定义地图状态
        MapStatus mMapStatus = new MapStatus.Builder()
                .target(cenpt)
                .zoom(16)
                .build();
        MapStatusUpdate mMapStatusUpdate = MapStatusUpdateFactory.newMapStatus(mMapStatus);
        //改变地图状态
        mBaiduMap.setMapStatus(mMapStatusUpdate);
    }
```
# 添加图钉
```
/**
     * 添加图钉
     * @param latitude  图钉纬度
     * @param longitude 图钉经度
     */
    private void addOverlay(double markerLatitude, double markerLongitude) {
        LatLng cenpt = new LatLng(markerLatitude, markerLongitude);
        //构建Marker图标
        bd = BitmapDescriptorFactory
                .fromResource(R.drawable.icon_geo_blue);
        //构建MarkerOption，用于在地图上添加Marker
        OverlayOptions option = new MarkerOptions()
                .position(cenpt)
                .icon(bd)
                .zIndex(9);//设置marker所在层级
        //在地图上添加Marker，并显示
        bdMarker = (Marker) (mBaiduMap.addOverlay(option));
    }
```
# 泡泡
泡泡指的是地图上的marker点击后显示的内容，一般显示该marker的详细信息
```
/**
     * 初始化泡泡窗口
     * @param marker 被点击的marker点
     */
    private void initInfoWindow(final Marker marker) {
        LatLng latLng = marker.getPosition();
        view = View.inflate(this, R.layout.pop_window, null);
        infoWindow = new InfoWindow(view, latLng, -47);
        mBaiduMap.showInfoWindow(infoWindow);
    }
```
隐藏泡泡
`mBaiduMap.hideInfoWindow()`
# 百度定位
- 在Mainfest中添加service
```
<service android:name="com.baidu.location.f" android:enabled="true" android:process=":remote"> </service>
```
- LocationService
```
/**
 * @author baidu
 */
public class LocationService {
    private LocationClient client = null;
    private LocationClientOption mOption;
    private LocationClientOption diyOption;
    private Object objLock = new Object();
    /***
     * @param locationContext
     */
    public LocationService(Context locationContext) {
        synchronized (objLock) {
            if (client == null) {
                client = new LocationClient(locationContext);
                client.setLocOption(getDefaultLocationClientOption());
            }
        }
    }
    /**
     * 注册监听
     * @param listener
     * @return
     */
    public boolean registerListener(BDLocationListener listener) {
        boolean isSuccess = false;
        if (listener != null) {
            client.registerLocationListener(listener);
            isSuccess = true;
        }
        return isSuccess;
    }
    /**
     * 反注册监听
     * @param listener
     */
    public void unregisterListener(BDLocationListener listener) {
        if (listener != null) {
            client.unRegisterLocationListener(listener);
        }
    }
    /***
     * @param option
     * @return isSuccessSetOption
     */
    public boolean setLocationOption(LocationClientOption option) {
        boolean isSuccess = false;
        if (option != null) {
            if (client.isStarted()) {
                client.stop();
            }
            diyOption = option;
            client.setLocOption(option);
            isSuccess = true;
        }
        return isSuccess;
    }
    public LocationClientOption getOption() {
        return diyOption;
    }
    /***
     * @return DefaultLocationClientOption
     */
    public LocationClientOption getDefaultLocationClientOption() {
        if (mOption == null) {
            mOption = new LocationClientOption();
            mOption.setLocationMode(LocationMode.Hight_Accuracy);//可选，默认高精度，设置定位模式，高精度，低功耗，仅设备
            mOption.setOpenGps(true); // 是否打开GPS
            mOption.setCoorType("bd09ll");//可选，默认gcj02，设置返回的定位结果坐标系，如果配合百度地图使用，建议设置为bd09ll;
            mOption.setScanSpan(1000);//可选，默认0，即仅定位一次，设置发起定位请求的间隔需要大于等于1000ms才是有效的
            mOption.setIsNeedAddress(true);//可选，设置是否需要地址信息，默认不需要
            mOption.setIsNeedLocationDescribe(true);//可选，设置是否需要地址描述
            mOption.setNeedDeviceDirect(true);//可选，设置是否需要设备方向结果
            mOption.setLocationNotify(false);//可选，默认false，设置是否当gps有效时按照1S1次频率输出GPS结果
            mOption.setIgnoreKillProcess(true);
            //可选，默认true，定位SDK内部是一个SERVICE，并放到了独立进程，设置是否在stop的时候杀死这个进程，默认不杀死
            mOption.setIsNeedLocationDescribe(true);//可选，默认false，设置是否需要位置语义化结果，可以在BDLocation
            // .getLocationDescribe里得到，结果类似于“在北京天安门附近”
            mOption.setIsNeedLocationPoiList(true);//可选，默认false，设置是否需要POI结果，可以在BDLocation
            // .getPoiList里得到
            mOption.SetIgnoreCacheException(false);//可选，默认false，设置是否收集CRASH信息，默认收集
            mOption.setIsNeedAltitude(false);//可选，默认false，设置定位时是否需要海拔信息，默认不需要，除基础定位版本都可用
        }
        return mOption;
    }
    /**
     * 服务开始
     */
    public void start() {
        synchronized (objLock) {
            if (client != null && !client.isStarted()) {
                client.start();
            }
        }
    }
    /**
     * 服务停止
     */
    public void stop() {
        synchronized (objLock) {
            if (client != null && client.isStarted()) {
                client.stop();
            }
        }
    }
}
```
- locationservice实例化
```
//获取locationservice实例
        locationService = ((MicroBandApp) getApplication()).locationService;
        locationService.setLocationOption(locationService.getDefaultLocationClientOption());
        //注册监听
        locationService.registerListener(this);
        // 开始定位
        locationService.start();
```
- 监听
`mBaiduMap.setMyLocationEnabled(true);`
在回调中获取当前的经纬度和地址
``
@Override
public void onReceiveLocation(BDLocation bdLocation) {
// 只是完成了定位
if (bdLocation == null || binding.bmapView == null) {
return;
}
Log.d(TAG, "onReceiveLocation: " + bdLocation.getLatitude() + "::" + bdLocation
.getLongitude());
}
```
5. 停止定位
```
```
locationService.unregisterListener(this); //注销掉监听
    locationService.stop(); //停止定位服务
```
```
***
#导航--从当前位置开始
***
1. 调用百度地图进行导航
```
/**
* 调用百度地图导航
* @param endLat
* @param endLon
*/
public void loadBaiduMap(double endLat,double endLon) {
if (isInstallByread("com.baidu.BaiduMap")) {
Intent intent = null;
try {
intent = Intent.getIntent("[baidumap://map/direction?origin=](https://link.jianshu.com?t=baidumap://map/direction?origin=)我的位置"+
"&destination=" +endLat+","+endLon+
"&mode=transit&sy=3&index=0&target=1");
} catch (URISyntaxException e) {
Log.e("intent", e.getMessage());
}
this.context.startActivity(intent); //启动调用
} else {
Toast.makeText(context, "请安装百度地图", Toast.LENGTH_SHORT).show();
}
}
```
2.  调用高德地图进行导航
```
/**
* 调用高德地图导航
* @param endLat
* @param endLon
*/
public void loadGaodeMap(double endLat,double endLon) {
if (isInstallByread("com.autonavi.minimap")) {
Intent intent = null;
try {
intent = Intent.getIntent("[androidamap://route?sourceApplication=amap](https://link.jianshu.com?t=androidamap://route?sourceApplication=amap)" +
"&dlat="+endLat+
"&dlon="+endLon+
"&dname=西溪新座&dev=0&t=1");
} catch (URISyntaxException e) {
Log.e("intent", e.getMessage());
}
this.context.startActivity(intent); //启动调用
} else {
Toast.makeText(context, "请安装高德地图", Toast.LENGTH_SHORT).show();
}
}
```
3. 参数
如果需要从非当前位置开始导航，可查询具体的参数设置
[百度URI API
](http://lbsyun.baidu.com/index.php?title=uri/api/android)
***
#定位服务
***
Android6.0以上定位服务的开启不仅需要在manifest中定义，用户还可以进行手动的开启或关闭
```
public class LocationPermissionUtil {
private static AlertDialog mAlertDialog;
private static final int PERMISSION_REQUEST_CODE = 0;//系统权限管理页面的参数
```
/**
 * Location service if enable
 *
 * @return location is enable if return true, otherwise disable.
 */
public static final boolean isLocationEnable(Context context) {
    LocationManager locationManager = (LocationManager) context.getSystemService(Context
            .LOCATION_SERVICE);
    boolean networkProvider = locationManager.isProviderEnabled(LocationManager
            .NETWORK_PROVIDER);
    boolean gpsProvider = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
    if (gpsProvider) {
        return true;
    }
    return false;
}
/**
 * 检查是否授予位置权限
 *
 * @return 权限检查通过返回true，不通过返回false
 */
public static boolean checkLocationPermission(Activity activity) {
    // For Android < Android M, self permissions are always granted.
    boolean result = true;
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
        if (getTargetSdkVersion(activity) >= Build.VERSION_CODES.M) {
            // targetSdkVersion >= Android M
            result = checkPermissions(activity, Manifest.permission.ACCESS_COARSE_LOCATION,
                    Manifest.permission.ACCESS_FINE_LOCATION);
        } else {
            // targetSdkVersion < Android M
            result = selfPermissionGranted(activity, Manifest.permission.ACCESS_COARSE_LOCATION,
                    Manifest.permission.ACCESS_FINE_LOCATION);
        }
    }
    return result;
}
/**
 * 目标sdk版本
 */
private static int getTargetSdkVersion(Context context) {
    try {
        final PackageInfo info = context.getPackageManager().getPackageInfo(
                context.getPackageName(), 0);
        return info.applicationInfo.targetSdkVersion;
    } catch (PackageManager.NameNotFoundException e) {
        e.printStackTrace();
    }
    return 0;
}
/**
 * 进入服务设置界面
 * @param context
 */
public static void startSetting(final Context context) {
    mAlertDialog = new AlertDialog.Builder(context)
            .setTitle("定位服务未开启")
            .setMessage("请在系统设置中开启定位服务")
            .setNegativeButton("设置", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    Intent locationIntent = new Intent(Settings
                            .ACTION_LOCATION_SOURCE_SETTINGS);
                    context.startActivity(locationIntent);
                }
            })
            .setPositiveButton("好的", null)
            .setCancelable(false)
            .show();
}
/**
 * targetSdkVersion < Android M
 * 检查系统权限是否被授权
 *
 * @return 权限检查通过返回true，不通过返回false
 */
public static boolean selfPermissionGranted(Context context, String... permissions) {
    List<String> lackPermissions = new ArrayList<>();
    for (String permission : permissions) {
        // targetSdkVersion < Android M, we have to use PermissionChecker
        if (PermissionChecker.checkSelfPermission(context, permission) != PackageManager
                .PERMISSION_GRANTED) {
            lackPermissions.add(permission);
        }
    }
    if (!lackPermissions.isEmpty()) {
        return false;
    }
    return true;
}
/**
 * 检查系统权限是否被授权，如果没有授权，则执行请求授权的操作
 *
 * @return 权限检查通过返回true，不通过返回false
 */
public static boolean checkPermissions(Activity activity, String... permissions) {
    List<String> lackPermissions = new ArrayList<>();
    for (String permission : permissions) {
        if (ContextCompat.checkSelfPermission(activity, permission) != PackageManager
                .PERMISSION_GRANTED) {
            lackPermissions.add(permission);
        }
    }
    if (!lackPermissions.isEmpty()) {
        requestPermissions(activity, lackPermissions.toArray(new String[lackPermissions.size()
                ]));
        return false;
    } else {
        return true;
    }
}
/**
 * 请求系统权限，兼容低版本
 */
public static void requestPermissions(Activity activity, String... permissions) {
    ActivityCompat.requestPermissions(activity, permissions, PERMISSION_REQUEST_CODE);
}
```
}
```
***
#链接
***
[百度api](http://wiki.lbsyun.baidu.com/cms/androidsdk/doc/v4_3_1/index.html?qq-pf-to=pcqq.c2c)
```
