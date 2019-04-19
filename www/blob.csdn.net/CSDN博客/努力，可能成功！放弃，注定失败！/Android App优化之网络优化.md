# Android App优化之网络优化 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月10日 08:49:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4302
## 引言
- [背景:Android App优化, 要怎么做?](http://www.jianshu.com/p/f7006ab64da7)
- [Android App优化之性能分析工具](http://www.jianshu.com/p/da2a4bfcba68)
- [Android App优化之提升你的App启动速度之理论基础](http://www.jianshu.com/p/98c1656a357a)
- [Android App优化之提升你的App启动速度之实例挑战](http://www.jianshu.com/p/4f10c9a10ac9)
- [Android App优化之Layout怎么摆](http://www.jianshu.com/p/4943dae4c333)
- [Android App优化之ANR详解](http://www.jianshu.com/p/6d855e984b99)
- [Android App优化之消除卡顿](http://www.jianshu.com/p/1fb065c806e6)
- [Android App优化之内存优化](http://www.jianshu.com/p/48475df838d9)
- [Android App优化之持久电量](http://www.jianshu.com/p/c55ef05c0047)
- [Android App优化之如何高效网络请求](http://www.jianshu.com/p/d4c2c62ffc35)
互联网时代, App作为于用户交互的端, 可以说实际上是一个界面, 产品的业务, 服务都是由Server提供的. 而App与Server的交互依赖于网络, 故而网络优化, 也是我们的App优化中不可缺少的一个优化项.
## 1, 网络连接对用户的影响
App的网络连接对于用户来说, 影响很多, 且多数情况下都很直观, 直接影响用户对这个App的使用体验. 其中较为重要的几点:
- 
**流量**
App的流量消耗对用户来说是比较敏感的, 毕竟流量是花钱的嘛. 现在大部分人的手机上都有安装流量监控的工具App, 用来监控App的流量使用. 如果我们的App这方面没有控制好, 会给用户不好的使用体验.
- 
**电量**
电量相对于用户来说, 没有那么明显. 一般用户可能不会太注意. 但是如[前文电量优化](http://www.jianshu.com/p/c55ef05c0047)中说的那样, 网络连接(radio)是对电量影响很大的一个因素. 所以我们也要加以注意.
- 
**用户等待**
也就是用户体验, 良好的用户体验, 才是我们留住用户的第一步. 如果App请求等待时间长, 会给用户网络卡, 应用反应慢的感觉, 如果有对比, 有替代品, 我们的App很可能就会被用户无情抛弃.
## 2, 分析网络连接的工具
### 2.1 Network Monitor
Android Studio内置的Monitor工具中就有一个Network Monitor:
![](http://upload-images.jianshu.io/upload_images/851999-15e7840ec312268e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Network Monitor
其中: 
- Rx --- R(ecive) 表示下行流量, 即下载接收.
- Tx --- T(ransmit) 表示上行流量, 即上传发送. 
**怎么使用Network Monitor?**
Network monitor实时跟踪选定应用的数据请求情况. 我们可以连上手机, 选定调试应用进程, 然后在App上操作我们需要分析的页面请求.
例如, 上图就是以[CoderPub](https://github.com/mingjunli/GithubApp)为例, 针对从repo列表界面进入repo详情界面的监控数据.
可以看到从10s到30s之间, 20s时间内发生了多次数据请求, 且22s到27s之间的请求数据量还很大.
分析代码可以看到, 在请求repo详情的时候是打包了很多请求的:
```java
```java
@Override
public Observable<RepoDetail> getRepoDetail(String owner, String name) {
    return Observable.zip(mRepoService.get(owner, name),
            mRepoService.contributors(owner, name),
            mRepoService.listForks(owner, name, "newest"),
            mRepoService.readme(owner, name),
            isStarred(owner, name),
            new Func5<Repo, ArrayList<User>, ArrayList<Repo>, Content, Boolean, RepoDetail>() {
                @Override
                public RepoDetail call(Repo repo, ArrayList<User> users, ArrayList<Repo> forks, Content readme, Boolean isStarred) {
                    RepoDetail detail = new RepoDetail();
                    repo.setStarred(isStarred);
                    detail.setBaseRepo(repo);
                    detail.setForks(forks);
                    // because the readme content is encode with Base64 by github.
                    readme.content = StringUtil.base64Decode(readme.content);
                    detail.setReadme(readme);
                    detail.setContributors(users);
                    return detail;
                }
            });
}
```
```
这也验证了14s到20s间的四次数据请求, 另外由于repo详情界面会显示作者以及贡献者的图片, 而图片的数据量相对大, 故而23s到27s间有多次数据量很大的请求发生.
> 
这个实际是有很多优化空间的, 我们稍后再说.
### 2.2 网络代理工具
一般来说, 网络代理工具有两个作用:
- 截获网络请求响应包, 分析网络请求
- 设置代理网络, 移动App开发中一般用来做不同网络环境的测试, 例如Wifi/4G/3G/弱网等.
代理工具很多, 诸如[Wireshark](https://www.wireshark.org/),
[Fiddler](http://www.telerik.com/fiddler),
[Charles](https://www.charlesproxy.com/)等, 在此不一一细说了, 使用方法自行问谷歌度娘. :)
## 3, 哪些方面取优化网络连接
第一节说到了网络请求对App和用户的影响, 那么我们怎么从哪些方面去优化网络进而减少甚至消灭这些影响呢?
简单来说, 两个方面:
- 
**减少Radio活跃时间**
- 也就是减少网络数据获取的频次.
- 这就减少了radio的电量消耗, 控制电量使用.
- 
**减少获取数据包的大小**
- 可以减少流量消耗
- 也可以让每次请求更快, 在网络情况不好的情况下也有良好表现, 提升用户体验.
**那么, 具体应该从哪些方面着手呢?**
### 3.1 接口设计
#### API设计
App与Server之间的API设计要考虑网络请求的频次, 资源的状态等. 以便App可以以较少的请求来完成业务需求和界面的展示.
例如, 注册登录. 正常会有两个API, 注册和登录, 但是设计API时我们应该给注册接口包含一个隐式的登录. 来避免App在注册后还得请求一次登录接口(有可能失败, 从而导致业务流程失败).
再例如, 上文提到的获取repo详情, 实际上请求了4个接口, 请求了repo的信息, forks列表, contributors列表, readme, 这是因为github提供的接口是尽量单一职责的. 然而在我们的实际开发中, 我们的Server除了提供这些单一职责的小接口外, 最好还能组合一个满足客户端业务需求的repo详情接口出来.
#### Gzip压缩
使用Gzip来压缩request和response, 减少传输数据量, 从而减少流量消耗.
#### 考虑使用Protocol Buffer代替JSON
从前我们传输数据使用XML, 后来使用JSON代替了XML, 很大程度上也是为了可读性和减少数据量(当然还有映射成POJO的方便程度).
[Protocol Buffer](https://github.com/google/protobuf/)是Google推出的一种数据交换格式.
如果我们的接口每次传输的数据量很大的话, 可以考虑下protobuf, 会比JSON数据量小很多. 
当然相比来说, JSON也有其优势, 可读性更高.
> 
本文以网络流量优化的角度推荐protobuf作为一个选择, 具体还需更具实际情况考虑.
#### 图片的Size
上面Network Monitor中看到的22s到27s之间的有多次请求, 且数据量还很大. 就是在获取图片资源.
图片相对于接口请求来说, 数据量要大得多. 故而也是我们需要优化的一个点.
我们可以在获取图片时告知服务器需要的图片的宽高, 以便服务器给出合适的图片, 避免浪费. 
我们现在很多公司的图片资源都是使用第三方的云存储服务的(七牛, 阿里云存储之类的).
以七牛为例, 可以在请求图片的url中添加诸如质量, 格式, width, height等path来获取合适的图片资源:
```xml
imageView2/<mode>/w/<LongEdge>
                 /h/<ShortEdge>
                 /format/<Format>
                 /interlace/<Interlace>
                 /q/<Quality>
                 /ignore-error/<ignoreError>
```
> 
参考[七牛官方文档](http://developer.qiniu.com/code/v6/api/kodo-api/image/imageview2.html).
### 3.2 网络缓存
适当的缓存, 既可以让我们的应用看起来更快, 也能避免一些不必要的流量消耗.
关于Android App的网络缓存, 请参考[MVP架构实现的Github客户端(4-加入网络缓存)](http://www.jianshu.com/p/faa46bbe8a2e)一文.
### 3.3 打包网络请求
当接口设计不能满足我们的业务需求时. 例如可能一个界面需要请求多个接口, 或是网络良好, 处于Wifi状态下时我们想获取更多的数据等.
这时就可以打包一些网络请求, 例如请求列表的同时, 获取Header点击率较高的的item项的详情数据.
> 
可以通过一些统计数据来帮助我们定位用户接下来的操作是高概率的, 提前获取这部分的数据.
### 3.4 监听相关状态
通过监听设备的状态:
- 休眠状态
- 充电状态
- 网络状态
结合[JobScheduler](https://developer.android.com/reference/android/app/job/JobScheduler.html)来根据实际情况做网络请求. 比方说Splash闪屏广告图片, 我们可以在连接到Wifi时下载缓存到本地; 新闻类的App可以在充电, Wifi状态下做离线缓存.
### 3.5 弱网测试&优化
除了正常的网络优化, 我们还需考虑到弱网情况下, App的表现.
### 3.5.1 弱网测试
有几种方式来模拟弱网进行测试.
#### Android Emulator
创建和启动Android模拟器可以设置网络速度和延迟:
**创建时**:
![](http://upload-images.jianshu.io/upload_images/851999-0062476d94a4a87a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
create emulator
**启动时**, 使用emulator命令:
```bash
$emulator -netdelay gprs -netspeed gsm -avd Nexus_5_API_22
```
具体参数参考[这里](https://developer.android.com/studio/run/emulator-commandline.html#netdelay)和[这里](https://developer.android.com/studio/run/emulator-commandline.html#netspeed),
 需要翻墙.
#### 使用网络代理工具
以[Charles](https://www.charlesproxy.com/)为例:
保持手机和PC处于同一个局域网, 在手机端wifi设置高级设置中设置代理方式为手动, 代理ip填写PC端ip地址, 端口号默认8888.
![](http://upload-images.jianshu.io/upload_images/851999-e5542fd23c2e2350.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
charles proxy
![](http://upload-images.jianshu.io/upload_images/851999-cae8443f32c04b30.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
charles throttling
#### 其他模拟弱网方式
如果你恰好也是iOS的开发者, Apple提供了[Network Link Conditioner](http://nshipster.cn/network-link-conditioner/), 非常好用.
可以模拟的网络情况与上述类似:
![](http://upload-images.jianshu.io/upload_images/851999-d4fdd5270af8537d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ios_network
如果你使用Linux环境开发, 还可以试下facebook出的[ATC](http://facebook.github.io/augmented-traffic-control/).
### 3.5.2 弱网优化
利用上述工具模拟弱网, 在弱网情况下体验我们的App. 一般来说, 网络延迟在60ms内, 是OK的, 超过200ms就比较糟糕了. 我们需要做的是在比较糟糕的网络环境下还能给用户较好的体验.
弱网优化, 本质上是在弱网的情况下能让用户流畅的使用我们的App. 我们要做的就是结合上述的优化项:
- 压缩/减少数据传输量
- 利用缓存减少网络传输
- 针对弱网(移动网络), 不自动加载图片
- 界面先反馈, 请求延迟提交
例如, 用户点赞操作, 可以直接给出界面的点赞成功的反馈, 使用[**JobScheduler**](https://developer.android.com/reference/android/app/job/JobScheduler.html)在网络情况较好的时候打包请求.
## 结语
网络优化, 是App优化中相当重要的一项优化. 除了客户端, 接口的优化外, 很多一部分优化还依赖于服务器端, 包括服务器端的代码开发, 部署方式等. 跟你的服务器开发/运维工程师一起聊聊这个话题吧:)
理论上这个应该是[App优化系列](http://www.jianshu.com/p/f7006ab64da7)的最后一篇, 惯例得做些总结啊. 但是如前面[内存优化](http://www.jianshu.com/p/48475df838d9)所言, 内存优化的相关东西将单拎出来, 但是还是会作为本系列的延续. 到时再总结吧.
> 
转载请注明出处, 欢迎大家分享到朋友圈, 微博~
文／anly_jun（简书作者）
原文链接：http://www.jianshu.com/p/d4c2c62ffc35
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
