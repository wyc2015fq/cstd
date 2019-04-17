# Android的四种定位方式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年12月01日 22:11:50[boonya](https://me.csdn.net/boonya)阅读数：10356








Android 定位一般有四种方法，这四种方式分别是：GPS定位，WIFI定准，基站定位，AGPS定位。                                   


#### （1） Android GPS：



     需要GPS硬件支持，直接和卫星交互来获取当前经纬度，这种方式需要手机支持GPS模块（现在大部分的智能机应该都有了）。通过GPS方式准确度是最高的，但是它的缺点也非常明显：1，比较耗电；2，绝大部分用户默认不开启GPS模块；3，从GPS模块启动到获取第一次定位数据，可能需要比较长的时间；4，室内几乎无法使用。这其中，缺点2,3都是比较致命的。需要指出的是，GPS走的是卫星通信的通道，在没有网络连接的情况下也能用。要实用Adnroid平台的GPS设备，首先需要添加上权限，所以需要添加如下权限：
  
`<span style="font-size:10px;"><uses-permission android:name= android.permission.ACCESS_FINE_LOCATION > </uses-permission></span>`
```java
package com.baidu.mapapi.demo;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.provider.Settings;
import android.widget.TextView;
import android.widget.Toast;

/**
 * 
 * @author boonya
 *
 *<uses-permission android:name= android.permission.ACCESS_FINE_LOCATION >
 * </uses-permission>
 */
public class GPSActivity extends Activity
{
	// 获取位置管理服务
	LocationManager locationManager;

	/**
	 * 首先判断GPS模块是否存在或者是开启：
	 */
	protected void openGPSSettings()
	{
		LocationManager alm = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		if (alm.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER))
		{
			Toast.makeText(this, "GPS模块正常", Toast.LENGTH_SHORT).show();
			return;
		}

		Toast.makeText(this, "请开启GPS！", Toast.LENGTH_SHORT).show();

		Intent intent = new Intent(Settings.ACTION_SECURITY_SETTINGS);

		// 此为设置完成后返回到获取界面
		startActivityForResult(intent, 0); 

	}

	/**
	 * 定位位置监听器
	 */
	private final LocationListener locationListener = new LocationListener()
	{
		public void onLocationChanged(Location location)
		{
			updateToNewLocation(location);
		}

		public void onProviderDisabled(String provider)
		{
			updateToNewLocation(null);
		}

		public void onProviderEnabled(String provider)
		{
		}

		@Override
		public void onStatusChanged(String provider, int status, Bundle extras)
		{
			// 通过GPS获取位置
			Location location = locationManager.getLastKnownLocation(provider);
			
			updateToNewLocation(location);
		}
	};

	/**
	 *  如果开启正常，则会直接进入到显示页面，如果开启不正常，则会进行到GPS设置页面
	 */
	protected void getLocation()
	{
		
		
		String serviceName = Context.LOCATION_SERVICE;
		
		locationManager = (LocationManager) this.getSystemService(serviceName);
		
		// 查找到服务信息
		Criteria criteria = new Criteria();
		// 高精度
		criteria.setAccuracy(Criteria.ACCURACY_FINE);
		criteria.setAltitudeRequired(false);
		criteria.setBearingRequired(false);
		criteria.setCostAllowed(true);
		// 低功耗
		criteria.setPowerRequirement(Criteria.POWER_LOW); 
		 // 获取GPS信息
		String provider = locationManager.getBestProvider(criteria, true);
		// 通过GPS获取位置
		Location location = locationManager.getLastKnownLocation(provider);
		
		updateToNewLocation(location);
		
		// 设置监听*器，自动更新的最小时间为间隔N秒(1秒为1*1000，这样写主要为了方便)或最小位移变化超过N米
		
		locationManager.requestLocationUpdates(provider, 10 * 1000, 50,locationListener);
	}

	// 到这里就可以获取到地理位置信息了，但是还是要显示出来，那么就用下面的方法进行显示：
	private void updateToNewLocation(Location location)
	{
		TextView tv1;
		tv1 = (TextView) this.findViewById(R.id.gps);
		if (location != null)
		{
			double latitude = location.getLatitude();
			double longitude = location.getLongitude();
			tv1.setText(" 维度：" + latitude + " \n经度" + longitude);
		} else
		{
			tv1.setText("无法获取地理信息");
		}
	}

}
```

#### （2)  Android 基站定位：

     Android 基站定位只要明白了基站/WIFI定位的原理，自己实现基站/WIFI定位其实不难。基站定位一般有几种，第一种是利用手机附近的三个基站进行三角定位，由于每个基站的位置是固定的，利用电磁波在这三个基站间中转所需要时间来算出手机所在的坐标；第二种则是利用获取最近的基站的信息，其中包括基站
 id，location area code、mobile country code、mobile network code和信号强度，将这些数据发送到google的定位web服务里，就能拿到当前所在的位置信息，误差一般在几十米到几百米之内。其中信号强度这个数据很重要，这里笔者就不多做解释了，直接给出一个文章，这个文章写的非常好:
[http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html)


