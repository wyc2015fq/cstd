# CornerStone的使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月16日 11:50:06[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：25373
> 
俗话说：“工欲善其事必先利其器”；
对于我们程序员来说，不管你是大神，还是小鱼小虾，进入公司之后，都用过源码管理工具，不然你就不是一个合格的程序员，现在各个公司用于源码管理工具通常有下面两种：
第一种，git：使用git的公司应该是最多的。
第二种，svn：目前我们公司使用的就是svn。
当然，他们两者的区别，在这里就不做解释了，一般选择用什么源码管理工具，一般会是整个项目的负责人来选择。
在iOS开发中，最好用的svn的Mac版源码管理工具就是Cornerstone，下面介绍CornerStone的使用：
一、安装
CornerStone是收费，如果是公司需要使用到，建议让公司购买软件使用版权，支持正版，当然网上也是有破解版本的。
二、配置SVN
![](http://upload-images.jianshu.io/upload_images/588630-8d4bf8bbbc0a28d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
打开Cornerstone，点击“+”号，添加一个仓库。
![](http://upload-images.jianshu.io/upload_images/588630-c58c78cf39769e4e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
`https://192.168.1.11:443/svn/FXBest`
1、Protocol：分为HTTP和HTTPS，如果公司给你服务器地址是HTTPS的话，就选择HTTPS，反之，就选择HTTP。
2、Server：填写服务器地址，例如：
```cpp
```cpp
192.168.1.11
```
```
3、Port：服务器地址中端口号，当然有时候是不需要填写的，使用默认的就可以了，例如：
```cpp
```cpp
192.168.1.11:443
```
```
4、Repository path：svn仓库的访问路径
5、NickName：仓库显示的名称
6、用户名
7、密码
ps：
1、填写好之后，会自动生成，这样的一串路径。如果你填写完之后不是这种svn://用户名@主机地址:端口号/路径的格式，则说明填写有误。例如：
`https://michael@192.168.1.11:443/svn/iOS`
2、小技巧，一般进入公司，项目负责人会给你开svn的账号，然后会把svn的路径发给你，拿到这串路径，直接复制一下，然后切换到CornerStone，发现svn的路径会自动填充到CornerStone上面。
三、基本使用
1、上传项目到远端仓库repository
可以直接将文件或者文件夹拖到repository的子文件夹中，或是选择软件上方的Import按钮上传，会弹出选项填写所在位置及名称，然后选择Import即可。
![](http://upload-images.jianshu.io/upload_images/588630-58d24a6615484248.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
2、下载项目
![](http://upload-images.jianshu.io/upload_images/588630-cdb6d44ad35cf50c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
一般分为：Export和Check Out
他们的区别在于，Export后的项目不会与repository中的源文件相关联，是一个独立的版本，而Check Out下来的文件会创建一个working copy，此文件与repository中源文件相关联，当有别人修改或是自己修改时，working copy会显示修改数量，白色数量为别人修改数量，灰色数量为自己修改数量，所以如果你是项目中的开发人员，可以选择check out，如果只是下载查看，不希望自己的修改影响到整个项目，最好是选择Export。
3、版本管理
每一次提交会创建一个新版本，在repository中会保存所有历史版本，可通过修改人及提交信息进行检索版本，所以用svn开发可以很好的控制项目出现不可解决及未知bug时代码的修复问题，例如：
![](http://upload-images.jianshu.io/upload_images/588630-2899e643efd55075.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
ps：注意事项：
1、先更新后提交，当你看到你check out下来的项目有白色圈时，说明有人已经提交了代码，这个时候，你应该先update一下，直至working copy不再显示白色圈，然后运行一下项目，确保项目没有冲突或者丢失的文件，然后在commit自己的代码。否则，会造成项目中出现多处冲突或者覆盖掉别人提交的代码。
2、在项目开发阶段，最好每次开发完一个独立的功能，或者在测试阶段，解决一个bug之后再提交代码到svn，不要连续多次重复提交，造成版本过多过杂。并且每次提交时务必填写提交信息，方便查看历史版本。
小技巧：
1、静态文件上传。在项目中引入第三库，在本地运行项目，发现没有任何问题，但是提交到svn，其他人update之后，在运行项目发现报错，很有可能是静态文件没有上传到svn造成的。
![](http://upload-images.jianshu.io/upload_images/588630-77a24a91a542e166.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
屏幕快照 2016-07-16 下午12.02.16.png
选择Preferences
![](http://upload-images.jianshu.io/upload_images/588630-d2d2791b16c1d99b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
勾选Use default global ignores，删除.a后，点击save。
然后，会发现check out下来的项目中会有这些问号的文件存在(下面的图只是为了举例使用)，把这些带有问号的文件commit到svn，问题就解决了。
![](http://upload-images.jianshu.io/upload_images/588630-8f4104c6a273c470.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
四、CornerStone常见的图标含义
![](http://upload-images.jianshu.io/upload_images/588630-303b290c0b2c3eac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
如果，你想working copy下来的项目中做了修改，那么在修改之后的文件后面会有一个“M”的标志。
![](http://upload-images.jianshu.io/upload_images/588630-d5a606137ba9126f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
如果，你想working copy下来的项目中新添加一个文件，那么新添加之后的文件后面会有一个“？”的标志，此时你需要点击add，添加到working copy下来的项目中，然后在commit。
![](http://upload-images.jianshu.io/upload_images/588630-8b6269e6f34ed652.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
![](http://upload-images.jianshu.io/upload_images/588630-9bd6ffcba52705ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
如果，你想working copy下来的项目中删除一个文件，那么删除之后的文件后面会有一个“D”的标志。
如果，你想working copy下来的项目中A和B同时修改了同一行代码然后commit到svn，那么冲突之后的文件后面会有一个“C”的标志。
ps：原则上同一组开发人员最好不要在同一文件中进行操作，但有时候必须去其他文件中进行操作，或者是误操作，如果同时多人在同一文件的同一位置修改代码，后提交的人会出现版本冲突文件，一般会有三个同样名称不同后缀的文件。
![](http://upload-images.jianshu.io/upload_images/588630-4c4bd6dd3af648a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
20150531131942823.png
其中.mine文件：本人所做修改，两个.r0XX文件：XX为数字，数字较小的为更改前的文件，较大的为更改后的文件，在文件中会有<<<< mine .r0XX和>>>>>等字样包含起来的代码，即冲突的地方，此时请和组内同事讨论或自己删除某部分修改文件后进行调试，修复文件。
五、创建分支和Tag
![](http://upload-images.jianshu.io/upload_images/588630-323fb2612f2a0c2c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
![](http://upload-images.jianshu.io/upload_images/588630-6aec248bbacf44dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
1、选择项目所在的远程仓库
2、选择要创建分支的项目
3、右键或者点击上图选择Branch
4、选择分支存放的路径
ps：Branch As：分支的名称
Where：把分支存放在什么位置
Tag同Branch一样
六、Merge
![](http://upload-images.jianshu.io/upload_images/588630-8f46ea1c0d11d539.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
如果两个分支需要合并到主干，Checkout到本地，点击需要合并到的项.
点击Merge
点击Sychronize Branch：选择需要从被合并的项目（merge from）合并到这里，然后提交就可以了（如果同时有两个分支，最需仍需要在分支上修改的话，先合并一个分支到主干，然后主干在合并到另一个分支，修改冲突后提交，前提是，刚开始主干和两个分支的代码一样，参考上边的步骤生成）
ps：1.在workcopying中选择目标copying，然后点击Merge，如图所示
2.选择Mergefrom的copying
3.Merge之前cornerstone会进行dry run，进行merge分析和预览
4.确认无误后Merge Changes （该操作是本地操作，注意解决冲突后在commit）
七、cornerStone错误异常处理
1、
![](http://upload-images.jianshu.io/upload_images/588630-08e8c3600b1ce750.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1272323-46573a888e452b07.jpg
主要是由于你操作异常导致的,解决方案是,把生成的错误文件删除,然后在重新安装一下就可以了。
![](http://upload-images.jianshu.io/upload_images/588630-a1ebe29db3f9ec40.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1272323-91a005057575e393.jpg
把红色箭头部分的文件删除就可以了。
2、
![](http://upload-images.jianshu.io/upload_images/588630-aa25715016f7b24d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
本地代码版本号与服务器当前最新版本号不一致导致
点击update to latest，更新服务器最新的就可以了。
先写这么多，以后遇到问题会有更新。
文／s_在路上（简书作者）
原文链接：http://www.jianshu.com/p/7f5c019c528b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
