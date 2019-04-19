# IOS LocationManager定位国内偏移，火星坐标(GCJ-02)解决方法 - 月若无涯的专栏 - CSDN博客
2015年06月08日 11:19:03[月若无涯](https://me.csdn.net/u010124617)阅读数：684标签：[iOS开发																[定位																[火星坐标](https://so.csdn.net/so/search/s.do?q=火星坐标&t=blog)](https://so.csdn.net/so/search/s.do?q=定位&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS开发&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)
原文中有数值错误，转载时已经经过修正！
IOS7 WGS-84转GCJ-02(火星坐标)
CLLocationManager类可以实时的获得我们位置的经纬度，并且可以通过经纬度在MapView上定位:
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- //创建CLLocationManager对象
- CLLocationManager *locationManager = [[CLLocationManager alloc] init];  
- 
- //设置委托对象为自己
-    [locationManager setDelegate:self];  
- 
- //要求CLLocationManager对象返回全部结果
-    [locationManager setDistanceFilter:kCLDistanceFilterNone];  
- 
- //要求CLLocationManager对象的返回结果尽可能的精准
-    [locationManager setDesiredAccuracy:kCLLocationAccuracyBest];  
- 
- //要求CLLocationManager对象开始工作，定位设备位置
-    [locationManager startUpdatingLocation];  
通过下面的CLLocationManager的委托方法可以得到或者更新locationManager的经纬度，并且显示到MapView上
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- //CLLocationManager委托方法
- -(void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations{  
- //得到newLocation
- CLLocation *loc = [locations objectAtIndex:0];  
- }  
但是对于国内地图而言，使用LocationManager定位所获得经纬度，是有一段较大距离的偏移的。
这是为什么呢？？这几天一直在查这方面的资料，各种google、baidu论坛里给出的答案也各有不同：
wifi 热点 基站 gps等等所导致的都有，最后查找还是找到了问题所在，原来国内地图使用的坐标系统是GCJ-02而ios
 sdk中所用到的是国际标准的坐标系统WGS-84。
因为国内使用的是加密后的坐标系GCJ-02就是网络上叫的火星坐标。
locationManager就是因为得到的是火星坐标偏移后的经纬度，所以导致在MapView上有很大的偏差，而在MKMapView上通过定位自己位置所获得的经纬度有是准确，因为apple已经对国内地图做了偏移优化。
**1、那么临时的解决方法：想要获得自己准确的经纬度可以直接通过MKMapView中对自身定位来获得：**
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- //定义一个MKMapView 并且调用setShowUserLocation:YES来获得自身的位置
- [self.mapView setShowsUserLocation:YES];  
- 
- //如果不想要显示这个MKMapView就将其隐藏
- [self.mapView setHidden:YES];  
然后通过MKMapView的委托方法来获取准确的经纬度:
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- -(void)mapView:(MKMapView *)mapView didUpdateUserLocation:(MKUserLocation *)userLocation{  
- CLLocationCoordinate2D coord = [userLocation coordinate];  
- NSLog(@"经度:%f,纬度:%f",coord.latitude,coord.longitude);  
- }  
这个方法虽然简单，也可以实时的获取自己的位置而且基本上都是准确的位置，但是也就无法使用locationManager中的一些功能了。
那么有没有更好的解方法可以让我们在火星坐标上使用locationManager来获得准确的位置呢？ 看了几篇博客给出的答案是相似的，就是调用apple的私有模块类中得方法来对经纬度做一个偏移修正：
[http://blog.csdn.net/zhibudefeng/article/details/8495837](http://blog.csdn.net/zhibudefeng/article/details/8495837)
[http://yach.me/blog/2011/11/09/huo-xing-zuo-biao-xi-wen-ti-zai-iosxia-de-xiu/](http://blog.csdn.net/zhibudefeng/article/details/8495837)
上面两篇文章中都提到了使用apple的私有模块MKLocationManager来修正偏移，不过遗憾的是如果使用了私有模块会导致AppStore审核失败,但是幸运的是这种方式只能在IOS5以前的系统中使用。。。。。。至少今天我们不用担心该不该使用这个方式。
接下来就是正题了：最后我找到了一份android的火星坐标转换的算法，然后自己改写成了以下将WGS-84坐标系统转为GCJ-02的Objective-C代码，这样就能方便的使用CLLocationManager这个类了。
2、将WGS-84转为GCJ-02(火星坐标):
新建一个类“WGS84TOGCJ02”，在.h头文件中定义：
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- //  Copyright (c) 2013年 swinglife. All rights reserved.
- //
- 
- #import <Foundation/Foundation.h>
- #import <MapKit/MapKit.h>
- 
- @interface WGS84TOGCJ02 : NSObject  
- //判断是否已经超出中国范围
- +(BOOL)isLocationOutOfChina:(CLLocationCoordinate2D)location;  
- //转GCJ-02
- +(CLLocationCoordinate2D)transformFromWGSToGCJ:(CLLocationCoordinate2D)wgsLoc;  
- @end
在WGS84TOGCJ02.m文件中：
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- //  Copyright (c) 2013年 swinglife. All rights reserved.
- //
- 
- #import "WGS84TOGCJ02.h"
- 
- constdouble a = 6378245.0;  
- constdouble ee = 0.00669342162296594323;  
- constdouble pi = 3.14159265358979324;  
- 
- @implementation WGS84TOGCJ02
- 
- +(CLLocationCoordinate2D)transformFromWGSToGCJ:(CLLocationCoordinate2D)wgsLoc  
- {  
-     CLLocationCoordinate2D adjustLoc;  
- if([self isLocationOutOfChina:wgsLoc]){  
-         adjustLoc = wgsLoc;  
-     }else{  
- double adjustLat = [self transformLatWithX:wgsLoc.longitude - 105.0 withY:wgsLoc.latitude - 35.0];  
- double adjustLon = [self transformLonWithX:wgsLoc.longitude - 105.0 withY:wgsLoc.latitude - 35.0];  
- double radLat = wgsLoc.latitude / 180.0 * pi;  
- double magic = sin(radLat);  
-         magic = 1 - ee * magic * magic;  
- double sqrtMagic = sqrt(magic);  
-         adjustLat = (adjustLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);  
-         adjustLon = (adjustLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);  
-         adjustLoc.latitude = wgsLoc.latitude + adjustLat;  
-         adjustLoc.longitude = wgsLoc.longitude + adjustLon;  
-     }  
- return adjustLoc;  
- }  
- 
- //判断是不是在中国
- +(BOOL)isLocationOutOfChina:(CLLocationCoordinate2D)location  
- {  
- if (location.longitude < 72.004 || location.longitude > 137.8347 || location.latitude < 0.8293 || location.latitude > 55.8271)  
- returnYES;  
- returnNO;  
- }  
- 
- +(double)transformLatWithX:(double)x withY:(double)y  
- {  
- double lat = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));  
-     lat += (20.0 * sin(6.0 * x * pi) + 20.0 *sin(2.0 * x * pi)) * 2.0 / 3.0;  
-     lat += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;  
-     lat += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;  
- return lat;  
- }  
- 
- +(double)transformLonWithX:(double)x withY:(double)y  
- {  
- double lon = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));  
-     lon += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;  
-     lon += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;  
-     lon += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;  
- return lon;  
- }  
- 
- @end
最后我们通过判断isLocationOutOfChina 然后调用transformLatWithX方法就能获取转换后的 CLLocationCoordinate2D结构类型
**[objc]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/16972351#)[copy](http://blog.csdn.net/swingpyzf/article/details/16972351#)[print](http://blog.csdn.net/swingpyzf/article/details/16972351#)[?](http://blog.csdn.net/swingpyzf/article/details/16972351#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83405/fork)
- //CLLocationManager委托方法
- -(void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations{  
- //得到newLocation
- CLLocation *loc = [locations objectAtIndex:0];  
- //判断是不是属于国内范围
- if (![WGS84TOGCJ02 isLocationOutOfChina:[loc coordinate]]) {  
- //转换后的coord
-         CLLocationCoordinate2D coord = [WGS84TOGCJ02 transformFromWGSToGCJ:[loc coordinate]];  
-     }  
- 
