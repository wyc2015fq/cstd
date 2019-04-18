# WSL 下的好终端 - wsltty - 知乎
# 



Win 10 的 WSL 默认使用原生的 cmd.exe 的窗口来做终端，用过的人都知道，cmd.exe 作为终端还有很多显示问题，特别是当你使用 tmux 或者用 ncurses 开发的文本界面程序的话，经常会出现字符错乱问题：
![](https://pic1.zhimg.com/v2-7fcd1ca6f7e20c413584f1a598c19cc8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1223' height='639'></svg>)
上图中，使用默认的 cmd.exe 窗口运行 tmux 进行左右分屏，用一会就会出现显示错乱，注意底部的tmux状态栏。

因此最佳的体验，当然是按上文《[WSL 服务自动启动的正确方法](https://zhuanlan.zhihu.com/p/47733615)》所说的配置 WSL 下面的 sshd服务，用终端模拟器链接上去，当远程服务器一样的使用。

除此之外还有一个不错的选择，使用 [wsltty](https://link.zhihu.com/?target=https%3A//github.com/mintty/wsltty)，注意更新到 1.9.3 以后的版本。注意不同的 wsl 发行版的启动方式是：

`%LOCALAPPDATA%\wsltty\bin\mintty.exe --WSL=Debian --configdir="%APPDATA%\wsltty" -~`

桌面上新创建一个快捷方式，将命令行复制过去，更改下图标，并且修改 "--WSL=?" 字段，双击图标即可启动对应的 wsl/linux 发行版。

wsltty 可以设置背景图，我们编辑：%APPDATA%/wsltty/config 文件，在最后加一行：

`Background=_/mnt/c/Users/XXXXX/sierra.bmp,39`

可以给我们的 WSL 设置一张背景图片，Background 设置中，开头的下划线代表图片的显示方式是自动缩放，铺满窗口，后面是一个图片的路径名，路径名为 /mnt/c/ 开头的 wsl 的路径格式，逗号后面的 39 代表 darken，将图片调暗，不然太亮没法看。

最终效果是：
![](https://pic4.zhimg.com/v2-81b23e4e3673e1948ea7ff032cae108f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='497'></svg>)
以及：
![](https://pic3.zhimg.com/v2-6d1c050529dd57738eb8bf3aa96dc976_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='497'></svg>)
加了背景图片的 WSL，使用起来舒服不少，好像在 mac 下面使用 iTerm2 的感觉一般，我喜欢在 wsltty 内开一个 tmux，左右分屏，开多窗口，用起来十分高效：
![](https://pic3.zhimg.com/v2-9f16dcdd2d64bc074ff8d595a9fa30ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1920' height='1030'></svg>)
注意 Vim 中如果要继续保持背景透明的话，需要把 Normal 和 NonText 的背景改为 None：


```
hi Normal guibg=NONE ctermbg=NONE
hi NonText guibg=NONE ctermbg=NONE
```


这样 Vim 的背景也能透明了，注意这和把终端窗口设置成半透明的效果完全不一样，注意 Vim 和 tmux 的状态栏和文字，如果单纯把窗口设置成半透明，那状态栏和文字也会跟着一起半透明，导致你根本看不清楚了。

而设置背景，可以保证文字和状态栏这些东西继续保持不透明，只有普通背景的地方才能看到图片的内容。

有了背景图片，可以提升你在 WSL 中工作的侵入式体验，工作起来更加专注，写起代码来更加神清气爽，iTerm2 最让我留恋的地方已经能够在 WSL 里体会到了。


