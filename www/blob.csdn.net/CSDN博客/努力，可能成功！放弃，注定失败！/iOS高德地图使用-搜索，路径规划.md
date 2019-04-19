# iOS高德地图使用-搜索，路径规划 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月09日 14:34:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：7178
最近赶项目，好久没更新。抽出时间把高德地图的搜索和路径规划整理一下
项目中想加入地图功能，使用高德地图第三方，想要实现***确定一个位置，搜索路线并且显示***的方法。耗了一番功夫，总算实现了。
效果
![](http://upload-images.jianshu.io/upload_images/1819486-2795e42def71ae19.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
WeChat_1462507820.jpeg
### 一、配置工作
##### 1.申请key
访问 [http://lbs.amap.com/dev/key/](http://lbs.amap.com/dev/key/) 在高度地图第三方开发平台申请一个key，注册账户，新建应用，这个没什么门槛。
得到这个key
![](http://upload-images.jianshu.io/upload_images/1819486-b57b14288f86e435.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-05-06 上午10.34.15.png
提示一下，这个key对应的bundle ID 要和工程里面的bundle ID 相同，不然每次打开地图都会报一个Invalid_user_scode的提示。
##### 2.导入第三方
方便起见 pod导入
```
pod 'AMap3DMap' #3D地图SDK
#pod 'AMap2DMap' #2D地图SDK(2D地图和3D地图不能同时使用，2选1)
pod 'AMapSearch' #搜索服务SDK
```
##### 3.打开工程的后台定位功能 更改info.plist
增加两条，一条请求位置时提示，一条https
![](http://upload-images.jianshu.io/upload_images/1819486-c98ce38457865050.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-05-06 上午11.02.06.png
![](http://upload-images.jianshu.io/upload_images/1819486-5133871038eadec6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-05-06 上午10.48.19.png
## 二、地图基本显示
把地图添加到view即可显示
```
- (void)viewDidLoad {
    [super viewDidLoad];
    //配置用户Key
    [MAMapServices sharedServices].apiKey = @"76bb9bc3718375ad03acba7c333694c4";
    //把地图放在底层
    [self.view insertSubview:self.mapView atIndex:0];
}
//地图懒加载
- (MAMapView *)mapView
{
    if (!_mapView) {
        _mapView = [[MAMapView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.view.bounds), CGRectGetHeight(self.view.bounds))];
        _mapView.delegate = self;
        _mapView.showsUserLocation = YES;    //YES 为打开定位，NO为关闭定位
        [_mapView setUserTrackingMode: MAUserTrackingModeFollow animated:NO]; //地图跟着位置移动
        //自定义定位经度圈样式
        _mapView.customizeUserLocationAccuracyCircleRepresentation = NO;
        //地图跟踪模式
        _mapView.userTrackingMode = MAUserTrackingModeFollow;
        //后台定位
        _mapView.pausesLocationUpdatesAutomatically = NO;
        _mapView.allowsBackgroundLocationUpdates = YES;//iOS9以上系统必须配置
    }
    return _mapView;
}
```
## 三、地图搜索功能
遵守协议 AMapSearchDelegate
高德提供了多种搜索方式，**POI搜索（关键字查询、周边搜索、多边形查询），检索现实中真实存在的地物。**
提示搜索，就是在还没有输入完全时，根据已有字符进行的搜索
```
//搜索框激活时，使用提示搜索
-(void)updateSearchResultsForSearchController:(UISearchController *)searchController
{
    //发起输入提示搜索
    AMapInputTipsSearchRequest *tipsRequest = [[AMapInputTipsSearchRequest alloc] init];
   //关键字
    tipsRequest.keywords = _searchController.searchBar.text;
   //城市
    tipsRequest.city = _currentCity;
    //执行搜索
    [_search AMapInputTipsSearch: tipsRequest];
}
//实现输入提示的回调函数
-(void)onInputTipsSearchDone:(AMapInputTipsSearchRequest*)request response:(AMapInputTipsSearchResponse *)response
{
    if(response.tips.count == 0)
    {
        return;
    }
    //通过AMapInputTipsSearchResponse对象处理搜索结果
    //先清空数组
    [self.searchList removeAllObjects];
    for (AMapTip *p in response.tips) {    
        //把搜索结果存在数组
        [self.searchList addObject:p];
    }
    _isSelected = NO;
    //刷新表视图
    [self.tableView reloadData];
}
```
点击进行poi搜索
```
//周边搜索
- (IBAction)searchAction:(id)sender {
    //初始化检索对象
    _search = [[AMapSearchAPI alloc] init];
    _search.delegate = self;
    //构造AMapPOIAroundSearchRequest对象，设置周边请求参数
    AMapPOIAroundSearchRequest *request = [[AMapPOIAroundSearchRequest alloc] init];
    //当前位置
    request.location = [AMapGeoPoint locationWithLatitude:_currentLocation.coordinate.latitude longitude:_currentLocation.coordinate.longitude];
    //关键字
    request.keywords = _searchController.searchBar.text;
    NSLog(@"%@",_searchController.searchBar.text);
    // types属性表示限定搜索POI的类别，默认为：餐饮服务|商务住宅|生活服务
    // POI的类型共分为20种大类别，分别为：
    // 汽车服务|汽车销售|汽车维修|摩托车服务|餐饮服务|购物服务|生活服务|体育休闲服务|
    // 医疗保健服务|住宿服务|风景名胜|商务住宅|政府机构及社会团体|科教文化服务|
    // 交通设施服务|金融保险服务|公司企业|道路附属设施|地名地址信息|公共设施
    //    request.types = @"餐饮服务|生活服务";
    request.radius =  5000;//<! 查询半径，范围：0-50000，单位：米 [default = 3000]
    request.sortrule = 0;
    request.requireExtension = YES;
    //发起周边搜索
    [_search AMapPOIAroundSearch:request];
}
//实现POI搜索对应的回调函数
- (void)onPOISearchDone:(AMapPOISearchBaseRequest *)request response:(AMapPOISearchResponse *)response
{
    if(response.pois.count == 0)
    {
        return;
    }
    //通过 AMapPOISearchResponse 对象处理搜索结果
    [self.dataList removeAllObjects];
    for (AMapPOI *p in response.pois) {
        NSLog(@"%@",[NSString stringWithFormat:@"%@\nPOI: %@,%@", p.description,p.name,p.address]);
        //搜索结果存在数组
        [self.dataList addObject:p];
    }
    _isSelected = YES;
    [self.tableView reloadData];
}
```
## 四、路径规划
**规划路径查询（驾车路线搜索、公交换成方案查询、步行路径检索），提前知道出行路线**
```
//规划线路查询
- (IBAction)findWayAction:(id)sender {
    //构造AMapDrivingRouteSearchRequest对象，设置驾车路径规划请求参数
    AMapWalkingRouteSearchRequest *request = [[AMapWalkingRouteSearchRequest alloc] init];
    //设置起点，我选择了当前位置，mapView有这个属性
    request.origin = [AMapGeoPoint locationWithLatitude:_currentLocation.coordinate.latitude longitude:_currentLocation.coordinate.longitude];
    //设置终点，可以选择手点
    request.destination = [AMapGeoPoint locationWithLatitude:_destinationPoint.coordinate.latitude longitude:_destinationPoint.coordinate.longitude];
//    request.strategy = 2;//距离优先
//    request.requireExtension = YES;
    //发起路径搜索，发起后会执行代理方法
    //这里使用的是步行路径
    [_search AMapWalkingRouteSearch: request]; 
}
//长按手势响应方法，选择路径规划的终点，手势自己加
- (void)handleLongPress:(UILongPressGestureRecognizer *)gesture
{
    if (gesture.state == UIGestureRecognizerStateBegan)
    {
       //在地图上长按的位置
        CGPoint p = [gesture locationInView:_mapView];
        NSLog(@"press on (%f, %f)", p.x, p.y);
    }
     //转换成经纬度
    CLLocationCoordinate2D coordinate = [_mapView convertPoint:[gesture locationInView:_mapView] toCoordinateFromView:_mapView];
   //赋值给目标点
    _destinationPoint = [[MAPointAnnotation alloc] init];
    _destinationPoint.coordinate = coordinate;
}
```
执行路径搜索后会执行代理方法
```
//实现路径搜索的回调函数
- (void)onRouteSearchDone:(AMapRouteSearchBaseRequest *)request response:(AMapRouteSearchResponse *)response
{
    if(response.route == nil)
    {
        return;
    }
    //通过AMapNavigationSearchResponse对象处理搜索结果
    NSString *route = [NSString stringWithFormat:@"Navi: %@", response.route];
    AMapPath *path = response.route.paths[0]; //选择一条路径
    AMapStep *step = path.steps[0]; //这个路径上的导航路段数组
    NSLog(@"%@",step.polyline);   //此路段坐标点字符串
    if (response.count > 0)
    {
        //移除地图原本的遮盖
        [_mapView removeOverlays:_pathPolylines];
        _pathPolylines = nil;
        // 只显⽰示第⼀条 规划的路径
        _pathPolylines = [self polylinesForPath:response.route.paths[0]];
        NSLog(@"%@",response.route.paths[0]);
        //添加新的遮盖，然后会触发代理方法进行绘制
        [_mapView addOverlays:_pathPolylines];
    }
}
```
每次添加路线，区域，或者大头针等都会触发下面的代理方法
```cpp
```cpp
//绘制遮盖时执行的代理方法
- (MAOverlayRenderer *)mapView:(MAMapView *)mapView rendererForOverlay:(id <MAOverlay>)overlay
{
    /* 自定义定位精度对应的MACircleView. */
    //画路线
    if ([overlay isKindOfClass:[MAPolyline class]])
    {
       //初始化一个路线类型的view
        MAPolylineRenderer *polygonView = [[MAPolylineRenderer alloc] initWithPolyline:overlay];
        //设置线宽颜色等
        polygonView.lineWidth = 8.f;
        polygonView.strokeColor = [UIColor colorWithRed:0.015 green:0.658 blue:0.986 alpha:1.000];
        polygonView.fillColor = [UIColor colorWithRed:0.940 green:0.771 blue:0.143 alpha:0.800];
        polygonView.lineJoinType = kMALineJoinRound;//连接类型
        //返回view，就进行了添加
        return polygonView;
    }
    return nil;
}
```
```
最后，这是demo，最好真机测试，模拟器的定位不好用
[https://github.com/DaLiWangCC/MyOpen](https://github.com/DaLiWangCC/MyOpen)
文／简单也好（简书作者）
原文链接：http://www.jianshu.com/p/82e5dd0f8f78
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
