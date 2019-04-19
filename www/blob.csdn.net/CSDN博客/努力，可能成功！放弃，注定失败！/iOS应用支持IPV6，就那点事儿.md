# iOS应用支持IPV6，就那点事儿 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月25日 10:07:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：11045
果然是苹果打个哈欠，iOS行业内就得起一次风暴呀。自从5月初Apple明文规定所有开发者在6月1号以后提交新版本需要支持IPV6-Only的网络，大家便开始热火朝天的研究如何支持IPV6，以及应用中哪些模块目前不支持IPV6。
### 一、IPV6-Only支持是啥？
首先IPV6，是对IPV4地址空间的扩充。目前当我们用iOS设备连接上Wifi、4G、3G等网络时，设备被分配的地址均是IPV4地址，但是随着运营商和企业逐渐部署IPV6 DNS64/NAT64网络之后，设备被分配的地址会变成IPV6的地址，而这些网络就是所谓的IPV6-Only网络，并且仍然可以通过此网络去获取IPV4地址提供的内容。客户端向服务器端请求域名解析，首先通过DNS64 Server查询IPv6的地址，如果查询不到，再向DNS Server查询IPv4地址，通过DNS64 Server合成一个IPV6的地址，最终将一个IPV6的地址返回给客户端。如图所示：
![](http://upload-images.jianshu.io/upload_images/1483938-b0ec6307df5176fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NAT64-DNS64-ResolutionOfIPv4_2x.png
在Mac OS 10.11＋的双网卡的Mac机器（以太网口＋无线网卡），我们可以通过模拟构建这么一个local IPv6 DNS64/NAT64 的网络环境去测试应用是否支持IPV6-Only网络，大概原理如下：
![](http://upload-images.jianshu.io/upload_images/1483938-90400afee99e61f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
local_ipv6_dns64_nat64_network_2x.png
- 参考资料：
- [https://developer.apple.com/library/mac/documentation/NetworkingInternetWeb/Conceptual/NetworkingOverview/UnderstandingandPreparingfortheIPv6Transition/UnderstandingandPreparingfortheIPv6Transition.html#//apple_ref/doc/uid/TP40010220-CH213-SW1](https://developer.apple.com/library/mac/documentation/NetworkingInternetWeb/Conceptual/NetworkingOverview/UnderstandingandPreparingfortheIPv6Transition/UnderstandingandPreparingfortheIPv6Transition.html#//apple_ref/doc/uid/TP40010220-CH213-SW1)
### 二、Apple如何审核支持IPV6-Only？
**首先第一点：**这里说的支持IPV6-Only网络，其实就是说让应用在 IPv6 DNS64/NAT64 网络环境下仍然能够正常运行。但是考虑到我们目前的实际网络环境仍然是IPV4网络，所以应用需要能够同时保证IPV4和IPV6环境下的可用性。从这点来说，苹果不会去扫描IPV4的专有API来拒绝审核通过，因为IPV4的API和IPV6的API调用都会同时存在于代码中。
**其次第二点：**Apple官方声明iOS9开始向IPV6支持过渡，在iOS9.2+支持IPV4地址合成IPV6地址。其提供的Reachability库在iOS8系统下，当从IPV4切换到IPV6网络，或者从IPV6网络切换到IPV4，是无法监控到网络状态的变化。也有一些开发者针对这些Bug询问Apple的审核部门，***给予的答复是只需要在苹果最新的系统上保证IPV6的兼容性即可***。
**最后第三点：**只要应用的主流程支持IPV6，通过苹果审核即可。对于不支持IPV6的模块，考虑到我们现实IPV6网络的部署还需要一段时间，短时间内不会影响我们用户的使用。但随着4G网络IPV6的部署，这部分模块还是需要逐渐安排人力进行支持。
### 三、应用如何支持IPV6-Only？
对于如何支持IPV6-Only，官方给出了如下几点标准：（这里就不对其进行解释了，大家看上面的参考链接即可）
```
```
1. Use High-Level Networking Frameworks;
2. Don’t Use IP Address Literals;
3. Check Source Code for IPv6 DNS64/NAT64 Incompatibilities;
4. Use System APIs to Synthesize IPv6 Addresses;
```
```
#### 3.1 NSURLConnection是否支持IPV6？
官方的这句话让我们疑惑顿生：
***using high-level networking APIs such as NSURLSession and the CFNetwork frameworks and you connect by name, you should not need to change anything for your app to work with IPv6 addresses***
只说了NSURLSession和CFNetwork的API不需要改变，但是并没有提及到NSURLConnection。 从上文的参考资料中，我们看到NSURLSession、NSURLConnection同属于Cocoa的url loading system，可以猜测出NSURLConnection在ios9上是支持IPV6的。
应用里面的API网络请求，大家一般都会选择AFNetworking进行请求发送，由于历史原因，应用的代码基本上都深度引用了AFHTTPRequestOperation类，所以目前API网络请求均需要通过NSURLConnection发送出去，所以必须确认NSURLConnection是否支持IPV6. 经过测试，NSURLConnection在最新的iOS9系统上是支持IPV6的。
#### 3.2 Cocoa的URL Loading System从iOS哪个版本开始支持IPV6？
目前我们的应用最低版本还需要支持iOS7，虽然苹果只要求最新版本支持IPV6－Only，但是出于对用户负责的态度，我们仍然需要搞清楚在低版本上URL Loading System的API是否支持IPV6.
（to fix me, make some experiments）待续～～～
#### 3.3 Reachability是否需要修改支持IPV6？
我们可以查到应用中大量使用了Reachability进行网络状态判断，但是在里面却使用了IPV4的专用API。
```
```
在Pods:Reachability中
AF_INET                  Files:Reachability.m
struct sockaddr_in       Files:Reachability.h , Reachability.m
```
```
那Reachability应该如何支持IPV6呢？
（1）目前Github的开源库Reachability的最新版本是3.2，苹果也出了一个Support IPV6 的Reachability的官方样例，我们比较了一下源码，跟Github上的Reachability没有什么差异。
（2）我们通常都是通过一个0.0.0.0 (ZeroAddress)去开启网络状态监控，经过我们测试，在iOS9以上的系统上IPV4和IPV6网络环境均能够正常使用；但是在iOS8上IPV4和IPV6相互切换的时候无法监控到网络状态的变化，可能是因为苹果在iOS8上还并没有对IPV6进行相关支持相关。（但是这仍然满足苹果要求在最新系统版本上支持IPV6的网络）。
（3）当大家都在要求Reachability添加对于IPV6的支持，其实苹果在iOS9以上对Zero Address进行了特别处理，[官方发言](https://developer.apple.com/library/ios/samplecode/Reachability/Listings/ReadMe_md.html#//apple_ref/doc/uid/DTS40007324-ReadMe_md-DontLinkElementID_11)是这样的：
reachabilityForInternetConnection: This monitors the address 0.0.0.0,
which reachability treats as a special token that causes it to actually
monitor the general routing status of the device, both IPv4 and IPv6.
```
+ (instancetype)reachabilityForInternetConnection {
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    return [self reachabilityWithAddress: (const struct sockaddr *) &zeroAddress];
}
```
综上所述，Reachability不需要做任何修改，在iOS9上就可以支持IPV6和IPV4，但是在iOS9以下会存在bug，但是苹果审核并不关心。
### 四、底层的socket API如何同时支持IPV4和IPV6？
由于在应用中使用了网络诊断的组件，大量使用了底层的 socket API，所以对于IPV6支持，这块是个重头戏。如果你的应用中使用了长连接，其必然会使用底层socket API，这一块也是需要支持IPV6的。 对于Socket如何同时支持IPV4和IPV6，可以参考谷歌的开源库[CocoaAsyncSocket](https://github.com/robbiehanson/CocoaAsyncSocket.git).
下面我针对我们的开源 [网络诊断组件](https://github.com/Lede-Inc/LDNetDiagnoService_IOS.git), 说一下是如何同时支持IPV4和IPV6的。
开源地址：[https://github.com/Lede-Inc/LDNetDiagnoService_IOS.git](https://github.com/Lede-Inc/LDNetDiagnoService_IOS.git)
这个网络诊断组件的主要功能如下：
- 本地网络环境的监测（本机IP＋本地网关＋本地DNS＋域名解析）；
- 通过TCP Connect监测到域名的连通性；
- 通过Ping 监测到目标主机的连通耗时；
- 通过traceRoute监测设备到目标主机中间每一个路由器节点的ICMP耗时；
#### 4.1 IP地址从二进制到符号的转化
之前我们都是通过inet_ntoa()进行二进制到符号，这个API只能转化IPV4地址。而inet_ntop()能够兼容转化IPV4和IPV6地址。 写了一个公用的in6_addr的转化方法如下：
```
+(NSString *)formatIPV6Address:(struct in6_addr)ipv6Addr{
    NSString *address = nil;
    char dstStr[INET6_ADDRSTRLEN];
    char srcStr[INET6_ADDRSTRLEN];
    memcpy(srcStr, &ipv6Addr, sizeof(struct in6_addr));
    if(inet_ntop(AF_INET6, srcStr, dstStr, INET6_ADDRSTRLEN) != NULL){
        address = [NSString stringWithUTF8String:dstStr];
    }
    return address;
}
```
#### 4.2 本机IP获取支持IPV6
相当于我们在终端中输入ifconfig命令获取字符串，然后对ifconfig结果字符串进行解析，获取其中en0（模拟器）、pdp_ip0（真机）的ip地址。
注意：
（1）在模拟器和真机上都会出现以FE80开头的IPV6单播地址影响我们判断，所以在这里进行特殊的处理（当第一次遇到不是单播地址的IP地址即为本机IP地址）。
（2）在IPV6环境下，真机测试的时候，第一个出现的是一个IPV4地址，所以在IPV4条件下第一次遇到单播地址不退出。
```php
```php
+ (NSString *)deviceIPAdress
{
        while (temp_addr != NULL) {
            NSLog(@"ifa_name===%@",[NSString stringWithUTF8String:temp_addr->ifa_name]);
            // Check if interface is en0 which is the wifi connection on the iPhone
            if ([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"] || [[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"pdp_ip0"])
            {
                //如果是IPV4地址，直接转化
                if (temp_addr->ifa_addr->sa_family == AF_INET){
                    // Get NSString from C String
                    address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
//                    if (address && ![address isEqualToString:@""] && ![address.uppercaseString hasPrefix:@"FE80"]) break;
                }
                //如果是IPV6地址
                else if (temp_addr->ifa_addr->sa_family == AF_INET6){
                    address = [self formatIPV6Address:((struct sockaddr_in6 *)temp_addr->ifa_addr)->sin6_addr];
                    if (address && ![address isEqualToString:@""] && ![address.uppercaseString hasPrefix:@"FE80"]) break;
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
}
```
```
#### 4.3 设备网关地址获取获取支持IPV6
其实是在IPV4获取网关地址的源码的基础上进行了修改，初开把AF_INET－>AF_INET6, sockaddr -> sockaddr_in6之外，还需要注意如下修改，就是拷贝的地址字节数。去掉了ROUNDUP的处理。 （解析出来的地址老是少了4个字节，结果是偏移量搞错了，纠结了半天），具体参考源码库。
```cpp
```cpp
/* net.route.0.inet.flags.gateway */
    int mib[] = {CTL_NET, PF_ROUTE, 0, AF_INET6, NET_RT_FLAGS, RTF_GATEWAY};
    if (sysctl(mib, sizeof(mib) / sizeof(int), buf, &l, 0, 0) < 0) {
         address = @"192.168.0.1";
    }
    ....
    //for IPV4
    for (i = 0; i < RTAX_MAX; i++) {
                if (rt->rtm_addrs & (1 << i)) {
                    sa_tab[i] = sa;
                    sa = (struct sockaddr *)((char *)sa + ROUNDUP(sa->sa_len));
                } else {
                    sa_tab[i] = NULL;
                }
            }
  //for IPV6
     for (i = 0; i < RTAX_MAX; i++) {
                if (rt->rtm_addrs & (1 << i)) {
                    sa_tab[i] = sa;
                    sa = (struct sockaddr_in6 *)((char *)sa + sa->sin6_len);
                } else {
                    sa_tab[i] = NULL;
                }
            }
```
```
#### 4.4 设备DNS地址获取支持IPV6
IPV4时只需要通过res_ninit进行初始化就可以获取，但是在IPV6环境下需要通过res_getservers()接口才能获取。
```
+(NSArray *)outPutDNSServers{
    res_state res = malloc(sizeof(struct __res_state));
    int result = res_ninit(res);
    NSMutableArray *servers = [[NSMutableArray alloc] init];
    if (result == 0) {
        union res_9_sockaddr_union *addr_union = malloc(res->nscount * sizeof(union res_9_sockaddr_union));
        res_getservers(res, addr_union, res->nscount);
        for (int i = 0; i < res->nscount; i++) {
            if (addr_union[i].sin.sin_family == AF_INET) {
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(addr_union[i].sin.sin_addr), ip, INET_ADDRSTRLEN);
                NSString *dnsIP = [NSString stringWithUTF8String:ip];
                [servers addObject:dnsIP];
                NSLog(@"IPv4 DNS IP: %@", dnsIP);
            } else if (addr_union[i].sin6.sin6_family == AF_INET6) {
                char ip[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(addr_union[i].sin6.sin6_addr), ip, INET6_ADDRSTRLEN);
                NSString *dnsIP = [NSString stringWithUTF8String:ip];
                [servers addObject:dnsIP];
                NSLog(@"IPv6 DNS IP: %@", dnsIP);
            } else {
                NSLog(@"Undefined family.");
            }
        }
    }
    res_nclose(res);
    free(res);
    return [NSArray arrayWithArray:servers];
}
```
#### 4.4 域名DNS地址获取支持IPV6
在IPV4网络下我们通过gethostname获取，而在IPV6环境下，通过新的gethostbyname2函数获取。
```
//ipv4
phot = gethostbyname(hostN);
//ipv6
 phot = gethostbyname2(hostN, AF_INET6);
```
#### 4.5 ping方案支持IPV6
Apple的官方提供了最新的支持IPV6的ping方案，参考地址如下：
[https://developer.apple.com/library/mac/samplecode/SimplePing/Introduction/Intro.html](https://developer.apple.com/library/mac/samplecode/SimplePing/Introduction/Intro.html)
只是需要注意的是：
（1）返回的packet去掉了IPHeader部分，IPV6的header部分也不返回TTL（Time to Live）字段；
（2）IPV6的ICMP报文不进行checkSum的处理；
#### 4.6 traceRoute方案支持IPV6
其实是通过创建socket套接字模拟ICMP报文的发送，以计算耗时；
两个关键的地方需要注意：
（1）IPV6中去掉IP_TTL字段，改用跳数IPV6_UNICAST_HOPS来表示；
（2）sendto方法可以兼容支持IPV4和IPV6，但是需要最后一个参数，制定目标IP地址的大小；因为前一个参数只是指明了IP地址的开始地址。千万不要用统一的sizeof(struct sockaddr), 因为sockaddr_in 和 sockaddr都是16个字节，两者可以通用，但是sockaddr_in6的数据结构是28个字节，如果不显式指定，sendto方法就会一直返回－1，erroNo报22 Invalid argument的错误。
关键代码如下：（完整代码参考开源组件）
```
//构造通用的IP地址结构stuck sockaddr
 NSString *ipAddr0 = [serverDNSs objectAtIndex:0];
    //设置server主机的套接口地址
    NSData *addrData = nil;
    BOOL isIPV6 = NO;
    if ([ipAddr0 rangeOfString:@":"].location == NSNotFound) {
        isIPV6 = NO;
        struct sockaddr_in nativeAddr4;
        memset(&nativeAddr4, 0, sizeof(nativeAddr4));
        nativeAddr4.sin_len = sizeof(nativeAddr4);
        nativeAddr4.sin_family = AF_INET;
        nativeAddr4.sin_port = htons(udpPort);
        nativeAddr4.sin_addr.s_addr = inet_addr([ipAddr0 UTF8String]);
        addrData = [NSData dataWithBytes:&nativeAddr4 length:sizeof(nativeAddr4)];
    } else {
        isIPV6 = YES;
        struct sockaddr_in6 nativeAddr6;
        memset(&nativeAddr6, 0, sizeof(nativeAddr6));
        nativeAddr6.sin6_len = sizeof(nativeAddr6);
        nativeAddr6.sin6_family = AF_INET6;
        nativeAddr6.sin6_port = htons(udpPort);
        inet_pton(AF_INET6, ipAddr0.UTF8String, &nativeAddr6.sin6_addr);
        addrData = [NSData dataWithBytes:&nativeAddr6 length:sizeof(nativeAddr6)];
    }
    struct sockaddr *destination;
    destination = (struct sockaddr *)[addrData bytes];
//创建socket
if ((recv_sock = socket(destination->sa_family, SOCK_DGRAM, isIPV6?IPPROTO_ICMPV6:IPPROTO_ICMP)) < 0)
if ((send_sock = socket(destination->sa_family, SOCK_DGRAM, 0)) < 0)
//设置sender 套接字的ttl
if ((isIPV6? 
setsockopt(send_sock,IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof(ttl)):
setsockopt(send_sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl))) < 0)
//发送成功返回值等于发送消息的长度
ssize_t sentLen = sendto(send_sock, cmsg, sizeof(cmsg), 0, 
(struct sockaddr *)destination, 
isIPV6?sizeof(struct sockaddr_in6):sizeof(struct sockaddr_in));
```
文／philon（简书作者）
原文链接：http://www.jianshu.com/p/a6bab07c4062
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
