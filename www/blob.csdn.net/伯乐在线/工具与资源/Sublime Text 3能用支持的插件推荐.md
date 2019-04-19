# Sublime Text 3能用支持的插件推荐 - 文章 - 伯乐在线
原文出处： [淡高](http://dengo.org/archives/923)
从二月份用测试版本build 3012开始用sublime text 3，虽然很多插件在sublime text 3不工作了，因为sublime text 3修复了2的一些bug、提升了性能并集成了不少常用插件的功能，当时没什么要求够用了，就这样傻傻地裸插件空手写代码用了快5个月。上月27号Sublime Text 3公测了，又可以试用了，估计插件的更新也差不多了吧，果断上github逛逛看看有没有sublime text 3能用的插件，经过几天折腾和试用，感觉不错，推荐给大家，各位就择你所好而用之。
**Package control 插件管理**
sublime text非常强大的插件管理的插件，按下Ctrl+Shift+P，输Package control就可以选择插件的安装、管理、删除等操作，因为sublime text 3的插件需要基于pyhone 3编写，所以用sublime text 2的安装方法不管用，可以看看我之前写的一篇文章，[Sublime text 3如何安装package control办法](http://dengo.org/archives/594)。
此外，安装sublime text 3的插件还可以在github上下载源文件，解压后改名放到
C:\Users\Mr.DenGo(你的电脑名)\AppData\Roaming\Sublime Text 3\Packages 中
重启sublime text 3即可生效。
**BracketHighlighter 高亮显示匹配的括号、引号和标签**
BracketHighlighter这个插件能在左侧高亮显示匹配的括号、引号和标签，能匹配的 `[] `,  `() `,  `{} `,  `"" `, `'' `,  `<tag></tag>等甚至是自定义的标签，当看到密密麻麻的代码分不清标签之间包容嵌套的关系时，这款插件就能很好地帮你理清楚代码结构，快速定位括号，引号和标签内的范围。`
![](http://ww3.sinaimg.cn/mw690/6941baebjw1ezbrurqxbtj20b209kab1.jpg)
插件下载：[https://github.com/facelessuser/BracketHighlighter/tree/BH2ST3](https://github.com/facelessuser/BracketHighlighter/tree/BH2ST3)
**TrailingSpacer****高亮显示多余的空格和Tab**
有时候在代码结尾打多了几个空格或Tab，一般不会察觉，TrailingSpacer这款插件能高亮显示多余的空格和Tab，并可以一键删除它们，有代码洁癖的朋友应该会喜欢这个插件。
![](http://ww1.sinaimg.cn/mw690/6941baebjw1ezbrtodotzj20b704b74m.jpg)
插件下载：[https://github.com/SublimeText/TrailingSpaces](https://github.com/SublimeText/TrailingSpaces)
注意，在github上下载的插件缺少了一个设置快捷键的文件，可以新建一个名字和后缀为Default (Windows).sublime-keymap的文件，添加以下代码，即可设置“删除多余空格”和“是否开启TrailingSpacer ”的快捷键了。


```
[
    { "keys": ["ctrl+alt+d"], "command": "delete_trailing_spaces" },
 
    { "keys": ["ctrl+alt+o"], "command": "toggle_trailing_spaces" }
]
```
**Alignment 等号对齐**
按Ctrl+Alt+A，可以将凌乱的代码以等号为准左右对其，适合有代码洁癖的朋友。
![](http://ww2.sinaimg.cn/large/6941baebjw1ezbrto089vg20jo0203yn.gif)
插件下载：[https://github.com/kevinsperrine/sublime_alignment/tree/python3](https://github.com/kevinsperrine/sublime_alignment/tree/python3)
**Clipboard-history 粘贴板历史记录**
有了这个插件，便可方便使用sublime text 3里的粘贴板历史记录内容，快捷键Ctrl+Shift+V可调出该历史记录面板，按方向键选择想要粘贴的历史记录。不过这是sublime text 2下的插件，Ctrl+Shift+D清除粘贴板历史记录好像不能生效，不过重启sublime也可清除粘贴板的历史记录。
![](http://ww4.sinaimg.cn/mw690/6941baebjw1ezbrtnr8koj20cl04e74c.jpg)
插件下载：[https://github.com/kemayo/sublime-text-2-clipboard-history](https://github.com/kemayo/sublime-text-2-clipboard-history)
**gbk4subl 支持GBK编码**
sublime text一个遗憾是不支持中文GBK编码，在编辑一些GBK编码的文件时就会出现乱码，还好sublime text 2有很多可以支持GBK编码的插件，可惜大多在sublime text 3下都不能工作，在github找了下，还好还有gbk4subl这款能让sublime text 3支持GBK编码的插件。
插件下载：[https://github.com/jeewood/gbk4subl](https://github.com/jeewood/gbk4subl)
**Tradsim 中文繁字体和简体字转换**
Tradsim是一款能把中文的繁字体和简体字互相转换的插件，只支持UTF-8编码，虽然觉得有点鸡肋，但还是介绍出来给需要的朋友吧，安装插件，因为插件包里没快捷键文件，所以我们可以自定义一个，可以新建一个名字和后缀为Default (Windows).sublime-keymap的文件，添加以下代码，即可设置“转换为简体字”和“转换为繁字体 ”的快捷键了。注意：转换是文件里全局的中文。


```
[
    {"keys": ["ctrl+alt+s"], "command": "tradsim_to_sim"},
 
    {"keys": ["ctrl+alt+t"], "command": "tradsim_to_trad"}
]
```
**SideBarEnhancements 侧边栏增强**
SideBarEnhancements本是增强侧边栏的插件，这里将教大家如何用来做sublime text 3浏览器预览插件，并可自定义浏览器预览的快捷键。
![](http://ww3.sinaimg.cn/large/6941baebjw1ezbrtnejzpg20rg0c0npd.gif)
安装此插件，点击工具栏的preferences > package setting > side bar > Key Building-User，键入以下代码，这里设置按Ctrl+Shift+C复制文件路径，按F1~F5分别在firefox，chrome，IE，safari，opera浏览器预览效果，当然你也可以自己定义喜欢的快捷键，最后注意代码中的浏览器路径要以自己电脑里的文件路径为准。


```
[
    { "keys": ["ctrl+shift+c"], "command": "copy_path" },
    //firefox
    { "keys": ["f1"], "command": "side_bar_files_open_with",
             "args": {
                "paths": [],
                "application": "C:\\software\\Browser\\Mozilla Firefox\\firefox.exe",
                "extensions":".*" //匹配任何文件类型
            }
    },
    //chrome
    { "keys": ["f2"], "command": "side_bar_files_open_with",
            "args": {
                "paths": [],
                "application": "C:\\Users\\Mr.DenGo\\AppData\\Local\\Google\\Chrome\\Application\\chrome.exe",
                "extensions":".*"
            }
     },
    //ie
     { "keys": ["f3"], "command": "side_bar_files_open_with",
             "args": {
                "paths": [],
                "application": "C:\\Program Files\\Internet Explorer\\iexplore.exe",
                "extensions":".*"
            }
    },
    //safari
    { "keys": ["f4"], "command": "side_bar_files_open_with",
            "args": {
                "paths": [],
                "application": "C:\\software\\Browser\\Safari\\safari.exe",
                "extensions":".*"
            }
     },
     //opera
     { "keys": ["f5"], "command": "side_bar_files_open_with",
             "args": {
                "paths": [],
                "application": "C:\\software\\Browser\\opera\\opera.exe",
                "extensions":".*"
            }
    }
]
```
**ColorPicker 调色盘**
在编辑CSS样式的时候，要加个自己喜欢颜色或改改颜色啥的，要到PS里去调色？ColorPicker可以让sublime text 3内置一个调色盘，调好颜色，点击OK就会在光标处生成十六进制颜色代码。
![](http://ww3.sinaimg.cn/mw690/6941baebjw1ezbrtmk66jj20hs0b5tcb.jpg)
插件下载：[https://github.com/weslly/ColorPicker](https://github.com/weslly/ColorPicker)
**Hex-to-HSL-Color Hex颜色模式转HSL颜色模式**
在用CSS3的一些属性时会用到HSL颜色模式，用软件转来转去确实麻烦，用Hex-to-HSL-Color这个插件便可轻松解决，鼠标选中一个或多个十六进制颜色代码，按下Shift+Ctrl+U立马转换为HSL颜色模式。
![](http://ww3.sinaimg.cn/mw690/6941baebjw1ezbrtm6l6uj209z04wglv.jpg)
插件下载：[https://github.com/atadams/Hex-to-HSL-Color](https://github.com/atadams/Hex-to-HSL-Color)
**CSScomb CSS属性排序**
有时候看看自己写的CSS文件，会不会觉得属性很乱查找不易维护难？CSScomb可以按照一定的CSS属性排序规则，将杂乱无章的CSS属性进行重新排序。选中要排序的CSS代码，按Ctrl+Shift+C，即可对CSS属性重新排序了，代码从此简洁有序易维护，如果不款选代码则插件将排序文件中所有的CSS属性。当然，可以自己自定义CSS属性排序规则，打开插件目录里的CSScomb.sublime-settings文件，更改里面的CSS属性顺序就行了。因为这个插件使用PHP写的，要使他工作需要在环境变量中添加PHP的路径，具体请看[github](https://github.com/csscomb/CSScomb/wiki/Requirements)上的说明。
![](http://ww2.sinaimg.cn/mw690/6941baebjw1ezbrtlv15lj209a06vweu.jpg)
插件下载：[https://github.com/csscomb/CSScomb-for-Sublime](https://github.com/csscomb/CSScomb-for-Sublime)
**CSS Compact Expand CSS属性展开收缩**
每个人写CSS都有不同的风格，有些人喜欢写成一行，有些人喜欢写成多行，各有各的好处，我倒喜欢将CSS写成一行，这样能减少CSS文件大小，且屏幕能显示更多的Class方便查找。如果阅读别人的代码不符合自己的习惯，可以用CSS Compact Expand这个插件将CSS格式化一下，按 Ctrl+Alt+[ 收缩CSS代码为一行显示，按 Ctrl+Alt+] 展开CSS代码为多行显示，此插件由sublime textQQ群里一个牛人封装。
![](http://ww1.sinaimg.cn/mw690/6941baebjw1ezbrtlgj0uj20dw04i0t0.jpg)
插件下载：[http://pan.baidu.com/share/link?shareid=251331&uk=590661480&third=15](http://pan.baidu.com/share/link?shareid=251331&uk=590661480&third=15)
**JsFormat javascript格式化**
有时从网上扒了人家的js代码来学习学习，打开发现被压缩了，这时就可以用JsFormat插件格式化js代码，恢复未压缩时候的排版，挺给力的。按快捷键Ctrl+Alt+F即可格式化当前的js文件了。
插件下载：[https://github.com/jdc0589/JsFormat](https://github.com/jdc0589/JsFormat)
**javascript-API-Completions**
支持Javascript、JQuery、Twitter Bootstrap框架、HTML5标签属性提示的插件，是少数支持sublime text 3的后缀提示的插件，HTML5标签提示sublime text 3自带，不过JQuery提示还是很有用处的，也可设置要提示的语言。
![](http://ww4.sinaimg.cn/large/6941baebjw1ezbrtl1y5jg20o009onpe.gif)
插件下载：[https://github.com/Pleasurazy/Sublime-JavaScript-API-Completions](https://github.com/Pleasurazy/Sublime-JavaScript-API-Completions)
**SublimeTmpl 快速生成文件模板**
一直都很奇怪为什么sublime text 3没有新建文件模板的功能，像html头部的DTD声明每次都要复制粘贴。用SublimeTmpl这款插件终于可以解脱了，SublimeTmpl能新建html、css、javascript、php、python、ruby六种类型的文件模板，所有的文件模板都在插件目录的templates文件夹里，可以自定义编辑文件模板。
SublimeTmpl默认的快捷键


```
ctrl+alt+h html
ctrl+alt+j javascript
ctrl+alt+c css
ctrl+alt+p php
ctrl+alt+r ruby
ctrl+alt+shift+p python
```
如果想要新建其他类型的文件模板的话，先自定义文件模板方在templates文件夹里，再分别打开Default (Windows).sublime-keymap、Default.sublime-commands、Main.sublime-menu、SublimeTmpl.sublime-settings这四个文件照着里面的格式自定义想要新建的类型，这里就详细介绍了，请各位自己折腾哈~
插件下载：[https://github.com/kairyou/SublimeTmpl](https://github.com/kairyou/SublimeTmpl)
**Emmet html/CSS快速编辑（原名Zen Coding）**
Zen Coding估计大家都不会陌生，前不久改名为Emmet了，虽然用Emmet编辑html很快，但是要用好用快它需要付出不小的学习成本，学习的曲线有点陡峭，以至于让新手好奇而畏惧，我看看热闹就行了，感觉编辑得再快思维跟不上也是白搭，对我来说sublime text 3自带的代码提示够用了。网上有很多教学视频，有兴趣学习的可以去了解下。
![](http://ww4.sinaimg.cn/mw690/6941baebjw1ezbrtjv4q3g20b8099tbu.gif)
插件下载：[https://github.com/sergeche/emmet-sublime](https://github.com/sergeche/emmet-sublime)
**sublimeLinter 代码错误提示**
好话写在前面：sublimeLinter是少数几个能在sublime text 3工作的代码检查插件，sublimeLinter能检查html、css、javascript、php等众多语言的错误代码并给出提示，前提是需要配置相应语言的环境，要检查js代码需要安装node.js，检查php代码需要安装php并配置环境等，用这个插件能及时帮我们纠正代码的错误，并培养我们良好的编码习惯和风格。
丑话说在后面：对这货“强制性”的代码风格检查有点不爽，有意义无意义的代码提示经常打乱我敲代码的节奏，并且在后台运行检查机制造成sublime text 3有点小卡顿，虽然部分问题可以设置sublimeLinter来解决，用了sublimeLinter一小段时间后还是放弃了，觉得代码提示对我应该没必要了。
![](http://ww4.sinaimg.cn/mw690/6941baebjw1ezbrtjmb4aj20i2095jsn.jpg)
插件下载：[https://github.com/SublimeLinter/SublimeLinter/tree/sublime-text-3](https://github.com/SublimeLinter/SublimeLinter/tree/sublime-text-3)
以上插件只是个人觉得好用，各位就择你所好而用之，在win8下sublime text 3中测试工作正常，点击这里还可以查看更多[sublime text 3支持的插件](https://github.com/wbond/sublime_package_control/wiki/Sublime-Text-3-Compatible-Packages)。细心的朋友也发现很多插件的快捷键重叠了，各位就依自己所好设置就行了。最后注意的是在github上下载插件时先看看插件有没有明确说明支持sublime text3，如果没有看看branch分支有没有st3版本的插件，不要下载错了插件版本。
