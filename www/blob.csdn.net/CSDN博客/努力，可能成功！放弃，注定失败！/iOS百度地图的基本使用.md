# iOS百度地图的基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月05日 11:31:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1954
1.如何点击按钮放大和缩小地图？
在地图上创建两个按钮加上点击事件，点击事件中分别写上如下方法即可实现：
创建按钮：
```
//左边按钮
UIButton *leftBtn = [UIButton buttonWithType:UIButtonTypeSystem];
leftBtn.frame = CGRectMake(SCREEN_WIDTH*0.6, SCREEN_HEIGHT*0.93, 60, 30);
[leftBtn setBackgroundImage:[UIImage imageNamed:@"left_btn"] forState:UIControlStateNormal];
[leftBtn addTarget:self action:@selector(leftBtnAction:) forControlEvents:UIControlEventTouchUpInside];
[self.view addSubview:leftBtn];
//右边按钮
UIButton *rightBtn = [UIButton buttonWithType:UIButtonTypeSystem];
rightBtn.frame = CGRectMake(SCREEN_WIDTH*0.6+61, SCREEN_HEIGHT*0.93, 60, 30);
[rightBtn setBackgroundImage:[UIImage imageNamed:@"right_btn"] forState:UIControlStateNormal];
[rightBtn addTarget:self action:@selector(rightAction:) forControlEvents:UIControlEventTouchUpInside];
[self.view  addSubview:rightBtn];
实现点击事件:
- (void)leftBtnAction:(UIButton *)btn
{
[_mapView setZoomLevel:_mapView.zoomLevel-3];//缩小地图
}
- (void)rightAction:(UIButton *)btn
{
[_mapView setZoomLevel:_mapView.zoomLevel+3];//放大地图
}
```
2.当在地图上大头针要实现连续点击事件上时该怎么做？（百度地图大头针默认只能点击一次）
```
//实现连续点击
[_mapView deselectAnnotation:view.annotation animated:YES];
```
3.当我们想要获取手机屏幕上能看到的所有大头针的经纬度时该怎么做？
```
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
NSLog(@"\n 左上  %f,%f---------\n 右上  %f,%f-------\n 左下  %f,%f----- \n 右下  %f,%f",leftUpLong,leftUpLati,rightUpLong,rightUpLati,leftDownLong,leftDownlati,rightDownLong,rightDownLati);
```
4，百度地图的一些基本设置：
```
//设置百度地图的等级
[_mapView setZoomLevel:10];
//是否显示比例尺
mapView.showMapScaleBar = YES;
//比例尺在地图上的位置  
mapView.mapScaleBarPosition = CGPointMake(10,mapView.frame.size.height-45);
//地图是否支持旋转，系统默认是旋转的，即为YES，不想地图旋转设为NO
_mapView.rotateEnabled = YES;
//设定地图是否现显示3D楼块效果
_mapView.buildingsEnabled = YES;
```
5.如何在地图上循环创建多个大头针？
```
//循环遍历数组，数组里面包含所有的经纬度
for (AnnotaionModel *model in _annotaionArray) {
//判断当经纬度为0的时候不在地图上显示
if ([model.Latitude doubleValue]==0||[model.Longitude doubleValue]==0) {
continue;
}
_annotion = [[BMKPointAnnotation alloc]init];
CLLocationCoordinate2D coor = CLLocationCoordinate2DMake([model.Latitude doubleValue]  , [model.Longitude doubleValue]);
_annotion.coordinate = coor;
[_mapView addAnnotation:_annotion];
}
```
6.当你不想用系统的大头针，想用自己设置的图片来显示大头针时怎么做？
```
#pragma mark --BMKMapViewDelegate百度地图代理方法---
- (BMKAnnotationView *)mapView:(BMKMapView *)mapView viewForAnnotation:(id )annotation
{
BMKPinAnnotationView *newAnnotationView = [[BMKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"myAnnotation"];
newAnnotationView.pinColor = BMKPinAnnotationColorPurple;
for (AnnotaionModel *model in _annotaionArray) {
//设置大头针图片
newAnnotationView.image = [UIImage imageNamed:@"icon_stop"];
//当设为YES时view被选中时会弹出气泡，annotation必须实现了title这个方法，当为NO时点击大头针不会弹出气泡
newAnnotationView.canShowCallout = NO;
return newAnnotationView;
}
```
7.如何获取自身定位的经纬度：
```
#pragma mark --BMKLocationServiceDelegate百度地图定位代理方法--
- (void)didUpdateBMKUserLocation:(BMKUserLocation *)userLocation
{
CLLocationCoordinate2D coor = CLLocationCoordinate2DMake(userLocation.location.coordinate.latitude, userLocation.location.coordinate.longitude);
NSLog(@"%f,%f",coor.latitude,coor.longitude);
}
```
8.如何实现定位？
```
//自iOS SDK v2.5.0起，为了对iOS8的定位能力做兼容，做了相应的修改，开发者在使用过程中注意事项如下： 需要在info.plist里添加（以下二选一，两个都添加默认使用NSLocationWhenInUseUsageDescription）：
NSLocationWhenInUseUsageDescription ，允许在前台使用时获取GPS的描述
NSLocationAlwaysUsageDescription ，允许永久使用GPS的描述
_locationService = [[BMKLocationService alloc]init];
_locationService.delegate =self;//设置代理
[_locationService startUserLocationService];
//实现定位的代理方法
#pragma mark ----BMKLocationServiceDelegate---
- (void)didUpdateBMKUserLocation:(BMKUserLocation *)userLocation
{
CLLocationCoordinate2D coor = CLLocationCoordinate2DMake(userLocation.location.coordinate.latitude, userLocation.location.coordinate.longitude);
_mapView.centerCoordinate = coor;
[_mapView updateLocationData:userLocation];
}
```
最后打个小广告不是介意哈！[的iOS](http://lib.csdn.net/base/ios)开发交流群：529052159，iOS的公众号：iOS的开发精髓，每天都会发布精美文章动动你的小手扫码关注吧！
![](http://upload-images.jianshu.io/upload_images/1617037-d36cb897a1a08d8c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
