# Vim高级进阶之ex命令集 - KinboSong的博客 - CSDN博客
2017年12月28日 15:36:11[songjinbo3](https://me.csdn.net/KinboSong)阅读数：225
个人分类：[C++开发](https://blog.csdn.net/KinboSong/article/category/6854166)
## 转载自alexhilton
ex是Vim对行编辑器ed的支持.ed是一个面向行的编辑器.在远古时代,还是黑白终端的时代,有一个流行的面向行的编辑器ed,也即把文件一行一行的读入,一次只显示一行,然后编辑这行.所以Vim中的ex命令都是ed编辑器所支持的命令,现代所用的流编辑器sed中的命令是ed命令的扩展.所以掌握了Vim中的ex编辑命令,稍加变通就连sed也一起搞定了.
Vim是全屏编辑器,因此不会像ed那样只显示一行,对于Vim,ed命令编辑的一般都是当前行,这就相当于ed中所显示的那行,实际上也是文件的当前行.
## 命令常识
在Vim中所有的ex编辑命令都是以:(冒号)开头
启动方式就是在命令模式下输入冒号,然后输入具体命令
在Vim的状态行(通常为屏幕最下面的一行),会显示你所输入的命令,同时也会用于显示命令的输出结果.
由于是面向行的编辑命令,所以基本单位是行,比如拷贝和剪切都是以行为单位(也即最少为一行).
## 命令格式
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- :[range] cmd args  
`:[range] cmd args`
## 杂项命令
这些命令跟具体的文本编辑无关,但是也非常的常用
> - **:=** – 打印文件的总行数
- **:/pattern/=** – 打印匹配pattern的行号
- **:w** – 把当前buffer写入文件,也就是说进行保存动作
- **:w!** – 强制保存,如果是只读文件也能进行保存动作
- **:q** – 退出当前文件,前提是已经保存过了,如果没有保存Vim会提醒你的.
- **:q!** – 强行退出,也就是说无论已经保存过与否都退出,可能会丢失所做的修改
- **:wq** – 保存并退出.
- **:wa** – 把所有的buffer写入文件,也即保存所有的文件(如果你打开了多个文件的话).
- **:x** – 保存并退出,程序员最应该使用的命令.它与:wq的区别在于,如果文件没有修改:x是不会修改文件的时间戳,因此就不需要重新编译.而:wq命令,即使文件未修改也会修改时间戳,也即会触发重新编译.
- **:xa** – 保存并退出所有的文件,如果打开了多个文件话.
- **:w***a-new-file* – 另存为a-new-file
- **:[range] w*** a-new-file* – 把[range]所指定的范围的内容另存为a-new-file
- **:[range] w >>***another-file* – 把[range]所指定的范围的内容追加到another-file中
- **:r***file* or** :read***file* – 把file文件中的内容读出来放到当前行下面.细节是打开file,拷贝其中的内容然后粘贴到当前行下面.
- **:n r***file* – 把file中的拷贝到第n行下面
- **:/pattern/ r***file* – 把file中的内容拷贝到第一个匹配pattern的行下面
- **:e ***another-file* – 把another-file读入内存并开始编辑
- **:e! **– 舍弃所有的未保存的修改,回到上一次文件所保存的状态
## 如何指定范围[range]
很多命令都需要指定范围和行号,如何做呢?
> 0. 不指定行号则通常代表当前行
1. 具体的绝对的行号,行号是从1开始到第x行,x为文件总行数.
> - **:n, m** – 文件的第n行到第m行,包含n和m
> 如:2, 10则是第2行到第10行
如果n=0或n=-1则n自动转成1；如果m大于x则转成x
特殊的行号
> - **.**(句点) – 当前行
- **$ **– 最后一行
> 如**:.,$**则表示文件中的所有行
> - **%** – 所有行等同于:1,x和:.,$
> 2. 相对于当前行和相对行号
> - **+/-n** – +表示从当前向第n行；-表示当前行向前减n行
- **+/-** – 前一行或后一行
> 如**:-,+ d** – 删除当前行,上一行和下一行,共三行；**:-2,+2 d**删除当前行,前二行后二行,共五行
用搜索匹配来指定行
> - **/pattern/** – 指定的是匹配pattern的下一行,也即当前行后面的第一个匹配pattern的行.这个只能指定一行
- **/pattern/, /pattern2/** – 第一个含有pattern的行到第一个含有pattern2的行.
> 注意,如果指定范围时后面的行号小于前面的行号,那么Vim会提示你[range]边界指示反了,要不要swap一下.
重新指定当前行
- **n; m** – 把第n行视为当前行.默认情况下光标所在为当前行.这个命令能快速指定当前行,可以省不少的力气
如:
- **:40; +50 d **– 删除第40行到第50行.这相当于:40跳到40行,然后执行+50 d.
- **:/old/; +10 d** – 删除从含有old的行到其下面的10行.这相当于然后搜索到含有old的行:/old/,然后执行:+10 d
## 编辑命令解析
- **:[range] d **– 删除.如:1, 20 d删除第1到第20行
- **:[range] mv target** – 后面需要加上移动的目的地.剪切[range]指定的部分粘贴到target所指定的位置.
- **:[range] co/t target **– 把[range]指定的部分拷贝到target位置
- **:[range] y/ya** – 复制[range]到剪切板中
- **:[range] y***name* – 复制[range]到剪切板中,并且命名为a,这样就可以同时操作多个拷贝
- **:n pu **– 把剪切板中的内容粘贴到第n行下面
- **:n pu***name* – 把剪切板中名字为name的拷贝粘贴到第n行下面
## 命令行启动命令
就是打开后进行一些操作
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- $vim +n file -- 打开file文件后跳到第n行  </span></span></li><li class=""><span>$vim + file – 打开file文件后跳到最后一行  
- $vim +/pattern file -- 打开file文件后跳到含有pattern的那行  </span></li></ol></div><pre><code class="language-plain">$vim +n file – 打开file文件后跳到第n行
$vim + file – 打开file文件后跳到最后一行$vim +/pattern file – 打开file文件后跳到含有pattern的那行
如,**$vim +/”your life” file** – 打开后跳到”your life”的那行
## 编辑多个文件
命令,模式和buffer会在多个文件和多个窗口之前共享.所以如果同时编辑多个文件会提升一些效率
> 1. 命令行启动多个文件
> 
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- $vim a.txt b.txt c.txt  </span></span></li></ol></div><pre class="plain" name="code" style="display: none;">$vim a.txt b.txt c.txt
> 2. 已经在编辑一个文件时,可以用:e命令开始编辑另外一个文件
- **:e ***another-file*
3. args – 显示所有的文件列表,当前的文件会以[filename]形式标出
> 
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- $vim a.txt b.txt c.txt  </span></span></li><li class=""><span>:args  ----->>>>  </span></li><li class="alt"><span>[a.txt] b.txt c.txt  </span></li><li class=""><span>:n     ----->>>>  </span></li><li class="alt"><span>a.txt [b.txt] c.txt  </span></li></ol></div><pre class="plain" name="code" style="display: none;">$vim a.txt b.txt c.txt
:args  —–>>>>
[a.txt] b.txt c.txt
:n     —–>>>>
a.txt [b.txt] c.txt
> 4. 在文件之间进行切换
- **:p[rev] **– 前一个文件
- **:n[ext] **– 下一个文件
- **:last ** – 最后一个文件
## 双文件模式
> 1. 启动方式:当编辑一个文件的时候,用e命令读入并编辑另外一个文件就进入了双文件模式
2. 二个文件速记符,相当于字符串宏,内容就是所编辑的二个文件名
- **% **– 当前文件名
- **#** – 前一个文件
3. 示例
> 
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- $vim a.txt  </span></span></li><li class=""><span>:w -- 保存修改  </span></li><li class="alt"><span>:e b.txt -- 开始编辑另外一个文件b.txt  </span></li><li class=""><span>:e # -- 开始编辑前一个文件,也就是文件a.txt  </span></li><li class="alt"><span>:e # -- 开始编辑前一个文件,这时是b.txt  </span></li><li class=""><span>:w %.bak -- 另存当前文件为b.txt.bak(此时当前文件%为b.txt)  </span></li><li class="alt"><span>:r # -- 拷贝a.txt的内容到b.txt当前行下  </span></li><li class=""><span>:split # -- 在新窗口中打开a.txt  </span></li><li class="alt"><span>:e! # -- 舍弃当前文件修改,并开始编辑a.txt  </span></li></ol></div><pre class="plain" name="code" style="display: none;">$vim a.txt
:w – 保存修改
:e b.txt – 开始编辑另外一个文件b.txt
:e # – 开始编辑前一个文件,也就是文件a.txt
:e # – 开始编辑前一个文件,这时是b.txt
:w %.bak – 另存当前文件为b.txt.bak(此时当前文件%为b.txt)
:r # – 拷贝a.txt的内容到b.txt当前行下
:split # – 在新窗口中打开a.txt
:e! # – 舍弃当前文件修改,并开始编辑a.txt
> 
## 组合命令
> 
可以用|来把几个命令组合在一起使用,与用;来组合Shell命令是一样的.
如
- **:1, 3 d | s/their/they** – 删除1到3行然后在当前行把their替换为they
## 获得帮助
对于任意一个命令都可以用
- **:help***cmd* – 来获取帮助文档
## 执行Shell命令
- **:!cmd** – 执行Shell命令,会跳到Terminal以显示命令的结果
对于大部分Shell命令可以通过内部来执行
- **:cmd** – 执行命令,其操作结果显示在状态行里.
如**:cd** – 切换当前目录；**:ls**遍历目录内容；**:pwd** – 查看当前工作目录
## 临时性的进入Shell
有时候想到到Shell终端进行某些操作,然后还要回来编辑文件.可以这么做:
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- :x – 保存并退出到终端；做想做的事情  
- $do all stuff you want  </span></li><li class="alt"><span>$vim file – 再次打开并编辑文件  
:x -- 保存并退出到终端；做想做的事情
$do all stuff you want
$vim file -- 再次打开并编辑文件
这是一个很常用的场景,所以就有了更快捷与方便的方法,可以shell或sh命令来做相同的事情
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- :sh – 回到终端  
- $ do you stuff  </span></li><li class="alt"><span>$exit – 当完事后用exit命令就会退出终端然后回到编辑器.  
```
:sh -- 回到终端
$ do you stuff
$exit -- 当完事后用exit命令就会退出终端然后回到编辑器.
```
sh命令很实用.比如常见的编辑编译运行调试例行,编译后就可以用:sh命令退出到Shell进行测试和调试,然后再exit到编辑中.
## Quickfix让编辑编译都不用离开Vim
### 如何编译
最简单的方法当然是直接运行gcc命令:
- **:!gcc -o list list.c -std=c99 -Wall -lm**
这绝对可行,但这很麻烦
Vim中可以使用make命令
- **:make list** – 编译list.c
但是这只会使用make的默认隐匿规则,相当弱,比如没有额外的选项,想指定如Wall或std都是不行的.
当然你可以写一个makefile,然后就可以开心的直接运行:make就可以了.
但如果没有makefile怎么办呢,特别是某些单个文件之类的.每个文件的编译目标和源码都不一样,这时就可以写一个简单的makefile,不指定具体的编译目标和源码文件,而只是定义一些简单的编译选项和规则:
> 
**[plain]**[view plain](#)[copy](#)
[print](#)[?](#)
- #makefile  
- LDLIBS= -lm  
- CFLAGS= -g -std=c99 -Wall -Werror  
```
#makefile
LDLIBS= -lm
CFLAGS= -g -std=c99 -Wall -Werror
```
这样,在Vim中:
- **:make list** – 用makefile中定义的规则来编译list.c
- **:make hello** – 用makefile中定义的规则来编译hello.c
还可以用makeprg命令来指定编译的目标和规则,但是这个命令不好用,而且每换一个新文件就要跟着一起改,不如makefile来的方便.
### 用Quickfix快速修改编译错误
运行了make命令编译之后,如果有编译错误Vim会以列表形式把编译错误列出,并eQuickfix工具快速帮你定位出错的行
- **:cc **– 显示编译错误的详细信息,这些信息显示在状态行里
- **:cn **– 下一个编译错误
- **:cp** – 前一个编译错误
- **:cw** – 打开Quickfix窗口.这会在屏幕下面打开一个新窗口,里面的内容就是一个编译错误列表
- **:cl** – 列出所有的编译错误
Quickfix实际上就是一个编译错误列表,每个Quickfix列表能显示10个错误,所以想看其他的错误(如果多于10的话)
- **:col** – 前一个列表
- **:cnew** – 后一个列表
### 示例:一个单文件的编辑编译运行和调试:
> **$vim hello.c</strong><br></span><span style="font-size:14px;">edit edit<br></span><span style="font-size:14px;"><strong>:make hello</strong> -- 编译hello<br></span><span style="font-size:14px;"><strong>:cc </strong>-- 显示编译错误信息<br></span><span style="font-size:14px;"><strong>:cn </strong>-- 下一个<br></span><span style="font-size:14px;"><strong>:cn </strong>-- 下一个<br></span><span style="font-size:14px;"><strong>:make hello</strong> -- 编译 ... Okay<br></span><span style="font-size:14px;"><strong>:sh</strong> -- 进入Shell<br></span><span style="font-size:14px;"><strong>$./hello** – 运行并测试程序, 有Bug
**$gdb hello</strong> -- 调试 ... 找到了问题<br></span><span style="font-size:14px;"><strong>$exit**
修复运行时问题
**:make hello:sh$./hello<br></span></strong><span style="font-size:14px;"><strong>$exit** – 回到编辑器
## 在Vim中使用强大的搜索神器grep
搜索匹配神器grep也可以在Vim中使用,使用方式
- **:gr/grep [opt] *pattern files*** – 在files中搜索匹配pattern,与Shell命令中的grep的使用方式是一样的.
结果是以Quickfix列表形式给出的,因此Quickfix的命令这里也适用:
- **:cn** – 下一处匹配
- **:cp** – 前一处匹配 
- **:cw** – 打开Quickfix列表,里面是超链接,点击后可以直接进入那个匹配
- **:cl **– 相看所有的匹配
这些命令会从一个匹配跳到下一个或上一个匹配.如果超出了这个文件则会进入另外的文件.所以能在不同文件之间跳来跳去.很是强大.
更多的内容请查询帮助**:help grep**
## 全局搜索和替换
这也是用ex命令,因为比较多可以单独写一篇,请看[这篇文章](http://blog.csdn.net/hitlion2008/article/details/7964811)
