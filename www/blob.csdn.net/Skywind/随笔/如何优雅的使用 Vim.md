# Skywind Inside » 如何优雅的使用 Vim
## 如何优雅的使用 Vim
June 20th, 2017[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
根据 Bram 前后几个关于高效使用 Vim的视频，大家每天需要花很多时间来编辑：代码、文档、邮件、日志 等等，除去这些外，还要分时间参加会议和人沟通，每个人的时间却都是不够的，优雅使用 Vim 无外乎：
- 检测不高效的地方：你的整个工作流里，什么地方比较浪费时间？
- 寻找一个更快的方式：官方文档，学习他人经验，自己编写 VimScript
- 使它习惯化：开始使用，并且不断完善
以上三点反复循环，能让你的 Vim 越来越顺手。所以重点是根据自己的工作流不断迭代。而不是象大部分教程那样教你安装一大堆插件。插件都是别人写的为了解决通用需求而提炼的东西，和每个人的具体需求都有差别。上面这三点我屡试不爽，随着时间增长，有种越来越顺手的感觉，举几个我具体碰到的例子：
#### 问题1：边开发边参考网上解决方案的问题
比如碰到问题搜到一段代码，需要试一下，一会又看会 Chrome ，一会又切回 GVim 里去写代码，反复 ALT*TAB，有时候中间使用了一下资源管理器或者其他程序，ALT*TAB 的顺序就会被打乱，你一切换就切跑了，十分低效。
于是我用 VimScript + 内嵌 Python 写了一个功能，按快捷键可以让 GVim 在透明/不透明两种状态间自由切换：
![](https://skywind3000.github.io/word/images/vim-demo-2.png)
就是 VimScript 简单封装一个函数，里面用内嵌 Python 找到 GVim 的顶层 HWND，并设置透明度。平时默认不透明，需要参考其他资料时切换成透明，参考完了又快捷键切换回来，感觉比缘来切来切去顺畅很多。
#### 问题2：浏览文档时的窗口滚动问题
比如你在抄写或者改写一段代码，窗口分为左右两个，左边是你引用参考的源代码，右边是你正在编辑的源代码。你抄着抄着，抄到左边最后一行了，或者你想前后看看正在引用的文本，你就需要将焦点从右边切换到左边，滚动，再切换交点回来，十分麻烦，于是撸一小段 VimScript 来解决这个问题：
```text
" 0:up, 1:down, 2:pgup, 3:pgdown, 4:top, 5:bottom
function! Tools_PreviousCursor(mode)
    if winnr('$') <= 1
        return
    endif
    noautocmd silent! wincmd p
    if a:mode == 0
        exec "normal! \<c-y>"
    elseif a:mode == 1
        exec "normal! \<c-e>"
    elseif a:mode == 2
        exec "normal! ".winheight('.')."\<c-y>"
    elseif a:mode == 3
        exec "normal! ".winheight('.')."\<c-e>"
    elseif a:mode == 4
        normal! gg
    elseif a:mode == 5
        normal! G
    elseif a:mode == 6
        exec "normal! \<c-u>"
    elseif a:mode == 7
        exec "normal! \<c-d>"
    elseif a:mode == 8
        exec "normal! k"
    elseif a:mode == 9
        exec "normal! j"
    endif
    noautocmd silent! wincmd p
endfunc
```
把这个函数绑定到 ALT*U, ALT*D 两个按键上，你正在编辑着当前文档时，不用退出 INSERT 模式，更不用切换窗口交点，直接 ALT*U, ALT*D，就可以上下滚动正在参考的文档内容了，有了这个改进后，我的工作又高效了那么一点点。
同理，Quickfix 窗口经常用来查看编译错误，或者 Grep 结果，我也写了一个专门针对 Quickfix 窗口的滚屏函数，不用切焦点随时浏览 Quickfix 内容。
#### 问题3：Tag Preview 十分低效
默认 CTRL-] 可以跳转到 Tag 定义处，但有时候我就像看一眼，并不像把当前文档给切走，也不想预览下其他文件就开一大堆文件，污染 buffer list。后来发现一个快捷键 `<C-W><C-]>` 可以打开预览窗口预览符号定义。
用了一段时间又发现两个问题：每次 `<c-w><c-]>` 如果有多处定义，vim会打印显示一大串定义位置，让你选择要看哪个，比较讨厌，而且开始预览时是默认上下分屏，现在都是宽屏显示器时代了，上下分屏十分影响观感。于是自己用 VimScript 重新撸了 Tag Preview：
![](https://skywind3000.github.io/word/images/vim-demo-3.png)
需要预览时按下 “ALT+;”，右边弹出预览窗口，并且高亮符号名称，下面显示该 tag一共有 3处定义，现在正在查看第1个定义，连续按下 “ALT+;” 可以将右边预览窗口切换到下一处定义，切换到最后一个了又会折头到第一个。
配合前面的当前窗口控制其他窗口滚动的代码，编辑模式下直接 “ALT+;” 打开 tag预览并切换到你想要的定义处，然后 ALT+U, ALT+D 上下滚动预览窗口来查看前后内容，看完了 “ALT+SHIFT+;” 就可以关闭预览窗口。
从开始到结束不需要切换窗口交点，更不需要切换编辑模式，还没有 `<c-w><c-]>` 乱七八糟的列表选择过程，使用一个星期以后，彻底淘汰原生的 tag功能了，感觉自己的流畅度又提升了一些。
具体代码见：
[https://github.com/skywind3000/vim/blob/master/autoload/asclib.vim#L365](https://github.com/skywind3000/vim/blob/master/autoload/asclib.vim#L365)
后面继续迭代，有时在函数调用时候，想不起该函数的参数了，需要查看一下原型（声明和参数），不需要打开整个预览窗口查看实现，我就又开发了一个查看函数原型的 VimScript 函数，绑定到 ALT_Q 上，同样不需要退出插入模式：
![](https://skywind3000.github.io/word/images/vim-demo-4.png)
按 ALT*Q 可以循环显示该函数原型的多处定义，就在最下面一行，连 Preview 窗口都不需要。有的自动补全插件可以显示函数的原型，比如 YCM，但是支持的语言很少，你换个语言，YCM就不能帮你显示原型了，这时 ALT*Q 来查看原型支持多达 85 种 Universal Ctags 支持的语言，基本满足我的需求，我平时主要用的编程语言大概有 4种，如今不管写什么但凡想不起定义来的时候，ALT_Q 一下就出来了，简洁明了。
#### 问题4：代码片段
个人编码习惯，经常输入一些格式化注释：
```text
//---------------------------------------------------------------------
// 这里是注释
//---------------------------------------------------------------------
```
以前基本都是复制粘贴，或者重新敲一遍那么多减号，每次都觉得有些麻烦。久而久之就想有没有更好点的方法，于是继续编写 VimScript 用来快捷键一键生成我要的这个注释框，顺畅了不少，又进一步添加了不少一键生成的文字模版，比如 copyright 信息，简单的一个带 main函数和一些 include 的代码框架。
期间发现了 UltiSnip ，是比较好用，就是比较慢，打开新文件速度慢了不少，所以我目前还是使用自己的 VimScript 实现一些简单模版的代码片段快速插入。
#### 问题5：加速 Cygwin 开发
调试跨平台代码时，Cygwin 是一个离不开的东西，同一个功能，我一会需要用 MinGW 编译测试一下，一会又需要用 Cygwin 测试一下。传统编译 Cygwin 无外乎打开 Cygwin 的终端窗口，调用 make 或者 gcc 编译当前代码，然后又运行，窗口切来切去，十分别扭，于是开始想，有没有可能同时在 GVim 里面快捷完成 MinGW 和 Cygwin 的一键编译和运行呢？
之前比较麻烦，但是 Vim 8.0 和 NeoVim 发布后提供了后台任务机制，于是我撸了个插件 ：AsyncRun：https://github.com/skywind3000/asyncrun.vim
使用 “:AsyncRun ” 来用后台运行的模式代替原先的阻塞式的 “！” 命令。
然后设置了 F7 编译 MinGW 程序：
```text
:noremap <F7> :AsyncRun gcc "%" -o "%<" <cr>
```
并设置了 F8 编译 Cygwin 程序：
```text
:noremap <F8> :AsyncRun d:\cygwin\bin\bash --login -c "gcc % -o %<"
```
继续设置了其他快捷键用于打开 cmd窗口运行刚才的 MinGW程序或者 Cygwin 程序
这样不切换 Vim，不用打开 Cygwin 窗口，不用敲命令，一个快捷键，完成了 Cygwin 的编译，自此调试跨平台程序顺畅了很多。
#### 问题6：加速 Android NDK 开发效率低
先前我在开发 Android 下 OpenSLES 的相关功能时，先要写个 NDK工程，又需要命令行编辑，然后导到 Eclipse 里面或者 Android Studio 里面，写两行 java ，做一个 gui 来调用刚才 NDK生成的 .so 动态库，十分别扭，每次改一个地方就需要折腾一半天才看得到效果，使用频率最高的：“编辑-编译-运行” 循环如果无法做到足简短紧凑的话，平时工作急不死你。
于是我开始考虑，对于大部分非 GUI的 NDK功能，有没有可能象开发调试 Windows 命令行一样的只需要写 C/C++，不需要外面套一层 java gui，并且，继续象 MinGW/Cygwin 一样的做到一键编译，一键运行呢？
于是结合前面的 AsyncRun 插件和一些少量的 Python + VimScript 脚本，实现了编译和运行：
![](https://skywind3000.github.io/word/images/vim-demo-5.png)
F9 按 Android 命令行程序编译 NDK代码或者工程，并异步调用 adb 上传到设备的 /data/local/tmp/ 下设置好 755
F10 打开 cmd窗口，调用 adb 运行刚才编译的文件，然后退出。
支持模拟器和真实设备，最终 android下面整个 “编辑-编译-运行” 的工作流比以前缩短了很多，跟开发PC命令行程序一样的开发 Android NDK 程序，比起以前快多了。
类似的功能还有很多，比如玩 Prolog 时，Vim 里面写好了程序，不想每次打开 SWI-Prolog 的 GUI窗口，手动在那里加载半天，程序修改完又要在 SWI-Prolog 里敲一堆命令来复位加载运行，后来也直接集成了一个快捷键完成这些事情。
不用换一个语言就换一个 编辑器/IDE 那么麻烦，那么多不同语言不同平台的开发任务，全部都用 Vim 解决了，还跟原生的一样。
#### 问题7：高效的 Grep
在工程中查找字符串，对于特定语言使用 gtags / cscope 之外，对于通用语言，传统的 grep 用的也比较多。传统 vim 内 grep 不支持异步， 需要等待 Grep 结束后才能返回，并没有很好的跟进 Vim8/ NeoVim 的异步机制，于是用我前面前面定义的 AsyncRun 插件来配合使用：
```text
:AsyncRun! grep -R word . 
:AsyncRun! grep -R <cword> .
```
其中 `<cword>` 的意思是当前光标下的符号，再把该功能绑定到快捷键上，一键即可在当前目录下 Grep 光标下面的符号，结果实时输出到 Quickfix 窗口，双击 Quickfix 的具体输出就可以打开对应文件跳转到对应行号。
#### 问题8：日常小工具
之前学习做爬虫时，经常从网上复制一些 html 片段，都是挤在一起的，需要格式化后查看的更清楚一点，类似这种小功能，基本懒得再去找专门的格式化工具，三分钟几行 VimScript 就搞定了：
```text
function! asclib#html_prettify()
    if &ft != 'html'
        echo "not a html file"
        return
    endif
    silent! exec "s/<[^>]*>/\r&\r/g"
    silent! exec "g/^$/d"
    exec "normal ggVG="
endfunc
```
将该函数绑定到快捷键上后，粘贴 html 到 vim 里一键就完成了格式化工作。我在 Vim 里写了大量类似的小工具，比如GTAGS支持（GTAGS官方插件太难用，又不支持Vim8的异步模式），比如一键 touch 当前项目 wsgi 的主文件，源代码头文件快速切换（有一个类似插件，但是我嫌它太难用了），快速打开 Zeal 查看 Dash 文档。。。。。
这些都是围绕自己工作习惯来优化的，这些具体的问题都不是拼凑通用插件能解决的。编写 VimScript 是一件值得花时间学习的技巧，当你熟练掌握后，你就脱开了各种插件的束缚，能够让你的 Vim 日常使用得以不断的进化再进化，这远远不是堆砌插件所能比的效果。
#### 回顾一下：
磨刀不误砍柴工，熟练掌握 VimScript，熟读官方文档的情况下，坚持：“发现问题，找到更高效的方法，习惯化” 的三个循环，让我工作流在不同地方都变的优雅和流畅了那么一点点，日积月累，流畅的编辑体验让身心愉悦。
其他编辑器/IDE的效率是常数，上手以后效率就很难提升了，Vim效率是变量，越调教越顺手。它的反面，低效的习惯是：
- 你已经学会了编辑器的基本使用方法，却不花间去学习新的技巧的话，将永远停留在最原始的使用方式，得不到提高。
- 你想要学习一切技巧，让自己每个步奏都用最优方式解决的话，你又会浪费大量时间，学习一大堆永远用不上的东西。
- 摆弄各种插件，插件能做的可以做一下，插件不能做的或者做的不太好的，束手无策。
#### 更多参考：
《Practical Vim》，推荐学完 vimtutor 开始使用时阅读。
《Learn Vimscript the Hard Way》，VimScript 上手必备。
官方必读1：h index （Vim 中所有默认键位说明）
官方必读2：h quickref（速查手册）
扩展你的键位：《[Vim 中正确使用 Alt映射](http://www.skywind.me/blog/archives/1846)》
超脱堆插件的误区，根据你的日常工作情况，不断打磨编辑器的利爪吧。
