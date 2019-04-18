# 【从 0 开始开发一款直播 APP】15 Android 定位详解之 LocationManager & Geocoder 实现直播定位... - weixin_33985507的博客 - CSDN博客
2017年05月24日 09:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
> 
本文为菜鸟窝作者蒋志碧的连载。“从 0 开始开发一款直播 APP ”系列来聊聊时下最火的直播 APP，如何完整的实现一个类"腾讯直播"的商业化项目
现在Android 开发获取用户地理位置已经愈发简单，各种地图 SDK 都提供精确定位方法。不过如果需求中只是需要模糊定位到用户的城市，系统 API 完全可以满足需求，这时如果再集成一个地图 SDK 显得很臃肿。系统 API 进行定位都很普遍了，只是在 Android 6.0 版本加入了危险权限的动态验证，只需要在原来基础上判断权限即可。
Android 系统提供了地理位置服务相关的 API 方便开发者去获取当前的地理位置，在 android.loaction 包下主要有以下两个类。
1、[LocationManager](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/LocationManager.html)：用于获取地理位置的经纬度信息
2、[Geocoder](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/Geocoder.html)：根据经纬度获取详细信息/根据相信地址获取经纬度信息
接下来我们将实现直播中的定位功能。
代码中涉及到 MVP实现定位，Android 6.0 运行时权限验证，带动画的自定义 switch 控件，请戳链接
[【从 0 开始开发一款直播 APP】5.1 MVP 完全解析 -- 实现直播登录](https://link.jianshu.com?t=http://www.cniao5.com/forum/thread/3707)
[【从 0 开始开发一款直播 APP】13 Android 6.0 运行时权限](https://link.jianshu.com?t=http://www.cniao5.com/forum/thread/3731)
[【从 0 开始开发一款直播 APP】14 animation-list 逐帧动画自定义 Switch 控件](https://link.jianshu.com?t=http://www.cniao5.com/forum/thread/3732)
定位主要有以下权限:
```
//网络权限
<uses-permission android:name="android.permission.INTERNET"/>
//模糊定位权限：一般用于网络定位
<uses-permissionandroid:name="android.permission.ACCESS_COARSE_LOCATION"/>
//精确定位权限：一般用于gps定位
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"/>
```
## 一、LocationManager
LocationManager 提供系统定位服务，这些服务允许应用程序定期更新设备的地理位置，或者当设备进入给定地理位置附近时，触发应用指定意图。
LocationManager 不能直接实例化，需要使用如下方法
```
Context.getSystemService(Context.LOCATION_SERVICE)
```
在 LocationManager 中必须了解两个重要知识点：
1、provider
2、LocationListener
### 1.1、provider
位置信息提供者。android系统一般提供三种方式获取地理位置信息。
1、[GPS_PROVIDER](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/LocationManager.html#GPS_PROVIDER)：通过 GPS 来获取地理位置的经纬度信息。优点（获取地理位置信息精确度高），缺点（只能在户外使用，获取经纬度信息耗时，耗电）。
2、[NETWORK_PROVIDER](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/LocationManager.html#NETWORK_PROVIDER)：通过移动网络的基站或者 Wi-Fi 来获取地理位置。优点（只要有网络，就可以快速定位，室内室外都可），缺点（精确度不高）。
3、[PASSIVE_PROVIDER](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/LocationManager.html#PASSIVE_PROVIDER)：被动接收更新地理位置信息，而不用自己请求地理位置信息。该 PASSIVE_PROVIDER 返回的位置是通过其他 providers  产生的，你可以查询 getProvider() 方法决定位置更新的由来，需要 ACCESS_FINE_LOCATION 权限，但是如果未启用 GPS，则此 provider 可能只返回粗略位置匹配。
LocationManager 提供了以下几种方法来获得地理位置提供者。
```
//返回当前设备的所有 provider
public List<String> getAllProviders();
//当 enabledOnly 为 true 时，返回当前设备可使用的 provider，enabledOnly 为发false，返回所有 provider
public List<String> getProviders(boolean enabledOnly);
//返回当前设备最符合条件的 provider。Criteria：指定条件，enabledOnly：返回当前设备可用的 provider
public String getBestProvider(Criteria criteria, boolean enabledOnly);
//返回符合条件的 provider，Criteria：指定条件，enabledOnly：返回当前设备可用的 provider
public List<String> getProviders(Criteria criteria, boolean enabledOnly);
```
#### Criteria
指示选择 provider 的应用程序标准的类。providers 可以根据准确性，功率使用情况，报告高度，速度和方位的能力以及货币成本进行选择。该类用于指定率先选择最符合条件的 provider，根据 [Cirteria](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/Criteria.html) 指定的条件，设备会自动选择哪种 provider。
```
//指示纬度和经度所需的精度。参数：Criteria.ACCURACY_FINE：表示高精确度。Criteria.ACCURACY_COARSE：表示模糊精确度。
public void setAccuracy(int accuracy);
//是否需要海拔信息
public void setAltitudeRequired(boolean altitudeRequired);
//设置方位精度。参数：Criteria.NO_REQUIREMENT 无, Criteria.ACCURACY_LOW 低, Criteria.ACCURACY_HIGH 高。
public void setBearingAccuracy(int accuracy);
//指示是否要求方位信息
public void setBearingRequired(boolean bearingRequired);
//是否允许收费
public void setCostAllowed(boolean costAllowed);
//设置水平方向精准度
public void setHorizontalAccuracy(int accuracy);
//设置垂直方向精准度
public void setVerticalAccuracy(int accuracy);
//设置电池消耗要求 参数：Criteria.NO_REQUIREMENT 无, Criteria.POWER_LOW 低, Criteria.POWER_MEDIUM 中, Criteria.POWER_HIGH 高。
public void setPowerRequirement(int level);
//设置速度精度
public void setSpeedAccuracy(int accuracy);
//是否要求速度信息
public void setSpeedRequired(boolean speedRequired);
//-----------------------------split line-------------------------------------
                             Criteria 的使用
//-----------------------------split line-------------------------------------
Criteria criteria = new Criteria();
criteria.setAccuracy(Criteria.ACCURACY_FINE);//设置定位精准度
criteria.setAltitudeRequired(false);//是否要求海拔
criteria.setBearingRequired(true);//是否要求方向
criteria.setCostAllowed(true);//是否要求收费
criteria.setSpeedRequired(true);//是否要求速度
criteria.setPowerRequirement(Criteria.NO_REQUIREMENT);//设置电池耗电要求
criteria.setBearingAccuracy(Criteria.ACCURACY_HIGH);//设置方向精确度
criteria.setSpeedAccuracy(Criteria.ACCURACY_HIGH);//设置速度精确度
criteria.setHorizontalAccuracy(Criteria.ACCURACY_HIGH);//设置水平方向精确度
criteria.setVerticalAccuracy(Criteria.ACCURACY_HIGH);//设置垂直方向精确度
//返回满足条件的，当前设备可用的location provider，当第二个参数为false时，返回当前设备所有provider中最符合条件的那个provider（但是不一定可用）。
String mProvider  = mLocationManager.getBestProvider(criteria,true);
```
### 1.2、LocationListener 位置监听器接口
```
public interface LocationListener {
    //当坐标改变时触发此函数
    void onLocationChanged(Location location);
  
    //当provider的状态改变时，该方法被调用。状态有三种：
    //LocationProvider#OUT_OF_SERVICE：无服务
    //LocationProvider#TEMPORARILY_UNAVAILABLE：provider不可用
    //LocationProvider#AVAILABLE：provider可用
    void onStatusChanged(String provider, int status, Bundle extras);
  
    //当provider可用时调用，比如 GPS 可用时就会调用该方法。
    void onProviderEnabled(String provider);
  
    //当provider不可用时调用该方法。比如 GPS 未打开，GPS 不可用就会调用该方法。
    void onProviderDisabled(String provider);
}
```
### 1.3、获得 LocationManager 实例
```
protected LocationManager  locationManager;
locationManager=(LocationManager) getSystemService(Context.LOCATION_SERVICE);
```
### 1.4、绑定监听和解绑监听
```
locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, locationListener);
locationManager.removeUpdates(locationListener);
```
## 二、Geocoder
Geocoder 用于处理正向编码和反向编码。地理编码是将街道地址或其他地理位置变换为（纬度，经度）坐标的过程。逆向地理编码是将（纬度，经度）坐标转换为（部分）地址的过程。反向地理编码位置描述中的细节数量可能会有所不同，例如可能包含最近建筑物的完整街道地址，而另一个可能只包含城市名称和邮政编码。Geocoder 类需要一个未包含在核心 android 框架中的后端服务。如果平台中没有后端服务，Geocoder 查询方法将返回一个空列表。使用 isPresent() 方法来确定Geocoder 实现是否存在。由于国内使用不了Google Services 服务，因此一般的手机厂商都会在自己的手机内内置百度地图服务，或者高德地图服务来替代 Google Services 服务。
**主要有以下方法**
**isPresent()**：判断当前设备是否内置了地理位置服务。返回 true 表示 Geocoder 地理编码可以使用，否则不可使用。
**getFromLocationName()**：返回描述地理位置信息的集合。locationName：地址，maxResults：返回地址数目（1-5）
**getFromLocation()**：根据经纬度返回对应的地理位置信息。latitude：纬度，longitude：经度，maxResults：返回地址的数目（1-5）。
![5205232-23b81f480b196750.jpeg](https://upload-images.jianshu.io/upload_images/5205232-23b81f480b196750.jpeg)
### Geocoder 代码示例
```
private static String getAddressFromLocation(final Activity activity, Location location) {
    //Geocoder初始化
    Geocoder geocoder = new Geocoder(activity);
    //判断Geocoder地理编码是否可用
    boolean falg = geocoder.isPresent();
    try {
        //获取纬度和经度
        double latitude = location.getLatitude();
        double longitude = location.getLongitude();
        //根据经纬度获取地理信息
        List<Address> addresses = geocoder.getFromLocation(latitude, longitude, 1);
        if (addresses.size() > 0) {
            //返回当前位置，精度可调
            Address address = addresses.get(0);
            String sAddress;
            if (!TextUtils.isEmpty(address.getLocality())) {
                if (!TextUtils.isEmpty(address.getFeatureName())) {
                    //存储 市 + 周边地址
                    sAddress = address.getLocality() + " " + address.getFeatureName();
                  
                    //address.getCountryName() 国家
                    //address.getPostalCode() 邮编
                    //address.getCountryCode() 国家编码
                    //address.getAdminArea() 省份
                    //address.getSubAdminArea() 二级省份
                    //address.getThoroughfare() 道路
                    //address.getSubLocality() 二级城市
                } else {
                    sAddress = address.getLocality();
                }
            } else {
                sAddress = "定位失败";
            }
            return sAddress;
        }
    } catch (IOException e) {
        e.printStackTrace();
    }
    return "";
}
```
## 三、LocationManager 使用初探
1、获取 LocationManager
2、判断定位服务（GPS，WIFI，基站）是否可用
3、设置定位监听，获取经纬度
```
public void getMyLocation(Context context){
  // 获取 LocationManager 实例
  mLocationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
  // 判断网络定位是否可用，可替换成 GPS 定位。
  if (mLocationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER)) {
            mLocationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,
                    0, 0, new LocationListener() {
                @Override
                public void onLocationChanged(Location location) {
                    //位置发生改变时回调该函数
                    location.getLatitude();//纬度
                    location.getLongitude();//经度
                }
                @Override
                public void onStatusChanged(String provider, int status, Bundle extras){
                      //状态改变回调
                      //provider：定位器名称（NetWork,Gps等）
                      //status: 3种状态，超出服务范围，临时不可用，正常可用
                      //extras: 包含定位器一些细节信息
                }
                @Override
                public void onProviderEnabled(String provider) {
                    //定位开启回调
                }
                @Override
                public void onProviderDisabled(String provider) {
                    //定位关闭回调
                }
            });
        } 
}
```
## 四、定位工具类封装：LocationMgr
主要功能：监测定位权限，通过网络获取位置信息，通过经纬度解码地理位置
```
public class LocationMgr {
    private static String TAG = LocationMgr.class.getSimpleName();
    private static LocationListener mLocationListener;
    //监测定位权限
    public static boolean checkLocationPermission(final Activity activity) {
        if (Build.VERSION.SDK_INT >= 23) {
            if (PackageManager.PERMISSION_GRANTED != ActivityCompat.checkSelfPermission(activity, Manifest.permission.ACCESS_FINE_LOCATION)) {
                ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, Constants.LOCATION_PERMISSION_REQ_CODE);
                return false;
            }
        }
        return true;
    }
    /**
     * 根据经纬度解码地理位置
     * @param activity
     * @param location
     * @return
     */
    private static String getAddressFromLocation(final Activity activity, Location location) {
        Geocoder geocoder = new Geocoder(activity);
        try {
            double latitude = location.getLatitude();
            double longitude = location.getLongitude();
            Log.d(TAG, "getAddressFromLocation->lat:" + latitude + ", long:" + longitude);
            List<Address> addresses = geocoder.getFromLocation(latitude, longitude, 1);
            if (addresses.size() > 0) {
                //返回当前位置，精度可调
                Address address = addresses.get(0);
                String sAddress;
                if (!TextUtils.isEmpty(address.getLocality())) {
                    if (!TextUtils.isEmpty(address.getFeatureName())) {
                        sAddress = address.getLocality() + " " + address.getFeatureName();
                    } else {
                        sAddress = address.getLocality();
                    }
                } else {
                    sAddress = "定位失败";
                }
                return sAddress;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }
    /**
     * 获取位置
     * @param activity
     * @param locationListener
     * @return
     */
    public static boolean getMyLocation(final Activity activity, final onLocationListener locationListener) {
        final LocationManager locationManager = (LocationManager) activity.getSystemService(Context.LOCATION_SERVICE);
      //判断网络定位是否可用
        if (!locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER)) {
            //调用定位提示对话框，打开定位功能
            AlertDialog.Builder builder
                    = new AlertDialog.Builder(activity);
            builder.setMessage("尚未开启位置定位服务");
            builder.setPositiveButton("开启", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    //启动定位Activity
                    Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
                    activity.startActivity(intent);
                }
            });
            builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                }
            });
            
            builder.show();
            return false;
        }
        //权限检查
        if (!checkLocationPermission(activity)) {
            return true;
        }
      // 通过网络获取位置
        Location curLoc = locationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
        if (null == curLoc) {
            mLocationListener = new LocationListener() {
                @Override
                public void onLocationChanged(Location location) {
                    //获取解码的地理位置
                    String strAddress = getAddressFromLocation(activity, location);
                    if (TextUtils.isEmpty(strAddress)) {
                      //定位失败回调
                        locationListener.onLocationChanged(-1, 0, 0, strAddress);
                    } else {
                      //定位成功回调
                        locationListener.onLocationChanged(0, location.getLatitude(), location.getLongitude(), strAddress);
                    }
                  //关闭 GPS 定位功能
                    locationManager.removeUpdates(this);
                }
                @Override
                public void onStatusChanged(String provider, int status, Bundle extras) {
                  //关闭 GPS 定位功能
                    locationManager.removeUpdates(this);
                }
                @Override
                public void onProviderEnabled(String provider) {
                  //关闭 GPS 定位功能
                    locationManager.removeUpdates(this);
                }
                @Override
                public void onProviderDisabled(String provider) {
                  //关闭 GPS 定位功能
                    locationManager.removeUpdates(this);
                }
            };
          //设置监听器，自动更新的最小时间为间隔N秒(1秒为1*1000，这样写主要为了方便)或最小位移变化超过N米
locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,8000,0,mLocationListener);
        }else {
            //获取解码的地理位置
            String strAddress = getAddressFromLocation(activity,curLoc);
            if (TextUtils.isEmpty(strAddress)) {
              //定位失败回调
                locationListener.onLocationChanged(-1, 0, 0, strAddress);
            } else {
              //定位成功回调
                locationListener.onLocationChanged(0, curLoc.getLatitude(), curLoc.getLongitude(), strAddress);
            }
        }
        return true;
    }
    //自定义定位监听回调接口
    public interface onLocationListener {
        void onLocationChanged(int code, double lat1, double long1, String location);
    }
}
```
## 五、LocationMgr 的使用
### 5.1、PublishPresenter ：定位逻辑实现类
初始化 onLocationListener 定位监听接口，如果定位成功，通过调用 mIPublishView.doLocationSuccess(location) 方法，提示定位成功，mIPublishView.doLocationFailed() 提示定位失败。定位按钮点击调用 doLocation() 方法最终完成位置信息展示。
```
public class PublishPresenter extends IPublishPresenter {
    private IPublishView mIPublishView;
    private String TAG = PublishPresenter.class.getSimpleName();
    public PublishPresenter(IPublishView iPublishView) {
        super(iPublishView);
        this.mIPublishView = iPublishView;
    }
  public LocationMgr.onLocationListener getOnLocationListener() {
      return mOnLocationListener;
  }
    //初始化定位监听回调接口
  private LocationMgr.onLocationListener mOnLocationListener = new LocationMgr.onLocationListener() {
      @Override
      public void onLocationChanged(int code, double lat1, double long1, String location) {
          //0 表示成功
          if (0 == code) {
              mIPublishView.doLocationSuccess(location);
          } else {
              mIPublishView.doLocationFailed();
          }
      }
  };
  
  //定位
  @Override
  public void doLocation() {
      if (LocationMgr.checkLocationPermission(mIPublishView.getActivity())) {
          //成功返回地理位置信息结果
          boolean success = LocationMgr.getMyLocation(mIPublishView.getActivity(), mOnLocationListener);
          if (!success) {
              mIPublishView.doLocationFailed();
          }
      }
  }
}
```
### 5.2、PublishActivity
实例化定位按钮，调用定位功能 mPublishPresenter.doLocation()，实现定位成功和失败方法并作处理。
```
public class PublishActivity extends BaseActivity implements View.OnClickListener, IPublishView{
private TextView mTvLBS;
private PublishPresenter mPublishPresenter;
@Override
protected void setListener() {
    mBtnLBS.setOnClickListener(this);
}
@Override
public void onClick(View v) {
    switch (v.getId()) {
        //定位
        case R.id.btn_lbs:
            if (mBtnLBS.isChecked()) {
                mBtnLBS.setChecked(false, true);
                mTvLBS.setText("不显示地理位置");
            } else {
                mBtnLBS.setChecked(true, true);
                mTvLBS.setText("正在定位中");
                //调用定位功能
                mPublishPresenter.doLocation();
            }
            break;
    }
}
//定位成功
@Override
public void doLocationSuccess(String location) {
    mTvLBS.setText(location);
}
//定位失败
@Override
public void doLocationFailed() {
    mTvLBS.setText("定位失败");
    mBtnLBS.setChecked(false, false);
}
    /**
     * 权限验证回调
     * <p>
     * 1、权限通过 ActivityCompat 类的 checkSelfPermission() 方法判断是否有所需权限。PublishPresenter.java # checkPublishPermission()
     * 2、权限请求是通过 ActivityCompat 类中的 requestPermissions() 方法，
     * 在 OnRequestPermissionsResultCallback # onRequestPermissionsResult() 方法中回调。PublishActivity.java # onRequestPermissionsResult()
     * 3、应用程序可以提供一个额外的合理的使用权限调用 Activitycompat # shouldShowRequestPermissionRationale() 方法。
     * Android 原生系统中，如果第二次弹出权限申请的对话框，会出现「以后不再弹出」的提示框，如果用户勾选了，你再申请权限，
     * 则 shouldShowRequestPermissionRationale() 返回 true，意思是说要给用户一个解释，告诉用户为什么要这个权限。
     *
     * @param requestCode  请求码
     * @param permissions  权限数组
     * @param grantResults 授予结果数组
     */
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            //定位权限
            case Constants.LOCATION_PERMISSION_REQ_CODE:
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    //获取地理位置失败处理
                    if (!LocationMgr.getMyLocation(this, mPublishPresenter.getOnLocationListener())) {
                        mTvLBS.setText("定位失败");
                        mBtnLBS.setChecked(false, false);
                    }
                }
                break;
        }
    }
}
```
运行结果
![5205232-719135303ff6bfd7.jpeg](https://upload-images.jianshu.io/upload_images/5205232-719135303ff6bfd7.jpeg)
详情转至 [GitHub](https://link.jianshu.com?t=https://github.com/angelOnly/LiveStreaming)
参考：
[https://developer.android.com/reference/android/location/LocationManager.html#GPS_PROVIDER](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/LocationManager.html#GPS_PROVIDER)
[https://developer.android.com/reference/android/location/Geocoder.html](https://link.jianshu.com?t=https://developer.android.com/reference/android/location/Geocoder.html)
[http://blog.csdn.net/feiduclear_up/article/details/50704127](https://link.jianshu.com?t=http://blog.csdn.net/feiduclear_up/article/details/50704127)
[https://my.oschina.net/JumpLong/blog/89266](https://link.jianshu.com?t=https://my.oschina.net/JumpLong/blog/89266)
> 
撸这个项目的一半，你就是大神 , 戳[http://mp.weixin.qq.com/s/ZagocTlDfxZpC2IjUSFhHg](https://link.jianshu.com?t=http://mp.weixin.qq.com/s/ZagocTlDfxZpC2IjUSFhHg)
