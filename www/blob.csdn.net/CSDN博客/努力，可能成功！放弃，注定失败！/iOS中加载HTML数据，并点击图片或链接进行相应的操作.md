# iOS中加载HTML数据，并点击图片或链接进行相应的操作 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月09日 08:54:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1274
开发中，有时候我们需要展现HTML类型的数据，这时候，我们多数选择用UIWebView加载，但是当我们需要加载大量的HTML数据时，使用UIWebView加载，性能会降低，尤其是当UITableViewCell需要放大量的HTML数据时，如果你用UIWebView加载这些HTML数据，你会发现，tableview会有卡顿的现象。
那怎样去优化这个卡顿现象呢？我们知道WKWbebView的加载速度比UIWebView提升差不多一倍的, 内存使用上面,反而还少了一半，所以我们可以采用WKWebView对其进行优化。今天把一个三方类介绍给大家，那就是[IMYWebView](https://github.com/li6185377/IMYWebView),这个类兼容WKWebView和UIWebView，实现了UIWebView到WKWebView的对接，即使你之前的项目是用的UIWebView,那也没关系，只要将UIWebView换成IMYWebView就可以了。
另外，加载HTML数据自然绝多数会有图片和内置链接，我们也会希望可以点击图片进行浏览、放大和缩小，可以点开链接，那么，该怎样去实现呢？
1、在工程中导入IMYWebView
`#import "IMYWebView.h"`
2、创建IMYWebView,并嵌套到TableView中，实现tableview的相关代理方法
```
@interface ViewController ()<UITableViewDataSource, UITableViewDelegate,IMYWebViewDelegate,HZPhotoBrowserDelegate>
@property(nonatomic, strong)UITableView *tableView;
@property(nonatomic, assign)CGFloat webviewHight;//记录webview的高度
@property(nonatomic, copy)NSString *HTMLData;//需要加载的HTML数据
@property(nonatomic, strong)NSMutableArray *imageArray;//HTML中的图片个数
@property(nonatomic, strong)IMYWebView *IMYwebView;
@end
```
```
- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.navigationBar.translucent = NO;
    self.title = @"tableviewcell嵌套webview载HTML";
    self.view.backgroundColor = [UIColor whiteColor];
//    获取HTML数据
     [self getHTMLData];
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width , self.view.frame.size.height - 64) style:UITableViewStylePlain];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.tableHeaderView = [[UIView alloc] init];
    [self.view addSubview:_tableView];
    _htmlWebView = [[IMYWebView alloc] init];
    _htmlWebView.frame = CGRectMake(0, 0, _tableView.frame.size.width, 1);
    _htmlWebView.delegate = self;
    _htmlWebView.scrollView.scrollEnabled = NO;//设置webview不可滚动，让tableview本身滚动即可
    _htmlWebView.scrollView.bounces = NO;
    _htmlWebView.opaque = NO;
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return 4;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row != 3) {
         return 60;
    }else{
        return _webviewHight;//cell自适应webview的高度
    }
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *identifier = @"cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    if (!cell) {
            cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifier];
                }
    if (indexPath.row == 3) {
        [cell.contentView addSubview:_htmlWebView];
        //加载HTML数据
        [_htmlWebView loadHTMLString:_HTMLData baseURL:nil];
    }else{
        cell.textLabel.text = [NSString stringWithFormat:@"第%ld行",(long)indexPath.row];
    }
     return cell;
}
```
3、实现IMYWebView的相关代理方法
```
-(void)webViewDidFinishLoad:(IMYWebView *)webView{
//加载完毕后重新计算webview的高度
    [self.htmlWebView evaluateJavaScript:@"document.documentElement.scrollHeight" completionHandler:^(id object, NSError *error) {
        CGFloat height = [object integerValue];
        if (error != nil) {
        }else{
            _webviewHight = height;
            [_tableView beginUpdates];
            self.htmlWebView.frame = CGRectMake(_htmlWebView.frame.origin.x,_htmlWebView.frame.origin.y, _tableView.frame.size.width, _webviewHight );
              [_tableView endUpdates];
        }
    }];
//    插入js代码，对图片进行点击操作
    [webView evaluateJavaScript:@"function assignImageClickAction(){var imgs=document.getElementsByTagName('img');var length=imgs.length;for(var i=0; i < length;i++){img=imgs[i];if(\\"ad\\" ==img.getAttribute(\\"flag\\")){var parent = this.parentNode;if(parent.nodeName.toLowerCase() != \\"a\\")return;}img.onclick=function(){window.location.href='image-preview:'+this.src}}}" completionHandler:^(id object, NSError *error) {
    }];
    [webView evaluateJavaScript:@"assignImageClickAction();" completionHandler:^(id object, NSError *error) {
    }];
    //获取HTML中的图片
    [self getImgs];
}
-(BOOL)webView:(IMYWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType{
    if ([request.URL isEqual:@"about:blank"])
    {
        return true;
    }
    if ([request.URL.scheme isEqualToString: @"image-preview"])
    {
        NSString *url = [request.URL.absoluteString substringFromIndex:14];
        //启动图片浏览器， 跳转到图片浏览页面
        if (_imageArray.count != 0) {
            HZPhotoBrowser *browserVc = [[HZPhotoBrowser alloc] init];
            browserVc.imageCount = self.imageArray.count; // 图片总数
            browserVc.currentImageIndex = [_imageArray indexOfObject:url];//当前点击的图片
            browserVc.delegate = self;
            [browserVc show];
        }
        return NO;
    }
    //    用户点击文章详情中的链接
    if ( navigationType == UIWebViewNavigationTypeLinkClicked ) {
            WebViewURLViewController *webViewVC = [WebViewURLViewController new];
            webViewVC.URLString = request.URL.absoluteString;
            [self.navigationController pushViewController:webViewVC animated:YES];
        return NO;
    }
        return YES;
}
```
4、获取文章图片个数的方法
```
#pragma mark -- 获取文章中的图片个数
- (NSArray *)getImgs
{
    NSMutableArray *arrImgURL = [[NSMutableArray alloc] init];
    for (int i = 0; i < [self nodeCountOfTag:@"img"]; i++) {
        NSString *jsString = [NSString stringWithFormat:@"document.getElementsByTagName('img')[%d].src", i];
        [_htmlWebView evaluateJavaScript:jsString completionHandler:^(NSString *str, NSError *error) {
            if (error ==nil) {
                [arrImgURL addObject:str];
            }
        }];
    }
    _imageArray = [NSMutableArray arrayWithArray:arrImgURL];
    return arrImgURL;
}
// 获取某个标签的结点个数
- (NSInteger)nodeCountOfTag:(NSString *)tag
{
    NSString *jsString = [NSString stringWithFormat:@"document.getElementsByTagName('%@').length", tag];
   int count =  [[_htmlWebView stringByEvaluatingJavaScriptFromString:jsString] intValue];
    return count;
}
```
5、点击图片进行浏览的相关方法，这个图片浏览是封装好的一个浏览器，在这里暂时不做介绍
```
#pragma mark - photobrowser代理方法
- (UIImage *)photoBrowser:(HZPhotoBrowser *)browser placeholderImageForIndex:(NSInteger)index
{
    //图片浏览时，未加载出图片的占位图
    return [UIImage imageNamed:@"gg_pic@2x"];
}
- (NSURL *)photoBrowser:(HZPhotoBrowser *)browser highQualityImageURLForIndex:(NSInteger)index
{
    NSString *urlStr = [self.imageArray[index] stringByReplacingOccurrencesOfString:@"thumbnail" withString:@"bmiddle"];
```
6、最后的运行结果如图：
![](http://upload-images.jianshu.io/upload_images/972702-5e6b06aa023cf041.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
结果示意图
7.有时候我们需要给webview加个头视图或者尾视图，跟随webview一起滚动，如果这时我们的webview不是嵌在tableview中的，那该怎么办呢？很简单，只要将头视图加在webview的scrollview上，设置scrollview的contentInset(UIEdgeInsetsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right)头视图的高度为top，尾视图的高度为bottom,根据自己的需要去设置相应的高度)就可以了
```cpp
```cpp
//给scrollview添加头视图
    //设置web view的scrollView的上、左、下、右的距离
    _wkwebView.scrollView.contentInset = UIEdgeInsetsMake(40, 0, 0, 0);
    _titleLabel = [[UILabel alloc] init];
    [_wkwebView.scrollView addSubview:_titleLabel];
    _titleLabel.frame = CGRectMake(0, -40, self.wkwebView.frame.size.width, 40);
    [self.wkwebView.scrollView addSubview:_titleLabel];
  _titleLabel.text = @"web view的头视图";
```
```
8、最后，给大家分享一些用js获取网页一些相关元素的代码
```java
```java
document：属性
document.title                 //设置文档标题等价于HTML的<title>标签
document.bgColor               //设置页面背景色
document.fgColor               //设置前景色(文本颜色)
document.linkColor             //未点击过的链接颜色
document.alinkColor            //激活链接(焦点在此链接上)的颜色
document.vlinkColor            //已点击过的链接颜色
document.URL                   //设置URL属性从而在同一窗口打开另一网页
document.fileCreatedDate       //文件建立日期，只读属性
document.fileModifiedDate      //文件修改日期，只读属性
document.fileSize              //文件大小，只读属性
document.cookie                //设置和读出cookie
document.charset               //设置字符集 简体中文:gb2312
document：方法
document.write()                      //动态向页面写入内容
document_createElement_x_x(Tag)           //创建一个html标签对象
document.getElementByIdx_x_x(ID)           //获得指定ID值的对象
document.getElementsByName(Name)      //获得指定Name值的对象
document.body.a(oTag)
body：子对象
document.body                   //指定文档主体的开始和结束等价于<body></body>
document.body.bgColor           //设置或获取对象后面的背景颜色
document.body.link              //未点击过的链接颜色
document.body.alink             //激活链接(焦点在此链接上)的颜色
document.body.vlink             //已点击过的链接颜色
document.body.text              //文本色
document.body.innerText         //设置<body>...</body>之间的文本
document.body.innerHTML         //设置<body>...</body>之间的HTML代码
document.body.topMargin         //页面上边距
document.body.leftMargin        //页面左边距
document.body.rightMargin       //页面右边距
document.body.bottomMargin      //页面下边距
document.body.background        //背景图片
document.body.a(oTag) //动态生成一个HTML对象
location：子对象
document.location.hash          // #号后的部分
document.location.host          // 域名+端口号
document.location.hostname      // 域名
document.location.href          // 完整URL
document.location.pathname      // 目录部分
document.location.port          // 端口号
document.location.protocol      // 网络协议(http:)
document.location.search        // ?号后的部分
常用对象事件:
documeny.location.reload()          //刷新网页
document.location.reload(URL)       //打开新的网页
document.location.assign(URL)       //打开新的网页
document.location.replace(URL)      //打开新的网页
selection-选区子对象
document.selection
```
```
文／若锦（简书作者）
原文链接：http://www.jianshu.com/p/149517ed8535
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
