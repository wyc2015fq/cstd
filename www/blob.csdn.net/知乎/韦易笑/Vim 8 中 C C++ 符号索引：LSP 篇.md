# Vim 8 中 C/C++ 符号索引：LSP 篇 - 知乎
# 



接前面《[Vim 8 中 C/C++ 符号索引：GTags 篇](https://zhuanlan.zhihu.com/p/36279445)》，之前我们搭建了一套体验超过任何编辑器的静态符号索引系统，今天将使用 LSP 进行语义级别的符号检索。




**LSP 是什么？**

避免没了解过的人点开网页搜半天，这里一句话说清楚 [LSP（Language Server Protocol）](https://link.zhihu.com/?target=http%3A//langserver.org/)：LSP 是一套通信协议，遵从 LSP 规范的客户端（各种编辑器/IDE）可以通过众多 LSP 服务端按协议标准进行通信，由客户端完成用户界面相关的事情，由服务端提编程语言相关的：补全，定义引用查找，诊断，帮助文档，重构等服务。
![](https://pic2.zhimg.com/v2-cc3fa93a57664e671dba146743c9bfb1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='638' height='479'></svg>)LSP 关系图
LSP 协议就是一套解偶合的标准，比如 C/C++ 补全，以前是 NotePad++ 里做一套，VS里做一套，Vim里做一套，Emacs里又做一套，C/C++ 还好，但是其他语言就惨了，各个编辑器实现的质量参差不齐。LSP 出现后能让编辑器专心提供最好的用户体验，而 LSP Server 则专心将具体语言的各种集成服务做透，做扎实。

这样同一个 LSP Server 可以服务于不同的编辑器 / IDE，不同的编辑器/IDE 一旦支持 LSP，则可以瞬间对接高质量的各种 LSP Servers。




**LSP 服务端和客户端怎么通信的？**

说起 LSP 服务端大家不要误会成 http server 那样的常驻后台服务，好像编辑器只有通过 TCP发送请求上来才行。实际上，LSP Servers 都是一个个命令行程序，由编辑器（也就是客户端）启动，通过管道发送 JSON RPC 命令同 LSP Server 交流，退出编辑器，LSP 服务端也就关闭了。




**有哪些 C/C++ 的 LSP 服务端？**

在 [langserver.org](https://link.zhihu.com/?target=http%3A//langserver.org/) 上有表格可以查，纵向是不同 LSP 实现，横向是该实现支持的功能：
![](https://pic4.zhimg.com/v2-914ea633164907d3a060eeb72edbd97b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1133' height='738'></svg>)
目前 C/C++ 的 LSP Server 实现主要有三个：

[clangd](https://link.zhihu.com/?target=https%3A//clang.llvm.org/extra/clangd.html)：目前看起来最稳定健壮，但是尚无索引系统，不支持多 Translation Units，所以可以补全，查找定义，但是无法搜索引用。

[cquery](https://link.zhihu.com/?target=https%3A//github.com/cquery-project/cquery)：支持多 Translation Units，可以搜索引用，性能不错，但是稳定性不如 clangd。

[ccls](https://link.zhihu.com/?target=https%3A//github.com/MaskRay/ccls)：cquery 的 fork，据说解决了一些 cquery 的痛点。

clangd 虽然是最稳定的，但不支持查找引用，所以我们选择 cquery 作 C/C++ 的 LSP 服务端。




**Vim 中如何支持 LSP ？**

目前有两个插件可以为 Vim 提供 LSP 功能：[vim-lsp](https://link.zhihu.com/?target=https%3A//github.com/prabirshrestha/vim-lsp) 和 [LanguageClient-neovim](https://link.zhihu.com/?target=https%3A//github.com/autozimu/LanguageClient-neovim)。我最开始用的 vim-lsp 搭配 cquery，发现有不少bug，比如重构时变量改名，居然会帮我改错掉，于是我找到了后者，LanguageClient-neovim (简称 LCN）相对前者稳定不少也更活跃，可以比较下，vim-lsp 有一百多次提交 300 多星，而 LCN 有一千多次提交 1000 多星。




**如何配置 LCN 支持 C/C++ ？**

首先要安装 LCN，其次要安装 C/C++ 的 LSP 服务端 cquery，我们简单调教一下 LCN：


```
Plug 'autozimu/LanguageClient-neovim', { 'branch': 'next' }

let g:LanguageClient_loadSettings = 1
let g:LanguageClient_diagnosticsEnable = 0
let g:LanguageClient_settingsPath = expand('~/.vim/languageclient.json')
let g:LanguageClient_selectionUI = 'quickfix'
let g:LanguageClient_diagnosticsList = v:null
let g:LanguageClient_hoverPreview = 'Never'
let g:LanguageClient_serverCommands = {}
let g:LanguageClient_serverCommands.c = ['cquery']
let g:LanguageClient_serverCommands.cpp = ['cquery']

noremap <leader>rd :call LanguageClient#textDocument_definition()<cr>
noremap <leader>rr :call LanguageClient#textDocument_references()<cr>
noremap <leader>rv :call LanguageClient#textDocument_hover()<cr>
```


这里主要是定义了一些行为，比如告诉 LCN 查找定义或者引用时将结果输出到 quickfix 窗口，以及规定了 C/C++ 程序的话启动 cquery 程序作为 LSP Server，最后定义了搜索定义，搜索引用以及查看文档等几大主要功能的快捷键。

这些配置名称都很值观，hover 的意思取自鼠标在函数名上悬停时显示的帮助文档，这里衍生出取得函数或者变量的帮助文档以及参数原型的意思，LCN会默认始用 preview 窗口来显示帮助文档，个人习惯不喜欢 preview，所以设置了 Never 来禁止 preview 窗口，只让帮助信息显示在下面命令行。

配置中涉及到一个额外的 languageclient.json 文件，内容就是指定下 cache 目录：


```
{
    "initializationOptions": {
            "cacheDirectory": "C:/users/linwei/.cache/LanguageClient"
    }
}
```


根据需要自己修改下路径即可，LCN 还需要安装一个 rust 的可执行程序，名字为 languageclient (.exe) 放在插件目录的 bin 目录下面。非 Windows 可以用插件里的 [install.sh](https://link.zhihu.com/?target=http%3A//install.sh/) 程序自动下载，Windows 可以用 powershell 去运行 install.ps1 进行下载：

`powershell -file install.ps1`

如果没搞定还可以到 LCN 的 [官网](https://link.zhihu.com/?target=https%3A//github.com/autozimu/LanguageClient-neovim) 里面找 release，有编译好的对应平台的版本，下载下来重命名（需要的话）为 languageclient （windows 下追加 .exe）自己放到 bin 目录下也是一样的。

以后 Vim Plug 更新了都需要再运行一下这个脚本，可以配置到 vim-plug 中来自动做这个事情。配置完了 LCN 就可以用了么？别急，咱们还需要编译 cquery。这个相对简单，到其[官网](https://link.zhihu.com/?target=https%3A//github.com/cquery-project/cquery)克隆代码按照[说明](https://link.zhihu.com/?target=https%3A//github.com/cquery-project/cquery/wiki/Getting-started) cmake 一步步的编译出 cquery 的可执行，连同下载的对应版本的 libclang 动态库一起放到 PATH 环境变量的某目录中即可。

安装好 cquery 还需要在你源代码的项目目录放一个名为 .cquery 的文件，内容类似：


```
%clang
%c -std=gnu11
%cpp -std=gnu++14
-pthread
-ID:/dev/mingw/include
```


就是指定一下编译参数，需要自己调整一下，有了这个文件 cquery 才能正确识别你的项目目录在哪里，以及各个文件的编译参数。

最后大功告成，试着打开你的 C/C++ 源代码，用上面的快捷键搜索一下符号定义和引用试试，别忘记打开 quickfix 窗口查看具体结果。

如此你得到了一个目前相对比较靠谱的基于语义的符号索引系统。




**用 LSP 进行符号搜索同 gtags 的比较**

前面的 gtags 也能搜索定义和引用，但是 gtags 基于静态符号分析，无法真的像编译器一样给出绝对准确的定义和引用，比如同一个函数有多个定义，根据宏不同条件编译的结果也不同，gtags 搜索定义会把他们全部搜索出来，而 cquery 却能准确的找到真的定义是哪个。

再比如搜索某对象的方法名，gtags 并不能知道该对象的类型，只能按照符号名搜索出一大堆的有可能不同类的同名方法出来，或者名字相同但是参数不同的一堆方法来，虽然这类情况不多，但搜索出来需要你人工二次过滤一下。cquery 就没有这个问题，基于 clang 的语义分析，真的能够定位到你调用的是什么方法来，特别是大型项目，能帮助你省掉不少时间。

但是，还是那句老话，整套 LSP 安装极度麻烦，支持的语言目前也不算多，除了安装 LCN 以外，你用一个语言就要安装那个语言的 LSP 服务端，还要保持日常更新，而 gtags 只需要配置一次，就能通杀 50 种语言。

所以我是同时使用 gtags 和 LSP 的，而 LSP 我也只使用了 cquery，碰到 C/C++ 项目的话我会用 LCN 搜索符号，碰到其他项目或者远程机器上没有配置 LSP ，我就用 gtags。而 cquery 巨费内存，相对于超大型 C/C++ 项目，我还是会切换回 gtags，比如 linux kernel。

cquery有点好处是你不在项目根目录建立 .cquery 文件，它就不会工作，所以我一般默认情况下先使用 gtags，碰到要花时间的项目我再建立这个文件把 cquery 运行起来。

所以没有说一定要用哪个代替哪个的关系，你可以把 gtags 作为一个保底方案先配置起来，一口气为 50 种语言提供符号索引，然后再针对日常用的多的开发语言配置一两个 LSP 服务端，然后两个看心情换着用。




**结 论**

LSP当然是未来的趋势，但说了那么多它的好话，也要说说目前 LSP 存在的问题，稳定性不够，标准制定有些地方不够清晰，导致各个 LSP server 按照自己的想法发挥，而编辑器中的 LSP client 在碰到这些情况会比较难处理。

目前 LCN 搭配各类 LSP 最好用的地方就是用来查找符号和 hover 帮助，补全的话还有很多问题，比如 [这里](https://link.zhihu.com/?target=https%3A//www.v2ex.com/t/451128) 的讨论。而且补全问题不关是 LCN 本身的bug，还有 LSP 协议有些地方含糊不清，然后服务端各行其事的原因，目前 LSP 协议又出了 2.0 版本，马上还要发布 2.1，不同的服务端和客户端都处在一个新老交替的过程中，总之就是还没稳定下来，这些问题短期内难以得到解决。

最后是 cquery 的实现问题，我上面反复都说，我只用 cquery 搜索符号，从来不用它做补全，cquery 的补全系统 bug 不少，YCM 补全在健壮性和稳定性上更胜一筹，详见 YCM 维护者的描述：

> As a YouCompleteMe developer, I'm probably biased, however I have taken a look at clangd and cquery as LSP servers. The point is that YCM team would like to use something with indexing support, but so far either of the C++ servers would introduce big regressions compared to what YCM can currently do.
Clangd has a smaller feature set than YCM actually (no GoToInclude, GetType and GetDoc).
Cquery has a great feature set, but its include completion has a lot of bugs and assumes some things that would break some of YCM users' use cases. Another thing YCM does better is compiler flags sanitation (I'm working on getting cquery on par with YCM there).
YCM is more stable and robust than cquery.

YCM曾经因为自身不能搜索引用，想把 c/c++ 模块换成 cquery，但是调查了一下发现 cquery 在补全方面还无法为用户提供稳定的服务，不能因为引入新功能而让老功能倒退。

NeoVim 最新的 PR 里加入了 LSP 的支持，我没编译过 NeoVim，都是下载稳定版，所以我也没有细看，等正式发布了可以再研究一下，希望能解决一些上面碰到的问题。

所以目前 C/C++ 的补全最佳方案任然是 YCM，符号搜索最佳方案是 gtags + cquery 搭配使用，LSP 目前来说在符号检索这件事情上，让 IDE 对于编辑器而言不再具有优势。对 LSP 的未来看好，对其现状表示谨慎的乐观。




--

后记：那么如果远程服务器上没环境，既没有 LSP 也没有 gtags 偶尔看代码需要搜索点符号有什么办法呢？当然有啊，grep，作为最老牌的文本搜索工具，一直承担着符号搜索的重任，当你没环境时，当你碰到一门语言 ctags/gtags/lsp 都支持不好的时候（比如某新语言尝鲜），你至少还有个 grep 兜底，后面有空可以继续介绍，更多内容见专栏：[Vim](https://zhuanlan.zhihu.com/vimrc) 。






