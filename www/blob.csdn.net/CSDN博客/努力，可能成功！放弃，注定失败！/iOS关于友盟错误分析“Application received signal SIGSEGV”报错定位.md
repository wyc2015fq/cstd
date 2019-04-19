# iOS关于友盟错误分析“Application received signal SIGSEGV”报错定位 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 11:00:01[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：128标签：[iOS关于友盟错误分析“Application receiv](https://so.csdn.net/so/search/s.do?q=iOS关于友盟错误分析“Application receiv&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
错误信息：
Application received signal SIGSEGV
错误详情：
![](https://upload-images.jianshu.io/upload_images/2285652-6756518095bd2178.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/725/format/webp)
错误详情.png
错误分析：
SIGSEGV和SIGBUS一般是由于访问已被释放的内存或者调用不存在的方法所致。
错误定位：
![](https://upload-images.jianshu.io/upload_images/2285652-d57c2ed965dc9989.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/650/format/webp)
单条错误详情.png
XingYunGlobalMeeting -- 应用名称
0x10020d274 -- 崩溃时的调用方法的16进制函数地址
XingYunGlobalMeeting + 2151028 -- 文件的地址以及方法所在的行的位置
接下来就是本文的重点，将这条“密文”解析成“明文”了。
首先，要找到该项目发布时Archive编译出来的.xcarchive文件。这个文件包含了.DSYM文件，可以通过XCode->菜单栏中的Window->Organizer，打开Archives列表，找到对应的文件，点击右键Show in Finder。
![](https://upload-images.jianshu.io/upload_images/2285652-c4dc773f6d54b63d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
通过Organizer找到.xcarchive文件.png
![](https://upload-images.jianshu.io/upload_images/2285652-beb5046b5795bd41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
在Finder中显示.xcarchive文件.png
![](https://upload-images.jianshu.io/upload_images/2285652-cf87006abcffc492.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/921/format/webp)
在Finder里所要的.xcarchive文件.png
打开该文件，选中该文件，点击右键 -> 显示包内容。
![](https://upload-images.jianshu.io/upload_images/2285652-3ea60c53e8381f89.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/656/format/webp)
xcarchive显示包内容.png
说明：正常的话（该版本只Archive一次的话），包里面dSYMs文件夹里只有一个以项目名称命名的.app.dSYM文件（如上图中的XingYunGlobalMeeting.app.dSYM文件）。如果同一版本重新Archive（即多次Archive）的话，会有多个.dSYM文件，此时我们就要根据友盟错误分析错误详情里的倒数第五行的dSYM UUID确定对应的.dSYM文件。
![](https://upload-images.jianshu.io/upload_images/2285652-c2d5e55eceb71f18.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/399/format/webp)
根据UUID确定对应的.dSYM文件.png
然后就要通过这个文件定位错误信息了，共有三种方法：
方法一：使用xcrun atos命令
【说明：最靠谱！推荐使用该方法，特别是该项目的该版本重复Archive的话（即上一步骤有多个.dSYM文件）的话，下面的两种方法可能不能使用】
1、选中上一步骤确定的对应.dSYM文件，点击右键显示包文件，按照路径->Contents->Resources->DWARF，找到以项目名命名的文件。
![](https://upload-images.jianshu.io/upload_images/2285652-22937914a2be875c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/811/format/webp)
显示.dSYM文件中的包文件.png
2、打开终端Terminal，输入 atos -o ，然后把上一步中定位的文件拖入终端，补入文件路径，空格，输入友盟错误详情中单条错误信息里的16进制函数地址（如：0x100037da4）
完整命令：
XingYundeMac-mini:~ xingyun$ atos -o /Users/xingyun/Library/Developer/Xcode/Archives/2017-03-20/XingYunGlobalMeeting\ 2017-3-20\ 下午7.20.xcarchive/dSYMs/D29F668F-D4A2-353E-97DC-CD9BFC78695E.dSYM/Contents/Resources/DWARF/XingYunGlobalMeeting 0x100037da4
3、终端返回我们看得懂得的“明文”错误信息:
__41-[MSSBrowseCollectionViewCell createCell]_block_invoke (in XingYunGlobalMeeting) (MSSBrowseCollectionViewCell.m:37)
发生崩溃的方法：-[MSSBrowseCollectionViewCell createCell]_block_invoke
崩溃所在源码文件：MSSBrowseCollectionViewCell.m
发生崩溃的方法在源文件中的行号：37
![](https://upload-images.jianshu.io/upload_images/2285652-468a800915a62b41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/587/format/webp)
方法一atos命令解析错误.png
方法二：使用dwarfdump命令
1、可先将.app.dSYM文件拷贝到一目录下备用，如和崩溃日志放在一个目录下。
2、打开终端，cd到该目录下，执行命令：
$ dwarfdump --arch=arm64 --lookup=[崩溃日志上项目对应的某崩溃地址] [dSYM文件的绝对路径]
方法三：可视化工具dSYMTools
下载链接（源码）：[https://github.com/answer-huang/dSYMTools](https://link.jianshu.com?t=https://github.com/answer-huang/dSYMTools)
下载后在Xcode中运行，然后按照以下使用步骤进行：
1、将打包发布软件时的xcarchive文件拖入软件窗口内的任意位置(支持多个文件同时拖入，注意：文件名不要包含空格)
2、选中任意一个版本的xcarchive文件，右边会列出该xcarchive文件支持的CPU类型，选中错误对应的CPU类型。
3、对比错误给出的UUID和工具界面中给出的UUID是否一致。
4、将错误地址以及 Slide Address 输入工具的文本框中，点击分析。
![](https://upload-images.jianshu.io/upload_images/2285652-7204fa926c234a20.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/521/format/webp)
dSYMTools.png
方法四：友盟错误分析工具
1、[下载错误分析工具](https://link.jianshu.com?t=http://dev.umeng.com/files/download/umcrashtool.zip)
，并解压zip得到umcrashtool文件，可将umcrashtool与已下载的xxx.csv文件放入同一目录下。
2、第二步 在terminal中运行umcrashtool命令，参数为错误分析的.csv文件绝对路径，如下：sanzhang$ ./umcrashtool [absolutely_path_of_csv_file] 将umcrashtool与错误分析.csv文件放入同一目录下
![](https://upload-images.jianshu.io/upload_images/2285652-9baf718dba28c29e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/601/format/webp)
csv文件和umcrashtool放在同一目录下.png
3、在terminal中运行umcrashtool，提示如下: Usage: umcrashtool [export-file-path]，定位后的代码及行数会写入错误分析-symbol.csv文件，与原文件在同一目录下。用工具打开新生成的xxx-symbol.csv文件，便可查看错误发生的源码文件及行数。
*注：如果错误分析没有成功，请先确保对应的 xxx.dSYM 文件在 ~/Library/Developer/Xcode/ 或该路径的子目录下。（对于每一个产品发布时archive操作会将dsym文件存放到~/Library/Developer/Xcode/Archives路径下，因此建议保留该路径下的文件，以便后续用工具分析错误。）*
作者：MeaningfulLiu
链接：https://www.jianshu.com/p/59eb849b0486
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
