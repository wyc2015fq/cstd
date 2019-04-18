# Skywind Inside » Vim 中正确使用 Alt映射
## Vim 中正确使用 Alt映射
November 19th, 2016[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
最简单的做法是：首先将终端软件的 “使用 Alt键作为 Meta键” 的功能打开，其次将 Alt的模式改为 **ESC+字母**，意思是如果你在终端下按下 ALT+X，那么终端软件将会发送 `<ESC>x` 两个字节过去，字节码为：0x27, 0x78。如果你使用过 NeoVim 或者 Emacs的话，这一步应该早就做过了。
**XShell4 终端设置：**
![](https://skywind3000.github.io/word/images/vim_altmap_1.png)
**SecureCRT：终端设置**
![](https://skywind3000.github.io/word/images/vim_altmap_2.png)
其他终端软件里：
- Putty/MinTTY 默认ALT+X 就是发送 `<ESC>x`过去
- Mac下面的 iTerm2/Terminal.app 需要跟 XShell / SecureCRT一样设置一下
- Ubuntu 下面的 GnomeTerminal 默认也是发送 `<ESC>x`过去的
- 任意平台下面的 xterm 可以配置 `~/.Xdefaults` 来设置这个行为。 
这样的话，不管是 NeoVim 还是 Emacs都识别了，Vim 的话，你可以简单这样：
```text
noremap <ESC>x :echo "ALT-X pressed"<cr>
```
注意 ESC后面是小写 x，如果你是大写 X就变成 ALT+SHIFT+X了。于是你在 Vim 中，ALT+X就能看到后面输出的那句话了。看到这里你也许要问：这和我快速按下 ESC再马上按下 x键有什么区别？答案是没有区别，在终端里面这两个操作是一模一样的键盘码传送过去。
就像你不设置 `ttimeout` 和 `ttimeoutlen`，然后快速在 VIM 里面按下 `<ESC>OP`，Vim 会以为你按下了 `<F1>`一样。因为 F1 的终端下字符串序列就是 `<ESC>OP` ，而你在 Insert 模式下面马上 `<ESC>` 退出并按下大写 O ，向上插入一行，Vim 将会等待一秒钟（默认 timeout ），确认后面没有一个 P，才会进一步确认不是F1，而是向上插行。
所以更好的做法是直接按照 `<M-x>` 进行映射，并且告诉 vim，`<M-x>`的键盘序列码是多少，然后再加上 ttimeoutlen超时：
```text
noremap <M-x> :echo "ALT-X pressed"<cr>
exec "set <M-x>=\ex"
set ttimeout ttimeoutlen=100
```
这样做的好处是告诉 Vim, ESC+x是一个完整的按键码，并且需要在 100ms以内进行判断，即，如果收到 ESC，并且100ms以后没有后续的x，则是认为是一个单独的ESC键，退出 INSERT模式，否则认为是按下了 ALT+X，这和 Vim处理方向键，处理 F1, F2等功能键的原则是相同的，具体见 `:h set-termcap`:
```text
*:set-termcap* *E522*
For {option} the form "t_xx" may be used to set a terminal option.  This will
override the value from the termcap.  You can then use it in a mapping.  If
the "xx" part contains special characters, use the <t_xx> form: >
    :set <t_#4>=^[Ot
This can also be used to translate a special code for a normal key.  For
example, if Alt-b produces <Esc>b, use this: >
    :set <M-b>=^[b
(the ^[ is a real <Esc> here, use CTRL-V <Esc> to enter it)
The advantage over a mapping is that it works in all situations.
You can define any key codes, e.g.: >
    :set t_xy=^[foo;
There is no warning for using a name that isn't recognized.  You can map these
codes as you like: >
    :map <t_xy> something
```
这是最完善的 ALT键解决方案了，网上有个流传很广的方式是 `map <ESC>x <M-x>` 然后你后面再映射 `<M-x>` 时就能被触发到，这是错误的方法，不能使用更短的 `ttimeoutlen`来识别键盘码，而会使用普通组合键的 `timeoutlen`来判断，后者一般设置为默认 1000毫秒，所以这样把 26个字母映射后，你 ESC退出 INSERT模式后，一秒内按了任何一个字母就会被当成 ALT+X来处理了，经常误操作。
如此，我们可以在 .vimrc中 for循环将 `<M-0>` 到 `<M-9>`，`<M-a>` 到 `<M-z>`等全部 set一遍，vim中即可正常使用。
早年的终端，处理ALT组合键时，是将单个字符的最高位设置成 1，这也是 vim的默认处理方式，如今 rxvt终端也支持这种模式（见上图 SecureCRT设置面板）。这种键盘码不是 ESC+x的模式，可以直接识别，不需要计算超时，缺点是支持终端较少，对终端编码格式有依赖。
如今基本上 `<ESC>+`的模式基本成为大部分终端的默认方式，主流操作了，详细可以看：`:h map-alt-keys` 以及  `:h set-termcap` 两个文档有具体说明，关于超时部分可以见（`:h esckeys` ）
当然，如果你真能在100ms内连续按下 ESC和 X的话，那是另外一回事情了，你可以调短 `ttimeoutlen`到50ms解决，但是不建议该值低于 25ms，否则在低速网络情况下，你按功能键会被vim错误识别成几个单独的按键序列。这不是 vim 的锅，而是终端标准的锅，ncurses 和 tmux 也都靠超时来检测功能键。
好了，帖一段代码吧：
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
command! -nargs=0 -bang VimMetaInit call Terminal_MetaMode(<bang>0)
```
使用：`:VimMetaInit` 将会把 vim里面的键位码定义为 ESC+ 序列，而使用 `:VimMetaInit!` 则可以定义为更不容易混淆的：`<ESC>]{0}x~` 格式（但是需要设置终端软件里每个ALT-x 组合键发送什么），将该代码放入你的 vimrc 文件中即可避免每次都调用。
**Vim 默认的 ALT编码**
比较一下 Vim默认的 ALT键识别方式（单字节高位设置1），比如 ALT+a，a的ascii码是 97，加上0x80以后值为 225，即发送一个 `\u00e1` 的 unicode字符过去，默认的vim就能识别成 ALT+a了，和 GVim的默认方式一样。看到这里你可能会问，为什么不把终端设置成 Vim的默认ALT编码，而是要弄一半天 `<ESC>x` 不当终端里面要设置，Vim里面也要重新设置一遍？默认方式 rxvt和 xterm等终端还不需要额外配置呢？这个方案好在哪里呢？
默认 ascii + 0x80的方式貌似省事，其实并不是，比如你想发送 unicode的 `\u00e1` 告诉 vim你按下了 ALT+a，那么你需要按照终端的字符编码格式来发送这个 unicode字符：比如你终端编码为 latin1的话，你只需要发送 0xe1 一个字节过去；而如果终端字符编码为 UTF-8的时候，你却需要发送 0xc3, 0xa1两个字节；GBK编码的话又要发送 0xa8, 0xa2两个完全不同的字节。
这就是一个问题，而我们使用 `<ESC>`的话，不管什么编码 `<ESC>` 都是 0x1b (27) 一个字节。再有一个问题是 iTerm2 之类的终端可以设置按下某键发送以 `<ESC>` 开头的字符串，却不能设置让你发送任意二进制，所以我们这个方式基本上是兼容所有终端的方式。
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
这份keymap配置除了保留了 SecureCRT 常用的 ALT*1 – ALT*9 ，ALT*B, ALT*R 和 ALT*I 外，其他的 alt 组合都设置成了 +ESC x 的 meta 键序列。并且将 ALT*SHIFT*1 到 ALT*SHIFT_9 映射到了终端里的 +ESC 1 到 +ESC 9 ，也就是说你的 ALT+数字 被保留给软件切换TAB用了，而 ALT+SHIFT+数字 被映射成了终端链接中的 ALT+数字，这样在终端里碰到需要 ALT+数字 的地方，可以用 ALT+SHIFT+数字 来代替。
设置好以后，你可以在终端下使用命令：
```text
showkey -a
```
来查看自己的设置正确不，是不是按下 ALT_a 后正确发送了 0x1b, 0x61 两个字节过去了？
