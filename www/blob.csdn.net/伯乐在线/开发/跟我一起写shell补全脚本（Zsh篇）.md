# 跟我一起写shell补全脚本（Zsh篇） - 文章 - 伯乐在线
原文出处： [spacewander](https://segmentfault.com/a/1190000002994217)
绝大部分日常使用Linux和OS X的程序员都会选择zsh作为自己的shell环境，毕竟对比于bash，zsh的便利性/可玩性要胜出很多，同时它又能兼容bash大多数的语法。不过相对而言，zsh补全脚本要比bash补全脚本要难写。zsh提供了非常多的补全的API，而且这些API功能有不少重叠的地方，掌握起来并不容易。不像bash，你只需记住三个API（`compgen`，`complete`，`compopt`）就能实现整个补全脚本。
这篇的任务跟上一篇的一样，需要实现一个针对`pandoc`的补全脚本，囊括下面三个目标：
- 支持主选项（General options）
- 支持子选项（Reader options/General writer options）
- 支持给选项提供参数值来源
## 何处安放脚本
在开始之前，需要说明下放置zsh脚本的地方，这样我们才能让接下来写的补全脚本发挥效力。
zsh在启动时会加载`$fpath`路径下的脚本文件。试试`echo $fpath`来看看这个变量的值。接下来我们可以把补全脚本放到`$fpath`的路径下，或者创建一个新的在`$fpath`路径中的目录：
- `mkdir ~/.fpath`
- 在`~/.zshrc`中添加`fpath=($HOME/.fpath $fpath)`
- 重启zsh
当我们把自己写的补全脚本放好后，每次zsh一启动，就会加载它。不过总不能每次修改完脚本后，都重启一次zsh吧。如果只是单纯更新补全脚本，可以执行`unfunction _pandoc && autoload -U _pandoc`，zsh就会重新加载补全脚本了。（其中`_pandoc`是补全脚本的名字）
## 支持主选项
还是跟上一篇一样，先解释一个实现第一个目标的程序，带各位入门：

Shell
```
zsh#compdef pandoc
# 把它命名为_pandoc，保存在$fpath路径下
_arguments 
    {-f,-r}'[-f FORMAT, -r FORMAT, Specify input format]' 
    {-t,-w}'[-t FORMAT, -w FORMAT, Specify output format]' 
    {-o,--output}'[-o FILE, --output=FILE, Write output to FILE instead of stdout]' 
    {-h,--help}'[Show usage message]' 
    {-v,--version}'[Print version]' 
    '*:files:_files'
```
就像bash的`complete`，zsh也有一个相对的表示补全的API，就是`compdef`。zsh补全脚本以`#compdef tools`开头，表示该文件是针对`tools`的补全脚本。当然你也可以像bash一样，直接`compdef _function tools`来指定`tools`的补全函数。
zsh补全API的第一梯队是`_alternative`、`_arguments`、`_describe`、`_gnu_generic`、`_regex_arguments`。它们直接提供补全的来源。这些API的概述见[https://github.com/zsh-users/zsh-completions/blob/master/zsh-completio…](https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org#main-utility-functions-for-overall-completion)。由于`_describe`能做的`_arguments`也能做，`_gnu_generic`是为GNU拓展的命令参数准备的，`_regex_arguments`就是正则匹配版的`_arguments`，所以只要记住`_arguments`和`_alternative`就够用了。
`_arguments`接受一连串的选项字符串，每个字符串代表一个选项。另外你还可以通过一些选项指定补全上的细节。举`-s`为例：假设你的工具支持`-a -b`两个选项，也支持`-ab`的方式来同时指定两个选项。如果没给`_arguments`提供`-s`的选项，那么zsh是不会补全出`-ab`，因为并不存在选项`-ab`。而提供了`-s`后，`_arguments`才允许你在已经输入`-a`的情况下，补全出`-ab`。
选项字符串的格式是这样的：`-x[description]:message:action`。你也可以写做`{-x,-y}[description]:message:action`形式，表示`-x`和`-y`是等价的写法。
- `-x`是选项的名字
- `[description]`是该选项的描述，可选
- `message`这一项我也不知道是什么意义……不过它是可选的，除非你需要指定action
- `action`用于生成复杂的补全。在这里你可以使用许多补全语法。一个常见的例子是使用辅助函数，比如`_files`表示补全当前路径下的文件名。详见：
- [https://github.com/zsh-users/zsh-completions/blob/master/zsh-completio…](https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org#actions)
- [https://github.com/zsh-users/zsh-completions/blob/master/zsh-completio…](https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org#functions-for-completing-specific-types-of-objects)
最后一行`'*:files:_files'`表示，如果找不到匹配的候选词，就补全文件名。
到目前为止，实现第一阶段目标的脚本所需的知识点已经讲解完毕。
`_arguments`有一个限制，它要求选项的名字符合某些特殊格式，比如以`-`、`+`、`=`等字符开头（所以才叫`_arguments`嘛）。如果你的工具接受`add`、`remove`之类的子命令，就需要用到`_alternative`。
`_alternative`支持的选项字符串格式跟`_arguments`很像，比如

Shell
```
_arguments 
    {-t,-w}'[-t FORMAT, -w FORMAT, Specify output format]'
```
等价于

Shell
```
_alternative 
    'writer:writer options:((-t:"-t FORMAT, -w FORMAT, Specify output format" -w:"-t FORMAT, -w FORMAT, Specify output format"))'
```
## 支持子选项
所谓的支持子选项，就是在某些选项存在的情况下，增加多一些选项。所以，我们所要做的，就是检查当前输入的命令行参数中是否存在某些参数，如果存在，增加新的选项。这一步可以分解成两个步骤，第一个是检查某些参数是否存在，第二个是增加新的选项。
之前写bash补全脚本的时候，是通过遍历某个存储有当前输入的常量数组，来检查某些参数是否存在。在网上搜索一番后，我发现zsh也有同样的常量数组，就叫做`words`，正好是bash那个的小写哈。那么接下来就是zsh的语法知识了：

Shell
```
zshif [[ ${words[(i)-f]} -le ${#words} ]] || [[ ${words[(i)-r]} -le ${#words} ]]
then
    # 修改补全候选列表
fi
if [[ ${words[(i)-t]} -le ${#words} ]] || [[ ${words[(i)-w]} -le ${#words} ]]
then
    # 修改补全候选列表
fi
```
这里用到一点zsh特有的下标语法，相当于`index()`。
那么下面是第二步，该怎么修改补全候选列表呢？如果直接用`_arguments`指定新的补全列表，会覆盖掉前面指定的补全列表。当然也可以把前面的补全列表复制一份，并添加新的选项，用它覆盖掉原来的补全列表。不过这么一来代码就不好看了。
想来zsh应该提供了对应的API的。果不其然，有一个`_values`可以用来干这事。`_values`功能跟`_arguments`差不多，而且它接受的选项列表是添加到原有的选项列表中的，而不是覆盖。所以最后的代码是这样的：

Shell
```
zshif [[ ${words[(i)-f]} -le ${#words} ]] || [[ ${words[(i)-r]} -le ${#words} ]]
then
    _values 'reader options' 
        '-R[Parse untranslatable HTML codes and LaTeX as raw]' 
        '-S[Produce typographically correct output]' 
        '--filter[Specify an executable to be used as a filter]' 
        '-p[Preserve tabs instead of converting them to spaces]'
fi
if [[ ${words[(i)-t]} -le ${#words} ]] || [[ ${words[(i)-w]} -le ${#words} ]]
then
    _values 'writer options' 
        '-s[Produce output with an appropriate  header  and  footer]' 
        '--template[Use FILE as a custom template for the generated document]' 
        '--toc[Include an automatically generated table of contents]'
fi
```
## 支持给选项提供参数值来源
最后一步是给`-f`和`-r`这两个选项提供读操作支持的FORMAT参数，给`-t`和`-w`这两个选项提供写操作支持的FORMAT参数。
在Bash篇的实现中，我们检查上一个词的值，如果它是`-f`或`-r`，那么对当前词补全读操作的FORMAT参数。对写操作的选项也同理。
在zsh中，我们可以用一个特殊的Action：`->VALUE`来实现。
`->VALUE`这样的Action会把`$state`变量设置成`VALUE`，接下来靠一个case语句块就能根据当前陷入的状态进行对应的参数补全。
那么该如何补全FORMAT参数列表呢？这里可以用上`_multi_parts`。
`_multi_parts`第一个参数是分隔符，之后接受一组候选词或一个候选词数组作为候选词列表。例如`_multi_parts , a,b,c`，就会生成`a b c`这个补全候选列表。
这里的FORMAT变量直接使用上一章的`$READ_FORMAT`和`$WRITE_FORMAT`。
我试了一下，如果把FORMAT变量当做字符串传递过去的话，其间的空格会被转义，导致无法分隔开来，于是就把它们改写成数组的形式。
另外，由于补全FORMAT参数时，不再需要补全选项了。所以把补全FORMAT参数的部分提到补全子选项的前面，并在补全后直接退出程序的执行。
最终完成的代码如下：

Shell
```
zsh#compdef pandoc
local READ_FORMAT WRITE_FORMAT
READ_FORMAT='(native json markdown markdown_strict markdown_phpextra 
markdown_github textile rst html docbook opml mediawiki haddock latex)'
WRITE_FORMAT='(native json plain markdown markdown_strict 
markdown_phpextra markdown_github rst html html5 latex beamer context 
man mediawiki textileorg textinfo opml docbook opendocument odt docx 
rtf epub epub3 fb2 asciidoc slidy slideous dzslides revealjs s5)'
_arguments 
    {-f,-r}'[-f FORMAT, -r FORMAT, Specify input format]: :->reader' 
    {-t,-w}'[-t FORMAT, -w FORMAT, Specify output format]: :->writer' 
    {-o,--output}'[-o FILE, --output=FILE, Write output to FILE instead of stdout]' 
    {-h,--help}'[Show usage message]' 
    {-v,--version}'[Print version]' 
    '*:files:_files'
case "$state" in
    reader )
        _multi_parts ' ' $READ_FORMAT && return 0
        ;;
    writer )
        _multi_parts ' ' $WRITE_FORMAT && return 0
esac
if [[ ${words[(i)-f]} -le ${#words} ]] || [[ ${words[(i)-r]} -le ${#words} ]]
then
    _values 'reader options' 
        '-R[Parse untranslatable HTML codes and LaTeX as raw]' 
        '-S[Produce typographically correct output]' 
        '--filter[Specify an executable to be used as a filter]' 
        '-p[Preserve tabs instead of converting them to spaces]'
fi
if [[ ${words[(i)-t]} -le ${#words} ]] || [[ ${words[(i)-w]} -le ${#words} ]]
then
    _values 'writer options' 
        '-s[Produce output with an appropriate  header  and  footer]' 
        '--template[Use FILE as a custom template for the generated document]' 
        '--toc[Include an automatically generated table of contents]'
fi
```
## 后话
由于zsh的补全功能实在强大，而这篇文章只是简略地讲讲如何写出一个zsh补全脚本，有许多zsh的补全机制都没能提到。所以补充一些写zsh补全脚本的资料，如果对这方面有兴趣可以继续跳坑：
- [zsh-completions](https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org)项目上的教程。这是我见过的最详尽的zsh补全脚本教程。
- [官方文档](http://zsh.sourceforge.net/Doc/Release/Completion-System.html)
- /usr/share/zsh/functions/Completion 也许你能从相似的命令的补全脚本中汲取灵感。
顺便一提，在查找资料的时候发现有人写了一个完整的pandoc的zsh补全脚本，感兴趣的话可以看一下：
[https://github.com/srijanshetty/zsh-pandoc-completion/blob/master/_pan…](https://github.com/srijanshetty/zsh-pandoc-completion/blob/master/_pandoc)
