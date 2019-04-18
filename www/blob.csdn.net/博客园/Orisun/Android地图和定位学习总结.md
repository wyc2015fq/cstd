# Android地图和定位学习总结 - Orisun - 博客园







# [Android地图和定位学习总结](https://www.cnblogs.com/zhangchaoyang/articles/1799531.html)





首届 Google 暑期大学生博客分享大赛——2010 Android 篇

**android.location包下有这么一些接口和类：**
**Interfaces**
GpsStatus.Listener
GpsStatus.NmeaListener
LocationListener
**Classes**
Address
Criteria
Geocoder
GpsSatellite
GpsStatus
Location
LocationManager
LocationProvider
**com.google.android.maps包下有这些类：**
**All Classes**
GeoPoint 
ItemizedOverlay 
ItemizedOverlay.OnFocusChangeListener 
MapActivity 
MapController 
MapView 
MapView.LayoutParams 
MapView.ReticleDrawMode 
MyLocationOverlay 
Overlay 
Overlay.Snappable 
OverlayItem 
Projection 
TrackballGestureDetector 
我们边看代码边熟悉这些类。




**要获取当前位置坐标，就是从Location对象中获取latitude和longitude属性。那Location对象是如何创建的？**
**LocationManager** locMan=(LocationManager)getSystemService(Context.LOCATION_SERVICE);//LocationManager对象只能这么创建，不能用new
**Location **location=locMan.getLastKnownLocation(LocationManager.GPS_PROVIDER);
if(location==null){
    location=locMan.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
}
//注意要为应用程序添加使用权限 

<uses-permissionandroid:name="android.permission.ACCESS_FINE_LOCATION"/>  
所谓getLastKnownLocation自然是获取最新的地理位置信息，那LocationManager.GPS_PROVIDER和LocationManager.NETWORK_PROVIDER有什么区别呢？俺也不是学通信的，对这个不了解，在网上看到有人想“在室外有GPS定位，在室内想用Wifi或基站定位”。
**除了直接使用LocationManager提供的静态Provider（如GPS_PROVIDER和NETWORK_PROVIDER等）外，还可以使用我们自己创建的LocationProvider对象。**
**创建LocationProvider对象一般要先创建Criteria对象，来设置我们的LocationProvider要满足什么样的标准**
Criteria myCri=new Criteria();
myCri.setAccuracy(Criteria.ACCURACY_FINE);//精确度
myCri.setAltitudeRequired(false);//海拔不需要
myCri.setBearingRequired(false);//Bearing是“轴承”的意思，此处可理解为地轴线之类的东西，总之Bearing Information是一种地理位置信息的描述
myCri.setCostAllowed(true);//允许产生现金消费
myCri.setPowerRequirement(Criteria.POWER_LOW);//耗电
String myProvider=locMan.getBestProvider(myCri,true);

#### public[String](http://www.cnblogs.com/reference/java/lang/String.html)getBestProvider([Criteria](http://www.cnblogs.com/reference/android/location/Criteria.html)criteria, boolean enabledOnly)

Returns the name of the provider that best meets the given criteria. Only providers that are permitted to be accessed by the calling activity will be returned. If several providers meet the criteria, the one with the best accuracy is returned. If no provider meets the criteria, the criteria are loosened in the following sequence:
power requirement
accuracy
bearing
speed
altitude
Note that the requirement on monetary cost is not removed in this process.
**Parameters**
criteria    the criteria that need to be matched
enabledOnly    if true then only a provider that is currently enabled is returned
**Returns**
name of the provider that best matches the requirements
only翻译为“最适合的"
Location location=locMan.getLastKnownLoation(myProvider);
double latitude=location.getLatitude();//获取纬度
double longitude=location.getLongitude();//获取经度

