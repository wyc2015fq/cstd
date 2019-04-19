# iOS开发之带你5分钟封装一个时间轴 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月27日 09:02:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2589标签：[iOS时间轴																[iOS学习资料																[iOS 资源大全																[ios开发](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS时间轴&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/1719878-d0d3a55b60d4c50a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Qinz
###### 本文来自简书，原文地址：[http://www.jianshu.com/p/bb6e9bf2bcc1](http://www.jianshu.com/p/bb6e9bf2bcc1)
###### 时间轴在一些app中用的场景还不少，原理实现起来较为简单，下面我们就来动手封装一个比较常用的时间轴，具体效果看下图：
![](http://upload-images.jianshu.io/upload_images/1719878-d5257fdbfd9b87c4.gif?imageMogr2/auto-orient/strip)
Qinz
- 1.首先我们创建一个UIView,在上面放一个tableView,声明一个方法，传递两个参数，第一个参数是需要将该时间轴放在哪个视图上，第二个参数是传递数据源，头文件下：
```
#import <UIKit/UIKit.h>
@interface QinzTimeLine : UIView
@property (nonatomic, strong) NSArray *titleArr;
-(void)setSuperView:(UIView*)superView DataArr:(NSMutableArray*)dataArr;
@end
```
- 2.我们再来看看.m文件,也就是最简单的tableView的应用，这里有一个 [self.tableView cellHeightForIndexPath:indexPath model:model keyPath:@"model" cellClass:[TimeLineCell class] contentViewWidth:self.frame.size.width]方法是用到了SDAutoLayout这个库用来自动计算cell高度的
```
#import "QinzTimeLine.h"
#import "SDAutoLayout.h"
#import "TimeLineCell.h"
@interface QinzTimeLine ()<UITableViewDelegate,UITableViewDataSource>
@property (nonatomic, strong) UITableView *tableView;
@property (nonatomic, strong) NSMutableArray *dataArr;
@end
@implementation QinzTimeLine
-(void)setSuperView:(UIView *)superView DataArr:(NSMutableArray *)dataArr{
    self.frame = superView.bounds;
    [superView addSubview:self];
    [self setUp];
    self.dataArr = dataArr;
}
-(void)setUp{
    self.tableView = [[UITableView alloc]init];
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    [self addSubview:self.tableView];
    self.tableView.sd_layout.topEqualToView(self).leftEqualToView(self).bottomEqualToView(self).rightEqualToView(self);
}
#pragma mark -- tableView的代理方法
#pragma mark -- 返回多少组
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}
#pragma mark -- 每组返回多少个
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.dataArr.count;
}
#pragma mark -- 每个cell的高度
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    TimeLineModel*model = self.dataArr[indexPath.row];
    return [self.tableView cellHeightForIndexPath:indexPath model:model keyPath:@"model" cellClass:[TimeLineCell class] contentViewWidth:self.frame.size.width];
}
#pragma mark -- 每个cell显示的内容
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    TimeLineCell*cell = [TimeLineCell timeLineCell:tableView];
    if (indexPath.row == 0) {
        cell.lineView.sd_layout.topSpaceToView(cell.pointView, 0);
        cell.lineView.backgroundColor = [UIColor grayColor];
        cell.pointView.backgroundColor = [UIColor redColor];
    }else{
        cell.lineView.sd_layout.topSpaceToView(cell.contentView, 0);
        cell.pointView.backgroundColor = [UIColor grayColor];
        cell.lineView.backgroundColor = [UIColor grayColor];
    }
    cell.model = self.dataArr[indexPath.row];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}
#pragma mark -- 选择每个cell执行的操作
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
}
```
- 3.关键在于tableViewCell布局，采用了Xib，方便对样式进行设置，布局依然采用的是SDAutoLayout这个库
![](http://upload-images.jianshu.io/upload_images/1719878-cf021a230985b90b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图片.png
- 4.看下布局代码,这里对titleLB的布局做高度自适应，及设置autoHeightRatio为0即可，然后我们直接在设置模型中调用 [self setupAutoHeightWithBottomView:self.titleLB bottomMargin:0]就自动完成了高度自适应，是不是很方便
```
- (void)awakeFromNib {
    [super awakeFromNib];
    self.pointView.sd_layout.topSpaceToView(self.contentView, 20).leftSpaceToView(self.contentView, 5).widthIs(8).heightEqualToWidth();
    self.pointView.sd_cornerRadius = @(4);
    self.lineView.sd_layout.topEqualToView(self.contentView).centerXEqualToView(self.pointView).widthIs(1).bottomSpaceToView(self.contentView, 0);
    self.ttimeLB.sd_layout.centerYEqualToView(self.pointView).leftSpaceToView(self.pointView, 10).rightSpaceToView(self.contentView, 10).heightIs(20);
    self.titleLB.sd_layout.topSpaceToView(self.ttimeLB, 15).leftEqualToView(self.ttimeLB).rightSpaceToView(self.contentView, 10).autoHeightRatio(0);
}
-(void)setModel:(TimeLineModel *)model{
    _model = model;
    self.titleLB.text=  model.title;
    self.ttimeLB.text = model.time;
    [self setupAutoHeightWithBottomView:self.titleLB bottomMargin:0];
}
```
- 5.到此，封装完毕，最后我们来看看控制器调用代码
```
- (void)viewDidLoad {
    [super viewDidLoad];
    self.automaticallyAdjustsScrollViewInsets = YES;
    self.timeLine = [[QinzTimeLine alloc]init];
    [self setData];
    [self.timeLine setSuperView:self.view DataArr:self.dataArr];
}
```
总结：整体主要采用tableView进行布局，然后让cell的高度自适应，需要注意的地方就是Cell中时间轴中的线条需要保持连贯，所以需要对第一个cell进行判断，让线条刚好与原点连接。
最后，附上demo供参考：[http://git.oschina.net/Qinz_323/qinztimeline](http://git.oschina.net/Qinz_323/qinztimeline)
