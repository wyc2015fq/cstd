# 跟我一起写shell补全脚本（Bash篇） - 文章 - 伯乐在线
原文出处： [spacewander](https://segmentfault.com/a/1190000002968868)
如果你是一个重度shell用户，一定会关注所用的shell的补全功能。某款shell的补全强弱，也许就是决定你的偏好的第一要素。
shell里面补全的影子无处不在，输入命令的时候可以有补全，敲打选项的时候可以有补全，选择文件的时候可以有补全。有些shell甚至支持通过补全来切换版本控制的分支。由于shell里面可以运行的程序千差万别，shell一般不会内置针特定对某个工具的补全功能。与之相对的，shell提供了一些补全用的API，交由用户编写对应的补全脚本。
在这里，我想向大家介绍如何利用提供的API，来编写一个shell补全脚本。由于需要覆盖的内容较多，所以分为Bash和Zsh两篇。也许有fish用户会抱怨，fish又一次被忽略了:D。之所以只有Bash和Zsh的内容，是因为：1. 这两种shell的用户占了shell用户的绝大多数。2. 我没有用过fish，所以对这方面也不了解。希望有人能够锦上添花，写一个fish版本的补全脚本教程。
既然想要写一个shell补全脚本，那么接下来要决定待补全的对象了。这里我选择[pandoc](http://pandoc.org/)作为目标。`pandoc`是文档转换器中的瑞士军刀，支持主流的各种标记语言，甚至对于PDF和MS Word也有一定程度上的支持。`pandoc`支持的选项琳琅满目，如果都要实现确实很花时间。所以这里就只实现General options，Reader options，General writer options大部分的内容。不管怎么说，这将会是一个“既不至于简单到让人丧失兴趣，又不至于困难到让人丧失信心”的任务。
安装`pandoc`的方式见官网上的说明，这里就不赘述了。安装完了之后，`man pandoc`就能看到各个选项的说明。大体上我们需要实现以下几个目标：
- 支持主选项（General options）
- 支持子选项（Reader options/General writer options）
- 支持给选项提供参数值来源。比如在敲`pandoc -f`之后，能够补全`FORMAT`的内容。
好，让我们开始给`pandoc`写补全脚本吧！
## 支持主选项
先列出实现了第一阶段目标的程序：

Shell
```
# 以pandoc的名字保存下面的程序
_pandoc() {
    local pre cur opts
    COMPREPLY=()
    #pre="$3"
    #cur="$2"
    pre=${COMP_WORDS[COMP_CWORD-1]}
    cur=${COMP_WORDS[COMP_CWORD]}
    opts="-f -r -t -w -o --output -v --version -h --help"
    case "$cur" in
    -* )
        COMPREPLY=( $( compgen -W "$opts" -- $cur ) )
    esac
}
complete -F _pandoc -A file pandoc
```
运行程序的方式：

Shell
```
shell$ . ./pandoc # 加载上面的程序
$ pandoc -[Tab][Tab] # 试一下补全能用不
```
现在我来解释下这个程序。

Shell
```
complete -F _pandoc -A file pandoc
```
是这段代码中最为关键的一行。其实该程序起什么名字都不重要，重要的是要有上面这一行。上面这一行指定bash在遇到`pandoc`这个词时，调用`_pandoc`这个函数生成补全内容。（叫`_pandoc`其实只是出于惯例，并不一定要在前面加下划线）。`complete -F`后面接一个函数，该函数将输入三个参数：要补全的命令名、当前光标所在的词、当前光标所在的词的前一个词，生成的补全结果需要存储到`COMPREPLY`变量中，以待bash获取。`-A file`表示默认的动作是补全文件名，也即是如果bash找不到补全的内容，就会默认以文件名进行补全。
假设你在键入`pandoc -o sth`后，连击两下Tab触发了补全，`_pandoc`会被执行，其中：
- `$1`的值为`pandoc`
- `$2`的值为`sth`
- `$3`的值为`-o`
- 由于`COMPREPLY`为空（只有`cur`以`-`开头时，`COMPREPLY`才会被填充），所以补全的内容是当前路径下的文件名。
你应该看到了，这里我把`$2`和`$3`都注释掉了。其实

Shell
```
pre="$3"
cur="$2"
```
和

Shell
```
pre=${COMP_WORDS[COMP_CWORD-1]} # COMP_WORDS变量是一个数组，存储着当前输入所有的词
cur=${COMP_WORDS[COMP_CWORD]}
```
是等价的。不过后者的可读性更好罢了。
最后解释下`COMPREPLY=( $( compgen -W "$opts" -- $cur ) )`这一行。
`opts`就是`pandoc`的主选项列表。
`compgen`接受的参数和`complete`差不多。这里它接受一个以`IFS`分割的字符串`"$opts"`作为补全的候选项（`IFS`即shell里面表示分割符的变量，默认是空格或者Tab、换行）。假如没有一项跟当前光标所在的词匹配，那么它返回当前光标所在的词作为结果。（也即是不补全）
实现第一个目标用到的东西就是这么多。接下来就是第二个目标了。
在继续之前，你需要把[Bash文档](http://www.gnu.org/software/bash/manual/html_node/Programmable-Completion-Builtins.html)看一遍。若能把其中的一些选项尝试一下就更好了。
## 支持子选项
接下来的目标是支持Reader options/General writer options。想判断是否需要补全Reader options/General writer options，先要确认输入的词里面是否有`-r`和`-f`（读），以及`-w`和`-t`（写）。前面提到的`COMP_WORDS`就派上用场了。只需要将它迭代一下，查找里面有没有我们需要确认的词。
假设我们已经确认了需要补全子选项，接下来就应该往原来的补全项中添加子选项的内容。需要补全读选项的添加读方面的选项，需要补全写选项的添加写方面的选项。既然补全选项是一个字符串，那么把要添加的字符串接到原来的`opts`后面就好了。这里要注意一点，假如前面的操作里面已经把某类子选项添加到`opts`了，那么就需要避免重复添加。
目前的实现代码如下：

Shell
```
_pandoc() {
    local pre cur
    COMPREPLY=()
    #pre="$3"
    #cur="$2"
    pre=${COMP_WORDS[COMP_CWORD-1]}
    cur=${COMP_WORDS[COMP_CWORD]}
    complete_options() {
        local opts i
        opts="-f -r -t -w -o --output -v --version -h --help"
        for i in "${COMP_WORDS[@]}"
        do
            if [ "$i" == "-f" -o "$i" == "-r" ]
            then
                opts="$opts"" -R -S --filter -p"
                break
            fi
        done
        for i in "${COMP_WORDS[@]}"
        do
            if [ "$i" == "-t" -o "$i" == "-w" ]
            then
                opts="$opts"" -s --template --toc"
                break
            fi
        done
        echo "$opts"
    }
    case "$cur" in
    -* )
        COMPREPLY=( $( compgen -W "$(complete_options)" -- $cur ) )
    esac
}
complete -F _pandoc -A file pandoc
```
注意跟上一个版本相比，这里把原来的`opts`变量替换成了`complete_options`这个函数的输出。通过使用函数，我们可以动态地提供补全的来源。比如我们可以在函数里列出符合特定条件的文件名，作为补全的候选词。
## 支持给选项提供参数值来源
好了，现在是最后一个子任务。大致浏览一下`pandoc`的文档，基本上就两类参数：`FORMAT`和`FILE`。（其它琐碎的我们就不管了，嘿嘿）
`FILE`好办，默认就可以补全路径嘛。那就看看`FORMAT`。`FORMAT`分两种，一种是读的时候支持的`FORMAT`，另一种是写的时候支持的`FORMAT`，这个把文档里面的复制一份，改改就能用了。我们把读操作支持的`FORMAT`叫做`READ_FORMAT`，相对的，写操作支持的`FORMAT`叫做`WRITE_FORMAT`。
补全的来源有了，想想什么时候把它放到`COMPREPLY`里去。前面补全选项的时候，是通过case语句中`-*`来匹配的。但是这里的`FORMAT`参数，只在特定选项后面才有意义。所以前面一直坐冷板凳的`pre`变量可以上场了。
`pre`中存储着光标前一个词。我们就用一个case语句判断前面是否是`-f`或`-r`，还是`-t`或`-w`。如果符合前面两个组合之一，用`compgen`配合`READ_FORMAT`或`WRITE_FORMAT`生成补全候选词列表，一切就跟处理`opts`时一样。由于此时继续参与下一个判断`cur`的case语句已经没有意义了，这里直接让它退出函数：

Shell
```
READ_FORMAT="native json markdown markdown_strict markdown_phpextra 
    markdown_github textile rst html docbook opml mediawiki haddock latex"
WRITE_FORMAT="native json plain markdown markdown_strict 
    markdown_phpextra markdown_github rst html html5 latex beamer context 
    man mediawiki textileorg textinfo opml docbook opendocument odt docx 
    rtf epub epub3 fb2 asciidoc slidy slideous dzslides revealjs s5"
case "$pre" in
-f|-r )
    COMPREPLY=( $( compgen -W "$READ_FORMAT" -- $cur ) )
    return 0
    ;;
-t|-w )
COMPREPLY=( $( compgen -W "$WRITE_FORMAT" -- $cur ) )
    return 0
esac
```
再`. ./pandoc`一下，试试看，是不是一切都ok？
诶呀，还有个问题！这次在尝试补全`FORMAT`的时候，还会把当前路径下的文件名补全出来。然而这并没有什么意义。所以在补全`FORMAT`的时候，得把路径补全关掉才行。
问题在于最后一句：`complete -F _pandoc -A file pandoc`。目前不管是什么情况，都会补全文件名。所以接下来得限定某些情况下才补全文件名。
第一步是移除最后一行的`-A file`，下一步是修改最底下的case语句，变成这样子：

Shell
```
case "$cur" in
-* )
    COMPREPLY=( $( compgen -W "$(complete_options)" -- $cur ) );;
* )
    COMPREPLY=( $( compgen -A file ))
esac
```
只有在没有找到对应的补全时，才会调用对路径的补全。
最终版本：

Shell
```
_pandoc() {
    local pre cur
    COMPREPLY=()
    #pre="$3"
    #cur="$2"
    pre=${COMP_WORDS[COMP_CWORD-1]}
    cur=${COMP_WORDS[COMP_CWORD]}
    READ_FORMAT="native json markdown markdown_strict markdown_phpextra 
    markdown_github textile rst html docbook opml mediawiki haddock latex"
    WRITE_FORMAT="native json plain markdown markdown_strict 
    markdown_phpextra markdown_github rst html html5 latex beamer context 
    man mediawiki textileorg textinfo opml docbook opendocument odt docx 
    rtf epub epub3 fb2 asciidoc slidy slideous dzslides revealjs s5"
    case "$pre" in
    -f|-r )
        COMPREPLY=( $( compgen -W "$READ_FORMAT" -- $cur ) )
        return 0
        ;;
    -t|-w )
        COMPREPLY=( $( compgen -W "$WRITE_FORMAT" -- $cur ) )
        return 0
    esac
    complete_options() {
        local opts i
        opts="-f -r -t -w -o --output -v --version -h --help"
        for i in "${COMP_WORDS[@]}"
        do
            if [ "$i" == "-f" -o "$i" == "-r" ]
            then
                opts="$opts"" -R -S --filter -p"
                break
            fi
        done
        for i in "${COMP_WORDS[@]}"
        do
            if [ "$i" == "-t" -o "$i" == "-w" ]
            then
                opts="$opts"" -s --template --toc"
                break
            fi
        done
        echo "$opts"
    }
    case "$cur" in
    -* )
        COMPREPLY=( $( compgen -W "$(complete_options)" -- $cur) )
        ;;
    * )
        COMPREPLY=( $( compgen -A file ))
    esac
}
complete -F _pandoc pandoc
```
## 最后的问题
现在补全脚本已经写好了，不过把它放哪里呢？我们需要找到这样的地方，每次启动bash的时候都会自动加载里面的脚本，不然每次都要手动加载，那可吃不消。
`.bashrc`是一个（不推荐的）选择，不过好在bash自己就提供了在启动时加载补全脚本的机制。
如果你的系统有这样的文件夹：`/etc/bash_completion.d`，那么你可以把补全脚本放到那。这样每次bash启动的时候就会加载你写的文件。
如果你的系统里没有这个文件夹，你需要查看下`/etc/bash_completion`这个文件。bash启动的时候，会执行`. /etc/bash_completion`，你可以把你的补全脚本放在这个地方。
正如许多配置文件一样，凡是有`/etc`版本的也对应的`~/.`版本。有`/etc/bash_completion`，自然也有`~/.bash_completion`。如果你只想让自己使用这个补全脚本，或者没有root权限，可以放在`~/.bash_completion`。
Bash补全脚本的内容就是这么多……请期待下一篇的Zsh补全脚本。
