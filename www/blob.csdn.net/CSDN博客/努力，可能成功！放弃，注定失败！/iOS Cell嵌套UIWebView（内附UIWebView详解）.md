# iOS Cell嵌套UIWebView（内附UIWebView详解） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月25日 10:38:57[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5120
## **前言**
> 
**背景：**最近做的项目中有这样一个需求，一个话题详情界面内部分内容为html标签，其他为普通内容，然后html标签是嵌套在Cell中的，一开始用的是UILabel加载html标签，结果发现对于图片标签没有更好的适应屏幕，果断换成UIWebView，使用WebView加载计算高度的时候是有些注意点的，所以在此记录一下，并总结一下相关知识，以备后续查阅。
## **一、cell嵌套webView具体代码及讲解**
### **1、自定义Cell并初始化数据**
```
```
if (indexPath.section == 0) {
        LSTopicDetailMainCell *cell = [tableView dequeueReusableCellWithIdentifier:[NSString stringWithUTF8String:object_getClassName([LSTopicDetailMainCell class])] forIndexPath:indexPath];
        cell.viewModel = self.viewModel.topMainCellViewModel;
        return cell;
    }
```
```
使用viewModel装配自定义Cell，注意其identifier已经注册，如下
`` [_mainTableView registerClass:[LSTopicDetailMainCell class] forCellReuseIdentifier:[NSString stringWithUTF8String:object_getClassName([LSTopicDetailMainCell class])]];``
### **2、返回高度**
比较麻烦的是高度的计算，因为我对于Cell自动布局高度的计算是用的 `UITableView+FDTemplateLayoutCell` 这个第三方，核心是提前计算高度及缓存，对UILabel加载Html标签来说很OK，但是对于webView来讲就有些问题，因为他的**高度需要在加载完的回调中去获取并刷新**，所以需要手动计算。
```
if (indexPath.section == 0) {
  return self.viewModel.topMainCellViewModel.cellHeight;
 }
```
### **3、自定义Cell内相关代码**
- **加载Html标签**
`[self.contentWebView loadHTMLString:viewModel.content baseURL:nil];`- **UIWebView回调处理（核心代码）**
```
- (void)webViewDidFinishLoad:(UIWebView *)webView {
    // 获取内容高度
    CGFloat height =  [[webView stringByEvaluatingJavaScriptFromString:@"document.documentElement.scrollHeight"] intValue];
    // 防止死循环
    if (height != _viewModel.htmlHeight) {
        _viewModel.htmlHeight = height;
        if (_viewModel.htmlHeight > 0) {
            // 更新布局
            CGFloat paddingEdge = 10;
            WS(weakSelf)
            [self.contentWebView mas_remakeConstraints:^(MASConstraintMaker *make) {
                make.left.equalTo(paddingEdge);
                make.right.equalTo(-paddingEdge);
                make.top.equalTo(weakSelf.headerImageView.mas_bottom).offset(paddingEdge);
                make.bottom.equalTo(-paddingEdge);
            }];
            // 刷新cell高度
            _viewModel.cellHeight = _viewModel.otherHeight + _viewModel.htmlHeight;
            [_viewModel.refreshSubject sendNext:nil];
        }
    }
}
```
上面的代码注释已经很清楚了，需要解释的是防止死循环的意思是你**刷新cell的代码在回调里，当你刷新的时候，他也会走回调，不判断处理的话会造成死循环。**
**4、刷新Cell**
```
[self.viewModel.topMainCellViewModel.refreshSubject subscribeNext:^(id x) {
        @strongify(self);
        [self.mainTableView reloadSection:0 withRowAnimation:UITableViewRowAnimationNone];
    }];
```
**5、完事**
## **二、Cell嵌套UILabel加载Html标签处理**
> 
假如你只是需要处理文字相关的Html标签的话，使用Label加载是最好的选择
**1、高度处理（核心代码）**
```
```
if (indexPath.section == 0) {
        return [tableView fd_heightForCellWithIdentifier:[NSString stringWithUTF8String:object_getClassName([LSTopicDetailMainCell class])] cacheByIndexPath:indexPath configuration:^(LSTopicDetailMainCell *cell) {
            @strongify(self);
            cell.viewModel = self.viewModel.topMainCellViewModel;
        }];
    }
