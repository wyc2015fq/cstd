# 关于iOS百度地图的基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年01月18日 10:12:37[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3582
      最近一段时间一直都想写一篇博客，想来想去不知道写些什么，因为刚进入iOS这个行业不到两个月（正式上班时间 ）也没学到很高深的经验技术啥的，真不知道有啥知识和大家分享的，另外一点怕写错了东西，被别人笑话，哎！我这个人就是要点面子，不过经过再三的思考还是决定放下面子，分享点东西给大家，虽然说我懂的不是太多，不过我想我分享的东西对一些刚进入iOS开发的新手们还是有些帮助的，还请大家高抬贵手，不喜勿喷哦！
最近工司项目中要用到百度地图，之前都没接触过百度地图，都不知道从何下手，所以不得不给自己定一个星期的时间来研究下百度地图官方iOS SDK。在这里也小小的总结了一些百度地图的常用方法，好了废话不多说，直接上代码吧！百度
1.百度地图基本属性：
_mapView = [[BMKMapViewalloc]initWithFrame:CGRectMake(0,
0, self.view.frame.size.width,
self.view.frame.size.height)];
//设置为卫星地图
    [_mapViewsetMapType:BMKMapTypeSatellite];
//设置为普通地图,系统默认为普通地图
    [_mapViewsetMapType:BMKMapTypeStandard];
//打开实时路况图层
    [_mapViewsetTrafficEnabled:YES];
//打开百度地图热力图图层（系统默认为关闭状态，关闭将YES设为NO）
    [_mapViewsetBaiduHeatMapEnabled:YES];
    [_mapViewsetBaiduHeatMapEnabled:NO];
//设定地图View能否支持用户多点缩放(双指)
_mapView.zoomEnabled =
YES;
//设置地图上是否显示比例尺
_mapView.showMapScaleBar =
YES;
//设置地图比例尺在地图上的位置
_mapView.mapScaleBarPosition =
CGPointMake(10,_mapView.frame.size.height-60);
//地图是否支持旋转，系统默认是旋转的，即为YES，不想地图旋转设为NO
_mapView.rotateEnabled =
YES;
//设定地图是否现显示3D楼块效果
_mapView.buildingsEnabled =
YES;
      //修改指南针在地图上的位置 
         pt = CGPointMake(10, 10);
         pt = CGPointMake(273, 10);
    [_mapView setCompassPosition:pt];
