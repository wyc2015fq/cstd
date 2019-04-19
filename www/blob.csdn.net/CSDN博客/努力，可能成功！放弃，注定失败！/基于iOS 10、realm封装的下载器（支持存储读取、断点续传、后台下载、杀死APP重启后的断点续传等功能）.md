# 基于iOS 10、realm封装的下载器（支持存储读取、断点续传、后台下载、杀死APP重启后的断点续传等功能） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月31日 17:04:07[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2022
尊重知识，转发请注明出处：[基于iOS 10、realm封装的下载器（支持存储读取、断点续传、后台下载、杀死APP重启后的断点续传等功能）](http://www.jianshu.com/p/b4edfa0b71d8)
> 
# 概要
在决定自己封装一个下载器前，我本以为没有那么复杂，可在实际开发过程中困难重重，再加上iOS10和Xcode8的发布，更是带来一些意外的麻烦，断断续续过了一个多月的时间才弄出一个可用的版本。目前网上关于iOS10下载模块出现的bug以及一些特殊情况如何处理的文章比较少，最起码我还没有看到过，这里抛砖引玉，给小伙伴们提供一些思路，也算是这篇文章存在的一点点价值。
公司一个音频项目的下载模块使用的是第三方的，总是会出现无法正常下载等问题，并且由于很难短时间内了解这个颇为庞大复杂的第三方库，所有比较难以解决出现的bug，因此我决定自己封装一个。当然网上会找到一些基于ASI封装的下载器，下载demo简单试用后均没发现什么问题，但是我还是弃用了，主要原因是怕出现问题，由于不了解这些第三方库和ASI而无法解决，另一方面确实不想再将ASI引入到项目里了，同时我觉得也确实应该好好研究下这方面的知识了。
在开发过程中发现这个太过频繁使用的功能在iOS端并不那么容易做好，基于Apple自己的接口开发确实比较难实现我们常用的下载需求，这或许就是AFN一直没有很好的实现下载模块的原因，AFN对下载的封装，完全基于Apple自己的接口简单的封装，其实和直接Apple的接口区别并不大，所以想直接使用AFN实现较为复杂下载功能的小伙伴可能要失望了。
下面说明下本文的讲解思路，主要是按照下载功能进行模块化的讲解，比如下载、断点续传、删除信息、更新信息等，单个功能分开阐述，比较利于理解，也方便大家分不同的时间阅读，避免一口气读完如此长的技术性文章的厌烦感，同时分模块阐述后大家觉得有用的可以借鉴下，觉得没用的大可当糟粕一样弃之。
`注意1、本文不敢妄称封装了可以直接在项目中使用的库。一方面由于我自己只是写了一个demo测试，还没有在实际的项目中应用测试；另一方面由于这里针对了iOS10以后苹果出现的下载的bug进行了特殊处理，后续苹果的API更新有可能会有变化。2、本文旨在给有需求的小伙伴提供一些思路和意见，如果对大家有些许作用是我的荣幸，文中有任何不妥和错误烦请大家不吝笔墨给我指出来，感激不尽。`
> 
# 下载
本文的下载主要针对NSURLSession展开，其他的下载方式比如使用NSData，本文应用不到，这里就不赘述了。
NSURLSession有2种下载模式
第一种：
```
NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.downloadUrl]];
self.downloadSession = [NSURLSession sharedSession];
self.downloadTask = [self.downloadSession downloadTaskWithRequest:request];
[self.downloadTask resume];
```
第二种
```
NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.downloadUrl]];
NSURLSessionConfiguration *sessionCon = [NSURLSessionConfiguration backgroundSessionConfigurationWithIdentifier:self.downloadUrl];
self.downloadSession = [NSURLSession sessionWithConfiguration:sessionCon delegate:self delegateQueue:[NSOperationQueue mainQueue]];
self.downloadTask = [self.downloadSession downloadTaskWithRequest:request];
[self.downloadTask resume];
```
2中方式的暂停下载和继续下载均可以使用
```
[self.downloadTask suspend];
[self.downloadTask resume];
```
当然暂停和继续还可以使用如下方式
```
[self.downloadTask cancelByProducingResumeData:^(NSData * _Nullable resumeData) {
    self.resumeData = resumeData;
}];
self.downloadTask = [self.downloadSession downloadTaskWithResumeData:self.resumeData];
[self.downloadTask resume];
```
`注意看到这里一些小伙伴可能会有些疑惑，两种下载方式和两种暂停继续的方式有何却别，分别针对的是何种使用场景，改如何选择，别着急，下面的内容都会说明，这里暂且有个印象就可以了。`
> 
# 后台下载
众所周知，自从NSURLSession发布后，就可以轻松的实现后台下载了，代码如下：
```
NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.downloadUrl]];
NSURLSessionConfiguration *sessionCon = [NSURLSessionConfiguration backgroundSessionConfigurationWithIdentifier:self.downloadUrl];
self.downloadSession = [NSURLSession sessionWithConfiguration:sessionCon delegate:self delegateQueue:[NSOperationQueue mainQueue]];
self.downloadTask = [self.downloadSession downloadTaskWithRequest:request];
[self.downloadTask resume];
```
`注意你没有看错，就是上面的第二种下载方式，这里也就是下载的2种方式的区别，第一种不支持后台下载，而第二种支持后台下载。`
> 
# 断点续传
适用于网络不中断、APP不重启、iOS9以及以前版本系统。
这里也有2种形式可以实现断点续传，在iOS9及以前的系统中区别并不大
第一种：
```
[self.downloadTask cancelByProducingResumeData:^(NSData * _Nullable resumeData) {
    self.resumeData = resumeData;
}];
self.downloadTask = [self.downloadSession downloadTaskWithResumeData:self.resumeData];
[self.downloadTask resume];
```
第二种：
```
[self.downloadTask cancelByProducingResumeData:^(NSData * _Nullable resumeData) {
}];
```
在中断下载后可以直接在block中获取继续下载需要使用的resumeData，还可以到代理方法中获取
```
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error {
if (error) {
        if ([error.userInfo objectForKey:NSURLSessionDownloadTaskResumeData]){
            self.resumeData = [error.userInfo objectForKey:NSURLSessionDownloadTaskResumeData];
            self.downloadTask = [self.downloadSession downloadTaskWithResumeData:self.resumeData];
            [self.downloadTask resume];
    }
}
```
`注意1、在继续下载的时候，需要有一个NSData形式的resumeData数据实现继续下载，通过转换可以看出，resumeData本质上是一个XML文件，主要记录的是当前下载的链接、已经下载的数据大小、总数据大小等恢复下载需要的信息，如下：`
如下是一个下载的resumeData的XML数据：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
<key>NSURLSessionDownloadURL</key>
<string>http://sbslive.cnrmobile.com/storage/storage2/18/01/18/46eeb50b3f21325a6f4bd0e8ba4d2357.3gp</string>
<key>NSURLSessionResumeBytesReceived</key>
<integer>68188</integer>
<key>NSURLSessionResumeCurrentRequest</key>
<data>
YnBsaXN0MDDUAQIDBAUGeXpYJHZlcnNpb25YJG9iamVjdHNZJGFyY2hpdmVyVCR0b3AS
AAGGoK8QGQcILEdNTlRVVlcrWDlZWmhpamtsbW5vcHVVJG51bGzfEB8JCgsMDQ4PEBES
ExQVFhcYGRobHB0eHyAhIiMkJSYnKCkpKywtLi8wMCkvNCspNjc4OTo7KSk+OykvQkMt
RVIkMV8QIF9fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzIwXxAgX19uc3VybHJl
cXVlc3RfcHJvdG9fcHJvcF9vYmpfMjFfEBBzdGFydFRpbWVvdXRUaW1lXxAecmVxdWly
ZXNTaG9ydENvbm5lY3Rpb25UaW1lb3V0XxAgX19uc3VybHJlcXVlc3RfcHJvdG9fcHJv
cF9vYmpfMTBWJGNsYXNzXxAgX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcF9vYmpfMTFf
ECBfX25zdXJscmVxdWVzdF9wcm90b19wcm9wX29ial8xMl8QIF9fbnN1cmxyZXF1ZXN0
X3Byb3RvX3Byb3Bfb2JqXzEzXxAaX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcHNfECBf
X25zdXJscmVxdWVzdF9wcm90b19wcm9wX29ial8xNF8QIF9fbnN1cmxyZXF1ZXN0X3By
b3RvX3Byb3Bfb2JqXzE1XxAacGF5bG9hZFRyYW5zbWlzc2lvblRpbWVvdXRfECBfX25z
dXJscmVxdWVzdF9wcm90b19wcm9wX29ial8xNl8QFGFsbG93ZWRQcm90b2NvbFR5cGVz
XxAgX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcF9vYmpfMTdfECBfX25zdXJscmVxdWVz
dF9wcm90b19wcm9wX29ial8xOFIkMF8QIF9fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bf
b2JqXzE5XxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcF9vYmpfOV8QH19fbnN1cmxy
ZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzhfEB9fX25zdXJscmVxdWVzdF9wcm90b19wcm9w
X29ial83XxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcF9vYmpfNl8QH19fbnN1cmxy
ZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzVfEB9fX25zdXJscmVxdWVzdF9wcm90b19wcm9w
X29ial80XxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcF9vYmpfM1IkMl8QH19fbnN1
cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzFfEB9fX25zdXJscmVxdWVzdF9wcm90b19w
cm9wX29ial8wXxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJvcF9vYmpfMhAJgACAACMA
AAAAAAAAAAiAAoAYgAeACoAKgACAB4ALgAAQAIAMgA0QAoAOgAiAAIAAgAmACIAAgAcQ
FoADgAKABgjTSA9JKUtMV05TLmJhc2VbTlMucmVsYXRpdmWAAIAFgARfEFtodHRwOi8v
c2JzbGl2ZS5jbnJtb2JpbGUuY29tL3N0b3JhZ2Uvc3RvcmFnZTIvMTgvMDEvMTgvNDZl
ZWI1MGIzZjIxMzI1YTZmNGJkMGU4YmE0ZDIzNTcuM2dw0k9QUVJaJGNsYXNzbmFtZVgk
Y2xhc3Nlc1VOU1VSTKJRU1hOU09iamVjdCNATgAAAAAAABAACRAEE///////////U0dF
VNNbXA9dYmdXTlMua2V5c1pOUy5vYmplY3RzpF5fYGGAD4AQgBGAEqRjZGVmgBOAFIAV
gBaAF1pVc2VyLUFnZW50VkFjY2VwdF8QD0FjY2VwdC1MYW5ndWFnZV8QD0FjY2VwdC1F
bmNvZGluZ18QL1pZTERvd25sb2FkZXIvMSBDRk5ldHdvcmsvODA4LjAuMiBEYXJ3aW4v
MTYuMC4wUyovKlVlbi11c11nemlwLCBkZWZsYXRl0k9QcXJfEBNOU011dGFibGVEaWN0
aW9uYXJ5o3N0U18QE05TTXV0YWJsZURpY3Rpb25hcnlcTlNEaWN0aW9uYXJ50k9Qdndc
TlNVUkxSZXF1ZXN0onhTXE5TVVJMUmVxdWVzdF8QD05TS2V5ZWRBcmNoaXZlctF7fF8Q
G05TS2V5ZWRBcmNoaXZlUm9vdE9iamVjdEtleYABAAgAEQAaACMALQAyADcAUwBZAJoA
nQDAAOMA9gEXAToBQQFkAYcBqgHHAeoCDQIqAk0CZAKHAqoCrQLQAvIDFAM2A1gDegOc
A74DwQPjBAUEJwQpBCsELQQ2BDcEOQQ7BD0EPwRBBEMERQRHBEkESwRNBE8EUQRTBFUE
VwRZBFsEXQRfBGEEYwRlBGcEaQRqBHEEeQSFBIcEiQSLBOkE7gT5BQIFCAULBRQFHQUf
BSAFIgUrBS8FNgU+BUkFTgVQBVIFVAVWBVsFXQVfBWEFYwVlBXAFdwWJBZsFzQXRBdcF
5QXqBgAGBAYaBicGLAY5BjwGSQZbBl4GfAAAAAAAAAIBAAAAAAAAAH0AAAAAAAAAAAAA
AAAAAAZ+
</data>
<key>NSURLSessionResumeEntityTag</key>
<string>"5534b35d-7c7be1"</string>
<key>NSURLSessionResumeInfoTempFileName</key>
<string>CFNetworkDownload_JhfLFD.tmp</string>
<key>NSURLSessionResumeInfoVersion</key>
<integer>2</integer>
<key>NSURLSessionResumeOriginalRequest</key>
<data>
YnBsaXN0MDDUAQIDBAUGUFFYJHZlcnNpb25YJG9iamVjdHNZJGFyY2hpdmVyVCR0b3AS
AAGGoKwHCCQ7QUJISUojS0xVJG51bGzfEBkJCgsMDQ4PEBESExQVFhcYGRobHB0eHyAh
IiMkJSYnKCgqJywjLS4vKionLyonNjclOVIkMV8QEHN0YXJ0VGltZW91dFRpbWVfEB5y
ZXF1aXJlc1Nob3J0Q29ubmVjdGlvblRpbWVvdXRfECBfX25zdXJscmVxdWVzdF9wcm90
b19wcm9wX29ial8xMFYkY2xhc3NfECBfX25zdXJscmVxdWVzdF9wcm90b19wcm9wX29i
al8xMV8QIF9fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzEyXxAgX19uc3VybHJl
cXVlc3RfcHJvdG9fcHJvcF9vYmpfMTNfEBpfX25zdXJscmVxdWVzdF9wcm90b19wcm9w
c18QIF9fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzE0XxAgX19uc3VybHJlcXVl
c3RfcHJvdG9fcHJvcF9vYmpfMTVfEBpwYXlsb2FkVHJhbnNtaXNzaW9uVGltZW91dF8Q
FGFsbG93ZWRQcm90b2NvbFR5cGVzUiQwXxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJv
cF9vYmpfOV8QH19fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzhfEB9fX25zdXJs
cmVxdWVzdF9wcm90b19wcm9wX29ial83XxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJv
cF9vYmpfNl8QH19fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzVfEB9fX25zdXJs
cmVxdWVzdF9wcm90b19wcm9wX29ial80XxAfX19uc3VybHJlcXVlc3RfcHJvdG9fcHJv
cF9vYmpfM1IkMl8QH19fbnN1cmxyZXF1ZXN0X3Byb3RvX3Byb3Bfb2JqXzFfEB9fX25z
dXJscmVxdWVzdF9wcm90b19wcm9wX29ial8wXxAfX19uc3VybHJlcXVlc3RfcHJvdG9f
cHJvcF9vYmpfMhAJIwAAAAAAAAAACIACgAuAB4AJgAmAAIAHgAoQABACgAiAAIAAgAeA
CIAAgAcQEIADgAKABgjTPA09Kj9AV05TLmJhc2VbTlMucmVsYXRpdmWAAIAFgARfEFto
dHRwOi8vc2JzbGl2ZS5jbnJtb2JpbGUuY29tL3N0b3JhZ2Uvc3RvcmFnZTIvMTgvMDEv
MTgvNDZlZWI1MGIzZjIxMzI1YTZmNGJkMGU4YmE0ZDIzNTcuM2dw0kNERUZaJGNsYXNz
bmFtZVgkY2xhc3Nlc1VOU1VSTKJFR1hOU09iamVjdCNATgAAAAAAABAACRP/////////
/9JDRE1OXE5TVVJMUmVxdWVzdKJPR1xOU1VSTFJlcXVlc3RfEA9OU0tleWVkQXJjaGl2
ZXLRUlNfEBtOU0tleWVkQXJjaGl2ZVJvb3RPYmplY3RLZXmAAQAIABEAGgAjAC0AMgA3
AEQASgB/AIIAlQC2ANkA4AEDASYBSQFmAYkBrAHJAeAB4wIFAicCSQJrAo0CrwLRAtQC
9gMYAzoDPANFA0YDSANKA0wDTgNQA1IDVANWA1gDWgNcA14DYANiA2QDZgNoA2oDbANu
A3ADcQN4A4ADjAOOA5ADkgPwA/UEAAQJBA8EEgQbBCQEJgQnBDAENQRCBEUEUgRkBGcE
hQAAAAAAAAIBAAAAAAAAAFQAAAAAAAAAAAAAAAAAAASH
</data>
<key>NSURLSessionResumeServerDownloadDate</key>
<string>Mon, 20 Apr 2015 08:05:49 GMT</string>
</dict>
</plist>
```
```
小伙伴们大概看一下就能从resumeData中看出每一项所表达的意思，其中NSURLSessionResumeOriginalRequest和NSURLSessionResumeCurrentRequest是二进制的，转换成字符串后依然是XML文件，同样是继续下载需要使用的，这里不必深究，大概明白就可以。2、小伙伴们可能会疑惑，2种获取继续下载的数据有何区别，严格来说在iOS9及之前版本肯定是没有区别的，均可以实现继续下载，但是在iOS10之后是有的，后面再说。并且第二种方式看似麻烦，后面也会讲解这种方式的好处。3、有些小伙伴可能会问使用下面的方式不是也可以实现继续下载的功能吗？这里还是有很大的区别的，如字面所表达的一样suspend是挂起的意思，而cancel是取消的意思，也就是说当调用suspend的时候当前的下载进程并没有被销毁，只是暂时停止下载而已，这个下载还占用着系统的资源，而调用cancel时当前的下载进程被销毁了，不占用系统资源，再次调用resume是没有作用的。这里小伙伴们可以先了解这点区别，在模拟器和真机开发中若是不了解这一点区别会造成一些奇怪的bug，后面会有说明。
 [self.downloadTask suspend]; [self.downloadTask resume];4、这里一些小伙伴还会有一个疑问，前面说过下载有2种方式，一种支持后台下载一种不支持，但是断点续传均可以使用同样的方式，那么是否有区别？其实我们在使用上没有区别，但是系统在处理时是有区别的。 这里先简单的说明下区别，使用NSURLSession下载时系统会在本地加保存2份信息，一份信息是我们要下载的文件本身，另一份信息是继续下载数据时需要的resumeData，其中要下载的文件本身我们是可以在沙盒目录中找到的，而resumeData只能通过系统获取。
 从上面的resumeData的XML信息中我们可以获取想要下载的文件储存在沙盒目录中的文件名，NSURLSessionResumeInfoTempFileName表示文件名，CFNetworkDownload_JhfLFD.tmp表示具体文件的文件名，文件名均以“CFNetworkDownload_”开头，以“.tmp”结尾，也就是说无论我们下载的是什么类型的文件，在下载完成前系统都会以.tmp的类型储存文件。那么针对2中下载方式，系统又是如何处理的呢，这里根据我的观察和测试，当使用后台下载模式时，系统会将未下载完成的临时文件储存在tmp文件夹下：/var/mobile/Containers/Data/Application/4BBAD185-327C-4BE5-8D9C-983DFDBAC133/tmp，这种情况下当APP被杀死时，tmp文件被清空，再次进入APP后将无法获取继续下载的数据和已经下载的文件本身。而使用后台下载模式，下载的数据本身就不会被存储到tmp文件夹下了，此时会储存在Library目录下，具体的文件路径是：/var/mobile/Containers/Data/Application/BDE5B81A-4E79-4210-BB7B-20C3B4035D63/Library/Caches/com.apple.nsurlsessiond/Downloads/com.zyldownloader.ZYLDownloaderTest/，其中com.zyldownloader.ZYLDownloaderTest文件夹是根据当前项目的bundleIdentifier命名的，这个文件夹里储存的就是我们未完成下载的文件。当APP被杀死后，这个文件夹不会被清空，再次启动APP后仍然可以获取未完成下载的.tmp文件。
```
到这里就可以实现一个简易的支持后台下载和断点续传的下载器了，只是要在比较理想的网络环境和iOS9及以前版本的系统下，若项目中的下载需求不高，到这里其实足够了，难度不大。但若想实现一个禁得起折腾的下载器，到这里还只是个开始，需要继续往下看。
> 
# APP被杀死后重启的断点续传
适用于网络不中断、iOS9以及以前版本系统。
上面我们实现了断点续传，但是当APP被杀死再重启后就无法在继续下载了，那么这里如何解决呢，系统其实也为我们做好了准备。当APP重新启动后，我们如果想继续下载就要获取resumeData，这里就需要通过代理方法获取了，首先要激活当前下载，代码如下：
```
NSURLSessionConfiguration *sessionCon = [NSURLSessionConfiguration backgroundSessionConfigurationWithIdentifier:self.downloadUrl];
_downloadSession = [NSURLSession sessionWithConfiguration:sessionCon delegate:self delegateQueue:[NSOperationQueue mainQueue]];
```
`注意这里激活下载的前提是，创建下载的时候使用的后台下载模式，同时要为当前下载传入一个ID以标识当前下载，比如我这里直接使用的下载连接作为标识，只有这样才能使用上面的代码激活代理，获取resumeData`
在代理中获取resumeData的代码如下：
```
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error {
if (error) {
        if ([error.userInfo objectForKey:NSURLSessionDownloadTaskResumeData]){
            self.resumeData = [error.userInfo objectForKey:NSURLSessionDownloadTaskResumeData];
            self.downloadTask = [self.downloadSession downloadTaskWithResumeData:self.resumeData];
            [self.downloadTask resume];
    }
}
```
`注意获取resumeData的方法与上面取消下载时获取的方法一致，这里就是这个方法的意义，是可以在APP重启后获取resumeData的。`
在网络正常，创建下载和激活下载正常的情况下，是可以正确获取resumeData的，从而实现了杀死APP后断点续传的功能。
> 
# 网络中断后又恢复的断点续传
适用于iOS9以及以前版本系统
很多小伙伴也许会不解，网络中断再恢复就继续下载啊，这有什么好说的，这似乎是理所当然的事情，我本来也是这么认为的，毕竟太爱了，也太相信，但是万万没想到当网络中断后，无论是通过cancel还是代理方法都无法获取resumeData，在代理方法中只能获取这些报错信息：
`Error Domain=NSURLErrorDomain Code=-1002 "unsupported URL"    UserInfo={NSLocalizedDescription=unsupported URL}`
1、然而对于我们实现继续下载没有意义。到这里我们似乎不知所措了，因为我们已经无法获取继续下载数据，这里也是我遇到的第一个比较难以解决的坑，网上各种查资料也没有找到解决办法，似乎这种情况就应该重新下载，但是这明显不符合用户对于下载的需求，甚至可以说是一种很差的体验。并且在网络中断后，沙盒目录下的未下载完成的文件也会被删除，然后替换成另外一组.tmp文件，可是这些写得.tmp文件不是我们已经下载的数据，暂且不知道用处，无法使用。
2、在尝试各种解决办法不通的情况下，我通过对resumeData XML数据的分析，决定自己生成resumeData，当然我自己在网上查阅众多资料没有发现和我一致的方案，所以暂且认为是我自己发现的方法，若有小伙伴发现比我早的使用这个方案的，请附上链接，我会把刚才那句“是我自己发现的方法”删除，所以不必太纠结这个，关键是我们的问题是否得到解决。
3、方案有了，那我们如何自己生成resumeData呢，通过上面转换成XML形式的resumeData，不难看出，对于一个固定下载链接，有一些信息是固定的，比如表示下载文件链接的NSURLSessionDownloadURL、本地缓存文件名NSURLSessionResumeInfoTempFileName等等，从直观分析看，处于变化的数据似乎只有表示已经下载的文件大小的NSURLSessionResumeBytesReceived，至于NSURLSessionResumeCurrentRequest和NSURLSessionResumeOriginalRequest，通过纵向分析同一个下载链接和横向对比多个下载链接，发现基本都是一样的，因此这里暂且认为他们也是固定的。
4、大概分析出一个规律，我们需要设置一个合理的创建resumeData的方案，经过多种方案的测试，我这里给出一种相对而言比较靠谱的方案，流程如下：
![](http://upload-images.jianshu.io/upload_images/1347742-f96cd57a20a8dffc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
网络中断恢复后断点续传流程
`注意这里为了用自己创建的resumeData实现断点续传的功能，需要在沙盒目录的Documents目录下创建3个文件夹：ZYLDownloads、ZYLResumeDownloads、ZYLUnDownloads，ZYLDownloads用于存储已经一下载好的文件，ZYLUnDownloads用于存储没有下载好的文件，ZYLResumeDownloads用于存储resumeData数据。`
下面逐步讲解
①获取系统提供的resumeData。这里在下载进度的代理中获取，代码如下：
```
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite {
    if (self.isBeginDownload == NO) {
    //还没有开始下载
    self.isBeginDownload = YES;
    //在这里取得继续下载的数据
    [self.downloadTask cancelByProducingResumeData:^(NSData * _Nullable resumeData) {
    //到代理中获取resumeData，此处获取的resumeData在iOS10和Xcode8中有可能无法使用，shit！
    }];
    }
}
```
`注意这里取消下载的时候使用的是cancelByProducingResumeData:^(NSData * _Nullable resumeData)，在这里其实是可以获取resumeData的，但是在iOS10中发现会出现无法使用的情况，因此这里到- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error 代理中获取，步骤和上面提到的一样，获取到resumeData，记录下来即可。`
②分析resumeData，代码如下:
```
- (void)parseResumeData:(NSData *)resumeData {
    NSString *XMLStr = [[NSString alloc] initWithData:resumeData encoding:NSUTF8StringEncoding];
    self.resumeString = [NSMutableString stringWithFormat:@"%@", XMLStr];
    NSRange tmpRange = [XMLStr rangeOfString:@"NSURLSessionResumeInfoTempFileName"];
    NSString *tmpStr = [XMLStr substringFromIndex:tmpRange.location + tmpRange.length];
    NSRange oneStringRange = [tmpStr rangeOfString:@"<string>"];
    NSRange twoStringRange = [tmpStr rangeOfString:@"</string>"];
    //记录tmp文件名
    self.tmpFilename = [tmpStr substringWithRange:NSMakeRange(oneStringRange.location + oneStringRange.length, twoStringRange.location - oneStringRange.location - oneStringRange.length)];
    //有数据，保存到本地
    //存储数据,:self.resumeDirectoryStr就是保存resumeData的路径ZYLResumeDownloads
    BOOL isS = [resumeData writeToFile:self.resumeDirectoryStr atomically:NO];
    if (isS) {
        //继续存储数据成功
        NSLog(@"继续存储数据成功");
    } else {
        //继续存储数据失败
        NSLog(@"继续存储数据失败");
    }
}
```
③分析成功后使用获取的resumeData继续下载，代码如下：
```
self.downloadTask = [self.downloadSession downloadTaskWithResumeData:newData];
[self.downloadTask resume];
```
④缓存下载的文件
这里采用的是每下载1M就缓存一次，因为在网络断开后这些数据会消失，并且我们无法准确及时的判断网络何时中断，所以只能采用这种看似笨拙的方法了，当然有一种可能就是，网络断开时下载的文件大小和网络恢复时下载的文件大小会不一致，有大概不超过1M的误差，还算在可以接受的范围内。这里在下载进度的代理方法中处理，代码如下：
```
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite {
    CGFloat addSize = (totalBytesWritten - self.lastDownloadSize) / 1024.0 / 1024.0;
    if (addSize >= 1.0) {
        //下载的量大于1M,迁移
        NSError *error = nil;
        if ([self.fileManager fileExistsAtPath:self.unDownloadStr]) {
            //存在则删除
            [self.fileManager removeItemAtPath:self.unDownloadStr error:nil];
        }
        BOOL isS = [self.fileManager copyItemAtPath:self.libraryUnDownloadStr toPath:self.unDownloadStr error:&error];
        if (isS) {
            //NSLog(@"移动成功");
        } else {
            NSLog(@"移动失败%@", error);
        }
        self.lastDownloadSize = totalBytesWritten;
    }
}
```
⑤网络中断后自行创建resumeData
由于网络中断我们是无法获取系统提供的resumeData的，所以要根据上面获取的信息自行创建，同时将数据保存到本地。代码如下：
```
- (void)updateLocalResumeData {
    //在这创建resumeData
    //首先取出沙盒目录下的缓存文件
    NSData *libraryData = [NSData dataWithContentsOfFile:self.unDownloadStr];
    NSInteger libraryLength = libraryData.length;
    //计算当期表示resumeData数据大小的range
    //记录tmp文件大小范围
    NSRange integerRange = [self.resumeString rangeOfString:@"NSURLSessionResumeBytesReceived"];
    NSString *integerStr = [self.resumeString substringFromIndex:integerRange.location + integerRange.length];
    NSRange oneIntegerRange = [integerStr rangeOfString:@"<integer>"];
    NSRange twonIntegerRange = [integerStr rangeOfString:@"</integer>"];
    self.libraryFilenameRange = NSMakeRange(oneIntegerRange.location + oneIntegerRange.length + integerRange.location + integerRange.length, twonIntegerRange.location - oneIntegerRange.location - oneIntegerRange.length);
    //用新的数据替换
    [self.resumeString replaceCharactersInRange:self.libraryFilenameRange withString:[NSString stringWithFormat:@"%ld", (long)libraryLength]];
    NSData *newResumeData = [self.resumeString dataUsingEncoding:NSUTF8StringEncoding];
    self.resumeData = newResumeData;
    //同时保存在本地一份
    //获取存储路径
    NSString *path = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0] stringByAppendingPathComponent:@"ZYLResumeDownloads"];
    //获取文件名
    NSString *resumeFileName = [path stringByAppendingPathComponent:[@"resume_" stringByAppendingString:[ZYLTool encodeFilename:self.downloadUrl]]];
    //存储数据
    BOOL isS = [self.resumeData writeToFile:resumeFileName atomically:NO];
    if (isS) {
        //继续存储数据成功
        NSLog(@"继续存储数据成功");
    } else {
        //继续存储数据失败
        NSLog(@"继续存储数据失败");
    }
}
```
`注意我这里只对一开始获取的系统提供的resumeData的NSURLSessionResumeBytesReceived数据进行了更新，经过多次测试，在不修改其他数据的情况下是可以继续下载的。`
⑥网络恢复后实现继续下载
在用自己的数据实现继续下载之前，要把library目录下的系统缓存文件删除，然后将自己缓存的未下载完成的文件移动到对应的文件夹下，然后再从本地读取缓存的resumeData，实现断点续传，代码如下：
```
- (void)resumeAtNoResumeData {
    [_downloadSession invalidateAndCancel];
    _downloadSession = nil;
    //去本地读取继续下载数据
    self.resumeData = [NSData dataWithContentsOfFile:self.resumeDirectoryStr];
    //将继续下载的数据移动到对应的目录下
    NSError *error = nil;
    if ([self.fileManager fileExistsAtPath:self.libraryUnDownloadStr]) {
        BOOL isS = [self.fileManager removeItemAtPath:self.libraryUnDownloadStr error:&error];
        if (!isS) {
            //移除失败
            NSLog(@"移除library下的继续下载数据对应的文件失败:%@", error);
        }
    }
    BOOL isS = [self.fileManager copyItemAtPath:self.unDownloadStr toPath:self.libraryUnDownloadStr error:&error];
    if (!isS) {
        //拷贝失败
        NSLog(@"拷贝继续下载文件到library下失败:%@", error);
    } else {
        //拷贝成功后开启继续下载
        //创建下载任务，继续下载
        self.downloadTask = [self.downloadSession downloadTaskWithResumeData:self.resumeData];
        [self.downloadTask resume];
    }
}
```
`注意上面讲一开始下载后，我们要在下载进度的代理中获取系统提供的resumeData数据，但是这种在本地已经有resumeData数据的前提下就不需要获取了。`
到这里网络中断后实现断点续传的功能就完成了，在iOS9及以前的版本是没有问题的，但是iOS10发布后就失效了，解决办法继续看下文。
> 
# 在iOS10下实现断点续传
把这一块单独拿出来讲是我始料未及的，不过它确实发生了。
事情是这样的，有一天iOS10发布了，Xcode8也发布了，废了好大得劲更新后，再次运行下载器demo，忽然发现下载器完全无法使用了，而控制台打出了一串串这样的报错信息：
```
```
*** -[NSKeyedUnarchiver initForReadingWithData:]: data is NULL
*** -[NSKeyedUnarchiver initForReadingWithData:]: data is NULL
Invalid resume data for background download. Background downloads must use http or https and must download to an accessible file.
```
```
从报错信息来看，是在继续下载时传入的resumeData不可用，可是这里用的是通过调用：cancelByProducingResumeData从系统获取的resumeData，并且从代理中获取的resumeData也是同样的报错信息，均不可用。经过多番查询，终于在stackoverflow找到了遇到相同问题的小伙伴，有大神给出了暂时的解决方案。问题确实出在resumeData中，其中NSURLSessionResumeCurrentRequest和NSURLSessionResumeOriginalRequest的解码有问题，这就是为什么-[NSKeyedUnarchiver
 initForReadingWithData:]: data is NULL会报错2次，解决的方案就是我们在拿到系统的resumeData后要检测数据是否可以正确解码，若不可需要从resumeData的XML数据中取出上面2项再次进行正确的编码，然后创建一个新的resumeData传给系统，完成继续下载，经测试可用，那位大神给的是swift版本的代码，我按照处理逻辑写出了OC代码，如下：
```
- (NSData *)getCorrectResumeData:(NSData *)resumeData {
    NSData *newData = nil;
    NSString *kResumeCurrentRequest = @"NSURLSessionResumeCurrentRequest";
    NSString *kResumeOriginalRequest = @"NSURLSessionResumeOriginalRequest";
    //获取继续数据的字典
    NSMutableDictionary* resumeDictionary = [NSPropertyListSerialization propertyListWithData:resumeData options:NSPropertyListMutableContainers format:NULL error:nil];
    //重新编码原始请求和当前请求
    resumeDictionary[kResumeCurrentRequest] = [self correctRequestData:resumeDictionary[kResumeCurrentRequest]];
    resumeDictionary[kResumeOriginalRequest] = [self correctRequestData:resumeDictionary[kResumeOriginalRequest]];
    newData = [NSPropertyListSerialization dataWithPropertyList:resumeDictionary format:NSPropertyListBinaryFormat_v1_0 options:NSPropertyListMutableContainers error:nil];
    return newData;
} 
- (NSData *)correctRequestData:(NSData *)data {
    NSData *resultData = nil;
    NSData *arData = [NSKeyedUnarchiver unarchiveObjectWithData:data];
    if (arData != nil) {
        return data;
    }
    NSMutableDictionary *archiveDict = [NSPropertyListSerialization propertyListWithData:data options:NSPropertyListMutableContainersAndLeaves format:nil error:nil];
    int k = 0;
    NSMutableDictionary *oneDict = [NSMutableDictionary dictionaryWithDictionary:archiveDict[@"$objects"][1]];
    while (oneDict[[NSString stringWithFormat:@"$%d", k]] != nil) {
        k += 1;
    }
    int i = 0;
    while (oneDict[[NSString stringWithFormat:@"__nsurlrequest_proto_prop_obj_%d", i]] != nil) {
        NSString *obj = oneDict[[NSString stringWithFormat:@"__nsurlrequest_proto_prop_obj_%d", i]];
        if (obj != nil) {
            [oneDict setObject:obj forKey:[NSString stringWithFormat:@"$%d", i + k]];
            [oneDict removeObjectForKey:obj];
            archiveDict[@"$objects"][1] = oneDict;
        }
        i += 1;
    }
    if (oneDict[@"__nsurlrequest_proto_props"] != nil) {
        NSString *obj = oneDict[@"__nsurlrequest_proto_props"];
        [oneDict setObject:obj forKey:[NSString stringWithFormat:@"$%d", i + k]];
        [oneDict removeObjectForKey:@"__nsurlrequest_proto_props"];
        archiveDict[@"$objects"][1] = oneDict;
    }
    NSMutableDictionary *twoDict = [NSMutableDictionary dictionaryWithDictionary:archiveDict[@"$top"]];
    if (twoDict[@"NSKeyedArchiveRootObjectKey"] != nil) {
        [twoDict setObject:twoDict[@"NSKeyedArchiveRootObjectKey"] forKey:[NSString stringWithFormat:@"%@", NSKeyedArchiveRootObjectKey]];
        [twoDict removeObjectForKey:@"NSKeyedArchiveRootObjectKey"];
        archiveDict[@"$top"] = twoDict;
    }
    resultData = [NSPropertyListSerialization dataWithPropertyList:archiveDict format:NSPropertyListBinaryFormat_v1_0 options:NSPropertyListMutableContainers error:nil];
    return resultData;
}
```
用法是将从系统获取的resumeData传给getCorrectResumeData:(NSData *)resumeData函数，获取正确的resumeData。如果有小伙伴想要swift版本的代码，我的demo里有。[或者参考原贴](http://stackoverflow.com/questions/39346231/resume-nsurlsession-on-ios10)。
> 
# 实现下载进度和下载速度
1、下载进度其实很容易实现，只需要在代理下载的代理方法中操作就可以了，代码如下：
```
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite {
    self.currentWriten = totalBytesWritten;
    self.downloaderProgress = (float)totalBytesWritten / (float)totalBytesExpectedToWrite;
}
```
2、在做下载速度时，我没有参考网上的资料，似乎也很少，我没有查阅不得而知。感觉可以靠自己的知识储备实现，就按照自己小学学过的一个公式实现的：v = s / t，即速度 = 距离 / 时间，当然下载速度应该是：速度 = 下载量 / 时间，我们一般看到的下载速度都是1s内的下载量，因此我这开启了一个定时器，每隔一秒计算一下下载量，从而计算出下载速度，代码如下：
```
//self.currentWriten表示当前的下载数据量实时在下载进度的代理中更新，self.lastWritten表示上一秒的数据下载量，每秒更新一次
self.speed = self.currentWriten - self.lastWritten ;
```
这里直接使用的单位是b，用户可读的下载速度基本是kb/s、m/s,因此需要根据不同的情况转化一下，参考代码如下：
```cpp
```cpp
NSString *speedStr = nil;
if (speed >= 0 && speed < 1024) {
    //B
    speedStr = [NSString stringWithFormat:@"下载速度为：%ldb/s", (long)speed];
} else if (speed >= 1024 && speed < 1024 * 1024) {
    //KB
    speedStr = [NSString stringWithFormat:@"下载速度为：%.2lfkb/s", (long)speed / 1024.0];
} else if (speed >= 1024 * 1024) {
    //MB
    speedStr = [NSString stringWithFormat:@"下载速度为：%.2lfmb/s", (long)speed / 1024.0 / 1024.0];
}
NSLog(@"文件：%@的下载速度：%@", downloaderUrl,speedStr);
```
```
这个只是我的实现方案，小伙伴们若有更好地实现方案，还请指教，非常感激。
> 
# 实现下载数量的控制
只有一个文件需要下载时通常可以不用考虑对下载数量进行控制，但是我们遇到的基本是需要下载多个文件的情况，在移动设备资源有限的前提下，合理控制下载数量变得很重要，同时这里也将回应上文中的一个疑点。
设置一个属性来表示和控制最多同时下载几个文件
```
/**
 * 同时下载的最大的文件数量
 */
