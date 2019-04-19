# 12 个 Vim 小窍门 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [周进林](http://www.jobbole.com/members/8zjl8) 翻译。未经许可，禁止转载！
英文出处：[Dalibor Nasevic](https://dalibornasevic.com/posts/43-12-vim-tips)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
这篇 Vim 技巧文章，包含了一些我觉得比较有意思的从中级到高级的 Vim 技巧，让我们一起来回顾一下。
### 1. 重复上一次 Ex 命令
在 Vim 中有 3 种命令行模式：
- Ex 命令模式： `：something`
- 搜素 模式： `/something`
- 表达式 模式（在插入模式里输入 <`C-r>=` 进入）： `=1+1<cr>`
假如我们通过以下命令在 Ex 命令模式里运行规范文件：


```
:!rspec spec/models/code_spec.rb
```
我们可以在普通模式里输入 `@:` 来重复上一次 Ex 命令。
### 2. 对 CSS 的属性进行排序
如果我们想对 CSS 选择器里面的属性行按照字母顺序排序，我们只需要在可视化模式里选择这些属性行并且输入 `:sort` ，然后就可以看到排序后的结果。


```
h4 {
    font-size: 11px;
    margin: 15px;
    background: red;
}
```
### 3. 选择复制并且粘贴文本
我们可以通过使用 `gv` 来选择上一次选择复制的文本。并且，可以使用 `gv` 来选择上一次粘贴的文本，`gb` 是一个我们已经添加到 vimrc 文件里的自定义映射。当我们进行方法重构、剪切-复制然后进行缩进对齐时，这些命令尤为好用。有了`gb` 命令，我们可以很容易地选中刚粘贴的文本，然后使用 `<` 或者 `>` 来修正缩进。


```
" select last paste in visual mode
nnoremap <expr> gb '`[' . strpart(getregtype(), 0, 1) . '`]'
```
### 4. 在插入模式里粘贴文本
假设我们现在处于插入模式并且想在不离开插入模式的情况下粘贴文本，我们可以使用 `<C-r>0` 命令。如果复制的文本里包括换行符，`<C-r><C-p>0` 命令可以帮我们处理缩进问题。
### 5. 在插入模式里删除
当我们处于 Vim 的插入模式、命令行模式或者 Shell 的命令行，我们可以使用以下快捷键来删除一个字符、单词或者一行：


```
<C-h> " 向前删除一个字符（类似退格键）
<C-w> " 向前删除一个单词
<C-u> " 删除至行首
<C-k> " 删除至行尾
```
### 6. 多行范围内运行正常模式命令
假设我们忘了给以下的 Javascript 代码行的行尾添加 `;`：


```
var element = $(this)
var tabName = element.data('tab')
var report = element.data('report')
```
要给每一行添加分号，我们可以在可视化模式里选中这3行代码，然后运行 `:normal A;`命令。Vim 会在选中的每一行中执行 `A;` 命令（添加`;`的命令）。我们也可以使用另外一种方式，我们可以对整个文件的内容使用 `:%normal A;` 命令。
### 7. 在多行上重复最后一次修改
在上面的代码片段中，如果我们只在第一行上面使用`A;` 命令向行尾添加`;`。我们可以选择2到3行的代码并且运行 `:'<,'> normal .`命令，为选中的每一行执行点号命令（dotcommand）重复上一次修改命令。
### 8. 在多个文件中执行替换
在 Vim 中，对多个文件执行替换操作并不是一个直观的操作。要在多个文件里进行搜索和替换，首先我们需要创建一个文件列表，之后在这些文件里面执行命令。我们可以使用 `:args` 命令来创建文件列表。例如，加载所有以“ext”开头的 Javascript 文件：


```
:args app/assets/javascripts/ext*.js
```
我们可以通过`:args`命令来查看有哪些文件被加载到参数列表中。当文件都加载到参数列表中后，我们可以运行搜索和替换命令：


```
:argdo %s/From/To/g
```
一旦修改应用到文件后，我们可以通过以下命令来更新参数列表中的文件：


```
:argdo update
```
### 9. 在多个文件中进行搜索和替换
当我们知道要执行替换命令的文件在那的情况下，之前的替换技巧会很有用。但是，在大多数情况下，我们想要的是，在一堆文件里搜索文本并且仅在匹配的文件里执行替换命令。要实现这个替换操作，我们可以使用`vimgrep` 命令在文件中搜索匹配的文本。`vimgrep` 命令会创建一个快速列表（quicklist）来展示包含与模式匹配的文本的文件列表，我们可以使用 `:copen` 命令来打开这个列表。如果我们想充分利用好快速列表（quicklist)，我们可以使用 `:Qargs` 命令把快速列表转换为参数列表， `:Qargs` 命令是我们在 vimrc 文件里设置的一个映射：


```
command! -nargs=0 -bar Qargs execute 'args' QuickfixFilenames()
" populate the argument list with each of the files named in the quickfix list
function! QuickfixFilenames()
  let buffer_numbers = {}
  for quickfix_item in getqflist()
    let buffer_numbers[quickfix_item['bufnr']] = bufname(quickfix_item['bufnr'])
  endfor
  return join(map(values(buffer_numbers), 'fnameescape(v:val)'))
endfunction
```
最后， 下面是一个在多个文件中进行搜索和替换的操作命令：


```
:vimgrep /CurrencyNumberHelper/ app/models/*.rb
:Qargs
:argdo %s/CurrencyNumberHelper/CurrencyHelper/g
:argdo update
```
buztard 在评论里提出一个更简单的方法，使用 ack-grep 来创建参数列表：


```
:args `ack-grep -l CurrencyNumberHelper`
# or, if you're using Ack plugin, then just :Ack CurrencyNumberHelper
:argdo %s/CurrencyNumberHelper/CurrencyHelper/g
:argdo update
```
如果你使用[版本比较新的 Vim](https://github.com/vim/vim/commit/aa23b379421aa214e6543b06c974594a25799b09)， 这个操作会更简单。正如 Thiago A. z 在评论里提出的方法，你可以简单地使用 `cdo` 和 `cfdo` 命令即可：


```
:Ack CurrencyNumberHelper
:cfdo s/CurrencyNumberHelper/CurrencyHelper/g | update
```
### 10. 编辑已经录制的宏
在录制宏的过程中经常会出现误操作，当出现出现误操作时，对已录制的宏进行编辑比重新录制更简单方便。假设我们录制了一个宏到寄存器a里，我们可以使用 `:put a` 命令把宏的内容打印到当前的缓冲区里，然后在 Vim 里对这个宏进行编辑。一旦对宏的内容进行了修改，我们可以选择修改后的宏并且使用 `"ay` 命令把它复制到寄存器`a`中。之后我们就可以使用 [`@a`](http://www.jobbole.com/members/a58797) 命令运行寄存器 a 中的新宏。
### 11. 在多个文件里运行宏
如果我们已经录制了宏，并且想在某些文件里运行它。首先，我们需要使用这些文件来建立一个参数列表。比如我们需要加载一个 Rails 应用的所有 model：


```
:args app/models/*.rb
```
然后我们使用以下命令老来运行宏（这个宏已经录制保存在寄存器 a1a 中）：


```
:argdo normal @a1a
```
最后，我们使用下面命令来保存所有缓冲区的修改：


```
:argdo update
```
### 12. 命令行中的 Vi 模式
Bash shell 提供了两种命令行编辑模式：emacs 和 vi 模式。emacs 编辑模式是命令行的默认模式，我们可以使用以下命令来把命令行的编辑模式改为 vi 模式：


```
set -o vi
```
然后我们就可以在命令行模式里使用 ESC 键来进入 vi 的编辑模式，并且可以使用 vi 文本行编辑能力。当我们在命令行里处于 vi 编辑模式时，我们可以使用 v 键弹出编辑器，我们在里面编辑保存的命令会直接在命令行里运行。这是一个很赞的功能， 你觉得呢？:)
如果我们需要返回 emacs 模式，打印一下命令即可：


```
set -o emacs
```
这里有一张[速查表](http://www.catonmat.net/download/bash-vi-editing-mode-cheat-sheet.txt)，记录了在 bash shell 中使用 vi 模式的可用命令。
在寻找更多的 Vim 技巧？ 可以查看作者的 [vimfiles](http://github.com/dalibor/vimfiles)，还有《[25个Vim教程、视频和资源](http://blog.jobbole.com/10250/)》和《[提高 Vim 使用效率的 12 个技巧](http://blog.jobbole.com/87481/)》。
