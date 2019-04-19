# VS Code（1.2.0）最新亮点和特性全介绍 - 文章 - 伯乐在线
原文出处： [code.visualstudio.com](https://code.visualstudio.com/Updates#1.2)   译文出处：[赖信涛](http://geek.csdn.net/news/detail/80215)
五月发布的VS Code加入了一些很棒的新特性，改进了许多现有功能并且修复了重要的Bug。
下面是这个版本的亮点：
- [**编辑器**](#_editor)：自动去除插入的空格，可以通过快捷键选择所有匹配项。预览窗口（Peek View）现在可以调整大小了。
- [**检查器**](#_linters)：现在支持修复所有检测到的问题，TSLint支持快速修复（Quick Fix）并且可以设置需要排除的文件。
- [**语言**](#_languages)：代码补全（IntelliSense），可以跳转或预览定义。查找调用功能已支持在CSS、SCSS、LESS文件中查找CSS变量。
- [**工作台**](#_workbench)：新集成的终端支持在VS Code里调用系统shell。支持在终端中取消快捷键绑定，在标签中增加了*progress*支持，可以通过[内部版](/insiders)使用。
- [**调试**](#_debugging)：现在，你可以控制Debug Console出现的方式。我们还改进了单单步调试的性能，带有大量本地变量的Node.js的程序调试性能得到大幅改善。
- [**扩展**](#_extension-authoring)：现在，你可以从命令行查看扩展列表、安装和卸载扩展。对扩展的开发者，我们对调试、TextMate语法和命令API进行了扩展，并且改进了语言服务器协议。
更多详细信息请继续往下阅读。
## 编辑器
### 自动去除空格
新起一行的时候，编辑器会根据缩进规则和上一行的内容自动插入空格。这样会在文件中留下多余的空格。感谢[PR #5423](https://github.com/Microsoft/vscode/pull/5423)的工作，新的`editor.trimAutoWhitespace`设置可以让编辑器跟踪自动加入的空格并根据情况进行移除，比如在某个地方输入时会自动移除。请注意，该设置默认开启。
![Trim Auto Whitespace](http://jbcdn2.b0.upaiyun.com/2016/06/7435366536f91d53803d09a85f0ea8db1.gif)
### 查找所有发现的匹配项
你可以通过快捷键快速选中所有找到的选项，OS X的快捷键是`⌥Enter`，Windows和Linux的快捷键是`Alt+Enter`。比起先前的查找和替换效率更高，为VS Code的多光标（multi-cursor）操作提升了一大块。感谢[PR #5715](https://github.com/Microsoft/vscode/pull/5715)的工作！
![Find All Matches](http://jbcdn2.b0.upaiyun.com/2016/06/edefa1bfb7b21a746e36946247de16891.gif)
### 配置基于单词的提示
当语言服务无法给出语义补全（提示）时，VS Code默认会给出基于单词的提示。这就是为什么在VS Code中编写某些语言代码时，IntelliSense会列出文件中所有单词。一些用户希望不要看到这个提示列表，这时你可以通过`editor.wordBasedSuggestions`关闭设置。
### 调整预览窗口大小
预览窗口编辑器用来展示相关引用的搜索结果，提供声明预览功能。现在支持对预览窗口调整大小了！
![Peek](http://jbcdn2.b0.upaiyun.com/2016/06/eaf0b0caabf23628ee55212f5633d015.gif)
## 工作台
### 选项卡
> 
注意：选项卡功能没有包含在1.2.0（2016年5月）的稳定版中。可以在VS Code[内部版](/insiders)使用。
[选项卡支持](https://github.com/Microsoft/vscode/issues/224)是来自[用户反馈](https://visualstudio.uservoice.com/forums/293070-visual-studio-code/suggestions/7752519-implement-tabs)中的一个功能。尽管完全实现该功能还需要多次迭代，但是我们还是想在五月的版本中展示一些开发进展。
在这个里程碑中，我们关注已打开的编辑窗口的 *堆栈* 管理。在下面的图片中，可以看到左边的面板打开了2个编辑器，中间的面板打开了4个编辑器，右边的面板也打开了超过2个编辑器。通过斜体列出的文件都是 *预览* 文件，你可以单击浏览文件，不用像以前那样在打开的编辑器列表中查找需要的文件用Tab选择。
![Tabs](http://jbcdn2.b0.upaiyun.com/2016/06/dca2e81ab4aac9820514f978c8d5ba85.png)
当你关闭编辑器时会恢复最近一次打开的编辑器堆栈，而不是像现在这样直接关闭面板。除此以外，我们还进行了许多用户体验研究。将研究得出的结果应用在设计中，这些改进包括：
- 重新设计overflow图标
- 能够指定 **快速打开（Quick Open）** 文件是固定（pinned）还是预览
- 增加了一个命令用来把预览状态的文件转为固定状态
这些工作已经加入到master分支，可以在[内部版](/insiders)使用。
### 终端集成
排在用户反馈[第2名](https://visualstudio.uservoice.com/forums/293070-visual-studio-code/suggestions/7752357-integrated-terminal)的功能是终端集成，[issue #143](https://github.com/Microsoft/vscode/issues/143)。在集成环境中直接嵌入终端会带来很大的方便，终端初始的路径是正在工作的目录。需要进行终端操作时，无需切换窗口或改变终端的状态，可以迅速上手。
在五月发布的版本中，我们发布了终端集成功能的初始版本。你可以通过快捷键打开终端，OS X的快捷键是 ⌃`，Windows和Linux的快捷键是 Ctrl+`。也可以通过 **View** | **Toggle Integrated Terminal** 菜单或者 **命令面板（Command Palette）** 中的 **View** > **Toggle Integrated Terminal** 打开。
默认打开的shell根据系统设置的环境变量决定，Linux和OS X根据 `$SHELL` 变量，Windows下采用的是`%COMSPEC%`变量。这些变量的内容可以通过设置选项中的`terminal.integrated.shell.*`设定覆盖。打开的集成终端和你常用的终端一样使用。
![Terminal](http://jbcdn2.b0.upaiyun.com/2016/06/cc49ada7bab8264f1402f27e8e927a85.png)
请注意，这是一个早期版本。目前VS Code只支持单个终端，并且还有一些与键盘相关的功能尚未支持，比如拷贝粘贴等。Windows 10用户可能在调用 `cmd.exe` 时遇到问题。如果发现这样的问题，请参照[#143](https://github.com/Microsoft/vscode/issues/143#issuecomment-221054202)的描述启用旧版本的 `cmd.exe`。完整的问题列表和功能需求可以在[这里](https://github.com/Microsoft/vscode/issues?q=is%3Aopen+is%3Aissue+label%3Aintegrated-terminal)找到。
> 
**注意：****启动集成终端** 快捷键（OS X是 ^`，Windows和Linux是 Ctrl+`）之前已经被 **切换已打开的编辑器** 占用。你可以根据自己的需要进行[自定义](https://code.visualstudio.com/docs/customization/keybindings#_customizing-shortcuts)。
### 定义快捷键取消规则
现在，你可以定义快捷键规则取消默认快捷键。通过 `keybindings.json` 可以方便地重新定义所有VS Code快捷键，但是要做一些特殊定制可能会比较困难，比如重载快捷键Tab或Esc。从这个版本开始，只要在`command`前面加上`-`就可以方便地定义一条快捷键取消规则。
下面是一个示例：

C#
```
// In Default Keyboard Shortcuts
...
{ "key": "tab", "command": "tab", "when": ... },
{ "key": "tab", "command": "editor.emmet.action.expandAbbreviation", "when": ... },
{ "key": "tab", "command": "jumpToNextSnippetPlaceholder", "when": ... },
{ "key": "tab", "command": "acceptQuickFixSuggestion", "when": ... },
{ "key": "tab", "command": "acceptSelectedSuggestion", "when": ... },
...
// To remove the second rule, for example, add in keybindings.json:
{ "key": "tab", "command": "-editor.emmet.action.expandAbbreviation" }
```
## 语言
### CSS变量
[在CSS文件中支持变量](https://developer.mozilla.org/en-US/docs/Web/CSS/Using_CSS_variables)是一个新功能提案，目前处在[草案](https://drafts.csswg.org/css-variables/)状态。VS Code目前对CSS变量提供富代码提示（IntelliSense）以及转到定义、Peek定义和在同一个文件中查找所有引用功能。
例如，这里我们可以预览`--bg-color`的定义，OS X快捷键`⌥F12`，Windows快捷键`Alt+F12`，Linux快捷键`Ctrl+Shift+F10`。
![css peek definition](http://jbcdn2.b0.upaiyun.com/2016/06/0532b60cb79e76aa397a28fedc448607.png)
## 检查器（Linter）
### ESLint
[ESLint扩展](https://marketplace.visualstudio.com/items?itemName=dbaeumer.vscode-eslint)现在支持“修复所有问题”命令。可以通过 `eslint.fixAllProblems` 设置功能快捷键，比如像下面这样：

C#
```
[
    { "key": "ctrl+shift+alt+f",   "command": "eslint.fixAllProblems",
                                     "when": "editorTextFocus" }
]
```
### TSLint
[TSLint扩展](https://marketplace.visualstudio.com/items?itemName=eg2.tslint)现在支持从待执行lint操作的文件中排除指定文件（[PR #47](https://github.com/Microsoft/vscode-tslint/pull/47)），并且还支持为TSLint告警提供快速更正（[PR #40](https://github.com/Microsoft/vscode-tslint/pull/40)）功能。感谢所有提交pull请求的开发者！
## Debugging
## 调试
### 控制打开调试控制台
为了在VS Code中提供更好的内部调试控制台，我们在 `launch.json` 中引入了 `internalConsoleOptions` 设置（参见[#6159](https://github.com/Microsoft/vscode/issues/6159)）。如果你想在调试过程中使用外部终端，同时在VS Code中关注调试的代码，这个功能会对你有很大帮助。提供的功能有：
- `neverOpen`：VS Code *永远不会* 打开调试控制台。
- `openOnFirstSessionStart`：VS Code在 *第一次* 调试会话时打开调试控制台。
- `openOnSessionStart`：VS Code在 *每一次* 调试会话时打开调试控制台。
### 改进Node.js单步调试性能
我们对VS Code的定义是保持轻量级与快速，因此需要对作用域内有大量本地变量的代码单步调试性能进行改进。针对Node.js调试我们做出了下列调整：
- 如果一个作用域内包含了 **超过100个本地变量**， VS Code会关闭针对该作用域的自动展开功能。
- 在手动展开作用域时，只会展示 **前100** 个本地变量。
作用域的提示只会显示前100个本地变量：
![100 Local Variables](http://jbcdn2.b0.upaiyun.com/2016/06/e3f2f0e8ec6d4f940c8ff598ba08bfab.png)
要了解不在前100出现的变量，只要在Watch表达式中加入该变量，并在调试控制台查看它的值。
## 编写扩展
### 调试扩展API
我们为VS Code调试器组件开放了一个 `vscode.startDebug` 命令作为扩展API。通过这个命令，既可以传入启动配置文件名也可以传入配置对象，编程启动调试会话：

C#
```
let launchConfig = {
        type: "node",
        request: "launch",
        program: "${workspaceRoot}/test.js",
        cwd: "${workspaceRoot}"
    };
    vscode.commands.executeCommand('vscode.startDebug', launchConfig).then(() => {
        vscode.window.showInformationMessage('Debug session started successfully');
    }, err => {
        vscode.window.showInformationMessage('Error: ' + err.message);
    });
```
更多细节可以在[这里](https://github.com/microsoft/vscode/issues/4615)找到。
### TextMate语法注入
现在可以通过扩展加入TextMate语法，将新的规则注入到已有语法实现高亮。这样就可以在string值或注释中添加颜色，比如对URL或TODO标记实现跨语言高亮。

C#
```
"grammars": [
  {
    "scopeName": "source.todo",
    "path": "./syntaxes/todo.tmLanguage",
    "injectTo": [  "source.js", "source.ts" ]
  }
]
```
### 文件比较
我们添加了新的API命令，可以通过diff编辑器对任意两个资源进行比较，比较命令示例：`commands.executeCommand('vscode.diff', uri1, uri2)`。
### 扩展示例更新
新的富扩展让你对 *虚拟文档*、*事件*，以及 *将语言功能作为命令* 有了丰富的示例。同时html预览的例子也进行了更新：
- [contentprovider-sample](https://github.com/Microsoft/vscode-extension-samples/tree/master/contentprovider-sample)
- [previewhtml-sample](https://github.com/Microsoft/vscode-extension-samples/tree/master/previewhtml-sample)
### 调试转换器（Debug Adapter）：支持“单个”或“全部”线程控制操作
在之前的VS Code调试协议中，我们为 `StoppedEvent` 增加了 `allThreadsStopped`属性。通过它，调试转换器能够向前端报告是一个线程还是所有线程已经停止。感谢开发者的反馈，让我们知道 `ContinueRequest` 也需要这个属性。
在1.9.0版本的[VS Code调试协议](https://github.com/Microsoft/vscode-debugadapter-node)，我们为`ContinueRequest`增加了`allThreadsContinued`属性。通过它，调试转换器能够向UI报告是一个线程还是所有线程在继续执行。更多细节可以在[这里](https://github.com/Microsoft/vscode/issues/6400)找到。
### 语言服务器协议
[语言服务器协议](https://github.com/Microsoft/vscode-languageserver-protocol)现在为遥测通知（telemetry notification）提供支持。在[节点客户端开发库](https://www.npmjs.com/package/vscode-languageclient)中已经实现了消息追踪功能，可以用来追踪服务器中的问题。通过 `${clientName}.trace.server` 设置启用。其中clientName是创建`LanguageClient`实例的名字，例如`eslint.trace.server`表示这是针对ESLint的linter服务。
### 命令行扩展管理
为了让自动化配置VS Code变得更加简单，现在你可以从命令行查看扩展列表、安装和卸载扩展。
示例：

C#
```
code --list-extensions
code --install-extension ms-vscode.cpptools
code --uninstall-extension ms-vscode.csharp
```
## Monaco编辑器
VS Code的核心是“Monaco”代码编辑器。你可以在在许多微软的产品中找到“Monaco”编辑器，比如OneDrive、VSTS、Azure、TypeScript playground，甚至在IE和Edge的F12工具中都可以发现它的身影。
我们的目标是让每个人都可以使用“Monaco”编辑器并且可以在任何浏览器中的使用。今天，我们需要面临的挑战是`vscode`仓库中嵌入的编辑器以及语言服务扩展依赖（非浏览器）客户端使用的技术。
要实现编辑器发布，第一步是[探索](https://github.com/alexandrudima/monaco-typescript)，将TypeScript语言服务从VS Code的源代码中提取出来作为独立的编辑器API使用。
接下来的几周，我们会继续重构API并且将打包自动化，目标是让“Monaco”编辑器可以在六月的VS Code版本中发布。
## 值得关注的Bug修复
- [959](https://github.com/Microsoft/vscode/issues/959)：在缩放和滚动时字体变得fuzzy
- [1000](https://github.com/Microsoft/vscode/issues/1000)：离线状态时响应很慢
- [2717](https://github.com/Microsoft/vscode/issues/2717)：设置“Define Keybinding”后keybindings.json失效
- [4541](https://github.com/Microsoft/vscode/issues/4541)：韩国语输入有问题
- [5645](https://github.com/Microsoft/vscode/issues/5645)：TS仓库对象调用响应缓慢
- [5780](https://github.com/Microsoft/vscode/issues/5780)：Linux下外部终端默认值可以改进
- [6029](https://github.com/Microsoft/vscode/issues/6029)：node v6无法调试
- [6151](https://github.com/Microsoft/vscode/issues/6151)：滚动条很丑，高dpi环境下显示不清晰
- [6432](https://github.com/Microsoft/vscode/issues/6432)：安装完成后，Unity桌面没有马上安装
- [6525](https://github.com/Microsoft/vscode/issues/6525)：文件名带空格的文件在Linux CLI下打开失败
- [6530](https://github.com/Microsoft/vscode/issues/6530)：磁盘驱动器字符大小写不匹配时，源代码地图不显示
- [6593](https://github.com/Microsoft/vscode/issues/6593)：在Unbuntu下有时候会出现两个桌面
- [6609](https://github.com/Microsoft/vscode/issues/6609)：需要修改”fold all“与”unfold all“快捷键
- [6878](https://github.com/Microsoft/vscode/issues/6878), [6916](https://github.com/Microsoft/vscode/issues/6916): 处理压缩的（minified）js文件会卡顿或崩溃
1.2.0版本更新[已关闭的Bug](https://github.com/Microsoft/vscode/issues?q=is%3Aissue+label%3Abug+milestone%3A%22May+2016%22+is%3Aclosed)和[已完成的功能](https://github.com/Microsoft/vscode/issues?q=is%3Aissue+milestone%3A%22May+2016%22+is%3Aclosed+label%3Afeature-request)。
## 下载
下载：[Windows](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/VSCodeSetup-stable.exe) | [OS X](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/VSCode-darwin-stable.zip) | Linux 64-bit [.zip](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/VSCode-linux-x64-stable.zip)[.deb](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/vscode-amd64.deb)[.rpm](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/vscode-x86_64.rpm) | Linux 32-bit [.zip](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/VSCode-linux-ia32-stable.zip)[.deb](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/vscode-i386.deb)[.rpm](https://az764295.vo.msecnd.net/stable/809e7b30e928e0c430141b3e6abf1f63aaf55589/vscode-i386.rpm)
