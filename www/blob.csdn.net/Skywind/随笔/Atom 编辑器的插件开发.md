# Skywind Inside » Atom 编辑器的插件开发
## Atom 编辑器的插件开发
March 14th, 2016[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
老王卖瓜，自卖自夸，Atom 比较方便的地方是可以用 javascript/coffee 给 Atom写插件，并且写起来很简单，我刚按说明给 Atom 写了一个插件：
[atom-shell-commands](https://atom.io/packages/atom-shell-commands)
用户自定义 Shell 命令，类似 NotePad++ 中的 “Run Commands”，EditPlus/UltraEdit里面的”User Tool”，以及 GEdit 中的 “External Tool” 和 TextMate 里的 “Shell Command”。
1. 用户可以自定义工具，并且配置到 Atom 中，比如一键调用编译器，一键运行，
2. 输出结果会显示再底部的 bottom panel 中
3. 点击错误输出可以跳转到对应有错误的文件上去
4. 自定义正则表达式匹配错误输出中包含的文件名和行号。
5. 全平台支持，再 Mac/Ubuntu/Windows 下充分的测试过。
初始化时，再你的用户配置中（Atom File->Open Your Config或者 ~/.atom/config.cson），写入类似：
```js
"atom-shell-commands":
    commands: [
      {
        name: "compile"
        command: "d:/dev/mingw/bin/gcc"
        arguments: [
          "{FileName}"
          "-o"
          "{FileNameNoExt}.exe"
        ]
        options:
          cwd: "{FileDir}"
          keymap: 'ctrl-2'
      }
    ]
```
的配置，就会创建一条名为 “atom-shell-commands:compile” 的命令，你可以通过 command palette来运行它或者使用快捷键 ctrl-2来直接运行。同时再 Atom 的 packages 目录下面的”Atom Shell Commands” 目录项下面也会多出一个名为“compile” 的命令。
注意，cson格式靠缩进来判断层次，同时 list 需要将 [ 和 key 写在同一行（commands，arguments），否则会被判断成字典。
靠，上面的代码缩进帖过来以后被弄没了，具体可以看：[https://atom.io/packages/atom-shell-commands](https://atom.io/packages/atom-shell-commands) 上面的格式，CSON是靠缩进来判断层次的，同时注意：commands: [ 后面的中括号要和 commands写在一行，后面的 arguments也一样，cson的 list 需要把 [ 和 key 写在一行，否则会被判断为字典。
每条命令可以通过如下字段来表示：
> 
name: 名字（必填）
command: 可执行路径（必填）
arguments: 参数（选填）
options: 扩展参数用来配置当前工作路径，文件是否保存，快捷键，环境变量等 （选填）
所有配置支持如下宏：
> 
{FileName} 当前正在编辑的文件名（不包括路径）
{FilePath} 包含全路径的文件名
{FileNameNoExt} 没有路径和扩展名的文件名
{FileExt} 文件扩展名
{FileDir} 文件路径
{ProjectDir} 工程路径（如果有工程的话）
{ProjectRel} 文件相对于工程的路径（如果有工程的话）
{CurRow} 当前行
{CurCol} 当前列
{CurLineText} 当前行的文本
{CurSelected} 当前选中文本
有了这些宏，你几乎可以做任何事情，比如到工程目录下调用make或者cmake，或者仅仅编译单个文件不必理会负责的工程，或者调用 ant去跑 build.xml，或者调用 java来运行编译出来的.class，调用python来运行当前脚本，或者当前目录下执行下 grep 关键字并且把grep的结果显示再下面的 bottom panel，打开帮助文档跳转到你选中的API上，或者对当前文件调用 svn diff，把结果输出到 bottom panel。
比如：调用 gcc / cl 一键调用编译功能
![](http://www.skywind.me/blog/wp-content/uploads/2016/03/1458010408_thumb.png)
比如：一键运行
![](http://www.skywind.me/blog/wp-content/uploads/2016/03/1458010376_thumb.png)
比如：一键运行（打开独立窗口运行）
![](http://www.skywind.me/blog/wp-content/uploads/2016/03/1458010419_thumb.png)
具体见项目主页：
[atom-shell-commands](https://atom.io/packages/atom-shell-commands)
