# iOS MVVM+RAC 从框架到实战 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月01日 11:24:19[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4881
[一、前言](http://www.jianshu.com/p/3beb21d5def2)
[二、谈谈MVVM和RAC](http://www.jianshu.com/p/3beb21d5def2)
[1、MVVM浅析](http://www.jianshu.com/p/3beb21d5def2)
[2、RAC浅浅析](http://www.jianshu.com/p/3beb21d5def2)
[3、本篇对两者的理解运用](http://www.jianshu.com/p/3beb21d5def2)
[二、框架部分](http://www.jianshu.com/p/3beb21d5def2)
[1、框架目录详解](http://www.jianshu.com/p/3beb21d5def2)
[2、基类详解](http://www.jianshu.com/p/3beb21d5def2)
[3、题外话](http://www.jianshu.com/p/3beb21d5def2)
[三、实战部分（经典列表的实现）](http://www.jianshu.com/p/3beb21d5def2)
[1、LSCircleListViewController的处理](http://www.jianshu.com/p/3beb21d5def2)
[2、View的处理](http://www.jianshu.com/p/3beb21d5def2)
[3、LSCircleListModel的处理](http://www.jianshu.com/p/3beb21d5def2)
[4、ViewModel的处理](http://www.jianshu.com/p/3beb21d5def2)
[5、APPDelegate的代码简化](http://www.jianshu.com/p/3beb21d5def2)
[四、后记](http://www.jianshu.com/p/3beb21d5def2)
## **一、前言**
> 
很早之前就想写写自己在设计模式方面的心得，但是一直感觉自己是井底之蛙，毕竟在iOS领域越深入越感到自己的无知，心中有着敬畏之心，就更没有自信去写这个东西（你也可以理解是没时间(＞﹏＜)，请原谅我的装逼，嘿嘿）.
> 
**对于设计模式这个让人又爱又恨的玩意，说来其实简单，但一千个人眼中就有一千种哈姆雷特，说他千变万化确实是事实，而且当你深入其中的时候你真的会上瘾，并乐此不疲**！
前几天自己写的**一篇[《iOS Xcode全面剖析》](http://www.jianshu.com/p/3c8527898e84)阅读量在短短一天内破千，还上了简书首页（你看这句话字体就知道不是广告了( ⊙o⊙ )）**，确实很开心，昨天又跟我一朋友用代码讲解了我对MVVM的理解及运用，此情此景下，脑袋一热搞出一篇来分享给大家也情有可原，当然更希望有更多的大神来指点一下，让我自己也让大家有提升就够了，万分感谢！
## **二、谈谈MVVM和RAC**
### **1、MVVM浅析**
> 
到这里我就默认你看过MVVM相关文章（毕竟相关文章已经可以用满天飞来形容了~(≧▽≦)/~啦啦啦！），仅仅简要谈谈我对其的理解。
MVC是构建iOS App的标准模式，是苹果推荐的一个用来组织代码的权威范式，市面上大部分App都是这样构建的，具体组建模式不细说，iOS入门者都比较了解（虽然不一定能完全去遵守），但其几个不能避免的问题却是很严重困扰开发者比如**厚重的ViewController、遗失的网络逻辑（没有属于它的位置）、较差的可测试性**等因此也就会有**维护性较强、耦合性很低**的一种新架构MVVM (MVC
 引申出得新的架构)的流行。
MVVM虽然来自微软，但是不应该反对它，它正式规范了正式规范了视图和控制器紧耦合的性质，如下图：
![](http://upload-images.jianshu.io/upload_images/1686195-716b74a75817279e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MVVM图示
ViewModel: 相比较于MVC新引入的视图模型。是视图显示逻辑、验证逻辑、网络请求等代码存放的地方，唯一要注意的是，任何视图本身的引用都不应该放在VM中，换句话说就是VM中不要引入UIKit.h (对于image这个，也有人将其看做数据来处理，这就看个人想法了，并不影响整体的架构)。
这样，首先解决了VC臃肿的问题，将逻辑代码、网络请求等都写入了VM中，然后又由于VM中包含了所有的展示逻辑而且不会引用V，所以它是可以通过编程充分测试的。
so，就是这个样子的，6666！
### **2、RAC浅浅析**
> 
特别浅。。。本文重点是框架及实战及MVVM思想，RAC这玩意话说学习曲线较长，难以理解，不好上手，是因为之前学习的时候使用者、中文教程还比较少，所以学习运用起来比较费劲，(当时确实废了好大得劲，实力装逼一把 @%&$%& )但现在已经成熟的烂大街了，只要有心，好的教程一大把，能潜下心来看我写的水文的人，拿下RAC不在话下！
ReactiveCocoa 可以说是结合了函数式编程和响应式编程的框架，也可称其为函数响应式编程（FRP）框架，强调一点，RAC虽然最大的优点是**提供了一个单一的、统一的方法去处理异步的行为，包括delegate方法,blocks回调,target-action机制,notifications和KVO.**但是不要简单的只是单纯的认为他仅仅就是减少代码复杂度，更好的配合MVVM而已，小伙子，这样你就小看它了。
它最大的与众不同是提供了一种新的写代码的思维，**由于RAC将Cocoa中KVO、UIKit event、delegate、selector等都增加了RAC支持，所以都不用去做很多跨函数的事。**
**如果全工程都使用RAC来实现，对于同一个业务逻辑终于可以在同一块代码里完成了，将UI事件，逻辑处理，文件或数据库操作，异步网络请求，UI结果显示，这一大套统统用函数式编程的思路嵌套起来，进入页面时搭建好这所有的关系，用户点击后妥妥的等着这一套联系一个个的按期望的逻辑和次序触发，最后显示给用户。**
额，就说这么多，再说就没头了~(≧▽≦)/~啦啦啦!
### **3、本篇对两者的理解运用**
**在此次介绍中，会使用MVVM+RAC结合的方式，搞定一个添加上拉加载及下拉刷新的列表，所以更多的诠释MVVM思想，而不是RAC的逻辑链式操作（这一点用登录界面来写更能体现Y^o^Y ），RAC在此扮演的更大一部分的角色是更好的解耦，减少代码复杂度，使代码层次分明、逻辑清晰更便于维护升级。**
## **二、框架部分**
### **1、框架目录详解**
首先介绍一下本框架的目录结构，如下图
![](http://upload-images.jianshu.io/upload_images/1686195-eaa3247b76c7da54.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**1、Frameworks**
存放系统库的虚拟文件夹, **目前搭建框架的时候需要手动添加一个名称为Frameworks的虚拟文件夹,这样你在Build Phases 中添加的系统库会自动归入此文件夹，不会直接在外部显示以至于打乱目录结构。**系统库添加流程如下:
![](http://upload-images.jianshu.io/upload_images/1686195-9083083f67063979.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
另外,细心地家伙会发现此目录中有两个相同的Frameworks, 那这到底是什么鬼？最上面的那个Frameworks是在自己搭框架自己添加的,当时的项目还很单纯, 没有这么淘气，**问题出在下面那个Pods Target上，添加它之后就会自动给你生成一个虚拟的Frameworks的文件夹**，那又该问了为啥不直接用下面那个呢？？？（废话真多！反正也没冲突，就留着吧╮(╯﹏╰）╭）
既然提到了Pods，那接下来讲讲CocoaPods(第三方类库管理工具)。
**2、CocoaPods**
当你开发iOS应用时，会**经常使用到很多第三方开源类库，比如JSONKit，AFNetWorking等等。可能某个类库又用到其他类库，所以要使用它，必须得另外下载其他类库，而其他类库又用到其他类库**，“子子孙孙无穷尽也”，反正在早期我是体会过这种痛苦，好心酸，手动一个个去下载所需类库是十分麻烦的。
还有另外一种常见情况是，**你项目中用到的类库有更新，你必须得重新下载新版本，重新加入到项目中**，十分麻烦。
CocoaPods就是帮你解决上面的问题的，话说这玩意应该是iOS最常用最有名的类库管理工具了，**作为iOS程序员的我们，掌握CocoaPods的使用是必不可少的基本技能**了，至于这玩意该咋用？
O(∩_∩)O哈哈~你觉得我会告诉你么？好吧，我这人还是很心软的，下面一张图告诉你该咋用...(๑乛◡乛๑ 磨人的小妖精)
![](http://upload-images.jianshu.io/upload_images/1686195-805a1ff43232609c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
☝(•̀˓◞•́)哎呦，不错哦~是不是get了一个新技能 ？6666！
**3、AppDelegate**
这个目录下放的是AppDelegate.h(.m)文件，是整个应用的入口文件，所以单独拿出来。一会儿告诉你如何写一个简洁的AppDelegate，会在这个文件夹里添加一些类，所以将其放入一个文件夹内还是很有必要的。
**4、Class**
工程主体类, 日常大部分开发代码均在这里,又细分了好多次级目录。
**通用类**
- **General :** 通用类（文件夹项目移植过程中都不需要更改的就能直接使用的）- **Base :** 基类 （整个框架的基类）
- **Categories :** 公共扩展类 （就是一些常用的类别，比如分享啊什么的）
- **Core :** 公共核心类（一般存放个人信息、接口API等）
- **Models :** 公共Model （公用的一些数据模型）
- **Views :** 公共View （封装的一些常用的View）
**工具类**
- **Helpers :** 工程的相关辅助类（比如类似数据请求、表单上传、网络监测等工具类）
**宏定义类**
- **Macro :** 宏定义类 （就是整个应用会用到的宏定义）- **AppMacro.h** app项目的相关宏定义
- **NotificationMacro.h** 通知相关的宏定义
- **VendorMacro.h** 第三方相关宏定义
- **UtilsMacro.h** 为简化代码的宏定义
- ...等等等等（其他随你定啦！Y^o^Y ）
**APP具体模块代码类**
- **Sections :** 各模块的文件夹（一般而言，我们以人为单位）- **LSSections** 王隆帅的文件夹
- **CLSections** 马成麟的文件夹
- ...等等等等（也可以写你最喜欢的苍老师的，叼叼的！）
每个成员的文件夹下是其所负责模块的文件夹，比如苍老师负责PHP界面模块（我也认为PHP是最好的语言！大家可以在评论区谈论一下！๑乛◡乛๑ 磨人的小妖精），如下（接着上面的个人文件夹）:
- **PHP :** 模块名，也可以是首页（HomePage）...等等- **ViewControllers** 界面控制器存放处（这是文件夹名）
- **ViewModels** 打杂的（MVVM的核心、解耦合、处理逻辑等）
- **Views** 界面相关View存放处不(界面相关子View)
- **Models** 数据模型存放处（各种单纯的数据模型，一点都不胖，是标准的瘦Model）
这就是标准的MVVM了。。。为啥不和上面目录连起来呢？为啥呢？为啥呢？因为臣妾做不到啊！！！（不会三级、四级列表的MarkDown写法，求大神支招！良辰必有重谢！）
**第三方类库**
- **Vendors :** 第三方的类库/SDK，如UMeng、WeiboSDK、WeixinSDK等等。
> 
到这哥们又该疑惑了，心里该碎碎念了：(๑⁼̴̀д⁼̴́๑)ﾄﾞﾔｯ‼ What are you 弄啥嘞！刚才刚讲了个第三方库管理CocoaPods，你丫这里自己又搞了一个，ԅ( ¯་། ¯ԅ) 信不信我突突了你！
哈哈哈，刚才的CocoaPods确实管理着大部分的第三方库，这里建立第三方库目录的原因有两个：**其一，并不是所有的你需要的第三方都支持pods的，所以还是需要手动添加一些类库。其二，一些第三方库虽然支持pods，但是需要我们去更改甚至自定义这个第三方，此时也需要放入这里，也防止使用pods一不小心更新掉你的自定义！**ᕕ(ᐛ)ᕗ 你来打我啊！
**5、Resource**
这里放置的是工程所需的一些资源，如下
- **Fonts** 字体
- **Images** 图片（当然你可以添加至Assets.xcassets, 没人拦着你）
- **Sounds** 声音
- **Videos** 视频
ok，目录就讲到这里！想知道更详细的可以私信我！
### **2、基类详解**
> 
这里着重讲解一下VC、V、VM的基类，其他的模式与View类似所以略过，其中TableViewCell的基类稍微特殊所以也提一下。
我目前的基类如下图：
![](http://upload-images.jianshu.io/upload_images/1686195-8238b46d0dcf925d.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
是不是眼花缭乱了..., 我曾经也看它不顺眼, 曾经尝试过把基类都干掉，然后遇到了一些麻烦...就妥协了，在文章的最后可以跟大家聊聊我是怎么去干掉基类，然后又失败的，这里先详细讲一下基类。
**1、YDViewController**
![](http://upload-images.jianshu.io/upload_images/1686195-b793d1be067dc270.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1686195-44de2525b64a9e5a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
函数的具体用意图已经标的很清楚了，这里简单讲一下四个函数的作用
- 
**yd_addSubviews :** 添加View到ViewController
- 
**yd_bindViewModel :** 用来绑定V(VC)与VM
- 
**yd_layoutNavigation :** 设置导航栏、分栏
- 
**yd_getNewData :** 初次获取数据的时候调用（不是特别必要）
**2、YDView**
![](http://upload-images.jianshu.io/upload_images/1686195-ec775ebbb187b564.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1686195-cd00d3a72e921321.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- **yd_setupViews :** 添加子View到主View
- **yd_bindViewModel :** 绑定V与VM
- **yd_addReturnKeyBoard :** 设置点击空白键盘回收
**3、YDViewModel**
![](http://upload-images.jianshu.io/upload_images/1686195-235c7b065d9e7018.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1686195-8406e4248714ced5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- **yc_initialize :** 进行一些逻辑绑定，网络数据请求处理。
- **LSRefreshDataStatus** 数据处理后需要进行的操作标识- **LSHeaderRefresh_HasMoreData** 下拉还有更多数据
- **LSHeaderRefresh_HasNoMoreData** 下拉没有更多数据
- **LSFooterRefresh_HasMoreData** 上拉还有更多数据
- **LSFooterRefresh_HasNoMoreData** 上拉没有更多数据
- **LSRefreshError** 刷新出错
- **LSRefreshUI** 仅仅刷新UI布局
**4、YDTableViewCell**
![](http://upload-images.jianshu.io/upload_images/1686195-a5e78f8ae6b9a1d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
由于Cell比较特殊，所以单拎出来说一下。观察上面的ViewMdoel、View等的基类会发现每个基类都会有数据绑定的地方，但是cell得数据绑定需要放在数据初始化的时候，因为所有的基类的数据逻辑绑定都是在没有返回初始化对象的时候调用的，但是cell中假如在那里面进行数据绑定会出现问题比如下图：
![](http://upload-images.jianshu.io/upload_images/1686195-58c3025025020e67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cell复用失败
上图中的函数假如是在 `bindViewModel` 内，则会复用失败，点击按钮是没有反应的，但是假如是在数据初始化的时候调用：比如
`setViewModel` 的时候，就会OK了，因为里面用到了**cell的在RAC中复用机制 `rac_prepareForReuseSignal` ，在cell还没有初始化返回的时候是失效的。**
### **3、题外话**
**基类的作用是统一管理，统一风格，便于编码，有更多的额外的附加功能的话，建议使用Protocol 或 Category，这样移植性强，便于管理与扩展，不至于牵一发而动全身。**
本篇基类核心是用VM来配置V（VC），并提供一些必须的Protocol方法来处理界面显示、逻辑，将代码风格规范化，各个部分的功能明朗化，这样，**当你需要写什么，需要找什么，需要更改什么的时候都会很明确这些代码的位置，逻辑更清晰，而不会浪费更多的时间在思考应该写在哪，该去哪找，要改的地方在哪这种不该费时间的问题上。**
## **三、实战部分（经典列表的实现）**
> 
这里讲一下如下界面的代码构造方式，很普通的一个列表：(懒得再写了，这是我之前做的一个项目的一个界面，之前基类讲解中会看到都是YD开头的，在这里是YC开头就这个区别而已)
![](http://upload-images.jianshu.io/upload_images/1686195-4cb3966fac6bc933.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
首先观察这个界面，需求是：头部的内容数量多的话是可以左右滑动的，然后整体是可以上拉加载的。我是这样处理的：**首先界面整体是一个TableView，然后分为一个Header、一个Section和主体列表Row。在Header上嵌套一个CollectionView保证可复用。**具体分层如下
![](http://upload-images.jianshu.io/upload_images/1686195-b2fa505d3c2f3560.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**然后处理完后的目录如下：**
![](http://upload-images.jianshu.io/upload_images/1686195-4a77224847a022b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**简单介绍一下：**
- **ViewController**- **LSCircleListViewController :** 界面主控制器，负责跳转、Navgation、TabBar等
- **View**- **LSCircleListView :** 界面主View，负责主要界面的显示
- **LSCircleListHeaderView :** 头部Header，封装的内部含有一个CollectionView
- **LSCircleListCollectionCell :** 头部Header中的CollectionView自定义的Cell
- **LSCircleListSectionHeaderView :** SectionView，此界面不需复用，所以单纯一个View即可，若需要复用需要TableViewHeaderFooterView
- **LSCircleListTableCell :** 主TableView的Cell
- **ViewModel**- **LSCircleListViewModel :** 界面主ViewModel
- **LSCircleListHeaderViewModel :** 头部Header对应的ViewModel
- **LSCircleListCollectionCellViewModel :** 头部CollectionCell及TableViewCell的ViewModel（因为二者的数据结构是一致的）
- **LSCircleListSectionHeaderViewModel :** Section的ViewModel
- **Model**- **LSCircleListModel :** 圈子的数据模型（header和tableViewCell数据结构是一致的）
> 
一个小小的界面这么多类...是不是难以接受了，淡定些，骚年！你要想想把这些个东西都放在VC内是个什么赶脚？也得好几千行呢！（有点夸张！不过也够头疼的），这么多类，这里着重讲一下主VC、主V、主VM、主M就ok，能详细讲明白MVVM之间是如何工作的就一通百通了。
### **1、LSCircleListViewController的处理**
先上代码：
```
//
//  LSCircleListViewController.m
//  ZhongShui
//
//  Created by 王隆帅 on 16/3/10.
//  Copyright © 2016年 王隆帅. All rights reserved.
//
#import "LSCircleListViewController.h"
#import "LSCircleListView.h"
#import "LSCircleListViewModel.h"
#import "LSCircleMainPageViewController.h"
#import "LSCircleMainPageViewModel.h"
#import "LSCircleListCollectionCellViewModel.h"
#import "LSNewCircleListViewController.h"
@interface LSCircleListViewController ()
@property (nonatomic, strong) LSCircleListView *mainView;
@property (nonatomic, strong) LSCircleListViewModel *viewModel;
@end
@implementation LSCircleListViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}
#pragma mark - system
- (void)updateViewConstraints {
    WS(weakSelf)
    [self.mainView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(weakSelf.view);
    }];
    [super updateViewConstraints];
}
#pragma mark - private
- (void)yc_addSubviews {
    [self.view addSubview:self.mainView];
}
- (void)yc_bindViewModel {
    @weakify(self);
    [[self.viewModel.cellClickSubject takeUntil:self.rac_willDeallocSignal] subscribeNext:^(LSCircleListCollectionCellViewModel *viewModel) {
        @strongify(self);
        LSCircleMainPageViewModel *mainViewModel = [[LSCircleMainPageViewModel alloc] init];
        mainViewModel.headerViewModel.circleId = viewModel.idStr;
        mainViewModel.headerViewModel.headerImageStr = viewModel.headerImageStr;
        mainViewModel.headerViewModel.title = viewModel.name;
        mainViewModel.headerViewModel.numStr = viewModel.peopleNum;
        LSCircleMainPageViewController *circleMainVC = [[LSCircleMainPageViewController alloc] initWithViewModel:mainViewModel];
        [self.rdv_tabBarController setTabBarHidden:YES animated:YES];
        [self.navigationController pushViewController:circleMainVC animated:YES];
    }];
    [self.viewModel.listHeaderViewModel.addNewSubject subscribeNext:^(id x) {
        @strongify(self);
        LSNewCircleListViewController *newCircleListVC = [[LSNewCircleListViewController alloc] init];
        [self.rdv_tabBarController setTabBarHidden:YES animated:YES];
        [self.navigationController pushViewController:newCircleListVC animated:YES];
    }];
}
- (void)yc_layoutNavigation {
    self.title = @"圈子列表";
    [self.rdv_tabBarController setTabBarHidden:NO animated:YES];
}
#pragma mark - layzLoad
- (LSCircleListView *)mainView {
    if (!_mainView) {
        _mainView = [[LSCircleListView alloc] initWithViewModel:self.viewModel];
    }
    return _mainView;
}
- (LSCircleListViewModel *)viewModel {
    if (!_viewModel) {
        _viewModel = [[LSCircleListViewModel alloc] init];
    }
    return _viewModel;
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
/*
#pragma mark - Navigation
// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
@end
```
**对于VC，分为三个模块，下面分别来说一下：**
**i 第一个模块：系统函数**
![](http://upload-images.jianshu.io/upload_images/1686195-04af573b1d6e0369.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
此函数是从iOS6.0开始在ViewController中新增一个更新约束布局的方法，这个方法默认的实现是调用对应View的
`updateConstraints` 。ViewController的View在更新视图布局时，会先调用ViewController的updateViewConstraints 方法**。我们可以通过重写这个方法去更新当前View的内部布局，而不用再继承这个View去重写-updateConstraints方法。我们在重写这个方法时，务必要调用 super 或者 调用当前View的 -updateConstraints 方法。**
**ⅱ 第二个模块 : 私有函数**
![](http://upload-images.jianshu.io/upload_images/1686195-e3bbee89dc027cd8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
前面基类内也提到了这三个函数的具体作用，即
- 
**yd_addSubviews :** 添加View到ViewController
- 
**yd_bindViewModel :** 这里绑定了两个跳转事件。
- 
**yd_layoutNavigation :** 设置了标题为“圈子列表”、及TabBar不隐藏
**ⅲ 第三个模块 : 懒加载**
![](http://upload-images.jianshu.io/upload_images/1686195-f66c401a951fa253.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这就不用解释了，用到时再加载。
### **2、View的处理**
先上代码
```
//
//  LSCircleListView.m
//  ZhongShui
//
//  Created by 王隆帅 on 16/3/10.
//  Copyright © 2016年 王隆帅. All rights reserved.
//
#import "LSCircleListView.h"
#import "LSCircleListViewModel.h"
#import "LSCircleListHeaderView.h"
#import "LSCircleListSectionHeaderView.h"
#import "LSCircleListTableCell.h"
@interface LSCircleListView () <UITableViewDataSource, UITableViewDelegate>
@property (strong, nonatomic) LSCircleListViewModel *viewModel;
@property (strong, nonatomic) UITableView *mainTableView;
@property (strong, nonatomic) LSCircleListHeaderView *listHeaderView;
@property (strong, nonatomic) LSCircleListSectionHeaderView *sectionHeaderView;
@end
@implementation LSCircleListView
/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect {
 // Drawing code
 }
 */
#pragma mark - system
- (instancetype)initWithViewModel:(id<YCViewModelProtocol>)viewModel {
    self.viewModel = (LSCircleListViewModel *)viewModel;
    return [super initWithViewModel:viewModel];
}
- (void)updateConstraints {
    WS(weakSelf)
    [self.mainTableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(weakSelf);
    }];
    [super updateConstraints];
}
#pragma mark - private
- (void)yc_setupViews {
    [self addSubview:self.mainTableView];
    [self setNeedsUpdateConstraints];
    [self updateConstraintsIfNeeded];
}
- (void)yc_bindViewModel {
    [self.viewModel.refreshDataCommand execute:nil];
    @weakify(self);
    [self.viewModel.refreshUI subscribeNext:^(id x) {
        @strongify(self);
        [self.mainTableView reloadData];
    }];
    [self.viewModel.refreshEndSubject subscribeNext:^(id x) {
        @strongify(self);
        [self.mainTableView reloadData];
        switch ([x integerValue]) {
            case LSHeaderRefresh_HasMoreData: {
                [self.mainTableView.mj_header endRefreshing];
                if (self.mainTableView.mj_footer == nil) {
                    self.mainTableView.mj_footer = [MJRefreshBackNormalFooter footerWithRefreshingBlock:^{
                        @strongify(self);
                        [self.viewModel.nextPageCommand execute:nil];
                    }];
                }
            }
                break;
            case LSHeaderRefresh_HasNoMoreData: {
                [self.mainTableView.mj_header endRefreshing];
                self.mainTableView.mj_footer = nil;
            }
                break;
            case LSFooterRefresh_HasMoreData: {
                [self.mainTableView.mj_header endRefreshing];
                [self.mainTableView.mj_footer resetNoMoreData];
                [self.mainTableView.mj_footer endRefreshing];
            }
                break;
            case LSFooterRefresh_HasNoMoreData: {
                [self.mainTableView.mj_header endRefreshing];
                [self.mainTableView.mj_footer endRefreshingWithNoMoreData];
            }
                break;
            case LSRefreshError: {
                [self.mainTableView.mj_footer endRefreshing];
                [self.mainTableView.mj_header endRefreshing];
            }
                break;
            default:
                break;
        }
    }];
}
#pragma mark - lazyLoad
- (LSCircleListViewModel *)viewModel {
    if (!_viewModel) {
        _viewModel = [[LSCircleListViewModel alloc] init];
    }
    return _viewModel;
}
- (UITableView *)mainTableView {
    if (!_mainTableView) {
        _mainTableView = [[UITableView alloc] init];
        _mainTableView.delegate = self;
        _mainTableView.dataSource = self;
        _mainTableView.backgroundColor = GX_BGCOLOR;
        _mainTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        _mainTableView.tableHeaderView = self.listHeaderView;
        [_mainTableView registerClass:[LSCircleListTableCell class] forCellReuseIdentifier:[NSString stringWithUTF8String:object_getClassName([LSCircleListTableCell class])]];
        WS(weakSelf)
        _mainTableView.mj_header = [MJRefreshNormalHeader headerWithRefreshingBlock:^{
            [weakSelf.viewModel.refreshDataCommand execute:nil];
        }];
        _mainTableView.mj_footer = [MJRefreshAutoNormalFooter footerWithRefreshingBlock:^{
            [weakSelf.viewModel.nextPageCommand execute:nil];
        }];
    }
    return _mainTableView;
}
- (LSCircleListHeaderView *)listHeaderView {
    if (!_listHeaderView) {
        _listHeaderView = [[LSCircleListHeaderView alloc] initWithViewModel:self.viewModel.listHeaderViewModel];
        _listHeaderView.frame = CGRectMake(0, 0, SCREEN_WIDTH, 160);
    }
    return _listHeaderView;
}
- (LSCircleListSectionHeaderView *)sectionHeaderView {
    if (!_sectionHeaderView) {
        _sectionHeaderView = [[LSCircleListSectionHeaderView alloc] initWithViewModel:self.viewModel.sectionHeaderViewModel];
    }
    return _sectionHeaderView;
}
#pragma mark - delegate
#pragma mark - UITableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.viewModel.dataArray.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    LSCircleListTableCell *cell = [tableView dequeueReusableCellWithIdentifier:[NSString stringWithUTF8String:object_getClassName([LSCircleListTableCell class])] forIndexPath:indexPath];
    if (self.viewModel.dataArray.count > indexPath.row) {
        cell.viewModel = self.viewModel.dataArray[indexPath.row];
    }
    return cell;
}
#pragma mark - UITableViewDelegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 100;
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (self.viewModel.dataArray.count > indexPath.row) {
        [self.viewModel.cellClickSubject sendNext:self.viewModel.dataArray[indexPath.row]];
    }
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    return self.sectionHeaderView;
}
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return 45;
}
@end
```
**主View分为四个模块：**
**ⅰ 第一个模块 : 系统函数**
![](http://upload-images.jianshu.io/upload_images/1686195-3c355545f5c2bded.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
每个View都会有对应的ViewModel，这样也更易复用，这里因为是主View，一般而言我都会使得VC和主V共用一个VM，这样对于跳转、数据共享等都有着极大的好处。
**ⅱ 第二个模块 : 私有函数**
![](http://upload-images.jianshu.io/upload_images/1686195-3d03377c21f50a84.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1686195-b091d5adf422ab6a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
具体作用途中已经标注，需要注意的是这些对于不同数据的处理，是我自己写的，逻辑上肯定没有那么缜密，仅供参考。
**ⅲ 第三个模块 : 懒加载**
![](http://upload-images.jianshu.io/upload_images/1686195-618f0549a7801cc9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里没啥好说的，就是用的MJRefresh这个第三方库做的刷新。不过，假如你细心的话肯定会发现下面那两个View都是用VM来配置初始化的，这个和主View的配置初始化的意义是一样的。
ⅳ 第四个模块 : 代理及数据源
![](http://upload-images.jianshu.io/upload_images/1686195-9cb36cb7628a742a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
其中使用的是自定义Cell，用ViewModel来配置，点击事件也是和之前的VC的跳转联系起来了，并将VM传过去。
### **3、LSCircleListModel的处理**
同样，先上代码
```
//
//  LSCircleListModel.h
//  ZhongShui
//
//  Created by 王隆帅 on 16/3/17.
//  Copyright © 2016年 王隆帅. All rights reserved.
//
#import <Foundation/Foundation.h>
@interface LSCircleListModel : NSObject
@property (nonatomic, copy) NSString *idStr;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *intro;
@property (nonatomic, copy) NSString *img;
@property (nonatomic, copy) NSString *memberCount;
@property (nonatomic, copy) NSString *topicCount;
@end
```
```
//
//  LSCircleListModel.m
//  ZhongShui
//
//  Created by 王隆帅 on 16/3/17.
//  Copyright © 2016年 王隆帅. All rights reserved.
//
#import "LSCircleListModel.h"
@implementation LSCircleListModel
+ (NSDictionary *)mj_replacedKeyFromPropertyName {
    return  @{
              @"idStr":@"id",
              @"title":@"title",
              @"intro":@"intro",
              @"img":@"img",
              @"memberCount":@"MemberCount",
              @"topicCount":@"TopicCount",
              };
}
@end
```
这个就不贴图介绍了，就是单纯的数据模型，使用了MJExtention这个数据模型转换框架。没有做任何其他的逻辑处理。
### **4、ViewModel的处理**
```
//
//  LSCircleListViewModel.h
//  ZhongShui
//
//  Created by 王隆帅 on 16/3/10.
//  Copyright © 2016年 王隆帅. All rights reserved.
//
#import "YCViewModel.h"
#import "LSCircleListHeaderViewModel.h"
#import "LSCircleListSectionHeaderViewModel.h"
@interface LSCircleListViewModel : YCViewModel
@property (nonatomic, strong) RACSubject *refreshEndSubject;
@property (nonatomic, strong) RACSubject *refreshUI;
@property (nonatomic, strong) RACCommand *refreshDataCommand;
@property (nonatomic, strong) RACCommand *nextPageCommand;
@property (nonatomic, strong) LSCircleListHeaderViewModel *listHeaderViewModel;
@property (nonatomic, strong) LSCircleListSectionHeaderViewModel *sectionHeaderViewModel;
@property (nonatomic, strong) NSArray *dataArray;
@property (nonatomic, strong) RACSubject *cellClickSubject;
@end
```
```
//
//  LSCircleListViewModel.m
//  ZhongShui
//
//  Created by 王隆帅 on 16/3/10.
//  Copyright © 2016年 王隆帅. All rights reserved.
//
#import "LSCircleListViewModel.h"
#import "LSCircleListCollectionCellViewModel.h"
#import "LSCircleListModel.h"
@interface LSCircleListViewModel ()
@property (nonatomic, assign) NSInteger currentPage;
@end
@implementation LSCircleListViewModel
- (void)yc_initialize {
    @weakify(self);
    [self.refreshDataCommand.executionSignals.switchToLatest subscribeNext:^(NSDictionary *dict) {
        @strongify(self);
        if (dict == nil) {
            [self.refreshEndSubject sendNext:@(LSRefreshError)];
            ShowErrorStatus(@"网络连接失败");
            return;
        }
        if ([dict[@"status"] integerValue] == 0) {
            self.listHeaderViewModel.dataArray = [[[([(NSDictionary *)dict[@"res"] arrayForKey:@"JoinCircles"]).rac_sequence map:^id(NSDictionary *dic) {
                LSCircleListModel *model = [LSCircleListModel mj_objectWithKeyValues:dic];
                LSCircleListCollectionCellViewModel *viewModel = [[LSCircleListCollectionCellViewModel alloc] init];
                viewModel.model = model;
                return viewModel;
            }] array] mutableCopy];
            self.dataArray = [[[([(NSDictionary *)dict[@"res"] arrayForKey:@"Circles"]).rac_sequence map:^id(NSDictionary *dic) {
                LSCircleListModel *model = [LSCircleListModel mj_objectWithKeyValues:dic];
                LSCircleListCollectionCellViewModel *viewModel = [[LSCircleListCollectionCellViewModel alloc] init];
                viewModel.model = model;
                return viewModel;
            }] array] mutableCopy];
            [self ls_setHeaderRefreshWithArray:dict[@"Circles"]];
            [self ls_dismiss];
        } else {
            [self.refreshEndSubject sendNext:@(LSRefreshError)];
            ShowMessage(dict[@"mes"]);
        }        
    }];
    [[[self.refreshDataCommand.executing skip:1] take:1] subscribeNext:^(id x) {
        @strongify(self);
        if ([x isEqualToNumber:@(YES)]) {
            [self ls_showWithStatus:@"正在加载"];
        }
    }];
    [self.nextPageCommand.executionSignals.switchToLatest subscribeNext:^(NSDictionary *dict) {
        @strongify(self);
        if (dict == nil) {
            [self.refreshEndSubject sendNext:@(LSRefreshError)];
            ShowErrorStatus(@"网络连接失败");
            return;
        }
        if ([dict[@"status"] integerValue] == 0) {
            NSMutableArray *recommandArray = [[NSMutableArray alloc] initWithArray:self.dataArray];
            for (NSDictionary *subDic in [(NSDictionary *)dict[@"res"] arrayForKey:@"Circles"]) {
                LSCircleListModel *model = [LSCircleListModel mj_objectWithKeyValues:subDic];
                LSCircleListCollectionCellViewModel *viewModel = [[LSCircleListCollectionCellViewModel alloc] init];
                viewModel.model = model;
                [recommandArray addObject:viewModel];
            }
            self.dataArray = recommandArray;
            [self ls_setFootRefreshWithArray:dict[@"Circles"]];
            [self ls_dismiss];
        } else {
            [self.refreshEndSubject sendNext:@(LSRefreshError)];
            ShowMessage(dict[@"mes"]);
        }
    }];
}
#pragma mark - private
- (NSMutableDictionary *)requestCircleListWithId:(NSString *)idStr currentPage:(NSString *)currentPage {
    idStr = IF_NULL_TO_STRING(idStr);
    currentPage = IF_NULL_TO_STRING(currentPage);
    NSMutableDictionary * dict = [@{@"MemberID": idStr, @"pageSize": LS_REQUEST_LIST_COUNT, @"pageIndex":currentPage} mutableCopy];
    return dict;
}
- (void)ls_setFootRefreshWithArray:(NSArray *)array {
    if (array.count < LS_REQUEST_LIST_NUM_COUNT) {
        [self.refreshEndSubject sendNext:@(LSFooterRefresh_HasNoMoreData)];
    } else {
        [self.refreshEndSubject sendNext:@(LSFooterRefresh_HasMoreData)];
    }
}
- (void)ls_setHeaderRefreshWithArray:(NSArray *)array {
    if (array.count < LS_REQUEST_LIST_NUM_COUNT) {
        [self.refreshEndSubject sendNext:@(LSHeaderRefresh_HasNoMoreData)];
    } else {
        [self.refreshEndSubject sendNext:@(LSHeaderRefresh_HasMoreData)];
    }
}
#pragma mark - lazyLoad
- (RACSubject *)refreshUI {
    if (!_refreshUI) {
        _refreshUI = [RACSubject subject];
    }
    return _refreshUI;
}
- (RACSubject *)refreshEndSubject {
    if (!_refreshEndSubject) {
        _refreshEndSubject = [RACSubject subject];
    }
    return _refreshEndSubject;
}
- (RACCommand *)refreshDataCommand {
    if (!_refreshDataCommand) {
        @weakify(self);
        _refreshDataCommand = [[RACCommand alloc] initWithSignalBlock:^RACSignal *(id input) {
            @strongify(self);
            return [RACSignal createSignal:^RACDisposable *(id<RACSubscriber> subscriber) {
                @strongify(self);
                self.currentPage = 1;
                [self.request POST:LS_URL_CIRCLE_MEMBER_LIST parameters:[self requestCircleListWithId:@"1" currentPage:[NSString stringWithFormat:@"%d",self.currentPage]] success:^(CMRequest *request, NSString *responseString) {
                    NSDictionary *dict = [responseString objectFromJSONString];
                    [subscriber sendNext:dict];
                    [subscriber sendCompleted];
                } failure:^(CMRequest *request, NSError *error) {
                    ShowErrorStatus(@"网络连接失败");
                    [subscriber sendCompleted];
                }];
                return nil;
            }];
        }];
    }
    return _refreshDataCommand;
}
- (RACCommand *)nextPageCommand {
    if (!_nextPageCommand) {
        @weakify(self);
        _nextPageCommand = [[RACCommand alloc] initWithSignalBlock:^RACSignal *(id input) {
            @strongify(self);
            return [RACSignal createSignal:^RACDisposable *(id<RACSubscriber> subscriber) {
                @strongify(self);
                self.currentPage ++;
                [self.request POST:LS_URL_CIRCLE_TOPIC_LIST parameters:nil success:^(CMRequest *request, NSString *responseString) {
                    NSDictionary *dict = [responseString objectFromJSONString];
                    [subscriber sendNext:dict];
                    [subscriber sendCompleted];
                } failure:^(CMRequest *request, NSError *error) {
                    @strongify(self);
                    self.currentPage --;
                    ShowErrorStatus(@"网络连接失败");
                    [subscriber sendCompleted];
                }];
                return nil;
            }];
        }];
    }
    return _nextPageCommand;
}
- (LSCircleListHeaderViewModel *)listHeaderViewModel {
    if (!_listHeaderViewModel) {
        _listHeaderViewModel = [[LSCircleListHeaderViewModel alloc] init];
        _listHeaderViewModel.title = @"已加入的圈子";
        _listHeaderViewModel.cellClickSubject = self.cellClickSubject;
    }
    return _listHeaderViewModel;
}
- (LSCircleListSectionHeaderViewModel *)sectionHeaderViewModel {
    if (!_sectionHeaderViewModel) {
        _sectionHeaderViewModel = [[LSCircleListSectionHeaderViewModel alloc] init];
        _sectionHeaderViewModel.title = @"推荐圈子";
    }
    return _sectionHeaderViewModel;
}
- (NSArray *)dataArray {
    if (!_dataArray) {
        _dataArray = [[NSArray alloc] init];
    }
    return _dataArray;
}
- (RACSubject *)cellClickSubject {
    if (!_cellClickSubject) {
        _cellClickSubject = [RACSubject subject];
    }
    return _cellClickSubject;
}
@end
```
**ViewModel也是分为三个模块，由于代码太多摘重要的讲**
**ⅰ 第一个模块 : 处理数据、逻辑模块**
![](http://upload-images.jianshu.io/upload_images/1686195-ac9cc33a580e93fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
处理数据这块，先用字典转为Model，在用Model配置ViewModel，ViewModel再去与UI及其逻辑对应。
**ⅱ 第二个模块 : 私有函数**
![](http://upload-images.jianshu.io/upload_images/1686195-f61c1c6329258047.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
对于请求参数字典，可以放在VM中，便于模块化移植，也可以放在公共API中便于管理，看个人选择了，没有绝对的好位置，只有更适合个人的位置。
另外两个函数就是处理下拉及上拉时有没有更多数据的私有函数。
**ⅲ 第三个模块 : 懒加载**
![](http://upload-images.jianshu.io/upload_images/1686195-53a1a63a84f8466e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
此数据请求用的是AFNetworking。
### **5、APPDelegate的代码简化**
一般而言，我们正式项目中会遇到很多需要启动项目时就加载的，所以很快APPDelegate就会越来越庞大，既然其他的代码都简化解耦了，这里也可以做一下处理。
**目录如下：**
![](http://upload-images.jianshu.io/upload_images/1686195-2abc3f69ae0aa8fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**简化后的AppDelegate如下：**
![](http://upload-images.jianshu.io/upload_images/1686195-1a27ef03663f6094.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**其他代码存放的位置如下：**
![](http://upload-images.jianshu.io/upload_images/1686195-03a5d7beb1063372.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**当类对象被引入项目时, runtime 会向每一个类对象发送 load 消息. load 方法还是非常的神奇的, 因为它会在每一个类甚至分类被引入时仅调用一次, 调用的顺序是父类优先于子类, 子类优先于分类. 而且 load 方法不会被类自动继承, 每一个类中的 load 方法都不需要像 viewDidLoad 方法一样调用父类的方法。**
这是利用了这个算是黑魔法的玩意，哈哈，就简化了APPDelegate！
## **四、后记**
~~当初本来想**干掉基类**来着，**想利用Category + Protocol并利用Runtime的Methode Swizzle 来给系统函数添加自己的私有函数**，当初VC已经搞定了，然而发现这样牵涉面太广，你对VC做了Category，UINavigationController 也会受到影响，假如你对View做了Category，其他继承View的也会有影响，而且当时交换方法都是在一个Category里管事，到第二个就覆盖了。。。不造为啥，因为知道这条路走不通就没继续搞下去了。。。~~
写到这里，大家应该都对我笔下的设计模式有了一些了解，因为里面涉及的东西确实太多，主要是这些玩意需要站在巨人的肩膀，遇到文中没有提到而且不懂得可以：
![](http://upload-images.jianshu.io/upload_images/1686195-805a1ff43232609c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
哈哈哈！别怪我...不是我不负责，因为你可以看看写到这里篇幅已经超出常人所能接受的了，而且我感觉我把各个细节已经都照顾到了吧（๑乛◡乛๑ 磨人的小妖精）！大家有什么疑惑我们可以在评论区交流！
**最后，真的很希望各位大神指出不足的地方，能让大家共同进步！**
**本文由简书作者 王隆帅 原创编写，转载请保留版权网址，感谢您的理解与分享，让生活变的更美好!（有点吓不住人，该这样说：如需转载请务必通知作者，否则法律责任后果自负！）**
文／王隆帅（简书作者）
原文链接：http://www.jianshu.com/p/3beb21d5def2
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