地图指南针
![](https://img-blog.csdn.net/20160118104843264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
地图路况开启图
![](https://img-blog.csdn.net/20160118104853640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
地图卫星开启图
![](https://img-blog.csdn.net/20160118104900564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
地图默认方式地图，普通地图
![](https://img-blog.csdn.net/20160118104907446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
地图热力开启图
![](https://img-blog.csdn.net/20160118104922988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.地图定位：一般刚做百度地图定位的同学们肯定都会遇到这种情况，为什么按照百度SDK上的方法写了以后，用模拟器运行代码怎么还是一直定位到北京，没有定位到自己的本身的位置。我刚做的时候也挺纳闷，弄了好几次都是在模拟器上无法定位到自己的位置，然后就是各种找度娘，各种逛博客，才知道Xcode的模拟器是无法来根据自己位置来定位的，如果想在模拟器上实现定位功能，第一个方法就是把模拟器上的经纬度写死，第二个方法就是将项目运行到真机上。（这里对第一种方法做下介绍，第二种方法就不用我多说了我想大家都会）
第一种方法：先把你的代码运行，点开模拟器
![](https://img-blog.csdn.net/20160118110821071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点开菜单栏的Debug 
![](https://img-blog.csdn.net/20160118110826917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击Debug下的location选择Custom Location选项
![](https://img-blog.csdn.net/20160118110840718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后输入你要定位到的经，纬度重新运行项目即可。
![](https://img-blog.csdn.net/20160118110845708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面给大家上定位的代码
#import <UIKit/UIKit.h>
#import <BaiduMapAPI_Map/BMKMapView.h>
#import "MyUtil.h"
#import <BaiduMapAPI_Location/BMKLocationService.h>
@interface LocationViewController :
UIViewController<BMKLocationServiceDelegate,BMKMapViewDelegate>
{
BMKMapView *_mapView；
   //定位
BMKLocationService *_locationService;
}
@end
//创建定位
_locationService = [[BMKLocationServicealloc]init];
_locationService.delegate =
self;
_mapView.showsUserLocation = 
YES;
  [_locationServicestartUserLocationService];
//遵循代理，实现代理方法
- (void)didUpdateBMKUserLocation:(BMKUserLocation *)userLocation
{   
//获取自身的位置
CLLocationCoordinate2D coor =
CLLocationCoordinate2DMake(userLocation.location.coordinate.latitude,
 userLocation.location.coordinate.longitude);
//将中心点设置为自身位置
_mapView.centerCoordinate = coor;
    [_mapViewupdateLocationData:userLocation];
}
3.大头针的创建：
-(void) viewDidAppear:(BOOL)animated
{
// 添加一个PointAnnotation    
        BMKPointAnnotation* annotation
=[[BMKPointAnnotation alloc]init];
        CLLocationCoordinate2D coor;
        coor.latitude=31.4;
        coor.longitude=121.24;
        annotation.coordinate= coor;
        annotation.title= @"这里是上海";
[_mapView addAnnotation:annotation];
}
// Override  
-(BMKAnnotationView
*)mapView:(BMKMapView
*)mapView viewForAnnotation:(id
<BMKAnnotation>)annotation  
{
if([annotation isKindOfClass:[BMKPointAnnotation
class]]){
            BMKPinAnnotationView *newAnnotationView
=[[BMKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"myAnnotation"];
            newAnnotationView.pinColor= BMKPinAnnotationColorPurple;     //设置大头针的颜色
            newAnnotationView.animatesDrop= YES;// 设置该标注点动画显示 
           //设置大头针在地图上是否可以拖动
           newAnnotationView.draggable =
YES;
return newAnnotationView;
}
return nil;
}
//根据自身位置创建大头针
- (void)didUpdateBMKUserLocation:(BMKUserLocation *)userLocation
{   
//获取自身的位置
CLLocationCoordinate2D coor = CLLocationCoordinate2DMake(userLocation.location.coordinate.latitude,
 userLocation.location.coordinate.longitude);
//将中心点设置为自身位置
_mapView.centerCoordinate = coor;
 BMKPointAnnotation* annotation =[[BMKPointAnnotation alloc]init];
    annotation.coordinate= coor;
[_mapView addAnnotation:annotation];
    [_mapViewupdateLocationData:userLocation];
}
4.在地图上添加按钮，实现点击按钮切换地图的等级，可放大放小
//创建按钮
- (void)createBtn
{
UIButton *leftBtn = [UIButtonbuttonWithType:UIButtonTypeSystem];
    leftBtn.frame =
CGRectMake(SCREEN_WIDTH*0.6,
SCREEN_HEIGHT*0.93,
60, 30);
    [leftBtn setBackgroundImage:[UIImageimageNamed:@"left_btn"]
forState:UIControlStateNormal];
    [leftBtn addTarget:selfaction:@selector(leftBtnAction:)
forControlEvents:UIControlEventTouchUpInside];
    [self.viewaddSubview:leftBtn];
UIButton *rightBtn = [UIButtonbuttonWithType:UIButtonTypeSystem];
    rightBtn.frame =
CGRectMake(SCREEN_WIDTH*0.6+61,
SCREEN_HEIGHT*0.93,
60, 30);
    [rightBtn setBackgroundImage:[UIImageimageNamed:@"right_btn"]
forState:UIControlStateNormal];
    [rightBtn addTarget:selfaction:@selector(rightAction:)
forControlEvents:UIControlEventTouchUpInside];
    [self.viewaddSubview:rightBtn];
}
//实现点击按钮地图放大和缩小，后面的数字表示，点击一次按钮放大或者缩小地图的等级数
- (void)leftBtnAction:(UIButton *)btn
{
    [_mapViewsetZoomLevel:_mapView.zoomLevel-3];
}
- (void)rightAction:(UIButton *)btn
{
    [_mapViewsetZoomLevel:_mapView.zoomLevel+3];
}
效果图：
![](https://img-blog.csdn.net/20160118114201896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
5.怎么获取我们手机屏幕范围内的大图针方法如下：
    //当前屏幕中心点的经纬度
    CGFloat centerLongitude = self.mapView.region.center.longitude;
    CGFloat centerLatitude = self.mapView.region.center.latitude;
    //当前屏幕显示范围的经纬度
    CLLocationDegrees pointssLongitudeDelta = self.mapView.region.span.longitudeDelta;
    CLLocationDegrees pointssLatitudeDelta = self.mapView.region.span.latitudeDelta;
    //左上角
    CGFloat leftUpLong = centerLongitude - pointssLongitudeDelta/2.0;
    CGFloat leftUpLati = centerLatitude - pointssLatitudeDelta/2.0;
    //右上角
    CGFloat rightUpLong = centerLongitude + pointssLongitudeDelta/2.0;
    CGFloat rightUpLati = centerLatitude - pointssLatitudeDelta/2.0;
    //左下角
    CGFloat leftDownLong = centerLongitude - pointssLongitudeDelta/2.0;
    CGFloat leftDownlati = centerLatitude + pointssLatitudeDelta/2.0;
    //右下角
    CGFloat rightDownLong = centerLongitude + pointssLongitudeDelta/2.0;
    CGFloat rightDownLati = centerLatitude + pointssLatitudeDelta/2.0;
   好吧，就写这么多吧！写的如果有不好的地方，有错的地方希望大家及时给我提出来！下次如果还有更好的方法会继续在和大家分享。最近我创了一个QQ群，里面都是一些刚走进iOS开发的同学，希望有意的朋友们可以一起进群跟我们一起探讨问题，一起学习  群号：529052159

