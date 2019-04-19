# tableViewCell的依次插入动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月27日 16:57:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3702
如果有一个小需求当我们的tableView不是直接加载到界面上,而是cell依次展示到我们的界面上只一个动画效果,我们需要怎嘛做呢?
其实很简单,我们数据源有多少 我么就依次 插入多少个cell 不就行了吗? 需要注意的地方时是,当我们要插入的时候 要保证,下面返回的cell个数是对的就行了呗.
```
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
return self.DatNum+1;   
}
```
下面是我的实现 直接拷贝代码沾到你的 ViewController.m里就行了
```
//  ViewController.m
//  tableViewCellAnimation
//
//  Created by 3D on 16/6/24.
//  Copyright © 2016年 3D. All rights reserved.
//
#import "ViewController.h"
@interface ViewController ()   <UITableViewDataSource,UITableViewDelegate>
@property(nonatomic,strong)UITableView *tableView;
@property(nonatomic,strong)NSArray *dataArr;
@property(nonatomic,strong)NSMutableArray *indesPaths;
@property (nonatomic,assign)int DatNum;
@property(nonatomic,strong) NSTimer * timer;
@end
@implementation ViewController
-(UITableView *)tableView{
if (!_tableView) {
    _tableView = [[UITableView alloc]initWithFrame:self.view.frame style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    [_tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"cell"];
    }
 return _tableView;
}
-(NSArray *)dataArr{
if (!_dataArr) {
    _dataArr = @[@"我是个1",
                 @"我是个2",
                 @"我是个3",
                 @"我是个4",
                 @"我是个5",
                 @"我是个6",
                 @"我是个7",
                 @"我是个8",
                 @"我是个9",
                 @"我是个10",
                 @"我是个11",
                 @"我是个12",
                 @"我是个13",
                 @"我是个14",
                 @"我是个15",
                                   ];
      }
  return _dataArr;
}
- (void)viewDidLoad {
[super viewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
[self.view addSubview:self.tableView];
self.DatNum = -1;
    NSMutableArray *indexPaths = @[].mutableCopy;
    self.indesPaths = indexPaths;
   self.timer =  [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(charusell) userInfo:nil repeats:YES];  
 }
-(void)charusell{
self.DatNum = self.DatNum +1;
if (self.DatNum < self.dataArr.count) {
    [self.indesPaths addObject:[NSIndexPath indexPathForItem:self.DatNum inSection:0]];
    [self.tableView insertRowsAtIndexPaths:self.indesPaths withRowAnimation:UITableViewRowAnimationRight];
    [self.indesPaths removeAllObjects];
}else{
    [self.timer invalidate];
   //记得当不用这个定时器的时候要销毁. 
   self.timer = nil;
   }
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
return self.DatNum+1;   
}
  -(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell" forIndexPath:indexPath];
cell.textLabel.text = self.dataArr[indexPath.row];
return cell;
 }
@end
```
效果如下
![](http://upload-images.jianshu.io/upload_images/1372768-69a252abf74939f2.gif?imageMogr2/auto-orient/strip)
2016-06-26 01_43_44.gif
推荐拓展阅读
[举报文章](#)- [广告及垃圾信息](/notes/4546879/abuse_reports?type=ad)
- [抄袭或未授权转载](/notes/4546879/abuse_reports?type=plagiarism)
- [其他](#report-modal)
著作权归作者所有
如果觉得我的文章对您有用，请随意打赏。您的支持将鼓励我继续创作！
文／大墙66370（简书作者）
原文链接：http://www.jianshu.com/p/ff0db8fafd0c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
