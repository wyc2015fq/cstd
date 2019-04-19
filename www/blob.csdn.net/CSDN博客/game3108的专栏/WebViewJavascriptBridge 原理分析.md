# WebViewJavascriptBridge 原理分析 - game3108的专栏 - CSDN博客
2015年05月15日 11:37:46[game3108](https://me.csdn.net/game3108)阅读数：596
个人分类：[转载																[iOS](https://blog.csdn.net/game3108/article/category/2844789)](https://blog.csdn.net/game3108/article/category/2926393)
最近项目中一直在做js与native交互的东西。对于WebViewJavascriptBridge很感兴趣，所以自己看了看源代码并且网上找了些分析学习一下。
原地址：http://www.2cto.com/kf/201503/384998.html
网上好多都是在介绍 WebView[Java](http://www.2cto.com/kf/ware/Java/)scriptBridge如何使用，这篇文章就来说说 WebViewJavascriptBridge 设计原理。
主要从两个过程来讲一下：js调用UIViewController中的代码（Native），Native调用js
### 1.概述
![\](http://www.2cto.com/uploadfile/Collfiles/20150325/20150325084129294.png)
首先有两个问题：
a.Native（中的UIWebView）是否可以直接调用js method（方法）？ 可以。
b.js 是否可以直接调用Native的mthod？不行。
明确上述两个问题，那么上图就不难明白了，webpage中的js method和webview本地的method之间关系。那WebViewJavascriptBridge出现是否解决这个问题（这个问题就是让js可以直接调用native的method）呢？答案是否定的？没有本质还是用uiwebview的代理方法进行字段拦截（判断url的scheme），实现js间接调用native的method。
我们来看WebViewJavascriptBridge提供的demo：
![\](http://www.2cto.com/uploadfile/Collfiles/20150325/20150325084129295.png)
主要的核心是下面两个，接下来我们就来讨论一下其设计原理。
### 2.js调用Native method
在概述中说过，js是不能直接调用native的method所以，需要借助- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType，这个方法大家不陌生，每次在重新定向URL的时候，这个方法就会被触发，通常情况，我们会在这里做一些拦截完成js和本地的间接交互什么的。那么WebViewJavascriptBridge也不另外，也是这么做。
我们先来看看在ExampleApp.[html](http://www.2cto.com/kf/qianduan/css/)文件中点击一个按钮发起请求的代码：
```java
var callbackButton = document.getElementById('buttons').appendChild(document.createElement('button'))
		callbackButton.innerHTML = 'Fire testObjcCallback'
		callbackButton.onclick = function(e) {
			e.preventDefault()
			log('JS calling handler "testObjcCallback"')
            //1
			bridge.callHandler('testObjcCallback', {'foo': 'cccccccccccc'}, function(response) {
				log('JS got response', response)
			})
		}
```
估计大家大体都能看懂，唯独有疑问的地方是：
```java
bridge.callHandler('testObjcCallback', {'foo': 'cccccccccccc'}, function(response) {
				log('JS got response', response)
			})
		}
```
这段代码先不说，上面代码就是一个按钮的普通单击事件方法。我们一起想一下，如果这个按钮需要被点击之后调用native中的funtion函数，之后需要把这个（native的）funtion函数处理结果返回给js中的方法继续处理。这个是我们需求，带着这个需求我们看一下这个方法，testObjcCallBack这个我们猜测一下应该native中的方法或者一个能够调用到方法的name/id，后面这个是个json｛‘foo’：‘ccccccccccccc’｝,应该是个参数，那么后面这个方法一看log应该知道，是对native返回的result进行处理的方法。拿具体是不是呢？只要找到callHandler方法就知道了。
在文件WebViewJavascriptBridge.js.txt里面我们找找这个方法：
```java
function callHandler(handlerName, data, responseCallback) {
		_doSend({ handlerName:handlerName, data:data }, responseCallback)
	}
```
这里又多了一个方法叫_doSend连个参数 第1个是字典key-value定义，第二个是一个方法的指针（看看上面的方法你就知道了），那我们必须在同一个文件里面看看能不能找到这个_doSend方法：
```java
function _doSend(message, responseCallback) {
		if (responseCallback) {
			var callbackId = 'cb_'+(uniqueId++)+'_'+new Date().getTime()
			responseCallbacks[callbackId] = responseCallback
			message['callbackId'] = callbackId
		}
		sendMessageQueue.push(message)
		messagingIframe.src = CUSTOM_PROTOCOL_SCHEME + '://' + QUEUE_HAS_MESSAGE
	}
```
找到了。
逐行分析一下，变量callbackId是个字符串，responseCallBacks[] 一看就知道是个字典 ，这个字典把回掉（我们猜测）的方法responseCallback给保存起来，这Key（也就是callbackId）应该是唯一的，通过计数和时间应该知道这个字符串应该是唯一的，message也是一个字典，这是给message添加了一个新的key-value。干嘛呢？我也不知道，我们来看看sendMessageQueue是什么，大家一个push就知道应该是个数组。他吧一个字典放到一个消息队列中（数组队列），让后产生一个src（url
 scheme）。
有两个变量我们看看：
```java
var CUSTOM_PROTOCOL_SCHEME = 'wvjbscheme'
	var QUEUE_HAS_MESSAGE = '__WVJB_QUEUE_MESSAGE__'
```
干嘛用，肯定是给webview 的 delegate判断用的，你感觉呢？（肯定是）
下面是在文件：WebViewJavascriptBridge.m
好了到了这里大家猜猜这个要干嘛？肯定是要发url让web截取对吧？那还用问啊，肯定是啊，已经说过了js能不能调用native的funtion函数？不能。我们来看看这个messagingIframe是：
```java
function _createQueueReadyIframe(doc) {
		messagingIframe = doc.createElement('iframe')
		messagingIframe.style.display = 'none'
		messagingIframe.src = CUSTOM_PROTOCOL_SCHEME + '://' + QUEUE_HAS_MESSAGE
		doc.documentElement.appendChild(messagingIframe)
	}
```
原来就是iframe，这个就不同给大家解释了。好了src一产生就会出现什么，uiwebview代理回掉截获，此时我们把目光回到UIWebview的Native下面:
```java
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    if (webView != _webView) { return YES; }
    NSURL *url = [request URL];
    __strong WVJB_WEBVIEW_DELEGATE_TYPE* strongDelegate = _webViewDelegate;
    
    if ([[url scheme] isEqualToString:kCustomProtocolScheme])
    {
        if ([[url host] isEqualToString:kQueueHasMessage])
        {
            //会走这里
            [self _flushMessageQueue];
        }
        else
        {
            NSLog(@"WebViewJavascriptBridge: WARNING: Received unknown WebViewJavascriptBridge command %@://%@", kCustomProtocolScheme, [url path]);
        }
        return NO;
    }
    else if (strongDelegate && [strongDelegate respondsToSelector:@selector(webView:shouldStartLoadWithRequest:navigationType:)])
    {
        return [strongDelegate webView:webView shouldStartLoadWithRequest:request navigationType:navigationType];
    }
    else
    {
        return YES;
    }
}
```
一看就头大，哈哈，是，我也头大。看看上面的注释说 会走这里，我们看看为什么会走那里，最外圈的if（[url scheme]）判断是
#define kCustomProtocolScheme @"wvjbscheme"
这个定义是什么意思，我们先不做解释，刚才我们说过js不能直接调用native的function，大家只要记住这点，接着往下走就是了。至于为什么走这里，自己看代码（上文有提到），我们看看_flushMessageQueue:
```java
- (void)_flushMessageQueue {
    NSString *messageQueueString = [_webView stringByEvaluating[JavaScript](http://www.2cto.com/kf/qianduan/JS/)FromString:@"WebViewJavascriptBridge._fetchQueue();"];
    //json转成数组
    id messages = [self _deserializeMessageJSON:messageQueueString];
    if (![messages isKindOfClass:[NSArray class]]) {
        NSLog(@"WebViewJavascriptBridge: WARNING: Invalid %@ received: %@", [messages class], messages);
        return;
    }
    for (WVJBMessage* message in messages) {
        if (![message isKindOfClass:[WVJBMessage class]]) {
            NSLog(@"WebViewJavascriptBridge: WARNING: Invalid %@ received: %@", [message class], message);
            continue;
        }
        [self _log:@"RCVD" json:message];
        //用于js回掉
        NSString* responseId = message[@"responseId"];
        if (responseId) {
            WVJBResponseCallback responseCallback = _responseCallbacks[responseId];
            responseCallback(message[@"responseData"]);
            [_responseCallbacks removeObjectForKey:responseId];
        } else {
            WVJBResponseCallback responseCallback = NULL;
            NSString* callbackId = message[@"callbackId"];
            if (callbackId) {
                responseCallback = ^(id responseData) {
                    if (responseData == nil) {
                        responseData = [NSNull null];
                    }
                    
                    WVJBMessage* msg = @{ @"responseId":callbackId, @"responseData":responseData };
                    [self _queueMessage:msg];
                };
            } else {
                responseCallback = ^(id ignoreResponseData) {
                    // Do nothing
                };
            }
            
            WVJBHandler handler;
            if (message[@"handlerName"]) {
                handler = _messageHandlers[message[@"handlerName"]];
            } else {
                handler = _messageHandler;
            }
            if (!handler) {
                [NSException raise:@"WVJBNoHandlerException" format:@"No handler for message from JS: %@", message];
            }
            
            handler(message[@"data"], responseCallback);
        }
    }
}
```
这下牛逼了，不忍直视啊！这么多，哈哈，多不可怕，可怕是你坚持不下去了。
我们逐行来看：
NSString *messageQueueString = [_webView stringByEvaluatingJavaScriptFromString:@"WebViewJavascriptBridge._fetchQueue();"];
我们必须回去到js文件中去，这里是webview直接调用js中的方法：
```java
function _fetchQueue() {
		var messageQueueString = JSON.stringify(sendMessageQueue)
		sendMessageQueue = []
		return messageQueueString
	}
```
谢天谢地这个方法代码不多，这个消息很眼熟，SendMessageQueue，刚才我们说什么来？他是一个字典，那里面有哪些东西，我么来看看
handlerName:handlerName,
data:data,
callbackId:callbackId
这个消息字典此时被取出来准备做什么，这里提示下我们已经走到webview 的delegate里面了，所以拿到这些信息肯定是调用native的method对吧？肯定是的。接着往下走，接着会把json字符串转成数组，然后进行判断，
```java
NSString* responseId = message[@"responseId"];
```
有没有responseid，你说又没，肯定没有啊（你不行看看上面），所以就这这里了
```java
WVJBResponseCallback responseCallback = NULL;
            NSString* callbackId = message[@"callbackId"];
            if (callbackId) {
                responseCallback = ^(id responseData) {
                    if (responseData == nil) {
                        responseData = [NSNull null];
                    }
                    
                    WVJBMessage* msg = @{ @"responseId":callbackId, @"responseData":responseData };
                    [self _queueMessage:msg];
                };
            } else {
                responseCallback = ^(id ignoreResponseData) {
                    // Do nothing
                };
            }
            
            WVJBHandler handler;
            if (message[@"handlerName"]) {
                handler = _messageHandlers[message[@"handlerName"]];
            } else {
                handler = _messageHandler;
            }
            if (!handler) {
                [NSException raise:@"WVJBNoHandlerException" format:@"No handler for message from JS: %@", message];
            }
            
            handler(message[@"data"], responseCallback);
```
这部分是重点，到底他是怎么要调用本地function的，callbackId大家熟悉吧，判断是否为空，不为空给他指定一个block，这个不说了，block指定，此时不调用（手动调用才会执行），这个刚才说了用来处理native的function处理的result用于把处理后的值返回给js的，接着往下去，看到handler这个方法会从message找到handlerName，这里我们看一下多了一个_messageHandlers字典，从这个字典获取一个block（WVJBHandler是一个block），直接执行了。那我们看看_messageHandlers是怎么被添加block的：
```java
- (void)registerHandler:(NSString *)handlerName handler:(WVJBHandler)handler {
    _messageHandlers[handlerName] = [handler copy];
}
```
那又是谁调用了这个方法：
找到了(在文件 ExampleAppViewController.m的viewdidload中)，这里有方法testObjecCallback
```java
[_bridge registerHandler:@"testObjcCallback" handler:^(id data, WVJBResponseCallback responseCallback) {
        NSLog(@"testObjcCallback called: %@", data);
        responseCallback(@"Response from testObjcCallback");
    }];
```
有点乱了。刚才我们的思路都是倒推的，如果我们整过来，首先肯定是viewdidload初始化，初始化之后会把这个block加入到_messageHandlers中，之后因为js调用动态读取这个block调用，在调用之前，我们又把定一个block付给回掉处理的responseCallback的block，这个block在handler中调用而调用，有点绕，自己可以多想想。
我们接着来看看：
```java
responseCallback = ^(id responseData) {
                    if (responseData == nil) {
                        responseData = [NSNull null];
                    }
                    
                    WVJBMessage* msg = @{ @"responseId":callbackId, @"responseData":responseData };
                    [self _queueMessage:msg];
                };
```
这个就是你绕的地方，他是后被定义的，所以一开不执行，只有在处理数据后回调才会被调用，这里有个方法_queueMessage：
```java
- (void)_queueMessage:(WVJBMessage*)message {
    if (_startupMessageQueue) {
        [_startupMessageQueue addObject:message];
    } else {
        [self _dispatchMessage:message];
    }
}
```
这里面还有个方法：
```java
- (void)_dispatchMessage:(WVJBMessage*)message {
    NSString *messageJSON = [self _serializeMessage:message];
    [self _log:@"SEND" json:messageJSON];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\\" withString:@"\\\\"];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\"" withString:@"\\\""];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\'" withString:@"\\\'"];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\n" withString:@"\\n"];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\r" withString:@"\\r"];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\f" withString:@"\\f"];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\u2028" withString:@"\\u2028"];
    messageJSON = [messageJSON stringByReplacingOccurrencesOfString:@"\u2029" withString:@"\\u2029"];
    NSString* javascriptCommand = [NSString stringWithFormat:@"WebViewJavascriptBridge._handleMessageFromObjC('%@');", messageJSON];
    if ([[NSThread currentThread] isMainThread]) {
        [_webView stringByEvaluatingJavaScriptFromString:javascriptCommand];
    } else {
        __strong WVJB_WEBVIEW_TYPE* strongWebView = _webView;
        dispatch_sync(dispatch_get_main_queue(), ^{
            [strongWebView stringByEvaluatingJavaScriptFromString:javascriptCommand];
        });
    }
}
```
我们在回到WebViewJavascriptBridge.js.txt文件中看到
```java
function _handleMessageFromObjC(messageJSON) {
		if (receiveMessageQueue) {
			receiveMessageQueue.push(messageJSON)
		} else {//肯定走这个  为什么呢？
			_dispatchMessageFromObjC(messageJSON)
		}
	}
```
再来看看：
```java
function _dispatchMessageFromObjC(messageJSON) {
		setTimeout(function _timeoutDispatchMessageFromObjC() {
			var message = JSON.parse(messageJSON)
			var messageHandler
			var responseCallback
			if (message.responseId) {
				responseCallback = responseCallbacks[message.responseId]
				if (!responseCallback) { return; }
				responseCallback(message.responseData)
				delete responseCallbacks[message.responseId]
			} else {
				if (message.callbackId) {
					var callbackResponseId = message.callbackId
					responseCallback = function(responseData) {
						_doSend({ responseId:callbackResponseId, responseData:responseData })
					}
				}
				
				var handler = WebViewJavascriptBridge._messageHandler
				if (message.handlerName) {
					handler = messageHandlers[message.handlerName]
				}
				
				try {
					handler(message.data, responseCallback)
				} catch(exception) {
					if (typeof console != 'undefined') {
						console.log("WebViewJavascriptBridge: WARNING: javascript handler threw.", message, exception)
					}
				}
			}
		})
	}
```
大家还记得我们返回的对象是：
```java
@{ @"responseId":callbackId, @"responseData":responseData }
```
所以这里messageHandlers刚才也说过了用来存方法的，callbackId被换了个名字叫responseId意思一样，只要值没变就行，所以就会执行：
```java
bridge.callHandler('testObjcCallback', {'foo': 'cccccccccccc'}, function(response) {
				log('JS got response', response)
			})
```
中的方法，好了，完了。
总结一下：js这边 先把方法名字、参数、处理方法保存成一个字典在转成json字符串，在通过UIWebview调用js中某个方法把这个json字符串传到Native中去（不是通过url传的，这样太low了），同时把这个处理的方法以key-value形式放到一个js的字典中。
UIWebView在收到这个json之后，进行数据处理、还有js的回掉的处理方法（就是那个callbackId）处理完成后也会拼成一个key-value字典通过调用js传回去（可以直接调用js）。
js在接到这个json后，根据responseId读取responseCallbacks中处理方法进行处理Native code返回的数据。
### 3.Native调用js method
过程不是直接调用js，也是通过js调用Native过程一样的处理方式。
大体来看一下，先看一个按钮的单击事件：
```java
- (void)callHandler:(id)sender {
    id data = @{ @"greetingFromObjC": @"Hi there, JS!" };
    [_bridge callHandler:@"testJavascriptHandler" data:data responseCallback:^(id response) {
        NSLog(@"testJavascriptHandler responded: %@", response);
    }];
}
```
看看callHandler:
```java
- (void)callHandler:(NSString *)handlerName data:(id)data responseCallback:(WVJBResponseCallback)responseCallback {
    [self _sendData:data responseCallback:responseCallback handlerName:handlerName];
}
```
看看_sendData:
```java
- (void)_sendData:(id)data responseCallback:(WVJBResponseCallback)responseCallback handlerName:(NSString*)handlerName {
    NSMutableDictionary* message = [NSMutableDictionary dictionary];
    
    if (data) {
        message[@"data"] = data;
    }
    
    if (responseCallback) {
        NSString* callbackId = [NSString stringWithFormat:@"objc_cb_%ld", ++_uniqueId];
        _responseCallbacks[callbackId] = [responseCallback copy];
        message[@"callbackId"] = callbackId;
    }
    
    if (handlerName) {
        message[@"handlerName"] = handlerName;
    }
    [self _queueMessage:message];
}
```
到_queueMessage:之后流程就和上面一样了，这里面native也有个：
```java
NSString* responseId = message[@"responseId"];
        if (responseId) {
            WVJBResponseCallback responseCallback = _responseCallbacks[responseId];
            responseCallback(message[@"responseData"]);
            [_responseCallbacks removeObjectForKey:responseId];
        }
```
这个和js中的处理思想是一样的。
总结：native将方法名、参数、回到的id放到一个对象中传给js。
js根据方法名字调用相应方法，之后将返回数据和responseId拼装，最后通过src 重定向到UIWebview 的delegate。
native得到数据后根据responseId调用事先装入_responseCallbacks的block，动态读取调用，从而完成交互。