```
```
**2、Masonry布局**
```
[self.contentLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(paddingEdge);
        make.right.equalTo(-paddingEdge);
        make.top.equalTo(weakSelf.headerImageView.mas_bottom).offset(paddingEdge);
        make.bottom.equalTo(-paddingEdge);
    }];
```
**3、Label加载Html**
```
NSAttributedString *content = [[NSAttributedString alloc] initWithData:[viewModel.content dataUsingEncoding:NSUnicodeStringEncoding] options:@{ NSDocumentTypeDocumentAttribute: NSHTMLTextDocumentType } documentAttributes:nil error:nil];
    self.contentLabel.attributedText = content;
```
## **三、UIWebView详解**
### **1、三种加载方式**
- **加载URL内容**
```
NSURL *url = [NSURL URLWithString:@"http://www.jianshu.com/users/1a9cd48c3cf0/latest_articles"]; 
[self.webview loadRequest:[NSURLRequest requestWithURL:url]];
```
- **加载本地的HTML文件**
```
NSString *htmlPath = [[[NSBundle mainBundle] bundlePath]
    stringByAppendingPathComponent:@"wanglongshuai.html"];
[self.webview loadRequest:[NSURLRequest requestWithURL:
    [NSURL fileURLWithPath:htmlPath]]];
```
- **加载html字符串**
```
NSString *htmlPath = [[[NSBundle mainBundle] bundlePath]
    stringByAppendingPathComponent:@"wanglongshuai.html"];
NSString *htmlString = [NSString stringWithContentsOfFile: htmlPath
                                    encoding:NSUTF8StringEncoding 
                                    error:NULL];
[self.webview loadHTMLString:htmlString baseURL:[NSURL
    fileURLWithPath:htmlPath]];
```
### **2、UIWebViewDelegate代理方法**
```
//准备加载内容: 通过返回值来进行是否加载的设置
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;
//开始加载
- (void)webViewDidStartLoad:(UIWebView *)webView;
//加载完成
- (void)webViewDidFinishLoad:(UIWebView *)webView;
//加载失败
- (void)webView:(UIWebView *)webView didFailLoadWithError:(nullable NSError *)error;
```
### **3、常用属性及API**
- **基本属性及API**
```
**webView的代理**
@property (nullable, nonatomic, assign) id <UIWebViewDelegate> delegate;
**内置的scrollView**
@property (nonatomic, readonly, strong) UIScrollView *scrollView NS_AVAILABLE_IOS(5_0);
**URL请求**
@property (nullable, nonatomic, readonly, strong) NSURLRequest *request;
**是否缩放到适合屏幕大小**
@property (nonatomic) BOOL scalesPageToFit;
**执行javaScript操作**
- (nullable NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script;
```
- **加载相关属性及API**
```
- (void)reload;  //重新加载数据
- (void)stopLoading;  //停止加载数据
@property (nonatomic, readonly, getter=isLoading) BOOL loading; //是否正在加载
- (void)goBack;    //返回上一级
- (void)goForward;  //跳转下一级
@property (nonatomic, readonly, getter=canGoBack) BOOL canGoBack; //能否返回上一级
@property (nonatomic, readonly, getter=canGoForward) BOOL canGoForward; //能否跳转下一级
```
- **视频相关属性**
```
//YES，自动检测网页上的电话号码，单击可以拨打  
@property (nonatomic) BOOL detectsPhoneNumbers NS_DEPRECATED_IOS(2_0, 3_0);
//设置某些数据变为链接形式，这个枚举可以设置如电话号，地址，邮箱等转化为链接
@property (nonatomic) UIDataDetectorTypes dataDetectorTypes NS_AVAILABLE_IOS(3_0);
//设置是否使用内联播放器播放视频
@property (nonatomic) BOOL allowsInlineMediaPlayback NS_AVAILABLE_IOS(4_0); // iPhone Safari defaults to NO. iPad Safari defaults to YES
//设置视频是否自动播放
@property (nonatomic) BOOL mediaPlaybackRequiresUserAction NS_AVAILABLE_IOS(4_0); // iPhone and iPad Safari both default to YES
//设置音频播放是否支持ari play功能
@property (nonatomic) BOOL mediaPlaybackAllowsAirPlay NS_AVAILABLE_IOS(5_0); // iPhone and iPad Safari both default to YES
//设置是否将数据加载如内存后渲染界面
@property (nonatomic) BOOL suppressesIncrementalRendering NS_AVAILABLE_IOS(6_0); // iPhone and iPad Safari both default to NO
//设置用户交互模式
@property (nonatomic) BOOL keyboardDisplayRequiresUserAction NS_AVAILABLE_IOS(6_0); // default is YES
```
文／王隆帅（简书作者）
原文链接：http://www.jianshu.com/p/abd95c1a9f75
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
