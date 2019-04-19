# PDF文件怎样解密 - xqhrs232的专栏 - CSDN博客
2013年12月07日 23:52:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：864
个人分类：[PC/Windows/Office技术](https://blog.csdn.net/xqhrs232/article/category/906929)
原文地址::[http://jingyan.baidu.com/article/2a1383286272c1074b134f6e.html](http://jingyan.baidu.com/article/2a1383286272c1074b134f6e.html)
- 
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/whcrop=92,69/sign=9687ff64c895d143da23b2611c80bf3b/f636afc379310a556739cd93b74543a9822610b3.jpg)1
- 
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/whcrop=92,69/sign=3bd982957d1ed21b799c78a7c21ee0fd/aa18972bd40735fa5a805dde9e510fb30f240860.jpg)2
- 
![PDF文件怎样解密](http://d.hiphotos.baidu.com/exp/whcrop=92,69/sign=ae7ec6a99925bc312b0857da31afb088/b2de9c82d158ccbfef0aa65d19d8bc3eb1354126.jpg)3
- 
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/whcrop=92,69/sign=c72d5a8e8d1001e94e69424dd77e46d5/8435e5dde71190ef122781b8ce1b9d16fcfa60fc.jpg)4
- 
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/whcrop=92,69/sign=eca3954eebc4b74534c1e154a08c232e/b21bb051f8198618d08cfce34aed2e738ad4e68a.jpg)5
- 
![PDF文件怎样解密](http://f.hiphotos.baidu.com/exp/whcrop=92,69/sign=903191573af33a879e385658a92c2d01/0dd7912397dda144e83c7ed5b2b7d0a20cf4869b.jpg)6
- 
![PDF文件怎样解密](http://c.hiphotos.baidu.com/exp/whcrop=92,69/sign=bac42fad513d26972e865e1f3a8b8fc2/9922720e0cf3d7ca61e32d11f21fbe096b63a9bb.jpg)7
[分步阅读](http://jingyan.baidu.com/album/2a1383286272c1074b134f6e.html)
**PDF文件是可以加密的，有时候我们从网络上下载的文件没有打开密码无法打开或者想打印但是文件有权限密码，有些网络上的书籍还带有数字证书等等，平时阅读使用的时候极为不方便，那么我们能不能去掉这些烦人的密码呢？下面我就将一些关于PDF文件解密方面的东东~~~★★★★★严正警告: 请勿使用本文介绍的方法做任何违规违法的事情，若发生此类事件，一切责任和本人无关，把持不住自己的，请勿再往下看~~~~谢谢**
## 工具/原料
- 
Advanced PDF Password Recovery 5.0
- 
PDF Password Remover 3.0
- 
PDF Unlocker
- 
Foxit PDF editor 2.2 （PJ版）
## 步骤/方法
- 
1
**解密需要打开口令的PDF文件**
破解这类PDF加密的软件还是比较多的，不过效果都不甚理想，因为这类软件基本上都是采用
暴力破解的方法，就是穷举法，如果你知道密码的部分内容也可以通过字典法来进行破解
反正基本上都差不多，这里我用 Advanced PDF Password Recovery 5.0 这款举例。
我们测试一个比较简单的3位纯数字加密的PDF文件，预先知道密码是123
基本设计如下：
开始于 0，结束于 999999（**长度选项卡里面设置最大长度口令字符数为6字符**，和999999对应，
目的是尝试0~999999的所有六位数的纯数字）
如果解密成功就会出现如下界面：
**如果是比较复杂的或者长度比较长的密码（特别是一些网站喜欢用网站的网址作为密码），**
**那么用这个方法几乎是无法破解的，如果遇上这样子的文件，我劝大家还是不要浪费时间，**
**洗洗睡吧~~~HOHO~~~**
- 
2
软件的运行界面如下： 
最上面一排是操作按键，中间一块是设置区 下面一块是信息区。
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/w=500/sign=001fa1c3a4c27d1ea5263bc42bd5adaf/f636afc379310a556739cd93b74543a9822610b3.jpg)
- 
3
攻击类型设置为暴力，范围选项我们勾选所有数字（先测试纯数字的），
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/w=500/sign=9952b39dd1c8a786be2a4a0e5708c9c7/aa18972bd40735fa5a805dde9e510fb30f240860.jpg)
- 
4
我们可以看一下，这个文件的密码是123 破解时间是19ms，说明简单的密码破解还是比较轻松的。
![PDF文件怎样解密](http://d.hiphotos.baidu.com/exp/w=500/sign=3b855dbb54e736d158138c08ab514ffc/b2de9c82d158ccbfef0aa65d19d8bc3eb1354126.jpg)
- 
5
**文件能够打开，但是被限制了权限的PDF文件的解密**
你可以直接将你要解密的PDF文件拖放到中间区域，然后会弹出包存对话框，你保存下来就是
已经破解完毕的。可以批量文件的，如果发现不是此类权限加密的，会有提示的。
**2.2 使用 PDF Unlocker 解密**
PDF Unlocker 的界面很简洁，让人一目了然：
按上面那个“浏览“选择要解密的PDF文件，然后按下面那个”浏览“选择好解密后的保存路径，
然后点下面的“解锁PDF文件” 就可以了~~~就这么简单~~~~
解密完，提示如下图：
- 
6
这类的文件是最常见的，特别是网站发布的一些文件资料等等，都是加了权限密码限制我们
- 
7
复制、打印、转换等等，这类的加密方式还是比较好破解。
- 
8
- 
9
此类的加密，用adobereader或者adobe acrobat打开后在标题栏的上面会有（已加密）字样：
- 
10
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/w=500/sign=51cbe00b103853438ccf8721a312b01f/8435e5dde71190ef122781b8ce1b9d16fcfa60fc.jpg)
- 
11
**2.1 使用PDF Password Remover 3.0解密**
- 
12
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/w=500/sign=32f113ade6cd7b89e96c3a833f254291/b21bb051f8198618d08cfce34aed2e738ad4e68a.jpg)
- 
13
![PDF文件怎样解密](http://f.hiphotos.baidu.com/exp/w=500/sign=92c25c9c0a46f21fc9345e53c6246b31/0dd7912397dda144e83c7ed5b2b7d0a20cf4869b.jpg)
- 
14
![PDF文件怎样解密](http://c.hiphotos.baidu.com/exp/w=500/sign=3d57b97807082838680ddc148899a964/9922720e0cf3d7ca61e32d11f21fbe096b63a9bb.jpg)
- 
15
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/w=500/sign=10bc7eb93d6d55fbc5c676265d224f40/d439b6003af33a87df7b127dc65c10385343b546.jpg)
- 
16
**数字证书加密的破解**
**数字证书的安全是比较高的，我们这里要讲的数字证书破解，指的是我们具有PDF文件的**
**证书和证书的密码的情况下的破解，证书和密码缺一不可。**
比如我们从某一个网站上下载了一分资料书籍，但是那份资料书籍是用数字证书加密的，同时
提供给读者一个数字证书和一个使用方法含证书密码，使用方法中指导用户怎么用adobe reader
或者adobe acrobat 软件阅读这样的文件，其他PDF阅读软件基本上是不能够使用的，即使我们用
adobe reader或者adobe acrobat打开文件后，我们能够做的仅仅是阅读，其他操作是一概不被
允许的。其实他的证书使用方法使我们进入了一个误区，下面大家按照我的方法用foxit PDF editor
就可以将数字证书剥离PDF文件，破解后的PDF文件可以不用证书或密码直接打开阅读，也可以
进行去其他任何操作。
**我们先记录一下解密前的文件的安全性。（打开文件，菜单，文件-------属性 安全选项卡）**
**文档限制小结：除了打印之外，其他都是不被允许的**
**接下来我们开始动手，我们先双击这个数字证书，将这个数字证书导入windows系统**
**（这是最关键的步骤）**
**选择“根据证书类型，自动选择证书存储” 当然也可以选择“将所有的证书放入下列存储”‘**
**然后“下一步”**
**完成**
**出现证书导入成功提示**
打开证书加密的PDF文件，
然后，**文档-------导出页面，**
**设置好导出页面的存放路径，页面数和总计页面数要一致，然后确定，**
**我们用adobe acrobat 打开看一下文件的安全性。（菜单，文件-------属性 安全选项卡）**
**安全型方法：显示的是无安全设置**
**文档限制小结：里面的全部项目均是被允许的，没有任何限制。这说明PDF文件解密成功！！！**
- 
17
**这样我们可以比较一下破解前后的效果~~~ 安全性方法：证书安全性**
![PDF文件怎样解密](http://c.hiphotos.baidu.com/exp/w=500/sign=79468e203912b31bc76ccd29b6193674/908fa0ec08fa513d2c5c7eb93d6d55fbb3fbd9e6.jpg)
- 
18
“下一步”
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/w=500/sign=22f0ab0394eef01f4d1418c5d0ff99e0/a686c9177f3e67095f4257983bc79f3df8dc5512.jpg)
- 
19
**“下一步”**
![PDF文件怎样解密](http://a.hiphotos.baidu.com/exp/w=500/sign=376ee94c9d16fdfad86cc6ee848e8cea/4034970a304e251f054bf69fa786c9177f3e531c.jpg)
- 
20
密码区键入证书使用说明里面的密码
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/w=500/sign=1e51da484134970a4773102fa5cbd1c0/38dbb6fd5266d016bfa68c4f972bd40735fa351d.jpg)
- 
21
![PDF文件怎样解密](http://d.hiphotos.baidu.com/exp/w=500/sign=3706f60fccbf6c81f7372ce88c3fb1d7/d53f8794a4c27d1e799f29a31bd5ad6eddc4381f.jpg)
- 
22
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/w=500/sign=22bd7b3734a85edffa8cfe23795509d8/f9dcd100baa1cd11ec076047b912c8fcc3ce2d27.jpg)
- 
23
然后我们就可以用Foxit PDF editor 2.2 **（这里用的是破解版的，非破解版无法去除证书的）**
![PDF文件怎样解密](http://g.hiphotos.baidu.com/exp/w=500/sign=4722ecb876094b36db921bed93cd7c00/bd315c6034a85edffb3eec4449540923dd547522.jpg)
- 
24
![PDF文件怎样解密](http://a.hiphotos.baidu.com/exp/w=500/sign=9683112ddab44aed594ebee4831d876a/3c6d55fbb2fb43167db9681d20a4462309f7d336.jpg)
- 
25
**按图设置导出页面对话框**
![PDF文件怎样解密](http://a.hiphotos.baidu.com/exp/w=500/sign=9316b9060fb30f24359aec03f894d192/8718367adab44aedc6ecac83b31c8701a18bfb33.jpg)
- 
26
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/w=500/sign=09942b80912397ddd67998046983b216/ae51f3deb48f8c54f842d7483a292df5e1fe7fc7.jpg)
- 
27
**成功创建了页面导出的文件，这个文件就是去掉了数字证书加密的最终我们需要的文件。**
![PDF文件怎样解密](http://b.hiphotos.baidu.com/exp/w=500/sign=ffb354daa9773912c4268561c8188675/f603918fa0ec08fa588d01c059ee3d6d54fbdac3.jpg)
- 
28
![PDF文件怎样解密](http://e.hiphotos.baidu.com/exp/w=500/sign=4848f09d7bcb0a4685228b395b62f63e/64380cd7912397dde5ff8a635982b2b7d1a287d6.jpg)
- 
29
**总结：PDF文件还有很多其他种类的加密方式，比如DRM加密，fileopen加密，目前这些还**
- 
30
**不能够被破解，这个也是没有办法的，希望本文介绍的几种破解方法能够为你解决不时之需。**
- 
31
END
## 注意事项
- 
**破解加密文件是有风险的，很有可能破解失败的话文件会被损坏，所以建议破解前做好文件的备份工作。**
- 
**★★★★★ 再次强调：切勿将本文介绍的方法用于违规违法行为中去~~~~**
## 参考资料
本经验系作者原创
