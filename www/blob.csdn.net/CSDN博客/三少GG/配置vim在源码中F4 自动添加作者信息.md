# 配置vim在源码中F4/自动添加作者信息 - 三少GG - CSDN博客
2013年07月31日 16:26:35[三少GG](https://me.csdn.net/scut1135)阅读数：3493
按F4添加作者信息
每个程序员都有自己心爱的代码编辑和查看工具，相信不少Linux下的程序员都喜欢用Vim，尽管Vim没有SourceInsight这种依托于桌面环境的编辑器看起来那样强大，但是Vim的高效、简洁以及强大的可配置性都让它吸引了无数忠实的fans。
很多编辑器都支持在源代码中自动添加作者信息的功能，SourceInsight就有这项功能。Vim默认是没有这个功能，但是只要我们稍作配置，就可以实现这个功能了。
在Linux下我们需要修改Vim的配置文件vimrc，位于： /etc/vim/vimrc
在window下Vim的配置文件位于安装目录下的_vimrc
然后在配置文件中添加如下信息，就可以实现在源代码中自动添加作者信息了：
```
"将键盘上的F4功能键映射为添加作者信息的快捷键
map <F4> ms:call TitleDet()<cr>'s
function AddTitle()
        call append(0,"/*******************************************************************************")
        call append(1," * Author	 : xxx | work at xxx.")
        call append(2," * Email	 : xxx@xxx.com")
        call append(3," * Last modified : ".strftime("%Y-%m-%d %H:%M"))
        call append(4," * Filename	 : ".expand("%:t"))
        call append(5," * Description	 : ")
        call append(6," * *****************************************************************************/")
        echohl WarningMsg | echo "Successful in adding the copyright." | echohl None
endf
"更新最近修改时间和文件名
function UpdateTitle()
        normal m'
        execute '/# *Last modified:/s@:.*$@\=strftime(":\t%Y-%m-%d %H:%M")@'
        normal "
        normal mk
        execute '/# *Filename:/s@:.*$@\=":\t\t".expand("%:t")@'
        execute "noh"
        normal 'k
        echohl WarningMsg | echo "Successful in updating the copy right."| echohl None
endfunction
"判断前10行代码里面，是否有Last modified这个单词，
"如果没有的话，代表没有添加过作者信息，需要新添加；
"如果有的话，那么只需要更新即可
function TitleDet()
        let n=1
        while n < 10
                let line = getline(n)
                if line =~'^\#\s*\S*Last\smodified:\S*.*$'
                        call UpdateTitle()
                        return
                endif
                let n = n + 1
        endwhile
        call AddTitle()
endfunction
```
### vim自动添加代码作者信息以及自动更新Last Changed时间2012-07-28 02:11 |(分类:[vim](http://blog.renren.com/blog/344527998/friends?categoryId=8250323))
所谓的作者信息和Last Changed时间，一般是这样的：
![](http://fmn.rrimg.com/fmn059/20120728/0105/b_large_oR90_29a5000014691263.jpg)
我们希望实现：
（1）每次新建一个代码文件（C/C++、perl、sh脚本等）时，vim能自动插入这些作者信息到文件头部，包括该文件的创建时间，且光标自动定位到Description处且切换到了Insert模式，等待作者插入代码的描述信息；
（2）每次修改这个文件后，要求VIM能自动更新Last Changed的时间。
这两项功能都要依靠VIM的插件来实现。所有提到的插件都可以在这里找到：
[http://www.vim.org](http://rrurl.cn/fiIzfw)
（1）为实现第一个功能，下载3个插件： c-support.vim，perl-support.vim，bash-support.vim。
插件不用安装，按说明把它们放入~/.vim/下相应目录即可，没有的目录一律新建。这3个插件分别能对C/C++、perl和shell脚本自动添加作者信息。进入它们各自的templates/Template文件，修改为你的相应信息即可。**新建一个cpp或脚本文件，你会发现VIM已经能贴心地替你添加作者信息了 +_+ 也能自动记录文件创建时间。**
如果你对以上插件自动给的排版不满意，以c-support.vim为例，你也可以修改~/.vim/c-support/templates/c.comments.template，找到第一个关键字"start"的代码行位置，这里你会看到作者信息的模板，任你DIY以得到任何你想要的排版顺序。perl、sh的插件也是到它们相似的目录下去修改各自的comments文件，而C++还需要修改第二个"start"出现的代码段，那是.h头文件的作者信息模板。
（2）上面3个插件很nice，但唯一的遗憾是不能更新每次修改代码的时间，那么这个作者信息对程序员的价值就要大打折扣了。稍候片刻，再添另一个插件，效果包您满意。这就是**TimeStamp.vim**。依葫芦画瓢同样把它放入正确目录，然后把下面3行放入你的~/.vimrc文件：
let timestamp_regexp = '\v\C%(<Last %([cC]hanged?|[Mm]odified):\s+)@<=.*$'
set modelines=20   
let g:timestamp_rep = '%m/%d/%Y %X'
最后如（1）一样打开那3大插件各自的comments文件：删除Revised一行，添加一行：
Last Changed: TIMESTAMP
或者
Last Modified: TIMESTAMP
这时你再新建一个源码文件，会发现作者信息里有TIMESTAMP这一行了。当你输入:w保存这个新文件时，TIMESTAMP唰地一下就变成了你保存的时间，再多:w几下，看着那不断刷新的Last Changed时间，幸福感有木有 :-)
附注：
TimeStamp.vim默认的是搜索一个源码文件前5行，发现TIMESTAMP这个标签就把它替换成当前系统时间。这是第一次保存时更新时间的机制。此插件另一个默认机制是每当你保存时，搜索前5行，发现有关键字Last Changed（或Last Modified）且后面紧跟一个时间，就会把这个时间替换为当前时间。这是第二次及以后每次保存时更新时间的机制。
所以，你也可以DIY这一部分，如这个命令
let timestamp_regexp = '\v\C%(<Last %([cC]hanged?|[Mm]odified):\s+)@<=.*$'
就能更改第二种机制所要搜索的关键字。这个命令
set modelines=20
则可以设置在多少行的范围内搜索。最后这个命令
let g:timestamp_rep = '%m/%d/%Y %X'
是设置输出时间的格式，分别是月、日、年和12小时制的时间。（还有%y两位数年份、%a星期、%b英文月份、%z时区、%T二十四小时制时间等）。
附注2：
一个在vim打开的文件中一键插入当前时间的方法
把以下2行放入你的~/.vimrc文件即可：
:nnoremap <F5> "=strftime("%m/%d/%Y %X")<CR>gP
:inoremap <F5> <C-R>=strftime("%m/%d/%Y %X")<CR>以后只要在vim的normal模式或insert模式下，按下F5，就会插入当前时间。同理，你也可以修改时间格式。strftime也就是timestamp.vim这个插件更新时间所用的函数，它是Unix系统的一个专用于把系统时间信息转换成规范格式的字符串的内建函数。
