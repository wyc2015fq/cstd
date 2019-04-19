# 如何优雅地使用 Sublime Text - 文章 - 伯乐在线
原文出处： [杨琼璞（@杨琼璞）](http://www.jeffjade.com/2015/12/15/2015-04-17-toss-sublime-text/)
Sublime Text 是一款具有代码高亮、语法提示、自动完成且反应快速的编辑器软件，不仅具有华丽的界面，还支持插件扩展机制，用她来写代码，绝对是一种享受。
相比于难于上手的Vim，浮肿沉重的Eclipse，VS，即便体积轻巧迅速启动的Editplus、Notepad++，在SublimeText面前大略显失色，无疑这款性感无比的编辑器是编程和写作最佳的选择，没有之一。
## 目录：
- [Sublime Text 2和3的对比](#one)
- [Sublime Text 3安装插件](#two)
- [Sublime Text 3插件推荐](#three)
- [定制属于自己的快捷键](#four)
- [Sublime不可不知的实用技巧](#five)
- [定制属于自己的个性化主题](#six)
- [Sublime锦上添花些许设置](#seven)
- [写在一路更来的结语](#eight)
## Sublime Text 2和3的对比
相比于2，Sublime Text 3就秒启动一项，就压倒性地胜利了。因此在之后的叙述中都以Sublime Text 3为主角。并且3一直在不断的完善更新，具体的差异可参看[Sublime Blog](http://www.sublimetext.com/blog/)。简单的说：
- ST3支持在项目目录里面寻找变量
- 提供了对标签页更好地支持（更多的命令和快捷键）
- 加快了程序运行的速度
- 更新了API，使用Python3.3
强烈推荐朋友们使用3! 唯快不破，不解释。
## Sublime Text 3安装插件
Sublime Text的强大就是她拥有强大的课可扩展性。您可根据自己的需要安装不同的插件；这使得她变的无比强大的同时又不失轻便。
- 插件安装方式一：直接安装：
> 
安装Sublime text 3插件很方便，可以直接下载安装包解压缩到Packages目录（菜单->preferences->packages）。
- 插件安装方式二：使用Package Control组件安装：
> 
按Ctrl+`调出console（注：安装有QQ输入法的这个快捷键会有冲突的，输入法属性设置-输入法管理-取消热键切换至QQ拼音）粘贴以下代码到底部命令行并回车：


```
import urllib.request,os; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf), 'wb').write(urllib.request.urlopen( 'http://sublime.wbond.net/' + pf.replace(' ','%20')).read())
```
> 
重启Sublime Text 3。如果在Perferences->package settings中看到package control这一项，则安装成功。按下Ctrl+Shift+P调出命令面板输入install 调出 Install Package 选项并回车，然后在列表中选中要安装的插件。
PS：国内使用SublimeText3，经常可能遇到无法安装可用插件问题，可remove掉Package Control重新安装下；如遇到连Package Control也无法安装，则可以在别处拷贝一份关于Package Control的文件－(Package Control.sublime-package)存放于`Installed Packages`目录之下即可。
Sublime Text 3插件推荐
无插件，不神器！根据自己的需要定制属于自己的强大插件集；下面是一些常用的推荐。
[Sublime Text3 插件：增强篇](http://www.jianshu.com/p/5905f927d01b)
[20个强大的SublimeText插件](http://blog.jobbole.com/58725/)
作为在移动端旧游之后，又Web端新游的自己，墙裂推荐以下这么几款插件：
[MarkDown Editing](https://github.com/SublimeText-Markdown/MarkdownEditing)
SublimeText不仅仅是能够查看和编辑 Markdown 文件，但它会视它们为格式很糟糕的纯文本。这个插件通过适当的颜色高亮和其它功能来更好地完成这些任务。
[SideBarFolders](https://github.com/titoBouzout/SideBarFolders)
打开的文件夹都太多了? 来用这个来管理文件夹，世界原来也可以这么美好。
![SideBarFolders](http://www.jeffjade.com/img/toss/sublimetext-folder.png)                                                                   SideBarFolders
[Sublime Terminal](http://wbond.net/sublime_packages/terminal)
这个插件可以让你在Sublime中直接使用终端打开你的项目文件夹，并支持使用快捷键。
[ColorPicker](http://weslly.github.io/ColorPicker/)
通常，如果你想使用一个颜色选择器则可能打开 Photoshop 或 GIMP。而在 Sublime Text 中，你可以使用内置的颜色选择器。安装完成后，只要按下Ctrl / Cmd + Shift + C 快捷键。
[SublimeREPL](https://github.com/wuub/SublimeREPL)
这可能是对程序员很有用的插件。SublimeREPL 允许你在 Sublime Text 中运行各种语言（NodeJS ， Python，Ruby， Scala 和 Haskell 等等）。
[Ctags插件](https://github.com/SublimeText/CTags)
有童鞋抱怨Sublime Text不能支持函数的跳转（比如像Eclipse那样，按住Control点击该方法或者对象，即可跳转到定义的地方； Alt+←即可回到原处）。其实Sublime Text也可以借助插件实现之（当然，有些情况下:Can not find defination）毕竟这个也是借助正则来匹配完成的。因此这个也就要求代码很规范。这个插件相对来讲会有些麻烦，具体的可以参见:[Sublime Text ctags 的配置](http://firstleaf.diandian.com/post/2013-03-29/40049695682).
[SublimeLinter插件](https://github.com/SublimeLinter)
SublimeLinter 是前端编码利器——Sublime Text 的一款插件，用于高亮提示用户编写的代码中存在的不规范和错误的写法，支持 JavaScript、CSS、HTML、Java、PHP、Python、Ruby 等十多种开发语言。这篇文章介绍如何在 Windows 中配置 SublimeLinter 进行 JS & CSS 校验。
比如写例如像lua这样的弱语言脚本代码，有这个可以规避掉很多不该有的低级错误吧？当然这也需要你SublimeLinter安装完毕之后再安装一个`SublimeLinter-lua`即可。具体的使用可以参见：[借助 SublimeLinter 编写高质量的 JavaScript & CSS 代码](http://web.jobbole.com/84683/)
[SideBarEnhancements插件](https://github.com/titoBouzout/SideBarEnhancements)
SideBarEnhancements是一款很实用的右键菜单增强插件；在安装该插件前，在Sublime Text左侧FOLDERS栏中点击右键，只有寥寥几个简单的功能；安装了就相当于给其丰了大胸一般。
更强大的是，该插件还能让我们自定义快捷键呼出某个浏览器以预览页面！这样就不用到项目目录下寻找和拖动到特定浏览器中预览了。
安装此插件后，点击菜单栏的preferences->package setting->side bar->Key Building-User，键入以下代码：


```
[   
    { "keys": ["ctrl+shift+c"], "command": "copy_path" },
    //chrome
    { "keys": ["f2"], "command": "side_bar_files_open_with",
            "args": {
                "paths": [],
                "application": "C:\\Users\\jeffj\\AppData\\Local\\Google\\Chrome\\Application\\chrome.exe",
                "extensions":".*"
            }
     }
]
```
这里设置按Ctrl+Shift+C复制文件路径，按F2即可在Chrome浏览器预览效果(如果需要的话，也可以根据自己的需要为Firefox，Safari，IE，Opera等加上)，当然你也可以自己定义喜欢的快捷键，最后注意代码中的浏览器路径要以自己电脑里的文件路径为准。
[HTML-CSS-JS Prettify](https://github.com/victorporof/Sublime-HTMLPrettify)
一款集成了格式化（美化）html、css、js三种文件类型的插件，即便html,js写在PHP文件之内。插件依赖于nodejs，因此需要事先安装nodejs，然后才可以正常运行。插件安装完成后，快捷键ctrl+shift+H完成当前文件的美化操作。插件对html、css文件的美化不是非常满意，但还可以，后面将说明如何修改css美化脚本。本人用起来超级爽的，鉴于篇幅，就不赘述，可以参见[这篇](http://frontenddev.org/article/sublime-does-text-three-plug-ins-html-and-css-js-prettify.html)介绍。
[CSScomb CSS属性排序](https://github.com/csscomb/CSScomb-for-Sublime):
有时候看看自己写的CSS文件，会不会觉得属性很乱查找不易维护难？CSScomb可以按照一定的CSS属性排序规则，将杂乱无章的CSS属性进行重新排序。选中要排序的CSS代码，按Ctrl+Shift+C，即可对CSS属性重新排序了，代码从此简洁有序易维护，如果不款选代码则插件将排序文件中所有的CSS属性。当然，可以自己自定义CSS属性排序规则，打开插件目录里的CSScomb.sublime-settings文件，更改里面的CSS属性顺序就行了。因为这个插件使用PHP写的，要使他工作需要在环境变量中添加PHP的路径，具体请看github上的说明。
[SublimeTmpl 快速生成文件模板](https://github.com/kairyou/SublimeTmpl)
一直都很奇怪为什么sublime text 3没有新建文件模板的功能，像html头部的DTD声明每次都要复制粘贴。用SublimeTmpl这款插件终于可以解脱了，SublimeTmpl能新建html、css、javascript、php、python、ruby六种类型的文件模板，所有的文件模板都在插件目录的templates文件夹里，可以自定义编辑文件模板。
SublimeTmpl默认的快捷键：


```
ctrl+alt+h html
ctrl+alt+j javascript
ctrl+alt+c css
ctrl+alt+p php
ctrl+alt+r ruby
ctrl+alt+shift+p python
```
如果想要新建其他类型的文件模板的话，先自定义文件模板方在templates文件夹里，再分别打开Default (Windows).sublime-keymap、Default.sublime-commands、Main.sublime-menu、SublimeTmpl.sublime-settings这四个文件照着里面的格式自定义想要新建的类型，这里就详细介绍了，请各位自己折腾哈~
[Javascript-API-Completions](https://github.com/Pleasurazy/Sublime-JavaScript-API-Completions):
支持Javascript、JQuery、Twitter Bootstrap框架、HTML5标签属性提示的插件，是少数支持sublime text 3的后缀提示的插件，HTML5标签提示sublime text3自带，不过JQuery提示还是很有用处的，也可设置要提示的语言。
![sublime-jq-plugin](http://www.jeffjade.com/img/toss/sublime-plugin-jq.gif)sublime-jq-plugin
SFTP：快速编辑远程服务器文件
在Win下用`Xftp` 和 `WinScp`，被这种需要切换点击or F5刷新的手动操作蛋疼到无语；故此一遇见这`SFTP`，顿觉这世界都美好了许多。当然Sublime下面也有些其他同步插件，比如`FtpSnyc`，但是配置起来的错误提示一点都不人性化，就毫不留情的舍弃了。Sublime下有`SFTP`，只要`Ctrl+S`即可同步本地到服务器，妥妥的爽歪歪有么有？如何配置，请参见[在 Sublime Text中使用 SFTP 插件快速编辑远程服务器文件](http://blog.wpjam.com/m/sublime-text-2-sftp/);如欲使用`FtpSync`可参见[Sublime使用及FtpSync远程同步](http://liuwanlin.info/sublimeshi-yong-ji-ftpsyncyuan-cheng-tong-bu/)；大道至简，因简而悦；开心垒码，值得折腾。
[WakaTime](https://wakatime.com/) — 记录你的Code时间;
[WakaTime](https://wakatime.com/)可以做到精确地统计到你花在某个项目上的时间;WakaTime针对不同的IDE，拥有不同的插件，在Sublime上安装着[插件](https://wakatime.com/help/plugins/sublime)，就能统计到我使用Sublime进行的所有项目的行为。可以高效管理和知晓自己code时间；并且，统计完善, 适合发朋友圈装逼（如果你喜欢的话）~
> 
Waka的基本设计和rescuetime类似。每个人注册完将获取一个key，装一个客户端，把key输进去（登陆是同一个道理），然后它就把本地的所有行为带个key扔给服务器来统计，一段时间之后给你个报表。不过Waka做的真的很精准，精确到每一个文件用了多少秒，每一种语言用了多少时间。
![](http://upload-images.jianshu.io/upload_images/82950-b9d7380d0608ecb1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装和使用都很简单，请参见[这里](https://wakatime.com/help/plugins/sublime-text)。另外一篇比较详细的文章[时间都去哪了?用RescueTime和WakaTime来记录你的时间](https://luolei.org/track-your-time/),对RescueTime和WakaTime有一个更为详细的叙述，可以一读。
剩下些许其他的可以按需安装的插件，比如：
- ConvertToUTF8 支持 GBK, BIG5, EUC-KR, EUC-JP, Shift_JIS 等编码的插件
- Bracket Highlighter 用于匹配括号，引号和html标签。对于很长的代码很有用。安装好之后，不需要设置插件会自动生效
- DocBlockr 可以自动生成PHPDoc风格的注释。它支持的语言有Javascript, PHP, ActionScript, CoffeeScript, Java, Objective C, C, C++
- Emmet(Zen Coding)快速生成HTML代码段的插件，强大到无与伦比:可以超快速编写HTML/CSS/JS，当然这个插件还支持多种编译环境，如常见的：Eclipse/Aptana、Coda、Notepad++、Adobe Dreamweaver、TextMate等，web开发必备！！！。
- jsFormat 格式化js代码，懂者自懂；强迫症Coder必备！默认快捷键Ctrl+Alt+F。
- phpFormat 格式化php代码，懂者自懂；强迫症Coder必备！
- CSS Compact Expand CSS属性展开收缩:写CSS的盆友，喜欢将其写多行还是一行(个人喜欢将其格式化为多行)？如果阅读别人的代码不符合自己的习惯，可以用CSS Compact Expand这个插件将CSS格式化一下，按 Ctrl+Alt+[ 收缩CSS代码为一行显示，按 Ctrl+Alt+] 展开CSS代码为多行显示；强迫症Coder必备！。
- Autoprefixer插件：这是一款CSS3私有前缀自动补全插件；该插件使用CanIUse资料库（当然，SublimeText自然也有CanIUse这个插件咯），能精准判断哪些属性需要什么前缀，与CssComb插件一样，该插件也需要系统已安装Node.js环境；使用方法：在输入CSS3属性后（冒号前）按Tab键即可。
- YUI Compressor：压缩JS和CSS文件，按F7键后，若压缩当前文件（demo.js），则压缩后的文件（demo.min.js）保存在该文件的同级目录，需要安装java的JDK。使用方法：[YUI Compressor](http://frontenddev.org/article/sublime-does-text-3-plug-in-yui-compressor.html)
- ClickableURLs：可点击的URL
使用小插件[ClickableURLs](https://github.com/leonid-shevtsov/ClickableUrls_SublimeText)可以让文件中的URL能够点击。
- 终极王道：自己编写专用的Sublime Text插件。虽然说各个方面比如移动端，Web前段，服务器端，非Coder的Writer所需要的方便已经被集成在了不同的插件中。但譬如，需要快捷打开PC端的某个模拟器，便捷的进行某些校验，只要你想的到的基本都可以将其在插件内，以快捷键处理之。至于如何编写SublimeText插件，请参看这里[编写自己的Sublime Text2 插件](http://www.bluesdream.com/blog/talk-about-the-icp-record-those-things.html)
## 定制属于自己的快捷键
- 首先要会使用SublimeText内置的快捷键：
比如 `Commond Shift P` 打开命令面板:例如打开Package Control安装各种插件；可以可以输入Set(Snytax)来改变使用的语言环境，瞬间切换等等。
- 设置快捷键。在SublimeText里，打开Preferences -> Key Bindings – User，我设置的快捷键：


```
[
    { "keys": ["ctrl+f9"], "command": "build" },
    { "keys": ["f10"], "command": "build", "args": {"variant": "Run"} },
    { "keys": ["ctrl+shift+x"], "command": "toggle_comment", "args": { "block": true } },
]
```
具体可参见这边文章 [Sublime Text3 快捷键汇总及设置快捷键配置环境变量](http://blog.csdn.net/moyan_min/article/details/11530751)
- 定制属于自己的专属快捷键：
比如就拿定制Sublime Text(2/3)编译lua 的快捷键来说：- 首先：下安装lua环境(MAC)


```
curl -s https://raw.githubusercontent.com/rudix-mac/rpm/2014.10/rudix.py | sudo python - install rudix
或者：curl -O https://raw.githubusercontent.com/rudix-mac/rpm/2014.10/rudix.py
sudo python rudix.py install rud
```
- 安装IED：SublimeText3;完毕后上面选择:Build System—New Build System 输入：


```
{  
    "cmd": ["/usr/local/bin/lua", "$file"],  
    "file_regex": "^(...*?):([0-9]*):?([0-9]*)",  
    "selector": "source.lua"  
}
```
保存为Lua.sublime-build，然后Tools-Build System上就能选择lua来编译脚本了）
PS：网上找到的大多数都是 “cmd”: [“lua”, “$file”]可能我是用rudix自动来安装lua的，需要改一下路径。
新建一个xx.lua文件，随便写个语句，选择lua为build system，按一下`command+b`，就能跑出来了。 如下图所示：
![LuaShortCutSublimeText](http://img2.ph.126.net/PI2F5lnTuwqeBm7KQx56zw==/1871245645272492575.png)
LuaShortCutSublimeText
WindowS下面的定制请参看这里[Sublime Text(2/3)编译lua](http://www.cnblogs.com/jadeboy/p/4055465.html)
## Sublime不可不知的实用技巧:
—-更新2015.05.21 15:50 ShenZhen@nanshan
对于这个依赖于插件的强大性感无比开源编辑器；优雅的适用之，就在于合理的使用需要的插件，走一条快准狠的键盘一刀流。这个当然可以自己用python来写，但是一般的需求基本上都已经有人完成，还是不浪费时间自己造轮子了。今儿就再赘述一些使用的经验，让使用她可以更加优雅。
(1) Ctrl+O(Command+O)可以实现头文件和源文件之间的快速切换
(2) 通过 View -> Side bar 可在左侧显示当前打开的文件列表
(3) ST3虽然不像notepad++可以在sidebar上显示函数列表，但是可通过Ctrl+R查看
(3) 通过 Preference -> Key binding user 可根据个人操作习惯自定义快捷键（包括ST3自带的和插件的）
(4) 双击可选中光标所在单词，三击可选中光标所在行(等同于`Ctrl＋L`(Command+L));
(5) Ctrl+Shift+T可以打开之前关闭的tab页，这点同chrome是一样的
(6)Ctrl+R定位函数；Ctrl+G定位到行；
(7) 单个文件批量修改：纯相同的内容：选中需要修改的内容`Alt+F3`(Mac下默认的是`Ctrl+Command+G`) ， 或者连续 Ctrl+D(Win) /连续 Command+D（Mac）之后重新写即可，使用`Ctrl + U`进行回退，使用Esc退出多重编辑。
不纯相同却有一定规律可循的内容：这个就需要借助SublimeText强大正则的支持了。可以使用`Ctrl+H`(Mac下是`Alt+Command+F`)调出：
![SublimeText](http://i.imgur.com/4NnE9SN.jpg)SublimeText
正则表达式是非常强大的文本查找&替换工具，Sublime Text中使用Alt + R切换正则匹配模式的开启/关闭。Sublime Text的使用[Boost里的Perl正则表达式风格](http://www.boost.org/doc/libs/1_44_0/libs/regex/doc/html/boost_regex/syntax/perl_syntax.html)。鉴于篇幅，加之本身就对这个不甚熟悉，这里就不再多啰嗦；以下是一些可以参考的文章：
- [正则表达式30分钟入门教程](http://blog.jobbole.com/96708/)
- [59分钟学会正则表达式](http://www.jeffjade.com/2014/12/25/2014-12-25-regular-expression/)
- [MSDN～正则表达式语法](https://msdn.microsoft.com/zh-cn/library/ae5bf541(v=vs.90).aspx)
- [月光博客～常用正则表达式](http://www.williamlong.info/archives/433.html)
(8) 有时我们需要对一片区域的所有行进行同时编辑，`Ctrl+Shift+L`可以将当前选中区域打散，然后进行同时编辑：
![SublimeText](http://i.imgur.com/0NHpXFl.gif)SublimeText
(9)有打散自然就有合并，`Ctrl + J`(mac下Command＋J)可以把当前选中区域合并为一行：
![SublimeText](http://i.imgur.com/hfRAyiy.gif)SublimeText
(10)在`Ctrl + P`(Command+P)匹配到文件后，我们可以进行后续输入以跳转到更精确的位置：
- @ 符号跳转：输入`@symbol`跳转到symbol符号所在的位置
- # 关键字跳转：输入`#keyword`跳转到keyword所在的位置
- : 行号跳转：输入`:12`跳转到文件的第12行。
(11)`Ctrl + Enter`(Mac~Command+Enter)在当前行下面新增一行然后跳至该行；`Ctrl + Shift + Enter`在当前行上面增加一行并跳至该行。
(12)Sublime Text的查找有不同的模式：`Alt + C`切换大小写敏感（Case-sensitive）模式，Alt + W切换整字匹配（Whole matching）模式，除此之外Sublime Text还支持在选中范围内搜索（Search in selection），这个功能没有对应的快捷键，但可以通过以下配置项自动开启。
> 
“auto_find_in_selection”: true
这样之后在选中文本的状态下范围内搜索就会自动开启，配合这个功能，局部重命名（Local Renaming）变的非常方便：
![SublimeText](http://i.imgur.com/QRIsDra.gif)SublimeText
(13)Windows下`Ctrl + ←/→`进行逐词移动，相应的，`Ctrl + Shift + ←/→`进行逐词选择。
![SublimeText](http://i.imgur.com/jFnTDl3.gif)SublimeText
而Mac下的`Command+←/→`是从一端移动到另一端；相应的，`Command + Shift + ←/→`是从一端选择到另一端。
(14) Windows下`Ctrl + ↑/↓`移动当前显示区域，`Ctrl + Shift + ↑/↓`移动当前行。
![SublimeText](http://i.imgur.com/v31pd3k.gif)SublimeText
而Mac下的`Command + ↑/↓`是从当前行移动到头/尾；相应的，`Command + Shift + ↑/↓`是从当前行选择到头/尾；
(15)使用`Ctrl + N`在当前窗口创建一个新标签，`Ctrl + W`关闭当前标签，`Ctrl + Shift + T`恢复刚刚关闭的标签。
(16)编辑代码时我们经常会开多个窗口，所以分屏很重要。Windows下：`Alt + Shift + 2`进行左右分屏，`Alt + Shift + 8`进行上下分屏，`Alt + Shift + 5`进行上下左右分屏（即分为四屏）。
(17)Sublime Text基本的手动格式化操作包括：`Ctrl + [`向左缩进(等同于将一块选中Shift+Tab)，Ctrl + ]向右缩进(等同于将一块选中后Tab键)，注解： Ctr+[ 和 Ctr+[ 针对一块连续内容使用，无需选中；此外Ctrl + Shift + V可以以当前缩进粘贴代码（非常实用）。
(18) ~~`Ctrl+Shift+V`~~(好吧，尝试了下Windows下的这个，目测不行，看来须得安装下[Clipboard-history](https://github.com/kemayo/sublime-text-2-clipboard-history)插件～（粘贴板历史记录）才行啊)（Mac下Command＋Shift＋V），粘贴之时可以调出之前粘贴过的内容（以一个轻量弹框显示以供选择），哇哦，才发现这个功能，感觉棒棒哒
![SublimeText](http://www.jeffjade.com/img/toss/sublimetext-hotkey.png)SublimeText
(19) Sublime text 删除插件步骤：“Ctrl+Shift+P”—“Remove Package”—“找到需要删除的插件，并点击即可删除”;
(20) 作为强大而小巧，性感且快捷的SublimeText，怎么能够允许不时弹个框提醒你购买或者别的，并且顶部有未注册这样破坏美感的存在呢？OK，输入[Sublime text 3最新版破解方法](http://www.jianshu.com/p/04e1b65dd2c0)中提供的注册码，就妥妥的哦了。
## 定制属于自己的个性化主题
SublimeText 她不仅仅是作为性感无比，强大可大编辑器的存在；要知道在如今这么靠脸吃饭的时代，她的美者这方面怎么可能不臻至完美呢？Mac和Windows都默认十多种不同风格的主题，想必会有您喜欢的。即便没有也没关系，我们可以自己定制自己喜欢的风格主题。[在线制作专属Sublime Text主题](http://tmtheme-editor.herokuapp.com/#/local/Monokai)。这个网址可以轻松按照自己的意愿制作喜欢的主题。完毕之后将生成的xx.sublime.theme文件，点开Preferences -> Browsr Packages这个，放在这个直属目录之下，即可在Theme处选择这个主题了，亲测没有问题。
后记：作为如此便捷的她(Sublime Text3)，值得分享肯定不止这些，在Coding and Writing路上，要继续去发现和发扬她的美，未完待续…
—写于2015.04.17 22:49 ShenZhen@nanshan
## Sublime锦上添花些许设置
—-更新2015.08.31 11:59 ShenZhen@nanshan—-
Sublime Text内运行javascript(ES6)
#### 首先安装[nodejs](http://nodejs.org/)
当然你可以使用其它诸如[jsc](https://trac.webkit.org/wiki/JSC)之类的环境来运行js, 本文使用的是[nodejs](http://nodejs.org/). 首先确保你的电脑已经安装好nodejs, 并已将其添加到环境变量中 (一般安装时自动添加或者询问是否添加)
#### 添加[build system](https://sublime-text-unofficial-documentation.readthedocs.org/en/sublime-text-2/file_processing/build_systems.html)
在sublime text中依次打开Tools -> Build System -> New Build System… 粘贴以下代码后保存(如Node.sublime-build), 然后把Build System设成Automatic
{ “cmd”: [“node”, “—use-strict”, “—harmony”, “$file”], “selector”: “source.js”}
#### 附加说明
在以上的build文件中(Node.sublime-build), node是执行命令, —harmony和—use-strict是执行参数, $file是当前文件名, 所以一次build操作实际上相当于在命令行中执行了node —use-strict —harmony filename
. —harmony表示启用[ES Harmony features](http://kangax.github.io/compat-table/es6/), 而这些features目前只能在strict模式下运行, 所以需要同时添加use-strict参数(详见[what-is-extended-mode](http://stackoverflow.com/questions/17253509/what-is-extended-mode)).
如果不想启用es6的特性,把build文件更改成以下代码保存即可.
{ “cmd”: [“node”, “$file”], “selector”: “source.js”}
#### 具体使用
在sublime test中新建一个test.js文件, 然后输入你的测试代码, 比如:


```
for (let i = 0; i < 3; i++) {
    console.log('i:', i);
}
```
使用快捷键 `ctrl + b`, 将得到以下执行结果：


```
i: 0i: 1i: 2[Finished in 0.1s]
```
温馨注解:
- (1)文件必须是存在于磁盘中的, 而不是untitled的, 否则sublime无法找到相应的文件.
- (2)Nodejs安装目录需要加入path环境变量中，否则Ctrl+B运行 会出现：[WinError 2] 系统找不到指定的文件。
### 更换Sublime Text程序图标
不仅主题可以更换，图标也可以。在[Dribbble](https://dribbble.com/)上有大量重新设计的Sublime Text精美图标。更换方法：
- 下载一个图标，有`.icn`s格式的最好。如果没有，用iConvert转换之。
- 终端执行：`open /Applications/Sublime\ Text.app/Contents/Resources/`
- 替换`Sublime Text 3.icns`或`Sublime Text 2.icns`文件。
![Sublime](http://segmentfault.com/img/bVchAA)
### Sublime 拼写检查
如果你经常使用SublimeText从事英文创作，那么启用拼写检查就非常有用处了。选择`Preferences > Settings – User`菜单，添加以下代码：
> 
“spell_check”: true,
## 写在一路更来的结语
不得不说的是此篇文章的命名，较之于内容来讲是大了很多。毕竟要优雅使用某个工具，并不是件很容易的事。一边发现学习，一边总结更新，到此已5个月整。然而，对她～SublimeText3三的使用和了解也是仅仅限于一般水平而已。只能说：每个人都工具的使用都怀有不同的态度和追求；但是，工欲善其事，必先利其器；此一点总是没错的。因此，在投资自己这里，除学习知识之外，工具的使用大抵就是最应该投资学习的，再次可以是蓝股等等，一如Google工程师@徐宥[工具的强是无敌的](http://blog.youxu.info/2008/03/10/tools-everywhere/)所言。
所谓，一个好汉三个帮；欲要优雅使用一个工具，辅助器具是必不可少的。而在于Win下以SublimeText来编程或者写作，譬如AutoHotKey，Listary等对个人来讲着实已成必备工具(在Mac下，有Alfred在手，可以说是天下我有)；其给人带来的方便快捷和意想不到的惊喜，足以可以令其称为Win下小众软件中实用最。当然，像AutoHotKey这种在乎于人强，其功能愈发强的工具，其修行之路漫漫，还当上下以求索之(15.09.17更)。
当然，也不能盲目停留在SublimeText这孤傲的世界里。虽性感强大如斯，毕竟也已逝老牌儿范儿了。新的时代总会崛起些新秀，比如Github出品的`Atom`许有赶超Sublime之势，目测充盈着些许不同的理念；有时间定当去学习下，知乎有话题：[如何评价 Github 发布的文本编辑器 Atom？](http://www.zhihu.com/question/22867204)，可预先了解下。
最后寄语：
工欲善其事，必先利其器！
公欲善其事，必先利其器！
参考文章出处：
[Sublime Text 全程指引 by Lucida](http://blog.jobbole.com/88648/)
[Sublime Text 3能用支持的插件推荐](http://blog.jobbole.com/96739/)
[Wakatime 测试工作时间](http://www.cnblogs.com/Leo_wl/p/4478147.html)
