# Skywind Inside » 终端软件里正确设置 ALT 键和 BACKSPACE 键
## 终端软件里正确设置 ALT 键和 BACKSPACE 键
February 23rd, 2018[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
不管你在终端下使用 vim/neovim, emacs, nano 或者 zsh，你都会碰到使用 ALT 键的情况（终端下叫做 meta键），而由于历史原因，大部分终端软件的默认设置都无法正确使用 ALT 键。
要在终端下正确使用 ALT键最简单的做法是：首先将终端软件的 “使用 Alt键作为 Meta键” 的功能打开，意思是如果你在终端下按下 ALT+X，那么终端软件将会发送 `<ESC>x` 两个字节过去，字节码为：0x27, 0x78。
**SecureCRT：终端设置**
![](https://skywind3000.github.io/word/images/vim_altmap_2.png)
**XShell4 终端设置：**
![](https://skywind3000.github.io/word/images/vim_altmap_1.png)
其他终端软件里：
- Putty/MinTTY 默认ALT+X 就是发送 `<ESC>x`过去
- Mac下面的 iTerm2/Terminal.app 需要跟 XShell / SecureCRT一样设置一下
- Ubuntu 下面的 GnomeTerminal 默认也是发送 `<ESC>x`过去的
- 任意平台下面的 xterm 可以配置 `~/.Xdefaults` 来设置这个行为。 
这样的话，终端里的软件就能识别你的 ALT 组合键了，设置好以后，你可以在终端下使用命令：
```text
showkey -a
```
来查看自己的设置正确不，是不是按下 ALT_a 后正确发送了 0x1b, 0x61 两个字节过去了？
**功能键超时**
那么终端里按下 ALT*X 和先按 ESC 键再按 X键有什么区别呢？答案是没有区别，你在 emacs 中快速先后按下 ESC 和 v 的话，emacs 就会以为你按了 ALT*v，然后给你来个上翻页了。
远程主机一般靠超时来区别到底是 ALT*X 还是先按 ESC 再按 X 键，如果你 100 毫秒内先后发送了 ESC 和 X 过来，远程主机会识别成 ALT*X 键，否则识别成 ESC 和 X 两个键，这个超时时间可以设置，一般设置成 100ms 或者 50ms ，但不要低于 25ms，否则网络一卡可能有概率会判断错误。
在 Vim/NeoVim 中你可以通过 ttimeoutlen 来设置功能键超时检测为 50 毫秒，比如：
```text
:set ttimeout ttimeoutlen=50
```
而 tmux 中，也有类似配置，比如：
```text
set-option -g escape-time 50
```
就连 ncurses 库中也有类似功能键超时检测的设置，来区别到底是功能键/组合键，还是数个单独的按键。这里你可能觉得这样靠超时检测很不可靠，那你要问 VT100, VT220 的标准制定者了。不过我个人常年设置成 50 毫秒后，连接到各种国内外速度不一的主机上并没有被网速慢把功能键卡成两个键的情况，应该是终端软件中本身也在尽量保证同一组按键序列能够尽量同时发送，所以我链接到国外 rtt=400 左右的主机上工作时，尽管网速经常不稳定，也没有发生错误识别的问题，那其他网速正常的主机就更不用担心了。
**更友好的终端设置**
上面在 SecureCRT / XShell 中设置了将 alt 键作为发送 +ESC x 的 meta 键后，你会发现，终端软件中固有的一些 ALT 组合键全部失效了，比如原来在终端中 ALT*1 到 ALT*9 可以切换终端的 TAB，ALT_B 可以打开链接管理器，这下都全部用不了了，这是件比较坑爹的事情，能不能保留有限的几个 ALT 组合键给终端软件使用，剩下的全部当作 meta 键呢？答案是可以的，先取消终端里 ALT 当作 meta 键的设置，恢复成默认状态，然后打开终端软件 keymap 设置窗口，将你不需要保留的 ALT 组合键全部设置成发送 +ESC x 字符串。
那么一个个设置可能有些麻烦，对于 SecureCRT 的话，我生成了一个配置文件：
```text
A   VK_A                    "\033a"
A   VK_D                    "\033d"
A   VK_E                    "\033e"
A   VK_G                    "\033g"
A   VK_H                    "\033h"
....
AS  VK_A                    "\033A"
AS  VK_B                    "\033B"
AS  VK_C                    "\033C"
AS  VK_D                    "\033D"
```
可以到 [这里](https://github.com/skywind3000/vim/blob/master/tools/conf/securecrt.key) 下载现成的，在 keymap editor 窗口中加载进去即可。
这份 keymap 配置除了保留了 SecureCRT 常用的 ALT*1 – ALT*9 ，ALT*B, ALT*R 和 ALT*I 外，其他的 alt 组合都设置成了 +ESC x 的 meta 键序列。并且将 ALT*SHIFT*1 到 ALT*SHIFT_9 映射到了终端里的 +ESC 1 到 +ESC 9 ，也就是说你的 ALT+数字 被保留给软件切换TAB用了，而 ALT+SHIFT+数字 被映射成了终端链接中的 ALT+数字，这样在终端里碰到需要 ALT+数字 的地方，可以用 ALT+SHIFT+数字 来代替。
设置好以后，可以继续使用 Linux 下的 `showkey -a` 命令查看一下是否正常。
**Vim里识别 ALT 键**
前面在终端软件里配置好 ALT键，但是 Vim 的话，由于历史原因，需要在你的 vimrc 里加一段键盘码配置：
```text
function! Terminal_MetaMode(mode)
    set ttimeout
    if $TMUX != ''
        set ttimeoutlen=30
    elseif &ttimeoutlen > 80 || &ttimeoutlen <= 0
        set ttimeoutlen=80
    endif
    if has('nvim') || has('gui_running')
        return
    endif
    function! s:metacode(mode, key)
        if a:mode == 0
            exec "set <M-".a:key.">=\e".a:key
        else
            exec "set <M-".a:key.">=\e]{0}".a:key."~"
        endif
    endfunc
    for i in range(10)
        call s:metacode(a:mode, nr2char(char2nr('0') + i))
    endfor
    for i in range(26)
        call s:metacode(a:mode, nr2char(char2nr('a') + i))
        call s:metacode(a:mode, nr2char(char2nr('A') + i))
    endfor
    if a:mode != 0
        for c in [',', '.', '/', ';', '[', ']', '{', '}']
            call s:metacode(a:mode, c)
        endfor
        for c in ['?', ':', '-', '_']
            call s:metacode(a:mode, c)
        endfor
    else
        for c in [',', '.', '/', ';', '{', '}']
            call s:metacode(a:mode, c)
        endfor
        for c in ['?', ':', '-', '_']
            call s:metacode(a:mode, c)
        endfor
    endif
endfunc
call Terminal_MetaMode(0)
```
然后你就可以正确在 Vim 映射 ALT 键了，具体原理见 `:help set-termcap` 以及：
[http://www.skywind.me/blog/archives/1846](http://www.skywind.me/blog/archives/1846)
其他的诸如 emacs, nano 和 neovim 等都不需要额外设置。
**终端里正确设置 BS 键**
还是 VT100 的历史原因，BACKSPACE 键和 CTRL-H 给混淆起来了，默认情况下，终端里不管按 CTRL-H 还是 BACKSPACE 时都是发送 ASCII 码为 0x08 的 `^H` 过去。导致我们想在 Vim/Emacs 中映射 CTRL-H 去干别的事情时会影响到 BACKSPACE 键的使用。
因此得按照 VT220 的新标准修改一下 BACKSPACE 的设置，让它发送 ASCII 码 0x7f 即 `^?` 过去：
- **SecureCRT**: Session Options -> Terminal -> Emulation -> Mapped Keys, 勾选 Backspace sends delete
- **XShell**: Properties -> Terminal -> Keyboard 里，把`<BS>`设置成 127，而 `<DEL>`设置成 VT220 Del
- **Putty**: 好像默认是 ^? 的不过需要到：Configuration -> Terminal -> Keyboard 下面下确认下 The Backspace key 是 Control-? (127)
- **Terminal.app**: 好像默认是发送 `^?` 的，你也可以到 Profiles Advanced 下面确认下 “Delete sends Control-H” 没有勾选。
- **iTerm2**: 默认也是发送 ^? 的，可以到 Profiles -> Keys下面确认一下 “Delete key sends ^H” 没有被勾选。
- **Gnome-Terminal**: 默认发送 ^? 的，参见具体文本配置文件。
- **MinTTY**: 设置 vt220/xterm 的话，默认发送 ^? 的，似乎还不能改。
你要深究原因的话，可以见：
[http://www.skywind.me/blog/archives/1857](http://www.skywind.me/blog/archives/1857)
修改好以后可以继续运行：
```text
showkey -a
```
检查一下，你的 BACKSPACE 键被按下时是否正确发送了 0x7f 字符过去。设置成功的话，终端下 CTRL-h 和 backspace 就不会出现混淆问题了。
