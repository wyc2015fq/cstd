# iOS tableView下拉图片变大 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月18日 22:32:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：997
项目需要就做了一个tableView上HeaderView放图片下拉变大上拉导航栏变色的demo.
直接上代码
```
//引入封装的类
#import "TransparentView.h"
#import "UINavigationBar+Transparent.h"
@property (nonatomic,strong)UITableView * tableView;
```
然后我用懒加载初始化一个tableView
```
#pragma mark --懒加载
- (UITableView *)tableView
{
    if (!_tableView) {
        _tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)];
        _tableView.dataSource = self;
        _tableView.delegate = self;
    }
    return _tableView;
}
##实现他的两个协议方法
#pragma mark -- UITableViewDataSource,UITableViewDelegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 20;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
    }
    cell.textLabel.text = [NSString stringWithFormat:@"测试%ld",indexPath.row];
    return cell;
}
```
在viewDidLoad创建头部图片添加到tableHeaderView
```
UIImageView *topView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"9.jpg"]];
    topView.frame = CGRectMake(0, 0, self.view.frame.size.width, 200);
     TransparentView *transparentView = [TransparentView dropHeaderViewWithFrame:topView.frame contentView:topView stretchView:topView];
    transparentView.frame = topView.frame;
    self.tableView.tableHeaderView = transparentView;
    [self.view addSubview:self.tableView];
```
改变导航栏颜色的方法
```
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.navigationController.navigationBar js_setBackgroundColor:[UIColor clearColor]];
}
- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.navigationController.navigationBar js_reset];
}
```
最重要的实现这个协议
```
//根据偏移量改变navigationController背景
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat offsetY = scrollView.contentOffset.y;
    if (offsetY >0) {
        CGFloat alpha = (offsetY -64) / 64 ;
        alpha = MIN(alpha, 0.9);
        [self.navigationController.navigationBar js_setBackgroundColor:[[UIColor orangeColor] colorWithAlphaComponent:alpha]];
    } else {
        [self.navigationController.navigationBar js_setBackgroundColor:[UIColor clearColor]];
    }
}
```
项目地址: [https://github.com/b1003572686/downPhoto](https://github.com/b1003572686/downPhoto)
![](http://upload-images.jianshu.io/upload_images/1832246-4ee16133866a2c97.gif?imageMogr2/auto-orient/strip)
Untitled.gif
文／少侠你好（简书作者）
原文链接：http://www.jianshu.com/p/8b6733ef6a7b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
