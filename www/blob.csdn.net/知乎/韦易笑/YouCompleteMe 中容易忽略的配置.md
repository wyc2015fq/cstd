# YouCompleteMe 中容易忽略的配置 - 知乎
# 



YCM 版本演进很快，以前是半异步（偶尔会卡），2017年中进化成全异步（完全无等待了），网上很多针对老版本的配置方法也都是有问题的，并不能发挥出 YCM 的最佳水平，下面列举额一些容易被忽略的配置问题：




## **语义补全自动触发**

大家都知道 YCM 有语义补全，但默认语义补全是要你输入 . 或者  -> 或者 :: 后才会弹出：
![](https://pic4.zhimg.com/v2-d3705f4503dc7a47a0a8dd3c565b7897_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='521'></svg>)
上图中 time 后面输入了点以后才会弹出基于语义的补全，而默认状态下即便你设置了：

`let g:ycm_min_num_identifier_candidate_chars = 2`

两个字符以后自动弹出的是基于符号的补全（当前所有 buffer 中收集到的符号），不是基于语义的补全，所以前面没有输入过 printf 的话，接下来输入了 pri 也不能自动语义补全为 printf：
![](https://pic1.zhimg.com/v2-bd811cdfd2930e9448d519f0fc949704_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='521'></svg>)
其实 YCM 里触发语义补全有一个快捷键：

`let g:ycm_key_invoke_completion = '<c-space>'`

默认是用 CTRL+SPACE 来触发补全的，中文操作系统下，CTRL+SPACE被系统劫持用作输入法切换，无法正确传到终端，所以一般要改成 CTRL+Z：

`let g:ycm_key_invoke_completion = '<c-z>'`

好了，那么我们输入到 pri 的时候按下 CTRL+Z，语义补全就弹出了：
![](https://pic1.zhimg.com/v2-b7d65848b6ea85363630e8e14aa50098_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='521'></svg>)
前面设置基于符号的补全，只需要输入两个字符，就会自动弹出，但它相对语义补全有三个不足：
- YCM需要在所有打开的文件中收集符号，除非前面写过 printf，或者同时打开的文件里有 printf，YCM才能正确进行符号补全，否则首次出现的东西 YCM不知道该怎么办。
- 编码时首次出现的符号才是我们最需要补全的东西，前面都写过了，还要你符号补全作甚？如果你第一次写了printf 的头三个字母，没有语义补全的话 YCM 就傻了。
- 符号补全只有一个符号名，不能像语义补全一样给出参数格式，返回值等丰富信息。

所以符号补全就是傻瓜猜测，语义补全才是我们最想要的东西，那为啥 YCM必须要输入了：. 或者 -> 或者 :: 才能弹出语义补全呢？对于顶层符号（比如 printf）的语义补全为啥必须要我按一个快捷键（上面设置的 CTRL+Z）呢？

因为 2017年 8月之前 YCM 还是半异步模式，符号补全很快，默认基本都可以自动弹出，而语义补全经常卡半天。所以为了避免用户每敲个字母就等待半天的尴尬，YCM 的语义补全一直使用被动触发（输入 ->或 . 或 ::，或者按 CTRL+SPACE/Z），这种方式解决了输入不流畅问题，但也带来了操作麻烦问题。

而 2017年 8月后的版本，请求发送过去，使用 timer 轮询结果，有结果才弹出，没返回也不影响输入，从这个版本开始 YCM 才进入了全异步时代，体验基本赶上 emacs 补全（emacs很早就有异步机制）和各种 IDE。

既然 YCM 进入了全异步模式，语义补全不会卡到用户输入了，那么可不可以不需要用户主动触发就自动弹出呢？在你的配置中加上这个语义触发的条件即可，默认的语义补全触发条件是  ycm_semantic_trigger ：


```
let g:ycm_semantic_triggers =  {
  \   'c' : ['->', '.'],
  \   'objc' : ['->', '.', 're!\[[_a-zA-Z]+\w*\s', 're!^\s*[^\W\d]\w*\s',
  \             're!\[.*\]\s'],
  \   'ocaml' : ['.', '#'],
  \   'cpp,objcpp' : ['->', '.', '::'],
  \   'perl' : ['->'],
  \   'php' : ['->', '::'],
  \   'cs,java,javascript,typescript,d,python,perl6,scala,vb,elixir,go' : ['.'],
  \   'ruby' : ['.', '::'],
  \   'lua' : ['.', ':'],
  \   'erlang' : [':'],
  \ }
```


代表在 C/C++ 下面，用户除了手工 CTRL+SPACE/C 外，写代码的时候必须输入：. 或 -> 或 :: 才能弹出语义补全，下面我们在自己的 vimrc 中加入如下额外设置：


```
let g:ycm_semantic_triggers =  {
			\ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
			\ 'cs,lua,javascript': ['re!\w{2}'],
			\ }
```


vimrc 中定义了 g:ycm_semantic_triggers 以后，默认的 . / -> / :: 是不会被覆盖的，只会追加成新的 trigger，这里我们追加了一个正则表达式，代表相关语言的源文件中，用户只需要输入符号的两个字母，即可自动弹出语义补全：

`['re!\w{2}']`

就是上面这个，括号里面的数字可以修改，这里我们默认输入两个字母才弹出。好了，接下来重启 Vim ，跑到刚才我们的小程序那里，输入 pr 后不用按任何键就自动弹出语义补全了：
![](https://pic1.zhimg.com/v2-4879f29ead6bc5f6a3ad9448d2944280_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='521'></svg>)
全异步机制的 YCM 搭配 g:ycm_semantic_triggers 设置，终于让 YCM 的补全效果如丝般顺滑了，自动语义补全使整个编码的工作流顺畅不少。

注意升级你的 VIM 为 8.0 以后，YCM到最新版。Windows 下的32位版本 YCM 你不用自己编译，我一直在维护更新，可以到这里下载：

[韦易笑：如何在 Windows 下使用 Vim 的 YouCompleteMe 插件？](https://www.zhihu.com/question/25437050/answer/95662340)

而 Linux 下面的 YCM 安装相对比较自动化一点，直接跑 [install.py](https://link.zhihu.com/?target=http%3A//install.py/) 即可。




## 补全对话框的颜色修改

不少人觉得 Vim 自动补全的弹出窗口默认配色很丑：
![](https://pic4.zhimg.com/v2-da9d58c77755a5f60ec538e1ac40d273_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='521'></svg>)
如果你和我一样想把它改成上面比较素雅的灰色的话，可以自己定义 highlight：


```
highlight PMenu ctermfg=0 ctermbg=242 guifg=black guibg=darkgrey
highlight PMenuSel ctermfg=242 ctermbg=8 guifg=darkgrey guibg=black
```


这个配置是我上面的灰色配置，xterm 下的配色见：

[Xterm, HEX, RGB, HSL](https://link.zhihu.com/?target=https%3A//jonasjacek.github.io/colors/)

对照这个表格你还可以改成你喜欢的颜色。




## 屏蔽一些不喜欢的 YCM 功能

我不喜欢 YCM 自动弹出函数原型预览窗口，它搞乱我的布局，我有其他方法查看函数的原型，如果你和我一样想关闭该功能的话，增加两行配置：


```
set completeopt=menu,menuone
let g:ycm_add_preview_to_completeopt = 0
```


这样讨厌的预览窗口就不会不经过我同意随便乱弹了。

YCM默认会显示诊断信息，语言标注出来你代码问题，我一般不需要，要分析问题我有其他手段，我更喜欢我原型编译或者静态检查工具以后，你再给我标注出问题来，你如果也想屏蔽 YCM 的诊断信息，你可以设置：

`let g:ycm_show_diagnostics_ui = 0`

这样你可以用其他插件来完成自动/非自动代码静态检查。




## 最终配置

好吧，下面是最终的 YCM 配置：


```
let g:ycm_add_preview_to_completeopt = 0
let g:ycm_show_diagnostics_ui = 0
let g:ycm_server_log_level = 'info'
let g:ycm_min_num_identifier_candidate_chars = 2
let g:ycm_collect_identifiers_from_comments_and_strings = 1
let g:ycm_complete_in_strings=1
let g:ycm_key_invoke_completion = '<c-z>'
set completeopt=menu,menuone

noremap <c-z> <NOP>

let g:ycm_semantic_triggers =  {
			\ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
			\ 'cs,lua,javascript': ['re!\w{2}'],
			\ }
```


最后建议设置一下：g:ycm_filetype_whitelist 这个白名单，避免编辑白名单外的文件类型时 YCM也在那分析半天，比如你打开个 1MB 的 TXT 文件，YCM还要再那里空跑半天就傻了：


```
let g:ycm_filetype_whitelist = { 
			\ "c":1,
			\ "cpp":1, 
			\ "objc":1,
                        。。。。
			\ "sh":1,
			\ "zsh":1,
			\ "zimbu":1,
			\ }
```


好了，祝各位使用愉快。




