# BroadcastRecevier和Service开启APP-GPS数据采集服务 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年02月10日 17:21:38[boonya](https://me.csdn.net/boonya)阅读数：3214标签：[android																[lbs																[gps																[broadcastreceiver																[sqlite](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)





首先明确，采集GPS数据定位的方式有：GPS卫星定位、WIFI定位、基站定位、AGPS定位。GPS的应用在LBS行业软件中被视为核心。

#### 几种定位方式讲解

（1）**Android GPS：**需要GPS硬件支持，直接和卫星交互来获取当前经纬度，这种方式需要手机支持GPS模块（现在大部分的智能机应该都有了）。通过GPS方式准确度是最高的，但是它的缺点也非常明显：1，比较耗电；2，绝大部分用户默认不开启GPS模块；3，从GPS模块启动到获取第一次定位数据，可能需要比较长的时间；4，室内几乎无法使用。这其中，缺点2,3都是比较致命的。需要指出的是，GPS走的是卫星通信的通道，在没有网络连接的情况下也能用。


（2）**Android Wifi定位：**根据一个固定的WifiMAC地址，通过收集到的该Wifi热点的位置，然后访问网络上的定位服务以获得经纬度坐标。因为它和基站定位其实都需要使用网络，所以在Android也统称为Network方式。


（3)**Android 基站定位：**Android 基站定位只要明白了基站/WIFI定位的原理，自己实现基站/WIFI定位其实不难。基站定位一般有几种，第一种是利用手机附近的三个基站进行三角定位，由于每个基站的位置是固定的，利用电磁波在这三个基站间中转所需要时间来算出手机所在的坐标；第二种则是利用获取最近的基站的信息，其中包括基站 id，location area code、mobile country code、mobile network code和信号强度，将这些数据发送到google的定位web服务里，就能拿到当前所在的位置信息，误差一般在几十米到几百米之内。


（4）**AGPS定位：**AGPS（AssistedGPS：辅助全球卫星定位系统）是结合GSM或GPRS与传统卫星定位，利用基地台代送辅助卫星信息，以缩减GPS芯片获取卫星信号的延迟时间，受遮盖的室内也能借基地台讯号弥补，减轻GPS芯片对卫星的依赖度。和纯GPS、基地台三角定位比较，AGPS能提供范围更广、更省电、速度更快的定位服务，理想误差范围在10公尺以内，日本和美国都已经成熟运用AGPS于LBS服务（Location
 Based Service，基于位置的服务）。AGPS技术是一种结合了网络基站信息和GPS信息对移动台进行定位的技术，可以在GSM/GPRS、WCDMA和CDMA2000网络中使进行用。该技术需要在手机内增加GPS接收机模块，并改造手机的天线，同时要在移动网络上加建位置服务器、差分GPS基准站等设备。AGPS解决方案的优势主要体现在其定位精度上，在室外等空旷地区，其精度在正常的GPS工作环境下，可以达到10米左右，堪称目前定位精度最高的一种定位技术。该技术的另一优点为：首次捕获GPS信号的时间一般仅需几秒，不像GPS的首次捕获时间可能要2～3分钟
 。

 综上所述，最优方案应该是AGPS定位，因为它省时（定位时间快）、省电、精度高，接受度更高。
   

#### 实现GPS数据采集逻辑构思

（1）、实现GPS数据本地化：采用SQLite
 按照一定的时间频率（一般10s、15s、20s、30s甚至可以更大，视应用数据量而定，一般同样的时间采集的点越多那么更容易描述这段时间GPS设备移动的轨迹）采集GPS到本地SQLite数据库存储之。


（2）、实现GPS数据定时上传：每隔半个小时，定期向服务器抛出线程（采用异步线程）执行本地数据向远处服务器同步操作。

（3）、注册服务并允许在APP后台：采用BroadcastReceiver来注册本地服务（数据采集）和远程服务（数据上传）。

#### 实现功能需求

##### 创建Local-Service

本地GPS数据采集Service，SystemLocalService.java


