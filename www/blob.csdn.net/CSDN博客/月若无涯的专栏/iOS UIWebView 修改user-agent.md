# iOS UIWebView 修改user-agent - 月若无涯的专栏 - CSDN博客
2015年04月27日 11:34:07[月若无涯](https://me.csdn.net/u010124617)阅读数：690标签：[iOS WebView 修改Agent](https://so.csdn.net/so/search/s.do?q=iOS WebView 修改Agent&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)

WebView 没有提供设置user-agent 的接口，无论是设置要加载的request，还是在delegate 中设置request，经测试都是无效的。如下：
方案一：
**[objc]**[view
 plain](http://blog.csdn.net/fangzhangsc2006/article/details/25840709#)[copy](http://blog.csdn.net/fangzhangsc2006/article/details/25840709#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/346025/fork)
- NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:url];  
- [request addValue:@"Jiecao/2.4.7" forHTTPHeaderField:@"user-agent"];  
- [self.webView loadRequest:request];  
无效！！！
方案二：
**[objc]**[view
 plain](http://blog.csdn.net/fangzhangsc2006/article/details/25840709#)[copy](http://blog.csdn.net/fangzhangsc2006/article/details/25840709#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/346025/fork)
- #pragma mark - webview delegate
- 
- - (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {  
- NSMutableURLRequest *req = (NSMutableURLRequest *)request;  
-     [req addValue:@"Jiecao/2.4.7" forHTTPHeaderField:@"user-agent"];  
- }  
**无效！！！**
我的需求是不光要能更改user-agent，而且要保留WebView 原来的user-agent 信息，也就是说我需要在其上追加信息。在stackOverflow上搜集了多方答案，最终汇总的解决方案如下：
在启动时，比如在AppDelegate 中添加如下代码：
**[objc]**[view
 plain](http://blog.csdn.net/fangzhangsc2006/article/details/25840709#)[copy](http://blog.csdn.net/fangzhangsc2006/article/details/25840709#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/346025/fork)
- //get the original user-agent of webview
- UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectZero];  
- NSString *oldAgent = [webView stringByEvaluatingJavaScriptFromString:@"navigator.userAgent"];  
- NSLog(@"old agent :%@", oldAgent);  
- 
- //add my info to the new agent
- NSString *newAgent = [oldAgent stringByAppendingString:@" Jiecao/2.4.7 ch_appstore"];  
- NSLog(@"new agent :%@", newAgent);  
- 
- //regist the new agent
- NSDictionary *dictionnary = [[NSDictionary alloc] initWithObjectsAndKeys:newAgent, @"UserAgent", nil nil];  
- [[NSUserDefaults standardUserDefaults] registerDefaults:dictionnary];  
这样，WebView在请求时的user-agent 就是我们设置的这个了，如果需要在WebView 使用过程中再次变更user-agent，则需要再通过这种方式修改user-agent， 然后再重新实例化一个WebView。
参考：
http://stackoverflow.com/questions/478387/change-user-agent-in-uiwebview-iphone-sdk/23654363#23654363
