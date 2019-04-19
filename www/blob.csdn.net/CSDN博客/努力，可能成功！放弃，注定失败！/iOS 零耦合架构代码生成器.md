# iOS 零耦合架构代码生成器 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月12日 09:29:24[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：752
> 
刚刚学习完SpriteKit的第一章, 项目的需求就压过来了, 诶, 时间都去哪啦, 接着之前的热修复架构, 这次为了更加好的进行代码规范, 我们将之间的架构模式进行模板代码生成, 这里会用到一些简单的python, 没接触过的同学们可以先去了解下, 今天我就将零耦合代码生成工具分享与你.
![](http://upload-images.jianshu.io/upload_images/1229762-5fc9acef40e4fe85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
本文来自简书，原文地址:[http://www.jianshu.com/p/47d565bf200e](http://www.jianshu.com/p/47d565bf200e)
贴上WWDC大会的图振奋一下, 最近苹果股票也赚了不少, 哈哈, 其实代码生成工具好久之前就想试试学着开发, 一直没有头绪和所谓的契机, 最近在学习算法的时候顺带学了下python, 发现python的写文件的代码之简洁, 再看看OC, 诶... 刚学iOS的时候还觉得OC是最好的语言, 说实话, 现在好久不敲OC, 都有些手生了.
先按之前的架构写出对应OC模板 没看过Swift版的点击 --> [Hybird 实现热修复架构 Downgradable](http://www.jianshu.com/p/7054a694cfeb)
### 生成器目录结构
```
├── Template //模板文件
│   ├── ControllerTemplate.h
│   ├── ControllerTemplate.m
│   ├── InterfaceTemplate.h
│   ├── ModelTemplate.h
│   ├── ModelTemplate.m
│   ├── PresenterTemplate.h
│   ├── PresenterTemplate.m
│   ├── ViewModelTemplate.h
│   ├── ViewModelTemplate.m
│   ├── ViewTemplate.h
│   └── ViewTemplate.m
├── builder.py //生成器执行文件
└── config.json //生成器配置文件
```
##### InterfaceTemplate.h
```
#import <UIKit/UIKit.h>
@protocol <#Unit#>ViewOperation <NSObject>
<#ViewOperation#>
@end
@protocol <#Unit#>ModelInterface <NSObject>
<#ModelInterface#>
@end
@protocol <#Unit#>ViewModelInterface <NSObject>
@property (nonatomic,strong) id<<#Unit#>ModelInterface> model;
- (void)initializeWithParameter:(NSDictionary *)parameter finishedCallBack:(void(^)())finishCallBack;
<#ViewModelInterface#>
@end
@protocol <#Unit#>ViewInterface <NSObject>
@property (nonatomic,strong) id<<#Unit#>ViewModelInterface> <#unit#>ViewModel;
@property (nonatomic,strong) id<<#Unit#>ViewOperation> <#unit#>Operation;
@end
```
- InterfaceTemplate 是根据规则指定的一套协议, 用来进行解耦
- ViewOperation view的用户交互方法规则
- ModelInterface model的数据结构的规则
- ViewModelInterface viewModel的数据获取规则
- ViewInterface view的适配器模式
##### ModelTemplate.h
```
#import <Foundation/Foundation.h>
#import "<#Unit#>Interface.h"
@interface <#Unit#>Model : NSObject <<#Unit#>ModelInterface>
<#ModelInterface#>
- (instancetype)initWithDictionary:(NSDictionary *)dictionary;
+ (<#Unit#>Model *)modelWithDictionary:(NSDictionary *)dictionary;
@end
```
##### ModelTemplate.m
```
#import "<#Unit#>Model.h"
@implementation <#Unit#>Model
- (instancetype)initWithDictionary:(NSDictionary *)dictionary {
    self = [super init];
    if (self) {
        [self setValuesForKeysWithDictionary:dictionary];
    }
    return self;
}
+ (<#Unit#>Model *)modelWithDictionary:(NSDictionary *)dictionary {
    return [[<#Unit#>Model alloc]initWithDictionary:dictionary];
}
@end
```
- 简单的使用KVC进行映射, 也可以使用Runtime或第三方库
##### ViewModelTemplate.h
```
#import <Foundation/Foundation.h>
#import "<#Unit#>Interface.h"
@interface <#Unit#>ViewModel : NSObject <<#Unit#>ViewModelInterface>
@property (nonatomic,strong) id<<#Unit#>ModelInterface> model;
- (void)initializeWithParameter:(NSDictionary *)parameter finishedCallBack:(void(^)())finishCallBack;
<#ViewModelInterface#>
@end
```
##### ViewModelTemplate.m
```
#import "<#Unit#>ViewModel.h"
#import "<#Unit#>Model.h"
@implementation <#Unit#>ViewModel
- (void)initializeWithParameter:(NSDictionary *)parameter finishedCallBack:(void(^)())finishCallBack {
}
<#ViewModelImplementation#>
@end
```
- 仅新增初始化获取数据的方法, 其他的获取数据方法根据规则进行累加
##### PresenterTemplate.h
```
#import <UIKit/UIKit.h>
#import "<#Unit#>Interface.h"
@interface <#Unit#>Presenter : NSObject<<#Unit#>ViewOperation>
@property (nonatomic,strong) id<<#Unit#>ViewInterface> <#unit#>View;
@property (nonatomic,strong) id<<#Unit#>ViewModelInterface> <#unit#>ViewModel;
- (void)adapterWith<#Unit#>View:(id<<#Unit#>ViewInterface>)<#unit#>View <#unit#>ViewModel:(id<<#Unit#>ViewModelInterface>)<#unit#>ViewModel;
@end
```
##### PresenterTemplate.m
```
#import "<#Unit#>Presenter.h"
@implementation <#Unit#>Presenter
- (void)adapterWith<#Unit#>View:(id<<#Unit#>ViewInterface>)<#unit#>View <#unit#>ViewModel:(id<<#Unit#>ViewModelInterface>)<#unit#>ViewModel {
    _<#unit#>View = <#unit#>View;
    _<#unit#>ViewModel = <#unit#>ViewModel;
    [self dynamicBinding];
}
- (void)dynamicBinding {
    __weak typeof(self) _self = self;
    __weak id<<#Unit#>ViewModelInterface> __<#unit#>ViewModel = _<#unit#>ViewModel;
    [_<#unit#>ViewModel initializeWithParameter:nil finishedCallBack:^{
        _self.<#unit#>View.<#unit#>ViewModel = __<#unit#>ViewModel;
        _self.<#unit#>View.<#unit#>Operation = _self;
    }];
}
<#ViewOperation_m#>
@end
```
- 仅实现了viewModel和view之间初始化的解耦传输
##### ViewTemplate.h
```
#import <UIKit/UIKit.h>
#import "<#Unit#>Interface.h"
@interface <#Unit#>View : UIView <<#Unit#>ViewInterface>
@property (nonatomic,strong) id<<#Unit#>ViewOperation> <#unit#>Operation;
@property (nonatomic,strong) id<<#Unit#>ViewModelInterface> <#unit#>ViewModel;
@end
```
##### ViewTemplate.m
```
#import "<#Unit#>View.h"
@interface <#Unit#>View () <UITableViewDataSource, UITableViewDelegate>
@property (nonatomic,strong) UITableView * tableView;
@end
@implementation <#Unit#>View
- (void)dealloc {
    NSLog(@"%@ - execute %s",NSStringFromClass([self class]),__func__);
}
- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self setupSubviews];
    }
    return self;
}
- (instancetype)initWithCoder:(NSCoder *)coder  {
    self = [super initWithCoder:coder];
    if (self) {
        [self setupSubviews];
    }
    return self;
}
- (UITableView *)tableView {
    if (!_tableView) {
        _tableView = [UITableView new];
        _tableView.dataSource = self;
        _tableView.delegate = self;
    }
    return _tableView;
}
- (void)setupSubviews {
    [self addSubview:self.tableView];
}
- (void)set<#Unit#>ViewModel:(id<<#Unit#>ViewModelInterface>)<#unit#>ViewModel {
    _<#unit#>ViewModel = <#unit#>ViewModel;
    [_tableView reloadData];
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 0;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString * identifier = @"identifier";
    UITableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:identifier];
    }
    return cell;
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 44;
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}
- (void)layoutSubviews {
    [super layoutSubviews];
    _tableView.frame = self.bounds;
}
@end
```
- 仅实现了一个基本的TableView, 如需更改可替换.
##### ControllerTemplate.h
```
#import <UIKit/UIKit.h>
#import "<#Root#>ViewController.h"
@interface <#Unit#>ViewController : <#Root#>ViewController
@end
```
##### ControllerTemplate.m
```
#import "<#Unit#>ViewController.h"
#import "<#Unit#>Presenter.h"
#import "<#Unit#>ViewModel.h"
#import "<#Unit#>View.h"
@interface <#Unit#>ViewController ()
@property (nonatomic,strong) <#Unit#>Presenter * <#unit#>Presenter;
@property (nonatomic,strong) <#Unit#>ViewModel * <#unit#>ViewModel;
@property (nonatomic,strong) <#Unit#>View * <#unit#>View;
@end
@implementation <#Unit#>ViewController
- (void)loadView {
    [super loadView];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupView];
    [self adapterView];
}
- (<#Unit#>Presenter *)<#unit#>Presenter {
    if (!_<#unit#>Presenter) {
        _<#unit#>Presenter = [<#Unit#>Presenter new];
    }
    return _<#unit#>Presenter;
}
- (<#Unit#>ViewModel *)<#unit#>ViewModel {
    if (!_<#unit#>ViewModel) {
        _<#unit#>ViewModel = [<#Unit#>ViewModel new];
    }
    return _<#unit#>ViewModel;
}
- (<#Unit#>View *)<#unit#>View {
    if (!_<#unit#>View) {
        _<#unit#>View = [<#Unit#>View new];
        _<#unit#>View.frame = self.view.bounds;
    }
    return _<#unit#>View;
}
- (void)setupView {
    [self.view addSubview:self.<#unit#>View];
}
- (void)adapterView {
    [self.<#unit#>Presenter adapterWith<#Unit#>View:self.<#unit#>View <#unit#>ViewModel:self.<#unit#>ViewModel];
}
@end
```
- 进行所有层的配置
以上的内容在[热修复架构](http://www.jianshu.com/p/7054a694cfeb)的时候分别分了五篇来进行讲解, 这里仅是把Swift替换成了OC的版本.
##### config.json
```
{
    "super" : "", //父类的前缀
    "controller" : "Home", //控制器的前缀名
    "model" : { //模型的属性定义
        "models" : "Array"
    },
    "viewModel" : [ //获取数据的方法定义
        "register",
        "login",
        "exit",
        "refresh"
    ],
    "presenter" : [ //用户交互的方法定义
        "pushToNextViewController",
        "popToRootViewController",
        "showAlert",
        "closeButtonClick"
    ]
}
```
- 生成器的设计思想是通过读取一个JSON的规则, 来生成对应的一套架构模板, 便于进行快速开发和代码规范. 当然规则可以根据不同的项目制定不同的规则, 这里为了演示只做了最简单的版本.
### 生成器的实现
进行代码生成, 之前的老大是通过java进行实现了,奈何我对java很是无感, 最近在学习算法, 准备尝试数据和人工智能的方向, 就捎带学习了Python, 这里就使用Python实现OC的代码架构生成.
##### builder.py
```
import os, sys, json //导入python模块
def cur_file_dir(): //获取路径
   path = sys.path[0]
   if os.path.isdir(path):
        return path
   elif os.path.isfile(path):
        return os.path.dirname(path)
file_json = open(cur_file_dir() + "/config.json", "r") //打开JSON的配置表
data_json = json.loads(file_json.read()) //读取配置表中的数据
root = str(data_json["super"]) //获取父类的前缀
controller = str(data_json["controller"]) //获取控制器的前缀名
presenter_h = ''
presenter_m = ''
for operation in data_json["presenter"]:
   presenter_h += '\n- (void)' + str(operation) + ';'
   presenter_m += '- (void)' + str(operation) + ' {\n\n}\n\n'
view_model_h = ''
view_model_m = ''
for request in data_json["viewModel"]: //遍历获取数据的方法
   view_model_h += '- (void)' + str(request) + 'WithParameter:(NSDictionary *)parameter finishedCallBack:(void (^)())finishCallBack;\n' //进行OC版本的字符串拼接
   view_model_m += '\n- (void)' + str(request) + 'WithParameter:(NSDictionary *)parameter finishedCallBack:(void (^)())finishCallBack {\n\n}\n'
model = ''
for key in data_json["model"]: //遍历数据模型
   type = ''
   modified = ''
   value = data_json["model"][key]
   if value == 'String':
      type = 'NSString * ' 
      modified = 'copy'
   elif value == 'Int':
         type = 'NSInteger '
         modified = 'assign'
   elif value == 'Float':
         type = 'CGFloat '
         modified = 'assign'
   elif value == 'Bool':
         type = 'BOOL '
         modified = 'assign'
   elif value == 'Array':
         type = 'NSArray * '
         modified = 'strong'
   elif value == 'Dictionary':
         type = 'NSDictionary * '
         modified = 'strong'
   else:
         type = value
         modified = 'strong'
   model += '\n@property (nonatomic,' + modified + ') ' + type + str(key) + ';' //拼接数据的属性
file_json.close() //关闭JSON文件
os.path.join(cur_file_dir(), controller) //进行文件夹之间的关联
os.mkdir(cur_file_dir() + "/" + controller) //创建关联文件夹
def builder(path, file): //获取模板生成的封装
   r = open(cur_file_dir() + '/Template/' + path, 'r') //打开模板文件
   d = r.read() //读模板文件的数据
   r.close() //关闭模板文件
   w = open(cur_file_dir() + '/' + controller + '/' + controller + file, 'w') //创建目标文件
   w.write(d.replace('<#Unit#>', controller).replace('<#unit#>', controller.lower()).replace('<#Root#>', root).replace('<#ViewOperation#>', presenter_h).replace('<#ViewOperation_m#>', presenter_m).replace('<#ModelInterface#>', model).replace('<#ViewModelInterface#>', view_model_h).replace('<#ViewModelImplementation#>', view_model_m)) 进行写入文件
   w.close() //关闭文件
builder('InterfaceTemplate.h', 'Interface.h')
builder('ControllerTemplate.h', 'ViewController.h')
builder('ControllerTemplate.m', 'ViewController.m')
builder('ModelTemplate.h', 'Model.h')
builder('ModelTemplate.m', 'Model.m')
builder('PresenterTemplate.h', 'Presenter.h')
builder('PresenterTemplate.m', 'Presenter.m')
builder('ViewModelTemplate.h', 'ViewModel.h')
builder('ViewModelTemplate.m', 'ViewModel.m')
builder('ViewTemplate.h', 'View.h')
builder('ViewTemplate.m', 'View.m')
```
- 这样通过在终端中 python builder.py, 就能根据JSON表进行架构的生成了.
##### 彩蛋:
最近看了下Realm数据库, 虽说是很简单便捷, 但是需要继承, 耦合度太高, 原先使用的FMDB需要写SQL语句, 操作繁琐, 也不能一步到位. CoreData, 一直没有尝试过, 之后会进行学习.
只进行页面的缓存, 没有读写需求的使用数据库真是太痛苦了. 还不如写plist一劳永逸.
###### .h
```
+ (void)cache:(Class)cls data:(NSDictionary *)data;  //存
+ (void)requestDataWithClass:(Class)cls finishedCallBack:(void(^)(NSDictionary * response))finishedCallBack; //读
```
###### .m
```
+ (NSString *)cacheWithClass:(Class)cls {
    return [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.plist", NSStringFromClass([cls class])]];
    //返回路径
}
+ (void)cache:(Class)cls data:(NSDictionary *)data {
    [data writeToFile:[DataBase cacheWithClass:cls] atomically:YES];
    //写入沙盒
}
+ (void)requestDataWithClass:(Class)cls finishedCallBack:(void(^)(NSDictionary * response))finishedCallBack {
    finishedCallBack([NSDictionary dictionaryWithContentsOfFile:[DataBase cacheWithClass:cls]]);
    //抛出数据
}
```
###### viewModel
```
- (void)dynamicBindingWithFinishedCallBack:(void (^)())finishCallBack {
    [DataBase requestDataWithClass:[NSObject class] finishedCallBack:^(NSDictionary *response) {
        NSDictionary * data = response;
        NSArray * models = data[@"models"];
        [self.models removeAllObjects];
        for (NSDictionary * dict in models) {
            [self.models addObject:[ModelTemplate modelWithDictionary:dict]];
        }
        finishCallBack();
    }];
    [NetWork requestDataWithType:MethodGetType URLString:@"http://localhost:3001/api/J1/getJ1List" parameter:nil finishedCallBack:^(NSDictionary * response){
        NSDictionary * data = response[@"data"];
        NSArray * models = data[@"models"];
        [self.models removeAllObjects];
        for (NSDictionary * dict in models) {
            [self.models addObject:[ModelTemplate modelWithDictionary:dict]];
        }
        [DataBase cache:[NSObject class] data:data[@"models"]];
        finishCallBack();
    }];
}
```
- 上面是在vm层进行获取数据时的使用, 这里用NSObject作为参数, 实际用时可以缓存不同的Model类. 这样数据读取和网络读取的代码也非常的工整了.
##### github 下载地址!!!
![](http://upload-images.jianshu.io/upload_images/1229762-0a46036fb613eb0f.jpeg!heading?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
