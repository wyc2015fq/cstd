
# windows下关于Objective-C开发环境的配置 - 高科的专栏 - CSDN博客

2013年09月22日 14:33:54[高科](https://me.csdn.net/pbymw8iwm)阅读数：8814


最近IOS一直很火，也想学习以以下OC开发，一般装个虚拟机，然后装个mac，我相信大多数人的机子跑不起来或者很卡，所以之前借鉴了一个文章，就是关于在windows下配置OC开发环境，这里我把自己的安装说一下，顺便以后查阅的时候方便查找。
1.首先要下载这4个东西：[http://www.gnu.org/software/gnustep/experience/Windows.html](http://www.gnu.org/software/gnustep/experience/Windows.html)
2.然后还有codeblocks:http://www.codeblocks.org/downloads/binaries
3.下面就是开始配置编译器。开启codeblocks后
进入*Settings->Compiler and debugger...，*选择*GNU GCC Compiler*编译器，按“Copy”按钮，并重新命名为*“**GNUstep
 MinGW Compiler**“*并保存。
![codeblocks：windows下关于Objective-C开发环境的配置](http://s8.sinaimg.cn/mw690/a67a99d7td4285a091ca7&690)
可能会有疑问为什么copy，实际上就是新建了一个编译器。没什么意思。
4，之后进入*Other Options*分页，录入：
-fconstant-string-class=NSConstantString -std=c99
![codeblocks：windows下关于Objective-C开发环境的配置](http://s10.sinaimg.cn/mw690/a67a99d7td4286b7984d9&690)
5，连接器设置 Linker stettings
在连接库（Link Libraries）中添加两个文件，如图。
它们在C:\IOS开发\GNUstep\GNUstep\System\Library\Libraries文件夹里找到（在自己的GNUstep安装路径中找。）
libgnustep-base.dll.a
libobjc.dll.a
![codeblocks：windows下关于Objective-C开发环境的配置](http://s16.sinaimg.cn/mw690/a67a99d7td42876aff13f&690)
6，指定搜索目录Search directories
Compiler（编译器）设置为C:\IOS开发\GNUstep\GNUstep\System\Library\Headers；
![codeblocks：windows下关于Objective-C开发环境的配置](http://s6.sinaimg.cn/mw690/a67a99d7td4287f14bf85&690)
Linker（连接器）设置为C:\IOS开发\GNUstep\GNUstep\System\Library\Libraries；
![codeblocks：windows下关于Objective-C开发环境的配置](http://s10.sinaimg.cn/mw690/a67a99d7t7b9da76565f9&690)
Toolchain里面设置为：C:\IOS开发\GNUstep（设置为GNUstep路径）
这一步网上好多blog中都未涉及，如果没有设置可能会引起在run时会出现error：no such file or ...后面这个词记不清了。）
![codeblocks：windows下关于Objective-C开发环境的配置](http://s3.sinaimg.cn/mw690/a67a99d7td4288eccf0b2&690)
7.添加Objective-C文件类型支持
1）进入*Settings->Environment...**，*选择*Files extension handling*添加*.m。如图：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s10.sinaimg.cn/mw690/a67a99d7td428a6036979&690)
2） 进入*Project->Project tree->Edit file types & categories...*，在*Sources*, 下面添加**.m*到文件类型列表中。如图：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s6.sinaimg.cn/mw690/a67a99d7td428b2769715&690)
位置放前后无所谓。
3） 进入*Settings->Editor...*，选择*Syntax highlighting*，点击“*Filemasks..*..”按钮，在弹出框尾部添加**.m*到文件类型。如图：![codeblocks：windows下关于Objective-C开发环境的配置](http://s1.sinaimg.cn/mw690/a67a99d7td428be74e600&690)
4）点击“*Keywords*...”按钮 (紧靠Filemasks...按钮) 添加下面Object-C的关键字到*Edit Keywords*列表中。
*@interface @implementation @end @class @selector @protocol @public @protected @private id BOOL YES NO SEL nil NULL self @protocol*
如图：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s11.sinaimg.cn/mw690/a67a99d7td428c9607aea&690)
8，现在为止，环境就算配置好了。下面就是验证一下配置的正确性。*重启codeblocks。*
首先，新建一个工程，选择*File->New->Project…,*会出现一个工程类型窗口，选择Console Application。
![codeblocks：windows下关于Objective-C开发环境的配置](http://s7.sinaimg.cn/mw690/a67a99d7td428dbdc2e56&690)
之后出现：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s6.sinaimg.cn/mw690/a67a99d7td428dfc5d2c5&690)
随便选个就行了。
随便给自己的工程起个名字：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s14.sinaimg.cn/mw690/a67a99d7t7b9db077dafd&690)
之后进行编译器选择，选择自己刚才调试的compiler：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s4.sinaimg.cn/mw690/a67a99d7td428e9903173&690)
9，把自己所建工程source中的main.cpp改成main.m.
![codeblocks：windows下关于Objective-C开发环境的配置](http://s12.sinaimg.cn/mw690/a67a99d7td428ff9f9a1b&690)
10,双击打开main.m把里面原来的代码改为objective-c的代码：
\#import
int main(int argc,const char *argv[]){
NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
NSLog(@"[%@",@"hello](mailto:%25@' rel=)world");
[pool drain];
return 0;
}
![codeblocks：windows下关于Objective-C开发环境的配置](http://s3.sinaimg.cn/mw690/a67a99d7td429060a90c2&690)
11，运行，进入Build->Build and Run或直接按F9.如果运行成功即会出现：
![codeblocks：windows下关于Objective-C开发环境的配置](http://s3.sinaimg.cn/mw690/a67a99d7td429106077d2&690)




