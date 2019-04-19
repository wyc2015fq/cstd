# iOS通过网络请求解析数据_中国省市区街道 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月25日 14:56:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1264
- 二话不说就上图,大家看看效果先
![](http://upload-images.jianshu.io/upload_images/1753861-4ff6f1f1e64ad176.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
上海市
![](http://upload-images.jianshu.io/upload_images/1753861-a8bbca40068bb942.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
云南省
##### 本篇文章主要涉及讲解表视图和集合视图和网络请求解析数据
- 以下的类的创建:
![](http://upload-images.jianshu.io/upload_images/1753861-87f77c10799df37d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里我给一个URL定位符:
> 
[http://apis.map.qq.com/ws/district/v1/list?key=K3VBZ-M6WWV-PPSPY-UVGGC-DRM2Z-PGBMV](http://apis.map.qq.com/ws/district/v1/list?key=K3VBZ-M6WWV-PPSPY-UVGGC-DRM2Z-PGBMV)
这里面是全国34个省直辖市, 市, 自治区, 市区,街道的详细介绍, 今天就讲一下网络请求的数据进行数据解析
#### ViewController.m
```
@interface ViewController ()<UICollectionViewDelegate,       UICollectionViewDataSource, UITableViewDelegate, UITableViewDataSource>
@property(nonatomic, retain)NSMutableArray *dataSourceProvinceArray;
/**省的集合数组*/
@property(nonatomic, retain)UICollectionView *collectionView;
/**市的表视图*/
@property(nonatomic, retain)UITableView *cityTableView;
/**区的表视图*/
@property(nonatomic, retain)UITableView *zoneTableView;
/**市的集合数组*/
@property(nonatomic, retain)NSMutableArray *dataSourceCityArray;
/**区的集合数组*/
@property(nonatomic, retain)NSMutableArray *dataSourceZoneArray;
/***/
@property(nonatomic, assign)NSInteger currentProvinceIndex;
@end
@implementation ViewController
```
- 咱们先把属性都准备好, 再把表视图和集合视图的协议签好
```
- (void)getData
{
    //请求数据并解析
    NSString *strURL = @"http://apis.map.qq.com/ws/district/v1/list?key=K3VBZ-M6WWV-PPSPY-UVGGC-DRM2Z-PGBMV";
    NSURL *URL = [NSURL URLWithString:strURL];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL];
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
    //数据源数组:
    self.dataSourceProvinceArray = [NSMutableArray array];
    NSArray *arr = [dic valueForKey:@"result"];
    for (NSDictionary *dic in arr[0])
    {
        ProvinceModel *model = [[ProvinceModel alloc] init];
        [model setValuesForKeysWithDictionary:dic];
        NSLog(@"%@ * %@", model.fullname, model.id);
        [self.dataSourceProvinceArray addObject:model];
        [model release];
    }
    //collectionView的数据刷新
}
```
- getData这个方法里是网络请求数据的解析省份数据信息
```
- (void)getcityDataById:(NSString *)proID
{
    NSString *urlString = [NSString stringWithFormat:@"http://apis.map.qq.com/ws/district/v1/getchildren?&id=%@&key=K3VBZ-M6WWV-PPSPY-UVGGC-DRM2Z-PGBMV", proID];
    NSURL *URL = [NSURL URLWithString:urlString];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL];
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
    NSArray *allArray = [dic objectForKey:@"result"];
    NSArray *array = [allArray objectAtIndex:0];
    //遍历当前数组给madel赋值
    self.dataSourceCityArray = [NSMutableArray array];
    for (NSDictionary *diction in array)
    {
        CityModel *model = [[CityModel alloc] init];
        [model setValuesForKeysWithDictionary:diction];
        [self.dataSourceCityArray addObject:model];
        [model release];
    }
    //请求市后 刷新tableView
    [self.cityTableView reloadData];
}
```
- getcityDataById:这个方法里是网络请求数据的解析市数据信息
```
- (void)getZoneDatabyId:(NSString *)cityID
{
    NSString *urlString = [NSString stringWithFormat:@"http://apis.map.qq.com/ws/district/v1/getchildren?&id=%@&key=K3VBZ-M6WWV-PPSPY-UVGGC-DRM2Z-PGBMV", cityID];
    NSURL *URL = [NSURL URLWithString:urlString];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL];
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
    NSArray *allArray = [dic objectForKey:@"result"];
    NSArray *array = [allArray objectAtIndex:0];
    //遍历当前数组给madel赋值
    self.dataSourceZoneArray = [NSMutableArray array];
    for (NSDictionary *diction in array)
    {
        ZoneModel *model = [[ZoneModel alloc] init];
        [model setValuesForKeysWithDictionary:diction];
        [self.dataSourceZoneArray addObject:model];
        [model release];
    }
    //请求区后 刷新tableView
    [self.zoneTableView reloadData];
}
```
- getZoneDatabyId:这个方法里是网络请求数据的解析区/街道数据信息
```
- (void)viewDidLoad {
    [super viewDidLoad];
    //设置省坐标为-1
    self.currentProvinceIndex = -1;
    //解析数据:
    [self getData];
    //初始化瀑布流flowLayout:
    UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
    flowLayout.itemSize = CGSizeMake(100, 50);
    flowLayout.minimumInteritemSpacing = 10;
    flowLayout.minimumLineSpacing = 10;
    flowLayout.sectionInset = UIEdgeInsetsMake(5, 5, 5, 5);
    flowLayout.scrollDirection = UICollectionViewScrollDirectionVertical;
    //初始化collectionView
    self.collectionView = [[UICollectionView alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, 300) collectionViewLayout:flowLayout];
    self.collectionView.delegate = self;
    self.collectionView.dataSource = self;
    self.collectionView.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:_collectionView];
    [_collectionView release];
    //注册collectionView的cell
    [self.collectionView registerClass:[ProvinceCell class] forCellWithReuseIdentifier:@"proCell"];
    //初始化市视图
    self.cityTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, _collectionView.frame.origin.y + _collectionView.frame.size.height, self.view.frame.size.width / 2.0, self.view.frame.size.height - 20 - _collectionView.frame.size.height) style:UITableViewStylePlain];
    self.cityTableView.delegate = self;
    self.cityTableView.dataSource = self;
    [self.view addSubview:_cityTableView];
    [_cityTableView release];
    //注册市tableView的cell
    [self.cityTableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"cityCell"];
    //初始化区视图
    self.zoneTableView = [[UITableView alloc] initWithFrame:CGRectMake(self.view.frame.size.width / 2.0,  _collectionView.frame.origin.y + _collectionView.frame.size.height,  self.view.frame.size.width / 2.0, self.view.frame.size.height - 20 - _collectionView.frame.size.height) style:UITableViewStylePlain];
    self.zoneTableView.delegate = self;
    self.zoneTableView.dataSource = self;
    [self.view addSubview:_zoneTableView];
    [_zoneTableView release];
    //注册区tableView的cell
    [self.zoneTableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"zoneCell"];
}
```
- 下面是表视图和集合视图的一些代理方法:
```
//--------------------------省collecctionView的代理方法--------------------------//
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return self.dataSourceProvinceArray.count;
}
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    ProvinceCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"proCell" forIndexPath:indexPath];
    //设置cell显示内容
    ProvinceModel *model = [self.dataSourceProvinceArray objectAtIndex:indexPath.row];
    cell.proNameLabel.text = model.fullname;
    return cell;
}
//item的点击方法
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    //判断当前省是否正在选中
    if (self.currentProvinceIndex != indexPath.row)
    {
        //获取当前省的model:
        ProvinceModel *model = [self.dataSourceProvinceArray objectAtIndex:indexPath.item];
        [self getcityDataById:model.id];
        //清空区的数组并更新
        [self.dataSourceZoneArray removeAllObjects];
        [self.zoneTableView reloadData];
        self.currentProvinceIndex = indexPath.row;
    }
}
//--------------------------城市tableView的代理方法--------------------------//
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (tableView == _cityTableView)
    {
        return self.dataSourceCityArray.count;
    }
    else
    {
        return self.dataSourceZoneArray.count;
    }
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (tableView == _cityTableView)
    {
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cityCell"];
        CityModel *model = [self.dataSourceCityArray objectAtIndex:indexPath.row];
        cell.textLabel.text = model.fullname;
        return cell;
    }
    else
    {
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"zoneCell"];
        ZoneModel *model = [self.dataSourceZoneArray objectAtIndex:indexPath.row];
        cell.textLabel.text = model.fullname;
        return cell;
    }
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (tableView == _cityTableView)
    {
        CityModel *model = [self.dataSourceCityArray objectAtIndex:indexPath.row];
        [self getZoneDatabyId:model.id];
    }
}
```
#### ProvinceCell.h
这里面只有一个属性
`@property(nonatomic, retain)UILabel *proNameLabel;`
#### ProvinceCell.m
```
- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        self.proNameLabel = [[UILabel alloc] init];
        self.proNameLabel.textAlignment = NSTextAlignmentCenter;
        self.proNameLabel.layer.borderWidth = 0.5;
        self.proNameLabel.layer.cornerRadius = 5;
        self.proNameLabel.font = [UIFont systemFontOfSize:12];
        [self.contentView addSubview:_proNameLabel];
        [_proNameLabel release];
    }
    return self;
}
//设置空间坐标frame
- (void)layoutSubviews
{
    [super layoutSubviews];
    self.proNameLabel.frame = self.bounds;
}
```
### CityModel.h
```
@property(nonatomic, copy)NSString *fullname;
@property(nonatomic, copy)NSString *name;
@property(nonatomic, copy)NSString *id;
```
### CityModel.m 和 ZoneModel.m
Model我们都只写一个容错方法就可以
`- (void)setValue:(id)value forUndefinedKey:(NSString *)key{}`
### ZoneModel.h
```
@property(nonatomic, copy)NSString *fullname;
@property(nonatomic, copy)NSString *name;
@property(nonatomic, copy)NSString *id;
```
##### 大功告成了, 小伙伴们是不是感觉真神奇! 中国那么大, 我想去看看! 要想去看看, 给个好评先!
文／一只不靠谱的猿_（简书作者）
原文链接：http://www.jianshu.com/p/7b7a355caa46
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