```java
package com.boonya.wtms.service;

import com.boonya.wtms.domain.GPSLocation;
import com.boonya.wtms.utils.Constant;
import com.boonya.wtms.utils.DateParser;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;

public class SystemLocalService extends Service
{

	static LocationManager loctionManager;

	static String provider;

	private GPSLocationService gpsLocationService;

	Context tag = SystemLocalService.this;

	@Override
	public void onCreate()
	{
		super.onCreate();

		// 获取GPS位置处理类实例
		gpsLocationService = new GPSLocationService(tag);

	}

	@Override
	public IBinder onBind(Intent arg0)
	{
		return null;
	}

	// 位置监听器
	@Override
	@Deprecated
	public void onStart(Intent intent, int startId)
	{
		// 启动GPS监听
		initGPSLocationListener();
		super.onStart(intent, startId);
	}

	@Override
	public void onDestroy()
	{// 重写的onDestroy方法
		loctionManager.removeUpdates(locationListener);
		super.onDestroy();
	}

	private final LocationListener locationListener = new LocationListener()
	{

		@Override
		public void onStatusChanged(String provider, int status, Bundle extras)
		{

		}

		@Override
		public void onProviderEnabled(String provider)
		{

		}

		@Override
		public void onProviderDisabled(String provider)
		{

		}

		// 当位置变化时触发

		@Override
		public void onLocationChanged(Location location)
		{
			if (location != null)
			{
				Thread thread = new Thread(new DoCollectThread(location));
				thread.start();
			}

		}

	};

	/**
	 * 初始化GPS位置监听
	 */
	protected void initGPSLocationListener()
	{

		String contextService = Context.LOCATION_SERVICE;

		// 通过系统服务，取得LocationManager对象

		loctionManager = (LocationManager) getSystemService(contextService);

		// 3. 得到位置提供器，通过位置提供器，得到位置信息，可以指定具体的位置提供器，也可以提供一个标准集合，让系统根据
		// 标准匹配最适合的位置提供器，位置信息是由位置提供其提供的。

		// a. 通过GPS位置提供器获得位置(指定具体的位置提供器)

		// String provider = LocationManager.GPS_PROVIDER;

		// Location location = loctionManager.getLastKnownLocation(provider);

		// b. 使用标准集合，让系统自动选择可用的最佳位置提供器，提供位置

		Criteria criteria = new Criteria();

		criteria.setAccuracy(Criteria.ACCURACY_FINE);// 高精度

		criteria.setAltitudeRequired(true);// 要求海拔

		criteria.setBearingRequired(true);// 要求方位

		criteria.setSpeedRequired(true); // 要求速度

		criteria.setCostAllowed(true);// 允许有花费

		criteria.setPowerRequirement(Criteria.POWER_LOW);// 低功耗

		// 从可用的位置提供器中，匹配以上标准的最佳提供器

		provider = loctionManager.getBestProvider(criteria, true);
		// 获得最后一次变化的位置
		// Location location = loctionManager.getLastKnownLocation(provider);

		// 最后将位置信息显示在TextView中，如图：

		// 监听位置的变化

		// 监听位置变化，2秒一次，距离10米以上
		loctionManager.requestLocationUpdates(provider, Constant.GPS_COLLECT_TIME, Constant.GPS_COLLECT_DISTANCE, locationListener);

	}

	/**
	 * 本地GPS采集数据入库至SQLite
	 * 
	 * @author boonya
	 * 
	 * @having-line---------------------------------------------------------
	 * @filename SystemLocalService.java
	 * @function TODO
	 * @start-at 2014-12-11,下午4:47:14
	 * @having-line---------------------------------------------------------
	 */
	class DoCollectThread implements Runnable
	{
		Location location;

		public DoCollectThread(Location location)
		{
			this.location = location;
		}

		@Override
		public void run()
		{
			GPSLocation gpsLocation = new GPSLocation(-1, location.getLatitude(), location.getLongitude(), location.getAltitude(), location.getBearing(), location.getSpeed(), DateParser.getSystemDateStr());

			gpsLocationService.saveToLocal(gpsLocation);

		}

	}

}
```


##### 创建Remote-Service

将本地任务同步到服务端Service，SystemRemoteService.java


