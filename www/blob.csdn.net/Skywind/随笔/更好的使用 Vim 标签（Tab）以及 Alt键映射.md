# Skywind Inside » 更好的使用 Vim 标签（Tab）以及 Alt键映射
## 更好的使用 Vim 标签（Tab）以及 Alt键映射
May 3rd, 2016[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
更好的使用 Vim7.0以后推出的标签（TAB）功能，同现代编辑器一样用标签（TAB）来管理多文件，代替传统 Buffer List：
![](http://www.skywind.me/blog/wp-content/uploads/2016/05/vim3.png)
让Minibufexplor/tabbar这些上个世纪的插件都退场吧，直接使用标准的标签功能会更加舒服。
**快捷键切换 TAB**
第一件事情就是要搞定标签快速切换问题，不管是:tabn X还是 Xgt都十分低效，我们需要更快速的在各个文件之间切换。最简单的是设置 0-9 来快速切换tab（默认leader是反斜杠，即先按下\键，再按数字键），不管终端还是GVIM都兼容：
```text
noremap <silent><tab>m :tabnew<cr>
noremap <silent><tab>e :tabclose<cr>
noremap <silent><tab>n :tabn<cr>
noremap <silent><tab>p :tabp<cr>
noremap <silent><leader>t :tabnew<cr>
noremap <silent><leader>g :tabclose<cr>
noremap <silent><leader>1 :tabn 1<cr>
noremap <silent><leader>2 :tabn 2<cr>
noremap <silent><leader>3 :tabn 3<cr>
noremap <silent><leader>4 :tabn 4<cr>
noremap <silent><leader>5 :tabn 5<cr>
noremap <silent><leader>6 :tabn 6<cr>
noremap <silent><leader>7 :tabn 7<cr>
noremap <silent><leader>8 :tabn 8<cr>
noremap <silent><leader>9 :tabn 9<cr>
noremap <silent><leader>0 :tabn 10<cr>
noremap <silent><s-tab> :tabnext<CR>
inoremap <silent><s-tab> <ESC>:tabnext<CR>
```
其次，GVIM/MacVim 下设置 ALT-0-9 来切换TAB：
```text
" keymap to switch table in gui
if has('gui_running') 
    set winaltkeys=no
    set macmeta
    noremap <silent><c-tab> :tabprev<CR>
    inoremap <silent><c-tab> <ESC>:tabprev<CR>
    noremap <silent><m-1> :tabn 1<cr>
    noremap <silent><m-2> :tabn 2<cr>
    noremap <silent><m-3> :tabn 3<cr>
    noremap <silent><m-4> :tabn 4<cr>
    noremap <silent><m-5> :tabn 5<cr>
    noremap <silent><m-6> :tabn 6<cr>
    noremap <silent><m-7> :tabn 7<cr>
    noremap <silent><m-8> :tabn 8<cr>
    noremap <silent><m-9> :tabn 9<cr>
    noremap <silent><m-0> :tabn 10<cr>
    inoremap <silent><m-1> <ESC>:tabn 1<cr>
    inoremap <silent><m-2> <ESC>:tabn 2<cr>
    inoremap <silent><m-3> <ESC>:tabn 3<cr>
    inoremap <silent><m-4> <ESC>:tabn 4<cr>
    inoremap <silent><m-5> <ESC>:tabn 5<cr>
    inoremap <silent><m-6> <ESC>:tabn 6<cr>
    inoremap <silent><m-7> <ESC>:tabn 7<cr>
    inoremap <silent><m-8> <ESC>:tabn 8<cr>
    inoremap <silent><m-9> <ESC>:tabn 9<cr>
    inoremap <silent><m-0> <ESC>:tabn 10<cr>
endif
```
或者在 MacVim 下还可以用 CMD_0-9 快速切换，按着更舒服：
```text
if has("gui_macvim")
    set macmeta
    noremap <silent><c-tab> :tabprev<CR>
    inoremap <silent><c-tab> <ESC>:tabprev<CR>
    noremap <silent><d-1> :tabn 1<cr>
    noremap <silent><d-2> :tabn 2<cr>
    noremap <silent><d-3> :tabn 3<cr>
    noremap <silent><d-4> :tabn 4<cr>
    noremap <silent><d-5> :tabn 5<cr>
    noremap <silent><d-6> :tabn 6<cr>
    noremap <silent><d-7> :tabn 7<cr>
    noremap <silent><d-8> :tabn 8<cr>
    noremap <silent><d-9> :tabn 9<cr>
    noremap <silent><d-0> :tabn 10<cr>
    inoremap <silent><d-1> <ESC>:tabn 1<cr>
    inoremap <silent><d-2> <ESC>:tabn 2<cr>
    inoremap <silent><d-3> <ESC>:tabn 3<cr>
    inoremap <silent><d-4> <ESC>:tabn 4<cr>
    inoremap <silent><d-5> <ESC>:tabn 5<cr>
    inoremap <silent><d-6> <ESC>:tabn 6<cr>
    inoremap <silent><d-7> <ESC>:tabn 7<cr>
    inoremap <silent><d-8> <ESC>:tabn 8<cr>
    inoremap <silent><d-9> <ESC>:tabn 9<cr>
    inoremap <silent><d-0> <ESC>:tabn 10<cr>
    noremap <silent><d-o> :browse tabnew<cr>
    inoremap <silent><d-o> <ESC>:browse tabnew<cr>
endif
```
这下很舒服了，和大部分主流编辑器一样切换tab十分轻松。那终端下 alt没法很好的映射怎么办呢？
**终端下映射 ALT-0-9 快速切换标签**
不推荐把中断里的ALT键设置成ESC+，大部分终端（XShell, SecureCRT, iTerm, gnome-terminal）都提供ALT键改为ESC+的模式，即按下ALT-A，终端会连续发送ESC(27)和 A(65)两个字节，这样在emacs下挺好用，但是在vim下就糟糕了，比如你映射了<ESC>A来代替 ALT-A，那么当你用ESC退出insert模式时，一秒钟内马上按下A来在行末追加内容，Vim会去触发你之前设置的ALT-A的功能，这就对 Vim 原有的功能造成了扰乱，那终端下如何正确设置 ALT功能键呢？
我们使用终端自定义按键序列，大部分终端都支持自定义按键发送特定字符串（XShell,SecureCRT,iTerm,gnome-terminal），比如我们可以把ALT-1设置成发送 `<ESC>]{0}1~` 这个字符串，而 ALT-2设置成发送 `<ESC>]{0}2~` 这个字符串，于是我们可以在Vim里面建立快捷映射：
```text
" set terminal and map alt+n or alt+shift+n to "<ESC>]{0}n~"
if !has('gui_running')
    noremap <silent><ESC>]{0}1~ :tabn 1<cr>
    noremap <silent><ESC>]{0}2~ :tabn 2<cr>
    noremap <silent><ESC>]{0}3~ :tabn 3<cr>
    noremap <silent><ESC>]{0}4~ :tabn 4<cr>
    noremap <silent><ESC>]{0}5~ :tabn 5<cr>
    noremap <silent><ESC>]{0}6~ :tabn 6<cr>
    noremap <silent><ESC>]{0}7~ :tabn 7<cr>
    noremap <silent><ESC>]{0}8~ :tabn 8<cr>
    noremap <silent><ESC>]{0}9~ :tabn 9<cr>
    noremap <silent><ESC>]{0}0~ :tabn 10<cr>
    inoremap <silent><ESC>]{0}1~ <ESC>:tabn 1<cr>
    inoremap <silent><ESC>]{0}2~ <ESC>:tabn 2<cr>
    inoremap <silent><ESC>]{0}3~ <ESC>:tabn 3<cr>
    inoremap <silent><ESC>]{0}4~ <ESC>:tabn 4<cr>
    inoremap <silent><ESC>]{0}5~ <ESC>:tabn 5<cr>
    inoremap <silent><ESC>]{0}6~ <ESC>:tabn 6<cr>
    inoremap <silent><ESC>]{0}7~ <ESC>:tabn 7<cr>
    inoremap <silent><ESC>]{0}8~ <ESC>:tabn 8<cr>
    inoremap <silent><ESC>]{0}9~ <ESC>:tabn 9<cr>
    inoremap <silent><ESC>]{0}0~ <ESC>:tabn 10<cr>
endif
```
如此由于终端上映射了 ALT-0-9 为 `<ESC>]{0}N~` 所以在 XShell/SecureCRT/iTerm下按 ALT-0-9 可以发送对应的字符串给 Vim并被上面的配置代码识别。
有人问XShell/SecureCRT下面ALT-0-9是用来快速切换终端标签的，怎么办呢？好办，在终端下把 ALT-SHIFT-0-9映射成刚才的字符串即可啊，这样保证不影响终端快捷键的同时，方便的使用<M-S-0/9>来切换标签和很方便，或者用先前的0-9来代替。
**避免按键冲突**
有人说这会不会和<ESC>退出insert后马上按]又发生冲突？其实不用当心，<ESC>后很少有机会按[，即使你按了]也很少有机会继续迅速按下{,0,}三个按键，连终端下F5即被映射为字符串：`<ESC>]15~` 所以你放心大胆的用`<ESC>]{`开头吧，即使你正常使用下按了马上按]，你会发现VIM等待了1秒钟，发现期间没有后续的{0}1~，就马上判断不是该映射，所以正常执行]功能。
这和 Vim中按下F1 的情况一样，xterm终端下被映射为字符串`<ESC>OP`，你在XShell/iTerm/SecureCRT下面敲下 F1实际会发送：0x1b (ESC), 0x4f(O), 0x50(P) 三字字节到服务端，所以你在Vim下一秒钟内快速敲入<ESC>OP三个键和F1键的功能相同–打开帮助。那么你可能会问，如果我想按ESC离开INSERT模式后马上按O在光标上面插入一行，在VIM里到底是被判断成我想要的模式？还是F1的前缀呢？其实是靠超时（help timeout），你马上离开插入模式后按了O，VIM会等待一秒钟知道确认没有后续追加按键了，才判断为`离开插入模式`和`向上插入一行`两个独立的动作，否则在timeout（默认1秒）内你又按了下P，那VIM就真的以为你按下 F1了。
如果你想避免退出插入模式马上按O,P被识别成F1，那么你可以先按O，然后等一秒过了真的插入一行了，再按下P，问题不大。或者用ko来代替，又或者直接使用GVIM/MacVim就没这个歧义了。
所以在终端下通过配置键盘映射，当按下ALT-X时，发送`<ESC>]{0}X~`字符串，可以比直接把ALT键设置成+能更好的减少按键歧义，正常使用很少后紧跟]的。
**设置标签文本**
默认的标签文本包含文件路径，十分凌乱，需要进一步修改，让它只显示文件名：
```text
" make tabline in terminal mode
function! Vim_NeatTabLine()
    let s = ''
    for i in range(tabpagenr('$'))
        " select the highlighting
        if i + 1 == tabpagenr()
            let s .= '%#TabLineSel#'
        else
            let s .= '%#TabLine#'
        endif
        " set the tab page number (for mouse clicks)
        let s .= '%' . (i + 1) . 'T'
        " the label is made by MyTabLabel()
        let s .= ' %{Vim_NeatTabLabel(' . (i + 1) . ')} '
    endfor
    " after the last tab fill with TabLineFill and reset tab page nr
    let s .= '%#TabLineFill#%T'
    " right-align the label to close the current tab page
    if tabpagenr('$') > 1
        let s .= '%=%#TabLine#%999XX'
    endif
    return s
endfunc
" get a single tab name 
function! Vim_NeatBuffer(bufnr, fullname)
    let l:name = bufname(a:bufnr)
    if getbufvar(a:bufnr, '&modifiable')
        if l:name == ''
            return '[No Name]'
        else
            if a:fullname 
                return fnamemodify(l:name, ':p')
            else
                return fnamemodify(l:name, ':t')
            endif
        endif
    else
        let l:buftype = getbufvar(a:bufnr, '&buftype')
        if l:buftype == 'quickfix'
            return '[Quickfix]'
        elseif l:name != ''
            if a:fullname 
                return '-'.fnamemodify(l:name, ':p')
            else
                return '-'.fnamemodify(l:name, ':t')
            endif
        else
        endif
        return '[No Name]'
    endif
endfunc
" get a single tab label
function! Vim_NeatTabLabel(n)
    let l:buflist = tabpagebuflist(a:n)
    let l:winnr = tabpagewinnr(a:n)
    let l:bufnr = l:buflist[l:winnr - 1]
    return Vim_NeatBuffer(l:bufnr, 0)
endfunc
" get a single tab label in gui
function! Vim_NeatGuiTabLabel()
    let l:num = v:lnum
    let l:buflist = tabpagebuflist(l:num)
    let l:winnr = tabpagewinnr(l:num)
    let l:bufnr = l:buflist[l:winnr - 1]
    return Vim_NeatBuffer(l:bufnr, 0)
endfunc
" setup new tabline, just like %M%t in macvim
set tabline=%!Vim_NeatTabLine()
set guitablabel=%{Vim_NeatGuiTabLabel()}
```
在GUI模式下，还可以配置标签的TIPS，鼠标移动到标签上，会自动显示该TIPS：
```text
" get a label tips
function! Vim_NeatGuiTabTip()
    let tip = ''
    let bufnrlist = tabpagebuflist(v:lnum)
    for bufnr in bufnrlist
        " separate buffer entries
        if tip != ''
            let tip .= " \n"
        endif
        " Add name of buffer
        let name = Vim_NeatBuffer(bufnr, 1)
        let tip .= name
        " add modified/modifiable flags
        if getbufvar(bufnr, "&modified")
            let tip .= ' [+]'
        endif
        if getbufvar(bufnr, "&modifiable")==0
            let tip .= ' [-]'
        endif
    endfor
    return tip
endfunc
set guitabtooltip=%{Vim_NeatGuiTabTip()}
```
**快速在标签中打开文件**
除了:tabedit 外，Vim自带netrw插件可以浏览目录，是代替nerdtree的好东西，使用:Explore打开netrw的文件浏览器后，选中文件按t键即可以再新标签打开对应文件了。
**快捷键在标签打开新文件**
我们配置按下 ALT-O 的时候在当前位置打开文件浏览器：
```text
" Open Explore in new tab with current directory
function! Open_Explore(where)
    let l:path = expand("%:p:h")
    if l:path == ''
        let l:path = getcwd()
    endif
    if a:where == 0
        exec 'Explore '.fnameescape(l:path)
    elseif a:where == 1
        exec 'vnew'
        exec 'Explore '.fnameescape(l:path)
    else
        exec 'tabnew'
        exec 'Explore '.fnameescape(l:path)
    endif
endfunc
```
这样可以把 call Open_Explore(2) 映射成ALT-O，这样如果在编辑一个文件时候按下 ALT-O，那么会在该文件的路径下展开一个新的tab并显示netrw的文件列表。
如果使用 GVIM/MacVim可以更优雅点，直接在当前文件的路径下，使用系统的文件查找窗口，选中文件后在新的Tab里打开：
```text
function! s:Filter_Push(desc, wildcard)
    let g:browsefilter .= a:desc . " (" . a:wildcard . ")\t" . a:wildcard . "\n"
endfunc
let g:browsefilter = ''
call s:Filter_Push("All Files", "*")
call s:Filter_Push("C/C++/Object-C", "*.c;*.cpp;*.cc;*.h;*.hh;*.hpp;*.m;*.mm")
call s:Filter_Push("Python", "*.py;*.pyw")
call s:Filter_Push("Text", "*.txt")
call s:Filter_Push("Vim Script", "*.vim")
function! Open_Browse(where)
    let l:path = expand("%:p:h")
    if l:path == '' | let l:path = getcwd() | endif
    if exists('g:browsefilter') && exists('b:browsefilter')
        if g:browsefilter != ''
            let b:browsefilter = g:browsefilter
        endif
    endif
    if a:where == 0
        exec 'browse e '.fnameescape(l:path)
    elseif a:where == 1
        exec 'browse vnew '.fnameescape(l:path)
    else
        exec 'browse tabnew '.fnameescape(l:path)
    endif
endfunc
```
然后把 `call Open_Browse(2)` 映射成 ALT-O。
**寻找Tag时在新标签打开**
以前寻找tag时使用<C-]>，现在改为<c-w><c-]>即可在新标签打开tag定义。
**Quickfix中在新标签打开文件**
编译或者 grep 的时候，信息输出到 Quickfix窗口中，按回车就容易把buf给切换走，使用标签后设置下面一项，即可更改quickfix行为为：如果有已打开文件，先复用，没有的话使用标签：
```shell
set switchbuf=useopen,usetab,newtab
```
如此Tab已经很好用了，再配置一些快捷键按ALT-方向键来tabmove，ALT-w来关闭标签，基本上和其他现代编辑器行为很类似了。
