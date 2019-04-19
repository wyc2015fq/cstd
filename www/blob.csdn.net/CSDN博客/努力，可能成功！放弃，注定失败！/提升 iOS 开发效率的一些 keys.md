# 提升 iOS 开发效率的一些 keys - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年03月28日 10:15:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：83
> 
首先题目写的有点大，这篇文章只是分享一下我平时用到的一些快捷键和习惯，希望能对大家平时的开发效率有所帮助。
本文来自小专栏,原文地址:[https://xiaozhuanlan.com/topic/7645321908](https://xiaozhuanlan.com/topic/7645321908)
## 至于为什么要写？
原因有几点：
- 工作中我发现有些iOS小伙伴平时开发的时候，比如在快速打开一个类文件的时候，还在项目目录里面一层一层的找，等等，希望本文能提升他们的开发幸福感。
- 我自认为我知道的快捷键还蛮多的，但是还是从同事那里学到了几个实用的?，这篇文章就当做记录分享自己平时开发的一些小技巧吧，大家看完有什么更好的tip 可以留言分享给我，相当于抛砖引玉吧，我会持续更新的~希望大家都能做一个`懒惰的程序员`（文末有我对`懒惰程序员`的定义）。
## 鼠标 和 触摸板
如果你用的iMac 大部分用的是苹果的鼠标，苹果的鼠标是可以设置智能手势的，这些在系统偏好里面都有教程和设置，大家可以去看一下，每每看到同事还在把iMac的鼠标当做windows的来用，我真想说这个鼠标贵是有道理的?。
鼠标：1. 一个手指右滑返回，在Xcode 中就不用每次都去点击返回上一个类了。2. 两个手指左\右滑切换桌面，MacOS的多桌面挺好的，可以一个放UI，一个放代码，一个放接口文档等等。3. 两个手指轻点，相当于F3上面的快捷键Miss Control，很方便。
触摸板：跟鼠标基本一样，切换桌面是三个手指左\右滑，Miss Control 是三个手指向上滑。
好了，废话不多说，直接进入关键的吧，我默认用快捷键的使用频次排序吧，希望大家都能动手尝试下。
## Xcode 中
- `Shift + Command + O` 弹出快速查找文件窗口（这个是字母O，Markdown O之后很像数字0 ），
```
Shift
 +Commond + 0
```
 是快速打开官方文档的快捷键。
- `Shift + Comand + j` 定位到文件所在目录，经常配合上面那条快捷键。
- `Control + Command + 上\下` 切换 `.m` 和`.h`。
- `Command + t` 新建一个Tab，这个很实用，我平时一般都会建3，4个Tab，关闭Tab 可以用 
```
Command
 + w
```
。
- 如果你不习惯开多个Tab，习惯开多个Xcode窗口，可以用Command + ` 来互相切换。
- `Control + Command + e` 可以批量修改光标所在位置的变量，像这样![](http://upload-images.jianshu.io/upload_images/1457495-905681620219f616.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- `Shift + Command + f` 打开全局搜索，当然你可以连续按住`Command`键连续按两次
 4，也会聚焦到全局搜索，全局搜索这里要说一下，搜索Xcode 不仅仅是支持text匹配，最常用的可以加个 `Any` 正则，就可以搜出如图中的这种，等等。
![](http://upload-images.jianshu.io/upload_images/1457495-e271cead8fed657a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- `Command + f` 在类中搜索 ，`enter` 匹配下一个 
```
Shift
 + enter
```
 匹配上一个。
- `Command + 上\下\左\右` 光标切换到类首，类尾，行首，行尾。
- `alt + 左\右` 光标左右移动一个单词。
- `Command + delete` 删除光标到行首的内容，同理
```
alt
 + delete
```
 删除光标前的一个单词，另外可以先切换到到行尾 用`Command + delete`删除一整行内容。
- 同理 `Command + Shift` 和 
```
Command
 + alt
```
加方向键就可以选中一整行或者多行了。
- 选中一段代码 `Control + i` 会自动缩进。
- `Command + \` 当前行加断点，
```
alt
 + Command + \
```
，新建一个`symbolic breakpoint`。
- `Command + n`新建文件 ，
```
Shift
 + Command + n
```
 新建工程
- `alt + Command + 左\右` 折叠\显示当前块，
```
Shift
 + alt + Command + 左\右
```
 折叠\显示当前文件中的块。
- `alt + Command + [` 上移，如果没有选中，默认上移当前行，
```
alt
 + Command + ]
```
 下移。
- `Command + ]` 向右缩进，支持多行，
```
Command
 + [
```
 向左缩进。
- `Shift + Command + k` product 清理，
```
Command
 + r
```
 Run，`Command + b` 编译。
- 还有一个重要的快捷键，☝️，`Control + Command + p` 快捷导入，像这样，![](http://upload-images.jianshu.io/upload_images/1457495-710fa3971957a3a1.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
很酷吧，需要导入一个工具才可以实现，[可以点击这里](https://github.com/markohlebar/Import)，Xcode 9 以前用过类似的插件也可以实现这个功能，之前还用[Peak君](http://mrpeak.cn/)的[FastStub-Xcode](https://github.com/music4kid/FastStub-Xcode)也挺爽的。
## Xcode 窗体相关
- `Shift + Command + Y` 隐藏 \ 显示 console 区，但是我建议显示console区 用 
```
Shift
 + Command + C
```
，会直接聚焦效果。
- `Command + k` console清屏。
- `Control + 1` 这个应该很多工程师都没用过�，当然点击小图标也可以。![](http://upload-images.jianshu.io/upload_images/1457495-a09c8500ed329e93.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
如图可以看到所有调用者，导入的文件，被那些导入等等相关依赖。还可以找到最近打开的文件，Recent Files
- `Control + 6` 查看当前类的方法列表，这里建议用 
```
pragma
 mark
```
 来合理分块，[相关扩展阅读可以看这里的代码规范建议](https://www.jianshu.com/p/bbb0b57eb168)
- `Command + 1,2,3,4,5,6,7,8,9` 切换左边窗体，
```
Command
 + 0
```
 显示 \ 隐藏左边窗体。
- `alt + Command + 0` 显示 \ 隐藏右边窗体，同理
```
alt
 + Command + 1,2
```
等也可以切换
- `Command + ,` 弹出 Perferences ,可以用 
```
Command
 + w
```
 隐藏
## 其他
- 还有一个多行编辑，有的时候也会用到，需要借助Sublime Text, 快捷键是 `Shift + Command + L`，退出多行编辑也是
```
Shift
 + Command + L
```
![](http://upload-images.jianshu.io/upload_images/1457495-4b1e05f2c78c0d22.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- `Shift + Command +3` 全屏截图，
```
Shift
 + Command + 4
```
 自选区域截图，`Shift + Command + 4 + 空格` 截图某一个窗体。
- `Command + F3` 快捷查看桌面。
- `Control + Command + 空格` 弹出表情输入框。
## 工具
推荐大家安装 `CheatSheet`，之后长按Command 就可以查看所有的快捷键了。
题外：在推荐给大家一个应用，[Noizio](http://www.noiz.io/)，环境噪音模拟工具，可以模拟在咖啡馆，可以模拟在海边等等。
## 关于懒惰程序员
懒惰对于程序员其实是一种夸奖，这里的懒惰并不是常规意义上的懒惰，也许你听过`懒惰是程序员的第一美德`。懒惰会让程序员想到一些更好的方案去解决手上的问题，往往的结果都是节省了很多体力劳动，以及达到同样的需求用的是最优雅最便捷的方式。
举一个实际例子就是，我们最近项目引入了国际化，大家都知道翻译真的是一个体力活，我身边的同事用Python写了一个[脚本](https://github.com/LeoXu92/LocalizeString-iOS)，可以自动检测需要翻译的文字，自动写到相应的Localization文件以及文字代码所在的位置，大大节省了体力劳动，这就是优秀的懒惰美德。
## 最后
如果大家有什么觉得好的快捷键或者是一些提升效率的idea，可以留言分享给我，也让我有一些好习惯补充，同时我也会更新本文，谢谢！
