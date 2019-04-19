# GCDAsyncSocket不通过服务器进行客户端间直接连接—iOS移动开发 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月26日 10:29:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1953
之前客户端与客户端间的交流是需要通过服务器的转发来进行
现在该方案是：
1.需要一个公共服务器来刷新在线客户端的列表
2.客户端与客户端的数据传递不再需要通过服务器转发
3.客户端与客户端之间建立一个长连接
(使用OC Object-C实现)
构思示意图如下：
![](http://upload-images.jianshu.io/upload_images/2599480-b4246729e50a40b7.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
构思图.jpeg
- 首先，我们需要写一个简单的公共服务器 
1.头文件
```
#import <Foundation/Foundation.h>
@interface ServerSocket : NSObject
/** 端口 */
@property (nonatomic,assign)uint16_t port;
/** 监听地址 */
@property (nonatomic,copy)NSString *listenURL;
/**
 *  单例类方法
 *
 *  @return 单例对象
 */
+(instancetype)shareServerSocket;
/**
 *  开始监听
 */
-(void)startAccept;
@end
```
2.实现文件
```
#import "ServerSocket.h"
#import "GCDAsyncSocket.h"
@interface ServerSocket () <GCDAsyncSocketDelegate>
  /** socket */
  @property (nonatomic,strong)GCDAsyncSocket *socket;
  ** 客户端socket数组 */
  @property (nonatomic,strong)NSMutableArray *clientSockets;
@end
@implementation ServerSocket
-(NSMutableArray *)clientSockets{
   if (!_clientSockets) {
        _clientSockets = [NSMutableArray array];
    }
    return _clientSockets;
 }
+(instancetype)shareServerSocket{
  static ServerSocket *serverSocket;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    serverSocket = [[self alloc]init];
  });
   return serverSocket;
}
-(instancetype)init{
  if (self = [super init]) {
    _socket = [[GCDAsyncSocket alloc]initWithDelegate:self delegateQueue:dispatch_get_global_queue(0, 0)];
  }
    return self;
}
-(instancetype)init{
   if (self = [super init]) {
      _socket = [[GCDAsyncSocket alloc]initWithDelegate:self delegateQueue:dispatch_get_global_queue(0, 0)];
    }
    return self;
  }
-(void)startAccept{   
   NSError *error = nil;
  [self.socket acceptOnInterface:self.listenURL port:self.port error:&error];
  if (error) {
      NSLog(@"开启监听失败 : %@",error);
  }else{
      NSLog(@"开启监听成功");
  }
}
#pragma mark - GCDAsyncSocketDelegate
-(void)socket:(GCDAsyncSocket *)sock didAcceptNewSocket:(GCDAsyncSocket *)newSocket{
  //存放客户端的socket对象。
  [self.clientSockets addObject:newSocket];
  [newSocket readDataWithTimeout:-1 tag:0];
  //向每一个客户端发送给在线客户端列表
  [self sendClientList];
}
#pragma mark - GCDAsyncSocketDelegate
-(void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err{
  //每当有客户端断开连接的时候，客户端数组移除该socket
  [self.clientSockets removeObject:sock];
  //向每一个客户端发送给在线客户端列表
  [self sendClientList];
}
-(void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{
  [sock readDataWithTimeout:-1 tag:tag];
}
/**
 *  向每一个连接的客户端发送所有
 */
-(void)sendClientList{
  //把socket对象中的host和post转化成字符串，存放到数组中
  NSMutableArray *hostArrM = [NSMutableArray array];
  for (GCDAsyncSocket *clientSocket in self.clientSockets) {
      NSString *host_port = [NSString stringWithFormat:@"%@:%d",clientSocket.connectedHost,clientSocket.connectedPort];
      [hostArrM addObject:host_port];
  }
  //再把数组发送给每一个连接的客户端
  NSData *clientData = [NSKeyedArchiver archivedDataWithRootObject:hostArrM];  
  for (GCDAsyncSocket *clientSocket in self.clientSockets) {
      [clientSocket writeData:clientData withTimeout:-1 tag:0];
  }
}
@end
```
- 由于客户端的代码较多，所以这里只写核心
![](http://upload-images.jianshu.io/upload_images/2599480-5f740fca8892261c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
工程文件.png
- ServerSocket.m 主要是客户端作为同时进行监听的服务，跟公共服务器的代码差不多，不同的是要增加一个协议和代理
```
@protocol ServerSocketDelegate <NSObject>
-(void)ServerSocket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag;
@end
/** 代理 */
@property (nonatomic,weak)id<ServerSocketDelegate> delegate;
/** 接收到其他客户端发来的信息，要把信息传递给代理，通知外界 */
-(void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{
    [sock readDataWithTimeout:-1 tag:0];
    if ([self.delegate respondsToSelector:@selector(ServerSocket:didReadData:withTag:)]) {
        [self.delegate ServerSocket:sock didReadData:data withTag:tag];
    }
}
```
- ListenDelegate.m 是用来接收自身客户端连接到其他客户端时候的代理，其实可以用当前的controller做为代理，但为了区分跟公共服务器的代理，所以这里另外新建一个类
```
#import "ListenDelegate.h"
#import <GCDAsyncSocket.h>
@interface ListenDelegate ()
@end
@implementation ListenDelegate
-(void)socket:(GCDAsyncSocket *)sock didAcceptNewSocket:(GCDAsyncSocket *)newSocket{
}
-(void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port{
     NSLog(@"客户端与客户端连接成功");
    [sock readDataWithTimeout:-1 tag:0 ];
}
-(void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err{
    NSLog(@"客户端与客户端连接失败 error:%@",err);
}
@end
```
- ViewController.m 主要界面和逻辑的实现 简单实现界面如下:
![](http://upload-images.jianshu.io/upload_images/2599480-59f8a914b3e5fa9b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
界面.png
- 主要代码如下:
```
#pragma mark - GCDAsyncSocketDelegate
-(void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{
    NSArray *clientOnLine = [NSKeyedUnarchiver unarchiveObjectWithData:data];
    //根据最新的在线列表来删除自身在线列表字典已经下线的客户端
    NSMutableArray *deleteKeys = [self.dataSoures mutableCopy];
    [deleteKeys removeObjectsInArray:clientOnLine];
    for (NSString * key in deleteKeys) {
        [self.mySocketsDict removeObjectForKey:key];
    }
    //获取最新的客户端在线列表
    self.dataSoures = [clientOnLine mutableCopy];
    dispatch_sync(dispatch_get_main_queue(), ^{
        [self.tableView reloadData];
    });
    [sock readDataWithTimeout:-1 tag:0];
}
-(void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port{
    NSLog(@"连接成功");
    //更新选中的socket
    self.activeSocket = sock;
    dispatch_sync(dispatch_get_main_queue(), ^{
        self.connectBtn.enabled = NO;
        self.disConnect.enabled = YES;
    });
    //开启自身的监听
    ServerSocket *listenSocket = [ServerSocket shareServerSocket];
    listenSocket.port = sock.localPort;
    listenSocket.listenURL = ListenHost;
    self.listenSocket = listenSocket;
    self.listenSocket.delegate = self;
    [listenSocket startAccept];
    [sock readDataWithTimeout:-1 tag:0];
}
-(void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err{
    //断开连接时，清空数据源
    [self.dataSoures removeAllObjects];
    dispatch_sync(dispatch_get_main_queue(), ^{
        self.connectBtn.enabled = YES;
        self.disConnect.enabled = NO;
        [self.tableView reloadData];
    });
}
#pragma mark - UITableViewDataSource
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSoures.count;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString * cellID = @"cellID";
    MyTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (!cell) {
        cell = [[MyTableViewCell alloc]initWithStyle:UITableViewCellStyleDefault     reuseIdentifier:cellID];
    }
    if (self.dataSoures.count) {
        cell.textLabel.text = self.dataSoures[indexPath.row];
    }
    return cell;
}
#pragma mark - UITableViewDelegate
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    ListenDelegate *ltnDelegate = [[ListenDelegate alloc]init];
    self.ltnDelegate = ltnDelegate;
    //根据选中的客户端进行长连接
    //查看已经保存的其他客户端列表字段中是否已经存在相应的socket
    NSString *hostStr = self.dataSoures[indexPath.row];
    GCDAsyncSocket *activeSocket = self.mySocketsDict[hostStr];
    //如果字典中没有对应的socket,则创建新的socket,并且存进字段
    if (!activeSocket) {
        activeSocket = [[GCDAsyncSocket alloc]initWithDelegate:ltnDelegate delegateQueue:dispatch_get_global_queue(0, 0)];
        self.mySocketsDict[hostStr] = activeSocket;
    }
    //更新选中的socket
    self.activeSocket = activeSocket;
    //行进socket连接
    NSArray *arr = [hostStr componentsSeparatedByString:@":"];
    NSError *error = nil;
    NSString *portStr = arr[1];
    [activeSocket connectToHost:arr[0] onPort:portStr.integerValue error:&error];
}
#pragma mark ServerSocketDelegate
-(void)ServerSocket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{
    //接受其他客户端发送的消息，并且显示
    NSString *receiveStr = [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
    dispatch_sync(dispatch_get_main_queue(), ^{
        NSString *resultStr = [NSString     stringWithFormat:@"%@\n%@",self.receiveDataLabel.text,receiveStr] ;
        self.receiveDataLabel.text = resultStr;
    });
}
```
完整项目下载地址：
[https://github.com/kinglchristina/ClientSocket.git](https://github.com/kinglchristina/ClientSocket.git)
文／MyMiaBaby（简书作者）
原文链接：http://www.jianshu.com/p/9f3341c86115
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
