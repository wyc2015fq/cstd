# cocos2d-x 3.10 安卓 屏幕适配 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2016年03月10日 11:32:43[dutong0321](https://me.csdn.net/dutong0321)阅读数：824
在cocos3d-x升级到3.10以后，不断的出现新的问题。
BUG：
在移植到安卓平台上，会出现屏幕在中央，而不能根据ResolutionPolicy的选项来进行屏幕的适配、缩放和拉伸。
但是，在PC机上面，这个问题是没有的。
思路：
因为在PC机上是没有问题的，怀疑是否是因为安卓部分出现了BUG，在安卓的代码中搜寻设置地点，未果。
因为控制屏幕说到底是GLiew来控制的，所以返回到AppDelegate.cpp中来寻找答案。
BUG原因：
在经过数次尝试以后，发现由于官方想要更完美的适配机型，设置了多种分辨率，并更具识别到的分辨率来进行内容的缩放。
但是官方也明白：不可能把所有的分辨率都列举出来，于是，我不知道是那个程序员写的这段代码，利用IF语句来进行比较，确定是否缩放以及缩放倍数。
好，现在我们假设
designResolutionSize：450*800
smallResolutionSize：450*800
largeResolutionSize：900*1600
frameSize：540*960
```cpp
if (frameSize.height > largeResolutionSize.height)
  {        
     director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
  }
  else
  {        
     director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
  }
```
（其中setContentScaleFactor是设置surface的大小）
好吗，经过这么一缩放辛辛苦苦适配成功的屏幕又变回smallResolutionSize了。
解决办法：
最简单的办法就是把这一段删掉（这部分所有的if else 以及 ifelse）就可以直接搞定。
额外福利：
```cpp
glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
```
这条语句就是用来设置屏幕的适配方案的，第一个参数是设计的宽度，第二个参数是设计的高度，第三个参数是适配的方法。
第三个参数的介绍：
```cpp
ResolutionPolicy::EXACT_FIT 拉伸屏幕，是屏幕大小完全与实际屏幕一致，但可能元素会发生变形
  ResolutionPolicy::FIXED_HEIGHT 按高度等比例缩放，但可能宽度或无法显示或有黑边
  ResolutionPolicy::FIXED_WIDTH 同理，按宽度等比例缩放，但可能高度或无法显示或有黑边
  ResolutionPolicy::NO_BORDER 等比例缩放，直到没有黑边，但可能部分内容无法看到
  ResolutionPolicy::SHOW_ALL 等比例缩放，直到高度或宽度刚好适应屏幕，但可能有黑边
```
其实，对于我来说，我比较偏爱EXACT_FIT，因为目前大部分屏幕都是成比例的，所以直接EXACT_FIT，元素变形的可能性也不大。其实，如果成比例的话，上面每一种都不错，效果都一样的完美。但是，目前网络上、书籍上大部分还是推荐SHOW_ALL，我猜想大概是有的手机是虚拟按键吧，所以各位因人而异吧！
呜呜，终于改好了 吐槽一下 博客的编辑器真不好用！！！