**我想知道当前位置描述（比如“武汉华中科技大学”而不是一个经纬值）呢？这就要使用GeoCoder创建一个Address对象了**。
**Geocoder** gc=new Geocoder(context,Locale.CHINA);//Locale是java.util中的一个类
List<Address> listAddress=gc.getFromLocation(latitude,longitude,1);
List<Address>     getFromLocation(double latitude, double longitude, int maxResults)
Returns an array of Addresses that are known to describe the area immediately surrounding the given latitude and longitude.(返回给定经纬值附近的一个Address）

既然是“附近”那实际编码时我们没必要把经纬值给的那么精确，而取一个近似的整数，像这样：

/*自经纬度取得地址，可能有多行地址*/

List<Address> listAddress=gc.getFromLocation((int)latitude,(int)longitude,1);

StringBuilder sb=new StringBuilder();

/*判断是不否为多行*/
if(listAddress.size()>0){
    Address address=listAddress.get(0);

　　for(int i=0;i<address.getMaxAddressLineIndex();i++){

　　　　sb.append(address.getAddressLine(i)).append("\n");

　　}

　　sb.append(address.getLocality()).append("\n");

　　sb.append(address.getPostalCode()).append("\n");

　　sb.append(address.getCountryName()).append("\n");

}

#### public intgetMaxAddressLineIndex()

Since:[API Level 1](http://www.cnblogs.com/guide/appendix/api-levels.html#level1)



Returns the largest index currently in use to specify an address line. If no address lines are specified, -1 is returned.



#### public[String](http://www.cnblogs.com/reference/java/lang/String.html)getAddressLine(int index)

Since:[API Level 1](http://www.cnblogs.com/guide/appendix/api-levels.html#level1)



Returns a line of the address numbered by the given index (starting at 0), or null if no such line is present.

String     getCountryName()

Returns the localized country name of the address, for example "Iceland", or null if it is unknown.

String     getLocality()

Returns the locality of the address, for example "Mountain View", or null if it is unknown.



反过来我们可以**输入地址信息获取经纬值**

Geocoder  mygeoCoder=new Geocoder(myClass.this,Locale.getDefault());

List<Address> lstAddress=mygeoCoder.getFromLocationName(strAddress,1);　　　　//strAddress是输入的地址信息

if(!lstAddress.isEmpty()){

　　Address address=lstAddress.get(0);

　　double latitude=address.getLatitude()*1E6;

　　double longitude=adress.getLongitude()*1E6;

　　GeoPoint geopoint=new GeoPoint((int)latitude,(int)longitude);

}



A class for handling geocoding and reverse geocoding. Geocoding is the process of transforming a street address or other description of a location into a (latitude, longitude) coordinate.
[Geocoder](http://www.cnblogs.com/reference/android/location/Geocoder.html#Geocoder%28android.content.Context%29)([Context](http://www.cnblogs.com/reference/android/content/Context.html)context)
Constructs a Geocoder whose responses will be localized for the default system Locale.

#### public[List](http://www.cnblogs.com/reference/java/util/List.html)<[Address](http://www.cnblogs.com/reference/android/location/Address.html)>getFromLocationName([String](http://www.cnblogs.com/reference/java/lang/String.html)locationName, int maxResults)

Since:[API Level 1](http://www.cnblogs.com/guide/appendix/api-levels.html#level1)



Returns an array of Addresses that are known to describe the named location, which may be a place name 

such as "Dalvik, Iceland", an address such as "1600 Amphitheatre Parkway, Mountain View, CA", an airport 

code such as "SFO", etc.. The returned addresses will be localized for the locale provided to this class's 

constructor.

The query will block and returned values will be obtained by means of a network lookup. The results are a best 

guess and are not guaranteed to be meaningful or correct. It may be useful to call this method from a thread 

separate from your primary UI thread.



##### Parameters

maxResults
max number of results to return. Smaller numbers (1 to 5) are recommended


##### Returns
- a list of Address objects. Returns null or empty list if no matches were found or there is no backend 
- service available.



##### Throws

[IOException](http://www.cnblogs.com/reference/java/io/IOException.html)
if the network is unavailable or any other I/O problem occurs




说了半天还只是个定位，地图还没出来。下面要用到**com.google.android.maps**包了

下面的代码我们让**地图移到指定点**

**GeoPoint** p=new GeoPoint((int)(latitude*1E6),(int)(longitude*1E6));

**MapView** mapview=(MapView)findViewById(R.id.mv);

**MapController** mapContr=mapview.getController();

mapview.displayZoomControls(true);//显示地图缩放的按钮

mapContr.animateTo(p);//带动画移到p点

mapContr.setZoom(7);

### setZoom
public int **setZoom**(int zoomLevel)Sets the zoomlevel of the map. The value will be clamped to be between 1 and 21 inclusive, thoughnot all areas have tiles at higher zoom levels. This just sets the level of the zoom directly; for a 
step-by-step zoom with fancy interstitial animations, use [`zoomIn()`](http://www.cnblogs.com/com/google/android/maps/MapController.html#zoomIn%28%29) or [`zoomOut()`](http://www.cnblogs.com/com/google/android/maps/MapController.html#zoomOut%28%29).- **Parameters:**

```

```
`zoomLevel`- At zoomLevel 1, the equator of the earth is 256 pixels long. Each successive zoom
 level is magnified by a factor of 2.- 
- **Returns:**

the new zoom level, between 1 and 21 inclusive.
**在地图上指定一点给出经纬值**

@Override

public boolean onTouchEvent(MotionEvent ev){

　　int actionType=ev.getAction();

　　switch(actionType){


　　case MotionEvent.ACTION_UP:

　　　　Projection projection=mapview.getProjection();

　　　　GeoPoint loc=projection.fromPixels((int)arg0.getX(),(int)arg0.getY());

　　　　String lngStr=Double.toString(loc.getLongitudeE6()/1E6);

　　　　String latStr=Double.toString(loc.getLatitudeE6()/1E6);

　　}

　　return false;


}
- public interface **Projection**

A Projection serves to translate between the coordinate system of x/y on-screen pixel coordinates and that




 of latitude/longitude points on the surface of the earth. You obtain a Projection from[`MapView.getProjection()`](http://www.cnblogs.com/com/google/android/maps/MapView.html#getProjection%28%29).




如果需要我们还可以**把经纬值转换成手机的屏幕坐标值**

Point screenCoords=new Point();         //android.graphics.Point;

GeoPoint geopoint=new GeoPoint((int)(latitude*1E6),(int)(longitude*1E6));

mapview.getProjection().toPixels(geopoint,screenCoords);

int x=screenCoords.x;

int y=screenCoords.y;

**放大缩小地图主要就是用setZoom(int ZoomLevel)函数**,让ZoomLevel不停往上涨（或往下降）就可以了
下面给出一个**com.google.android.maps.Overlay**的使用例子

import com.google.android.maps.GeoPoint;
import com.google.android.maps.MapActivity;
import com.google.android.maps.MapController;
import com.google.android.maps.MapView;
import com.google.android.maps.Overlay;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.os.Bundle;
import android.view.View;

public class MapsActivity extends MapActivity 
{    
    MapView mapView; 
    MapController mc;
    GeoPoint p;

    class MapOverlay extends com.google.android.maps.Overlay
    {
        @Override
        public boolean draw(Canvas canvas, MapView mapView, boolean shadow, long when) 
        {
            super.draw(canvas, mapView, shadow);                   

            //---translate the GeoPoint to screen pixels---
            Point screenPts = new Point();
            mapView.getProjection().toPixels(p, screenPts);

            //---add the marker---
            Bitmap bmp = BitmapFactory.decodeResource(
                getResources(), R.drawable.pushpin);            
            canvas.drawBitmap(bmp, screenPts.x, screenPts.y-50, null);         
            return true;
        }
    } 

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        //...
    }

    @Override
    protected boolean isRouteDisplayed() {
        // TODO Auto-generated method stub
        return false;
    }
}
**public void draw(android.graphics.Canvas canvas,**

**                 MapView mapView,**

**                 boolean shadow)**

Draw the overlay over the map. This will be called on all active overlays with shadow=true, to lay down 

the shadow layer, and then again on all overlays with shadow=false. By default, draws nothing.

Parameters:



canvas - The Canvas upon which to draw. Note that this may already have a transformation applied, so be sure 

to leave it the way you found it.



mapView - the MapView that requested the draw. Use MapView.getProjection() to convert between 

on-screen pixels and latitude/longitude pairs.

shadow - If true, draw the shadow layer. If false, draw the overlay contents.

**public boolean draw(android.graphics.Canvas canvas,**

****MapView mapView,****

**                    boolean shadow,**

**** long when)****

when - The timestamp of the draw.


Draw call for animated overlays. By default, calls through to draw(Canvas, MapView, boolean) and returns false.

Canvas公共方法

void     **drawBitmap(Bitmap bitmap, Rect src, Rect dst, Paint paint)**

Draw the specified bitmap, scaling/translating automatically to fill the destination rectangle.