####  （3）Android Wifi定位：

    根据一个固定的WifiMAC地址，通过收集到的该Wifi热点的位置，然后访问网络上的定位服务以获得经纬度坐标。因为它和基站定位其实都需要使用网络，所以在Android也统称为Network方式。Java代码：

```java
package com.baidu.mapapi.demo;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.Serializable;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONObject;
import android.content.Context;
import android.location.Location;
import android.net.wifi.WifiManager;
import android.util.Log;
/**
 * 
 * @author boonya
 *
 */
public class WiFiInfoManager implements Serializable
{
	private static final long serialVersionUID = 8575908357834967866L;

	private Context context;

	public WiFiInfoManager(Context context)
	{
		super();
		this.context = context;
	}

	public WifiInfo getWifiInfo()
	{
		WifiManager manager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = new WifiInfo();
		info.mac = manager.getConnectionInfo().getBSSID();
		Log.i(" TAG", "WIFI MACis: " + info.mac);
		return info;
	}

	public class WifiInfo
	{
		public String mac;

		public WifiInfo()
		{
			super();
		}
	}

	/**
	 * 上面是取到WIFI的mac地址的方法，下面是把地址发送给google服务器,代码如下
	 * @param wifi
	 * @return
	 */
	public static Location getWIFILocation(WifiInfo wifi)
	{
		if (wifi == null)
		{
			Log.i("TAG", " wifi is null");
			return null;
		}
		DefaultHttpClient client = new DefaultHttpClient();
		
		HttpPost post = new HttpPost("http://www.google.com/loc/json");
		
		JSONObject holder = new JSONObject();
		
		try
		{
			holder.put("version", "1.1.0 ");
			holder.put("host", " maps.google.com");
			JSONObject data;
			JSONArray array = new JSONArray();
			if (wifi.mac != null && wifi.mac.trim().length() > 0)
			{
				data = new JSONObject();
				data.put("mac_address", wifi.mac);
				data.put("signal_strength", 8);
				data.put("age", 0);
				array.put(data);
			}
			holder.put("wifi_towers", array);
			Log.i("TAG", "request json: " + holder.toString());
			StringEntity se = new StringEntity(holder.toString());
			post.setEntity(se);
			HttpResponse resp = client.execute(post);
			int state = resp.getStatusLine().getStatusCode();
			if (state == HttpStatus.SC_OK)
			{
				HttpEntity entity = resp.getEntity();
				if (entity != null)
				{
					BufferedReader br = new BufferedReader(new InputStreamReader(entity.getContent()));
					StringBuffer sb = new StringBuffer();
					String resute = "";
					while ((resute = br.readLine()) != null)
					{
						sb.append(resute);
					}
					br.close();
					Log.i("TAG", "response json: " + sb.toString());
					data = new JSONObject(sb.toString());
					data = (JSONObject) data.get("location");
					Location loc = new Location(android.location.LocationManager.NETWORK_PROVIDER);
					loc.setLatitude((Double) data.get("latitude"));
					loc.setLongitude((Double) data.get("longitude"));
					loc.setAccuracy(Float.parseFloat(data.get("accuracy").toString()));

					loc.setTime(System.currentTimeMillis());
					return loc;
				} else
				{
					return null;
				}
			} else
			{
				Log.v("TAG", state + "");
				return null;
			}
		} catch (Exception e)
		{

			Log.e("TAG", e.getMessage());
			return null;
		}
	}
}
```

WIFI定位与基站定位的结合，笔者也在网上找到一个很好的文章，笔者对此就不做任何解释，直接给出网址：[http://www.cnblogs.com/coffeegg/archive/2011/10/01/2197129.html](http://www.cnblogs.com/coffeegg/archive/2011/10/01/2197129.html)

#### **（4）AGPS定位：**



     AGPS（AssistedGPS：辅助全球卫星定位系统）是结合GSM或GPRS与传统卫星定位，利用基地台代送辅助卫星信息，以缩减GPS芯片获取卫星信号的延迟时间，受遮盖的室内也能借基地台讯号弥补，减轻GPS芯片对卫星的依赖度。和纯GPS、基地台三角定位比较，AGPS能提供范围更广、更省电、速度更快的定位服务，理想误差范围在10公尺以内，日本和美国都已经成熟运用AGPS于LBS服务（Location
 Based Service，基于位置的服务）。AGPS技术是一种结合了网络基站信息和GPS信息对移动台进行定位的技术，可以在GSM/GPRS、WCDMA和CDMA2000网络中使进行用。该技术需要在手机内增加GPS接收机模块，并改造手机的天线，同时要在移动网络上加建位置服务器、差分GPS基准站等设备。AGPS解决方案的优势主要体现在其定位精度上，在室外等空旷地区，其精度在正常的GPS工作环境下，可以达到10米左右，堪称目前定位精度最高的一种定位技术。该技术的另一优点为：首次捕获GPS信号的时间一般仅需几秒，不像GPS的首次捕获时间可能要2～3分钟。
                                             


转载自：[http://www.cnblogs.com/cuihongyu3503319/p/3863867.html](http://www.cnblogs.com/cuihongyu3503319/p/3863867.html)



