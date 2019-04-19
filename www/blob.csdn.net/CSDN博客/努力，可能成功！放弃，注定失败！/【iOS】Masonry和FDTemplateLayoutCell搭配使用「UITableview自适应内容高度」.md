# 【iOS】Masonry和FDTemplateLayoutCell搭配使用「UITableview自适应内容高度」 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月08日 15:08:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：675
> 
本文来自尚妆iOS团队[嘉文](https://weibo.com/u/3338232200)
发表于[尚妆github博客](https://github.com/ShowJoy-com/showjoy-blog/issues/24)，欢迎订阅！
# 准备:
###### 1.FDTemplateLayoutCell
- 由sunny大神出品的自动计算UITableviewCell高度
[FDTemplateLayoutCell_下载](https://github.com/forkingdog/UITableView-FDTemplateLayoutCell)
![](http://upload-images.jianshu.io/upload_images/1519620-9d921624a88db480.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
FDTemplateLayoutCell
![](http://upload-images.jianshu.io/upload_images/1519620-06aea39126aa55d7.gif?imageMogr2/auto-orient/strip)
UITableView-FDTemplateLayoutCell
###### 2.Masonry
- 目前最流行的AutoLayout框架,比较轻量级
[Masonry_下载](https://github.com/SnapKit/Masonry)
将上述两个第三方下载后(或者使用Cocoapods)导入工程,然后创建所需文件,此时的工程目录:
![](http://upload-images.jianshu.io/upload_images/1519620-cf40eda8e4c65e57.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
工程目录
### 自定义UITableView
MyTableViewCell.h 创建Model属性,用来传值
```
#import <UIKit/UIKit.h>
#import "Model.h"
@interface MyTableViewCell : UITableViewCell
@property (nonatomic, strong)Model *model;
@end
```
MyTableViewCell.m 使用Masonry布局
```
#import "MyTableViewCell.h"
#import "Masonry.h"
@implementation MyTableViewCell{
    UILabel *_textLB;
}
- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        [self createSubViews];
    }
    return self;
}
/**
 *  注意,不管布局多复杂,一定要有相对于cell.contentView的bottom的约束
 */
- (void)createSubViews{
    _textLB = [UILabel new];
    _textLB.backgroundColor = [UIColor orangeColor];
    _textLB.numberOfLines = 0;
    [self.contentView addSubview:_textLB];
    [_textLB mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.equalTo(self.contentView).offset(10);
        make.bottom.right.equalTo(self.contentView).offset(-10);
    }];
}
/**
 *  赋值
 *
 *  @param model ViewController传递过来的Model用来赋值
 */
- (void)setModel:(Model *)model{
    if (_model != model) {
        _model = model;
        _textLB.text = [NSString stringWithFormat:@"%@", model.text];
    }
}
```
### Model数据模型
Model.h 创建数据属性
```
#import <Foundation/Foundation.h>
@interface Model : NSObject
@property (nonatomic, copy)NSString *text;
@end
```
Model.m (使用KVC时，如果代码中的key值不存在，会抛出异常，可以在类中通过重写它提供下面的这个方法来解决这个问题)
```
#import "Model.h"
@implementation Model
- (void)setValue:(id)value forUndefinedKey:(NSString *)key{
}
@end
```
### ViewController视图控制器
ViewController.h
```
#import <UIKit/UIKit.h>
@interface ViewController : UIViewController
@end
```
ViewController.m 创建列表视图,并实现自适应高度
```
#import "ViewController.h"
#import "MyTableViewCell.h"
#import "Model.h"
#import "UITableView+FDTemplateLayoutCell.h"
@interface ViewController ()<UITableViewDataSource, UITableViewDelegate>
@end
@implementation ViewController{
    NSMutableArray *_allDataArr;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [UIColor lightGrayColor];
    [self initailData];
    [self createMianViews];
}
- (void)initailData{
    _allDataArr = [NSMutableArray array];
    /**
     *  虚拟数据
     */
    for (NSInteger i = 0; i < 3; i++) {
        Model *model = [Model new];
        model.text = @"在东方世界里，挑选小公牛到竞技场角斗有一定的程序。每一头被带进场地的公牛都要向手持长矛刺它的斗牛士发起进攻。其勇敢程度是根据它不顾矛刃的刺痛向斗牛士进攻的次数来认真评定的";
        [_allDataArr addObject:model];
    }
}
- (void)createMianViews{
    UITableView *myTableView = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStyleGrouped];
    myTableView.backgroundColor = [UIColor whiteColor];
    myTableView.delegate = self;
    myTableView.dataSource = self;
    myTableView.fd_debugLogEnabled = YES;       //打开自适应高度debug模式
    [self.view addSubview:myTableView];
    [myTableView registerClass:[MyTableViewCell class] forCellReuseIdentifier:@"cell"];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
#pragma mark -UITableViewDataSource
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    MyTableViewCell *cell = (MyTableViewCell *)[tableView dequeueReusableCellWithIdentifier:@"cell" forIndexPath:indexPath];
    [self setupModelOfCell:cell AtIndexPath:indexPath];
    return cell;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _allDataArr.count;
}
#pragma mark -UITableViewDelegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return [tableView fd_heightForCellWithIdentifier:@"cell" cacheByIndexPath:indexPath configuration:^(id cell) {
        [self setupModelOfCell:cell AtIndexPath:indexPath];
    }];
}
#warning 重点(自适应高度必须实现)
//预加载Cell内容
- (void)setupModelOfCell:(MyTableViewCell *)cell AtIndexPath:(NSIndexPath *)indexPath{
    cell.model = [_allDataArr objectAtIndex:indexPath.row];
}
@end
```
### 运行结果:
![](http://upload-images.jianshu.io/upload_images/1519620-5458257dc93421c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
运行结果
### 重点:
![](http://upload-images.jianshu.io/upload_images/1519620-5e589aaec7be64d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自适应内容高度关键
### 复杂视图:
![](http://upload-images.jianshu.io/upload_images/1519620-ca9064aa47b48ffe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
复杂视图自适应内容高度

