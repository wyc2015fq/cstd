
# 给自己的VIM配置 - 高科的专栏 - CSDN博客

2012年09月19日 09:31:14[高科](https://me.csdn.net/pbymw8iwm)阅读数：6388标签：[vim																](https://so.csdn.net/so/search/s.do?q=vim&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[templates																](https://so.csdn.net/so/search/s.do?q=templates&t=blog)[filter																](https://so.csdn.net/so/search/s.do?q=filter&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=templates&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=windows&t=blog)个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)
[
																								](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=vim&t=blog)
编辑 .vimrc 文件如下：
filetype plugin on

"autocmd Filetype cpp,c,java,cs set omnifunc=cppcomplete\#Complete
set nu
set nocp
set nobackup

let g:C_AuthorName      = 'gaoke'
let g:C_AuthorRef       = 'gaoke'
let g:C_Email           = 'gaoke@taomee.com'
let g:C_Company         = 'TAOMEE'
"let g:vimim_disable_smart_space=1
"let g:vimim_disable_chinese_punctuation=0

"autocmd BufEnter *.hpp
if (    expand('%f')=~ ".hpp$"  )
let g:C_Comments= 'false'
endif


set laststatus=2
autocmd BufEnter *.c  set filetype=cpp

"------------------------------------------------------------------------------
"获取当前路径的上一级的路径
function! GET_UP_PATH(dir)
let pos=len(a:dir)-1
while pos>0
if (a:dir[pos]=="/" )
returnstrpart(a:dir,0,pos)
endif
let pos=pos-1
endwhile
return  ""
endfunction

"设置相关tags
function! s:SET_TAGS()
"let dir = expand("%:p:h") "获得源文件路径
let dir =getcwd()  "获得源文件路径
set tags=
"在路径上递归向上查找tags文件
while dir!=""
if findfile("tags",dir ) !=""
"找到了就加入到tags
exec "set tags+=" . dir . "/tags"
endif
"得到上级路径
let dir=GET_UP_PATH(dir)
endwhile
if ( &filetype =="cpp" )
set tags+=~/.vim/tags
set tags+=~/.vim/tags_cpp
set tags+=~/.vim/tags_qt

endif
endfunction


"设置相关 include , for cmd : gf
function! s:SET_PATH( find_dir )
let dir = expand("%:p:h") "获得源文件路径
let dir_relative=''
let g:alternateSearchPath = ''
"let g:alternateSearchPath = 'sfr:../source,sfr:../src,sfr:../include,sfr:../inc,sfr:.'
"在路径上递归向上查找tags文件
while dir!=""
if finddir(a:find_dir ,dir ) !=""
"找到了就加入到tags
exec "set path+=" . dir . "/". a:find_dir
let g:alternateSearchPath = g:alternateSearchPath.'sfr:'.dir_relative.a:find_dir.","
endif
"得到上级路径
let dir_relative=dir_relative . "../"
let dir=GET_UP_PATH(dir)
endwhile
endfunction

autocmd BufEnter *.cpp,*.c,*.h call s:SET_TAGS()
autocmd BufEnter *.php call s:SET_TAGS()

autocmd BufEnter  *.cpp,*.c,*.h call s:SET_PATH("include")
autocmd BufEnter  *.php call s:SET_PATH("pub")

autocmd BufEnter *  set tabstop=4
autocmd BufEnter /usr/include/c++/*  set tabstop=8

autocmd BufEnter *    if ( &filetype == "php" )| map ,i <Esc>:e ~/DB/su/pub/| else | map ,i <Esc>:e ~/DB/include/| endif

"用于支持代码补全时，提示存在。
"set completeopt=menuone
inoremap <expr> <cr> pumvisible() ? "\<c-y>" : "\<c-g>u\<cr>"
"inoremap <expr> <c-n> pumvisible() ? "\<c-n>" : "\<c-n>\<c-r>=pumvisible() ? \"\\<down>\" : \"\\<cr>\""
inoremap <expr> <m-;> pumvisible() ? "\<c-n>" : "\<c-x>\<c-o>\<c-n>\<c-p>\<c-r>=pumvisible() ? \"\\<down>\" : \"\\<cr>\""

" 用于支持 . -> 代码补全
imap   <expr> <Backspace>  Ex_bspace()
imap   <expr> <Space>  Ex_space("\<Space>")

function! Ex_space ( char )
if (&filetype == "cpp" || &filetype == "c" )
let pre_str= strpart(getline('.'),0,col('.')-1)
if pumvisible() != 0
"in completing , complete it
return "\<CR>"
elseif pre_str  =~ "[.][\s\t]*$" || pre_str  =~ "->[\s\t]*$"
return "\<C-X>\<C-O>"
endif
endif

if (&filetype == "python" ||&filetype == "html"  ||&filetype == "php"     )
let pre_str= strpart(getline('.'),0,col('.')-1)
if pumvisible() != 0
"in completing , complete it
return "\<CR>"
elseif pre_str  =~ "[.][\s\t]*$" || pre_str  =~ "->[\s\t]*$"
return "\<C-X>\<C-O>\<C-P>\<C-R>=pumvisible() ? \"\\<down>\" : \"\"\<cr>"
endif
endif



"default
return a:char
endf

function! Ex_bspace()
if (&filetype == "cpp" || &filetype == "c" )
let pre_str= strpart(getline('.'),0,col('.')-2)
if pre_str  =~ "[.][ \t]*$" || pre_str  =~ "->[ \t]*$"
return "\<Backspace>\<C-X>\<C-O>"
endif
endif

if (&filetype == "python"|| &filetype == "html"  || &filetype == "python"  )
let pre_str= strpart(getline('.'),0,col('.')-2)
if pre_str  =~ "[.][ \t]*$"
return "\<Backspace>\<C-X>\<C-O>\<C-P>\<C-R>=pumvisible() ? \"\\<down>\" : \"\"\<cr>"
endif
endif

"default
return "\<Backspace>"
endf


function! FIND_PROTO()
"let szLine = getline('.')
"let startPos = getpos('.')[2]-1
"let startPos = (startPos < 0)? 0 : startPos
"if szLine[startPos] =~ '\w'
"let startPos = searchpos('\<\w\+', 'cbn', line('.'))[1] - 1
"endif

"let startPos = (startPos < 0)? 0 : startPos
"let szResult = matchstr(szLine, '\w\+', startPos)
""echo szResult
endfunction


".pc 文件设置为.ec 的高亮显示
"autocmd BufEnter *.pc  set filetype=esqlc
".c  .h 文件设为 .cpp
autocmd BufEnter *.c  set filetype=cpp

"------------------------------------------------------------------------------
set guifont=Bitstream\ Vera\ Sans\ Mono\ 11


"功能说明:加入或删除注释//
"映射和绑定
"nmap <C-C> <Esc>:Setcomment<CR>
"imap <C-C> <Esc>:Setcomment<CR>
"vmap <C-C> <Esc>:SetcommentV<CR>
"command! -nargs=0 Setcomment call s:SET_COMMENT()
"command! -nargs=0 SetcommentV call s:SET_COMMENTV()

"非视图模式下所调用的函数
function! s:SET_COMMENT()
let lindex=line(".")
let str=getline(lindex)
"查看当前是否为注释行
let CommentMsg=s:IsComment(str)
call s:SET_COMMENTV_LINE(lindex,CommentMsg[1],CommentMsg[0])
endfunction

function! SET_UAW()
let save_cursor = getpos(".")

let line = getline('.')
let col_num = col('.')
if match("ABCDEFGHIJKLMNOPQRSTUVWXYZ",line[col_num-1])!= -1
exec "normal! guaw"
else
exec "normal! gUaw"
endif

call setpos('.', save_cursor)
endfunction


function! MAKE_CUR_FILE()
let cur_file= expand("%f")
"split

exec "normal! gUaw"
endfunction



"视图模式下所调用的函数
function! s:SET_COMMENTV()
let lbeginindex=line("'<") "得到视图中的第一行的行数
let lendindex=line("'>") "得到视图中的最后一行的行数
let str=getline(lbeginindex)
"查看当前是否为注释行
let CommentMsg=s:IsComment(str)
"为各行设置
let i=lbeginindex
while i<=lendindex
call s:SET_COMMENTV_LINE(i,CommentMsg[1],CommentMsg[0])
let i=i+1
endwhile
endfunction

"设置注释
"index:在第几行
"pos:在第几列
"comment_flag: 0:添加注释符 1:删除注释符
function! s:SET_COMMENTV_LINE( index,pos, comment_flag )
let poscur = [0, 0,0, 0]
let poscur[1]=a:index
let poscur[2]=a:pos+1
call setpos(".",poscur) "设置光标的位置

if a:comment_flag==0
"插入//
exec "normal! i//"
else
"删除//
exec "normal! xx"
endif
endfunction

"查看当前是否为注释行并返回相关信息
"str:一行代码
function! s:IsComment(str)
let ret= [0, 0] "第一项为是否为注释行（0,1）,第二项为要处理的列，
let i=0
let strlen=len(a:str)
while i<strlen
"空格和tab允许为"//"的前缀
if !(a:str[i]==' ' ||a:str[i] == '' )
let ret[1]=i
if a:str[i]=='/' && a:str[i+1]=='/'
let ret[0]=1
else
let ret[0]=0
endif
return ret
endif
let i=i+1
endwhile
return [0,0]  "空串处理
endfunction


"ab E EXEC SQL
"ab S SQLCODE
ab \#x 薛朝文
ab \#l /*----------------------------------------------------------------------------*/
ab \#b /*******************************************************************************
ab \#e *******************************************************************************/
ab \#i \#include
ab \#d \#define
set fileencodings=ucs-bom,utf-8,gb2312,big5,euc-jp,euc-kr,latin1
"set fileencoding=gb2312
set tabstop=4
set incsearch
set nohlsearch
set cindent shiftwidth=4
command! Wq wq
command! W w
map ,n :n<CR>
map ,N :N<CR>
map ,a :A<CR>
map ,g :grep\s
"map <F6> <Esc>:/<C-R><C-W><Backspace><Backspace><Backspace><Backspace><CR>

map <F7> <Esc>:call Proto_find() <CR>
map <F6> <Esc>:call Proto_find() <CR>

"for grep cn
function! Do_cn()
try
exec "cn"
catch /E553/
exec "cc 1"
endtry
endfunction
function! P_grep_curword()
"得到光标下的单词
let curword=expand("<cword>")
exec "grep " . curword . " *.cpp *.c *.h *.hpp "
endfunction
map ,f <Esc>:call P_grep_curword() <CR>
map ,c <Esc>:call Do_cn() <CR>
map ,C <Esc>:cN<CR>


map ,r <Esc>:!~/.vim/mtags.sh<CR>
map ,m <Esc>:make<CR>

"转换单词大小写
map ,u <Esc>:call SET_UAW()<CR>

map ,p <Esc>:set paste<CR>i
autocmd InsertLeave * if &paste == 1|set nopaste |endif

"切换窗口
map ,w <Esc>:tabn<CR><C-W><C-W><CR>

if has("unix")
map ,e :e <C-R>=expand("%:p:h") . "/" <CR>
else
map ,e :e <C-R>=expand("%:p:h") . "\" <CR>
endif

map <F3> <Esc>yyp^6l<C-A>4l<C-A><Esc>
map <C-F12> :!ctags -R  --languages=c++ --c++-kinds=+p --fields=+iaS --extra=+q .<CR>


" 在视图模式下的整块移动
function! SET_BLOCK_MOVE_V( move_type )
if a:move_type==0
exec "'<,'>s/^//"
else
exec "'<,'>s/^//"
endif

let linecount = line("'>") - line("'<")
let save_cursor_begin = getpos("'<")
call setpos('.', save_cursor_begin)
exec  "normal! v" . linecount . "j"
endfunction

" 在正常模式下的整块移动
"大括号内向左移
:nmap <C-H> <Esc><i{
"大括号内向右移
:nmap <C-L> <Esc>>i{

"选择区移动
:vmap <C-L> <Esc>:call SET_BLOCK_MOVE_V(0) <CR>
:vmap <C-H> <Esc>:call SET_BLOCK_MOVE_V(1) <CR>

set smarttab
"set paste
syn on
"定位到原来的位置
autocmd BufReadPost *
\ if line("'\"") > 0 && line ("'\"") <= line("$") |
\   exe "normal! g'\"" |
\ endif

"支持STL模板
let OmniCpp_DefaultNamespaces   = ["std", "_GLIBCXX_STD"]
let OmniCpp_SelectFirstItem = 2
"----------------------------------------------------------
inoremap ( ()<ESC>i
inoremap [ []<ESC>i
"inoremap " ""<ESC>i
"inoremap ' ''<ESC>i
autocmd Syntax html,vim inoremap < <lt>><ESC>i| inoremap > <c-r>=ClosePair('>')<CR>
imap ) <c-r>=ClosePair(')')<CR>
imap ] <c-r>=ClosePair(']')<CR>
imap } <c-r>=CloseBracket()<CR>
"imap <CR> <c-r>=Fix_cr()<CR>
imap " <c-r>=QuoteDelim('"')<CR>
imap ' <c-r>=QuoteDelim("'")<CR>
inoremap { <c-r>=SET_BIG_PAIR()<CR>

function! SET_BIG_PAIR()
if (&filetype=="php" )
if match( getline('.'), '"' ) >= 0
return "{}\<ESC>i"
endif
endif
return "{\<CR>}\<ESC>O"
endf



function! ClosePair(char)
if getline('.')[col('.') - 1] == a:char
return "\<Right>"
else
return a:char
endif
endf

function! Fix_cr()
if pumvisible() == 0
let line = getline('.')
let col = col('.')
if line[col-1] == '"' ||  line[col-1] == "'"  || line[col-1] == "]"  ||line[col-1] == ")"
"Escaping out of the string
return "\<ESC>la"
endif
endif
return "\<CR>"

endf

function! CloseBracket()
if match(getline(line('.') + 1), '\s*}') < 0
return "\<CR>}"
else
return "\<ESC>j0f}a"
endif
endf


function! QuoteDelim(char)
let line = getline('.')
let col = col('.')

if (&filetype == "vim")
return a:char
endif

if line[col - 2] == "\\"
"Inserting a quoted quotation mark into the string
return a:char
elseif line[col - 1] == a:char
"Escaping out of the string
return "\<Right>"
else
"Starting a string
return a:char.a:char."\<ESC>i"
endif
endf
"autocmd CursorMovedI * if pumvisible() == 0|pclose|endif
"autocmd InsertLeave * if pumvisible() == 0|pclose|endif

set mouse=
"set cinoptions={0,1s,t0,n-2,p2s,(03s,=.5s,>1s,=1s,:1s
"------------------------------------------------
"source ~/.vim/borland.vim
"
"用于支持DB 协议查找 ：cmd<->function<->in<->out
function! Proto_find()

"得到光标下的单词
let curword=expand("<cword>")
python get_proto_key(vim.eval("curword"),"find_word" )
"设置搜索寄存器
let  @/ = find_word
call histadd("/", find_word )
"查找下一个..
exec "normal! n"
endfunction



"得到光标下的单词
function! GetCurWord()
return expand("<cword>")
"let szLine = getline('.')
"let startPos = getpos('.')[2]-1
"let startPos = (startPos < 0)? 0 : startPos
"if szLine[startPos] =~ '\w'
"let startPos = searchpos('\<\w\+', 'cbn', line('.'))[1] - 1
"endif

"let startPos = (startPos < 0)? 0 : startPos
"let szResult = matchstr(szLine, '\w\+', startPos)
"return szResult
endfunc

function! s:UserDefPython()
python << PYTHONEOF
import re
import sys
import vim
def get_proto_key(word,stroe_name):
if (word.isupper()):
word=word.lower();

if  re.search ("_in$", word ): value= word[:-3]
elif  re.search ("_in_header$", word ): value=word[:-10]
elif  re.search ("_out_header$", word ): value=word[:-11]
elif  re.search ("_out$", word ): value=word[:-4]
elif  re.search ("_cmd$", word ): value=word[:-4].lower()
elif  re.search ("_out_item$", word ): value=word[:-9]
elif  re.search ("_in_item$", word ): value=word[:-8]
else: value=word
if value!= word:
key="\<%s_CMD\>\|\<%s\>\|\<%s_in\>\|\<%s_in_header\>\|\<%s_out_header\>\|\<%s_out\>\|\<%s_in_item\>\|\<%s_out_item\>"%(value.upper(),
value,value,value,value,value,value,value)
else: key=word
vim.command("silent let %s='%s'" % (stroe_name,key))
PYTHONEOF
endfunction

if has('python')
call s:UserDefPython()
endif


autocmd BufRead,BufNewFile *.py set ai
autocmd BufRead *.py set smartindent cinwords=if,elif,else,for,while,try,except,finally,def,class


"ok this is added by gaoke"
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

set autoindent      " always set autoindenting on
if has("vms")
set nobackup      " do not keep a backup file, use versions instead
else
set backup        " keep a backup file
endif
set history=50      " keep 50 lines of command line history
set ruler       " show the cursor position all the time
set showcmd     " display incomplete commands
set incsearch       " do incremental searching

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" This is an alternative that also works in block mode, but the deleted
" text is lost and it only works for putting the current register.
"vnoremap p "_dp

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
syntax on
set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

" Enable file type detection.
" Use the default filetype settings, so that mail gets 'tw' set to 72,
" 'cindent' is on in C files, etc.
" Also load indent files, to automatically do language-dependent indenting.
filetype plugin indent on

" For all text files set 'textwidth' to 78 characters.
autocmd FileType text setlocal textwidth=78

" When editing a file, always jump to the last known cursor position.
" Don't do it when the position is invalid or when inside an event handler
" (happens when dropping a file on gvim).
autocmd BufReadPost *
\ if line("'\"") > 0 && line("'\"") <= line("$") |
\   exe "normal g`\"" |
\ endif

endif " has("autocmd")a
" 预设一个tags
set tags=tags,../tags

let $d_name_pre=strtrans(system("dirname `pwd`"))
" fix: system return value with a ^@ tail
let d_name=strpart($d_name_pre, 0, stridx($d_name_pre, "^@"))
if d_name =~ "^/home/zog/linux.*"
set tags=/home/zog/linux/tags
endif

set backupdir=~/vim_backup
set noignorecase
set so=15
set shiftwidth=4
set tabstop=4
set fencs=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
map <F7> :make<CR>
map <F12> :q <CR>
map <F8> /$^ <CR>


