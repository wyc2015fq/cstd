# iOS crash 解析崩溃日志 - weixin_33985507的博客 - CSDN博客
2017年03月13日 12:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
![1749075-3f14ac58b5f7e102.png](https://upload-images.jianshu.io/upload_images/1749075-3f14ac58b5f7e102.png)
 解析崩溃日志
 一  获取crash
1.用户把设备连接到电脑上，打开xcode－window，选中Devices－当前连接设备－Device Log，就可以查看所有当前设备的crash log，这个时候打开每一份crash的时候，发现这些文件的部分地址都会被转换成，类名，方法名和行号等。设备上的日志只用刚刚查看过都会被同步到organizer种,在LIBRARY下的Device Log可以查看。
2.如果你的应用已经上架，那么开发者可以通过iTunes Connect（Manage Your Applications - View Details - Crash Reports）获取用户的crash日志。不过这并不是100%有效的，而且大多数开发者并不依赖于此，因为这需要用户设备同意上传相关信息，详情可参见iOS: Providing Apple with diagnostics and usage information摘要。
以上两种方法 Xcode8.3.2之后 我没找到
3.在Xcode - window -Device - view device logs
![1749075-c768df6fa37f0a7d.png](https://upload-images.jianshu.io/upload_images/1749075-c768df6fa37f0a7d.png)
二：找到相对应的.dSYM文件，在哪呢？就在这个目录里： ~/Library/Developer/Xcode/Archives/ 或者然后打开打的包，xcarchive显示包内容 在子文件里找到 .dSYM 和 symbolicatecrash
将.dSYM      .app     symbolicatecrash   .crash  四个东西放到一个新建文件夹
1.查看 xx.app 文件的 UUID，terminal 中输入命令 ：
dwarfdump  --uuid xx.app/xx (xx代表你的项目名)
2.查看 xx.app.dSYM 文件的 UUID ，在 terminal 中输入命令：
dwarfdump --uuid xx.app.dSYM
3.crash 文件内第一行 Incident Identifier 就是该 crash 文件的 UUID
![1749075-4dc9ca09002d23e4.png](https://upload-images.jianshu.io/upload_images/1749075-4dc9ca09002d23e4.png)
注意： 三者UUID一致方可进行下一步操作
终端找到symbolicatecrash路径（Xcode8.2.1-8.3.3）
/Applications/Xcode.app/Contents/SharedFrameworks/DVTFoundation.framework/Versions/A/Resources/symbolicatecrash
会得到的symbolicatecrash的路径， 接下来前往文件夹路径拷贝symbolicatecrash
到这个目录去，把这个工具拷贝到和上述文件同一个目录
![1749075-d6066b9370358573.png](https://upload-images.jianshu.io/upload_images/1749075-d6066b9370358573.png)
cd进入新建文件夹
> 
**  MacBook-Pro:~ tianmingfu$ cd /Users/denmeiho/Desktop/Crash**
./symbolicatecrash    crash路径    dSYM路径 > xx.crash
> 
MacBook-Pro:Crash tianmingfu$ ./symbolicatecrash /Users/denmeiho/Desktop/Crash/MobileBankProject.crash  /Users/denmeiho/Desktop/Crash/MobileBankProject.app.dSYM >>result.crash
可能会报错
> 
Error: "DEVELOPER_DIR" is not defined at ./symbolicatecrash line 69.
继续在终端输入   改变环境变量
> 
export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer/
再次执行./symbolicatecrash    crash路径  dSYM路径  >  result.crash
最终会拿到解析出来的result.crash
注意： 如果命令不被允许可以 在  su - root 下运行
分析前：
> 
Last Exception Backtrace:
(0x1937a21c0 0x1921dc55c 0x1937a2094 0x19422f79c 0x199972dc4 0x1999720a4 0x19972c1bc 0x1002004a0 0x1002006b0 0x1000fb1ac 0x1002e6e70 0x1002e62e8 0x10017c2b8 0x1001173d8 0x1001145dc 0x10010d370 0x10011bfd4 0x19262d200 0x19262d1c0 0x192631d6c 0x19374ff2c 0x19374db18 0x19367c048 0x195102198 0x1996612fc 0x19965c034 0x1003699d4 0x1926605b8)
Thread 0 name:  Dispatch queue: com.apple.main-thread
Thread 0 Crashed:
0   libsystem_kernel.dylib        	0x0000000192772014 0x192753000 + 126996
1   libsystem_pthread.dylib       	0x000000019283a450 0x192835000 + 21584
2   libsystem_c.dylib             	0x00000001926e63e0 0x192683000 + 406496
3   libc++abi.dylib               	0x00000001921b12d4 0x1921b0000 + 4820
4   libc++abi.dylib               	0x00000001921cecc0 0x1921b0000 + 126144
5   libobjc.A.dylib               	0x00000001921dc844 0x1921d4000 + 34884
6   libc++abi.dylib               	0x00000001921cb66c 0x1921b0000 + 112236
7   libc++abi.dylib               	0x00000001921caf84 0x1921b0000 + 110468
8   libobjc.A.dylib               	0x00000001921dc690 0x1921d4000 + 34448
9   CoreFoundation                	0x00000001937a2094 0x193673000 + 1241236
10  Foundation                    	0x000000019422f79c 0x194181000 + 714652
11  UIKit                         	0x0000000199972dc4 0x1995e6000 + 3722692
12  UIKit                         	0x00000001999720a4 0x1995e6000 + 3719332
13  UIKit                         	0x000000019972c1bc 0x1995e6000 + 1335740
14  MobileBankProject             	0x00000001002004a0 0x1000d8000 + 1213600
15  MobileBankProject             	0x00000001002006b0 0x1000d8000 + 1214128
16  MobileBankProject             	0x00000001000fb1ac 0x1000d8000 + 143788
17  MobileBankProject             	0x00000001002e6e70 0x1000d8000 + 2158192
18  MobileBankProject             	0x00000001002e62e8 0x1000d8000 + 2155240
19  MobileBankProject             	0x000000010017c2b8 0x1000d8000 + 672440
20  MobileBankProject             	0x00000001001173d8 0x1000d8000 + 259032
21  MobileBankProject             	0x00000001001145dc 0x1000d8000 + 247260
22  MobileBankProject             	0x000000010010d370 0x1000d8000 + 217968
23  MobileBankProject             	0x000000010011bfd4 0x1000d8000 + 278484
24  libdispatch.dylib             	0x000000019262d200 0x19262c000 + 4608
25  libdispatch.dylib             	0x000000019262d1c0 0x19262c000 + 4544
分析后：
> 
Last Exception Backtrace:
0   CoreFoundation                	0x1937a21c0 0x193673000 + 1241536
1   libobjc.A.dylib               	0x1921dc55c 0x1921d4000 + 34140
2   CoreFoundation                	0x1937a2094 0x193673000 + 1241236
3   Foundation                    	0x19422f79c 0x194181000 + 714652
4   UIKit                         	0x199972dc4 0x1995e6000 + 3722692
5   UIKit                         	0x1999720a4 0x1995e6000 + 3719332
6   UIKit                         	0x19972c1bc 0x1995e6000 + 1335740
7   MobileBankProject             	0x1002004a0 -[CPMajorManager inRootController:] (CPMajorManager.m:130)
8   MobileBankProject             	0x1002006b0 __30-[CPMajorManager checkVersion]_block_invoke (CPMajorManager.m:147)
9   MobileBankProject             	0x1000fb1ac __53-[CPPlugin registPluginWithObject:withData:complete:]_block_invoke (CPPlugin.m:68)
10  MobileBankProject             	0x1002e6e70 __31-[CPVersionPlugin checkVersion]_block_invoke (CPVersionPlugin.m:34)
11  MobileBankProject             	0x1002e62e8 __37-[CSIICheckVersionUpdate requestMenu]_block_invoke (CSIICheckVersionUpdate.m:334)
12  MobileBankProject             	0x10017c2b8 -[CSIIBusinessLogic transactionSucceeded:] (CSIIBusinessLogic.m:286)
13  MobileBankProject             	0x1001173d8 -[CSIIBusinessTransaction disposeReturnCode:] (CSIIBusinessTransaction.m:706)
14  MobileBankProject             	0x1001145dc __54-[CSIIBusinessTransaction initWithTransaction:caller:]_block_invoke (CSIIBusinessTransaction.m:378)
15  MobileBankProject             	0x10010d370 __79-[CSIIMADPNetworkHttpClient initWithTransaction:onSuccessBlock:onFailureBlock:]_block_invoke (CSIIMADPNetworkHttpClient.m:109)
16  MobileBankProject             	0x10011bfd4 __72-[CSIIMADPAFHTTPRequestOperation setCompletionBlockWithSuccess:failure:]_block_invoke.143 (CSIIMADPAFHTTPRequestOperation.m:283)
（http://site.baidu.com）