@property (assign, nonatomic) NSInteger maxDownloaderNum;
```
同时声明了3个方法分别控制下载的流程，
分别是新加入一个下载的方法
```
- (void)addDownloader:(ZYLSingleDownloader *)downloader isHand:(BOOL)isHand isControl:(BOOL)isControl{
    //首先判断是不是手动开启新的下载
    if (isHand) {
        //是手动,强行开启下载
        //判断是否达到最大下载数目
        if (self.downloadingArray.count < self.maxDownloaderNum) {
            //没有
            if (![self.downloadingArray containsObject:downloader]) {
                [self.downloadingArray addObject:downloader];
                [self.waitingDownlodArray removeObject:downloader];
            }
        } else {
            if (![self.downloadingArray containsObject:downloader]) {
                [self.downloadingArray addObject:downloader];
                [self.waitingDownlodArray removeObject:downloader];
            }
            //达到了
            //暂停最前面的正在下载
            ZYLSingleDownloader *firstDownloader = [self.downloadingArray firstObject];
            [self removeDownloader:firstDownloader isHand:isHand isControl:YES];
        }
        //开启下载
        if (isControl) {
            downloader.isHand = isHand;
            [downloader start];
        }
    } else {
        //不是手动
        if (self.downloadingArray.count < self.maxDownloaderNum) {
            //还没有达到最大下载数
            if (![self.downloadingArray containsObject:downloader]) {
                [self.downloadingArray addObject:downloader];
                [self.waitingDownlodArray removeObject:downloader];
            }
            //开启下载
            if (isControl) {
                downloader.isHand = isHand;
                [downloader start];
            }
        } else {
            //已经达到了最大的下载数
            //判断正在正在下载的数组中是否有此下载器
            if ([self.downloadingArray containsObject:downloader]) {
            } else {
                [self.waitingDownlodArray addObject:downloader];
                NSLog(@"达到最大下载数目，已经加入待下载数组");
            }
        }
    }
}
```
`注意：这里有2个参数需要解释下1、一个是isHand，表示需要操作的当前下载器是否是执行强制操作，比如，当我设置最多同时下载3个文件，此时有3个文件正在下载，而这里又添加了一个下载，此时有2种情况，一种情况是把新的下载器加入等待队列，当前面的下载器下载完成后开启下载，另一种情况是要首先下载新添加的下载器，这种情况就要移除一个正在下载的下载器了，所以需要这个参数表明如何操作当前下载器。2、另一个是isControl，表示操作当前下载器后是否需要执行对应下载任务，比如添加一个下载器后，我们需要启动下载，但是有可能需要在别处启动下载，也有可能就在添加后启动下载，所以这里需要一个参数表示如何操作。`
移除一个下载的方法
```
- (void)removeDownloader:(ZYLSingleDownloader *)downloader isHand:(BOOL)isHand isControl:(BOOL)isControl {
    __block BOOL isE = NO;
    [self.downloadingArray enumerateObjectsUsingBlock:^(ZYLSingleDownloader *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if ([obj.downloadUrl isEqualToString:downloader.downloadUrl]) {
            isE = YES;
            *stop = YES;
        }
    }];
    if (isE) {
        //存在
        if (downloader.downloaderProgress >= 1.0) {
            //已经下载完成
            [self.downloadingArray removeObject:downloader];
        } else {
            //还没有下载完成
            if (![self.waitingDownlodArray containsObject:downloader]) {
                //在下载器没有被删除的时候添加到等待下载数组
                if (downloader.downloaderState != ZYLDownloaderStateDeleted) {
                    [self.waitingDownlodArray addObject:downloader];
                } else if (downloader.downloaderState == ZYLDownloaderStateDeleted) {
                    if ([self.waitingDownlodArray containsObject:downloader]) {
                        [self.waitingDownlodArray removeObject:downloader];
                    }
                }
            }
            [self.downloadingArray removeObject:downloader];
        }
        if (isControl == YES) {
            downloader.isHand = isHand;
            [downloader cancelRorOtherDownloader];
        }
        if (isHand) {
            //是手动
            [self checkDownloadProgressExceptDownloader:downloader];
        } else {
            //不是手动
            //检查下载流程
            [self checkDownloadProgressExceptDownloader:nil];
        }
    } else {
        //不存在
        NSLog(@"正在下载的文件中不存在这个下载");
        if (downloader.downloaderState == ZYLDownloaderStateDeleted) {
            //检测等待数组中是否有此数据
            if ([self.waitingDownlodArray containsObject:downloader]) {
                [self.waitingDownlodArray removeObject:downloader];
            }
        }
    }
}
```
有了添加和移除还不够，我们往往需要在移除一个下载后检测等待队列里是否有需要下载的下载器，因此还需要一个检查下载流程的方法
```
- (void)checkDownloadProgressExceptDownloader:(ZYLSingleDownloader *)downloader {
    //判断正在下载的数组中是否有空缺
    if (self.downloadingArray.count < self.maxDownloaderNum) {
        //有空缺
        //检查等待下载的数组中是否有数据
        if (self.waitingDownlodArray.count > 0) {
            //有
            //寻找第一个需要下载的数据
            __block ZYLSingleDownloader *firstDownloader = nil;
            if (downloader == nil) {
                [self.waitingDownlodArray enumerateObjectsUsingBlock:^(ZYLSingleDownloader *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if (obj.isHand == NO) {
                        firstDownloader = obj;
                        *stop = YES;
                    }
                }];
            } else {
                [self.waitingDownlodArray enumerateObjectsUsingBlock:^(ZYLSingleDownloader *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                    if (![obj.downloadUrl isEqualToString:downloader.downloadUrl] && obj.isHand == NO) {
                        firstDownloader = obj;
                        *stop = YES;
                    }
                }];
            }
            if (firstDownloader == nil) {
                NSLog(@"没有找到需要开启的下载的任务");
            } else {
                firstDownloader.isHand = NO;
                if (firstDownloader.downloaderProgress > 0) {
                    [firstDownloader resumeisHand:firstDownloader.isHand];
                } else {
                    [firstDownloader start];
                }
                if (![self.downloadingArray containsObject:firstDownloader]) {
                    [self.downloadingArray addObject:firstDownloader];
                    [self.waitingDownlodArray removeObject:firstDownloader];
                }
            }
        } else {
            //没有
            NSLog(@"已经没有等待下载的数据了");
        }
    } else {
        //没有空缺
        NSLog(@"已经达到最大的同时下载数目");
    }
}
```
`注意1、以上3个方法中有一些地方看不明白，不必深究，这里是从demo中截取的片段，需要结合其他地方一起理解。大体明白每个方法的作用即可。2、检查下载流程的方法有一点需要解释下，这里的参数isHand还可以用于控制下载流程，比如一个下载的的isHand是YES表示这个下载器是用户想让他停止下载的，因此即使正在下载的数量没有达到最大限制，也不应该自动开启这个下载，同时刚刚停止的那个下载也不应该立刻就开启，否则有可能出现无法停止某个下载的bug。3、我这做这个功能是的时候一直在模拟器运行，没有什么问题，但是当我在真机运行时，会发现同时下载的数量有可能不是我设置的最大数量，并且当暂停一个下载的时候，会出现不自动开启另一个下载的问题，还经常会出现下载失败的问题，可是网络明明是好的。经过反复测试，发现在真机上系统最大允许同时开启3个下载器，并且我们在暂停一个下载的时候不能使用suspend，前面说过，suspend只是将当前下载挂起，下载线程并没有销毁，还在占用系统资源，因此当采用suspend暂停时，是有可能不会自动开启下一个下载的，这里全部换成使用cnacel暂停下载，继续下载时使用resumeData。系统规定我们最多同时开启3个下载线程，参考多个成熟且具有下载功能的APP，基本是单个下载，也就是同时只能下载一个，因此这里也建议大家使用单个下载，如有需求开启多个，不要超过3个。`
> 
# 下载器整体思路总结
前面分模块阐述了下载器的各个部分，在开发一个可用的下载器时，这些模块并不是独立的，而是协同合作，因此这里阐述和总结一下整个下载器的实现思路。
首先看分析图：
![](http://upload-images.jianshu.io/upload_images/1347742-9ceeb57824d9abec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下载器下载流程
1、ZYLDownloader是下载器的控制器，主要功能是协调各个单独的下载器ZYLSingleDownloader，控制下载器的下载流程，本身不负责下载、暂停、继续功能。而ZYLSingleDownloader负责下载、暂停和继续下载；
2、添加一个下载只有一种情况，就是这个下载是没有被添加过的新的下载，若添加过，执行继续下载操作；
3、继续下载略微复杂些，涉及的情况会多一些。第一种情况是执行常规的暂停，这是我们只需要在暂停的时候拿到resumeData，继续的时候传入即可。第二种情况是APP重启后，我们需要通过激活，下载的代理方法中获取resumeData，然后继续下载。第三种情况是网络中断后，无法从系统获取resumeData时，去本地读取自行创建的resumeData完成继续下载。这里在判断APP是哪种情况，应该如何继续下载有些复杂，也很容易出错，我的方法可以看demo，仅供参考，大家可以根据自己的情况自行判断。
`注意在开发中发现，当处于没有网络的情况下，APP重启后激活继续下载，会损坏本地的继续下载数据，导致即使获取了resumeData也无法完成继续下载，因此在程序里对网络环境进行了判断，采用的是AFN，当没有网络时不激活，网络恢复后才可以激活。`
综上关于下载器的下载部分基本讲完了，一些细枝末节的我并没有提及或者比较少提及，大家看demo应该可以看明白，都比较简单，看不明白也没关系，明白了难点和关键点，完全可以自行封装一个下载器。自己动手丰衣足食嘛，看别人的代码总有那么一点无奈和辛酸。
> 
# 存储下载信息
文件下载了，最主要的还是应用，我们需要的不仅仅是文件本身，还有文件的名称、类型、下载链接、下载进度等信息，便于我们展示给用户。我这里采用的是目前移动端最为先进的数据数据库realm，一方面由于realm简单易用，另一方面realm高效免费，这里就不赘述realm的使用了，相信很多小伙伴已经接触过了，还不太了解的可以参考官方的介绍，写得很详细，也有中文版本，已经没有太大必要去阅读第三方的解读了，官方对各种问题的解答也很详细，还有专门的论坛提供技术支持，传送门在此，一看便知：[realm官方文档](https://realm.io/docs/objc/latest/)。
常规的数据库操作无非是增、删、改、查，这里也不例外，4中需求都有，我这里单独声明了一个ZYLSingleDownloaderModel类用于数据库操作。
1、首先看增加，代码如下：
```
- (void)saveDownloaderInfoWithSingleDownloader:(ZYLSingleDownloader *)singleDownloader {
    //创建存储对象
    ZYLSingleDownloaderModel *model = [[ZYLSingleDownloaderModel alloc] init];
    model.downloadUrl = singleDownloader.downloadUrl;
    model.fileType = singleDownloader.fileType;
    model.filename = singleDownloader.filename;
    model.downloaderProgress = singleDownloader.downloaderProgress;
    //存储到数据库
    RLMRealm *realm = [RLMRealm defaultRealm];
    [realm beginWriteTransaction];
    [realm addOrUpdateObject:model];
    [realm commitWriteTransaction];
}
```
`注意这里在缓存信息的时候，注意不要将本地文件的下载路径缓存到数据库，这个是没有意义的，因为每次启动APP，为了保证安全，沙盒目录的文件路径都是变化的，也就是说你上次缓存的文件路径这次是不可用的，所以我们只需要缓存文件名和文件所在的文件夹，每次使用时实时获取沙盒目录的路径即可。`
2、删除，代码如下：
```
- (void)deleteDownloaderInfoWithDownloderUrl:(NSString *)downloaderUrl {
    __block BOOL isD = NO;
    __block ZYLSingleDownloader *downloaderModel = nil;
    [self.singleDownloaderArray enumerateObjectsWithOptions:NSEnumerationReverse usingBlock:^(ZYLSingleDownloader *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if ([downloaderUrl isEqualToString:obj.downloadUrl]) {
            //已经存在这个下载了
            isD = YES;
            downloaderModel = obj;
            *stop = YES;
        }
    }];
    if (isD) {
        //存在
        //判断下载器的下载状态，做出相应的处理
        [downloaderModel judgeDownloaderStateToHandel];
        //判断是否在数据库中
        if (downloaderModel.isExistInRealm == YES) {
            //存在
            //1️⃣数据源中删除数据
            __weak __typeof(self)(weakSelf) = self;
            [self.singleDownloaderArray enumerateObjectsWithOptions:NSEnumerationReverse usingBlock:^(ZYLSingleDownloader*  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj.downloadUrl isEqualToString:downloaderModel.downloadUrl]) {
                    [weakSelf.singleDownloaderArray removeObject:obj];
                    *stop = YES;
                }
            }];
            //2️⃣数据库中删除数据
            [self deleteDownloaderFromReaml:downloaderModel];
            //3️⃣从沙盒目录中删除文件（下载的文件、继续下载数据、未下载完成的数据）
            //①下载的文件
            NSString *localUrl = [self.directoryStr stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.%@", [ZYLTool encodeFilename:downloaderModel.downloadUrl], downloaderModel.fileType]];
            if ([self.fileManager fileExistsAtPath:localUrl]) {
                //存在则删除
                if (![self.fileManager removeItemAtPath:localUrl error:nil]) {
                    NSLog(@"删除下载的文件失败");
                }
            }
            //②继续下载的数据
            NSString *resumeDataStr = [self.resumeDirectoryStr stringByAppendingPathComponent:[@"resume_" stringByAppendingString:[ZYLTool encodeFilename:downloaderModel.downloadUrl]]];
            if ([self.fileManager fileExistsAtPath:resumeDataStr]) {
                NSData *tempData = [NSData dataWithContentsOfFile:resumeDataStr];
                NSString *XMLStr = [[NSString alloc] initWithData:tempData encoding:NSUTF8StringEncoding];
                NSRange tmpRange = [XMLStr rangeOfString:@"NSURLSessionResumeInfoTempFileName"];
                NSString *tmpStr = [XMLStr substringFromIndex:tmpRange.location + tmpRange.length];
                NSRange oneStringRange = [tmpStr rangeOfString:@"<string>"];
                NSRange twoStringRange = [tmpStr rangeOfString:@"</string>"];
                //记录tmp文件名
                downloaderModel.tmpFilename = [tmpStr substringWithRange:NSMakeRange(oneStringRange.location + oneStringRange.length, twoStringRange.location - oneStringRange.location - oneStringRange.length)];
                //存在则删除
                if (![self.fileManager removeItemAtPath:resumeDataStr error:nil]) {
                    NSLog(@"删除继续下载的数据失败");
                } else {
                    //删除成功
                    //③删除未下载完成的数据
                    NSString *unDownloaderStr = [[[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0] stringByAppendingPathComponent:@"ZYLUnDownloads"] stringByAppendingPathComponent:downloaderModel.tmpFilename];
                    if ([self.fileManager fileExistsAtPath:unDownloaderStr]) {
                        if (![self.fileManager removeItemAtPath:unDownloaderStr error:nil]) {
                            NSLog(@"删除未下载完成的数据失败");
                        }
                    }
                }
            }
        } else {
            //不存在
            __weak __typeof(self)(weakSelf) = self;
            [self.singleDownloaderArray enumerateObjectsUsingBlock:^(ZYLSingleDownloader* _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj.downloadUrl isEqualToString:downloaderModel.downloadUrl]) {
                    [weakSelf.singleDownloaderArray removeObject:obj];
                    if ([self.waitingDownlodArray containsObject:obj]) {
                        [self.waitingDownlodArray removeObject:obj];
                    }
                    *stop = YES;
                }
            }];
            NSLog(@"数据库中不存在这个下载，无法在数据库中删除");
        }
    } else {
        //不存在
        NSLog(@"不存在这个下载，无法删除");
    }
}
- (void)deleteDownloaderFromReaml:(ZYLSingleDownloader *)downloader {
    //创建存储对象
    for (ZYLSingleDownloaderModel *model in self.allModels) {
        if ([model.downloadUrl isEqualToString:downloader.downloadUrl]) {
            //删除对象
            RLMRealm *realm = [RLMRealm defaultRealm];
            [realm beginWriteTransaction];
            [realm deleteObject:model];
            [realm commitWriteTransaction];
        }
    }
}
```
`注意小伙伴看我的删除是可能会觉得怎么如此复杂，因为这里需要判断要删除的数据是否在数据中存在、判断当前的下载状态、在数据库中删除的同时也要在数据源和本地删除关于当前下载的一切信息，还有终止当前的下载线程，保证腾出资源让下一个下载器可以开启。`
3、修改数据，代码如下：
```
- (void)updateDownloaderInfoWithDownloderUrl:(NSString *)downloaderUrl withFilename:(NSString *)filename fileType:(NSString *)fileType {
    //判断数据源中是否有此数据
    __block BOOL isE = NO;
    __block ZYLSingleDownloader *downloaderModel = nil;
    [self.singleDownloaderArray enumerateObjectsUsingBlock:^(ZYLSingleDownloader *  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if ([obj.downloadUrl isEqualToString:downloaderUrl]) {
            isE = YES;
            downloaderModel = obj;
            *stop = YES;
        }
    }];
    if (isE) {
        //存在
        //判断在数据库中是否存在
        if (downloaderModel.isExistInRealm) {
            //存在
            NSString *localUrl = [self.directoryStr stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.%@", [ZYLTool encodeFilename:downloaderModel.downloadUrl], downloaderModel.fileType]];
            //更新数据源
            if (filename != nil && ![filename isEqualToString:@""]) {
                downloaderModel.filename = filename;
            }
            if (fileType != nil && ![fileType isEqualToString:@""]) {
                downloaderModel.fileType = fileType;
            }
            //更新数据库
            [self saveDownloaderInfoWithSingleDownloader:downloaderModel];
            //判断是否下载完成
            if (downloaderModel.downloaderProgress >= 1.0) {
                //下载完成了
                //更新本地的下载好的文件的文件名
                //判断本地文件是否存在
                if ([self.fileManager fileExistsAtPath:localUrl]){
                    //根据新的文件信息更新文件名
                    NSString *newLocalUrl = [self.directoryStr stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.%@", [ZYLTool encodeFilename:downloaderModel.downloadUrl], downloaderModel.fileType]];
                    NSError *error = nil;
                    BOOL isS = [self.fileManager moveItemAtPath:localUrl toPath:newLocalUrl error:&error];
                    if (!isS) {
                        NSLog(@"修改本地下载的文件信息失败：%@", error);
                    }
                } else {
                    //没有本地文件
                    NSLog(@"没有本地缓存文件，无法本地文件");
                }
            } else {
                //没有下载完成
            }
        } else {
            //不存在
            //仅仅更新数据源
            if (filename != nil && ![filename isEqualToString:@""]) {
                downloaderModel.filename = filename;
            }
            if (fileType != nil && ![fileType isEqualToString:@""]) {
                downloaderModel.fileType = fileType;
            }
        }
    } else {
        //不存在
        NSLog(@"不存在这个下载器，无法更新数据");
    }
}
```
`注意修改数据和增加数据最终调用的realm代码是一样的，realm里有addOrUpdateObject方法，既可以添加也可以更新，可以避免很多bug，建议使用此方法。`
4、查看数据，代码如下
```
- (ZYLSingleDownloaderModel *)getDownloaderInfoWithDownloaderUrl:(NSString *)downloaderUrl {
    //首先判断下载连接是否在数据数组中
    ZYLSingleDownloaderModel *targetModel = [[ZYLSingleDownloaderModel alloc] init];
    __block BOOL isD = NO;
    __block ZYLSingleDownloader *downloaderModel = nil;
    [self.singleDownloaderArray enumerateObjectsUsingBlock:^(ZYLSingleDownloader *_Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if ([downloaderUrl isEqualToString:obj.downloadUrl]) {
            //已经存在这个下载了
            downloaderModel = obj;
            isD = YES;
        }
    }];
    if (isD) {
        //存在
        //判断是否存在于数据库中
        if (downloaderModel.isExistInRealm == YES) {
            //存在
            //判断这个文件是否下载完成
            if (downloaderModel.downloaderProgress >= 1.0) {
                //下载完成
                //判断沙盒目录是否存在此文件
                NSString *localUrl = [self.directoryStr stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.%@", [ZYLTool encodeFilename:downloaderModel.downloadUrl], downloaderModel.fileType]];
                if ([self.fileManager fileExistsAtPath:localUrl]) {
                    //存在
                    targetModel.localUrl = localUrl;
                    targetModel.downloadUrl = downloaderModel.downloadUrl;
                    targetModel.filename = downloaderModel.filename;
                    targetModel.fileType = downloaderModel.fileType;
                    targetModel.downloaderProgress = downloaderModel.downloaderProgress;
                    targetModel.isExistInRealm = YES;
                    return targetModel;
                } else {
                    //不存在
                    NSLog(@"沙盒目录没有对应的文件");
                    targetModel.localUrl = nil;
                    targetModel.downloadUrl = downloaderModel.downloadUrl;
                    targetModel.filename = downloaderModel.filename;
                    targetModel.fileType = downloaderModel.fileType;
                    targetModel.downloaderProgress = downloaderModel.downloaderProgress;
                    targetModel.isExistInRealm = YES;
                    return targetModel;
                }
            } else {
                //未下载完成
                NSLog(@"这个下载还没有完成");
                targetModel.localUrl = nil;
                targetModel.downloadUrl = downloaderModel.downloadUrl;
                targetModel.filename = downloaderModel.filename;
                targetModel.fileType = downloaderModel.fileType;
                targetModel.downloaderProgress = downloaderModel.downloaderProgress;
                targetModel.isExistInRealm = YES;
                return targetModel;
            }
        } else {
            //不存在
            NSLog(@"这个下载还没有开始");
            targetModel.localUrl = nil;
            targetModel.downloadUrl = downloaderModel.downloadUrl;
            targetModel.filename = downloaderModel.filename;
            targetModel.fileType = downloaderModel.fileType;
            targetModel.downloaderProgress = downloaderModel.downloaderProgress;
            targetModel.isExistInRealm = NO;
            return targetModel;
        }
    } else {
        //不存在
        NSLog(@"不存在这个下载");
        return nil;
    }
}
```
上面就是关于数据库的操作，比较简单，小伙伴们看看就明白了。
> 
# 尾巴
[demo下载地址](https://github.com/yueshangfeng/ZYLDownloader.git)
下载器刚写好，还是会有一些问题，不过在大多数情况下是可以正常运行的。小伙伴若发现什么问题，还请及时指正，感激不尽.
文／张云龙（简书作者）
原文链接：http://www.jianshu.com/p/b4edfa0b71d8
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