```java
package com.boonya.wtms.service;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.boonya.wtms.domain.GPSLocation;
import com.boonya.wtms.utils.Constant;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;

public class SystemRemoteService extends Service
{
	Context tag = SystemRemoteService.this;

	private GPSLocationService gpsLocationService;

	@Override
	public void onCreate()
	{
		super.onCreate();
		
		gpsLocationService=new GPSLocationService(tag);
	}

	@Override
	public IBinder onBind(Intent arg0)
	{
		return null;
	}

	@SuppressWarnings("deprecation")
	@Override
	public void onStart(Intent intent, int startId)
	{
		doService();
		
		super.onStart(intent, startId);
	}

	protected void doService()
	{
		// 调度任务执行
		new Timer().schedule(new TimerTask()
		{
			
			@Override
			public void run()
			{
				// 保存本地的GPS数据到远程服务器
				List<GPSLocation> gpsLocations=gpsLocationService.findAll();
				boolean flag=gpsLocationService.saveToServer(gpsLocations);
				if(flag)
				{
					// 删除已经上传的GPS数据
					gpsLocationService.delete(gpsLocations);
				}
			}
		}, 0, Constant.GPS_THREAD_TIME_WAIT);

		
	}

}
```

##### 创建系统BroadcastReceiver



```java
package com.boonya.wtms.service;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class SystemBroadcastReceiver extends BroadcastReceiver
{

	@Override
	public void onReceive(Context context, Intent intent)
	{
		// 调用Service
		context.startService(new Intent("com.boonya.wtms.service.System_LocalService"));
		Log.i("device-gps-local-service", "GPS本地采集服务启动成功!");

		// 调用Service
		context.startService(new Intent("com.boonya.wtms.service.System_RemoteService"));
		Log.i("device-gps-remote-service", "GPS远程上传服务启动成功!");
	}

}
```

##### 在AndroidManifest.xml注册Receiver和Service



```
<receiver android:name="com.boonya.wtms.service.SystemBroadcastReceiver" >
            <intent-filter>
                <action android:name="android.intent.action.BOOT_COMPLETED" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </receiver>
        

        <service android:name="com.boonya.wtms.service.SystemRemoteService" >
            <intent-filter>
                <action android:name="com.boonya.wtms.service.System_RemoteService" />

                <category android:name="android.intent.category.DEFAULT" />
            </intent-filter>
        </service>
        <service android:name="com.boonya.wtms.service.SystemLocalService" >
            <intent-filter>
                <action android:name="com.boonya.wtms.service.System_LocalService" />

                <category android:name="android.intent.category.DEFAULT" />
            </intent-filter>
        </service>
```

##### 加入访问设置权限



```
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" >
    </uses-permission>
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" >
    </uses-permission>
    <uses-permission android:name="android.permission.INTERNET" >
    </uses-permission>
    <uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" >
    </uses-permission>
    <!-- <uses-permission android:name="android.permission.WRITE_APN_SETTINGS"></uses-permission> -->
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" >
    </uses-permission>
    <uses-permission android:name="com.android.launcher.permission.READ_SETTINGS" />
    <uses-permission android:name="android.permission.WAKE_LOCK" >
    </uses-permission>
    <uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.ACCESS_GPS" />
```


注：此处GPS采集采用的是卫星定位（在采集过程中卫星探索很耗时间，且不确定性，有时候几分钟就连上了，有时候半个小时，甚至根本就连不上，这就要看Android GPS模块的生产厂商了），以上权限可能不是很完整，这个不做更正。



#### 关于性能的考虑

（1）、本地存储SQLite的存储能力：

     假设某个对象在SQLite中占用1kB，那么1000个对象约为1MB,而SQLite适合的是小数据量的存储，那么1000个，1500个是否还能正常存储呢？一般还是建议不超过500条（500KB）数据为宜，毕竟这是一个微型（MINI）数据库。

（2）、降低远程服务端的存储压力：

      鉴于SQLite的存储能力有限，所以本地数据到了服务端都会是一个问题，那就是存储，密集存储(复杂度O(n))还是散列存储(复杂度O(1))，这就是性能考虑的一个关键因素。这里使用的并不是采用Hash这种真实的散列存储，而是使用队列的方式，分散存储压力。](https://so.csdn.net/so/search/s.do?q=broadcastreceiver&t=blog)](https://so.csdn.net/so/search/s.do?q=gps&t=blog)](https://so.csdn.net/so/search/s.do?q=lbs&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




